######################################################################
#####                                                                #
#####     LaTeX - Makefile                                           #
#####                                                                #
######################################################################
MAIN_SRC           = report
PDFLATEX_VER       = 2011
LATEX_VER          = 2011
BIBTEX_VER         = 2011
MAKEGLOSSARIES_VER = 2011
LATEX_OPT          = --src-specials
ROOT_DIR           = .
CONTENT_DIR        = content
FIG_DIR            = figures
FIG_RAW_DIR        = figures_raw

# In general, the present Makefile will consider all *.tex files in
# the content directory as source files. Thus, if any of these files
# (or the main file) will change, a re-build is invoked. If you want
# the Makefile to consider only some specific source files, you can
# list them here. Note, that you do not have to attach the *.tex
# extensions!
CUSTOM_SRC         = #01_introduction 02_related_work


######################################################################
#####                                                                #
#####     Preparation Work                                           #
#####                                                                #
######################################################################
# Get the absolute directory path of the current.
ROOT_DIR_PATH = $(shell pwd)

# Get only the directory name of the root directory.
ROOT_DIR_NAME = $(shell basename $(ROOT_DIR_PATH))

# Get the source files which should be investigated for a re-build.
ifeq ($(CUSTOM_SRC), )
  CONTENT_SRC = $(wildcard $(ROOT_DIR)/$(CONTENT_DIR)/*.tex)
else
  CONTENT_SRC = $(addsuffix .tex, $(addprefix $(ROOT_DIR)/$(CONTENT_DIR)/, $(CUSTOM_SRC)))
endif

# Get only the path to the Tgif source files.
TGIF_SRCS       = $(wildcard $(ROOT_DIR)/$(FIG_RAW_DIR)/*.obj)
TGIF_SRCS_NAMES = $(basename $(TGIF_SRCS))

# Get only the path to the Tgif destination files.
TGIF_DEST       = $(wildcard $(ROOT_DIR)/$(FIG_DIR)/*.obj)
TGIF_DEST_NAMES = $(basename $(TGIF_SRCS))

# Derive the file names for the resulting EPS and PDF file names for
# the TGIF sources.
TGIF_OUTP_NAMES     = $(addprefix $(ROOT_DIR)/$(FIG_DIR)/, $(notdir $(TGIF_SRCS_NAMES)))
TGIF_OUTP_PDF_NAMES = $(addsuffix .pdf, $(TGIF_OUTP_NAMES))


######################################################################
#####                                                                #
#####     Main Targets                                               #
#####                                                                #
######################################################################
pdf: $(MAIN_SRC).pdf

all: pdf glossary bibtex pdf pdf

help:
	@echo
	@echo "***** IIS Semester Projects/Master Theses LaTeX Report Template *****"
	@echo
	@echo "Usage    : make [options] <target>"
	@echo
	@echo "Targets  : help      - Shows the help (this text)."
	@echo
	@echo "           all       - (Re)creates output file (PDF) including all other dependencies."
	@echo "           pdf       - (Re)creates the PDF output file."
	@echo "           figures   - (Re)creates all (out-dated) Tgif figures within"
	@echo "                       '$(FIG_RAW_DIR)' and copies them to '$(FIG_DIR)'."
	@echo "           bibtex    - (Re)creates the bibliography using BibTeX."
	@echo "           glossary  - (Re)creates all glossaries (List of acronyms, Glossary, etc.)."
	@echo "           tar       - Creates a TAR archive of the current LaTeX directory."
	@echo "           zip       - Creates a ZIP archive of the current LaTeX directory."
	@echo "           clean     - Cleans up the directory structure."
	@echo
	@echo "Options  : -B        - Always build (regardless of whether the dependencies"
	@echo "                       are outdated or not)."
	@echo
	@echo "Examples : make"
	@echo "           make figures"
	@echo "           make bibtex"
	@echo

%.pdf: %.tex $(CONTENT_SRC)
	pdflatex-$(PDFLATEX_VER) $(MAIN_SRC).tex

figures: tgif_figures

view:
	okular $(MAIN_SRC).pdf &

bibtex:
	pdflatex-$(PDFLATEX_VER) $(MAIN_SRC).tex
	bibtex-$(BIBTEX_VER) $(MAIN_SRC)
	pdflatex-$(PDFLATEX_VER) $(MAIN_SRC).tex
	pdflatex-$(PDFLATEX_VER) $(MAIN_SRC).tex

glossary:
	makeglossaries-$(MAKEGLOSSARIES_VER) $(MAIN_SRC)

tar: clean
	@echo "***** Creating a TAR archive of the current directory."
	@tar -czf $(ROOT_DIR_NAME).tar.gz --exclude .svn \
	--exclude $(ROOT_DIR_NAME).tar.gz --exclude README_supervisors \
	--exclude $(ROOT_DIR_NAME).zip \
	../$(ROOT_DIR_NAME)/

zip: clean
	@echo "***** Creating a ZIP archive of the current directory."
	-@rm $(ROOT_DIR_NAME).zip
	@zip -9 -r -q $(ROOT_DIR_NAME).zip * \
	-x '*/.svn/*' -x 'README_supervisors' \
	-x '$(ROOT_DIR_NAME).tar.gz' \
	-x '$(ROOT_DIR_NAME)' -x '$(ROOT_DIR_NAME)'

clean:
	@echo "***** Cleaning the LaTeX directory structure."
	@rm -rf $(ROOT_DIR)/*.acr $(ROOT_DIR)/*.alg $(ROOT_DIR)/*.bbl
	@rm -rf $(ROOT_DIR)/*.blg $(ROOT_DIR)/*.glg $(ROOT_DIR)/*.gls
	@rm -rf $(ROOT_DIR)/*.aux $(ROOT_DIR)/*.glo $(ROOT_DIR)/*.ist
	@rm -rf $(ROOT_DIR)/*.lof $(ROOT_DIR)/*.log $(ROOT_DIR)/*.lot
	@rm -rf $(ROOT_DIR)/*.toc $(ROOT_DIR)/*.acn $(ROOT_DIR)/*.out
	@rm -rf $(ROOT_DIR)/*.tex~ $(ROOT_DIR)/*.tex.backup
	@rm -rf $(ROOT_DIR)/*.fdb_latexmk $(ROOT_DIR)/*.fls
	@rm -rf $(ROOT_DIR)/Makefile~ $(ROOT_DIR)/pso.conv-*
	@rm -rf $(ROOT_DIR)/auto
	@rm -rf $(ROOT_DIR)/content/*.tex~ $(ROOT_DIR)/content/*.aux
	@rm -rf $(ROOT_DIR)/content/*.log $(ROOT_DIR)/content/auto
	@rm -rf $(ROOT_DIR)/content/*.tex.backup
	@rm -rf $(ROOT_DIR)/content/*.fdb_latexmk $(ROOT_DIR)/content/*.fls
	@rm -rf $(ROOT_DIR)/glossaries/*.tex~ $(ROOT_DIR)/glossaries/auto
	@rm -rf $(ROOT_DIR)/preamble/auto $(ROOT_DIR)/preamble/*.tex~
	@rm -rf $(ROOT_DIR)/preamble/*.log
	@rm -rf $(ROOT_DIR)/bib/auto $(ROOT_DIR)/bib/*.bib~
	@rm -rf $(ROOT_DIR)/bib/*.bib.backup
	@rm -rf $(ROOT_DIR)/*.mtc* $(ROOT_DIR)/*.maf


######################################################################
#####                                                                #
#####     Sub Targets                                                #
#####                                                                #
######################################################################

tgif_figures: $(TGIF_OUTP_PDF_NAMES)


$(TGIF_OUTP_PDF_NAMES): $(ROOT_DIR)/$(FIG_DIR)/%.pdf : $(ROOT_DIR)/$(FIG_RAW_DIR)/%.obj
	@echo "*****"
	@echo "***** Printing Tgif figure:"
	@echo "***************************"
	@echo "***** $(ROOT_DIR)/$(FIG_RAW_DIR)/$*.eps"
	@tgif -print -epsi -color -quiet $(ROOT_DIR)/$(FIG_RAW_DIR)/$*.obj
	@echo "*****"
	@echo "***** Converting Tgif EPS to PDF:"
	@echo "*********************************"
	@echo "***** $(ROOT_DIR)/$(FIG_RAW_DIR)/$*.eps --> $(ROOT_DIR)/$(FIG_RAW_DIR)/$*.pdf"
	@epstopdf $(ROOT_DIR)/$(FIG_RAW_DIR)/$*.eps
	@echo "*****"
	@echo "***** Moving EPS and PDF figures"
	@echo "********************************"
	@echo "***** $(ROOT_DIR)/$(FIG_RAW_DIR)/$*.eps --> $(ROOT_DIR)/$(FIG_DIR)/$*.eps"
	@echo "***** $(ROOT_DIR)/$(FIG_RAW_DIR)/$*.pdf --> $(ROOT_DIR)/$(FIG_DIR)/$*.pdf"
	-@mv $(ROOT_DIR)/$(FIG_RAW_DIR)/$*.eps $(ROOT_DIR)/$(FIG_DIR)/
	-@mv $(ROOT_DIR)/$(FIG_RAW_DIR)/$*.pdf $(ROOT_DIR)/$(FIG_DIR)/
	@echo "*****"

clean-tgif-figures:
	-@rm -f $(TGIF_OUTP_EPS_NAMES) $(TGIF_OUTP_PDF_NAMES)

clean-figures:
