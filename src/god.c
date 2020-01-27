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
#include <math.h>
#include <float.h>
#include <string.h>

#include "iio.h"
#include "acontrario.h"
#include "misc.h"
#include "windows.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int main(int argc, char **argv) {
    /* inputs */
    if (argc < 3) error ("use: god <image> <window_size>");

    /* read input parameters */
    double *input = NULL; // input image
    int X,Y,C;
    input = iio_read_image_double_split(argv[1], &X, &Y, &C);

    int window_size = atoi(argv[2]);

    if (window_size <= 0 || window_size > X-3 || window_size > Y-3
        || window_size%8 != 0) {
        if (window_size%8 !=0)
            error("window_size should be an integer multiple of 8 \n"
                  "0 <= window_size <= min(height, width)\n"
                  "you may want to try %d", window_size/8 * 8);

        error("window_size should be an integer\n"
              "0 <= window_size <= min(height, width)");
    }

    /* outputs */
    FILE *list_windows ;
    list_windows = fopen("list_windows.txt", "w");

    /* maximum number of tests */
    double logNT = log10(X*Y) + log10(16.0);

    /* work on Y channel
       xcalloc initializes the data to zero */
    double *image = xcalloc(X*Y,sizeof(double)); // I
    rgb2y(input, image, X, Y, C);

    /* compute absolute value of cross_difference.
       xcalloc initializes the data to zero */
    double *cross_diff = xcalloc(X*Y,sizeof(double));
    cross_difference(image, cross_diff, X, Y);

    /* results */
    int num_meaningful_windows[8*8] = {0}; // meaningful votes

    /* best NFA initialized to maximum number of tests */
    double best_lnfa[8*8] ;
    for (int i=0; i<64; i++) best_lnfa[i] = logNT;

    /* work on several windows */
#pragma omp parallel for
    for (int x0=1; x0<X-2; x0+=window_size)
        for (int y0=1; y0<Y-2; y0+=window_size)
            for (int x1=x0+window_size-1; x1<X-2; x1+=window_size)
                for (int y1=y0+window_size-1; y1<Y-2; y1+=window_size) {
                    window win;

                    win.coord_a.x = x0;
                    win.coord_a.y = y0;
                    win.coord_b.x = x1;
                    win.coord_b.y = y1;

                    vote(&win, cross_diff, X, Y);

                    compute_NFA(&win, logNT);

#pragma omp critical
                    {
                        /* store window statistics */
                        double lnfa = MAX(win.lnfa.x, win.lnfa.y);
                        int index = win.grid.x + 8*win.grid.y;
                        if (lnfa < 0.0) {
                            num_meaningful_windows[index]++;
                            if (lnfa < best_lnfa[index])
                                best_lnfa[index] = lnfa;
                        }

                        /* write window attributes to window list file */
                        fprintf(list_windows, "%d %d %d %d %g %g %d %d \n",
                                win.coord_a.x, win.coord_a.y,
                                win.coord_b.x, win.coord_b.y,
                                win.lnfa.x, win.lnfa.y,
                                lnfa < 0.0 ? win.grid.x : -1,
                                lnfa < 0.0 ? win.grid.y : -1);
                    }
                }

    /* number of total windows tested */
    int num_windows = (int)((X-3)/window_size) * (int)((X-3)/window_size + 1) *
        (int)((Y-3)/window_size) * (int)((Y-3)/window_size + 1) / 4;

    /* print global summary */
    summary(num_meaningful_windows, best_lnfa, num_windows);

    /* save images */
    for (int i=0; i<X*Y; i++)
        cross_diff[i] *= 20.0;

    iio_write_image_double_split("cross_diff.png", cross_diff, X, Y, 1);
    iio_write_image_double_split("luminance.png", image, X, Y, 1);

    /* save txt files */
    fclose(list_windows);

    /* free memory */
    free(input);
    free(image);
    free(cross_diff);

    return EXIT_SUCCESS;
}
