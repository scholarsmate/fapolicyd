AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -I${top_srcdir} -I${top_srcdir}/src

LDADD = ${top_srcdir}/src/libfapolicyd-analyzer.la

bin_PROGRAMS = sha256_test match_test rpm_test table_test

if WITH_RPM
bin_PROGRAMS += rpmdb_read_test rpmdb_table_test
rpmdb_read_test_SOURCES = rpmdb_read-test.c
rpmdb_table_test_SOURCES = rpmdb_table-test.cpp
endif

sha256_test_SOURCES = sha256-test.c

match_test_SOURCES = match-test.c

rpm_test_SOURCES = rpm-test.cpp

table_test_SOURCES = table-test.cpp
