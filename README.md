# Output Match Testing Tool

Testing tool for checking programs console output.


## Overview

The tool allows you to check the program correctness. It will run
the SUT (System Under Test) program and match the standard output
and exit code to assumptions given in test file.


## Getting Started

To run the tool you will need to compile it from the sources (see
the *Installing from the sources* section) or get the already
compiled binary. Then you will need to write the test and run
the tool with specified path to the SUT. The tool will run the SUT,
check the expectations and print the results.

The next steps of this introduction will use the Unix `cat` program.
Here is what the manual says about it: *concatenate files and print
on the standard output*. When the input file is not given the standard
input is printed on the standard output, we will use this.

Here is the first test file example for the `cat` program:

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

You will see more info when the test fails. See this test example
for the `cat` program:

```
RUN
WITH INPUT
Some text.
EXPECT EXIT CODE 1
EXPECT OUTPUT
Some other text.
```

Here are the results:

```
Testing: /bin/cat
Running test: examples/cat-failing_scenario-will_exit_with_non_zero_exit_status_and_some_output_when_some_other_input_is_given.omtt
Verdict: FAIL
--------------------
=> Cause:
Exit code doesn't match.
Expected: 1
Got: 0
--------------------
=> Cause:
Output doesn't match.
First difference at byte: 5
Expected context:
S    o    m    e    SPC  o    t    h    e    r    SPC  t
                         ^
0x53 0x6f 0x6d 0x65 0x20 0x6f 0x74 0x68 0x65 0x72 0x20 0x74
Output context:
S    o    m    e    SPC  t    e    x    t    .
                         ^
0x53 0x6f 0x6d 0x65 0x20 0x74 0x65 0x78 0x74 0x2e
```

It's possible to search for only part of output, it won't give you detailed description about the mismatch like in full output match. Here's example:

```
RUN
WITH INPUT
Hello world.
EXPECT IN OUTPUT
Hello
EXPECT IN OUTPUT
world
EXPECT EXIT CODE 0
```

The results:

```
Testing: /bin/cat
Running test: examples/cat-will_match_part_of_output.omtt
Verdict: PASS
```

See more tests examples in the `examples` directory.


## Installing from the sources

### Preparing the sources

#### Prerequisites

To build the software you will need to have:

* the C++ 17 compiler,
* GNU Build System (Autotools) - `autoconf` and `automake` packages in Ubuntu/Debian,
* Boost C++ Libraries 1.65 (Program Options) - `libboost-program-options-dev` package in Ubuntu/Debian,
* Robot Framework - `robotframework` package in Python Package Index (to run system tests).

The only supported platform is Linux. To run the tool on Windows
you can try to use Cygwin, but there is no guarantee it will work.

#### Initialize the dependencies

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


### Building

When the sources are prepared (like these with the release archive),
use the standard commands for Autotools to build
the project - `./configure`, `make`.


### Executing the tests

There are two kind of tests - unit tests and system tests. Unit tests
checks small parts of code, system tests checks the whole program.

To execute the unit tests run the following command:

```
make unittests
```

To execute the system tests  run the following command:

```
make systemtests
```

To execute all tests run:

```
make check
```


### Installing

When all the tests pass, use the `make install` command
to install the project.


## Versioning

The project uses [Semantic Versioning](https://semver.org) 2.0.0. The project version
format is MAJOR.MINOR.PATCH.

The specification says:

1. Major version X (X.y.z | X > 0) - incremented if any backwards incompatible
   changes are introduced to the public API. It MAY also include minor and patch
   level changes. Patch and minor version MUST be reset to 0 when major version
   is incremented.
2. Minor version Y (x.Y.z | x > 0) - incremented if new, backwards compatible
   functionality is introduced to the public API. It MUST be incremented if any public
   API functionality is marked as deprecated. It MAY be incremented if substantial
   new functionality or improvements are introduced within the private code. It MAY
   include patch level changes. Patch version MUST be reset to 0 when minor version
   is incremented.
3. Patch version Z (x.y.Z | x > 0) - incremented if only backwards compatible
   bug fixes are introduced. A bug fix is defined as an internal change that fixes
   incorrect behavior.

Read the [VERSIONING.md](VERSIONING.md) file for details.


## License

This project is licensed under the BSD 3-Clause License - see
the [LICENSE](LICENSE) file for details.


## Authors

* **Adam Chyła** - [chyla.org](https://chyla.org/)
