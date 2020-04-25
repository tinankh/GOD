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

#ifndef JPEGBLOCKS_H
#define JPEGBLOCKS_H

typedef struct coord {
    int x;
    int y;
} coord;

typedef struct couple {
    double x;
    double y;
} couple;

typedef struct window {
    int vote_col[8];
    int vote_row[8];
    int nx, ny, kx, ky;
    couple lnfa;
    coord grid;
    coord coord_a; // top left
    coord coord_b; // bottom right
} window;

void cross_difference(double *image, double *cross_diff, int X, int Y);

void compute_NFA(window *win, double logNT);

void vote(window *win, double *cross_diff, int X, int Y);

void summary(int *num_meaningful_windows, double *best_lnfa, int num_windows);

#endif
