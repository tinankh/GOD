EXEC=./god
SRCDIR=src
CFLAGS=-std=gnu99 -O3 $(OpenMP)
LIB=-lpng -ltiff -ljpeg -lm $(OpenMP)
SRC_FILES=$(wildcard $(SRCDIR)/*.c)

all: $(EXEC)

$(EXEC): $(SRC_FILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LIB)

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
	$(RM) -rf $(EXEC)
	$(RM) -rf list_blocks.txt meaningful_n0.txt nonmeaningful.txt
	$(RM) -rf ballot_block.png cross_diff.png result_block.png

.PHONY: clean
