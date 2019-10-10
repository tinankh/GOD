BUILDDIR=./build
SRCDIR=./src
INCDIR=./include
TESTDIR=./test
DOCDIR=./doc

CC=gcc #or clang
CFLAGS=-std=gnu99 -O3 -g -I$(INCDIR) $(OpenMP)
LIB=-lpng -ltiff -ljpeg -lm $(OpenMP)

SRC_FILES=$(wildcard $(SRCDIR)/*.c)
SRC_FILES:=$(filter-out $(SRCDIR)/god.c, $(SRC_FILES))

OBJ_FILES=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC_FILES))

EXEC0=./god

all: ./god

$(EXEC0): $(OBJ_FILES) $(BUILDDIR)/god.o
	$(CC) $^ -o $@ $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

openmp: OpenMP=-fopenmp
openmp: all

test: $(EXEC1) $(EXEC2)
	@echo
	@echo test on pelican.png
	@echo ===================
	$(EXEC0) pelican.png 128
	@echo
	@echo test on roma.png
	@echo ================
	$(EXEC0) roma.png 128

clean:
	$(RM) -rf list_blocks.txt meaningful_n0.txt nonmeaningful.txt
	$(RM) -rf $(BUILDDIR) $(EXEC) god

.PHONY: clean
