
SUBDIRS = src init doc
if WITH_ANALYZER
SUBDIRS += analyzer
endif
EXTRA_DIST = ChangeLog AUTHORS NEWS README.md INSTALL fapolicyd.spec dnf/fapolicyd-dnf-plugin.py autogen.sh

clean-generic:
	rm -rf autom4te*.cache
	rm -f *.rej *.orig *.lang *.list
