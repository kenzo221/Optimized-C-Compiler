#Define the location of the include directory and the location to install the compiler binary
INCDIR=/tmp/include
BINDIR=/tmp

HSRCS= data.h declaration.h definitions.h
SRCS=  codegenerator.c declaration_parse.c expression_parse.c generator.c main.c 		miscellaneous.c \
	folding.c lexical_scanner.c statement_parse.c symboltable.c ASTtree.c datatypes.c

occ: $(SRCS) $(HSRCS)
	cc -o occ -g -Wall -DINCDIR=\"$(INCDIR)\" $(SRCS)

install: occ
	mkdir -p $(INCDIR)
	rsync -a include/. $(INCDIR)
	cp occ $(BINDIR)
	chmod +x $(BINDIR)/occ

clean:
	rm -f occ *.o *.s out a.out

test: install tests/runtests
	(cd tests; chmod +x runtests; ./runtests)
