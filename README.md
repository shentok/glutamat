Glutamat
========

Glutamat is a Clang plugin which warns about definitions of singleton classes.

Singletons are basically global variables that are wrapped inside a class and
can be read from and written to by virtually anybody. They are usually accessed
from a static, parameterless method which returns a non-const pointer or
reference to the wrapped object, often named `getInstance()`. Those properties
(except the name) are checked by the plugin and, when fulfilled, a warning is
raised.

### Todo

- [ ] Warn when encountering *usage* of such methods


How to build
------------

Glutamat was tested on Linux only.

### Prerequisites

1. **C++ compiler** to compile the sources.
2. **cmake** to configure the build process.
3. **make** to run the build. Makefiles generated by `cmake`.

4. Install **LLVM/Clang**. Either you do install from sources or package
   for your distribution, the Clang version shall match with Glutamat version.
   Make sure that `llvm-config` and `clang` executables are in the `PATH`
   environment.

5. Install **Lit**. This is optional, do only if you want to run the tests!
   Lit is the [LLVM Integrated Tester][LIT]. If you installed Clang from
   sources, you shall have it automaticaly. If your package manager does
   not provide it, you can simply install from [PyPI][PyPI] because it is
   written in python.

   [LIT]: http://llvm.org/docs/CommandGuide/lit.html
   [PyPI]: https://pypi.python.org/pypi/lit

### Build Glutamat

It could be the best to build it in a separate build directory.

    cmake $GLUTAMAT_SOURCE_DIR
    make all
    make install  # to install
    make check    # to run tests
    make package  # to create tgz, rpm, deb packages

You can configure the build process with passing arguments to cmake.


How to use
----------

To run the plugin against your sources you need to tune the build
script of your project. Sure you need to replace the compiler to
Clang. To hook the plugin into the Clang driver, you need to pass
extra flags which are Clang specific, therefore those are 'escaped'
like this:

    CC="clang"
    CXX="clang++"
    CXX_FLAGS+=" -Xclang -load -Xclang $GLUTAMAT_LIB_PATH/libglutamat.so"
    CXX_FLAGS+=" -Xclang -add-plugin -Xclang glutamat"


Problem reports
---------------

If you find a bug in this documentation or elsewhere in the program or would
like to propose an improvement, please use the project's [github issue
tracker][ISSUES]. Please describing the bug and where you found it. If you
have a suggestion how to fix it, include that as well. Patches are also
welcome.

  [ISSUES]: https://github.com/shentok/Glutamat/issues
