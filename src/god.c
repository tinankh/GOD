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
#include "jpegblocks.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int main(int argc, char **argv) {
    /* outputs */
    FILE *list_blocks_file ;
    list_blocks_file = fopen("list_blocks.txt", "w");

    FILE *meaningful_none0;
    meaningful_none0 = fopen("meaningful_n0.txt", "w");

    FILE *nonmeaningful;
    nonmeaningful = fopen("nonmeaningful.txt", "w");

    /* inputs */
    if (argc < 3) error ("use: god <image> <window_size>");

    /* read input parameters */
    double *input = NULL; // input image
    int X,Y,C;
    input = iio_read_image_double_split(argv[1], &X, &Y, &C);

    int block_size = atoi(argv[2]);
    if (block_size < 0 || block_size > X || block_size > Y)
        error("0 < block_size <= min(height, width)");

    /* computed values */
    int nb_block_X = X / block_size; int nb_block_Y = Y / block_size;

    double logNT = log10(nb_block_X * (nb_block_X+1) / 2.0) +
        log10(nb_block_Y * (nb_block_Y+1) / 2.0) + log10(16.0);

    /* work on Y channel */
    double *image = xcalloc(X*Y,sizeof(double)); // I
    rgb2y(input, image, X, Y, C);

    /* compute absolute value of cross_difference */
    double *cross_diff = xcalloc(X*Y,sizeof(double)); // dI
    cross_difference(image, cross_diff, X, Y);

    /* visual results */
    int result_block[8*8] = {0}; // meaningful votes
    int ballot_block[8*8] = {0}; // all votes

    /* results */
    int area = 0;
    coord index = {0,0};
    blockvote** list_Bv = xcalloc(nb_block_X * nb_block_Y, sizeof(blockvote*));

    for (int i=0; i<nb_block_X*nb_block_Y; i++) {
        index.x = i % nb_block_X;
        index.y = i / nb_block_X;
        area = (nb_block_X - index.x) * (nb_block_Y - index.y);
        list_Bv[i] = xcalloc(area, sizeof(blockvote));
    }

    /* work on several blocks */
