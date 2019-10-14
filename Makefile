BUILDDIR=./build
SRCDIR=./src

CFLAGS=-std=gnu99 -O3 $(OpenMP)
LIB=-lpng -ltiff -ljpeg -lm $(OpenMP)

SRC_FILES=$(wildcard $(SRCDIR)/*.c)
SRC_FILES:=$(filter-out $(SRCDIR)/god.c, $(SRC_FILES))

OBJ_FILES=$(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRC_FILES))

EXEC=./god

all: $(EXEC)

$(EXEC): $(OBJ_FILES) $(BUILDDIR)/god.o
	$(CC) $^ -o $@ $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

openmp: OpenMP=-fopenmp
openmp: all

test: $(EXEC)
	@echo
	@echo test on pelican.png
	@echo ===================
	$(EXEC) pelican.png 128
	@echo
	@echo test on roma.png
	@echo ================
	$(EXEC) roma.png 128

clean:
	$(RM) -rf $(BUILDDIR) $(EXEC)
	$(RM) -rf list_blocks.txt meaningful_n0.txt nonmeaningful.txt
	$(RM) -rf ballot_block.png cross_diff.png result_block.png

.PHONY: clean
