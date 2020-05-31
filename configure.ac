#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([omtt], [master], [adam@chyla.org])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 unittests/Makefile
 systemtests/Makefile
 systemtests/apps/Makefile
])

# Change default tar command
am__tar='$${TAR-tar} -c -h --format pax -f - "$$tardir"' am__untar='$${TAR-tar} xf -'

# Checks for programs.
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_17([noext], [mandatory])
AM_PATH_PYTHON([3.6.0])

# Checks for libraries.
AX_BOOST_BASE([1.65], [], [AC_MSG_ERROR([cannot find Boost libraries])])

AX_BOOST_PROGRAM_OPTIONS

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_OUTPUT
