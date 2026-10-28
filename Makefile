CC 		= gcc

CFLAGS 		= -Wall -pedantic -ansi

CPPFLAGS        =-I./include

LDFLAGS		=-L./lib

LDLIBS		= -limage

INC_DIR	        =./include

SRC_DIR         =./src

BIN_DIR		=./bin

CURRENT_DIR	=.

C_FILES_Q = $(SRC_DIR)/prog_quantification.c $(SRC_DIR)/quantification.c $(SRC_DIR)/histogram.c
C_FILES_QL = $(SRC_DIR)/prog_quantificationL_1_2.c $(SRC_DIR)/quantification.c $(SRC_DIR)/histogram.c
C_FILES_P = $(SRC_DIR)/ppmhist.c $(SRC_DIR)/histogram.c
C_FILES_T = $(SRC_DIR)/test/test_projet.c $(SRC_DIR)/quantification.c $(SRC_DIR)/histogram.c


ifeq ($(DEBUG), 1)
	CFLAGS += -g
endif

.PHONY: all ppmhist prog_quantification test_projet clean

all: ppmhist quantification 


ppmhist: $(C_FILES_P) $(INC_DIR)/histogram.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $(C_FILES_P) -o $(BIN_DIR)/$@ $(LDFLAGS) $(LDLIBS)

prog_quantification: $(C_FILES_Q) $(INC_DIR)/histogram.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $(C_FILES_Q) -o $(BIN_DIR)/$@ $(LDFLAGS) $(LDLIBS)

prog_quantificationL: $(C_FILES_QL) $(INC_DIR)/histogram.h
	$(CC) $(CFLAGS) $(CPPFLAGS) $(C_FILES_QL) -o $(BIN_DIR)/$@ $(LDFLAGS) $(LDLIBS)

test_projet: $(C_FILES_T) $(INC_DIR)/histogram.h
	$(CC) -std=c99 -D_POSIX_C_SOURCE=200809L $(CFLAGS) $(CPPFLAGS) $(C_FILES_T) -o $(BIN_DIR)/$@ $(LDFLAGS) $(LDLIBS) -lm 

# gcc -Wall -ansi -pedantic -Iinclude src/quantification.c src/histogram.c -o bin/quantification -Llib -limage
#gcc -std=c99 -D_POSIX_C_SOURCE=200809L -Wall -Iinclude -Itest     src/test/test_projet.c     src/histogram.c     src/quantification.c     -Llib -limage     -o test_runner -lm

clean:
	rm -r $(BIN_DIR)/*
