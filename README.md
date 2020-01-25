# Output Match Testing Tool

Testing tool for checking programs console output.


## Overview

The tool allows you to check the program correctness. It will run
the SUT (System Under Test) program and match the stdout and exit code
to assumptions given in test file.


## Getting Started

These instructions will get you a copy of the project up and running
on your local machine for development and testing purposes.


### Prerequisites

To build the software you will need to have:

* the C++ 17 compiler,
* GNU Build System (Autotools) - autoconf and automake packages in Ubuntu/Debian,
* Python 3 with working venv module - python3 and python3-venv packages in Ubuntu/Debian.


### Preparing the sources

After cloning the repository, please init and fetch all data for the git submodules with the commands:

```
$ git submodule init
$ git submodule update
```

Please also initialize the Autotools:

```
$ autoreconf --install
```

### Building the project

When the sources are prepared use the standard commands for Autotools to build and install the project - `./configure`, `make`, `make install`.


## Authors

* **Adam Chyła** - [chyla.org](https://chyla.org/)


## License

This project is licensed under the BSD 3-Clause License - see the [LICENSE](LICENSE) file for details.
