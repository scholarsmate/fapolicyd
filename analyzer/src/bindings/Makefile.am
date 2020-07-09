AM_CPPFLAGS = -I${top_srcdir} -I${top_srcdir}/src

BUILT_SOURCES = python/fapolicyanalyzer.py python/fapolicyanalyzer_wrap.cpp
CLEANFILES = python/fapolicyanalyzer.py python/fapolicyanalyzer_wrap.cpp

# Tell Make how to create the generated SWIG files for Python.
python/fapolicyanalyzer.py python/fapolicyanalyzer_wrap.cpp: fapolicyanalyzer.i
	$(SWIG) -Wall -c++ $(AX_SWIG_PYTHON_OPT) -I${top_srcdir}/src -o $@ $<

