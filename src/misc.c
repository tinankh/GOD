/*----------------------------------------------------------------------------

  Copyright (c) 2018-2019 Rafael Grompone von Gioi <grompone@gmail.com>
  Copyright (c) 2018-2019 Tina Nikoukhah <tinanikoukhah@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Affero General Public License as
  published by the Free Software Foundation, either version 3 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/>.

  ----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

void error(char * msg) {
    fprintf(stderr,"Error: %s\n",msg);
    exit(EXIT_FAILURE);
}

void * xmalloc(size_t n_items, size_t size) {
    void * p;
    if (size == 0)
        error("xmalloc: zero size");
    p = malloc(n_items*size);
    if (p == NULL)
        error("xmalloc: out of memory");
    return p;
}

void * xcalloc(size_t n_items, size_t size) {
    void * p;
    if (size == 0)
        error("xcalloc: zero size");
    p = calloc(n_items, size);
    if (p == NULL)
        error("xcalloc: out of memory");
    return p;
}

int grayscale(double *input, double *image, int X, int Y, int C) {
    int x, y, c;
    for (x=0; x<X; x++)
        for (y=0; y<Y; y++) {
            for (c=0; c<C; c++)
                image[x+y*X] += input[x+y*X+c*X*Y];
        }
    return 1;
}

int rgb2y(double *input, double *image, int X, int Y, int C) {
    int x, y;
    if (C >= 3) {
        for(x=0; x<X; x++)
            for(y=0; y<Y; y++)
                image[x+y*X] = 0.299*input[x+y*X]
                    +0.587*input[x+y*X+X*Y]
                    +0.114*input[x+y*X+2*X*Y];
    }
    else
        grayscale(input, image, X, Y, C);
    return 1;
}
