# Output Match Testing Tool

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.


### Prerequisites

To build the software you will need to have:

* the C++ 17 compiler,
* GNU Build System (Autotools) - autoconf and automake packages in Ubuntu/Debian.


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
