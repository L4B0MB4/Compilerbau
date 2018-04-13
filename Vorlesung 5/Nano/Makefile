CC   = gcc


all: typesNsymbols

clean:
	@touch __dummy.o __dummy~ typesNsymbols nanoparser.output TAGS nanoparser.tab.c
	rm nanoparser.tab.[ch] *.o *~ typesNsymbols nanoparser.output TAGS

typesNsymbols: symbols.c types.c testprogramm.c
	$(CC) symbols.c types.c testprogramm.c -o typesNsymbols
