PRJ_NAME = Rapport_ME3_BITON_GOUY

SOURCE = main
OUT_DIR = ./temp
LATEX_CMD = latex -output-format='pdf' -output-directory=$(OUT_DIR) $(SOURCE).tex

draw_src = $(shell find ./img-data -type f -name "*.drawio.svg")
draw_target = $(draw_src:./img-data/%.drawio.svg=./img-data/%.drawio.png)

#https://unix.stackexchange.com/questions/140912/no-target-error-using-make

# Needed: "latex" and "texlive-bibtex-extra"
./$(OUT_DIR)/$(SOURCE).aux: *.tex ./sub/* $(draw_target) makefile *.bib
	if [ ! -d "temp/" ]; then mkdir temp;  mkdir temp/sub; fi
	$(LATEX_CMD)
	bibtex ./$(OUT_DIR)/$(SOURCE)
# https://www.dickimaw-books.com/software/bib2gls/
# Java needed to execute
#	bib2gls ./$(OUT_DIR)/$(SOURCE)
	mv ./temp/main.pdf ./$(PRJ_NAME).pdf

# Generate png from svg
#	Input svg file, <name>.drawio.svg
#	Output png file, <name>.drawio.png
./img-data/%.drawio.png: ./img-data/%.drawio.svg
	inkscape --export-area-drawing --export-dpi=300 --export-png=$@ $<

# Delete .aux files (to force recompile)
clean:
	find . -name '*.aux' -delete

# Delete all compilation output files
reset:
	make clean
	rm -f $(PRJ_NAME).pdf
	rm -R -f temp

# Copy the current version of the report at the root of the project
release:
	make
# For a release, it is recommended to compile thrice (for labels and references)
	$(LATEX_CMD)
	$(LATEX_CMD)
	mv ./temp/main.pdf ./$(PRJ_NAME).pdf
	cp -f ./$(PRJ_NAME).pdf ../
