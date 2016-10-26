all: example2 main

AC_PATH := ../lib/
CFLAGS := -I$(AC_PATH) -L$(AC_PATH) -lahocorasick -fopenmp -w
OS := $(shell uname)
ifeq ($(OS), Darwin)
CC := gcc-5
else
CC := gcc
endif

example2: example2.c
	$(CC) -o ../bin/example2 example2.c $(CFLAGS)
main: main.c
	$(CC) -o ../bin/main main.c $(CFLAGS) -lm
