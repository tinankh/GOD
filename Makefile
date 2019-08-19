BINDIR=./bin
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

EXEC0=$(BINDIR)/main

all: $(BINDIR)/main

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
	$(EXEC0) pelican.ppm 128 > temp
	cat global_result.txt
	@echo
	@echo test on roma.pgm
	@echo ================
	$(EXEC0) roma.pgm 128 >> temp
	cat global_result.txt
	$(RM) temp

clean:
	$(RM) -rf *.png global_result.txt significant_n0.txt insignificant.txt
	$(RM) -rf $(BUILDDIR) $(EXEC)

.PHONY: clean
