
all: example2 main
AC_PATH := ../ahocorasick/
CFLAGS := -I$(AC_PATH) -L$(AC_PATH) -lahocorasick -fopenmp -w
OS := $(shell uname)
ifeq ($(OS), Darwin)
CC := gcc-5
else
CC := gcc
endif
example2: example2.c
	cd $(AC_PATH) && make
	$(CC) -o example2 example2.c $(CFLAGS)
main: main.c
	cd $(AC_PATH) && make
	$(CC) -o main main.c $(CFLAGS)


clean:
#	cd $(AC_PATH) && make clean
	rm -f example2
	rm -f main

run:
	./main -v -P pat file