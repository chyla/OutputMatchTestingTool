# Output Match Testing Tool

Testing tool for checking programs console output.

## Overview

The tool allows you to check the program correctness. It will run
the SUT (System Under Test) program and match the standard output
and exit code to assumptions given in test file.

## Installing

### Zero Install

It is recommended to use [Zero Install - A decentralised cross-platform software
installation system](https://0install.net/). Zero Install will download the OMTT
in a secure way and will care about updates.

After installing Zero Install, type in the shell:

```bash
0install add omtt https://apps.chyla.org/omtt.xml
```

May be needed to modify your `PATH` variable, Zero Install will notify you.

### Manual

Binaries packages can be downloaded from the
[GitHub projects releases page](https://github.com/chyla/OutputMatchTestingTool/releases).

Download the binaries and place them in your PATH.

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

### Full output match

Here is the first test file example for the `cat` program:

```text
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

```text
omtt --sut /bin/cat cat-will_print_input_and_exit_with_zero.omtt
```

The tool will print the summary containing the most basic information
and the test verdict:

```text
Testing: /bin/cat
====================
Running test (1/1): cat-will_print_input_and_exit_with_zero.omtt
Verdict: PASS
====================
1 tests total, 1 passed, 0 failed
```

You will see more info when the test fail. See this test example
for the `cat` program:

```text
RUN
WITH INPUT
Some text.
EXPECT EXIT CODE 1
EXPECT OUTPUT
Some other text.
```

Here are the results:

```text
Testing: /bin/cat
====================
Running test (1/1): examples/cat-failing_scenario-will_exit_with_non_zero_exit_status_and_some_output_when_some_other_input_is_given.omtt
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
Expected (context):
S    o    m    e    SPC  o    t    h    e    r    SPC  t
                         ^
0x53 0x6f 0x6d 0x65 0x20 0x6f 0x74 0x68 0x65 0x72 0x20 0x74
Got (context):
S    o    m    e    SPC  t    e    x    t    .
                         ^
0x53 0x6f 0x6d 0x65 0x20 0x74 0x65 0x78 0x74 0x2e
====================
1 tests total, 0 passed, 1 failed
```

### Partial output match

It's possible to search for only part of output, it won't give you detailed description about the mismatch like in full output match. Here's example:

```text
RUN
WITH INPUT
Hello world.
EXPECT IN OUTPUT
Hello
EXPECT IN OUTPUT
WORLD
EXPECT EXIT CODE 0
```

The results:

```text
Testing: /bin/cat
====================
Running test (1/1): examples/cat-failing_scenario-mismatch_while_matching_part_of_output.omtt
Verdict: FAIL
--------------------
=> Cause:
Text not found in output.
Expected (context):
W    O    R    L    D
0x57 0x4f 0x52 0x4c 0x44
====================
1 tests total, 0 passed, 1 failed
```

### Comments

Comments begins with `/*` and ends with `*/`, are allowed only on top
of the test (see the `cat-will_print_input_and_exit_with_zero.omtt` example):

```text
/*
 * This test will check cat prints given input and exits with zero.
 */

RUN
WITH INPUT
Hello world!
EXPECT OUTPUT
Hello world!
EXPECT EXIT CODE 0
```

### Messages from SUT standard error

Messages printed by SUT on standard error are shown after the test inside
the 'SUT error messages printed during test execution' section. The messages
do not affect the test result.

Here is the example result:

```text
Testing: systemtests/apps/scaterr
====================
Running test (1/1): systemtests/omtt/scaterr-error_text_on_input.omtt
Verdict: PASS
--------------------
=> SUT error messages printed during test execution:
example error message
====================
1 tests total, 1 passed, 0 failed
```

### Running multiple tests

Multiple tests can be executed by passing its paths in command line:

```text
omtt --sut /bin/cat examples/cat-will_match_part_of_output.omtt examples/cat-will_print_input_and_exit_with_zero.omtt
```

Tests will be executed in order they appeared in command line:

```text
Testing: /bin/cat
====================
Running test (1/2): examples/cat-will_match_part_of_output.omtt
Verdict: PASS
====================
Running test (2/2): examples/cat-will_print_input_and_exit_with_zero.omtt
Verdict: PASS
====================
2 tests total, 2 passed, 0 failed
```

With a little help of shell you can run tests by name pattern:

```text
omtt --sut /bin/cat examples/cat-will*.omtt
```

Results:

```text
Testing: /bin/cat
====================
Running test (1/4): examples/cat-will-exit-with-zero.omtt
Verdict: PASS
====================
Running test (2/4): examples/cat-will_match_part_of_output.omtt
Verdict: PASS
====================
Running test (3/4): examples/cat-will-output-user-given-input.omtt
Verdict: PASS
====================
Running test (4/4): examples/cat-will_print_input_and_exit_with_zero.omtt
Verdict: PASS
====================
4 tests total, 4 passed, 0 failed
```

### Examples

See more tests examples in the `examples` directory.

## Building the sources

### Prerequisites

To build the software you will need to have:

* the C++ 17 compiler,
* GNU Build System (Autotools) - `autoconf` and `automake` packages in Ubuntu/Debian,
* Boost C++ Libraries 1.65 (Program Options) - `libboost-program-options-dev` package in Ubuntu/Debian,
* Python 3.6 or newer - `python3` package in Ubuntu/Debian (to run system tests),
* Robot Framework - `robotframework` package in Python Package Index (to run system tests).

The only supported platform is Linux. To run the tool on Windows
you can try to use Cygwin, but there is no guarantee it will work.

### Preparing the sources

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

### Building

When the sources are prepared (like these with the release archive),
use the standard commands for Autotools to build
the project - `./configure`, `make`.

#### Cygwin

To build on Cygwin set the `CCFLAGS` and `CXXFLAGS` environment variables
to the `-D_GNU_SOURCE` value, before executing `configure` script:

```bash
export CCFLAGS="$CCFLAGS -D_GNU_SOURCE"
export CXXFLAGS="$CXXFLAGS -D_GNU_SOURCE"
```

### Executing the tests

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
