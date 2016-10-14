ACVERSION := 1.3
LIBNAME := libahocorasick-$(ACVERSION).a
CFLAGS := -Wall

$(LIBNAME): aho_corasick.o node.o
	ar -cvq $(LIBNAME) aho_corasick.o node.o
	ln -s -f $(LIBNAME) libahocorasick.a

aho_corasick.o: aho_corasick.c aho_corasick.h node.o
	cc -c aho_corasick.c $(CFLAGS)

node.o: node.c node.h ac_types.h config.h
	cc -c node.c $(CFLAGS)

clean:
	unlink libahocorasick.a
	rm -f aho_corasick.o node.o $(LIBNAME)

