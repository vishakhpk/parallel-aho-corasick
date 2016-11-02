all: example2.o parallel.o serial.o

AC_PATH := ../lib/
CFLAGS := -I$(AC_PATH) -L$(AC_PATH) -lahocorasick -fopenmp -w
OS := $(shell uname)
ifeq ($(OS), Darwin)
CC := gcc-5
else
CC := gcc
endif

example2.o: example2.c
	mkdir -p ../bin
	$(CC) -o ../bin/example2 example2.c $(CFLAGS)
parallel.o: parallel.c
	$(CC) -o ../bin/parallel parallel.c $(CFLAGS) -lm
serial.o: serial.c
	$(CC) -o ../bin/serial serial.c $(CFLAGS) -lm
