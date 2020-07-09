AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -I${top_srcdir} -I${top_srcdir}/src

LDADD = ${top_srcdir}/src/libfapolicyd-analyzer.la

bin_PROGRAMS = sha256_test match_test rpm_test table_test

sha256_test_SOURCES = sha256-test.c

match_test_SOURCES = match-test.c

rpm_test_SOURCES = rpm-test.cpp

table_test_SOURCES = table-test.cpp
