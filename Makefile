ROOT=../bcl

CFLAGS=-Wall -Wextra -std=c99 -I$(ROOT)/include 
LDFLAGS=-L$(ROOT)/lib
LDLIBS=-lm -lbcl

BIN=color-transfer

all: $(BIN)

clean:
	$(RM) $(BIN) *.ppm

.PHONY: clean all


