[Editorconfig][]
==============

Editorconfig makes it easy to maintain the correct coding style when switching
between different text editors and between different projects.  The
Editorconfig project maintains a file format and plugins for various text
editors which allow this file format to be read and used by those editors.


The File Format
---------------

Editorconfig files use an INI file format with filename globs as section names.
Editorconfig files should be named `.editorconfig` and should be placed in or
above the directory of the files they will affect.  Here is an example
Editorconfig file that tells text editors to use hard tab indentation for
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

This is the README file for the *Editorconfig core* codebase.  This code
produces a program that accepts a filename as input and will look for
`.editorconfig` files with sections applicable to the given file, outputting
any properties found.

When developing an editor plugin for reading Editorconfig files, the
Editorconfig core code can be used to locate and parse these files.  This means
the file locator, INI parser, and file globbing mechanisms can all be
maintained in one code base, resulting in less code repitition between plugins.


Installation
------------

To install the Editorconfig core from source see the `INSTALL` file.


Getting Help
------------

For help with Editorconfig, please write to our [mailing list][].


[Editorconfig]: http://treyhunner.github.com/editorconfig "Editorconfig Homepage"
[mailing list]: http://groups.google.com/group/editorconfig "Editorconfig mailing list"
[vim]: https://github.com/treyhunner/editorconfig-vim "Editorconfig vim plugin"