# Building the sources

## Prerequisites

To build the software you will need to have:

* the C++ 17 compiler,
* GNU Build System (Autotools) - `autoconf` and `automake` packages in Ubuntu/Debian,
* Boost C++ Libraries 1.65 (Program Options) - `libboost-program-options-dev` package in Ubuntu/Debian,
* Python 3.6 or newer - `python3` package in Ubuntu/Debian (to run system tests),
* Robot Framework - `robotframework` package in Python Package Index (to run system tests).

The only supported platform is Linux. To run the tool on Windows
you can try to use Cygwin, but there is no guarantee it will work.

## Preparing the sources

*This section applies only to the git cloned sources, not the release
archive.*

After cloning the git repository, please init and fetch all git
submodules data with the commands:

```bash
git submodule init
git submodule update
```

Please also initialize the Autotools:

```bash
autoreconf --install
```

## Building

When the sources are prepared (like these with the release archive),
use the standard commands for Autotools to build
the project - `./configure`, `make`.

### Cygwin

To build on Cygwin set the `CCFLAGS` and `CXXFLAGS` environment variables
to the `-D_GNU_SOURCE` value, before executing `configure` script:

```bash
export CCFLAGS="$CCFLAGS -D_GNU_SOURCE"
export CXXFLAGS="$CXXFLAGS -D_GNU_SOURCE"
```

## Executing the tests

There are two kind of tests - unit tests and system tests. Unit tests
checks small parts of code, system tests checks the whole program.

To execute the unit tests run the following command:

```bash
make unittests
```

To execute the system tests  run the following command:

```bash
make systemtests
```

To execute all tests run:

```bash
make check
```

## Installing

When all the tests pass, use the `make install` command to install the project.
