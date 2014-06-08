# Makefile for Ottawa Linux Symposium papers
#
# To build your paper, simply type "make".
# The output appears in a subdirectory called "output".
#
# Type "make clean" to remove all the generated files.
# Note that the whole "output" directory will be removed.

# The main input filename is assumed to be $AUTHOR.tex
AUTHOR		:= $(notdir $(CURDIR))

# Graphics need to be converted to EPS/PDF formats.
$(AUTHOR)DIA	:= $(wildcard *.dia)
$(AUTHOR)DOT	:= $(wildcard *.dot)
$(AUTHOR)FIG	:= $(wildcard *.fig)
$(AUTHOR)NEATO	:= $(wildcard *.neato)
$(AUTHOR)PLOT	:= $(wildcard *.plot)
$(AUTHOR)PNG	:= $(wildcard *.png)
$(AUTHOR)SVG	:= $(wildcard *.svg)

# BibTeX database (optional)
$(AUTHOR)BIB	:= $(wildcard *.bib)

# Pull in common makefile to do the rest of the work
-include common/Rules.mk ../common/Rules.mk