#pragma omp parallel
    {
        int ii,jj;

        blockvote Bv;

        /* visual results */
        int _result_block[8*8] = {0};
        int _ballot_block[8*8] = {0};

#pragma omp for nowait
        for(int x0=0; x0<X; x0+=block_size)
            for(int y0=0; y0<Y; y0+=block_size) {
                for(int x1=x0+block_size-1; x1<X; x1+=block_size)
                    for(int y1=y0+block_size-1; y1<Y; y1+=block_size) {
                        memset(&Bv, 0, sizeof(blockvote));
                        Bv.coord_a.x = x0; Bv.coord_a.y = y0;
                        Bv.coord_b.x = x1; Bv.coord_b.y = y1;

                        ii = x0/block_size + y0/block_size * nb_block_X;
                        jj = x1/block_size - x0/block_size
                            + (y1/block_size - y0/block_size) *
                            (nb_block_X - x0/block_size);

                        vote(&Bv, cross_diff, X, Y, logNT);

                        /* none meaningful */
                        _ballot_block[Bv.grid.x + Bv.grid.y*8] ++;

                        /* meaningful */
                        if (Bv.meaningful) {
                            _result_block[Bv.grid.x + Bv.grid.y*8] ++;
                            if (Bv.grid.x + Bv.grid.y*8 != 0) {
                                fprintf(meaningful_none0, "%d,%d,%d,%d\n",
                                        Bv.coord_a.x, Bv.coord_a.y,
                                        Bv.coord_b.x, Bv.coord_b.y);
                            }
                        } else {
                            fprintf(nonmeaningful, "%d,%d,%d,%d\n",
                                    Bv.coord_a.x, Bv.coord_a.y,
                                    Bv.coord_b.x, Bv.coord_b.y);
                        }

                        memcpy(&list_Bv[ii][jj], &Bv, sizeof(blockvote));
                    }
            }
#pragma omp critical
        {
            for (int i=0; i<8*8; i++) {
                ballot_block[i] += _ballot_block[i];
                result_block[i] += _result_block[i];
            }
        }
    }

    /* print print */
    double best_log_nfa = DBL_MAX;
    double worst_log_nfa = -DBL_MAX;
    double mean_log_nfa;

    int N = nb_block_X * (nb_block_X + 1) * nb_block_Y * (nb_block_Y + 1) / 4;
    couple* list_NFA = xcalloc(N, sizeof(couple));

    coord maingrid = {-1,-1};
    int incr = 0;
    couple sum = {0,0};

    /* go through each block */
    for (int i=0; i<nb_block_X*nb_block_Y; i++) {
        index.x = i % nb_block_X;
        index.y = i / nb_block_X;
        area = (nb_block_X - index.x) * (nb_block_Y - index.y);

        for (int j=0; j<area; j++) {
            /* print result for each block */
            fprintf(list_blocks_file, "Block[%i][%i]:\n", i, j);
            print_results(&list_Bv[i][j], list_blocks_file);

            /* NFA sorting */
            list_NFA[incr] = list_Bv[i][j].lnfa;
            incr ++;

            sum.x += list_Bv[i][j].lnfa.x;
            sum.y += list_Bv[i][j].lnfa.y;
            best_log_nfa = MIN(best_log_nfa, MAX(list_Bv[i][j].lnfa.x,
                        list_Bv[i][j].lnfa.y));

            if (list_Bv[i][j].meaningful) {
                worst_log_nfa = MAX(worst_log_nfa, MAX(list_Bv[i][j].lnfa.x,
                            list_Bv[i][j].lnfa.y));

                /* find main grid */
                if (maingrid.x == -1 || maingrid.y == -1)
                    maingrid = list_Bv[i][j].grid;
                else if (maingrid.x != list_Bv[i][j].grid.x &&
                        maingrid.y != list_Bv[i][j].grid.y) {
                    if (result_block[maingrid.x + maingrid.y*8]
                            <= result_block[list_Bv[i][j].grid.x
                            + list_Bv[i][j].grid.y*8]) {
                        fprintf(list_blocks_file, "Main grid changed!\n");
                        maingrid = list_Bv[i][j].grid;
                    }
                }
            }
        }
    }

    /* print global output */
    int nb_grids = 0;
    printf("number of blocks: %i\n", N);
    printf("number of blocks (meaningful / non-meaningful) "
           "for each JPEG grid origin:\n");
    for (int j=0; j<8; j++) {
        for (int i=0; i<8; i++) {
            printf("%4i/%-4i ", result_block[i+j*8], ballot_block[i+j*8]);
            if (result_block[i+j*8] > 0)
                nb_grids ++;
        }
        printf("\n");
    }
    printf("number of different meaningful grids: %d\n", nb_grids);

    mean_log_nfa = MAX(sum.x/N, sum.y/N);

    if (worst_log_nfa != -DBL_MAX)
        printf("worst meaningful NFA = 10^%g\n",worst_log_nfa);
    printf("best NFA = 10^%g\n", best_log_nfa);
    printf("mean NFA = 10^%g\n", mean_log_nfa);

    if (maingrid.x != -1 && maingrid.y != -1)
        printf("main grid origin %d %d\n\n", maingrid.x, maingrid.y);
    else
        printf("main grid origin NOT found\n\n");

    if (nb_grids > 1)
        printf("more than one meaningful grid, "
               "this may indicate the presence of a forgery!\n");

    if (maingrid.x != -1 && maingrid.y != -1 &&
        (maingrid.x != 0 || maingrid.y != 0))
        printf("main grid different from (0,0), "
               "the image may have been cropped!\n");

    if ( nb_grids <= 1 && ( (maingrid.x == -1 && maingrid.y == -1 ) ||
                            (maingrid.x ==  0 && maingrid.y ==  0 ) ) )
        printf("no suspicious trace found in the image\n");

    /* save images */
    for (int i=0; i<X*Y; i++)
        cross_diff[i] *= 20.0;

    iio_write_image_double_split("cross_diff.png",cross_diff,X,Y,1);
    iio_write_image_int_vec("ballot_block.png", ballot_block, 8, 8, 1);
    iio_write_image_int_vec("result_block.png", result_block, 8, 8, 1);

    /* save txt files */
    fclose(list_blocks_file);
    fclose(meaningful_none0);
    fclose(nonmeaningful);

    /* free memory */
    free(input);
    free(image);
    free(cross_diff);
    for (int i=0; i<nb_block_X*nb_block_Y; i++)
        free(list_Bv[i]);
    free(list_Bv);
    free(list_NFA);

    return EXIT_SUCCESS;
}
