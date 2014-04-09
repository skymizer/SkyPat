all:
	pdflatex -output-format=pdf ./pat.tex
	bibtex ./pat.aux
	pdflatex -output-format=pdf ./pat.tex
	pdflatex -output-format=pdf ./pat.tex

clean:
	@rm pat.aux pat.bbl pat.blg pat.log pat.pdf texput.log
