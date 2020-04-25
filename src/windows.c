/*----------------------------------------------------------------------------

  Copyright (c) 2018-2020 Rafael Grompone von Gioi <grompone@gmail.com>
  Copyright (c) 2018-2020 Tina Nikoukhah <tinanikoukhah@gmail.com>

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
#include <string.h>
#include <math.h>

#include "acontrario.h"
#include "windows.h"
#include "misc.h"

/**
 *   Computes the cross-difference of an image
 */
void cross_difference(double *image, double *cross_diff, int X, int Y) {
    for (int x=0; x<(X-1); x++)
        for (int y=0; y<(Y-1); y++) {
            cross_diff[x+y*X] = fabs(image[x+y*X] + image[x+1+(y+1)*X]
                                     - image[x+1+y*X] - image[x+(y+1)*X]);
        }
}

/**
 *    Computes the NFA of a window
 */
void compute_NFA(window *win, double logNT) {
    win->nx = 0;
    win->kx = 0;
    win->ny = 0;
    win->ky = 0;

    for (int i=0; i<8; i++) {
        win->nx += win->vote_col[i];
        if (win->vote_col[i] > win->kx) {
            /* new max of votes in x */
            win->kx = win->vote_col[i];
            win->grid.x = (i+1) % 8;
        }
        win->ny += win->vote_row[i];
        if (win->vote_row[i] > win->ky) {
            /* new max of votes in y */
            win->ky = win->vote_row[i];
            win->grid.y = (i+1) % 8;
        }
    }

    int w = win->coord_b.x - win->coord_a.x + 1;
    int h = win->coord_b.y - win->coord_a.y + 1;

    win->lnfa.x = log_nfa( (int)(w*h/16), win->kx/2,
                           (double)win->nx/(w*h), logNT );
    win->lnfa.y = log_nfa( (int)(w*h/16), win->ky/2,
                           (double)win->ny/(w*h), logNT);
}

/**
 *   Performs the voting process of a window
 */
void vote(window *win, double *cross_diff, int X, int Y) {
    for (int i=0; i<8; i++)
        win->vote_col[i] = win->vote_row[i] = 0;

    for (int x=win->coord_a.x; x<=win->coord_b.x; x++)
        for (int y=win->coord_a.y; y<=win->coord_b.y; y++) {
            if( x>0 && x<(X-2) && cross_diff[x+y*X] > cross_diff[x-1+y*X]
                && cross_diff[x+y*X] > cross_diff[x+1+y*X] )
                win->vote_col[x%8] += 1;
            if( y>0 && y<(Y-2) && cross_diff[x+y*X] > cross_diff[x+(y-1)*X]
                && cross_diff[x+y*X] > cross_diff[x+(y+1)*X] )
                win->vote_row[y%8] += 1;
        }
}

/**
 *   Print global summary
 */
void summary(int *num_meaningful_windows, double *best_lnfa, int num_windows) {
    int most_meaningful_grid = -1;
    int num_meaningful_grids = 0;
    int second_most_meaningful_grid = -1;
    int total_num_meaningful_windows = 0;

    for (int i=0; i<64; i++)
        if (best_lnfa[i] < 0.0) {
            num_meaningful_grids++;
            total_num_meaningful_windows += num_meaningful_windows[i];

            if (most_meaningful_grid == -1 ||
                best_lnfa[i] < best_lnfa[most_meaningful_grid]) {
                if (most_meaningful_grid != -1)
                    second_most_meaningful_grid = most_meaningful_grid;
                most_meaningful_grid = i;
            }
            else if (second_most_meaningful_grid == -1 ||
                     best_lnfa[i] < best_lnfa[second_most_meaningful_grid])
                second_most_meaningful_grid = i;
        }

    /* print global output */
    printf("total number of evaluated windows: %i\n", num_windows);

    printf("number of meaningful windows: %i (%i %%)\n",
           total_num_meaningful_windows,
           100 * total_num_meaningful_windows / num_windows);

    printf("number of meaningful windows for each JPEG grid origin:\n");
    for (int j=0; j<8; j++) {
        for (int i=0; i<8; i++)
            printf("%8i ", num_meaningful_windows[i+j*8]);
        printf("\n");
    }

    printf("best log(NFA) for each JPEG grid origin:\n");
    for (int j=0; j<8; j++) {
        for (int i=0; i<8; i++)
            if (best_lnfa[i+j*8] < 0.0)
                printf("%8.1f ", best_lnfa[i+j*8]);
            else printf("       - ");
        printf("\n");
    }

    printf("number of meaningful JPEG grids found: %d\n",
           num_meaningful_grids);

    if (most_meaningful_grid != -1)
        printf("most meaningful JPEG grid origin (%d,%d) with NFA: 10^%g\n",
               most_meaningful_grid%8, most_meaningful_grid/8,
               best_lnfa[most_meaningful_grid]);
    else
        printf("no meaningful grid found\n");

    if (num_meaningful_grids > 1) {
        printf("second most meaningful JPEG grid origin "
               "(%d,%d) with NFA: 10^%g\n",
               second_most_meaningful_grid%8, second_most_meaningful_grid/8,
               best_lnfa[second_most_meaningful_grid]);
        printf("\nThis image shows more than one meaningful JPEG grid.  "
               "This may be caused by\nimage manipulations such as resampling "
               "or some particular image contents.\nIt may also be due to a "
               "forgery in the image.  Please examine the deviant\nmeaningful "
               "blocks to make your own opinion about a potential forgery.\n");
    }

    if (most_meaningful_grid != -1 && most_meaningful_grid != 0
        && second_most_meaningful_grid == -1)
        printf("\nThe most meaningful JPEG grid origin is not (0,0).  "
               "This may indicate that\nthe image have been cropped.\n");

    if (num_meaningful_grids <= 1 && (most_meaningful_grid == -1 ||
                                      most_meaningful_grid == 0))
        printf("\nNo suspicious traces found in the image "
               "with the performed analysis.\n");
}
