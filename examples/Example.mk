# TODO: Using M4 or something for readability.
SHELL := /usr/bin/env bash
define gen_sample_make
.PHONY: SampleMakefile
SampleMakefile:
	echo -e 'INCDIR=../../../../include\nLIBDIR=../../../../lib\nrun: $1\n	LD_LIBRARY_PATH=$$$$(LIBDIR) ./$$$$<\n$$(strip $1): $(filter-out %.h,$2)\n	$$$$(CXX) -I$$$$(INCDIR) -L$$$$(LIBDIR) $$$$^ -fno-rtti -lskypat -o $$$$@\nclean:\n	rm -f $1\n' > $$@

install-data-hook:
	cd $$(DESTDIR)$$($(strip $1)dir) && mv SampleMakefile Makefile
endef
