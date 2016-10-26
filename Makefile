AC_PATH   := lib/
SRC_PATH  := src/
BIN_PATH  := bin/
DATA_PATH := data/

build:
	cd $(AC_PATH) && make
	cd $(SRC_PATH) && make

clean:
	# cd $(AC_PATH) && make clean
	rm -f $(BIN_PATH)*

run:
	$(BIN_PATH)main -v -P $(DATA_PATH)patterns/example2.pat $(DATA_PATH)files/example2
