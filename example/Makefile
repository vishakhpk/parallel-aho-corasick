
PROGRAM := example2
AC_PATH := ../ahocorasick/
CFLAGS := -I$(AC_PATH) -L$(AC_PATH) -lahocorasick -fopenmp -w
OS := $(shell uname)
ifeq ($(OS), Darwin)
CC := gcc-5
else
CC := gcc
endif
$(PROGRAM): example2.c
	cd $(AC_PATH) && make
	$(CC) -o $(PROGRAM) example2.c $(CFLAGS)

clean:
#	cd $(AC_PATH) && make clean
	rm -f $(PROGRAM)

