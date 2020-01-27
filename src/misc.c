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
#include <stdarg.h>

/*----------------------------------------------------------------------------*/
/** Fatal error, print a formated message to standard-error output and exit.

    The syntax is exactly as the one of printf, but it adds "error: "
    before the message and then it end the program with an error number.
 */
void error(char * msg, ...)
{
  va_list argp;

  fprintf(stderr,"error: ");

  /* print the formated error message */
  va_start(argp,msg);
  vfprintf(stderr,msg,argp);
  va_end(argp);

  fprintf(stderr,"\n");

  /* end execution with an error */
  exit(EXIT_FAILURE);
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

/* this function assumes that *image is allocated and initialized to zero */
void image2grayscale(double *input, double *image, int X, int Y, int C) {
    for (int x=0; x<X; x++)
        for (int y=0; y<Y; y++)
            for (int c=0; c<C; c++)
                image[x+y*X] += input[x+y*X+c*X*Y];
}

/* this function assumes that *image is allocated and initialized to zero */
void rgb2y(double *input, double *image, int X, int Y, int C) {
    if (C >= 3) {
        for (int x=0; x<X; x++)
            for (int y=0; y<Y; y++)
                image[x+y*X] = 0.299*input[x+y*X]
                    +0.587*input[x+y*X+X*Y]
                    +0.114*input[x+y*X+2*X*Y];
    }
    else
        image2grayscale(input, image, X, Y, C);
}
