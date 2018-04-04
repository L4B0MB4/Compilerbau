# Set variables
LEX  = flex
YACC = bison --verbose
CC   = gcc
LD   = gcc
CFLAGS = -ggdb -Wall -Wno-unused-label -Wno-unused-function

# The following are not real targets, i.e. not files that need to be created
.PHONY: all clean

all: nanolex2

clean:
	touch a~ nanolex_int.c a.o
	rm *~ nanolex_int.c *.o

grammar.tab.h: grammar.y
	$(YACC) -d grammar.y

grammar.tab.c: grammar.y
	$(YACC) -d grammar.y

nanolex_int.c: nanolex_int.l grammar.tab.h

grammar.tab.o: grammar.tab.c

nanolex_int.o: nanolex_int.c


nanolex2: nanolex_int.o grammar.tab.o
	$(LD) -o nanolex2 nanolex_int.o grammar.tab.o
