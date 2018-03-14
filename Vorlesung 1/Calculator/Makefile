LEX  = flex
YACC = bison
CC   = gcc
LD   = gcc

all: scicalc


scicalclex.c: scicalclex.l scicalcparse.tab.h

scicalcparse.tab.h: scicalcparse.y
	$(YACC) -d scicalcparse.y

scicalcparse.tab.c: scicalcparse.y
	$(YACC) -d scicalcparse.y

scicalcparse.tab.o: scicalcparse.tab.c

scicalc: scicalclex.o scicalcparse.tab.o
	$(LD) scicalclex.o scicalcparse.tab.o -o scicalc
