LEX  = flex
YACC = bison
CC   = gcc
LD   = gcc
CFLAGS = -std=gnu99 -ggdb


.PHONY: all clean tags

all: exprcc

clean:
	@touch __dummy.o __dummy~ exprcc nanoparser.output TAGS nanoparser.tab.c
	rm nanoparser.tab.[ch] *.o *~ exprcc nanoparser.output TAGS

tags:
	etags *.c *.h *.y *.l

nanolex.c: nanolex.l nanoparser.tab.h

nanoparser.tab.h: nanoparser.y ast.h
	$(YACC) --verbose -d nanoparser.y

nanoparser.tab.c: nanoparser.y ast.h
	$(YACC) -d nanoparser.y

nanoparser.tab.o: nanoparser.tab.c

ast.o: ast.h ast.c

exprcc: nanolex.o nanoparser.tab.o ast.o
	$(LD) nanolex.o nanoparser.tab.o ast.o -o exprcc
