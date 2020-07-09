AUTOMAKE_OPTIONS = subdir-objects
SUBDIRS = . test
AM_CPPFLAGS = -I${top_srcdir} -I${top_srcdir}/analyzer

lib_LTLIBRARIES = libfapolicyd-analyzer.la
libfapolicyd_analyzer_la_SOURCES = \
	libfapolicyd-analyzer/detail/macros.h \
	libfapolicyd-analyzer/error_codes.h \
	libfapolicyd-analyzer/match.c \
	libfapolicyd-analyzer/match.h \
	libfapolicyd-analyzer/sha256.c \
	libfapolicyd-analyzer/sha256.h \
	libfapolicyd-analyzer/system_trust.cpp \
	libfapolicyd-analyzer/system_trust.hpp \
	libfapolicyd-analyzer/table.cpp \
	libfapolicyd-analyzer/table.hpp

# Ask autotools to install the generated Python file.
python_PYTHON = $(srcdir)/analyzer/bindings/python/fapolicyanalyzer.py

# This is the Python module.
pyexec_LTLIBRARIES = _fapolicyanalyzer.la

# It is composed of the wrapper C file and has other dependencies.
_fapolicyanalyzer_la_SOURCES = \
	bindings/python/fapolicyanalyzer_wrap.cpp \
	libfapolicyd-analyzer/error_codes.h \
	libfapolicyd-analyzer/match.h \
	libfapolicyd-analyzer/sha256.h \
	libfapolicyd-analyzer/system_trust.hpp \
	libfapolicyd-analyzer/table.hpp

# -module tells libtool we don't want a 'lib' prefix.
_fapolicyanalyzer_la_LDFLAGS = -module ${PYTHON_LDFLAGS}

# Preprocessing flags to build the Python module.
_fapolicyanalyzer_la_CPPFLAGS = -I$(srcdir)/analyzer ${PYTHON_CPPFLAGS}

# The Python module needs to link to the libfapolicyd-analyzer library.
_fapolicyanalyzer_la_LIBADD = libfapolicyd-analyzer.la

#sbin_PROGRAMS = fapolicyd-analyzer
#fapolicyd_analyzer_SOURCES = cli/fapolicyd-analyzer.c
#fapolicyd_analyzer_LDADD = libfapolicyd-analyzer.la
