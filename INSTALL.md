Runtime Dependency
==================

- [PCRE2][] (>= 10.00, since version 0.12.0)

Installing From a Binary Package
================================

Windows binary packages can be downloaded the [Release page][].

Windows users can also install EditorConfig core by [Chocolatey](http://chocolatey.org/packages/editorconfig.core).

Debian (Jessie and later): `apt-get install editorconfig`

ArchLinux: `pacman -S editorconfig-core-c`

Fedora: `dnf install editorconfig`

Mac OS X users can `brew install editorconfig` with [Homebrew](http://brew.sh).
Generally Linux users can also install with [LinuxBrew](https://github.com/Homebrew/linuxbrew)
by `brew install editorconfig`.

Manually Install from Source
============================

**(To build on Windows: Since it's usually a lot harder to manually build on Windows, see the
section "Automated Build on Windows" for an easier way.)**

Dependencies
------------

In principle, you should be able to build the source using any C compiler that
is C99 compliant (i.e., virtually any C compiler that is not archaic). Before
installing, you need to install the building tool [cmake][] (version >= 3.16.3)
and dependency [PCRE2][]. To install cmake and pcre with package manager:

- Arch Linux: `pacman -S cmake pcre2`
- Homebrew on OS X: `brew install cmake pcre2`
- Ubuntu/Debian: `apt-get install cmake libpcre2-dev`

Make sure cmake is in your PATH environment variable. on Windows, you also need
to build PCRE2 from source. If you want to build the MT version of this library,
after running `cmake .` in the PCRE2 source directory, please replace all
occurrence of "MD" with "MT" in the CMakeCache.txt.

Obtaining Source
----------------

The easiest way to obtain EditorConfig C core source is using git:

    git clone --recursive https://github.com/editorconfig/editorconfig-core-c.git

Alternatively, you can download the source tarball/zipfile from the [Release
page][] and unarchive it.

Start Building
--------------

Switch to the root directory of EditorConfig C core source directory and execute
the following command:

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

Automated Build on Windows
==========================

Requirements on Windows are [Visual Studio] 2015, 2017, 2019 or 2022, [cmake] 3.16.3 or higher and Powershell 3 or higher. For Visual Studio the community edition is sufficient, but the [C++ workload](https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=vs-2017) is required.

    Non-static build is currently not supported.

Download pcre2
--------------

You have to download and extract the pcre2 sources after checkout.

```powershell
~> ./init.ps1 [-pcre "10.46"]
```

Arguments:

    -pcre  Optional; pcre2 version to download.

Build Library
-------------

To build pcre2 and editorconfig core library in one step use the `-init` and `-install` arguments at the same time.

```powershell
~> ./build.ps1 -init -install
```

The `-init` argument will generate the required cmake build files for Visual Studio. This is required after initial checkout or `CMakeLists.txt` changes.
The `-install` argument will put the binaries to a location (`bin/$(ARCH)-static/build`) that the editorconfig project can find and link the library. This folder can be used to distribute the build binaries.
For the other arguments please see below.

```powershell
~> ./build.ps1 [-proj all | core | pcre2] [-init] [-install] [-vsver 17 | 16 | 15 | 14 ] [-arch x64 | x86 | arm64] [-config Release | Debug]
```

Arguments:

    -proj Optional; Project to build.
    -init Optional; (Re)Generate cmake build files, required first time or on `CMakeLists.txt` changes.
    -install Optional; Install to `bin/$(ARCH)-static/build` folder.
    -vsver Optional; Visual Studio version (major version number) to use.
    -arch Optional; Architecture to build for.
    -config Optional; Debug or release build.

### Build pcre2 Library

```powershell
~> ./build.ps1 -proj pcre2 -init -install
```

### Build EditorConfig Core Library

```powershell
~> ./build.ps1 -proj core -init
```


### Run Tests
On Windows the test `utf_8_char` is disabled.

```powershell
~> ./test.ps1 [-arch x64 | x86 | arm64] [-config Release | Debug]
```

Arguments:

    -arch Optional; Architecture to build for.
    -config Optional; Debug or release build.
 

[cmake]: https://cmake.org
[PCRE2]: https://pcre.org/
[Visual Studio]: https://visualstudio.microsoft.com
[Release page]: https://github.com/editorconfig/editorconfig-core-c/releases
