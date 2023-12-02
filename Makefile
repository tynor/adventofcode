YEAR ?= $(AOC_YEAR)
DAY ?= $(AOC_DAY)

SRCDIR = ./$(YEAR)/day-$(DAY)

.PHONY : part1 part2 init

part1 : ./.build/$(YEAR)_$(DAY)_1 $(SRCDIR)/input.txt
	$^ <$(SRCDIR)/input.txt

part2 : ./.build/$(YEAR)_$(DAY)_2 $(SRCDIR)/input.txt
	$^ <$(SRCDIR)/input.txt

init : $(SRCDIR)/part1.c $(SRCDIR)/input.txt

$(SRCDIR)/part1.c : $(SRCDIR)/input.txt
	touch $@

./.build/$(YEAR)_$(DAY)_* : $(SRCDIR)/part*.c
    clang -o $@ \
        -Wall -Wextra -pedantic \
        -std=c2x \
        -I./common \
        -fsanitize=address,undefined \
        -O2 -g3 \
        $^ \
		-MD -MMD -MF $@.d

$(SRCDIR)/input.txt :
	mkdir -p $(SRCDIR)
	curl -o $@ --cookie session=$(SESS) \
		"https://adventofcode.com/$(YEAR)/day/$(DAY)/input"
