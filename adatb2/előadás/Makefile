source = $(wildcard *.md)
target_pdf := $(source:.md=.pdf)
target_html := $(source:.md=.html)
target_self_contained_html := $(source:.md=-self-contained.html)

all: $(target_pdf) $(target_html) $(target_self_contained_html)

%.pdf: %.md
	pandoc $^ -o $@

%.html: %.md
	pandoc $^ -s -t html5 --mathjax="empty.js" --section-divs -o $@
	sed -i 's%</head>%<script type="text/javascript" src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script></head>%' $@

%-self-contained.html: %.md
	pandoc $^ -s --self-contained -t html5 --mathjax="empty.js" --section-divs -o $@
	sed -i 's%</head>%<script type="text/javascript" src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"></script></head>%' $@
