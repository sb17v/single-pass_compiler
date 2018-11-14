CC=cc
CPP=c++
LIBS=
COFLAGS=-g -O2
CWFLAGS=-Wall -Wextra
CIFLAGS=
CMFLAGS=
CFLAGS=$(CWFLAGS) $(COFLAGS) $(CIFLAGS) $(CMFLAGS)
all:	lextest compile
lextest:	javaclass.h bytecode.h error.h global.h init.h lexer.h symbol.h javaclass.o bytecode.o error.o init.o lexer.o symbol.o lextest.o
			$(CC) $(CFLAGS) -o lextest lextest.c javaclass.o bytecode.o init.o lexer.o symbol.o error.o -I.

compile:	javaclass.h bytecode.h error.h global.h init.h lexer.h symbol.h javaclass.o bytecode.o error.o init.o lexer.o symbol.o compile.o
			$(CC) $(CFLAGS) -o compile compile.c javaclass.o bytecode.o init.o lexer.o symbol.o error.o -I.
#lextest_main:	lextest
#		./lextest < mytest
.c.o:
		$(CC) -c $<

.PHONY: 	clean distclean
clean:
		-rm -f *.o
distclean:
		-rm -f compile lextest *.o *.class
