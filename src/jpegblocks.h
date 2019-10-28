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

typedef struct _blockvote {
    int vote_col[8];
    int vote_row[8];
    int nx, ny, kx, ky;
    couple lnfa;
    coord grid;
    coord coord_a; // top left
    coord coord_b; // bottom right
    coord size;
    int meaningful;
} blockvote;

void cross_difference(double *image, double *cross_diff, int X, int Y);

void compute_NFA(blockvote *Bv, double logNT);

void vote(blockvote *Bv, double *cross_diff, int X, int Y, double logNT);

void print_results(blockvote *Bv, FILE *list_blocks_file);

#endif
