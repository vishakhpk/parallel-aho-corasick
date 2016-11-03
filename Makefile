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

parallel: build
	$(BIN_PATH)parallel -v -t -P $(DATA_PATH)patterns/example2.pat $(DATA_PATH)files/example2.txt

serial: build
	$(BIN_PATH)serial -v -t -P $(DATA_PATH)patterns/example2.pat $(DATA_PATH)files/example2.txt
