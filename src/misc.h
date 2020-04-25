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

#ifndef MISC_H
#define MISC_H

#include <stdlib.h>

void error(char * msg, ...);

void * xcalloc(size_t n_items, size_t size);

void image2grayscale(double *input, double *image, int X, int Y, int C);

void rgb2y(double *input, double *image, int X, int Y, int C);

#endif
