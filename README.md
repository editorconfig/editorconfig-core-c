[EditorConfig][]
==============

EditorConfig makes it easy to maintain the correct coding style when switching
between different text editors and between different projects.  The
EditorConfig project maintains a file format and plugins for various text
editors which allow this file format to be read and used by those editors.


The File Format
---------------

EditorConfig files use an INI file format with filename globs as section names.
EditorConfig files should be named `.editorconfig` and should be placed in or
above the directory of the files they will affect.  Here is an example
EditorConfig file that tells text editors to use hard tab indentation for
JavaScript files, 2 space indentation for HTML files and 4 space indentation
for Python files:

    [*.js]
    indent_style = tab

    [*.html]
    indent_style = space
    tab_width = 2

    [*.py]
    indent_style = space
    tab_width = 4


Supported editors
-----------------

The only editor currently supported is [vim][].  If you want to help
add support for your favorite editor, see the Contributing section below.


Contributing
------------

This is the README file for the *EditorConfig core* codebase.  This code
produces a program that accepts a filename as input and will look for
`.editorconfig` files with sections applicable to the given file, outputting
any properties found.

When developing an editor plugin for reading EditorConfig files, the
EditorConfig core code can be used to locate and parse these files.  This means
the file locator, INI parser, and file globbing mechanisms can all be
maintained in one code base, resulting in less code repitition between plugins.


Installation
------------

To install the EditorConfig core from source see the `INSTALL` file.


Getting Help
------------

For help with EditorConfig, please write to our [mailing list][].

If you are writing a plugin a language that can import C libraries, you may
want to import and use the EditorConfig library directly.  If you do use the
EditorConfig core as a C library, check the [documentation][] for help.


[EditorConfig]: http://editorconfig.github.com/editorconfig "EditorConfig Homepage"
[mailing list]: http://groups.google.com/group/editorconfig "EditorConfig mailing list"
[vim]: https://github.com/editorconfig/editorconfig-vim "EditorConfig vim plugin"
[documentation]: http://editorconfig.github.com/editorconfig/core-docs/ "EditorConfig Core documentation"
