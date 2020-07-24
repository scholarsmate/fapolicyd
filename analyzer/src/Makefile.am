AUTOMAKE_OPTIONS = subdir-objects
SUBDIRS = . test
AM_CPPFLAGS = -I${top_srcdir} -I${top_srcdir}/analyzer

lib_LTLIBRARIES = libfapolicyd-analyzer.la
libfapolicyd_analyzer_la_SOURCES = \
	libfapolicyd-analyzer/detail/macros.h \
	libfapolicyd-analyzer/error_codes.h \
	libfapolicyd-analyzer/match.c \
	libfapolicyd-analyzer/match.h \
	libfapolicyd-analyzer/rpmdb_read.c \
	libfapolicyd-analyzer/rpmdb_read.h \
	libfapolicyd-analyzer/rpmdb_table.cpp \
	libfapolicyd-analyzer/rpmdb_table.hpp \
	libfapolicyd-analyzer/sha256.c \
	libfapolicyd-analyzer/sha256.h \
	libfapolicyd-analyzer/system_trust.cpp \
	libfapolicyd-analyzer/system_trust.hpp \
	libfapolicyd-analyzer/table.cpp \
	libfapolicyd-analyzer/table.hpp

# Ask autotools to install the generated Python file.
python_PYTHON = $(srcdir)/analyzer/src/bindings/python/fapolicyanalyzer.py

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
_fapolicyanalyzer_la_CPPFLAGS = ${PYTHON_CPPFLAGS}

# The Python module needs to link to the libfapolicyd-analyzer library.
_fapolicyanalyzer_la_LIBADD = libfapolicyd-analyzer.la

sbin_PROGRAMS = fapolicyd-analyzer-server fapolicyd-analyzer-client

fapolicyd_analyzer_server_SOURCES = server/fapolicyd-analyzer-server.cpp
fapolicyd_analyzer_server_LDADD = libfapolicyd-analyzer.la @DBUS_CXX_LIBS@
fapolicyd_analyzer_server_CPPFLAGS = @DBUS_CXX_CFLAGS@

fapolicyd_analyzer_client_SOURCES = server/fapolicyd-analyzer-client.cpp
fapolicyd_analyzer_client_LDADD = libfapolicyd-analyzer.la @DBUS_CXX_LIBS@
fapolicyd_analyzer_client_CPPFLAGS = @DBUS_CXX_CFLAGS@

