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
#include <string.h>
#include <math.h>

#include "acontrario.h"
#include "jpegblocks.h"
#include "misc.h"


/**
 *   Computes the cross-difference of an image
 */
void cross_difference(double *image, double *cross_diff, int X, int Y) {
    int x, y;
    for (x=0; x<(X-1); x++)
        for (y=0; y<(Y-1); y++) {
            cross_diff[x+y*X] = fabs(image[x+y*X] + image[x+1+(y+1)*X]
                                     - image[x+1+y*X] - image[x+(y+1)*X]);
        }
}

/**
 *    Computes the NFA of a blockvote
 */
void compute_NFA(blockvote *Bv, double logNT) {
    int i;
    Bv->nx = 0;
    Bv->kx = 0;
    Bv->ny = 0;
    Bv->ky = 0;

    for (i=0; i<8; i++) {
        Bv->nx += Bv->vote_col[i];
        if (Bv->vote_col[i] > Bv->kx) {
            /* new max of votes in x */
            Bv->kx = Bv->vote_col[i];
            Bv->grid.x = (i+1) % 8;
        }
        Bv->ny += Bv->vote_row[i];
        if (Bv->vote_row[i] > Bv->ky) {
            /* new max of votes in y */
            Bv->ky = Bv->vote_row[i];
            Bv->grid.y = (i+1) % 8;
        }
    }

    int w = Bv->coord_b.x - Bv->coord_a.x + 1;
    int h = Bv->coord_b.y - Bv->coord_a.y + 1;

    Bv->lnfa.x = log_nfa((int)(w*h/16), Bv->kx/2, (double)Bv->nx/(w*h), logNT);
    Bv->lnfa.y = log_nfa((int)(w*h/16), Bv->ky/2, (double)Bv->ny/(w*h), logNT);
}

/**
 *   Performs the voting process of a blockvote
 */
void vote(blockvote *Bv, double *cross_diff,
         int X, int Y, double logNT) {
    int x, y, i;
    for (i=0; i<8; i++)
        Bv->vote_col[i] = Bv->vote_row[i] = 0;

    for (x=Bv->coord_a.x; x<=Bv->coord_b.x; x++) {
        for (y=Bv->coord_a.y; y<=Bv->coord_b.y; y++) {
            if( x>0 && x<(X-1) && cross_diff[x+y*X] > cross_diff[x-1+y*X]
                && cross_diff[x+y*X] > cross_diff[x+1+y*X] ) {
                Bv->vote_col[x%8] += 1;
            }
            if( y>0 && y<(Y-1) && cross_diff[x+y*X] > cross_diff[x+(y-1)*X]
                && cross_diff[x+y*X] > cross_diff[x+(y+1)*X] ) {
                Bv->vote_row[y%8] += 1;
            }
        }
    }
}

void print_results(blockvote *Bv, FILE *list_windows_file) {
    int i;

    fprintf(list_windows_file, "========================================"
            "========================================\n");
    fprintf(list_windows_file, "votes by columns: ");
    for (i=0; i<8; i++) fprintf(list_windows_file, "%d ", Bv->vote_col[i]);
    fprintf(list_windows_file, "\n");
    fprintf(list_windows_file, "votes by rows:    ");
    for (i=0; i<8; i++) fprintf(list_windows_file, "%d ", Bv->vote_row[i]);
    fprintf(list_windows_file, "\n");
    fprintf(list_windows_file, "nx %d kx %d nfa_x 10^%g\n",
            Bv->nx,Bv->kx, Bv->lnfa.x);
    fprintf(list_windows_file, "ny %d ky %d nfa_y 10^%g\n",
            Bv->ny,Bv->ky, Bv->lnfa.y);
    fprintf(list_windows_file, "block origin %d %d\n",
            Bv->grid.x, Bv->grid.y);
    if (Bv->lnfa.x < 0.0 && Bv->lnfa.y < 0.0)
        fprintf(list_windows_file, "JPEG grid found!\n");
    else
        fprintf(list_windows_file, "JPEG grid NOT found!\n");
    fprintf(list_windows_file, "========================================"
            "========================================\n\n");
}
