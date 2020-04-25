EXEC=./god
SRCDIR=src
CFLAGS=-O3 $(OpenMP)
LIB=-lpng -ltiff -ljpeg -lm
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
	$(RM) -rf list_windows.txt
	$(RM) -rf cross_diff.png luminance.png

.PHONY: clean
