/*
 * Copyright (c) 2014-2019 Hong Xu <hong AT topbug DOT net>
 * Copyright (c) 2018 Sven Strickroth <email AT cs-ware DOT de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
*/

#include "global.h"

#include <ctype.h>
#include <string.h>
#include <pcre2.h>

#define utarray_oom() { return -2; }
#include "utarray.h"
#include "misc.h"
#include "util.h"

#include "ec_glob.h"

/* Special characters */
const char ec_special_chars[] = "?[]\\*-{},";

typedef struct int_pair
{
    int     num1;
    int     num2;
} int_pair;
static const UT_icd ut_int_pair_icd = {sizeof(int_pair),NULL,NULL,NULL};

/* concatenate the string then move the pointer to the end */
#define STRING_CAT(p, string, end)  do {    \
    size_t string_len = strlen(string); \
    if (p + string_len >= end) \
        return -1; \
    strcat(p, string); \
    p += string_len; \
} while(0)

/* safely add a char to a string then move the pointer to the end */
#define ADD_CHAR(string, new_chr, end)  do {    \
    if (string + 1 >= end) \
        return -1; \
    *(string ++) = new_chr; \
} while(0)

#define PATTERN_MAX  4097
/*
 * Whether the string matches the given glob pattern. Return 0 if successful, return -1 if a PCRE
 * error or other regex error occurs, and return -2 if an OOM outside PCRE occurs.
 */
