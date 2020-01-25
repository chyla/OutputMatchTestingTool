# Output Match Testing Tool

Testing tool for checking programs console output.


## Overview

The tool allows you to check the program correctness. It will run
the SUT (System Under Test) program and match the standard output
and exit code to assumptions given in test file.


## Getting Started

To run the tool you will need to compile it from the sources (see
the `Building the sources` section) or get the already compiled binary.
Then you will need to write the test and run the tool with specified
path to the SUT. The tool will run the SUT, check the expectations
and print the results.

Here is the example test file for the `cat` program:

```
RUN
WITH INPUT
Hello world!
EXPECT EXIT CODE 0
EXPECT OUTPUT
Hello world!
```

I hope the test file is self-documenting and everything is obvious. Save
the test to `cat-will_print_input_and_exit_with_zero.omtt` file, then run
the tool like that to execute the test with the `cat` binary:

```
omtt --sut /bin/cat cat-will_print_input_and_exit_with_zero.omtt
```

The tool will print the summary containing the most basic informations
and the test verdict:

```
Testing: /bin/cat
Running test: cat-will_print_input_and_exit_with_zero.omtt
Verdict: PASS
```


## Building the sources

### Prerequisites

To build the software you will need to have:

* the C++ 17 compiler,
* GNU Build System (Autotools) - autoconf and automake packages in Ubuntu/Debian,
* Python 3 with working venv module - python3 and python3-venv packages in Ubuntu/Debian.

### Preparing the sources

*This section applies only to the git cloned sources, not the release
archive.*

After cloning the git repository, please init and fetch all git
submodules data with the commands:

```
$ git submodule init
$ git submodule update
```

Please also initialize the Autotools:

```
$ autoreconf --install
```

### Building the project

When the sources are prepared (like these with the release archive),
use the standard commands for Autotools to build and install
the project - `./configure`, `make`, `make install`.


## Authors

* **Adam Chyła** - [chyla.org](https://chyla.org/)


## License

This project is licensed under the BSD 3-Clause License - see
the [LICENSE](LICENSE) file for details.
