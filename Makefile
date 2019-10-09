BUILDDIR=./build
SRCDIR=./src
INCDIR=./include
TESTDIR=./test
DOCDIR=./doc

CC=gcc #or clang
CFLAGS=-std=gnu99 -O3 -g -Wall -Wextra -I$(INCDIR) -DNDEBUG $(OpenMP)
LIB=-lpng -ltiff -ljpeg -lm $(OpenMP)

SRC_FILES=$(wildcard $(SRCDIR)/*.c)
SRC_FILES:=$(filter-out $(SRCDIR)/main.c, $(SRC_FILES))

OBJ_FILES=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC_FILES))

EXEC0=./main

all: ./main

$(EXEC0): $(OBJ_FILES) $(BUILDDIR)/main.o
	$(CC) $^ -o $@ $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

openmp: OpenMP=-fopenmp
openmp: all

test: $(EXEC1) $(EXEC2)
	@echo
	@echo test on pelican.ppm
	@echo ===================
	$(EXEC0) pelican.ppm 128
	@echo
	@echo test on roma.pgm
	@echo ================
	$(EXEC0) roma.pgm 128

clean:
	$(RM) -rf *.png list_blocks.txt meaningful_n0.txt nonmeaningful.txt
	$(RM) -rf $(BUILDDIR) $(EXEC) main

.PHONY: clean