EDITORCONFIG_LOCAL
int ec_glob(const char *pattern, const char *string)
{
    size_t                    i;
    int_pair *                p;
    char *                    c;
    char                      pcre_str[2 * PATTERN_MAX] = "^";
    char *                    p_pcre;
    char *                    pcre_str_end;
    int                       brace_level = 0;
    _Bool                     is_in_bracket = 0;
    int                       error_code;
    size_t                    erroffset;
    pcre2_code *              re;
    int                       rc;
    size_t *                  pcre_result;
    pcre2_match_data *        pcre_match_data;
    char                      l_pattern[2 * PATTERN_MAX];
    _Bool                     are_braces_paired = 1;
    UT_array *                nums;     /* number ranges */
    int                       ret = 0;

    strcpy(l_pattern, pattern);
    p_pcre = pcre_str + 1;
    pcre_str_end = pcre_str + 2 * PATTERN_MAX;

    /* Determine whether curly braces are paired */
    {
        int     left_count = 0;
        int     right_count = 0;
        for (c = l_pattern; *c; ++ c)
        {
            if (*c == '\\' && *(c+1) != '\0')
            {
                ++ c;
                continue;
            }

            if (*c == '}')
                ++ right_count;
            else if (*c == '{')
                ++ left_count;

            if (right_count > left_count)
            {
                are_braces_paired = 0;
                break;
            }
        }

        if (right_count != left_count)
            are_braces_paired = 0;
    }

    /* used to search for {num1..num2} case */
    re = pcre2_compile("^\\{[\\+\\-]?\\d+\\.\\.[\\+\\-]?\\d+\\}$", PCRE2_ZERO_TERMINATED, 0,
            &error_code, &erroffset, NULL);
    if (!re)        /* failed to compile */
        return -1;

    utarray_new(nums, &ut_int_pair_icd);

    for (c = l_pattern; *c; ++ c)
    {
        switch (*c)
        {
        case '\\':      /* also skip the next one */
            if (*(c+1) != '\0')
            {
                ADD_CHAR(p_pcre, *(c++), pcre_str_end);
                ADD_CHAR(p_pcre, *c, pcre_str_end);
            }
            else
                STRING_CAT(p_pcre, "\\\\", pcre_str_end);

            break;
        case '?':
            STRING_CAT(p_pcre, "[^/]", pcre_str_end);
            break;
        case '*':
            if (*(c+1) == '*')      /* case of ** */
            {
                STRING_CAT(p_pcre, ".*", pcre_str_end);
                ++ c;
            }
            else                    /* case of * */
                STRING_CAT(p_pcre, "[^\\/]*", pcre_str_end);

            break;
        case '[':
            if (is_in_bracket)     /* inside brackets, we really mean bracket */
            {
                STRING_CAT(p_pcre, "\\[", pcre_str_end);
                break;
            }

            {
                /* check whether we have slash within the bracket */
                _Bool           has_slash = 0;
                char *          cc;
                for (cc = c; *cc && *cc != ']'; ++ cc)
                {
                    if (*cc == '\\' && *(cc+1) != '\0')
                    {
                        ++ cc;
                        continue;
                    }

                    if (*cc == '/')
                    {
                        has_slash = 1;
                        break;
                    }
                }

                /* if we have slash in the brackets, just do it literally */
                if (has_slash)
                {
                    char *           right_bracket = strchr(c, ']');

                    if (!right_bracket)  /* The right bracket may not exist */
                        right_bracket = c + strlen(c);

                    strcat(p_pcre, "\\");
                    strncat(p_pcre, c, right_bracket - c);
                    if (*right_bracket)  /* right_bracket is a bracket */
                        strcat(p_pcre, "\\]");
                    p_pcre += strlen(p_pcre);
                    c = right_bracket;
                    if (!*c)
                        /* end of string, meaning that right_bracket is not a
                         * bracket. Then we go back one character to make the
                         * parsing end normally for the counter in the "for"
                         * loop. */
                        c -= 1;
                    break;
                }
            }

            is_in_bracket = 1;
            if (*(c+1) == '!')     /* case of [!...] */
            {
                STRING_CAT(p_pcre, "[^", pcre_str_end);
                ++ c;
            }
            else
                STRING_CAT(p_pcre, "[", pcre_str_end);

            break;

        case ']':
            is_in_bracket = 0;
            ADD_CHAR(p_pcre, *c, pcre_str_end);
            break;

        case '-':
            if (is_in_bracket)      /* in brackets, - indicates range */
                ADD_CHAR(p_pcre, *c, pcre_str_end);
            else
                STRING_CAT(p_pcre, "\\-", pcre_str_end);

            break;
        case '{':
            if (!are_braces_paired)
            {
                STRING_CAT(p_pcre, "\\{", pcre_str_end);
                break;
            }

            /* Check the case of {single}, where single can be empty */
            {
                char *                   cc;
                _Bool                    is_single = 1;

                for (cc = c + 1; *cc != '\0' && *cc != '}'; ++ cc)
                {
                    if (*cc == '\\' && *(cc+1) != '\0')
                    {
                        ++ cc;
                        continue;
                    }

                    if (*cc == ',')
                    {
                        is_single = 0;
                        break;
                    }
                }

                if (*cc == '\0')
                    is_single = 0;

                if (is_single)      /* escape the { and the corresponding } */
                {
                    const char *        double_dots;
                    int_pair            pair;

                    pcre2_match_data *  match_data = pcre2_match_data_create_from_pattern(re, NULL);

                    /* Check the case of {num1..num2} */
                    rc = pcre2_match(re, c, cc - c + 1, 0, 0, match_data, NULL);

                    pcre2_match_data_free(match_data);

                    if (rc < 0)    /* not {num1..num2} case */
                    {
                        STRING_CAT(p_pcre, "\\{", pcre_str_end);

                        memmove(cc+1, cc, strlen(cc) + 1);
                        *cc = '\\';

                        break;
                    }

                    /* Get the range */
                    double_dots = strstr(c, "..");
                    pair.num1 = ec_atoi(c + 1);
                    pair.num2 = ec_atoi(double_dots + 2);

                    utarray_push_back(nums, &pair);

                    STRING_CAT(p_pcre, "([\\+\\-]?\\d+)", pcre_str_end);
                    c = cc;

                    break;
                }
            }

            ++ brace_level;
            STRING_CAT(p_pcre, "(?:", pcre_str_end);
            break;

        case '}':
            if (!are_braces_paired)
            {
                STRING_CAT(p_pcre, "\\}", pcre_str_end);
                break;
            }

            -- brace_level;
            STRING_CAT(p_pcre, ")", pcre_str_end);
            break;

        case ',':
            if (brace_level > 0)  /* , inside {...} */
                STRING_CAT(p_pcre, "|", pcre_str_end);
            else
                STRING_CAT(p_pcre, "\\,", pcre_str_end);
            break;

        case '/':
            // /**/ case, match both single / and /anything/
            if (!strncmp(c, "/**/", 4))
            {
                STRING_CAT(p_pcre, "(\\/|\\/.*\\/)", pcre_str_end);
                c += 3;
            }
            else
                STRING_CAT(p_pcre, "\\/", pcre_str_end);

            break;

        default:
            if (!isalnum(*c))
                STRING_CAT(p_pcre, "\\", pcre_str_end);

            ADD_CHAR(p_pcre, *c, pcre_str_end);
        }
    }

    *(p_pcre ++) = '$';

    pcre2_code_free(re); /* ^\\d+\\.\\.\\d+$ */

    re = pcre2_compile(pcre_str, PCRE2_ZERO_TERMINATED, 0, &error_code, &erroffset, NULL);

    if (!re)        /* failed to compile */
    {
        utarray_free(nums);
        return -1;
    }

    pcre_match_data = pcre2_match_data_create_from_pattern(re, NULL);
    rc = pcre2_match(re, string, strlen(string), 0, 0, pcre_match_data, NULL);

    if (rc < 0)     /* failed to match */
    {
        if (rc == PCRE2_ERROR_NOMATCH)
            ret = EC_GLOB_NOMATCH;
        else
            ret = rc;

        goto cleanup;
    }

    /* Whether the numbers are in the desired range? */
    pcre_result = pcre2_get_ovector_pointer(pcre_match_data);
    for(p = (int_pair *) utarray_front(nums), i = 1; p;
            ++ i, p = (int_pair *) utarray_next(nums, p))
    {
        const char * substring_start = string + pcre_result[2 * i];
        size_t  substring_length = pcre_result[2 * i + 1] - pcre_result[2 * i];
        char *       num_string;
        int          num;

        /* we don't consider 0digits such as 010 as matched */
        if (*substring_start == '0')
            break;

        num_string = strndup(substring_start, substring_length);
        if (num_string == NULL) {
          ret = -2;
          goto cleanup;
        }
        num = ec_atoi(num_string);
        free(num_string);

        if (num < p->num1 || num > p->num2) /* not matched */
            break;
    }

    if (p != NULL)      /* numbers not matched */
        ret = EC_GLOB_NOMATCH;

 cleanup:

    pcre2_code_free(re);
    pcre2_match_data_free(pcre_match_data);
    utarray_free(nums);

    return ret;
}
