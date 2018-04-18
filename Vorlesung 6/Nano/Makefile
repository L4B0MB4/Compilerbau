LEX  = flex
YACC = bison
CC   = gcc
LD   = gcc
CFLAGS = -std=gnu99 -ggdb


.PHONY: all clean tags

all: semanticTables

clean:
	@touch __dummy.o __dummy~ semanticTables nanoparser.output TAGS nanoparser.tab.c
	rm nanoparser.tab.[ch] *.o *~ semanticTables nanoparser.output TAGS
clear:
	@touch __dummy.o __dummy~ nanoparser.output TAGS nanoparser.tab.c
	rm nanoparser.tab.[ch] *.o *~ nanoparser.output TAGS

tags:
	etags *.c *.h *.y *.l

nanolex.c: nanolex.l nanoparser.tab.h

nanoparser.tab.h: nanoparser.y ast.h
	$(YACC) --verbose -d nanoparser.y

nanoparser.tab.c: nanoparser.y ast.h
	$(YACC) -d nanoparser.y

nanoparser.tab.o: nanoparser.tab.c

ast.o: ast.h ast.c

types.o: types.h types.c

symbols.o: symbols.h symbols.c

semantic.o: semantic.h semantic.c

semanticTables: nanolex.o nanoparser.tab.o ast.o types.o symbols.o semantic.o
	$(LD) nanolex.o nanoparser.tab.o ast.o types.o symbols.o semantic.o -o semanticTables && make clear
