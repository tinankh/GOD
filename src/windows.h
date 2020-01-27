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
