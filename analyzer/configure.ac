#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_CONFIG_MACRO_DIR([m4])
AC_INIT(fapolicyd-analyzer,0.0.1)
AC_CONFIG_SRCDIR([src/cli/fapolicyd-analyzer.c])
AC_CONFIG_HEADERS([config.h])

AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([foreign subdir-objects])

# Checks for programs.
AM_PROG_AR
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

AX_CXX_COMPILE_STDCXX_11

# Find python executable.
AM_PATH_PYTHON

# Get CPPFLAGS, etc., for Python module compilation.
# http://www.gnu.org/software/autoconf-archive/ax_python_devel.html
AX_PYTHON_DEVEL

# Find swig executable.
# http://www.gnu.org/software/autoconf-archive/ax_pkg_swig.html
AX_PKG_SWIG([], [], [AC_MSG_ERROR([SWIG is required to build.])])

# Get SWIGFLAGS, CPPFLAGS, etc., for Python generation by swig.
# http://www.gnu.org/software/autoconf-archive/ax_swig_python.html
AX_SWIG_PYTHON

# Use libtool to link libraries and modules.
LT_INIT

# Checks for libraries.
AM_PROG_CC_C_O

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h float.h inttypes.h limits.h memory.h stddef.h stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([floor memmove memset strchr strdup strstr])

AX_BOOST_BASE([1.66])
withval=""
AC_ARG_WITH(rpm,
AS_HELP_STRING([--with-rpm],[Use the rpm database as a trust source]),
use_rpm=$withval,use_rpm=yes)

if test x$use_rpm = xyes ; then
    AC_CHECK_LIB(rpm, rpmtsInitIterator, , [AC_MSG_ERROR([librpm not found])], -lrpm)
    AC_CHECK_LIB(rpmio, rpmFreeCrypto, , [AC_MSG_ERROR([librpmio not found])], -lrpmio)
    AC_DEFINE(USE_RPM,1,[Define if you want to use the rpm database as trust source.])
fi
AM_CONDITIONAL(WITH_RPM, test x$use_rpm = xyes)

AC_CONFIG_FILES([Makefile
                 src/test/Makefile
                 src/Makefile
                 src/bindings/Makefile])
AC_OUTPUT
