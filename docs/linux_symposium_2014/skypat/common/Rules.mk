#
# Rules.mk
# Global rules used by all OLS papers
#
# Please do not modify this file without consulting the
# OLS Proceedings team in advance.
#

ifndef AUTHOR
$(error AUTHOR not defined)
endif

# Put all latex/pdflatex output into its own subdirectory
OUTPUT := output

# Default target to build
TARGETS += $(OUTPUT)/$(AUTHOR).pdf
TARGETS += $(OUTPUT)/$(AUTHOR).ps

#
# Default target: users will run "make" and "make clean"
#
.PHONY: all clean
all: $(TARGETS)
clean:
	rm -rf $(OUTPUT)

#
# Additional build targets for graphics needing conversion
#
$(AUTHOR)EPS := $(addprefix $(OUTPUT)/,\
		$($(AUTHOR)DOT:.dot=.eps) \
		$($(AUTHOR)DIA:.dia=.eps) \
		$($(AUTHOR)FIG:.fig=.eps) \
		$($(AUTHOR)NEATO:.neato=.eps) \
		$($(AUTHOR)PLOT:.plot=.eps) \
		$($(AUTHOR)PNG:.png=.eps) \
		$($(AUTHOR)SVG:.svg=.eps))

$(AUTHOR)PDF := $(addprefix $(OUTPUT)/,\
		$($(AUTHOR)DOT:.dot=.pdf) \
		$($(AUTHOR)DIA:.dia=.pdf) \
		$($(AUTHOR)FIG:.fig=.pdf) \
		$($(AUTHOR)NEATO:.neato=.pdf) \
		$($(AUTHOR)PLOT:.plot=.pdf) \
		$($(AUTHOR)PNG:.png=.pdf) \
		$($(AUTHOR)SVG:.svg=.pdf))

#
# Options for running latex/pdflatex
#
# - Disable LaTeX interactive mode since it confuses many users.
# - Report errors in quickfix format for convenience with Vim/Emacs/etc.
# - Place all output files into a subdirectory.
LATEX_OPTS := -interaction=nonstopmode
LATEX_OPTS += -file-line-error
LATEX_OPTS += -output-directory=$(OUTPUT)

# Adjust the LaTeX search path to find ols.cls
export TEXINPUTS:=$(OUTPUT):.:..:common:../common:

# Handle a single BibTeX database
$(AUTHOR)BBL := $(addprefix $(OUTPUT)/,$($(AUTHOR)BIB:.bib=.bbl))

$($(AUTHOR)BBL): $($(AUTHOR)BIB)
	cp $< $(OUTPUT)
	test -d common && cp common/*.bst $(OUTPUT) || cp ../common/*.bst $(OUTPUT)
	-pdflatex $(LATEX_OPTS) $(AUTHOR)
	cd $(OUTPUT) && bibtex $(AUTHOR)

# Generate PDF, run it three times to get xrefs right
$(OUTPUT)/$(AUTHOR).pdf: $(OUTPUT) $(AUTHOR).tex $($(AUTHOR)BBL) $($(AUTHOR)PDF)
	pdflatex $(LATEX_OPTS) $(filter %.tex,$+)
	pdflatex $(LATEX_OPTS) $(filter %.tex,$+)
	pdflatex $(LATEX_OPTS) $(filter %.tex,$+)
	@echo "Output $@ created"

# Generate PS version
$(OUTPUT)/$(AUTHOR).ps: $(OUTPUT) $(AUTHOR).tex $($(AUTHOR)BBL) $($(AUTHOR)EPS)
	latex $(LATEX_OPTS) $(filter %.tex,$+)
	latex $(LATEX_OPTS) $(filter %.tex,$+)
	latex $(LATEX_OPTS) $(filter %.tex,$+)
	dvips -o $@ $(OUTPUT)/$(AUTHOR).dvi
	@echo "Output $@ created"

$(OUTPUT):
	mkdir $(OUTPUT)

#
# Pattern rules for converting graphic formats to EPS/PDF.
#

# Generic EPS-to-PDF, useful for gnuplot/dia/etc.
$(OUTPUT)/%.pdf: %.eps
	epstopdf --outfile=$@ $<

$(OUTPUT)/%.pdf: $(OUTPUT)/%.eps
	epstopdf --outfile=$@ $<

# dia to eps
$(OUTPUT)/%.eps: %.dia
	dia --export=$@ -t eps-builtin $^

# gnuplot, must set terminal postscript eps with correct output file
$(OUTPUT)/%.eps: %.plot
	gnuplot $<

# graphviz dot files, to eps
$(OUTPUT)/%.eps: %.dot
	dot $< -o $@ -T ps
	ps2epsi $@ tmp.epsi
	rm -f $@
	mv tmp.epsi $@

# graphviz dot files, to pdf
$(OUTPUT)/%.pdf: %.dot
	dot $< -o $@ -T pdf

# graphviz neato files, to eps
$(OUTPUT)/%.eps: %.neato
	neato $< -o $@ -T ps
	ps2epsi $@ tmp.epsi
	rm -f $@
	mv tmp.epsi $@

# graphviz neato files, to pdf
$(OUTPUT)/%.pdf: %.neato
	neato $< -o $@ -T pdf

# Inkscape to PDF - the crop is not always necessary.
$(OUTPUT)/%.pdf: %.svg
	inkscape -z -d 1200 -f $^ -A $@.tmp
	pdfcrop --clip $@.tmp $@
	rm -f $@.tmp

# Inkscape to EPS
$(OUTPUT)/%.eps: %.svg
	inkscape -z -d 1200 -f $^ -E $@

# fig files - to eps
$(OUTPUT)/%.eps: %.fig
	fig2dev -L eps $< >$@

# fig files - to pdf
$(OUTPUT)/%.pdf: %.fig
	fig2dev -L pdf $< >$@

# PNG images - avoid using because they do not scale well
$(OUTPUT)/%.eps : %.png
	convert $^ $@
