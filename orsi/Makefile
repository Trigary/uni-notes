source = $(wildcard *.md)
target_pdf := $(source:.md=.pdf)

all: $(target_pdf)

%.pdf: %.md Makefile
	pandoc $< -o $@ --toc -V lang=hu -V geometry:margin=3cm
