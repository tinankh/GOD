#ifndef MISC_H
#define MISC_H

#include <stdlib.h>

void error(char * msg);
void * xmalloc(size_t n_items, size_t size);
void * xcalloc(size_t n_items, size_t size);

int grayscale(double *input, double *image, int X, int Y, int C);
int rgb2y(double *input, double *image, int X, int Y, int C);

#endif