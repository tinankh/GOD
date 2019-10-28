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
#include "misc.h"

/* LN10 */
#ifndef M_LN10
#define M_LN10      2.30258509299404568401799145468436421
#endif /* !M_LN10 */

/**
 * @brief   Computes the logarithm of NFA to base 10, NFA = NT.b(n,k,p)
 * @param       n total number of votes
 * @param       k votes for best origin coordinate
 * @param       p probability
 * @param       logNT    logarithm of Number of Tests
 * @return      log10(NFA)
 */

#define TABSIZE 100000

double log_nfa(int n, int k, double p, double logNT) {
    static double inv[TABSIZE];  /* table to keep computed inverse values */
    double tolerance = 0.1;      /* an error of 10% in the result is accepted */
    double log1term,term,bin_term,mult_term,bin_tail,err;
    double p_term = p / (1.0-p);
    int i;

    if (n<0 || k<0 || k>n || p<0.0 || p>1.0) {
        printf("wrong n(%i), k(%i) or p(%f) values in nfa()\n", n, k, p);
        error("wrong n(%i), k(%i) or p(%f) values in nfa()");
    }

    if (n==0 || k==0) return logNT;
    if (n==k) return logNT + (double)n * log10(p);

    log1term = lgamma((double)n+1.0) - lgamma((double)k+1.0)
        - lgamma((double)(n-k)+1.0)
        + (double)k * log(p) + (double)(n-k) * log(1.0-p);

    term = exp(log1term);
    if (term == 0.0) {                       /* the first term is almost zero */
        if ((double)k > (double)n * p)    /* at beginning or end of the tail? */
            return log1term / M_LN10 + logNT; /* end: use just the first term */
        else
            return logNT;                     /* begin: the tail is roughly 1 */
    }

    bin_tail = term;
    for (i=k+1; i<=n; i++) {
        bin_term = (double)(n-i+1) * ( i<TABSIZE ?
                                       (inv[i]?inv[i] : (inv[i]=1.0/(double)i))
                                       : 1.0/(double)i );
        mult_term = bin_term * p_term;
        term *= mult_term;
        bin_tail += term;
        if (bin_term<1.0) {
            /* when bin_term<1 then mult_term_j<mult_term_i for j>i.
               then, the error on the binomial tail when truncated at
               the i term can be bounded by a geometric serie of form
               term_i * sum mult_term_i^j.                            */
            err = term * ( ( 1.0 - pow( mult_term, (double)(n-i+1) ) ) /
                           (1.0-mult_term) - 1.0 );

            /* one wants an error at most of tolerance*final_result, or:
               tolerance * abs(-log10(bin_tail)-logNT).
               now, the error that can be accepted on bin_tail is
               given by tolerance*final_result divided by the derivative
               of -log10(x) when x=bin_tail. that is:
               tolerance * abs(-log10(bin_tail)-logNT) / (1/bin_tail)
               finally, we truncate the tail if the error is less than:
               tolerance * abs(-log10(bin_tail)-logNT) * bin_tail        */
            if (err < tolerance * fabs(-log10(bin_tail)-logNT) * bin_tail)
                break;
        }
    }
    return log10(bin_tail) + logNT;
}
