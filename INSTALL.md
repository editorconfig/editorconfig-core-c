Dependency
==========

- [pcre](http://www.pcre.org/) (Since version 0.12.0)

Installing from a binary package
================================

Windows binary packages can be downloaded [here](http://sourceforge.net/projects/editorconfig/files/EditorConfig-C-Core/).

Windows users can also install EditorConfig core by [Chocolatey](http://chocolatey.org/packages/editorconfig.core).

Debian (Jessie and later): `apt-get install editorconfig`

ArchLinux: `pacman -S editorconfig-core-c`

Mac OS X users can `brew install editorconfig` with [Homebrew](http://brew.sh).
Generally Linux users can also install with [LinuxBrew](https://github.com/Homebrew/linuxbrew)
by `brew install editorconfig`.

Installing from source
======================

Before installing, you need to install the building tool [cmake][] and
dependency [pcre][]. To install cmake and pcre with package manager:

- Arch Linux: `pacman -S cmake pcre`
- Homebrew on OS X: `brew install cmake pcre`
- Ubuntu/Debian: `apt-get install cmake libpcre3-dev`

Make sure cmake is in your PATH environment variable. on Windows, you also need
to build pcre from source. If you want to build the MT version of this library,
after running `cmake .` in the pcre source directory, please replace all
occurrence of "MD" with "MT" in the CMakeCache.txt.

Switch to the root directory of editorconfig and execute the following command:

    cmake .

If successful, the project file will be generated. There are various options
could be used when generating the project file:

    -DBUILD_DOCUMENTATION=[ON|OFF]          Default: ON
    If this option is on and doxygen is found, the html documentation and
    man pages will be generated.
    e.g. cmake -DBUILD_DOCUMENTATION=OFF .

    -DBUILD_STATICALLY_LINKED_EXE=[ON|OFF]  Default: OFF
    If this option is on, the executable will be linked statically to all
    libraries. On MSVC, this means that EditorConfig will be statically
    linked to the executable. On GCC, this means all libraries (glibc and 
    EditorConfig) are statically linked to the executable.
    e.g. cmake -DBUILD_STATICALLY_LINKED_EXE=ON .

    -DINSTALL_HTML_DOC=[ON|OFF]             Default: OFF
    If this option is on and BUILD_DOCUMENTATION is on, html documentation
    will be installed when execute "make install" or something similar.
    e.g. cmake -DINSTALL_HTML_DOC=ON .

    -DDOXYGEN_EXECUTABLE=/path/to/doxygen
    If doxygen could not be found automatically and you need to generate
    documentation, try to set this option to the path to doxygen.
    e.g. cmake -DDOXYGEN_EXECUTABLE=/opt/doxygen/bin/doxygen .

    -DMSVC_MD=[ON|OFF]                      Default: OFF
    Use /MD instead of /MT flag when compiling with Microsoft Visual C++. This
    option takes no effect when using compilers other than Microsoft Visual
    C++. Note that the option chosen here must be consistent with pcre.
    e.g. We want to use /MD instead of /MT when compiling with MSVC.
    cmake -DMSVC_MD=ON .

On UNIX/Linux with gcc, the project file is often a Makefile, in which case you
can type "make" to compile editorconfig.  If you are using a different compiler
or platform the compilation command may differ. For example, if you generate an
NMake Makefile and you want to use Microsoft Visual C++ to build editorconfig,
then the build command would be "nmake".

After the compilation succeeds, use the following command to install (require
root or admin privilege):

    make install

This command will copy all the files you need to corresponding directories:
editorconfig libraries will be copied to PREFIX/lib and executables will be
copied to PREFIX/bin.

Note that you have to use ldconfig or change LD_LIBRARY_PATH to specify the
PREFIX/lib as one of the library searching directory on UNIX/Linux to make sure
that source files could be linked to the libraries and executables depending on
these libraries could be executed properly.

On Windows, via Developer Command Prompt for Visual Studio:

    msbuild all_build.vcxproj /p:Configuration=Release

[cmake]: http://www.cmake.org
[pcre]: http://pcre.org
