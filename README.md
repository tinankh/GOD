GOD
===

Version 1 - December 2017
Automatic JPEG Grid Detection with Controlled False Alarms, and Its
Image Forensic Applications

Version 2 - January 2020
Local JPEG Grid Detector via Blocking Artifacts


by Rafael Grompone von Gioi <grompone@gmail.com>
and Tina Nikoukhah <tinanikoukhah@gmail.com>


Introduction
------------

This program implements a local a-contrario detector of the JPEG block origin.


Online Demo
------------

[IPOL](https://ipolcore.ipol.im/demo/clientApp/demo.html?id=283)


Requirements
------------

The following libraries are required for image input/output:

  - libpng
  - LibTIFF
  - libjpeg


Files
-----

- src/god.c: Main code.

- src/acontrario.c and src/acontrario.h: A contrario formulation.

- src/windows.c and src/windows.h: Functions used for the voting process.

- src/misc.c and src/misc.h: Useful functions.

- README.md: this file.

- LICENSE: GNU AFFERO GENERAL PUBLIC LICENSE Version 3.

- Makefile: Compilation instructions.

- src/iio.c and src/iio.h: [iio](https://github.com/mnhrdt/iio) code and header.

- *.png: Test images.

- create_svg.py: Creates final masks from txt files to svg files. A typical execution is as follows:
```
python create_svg.py luminance.png list_windows.txt > <output.svg>
```


Compiling
---------

The compiling instruction is
```bash
  make
```
or if you want the code to be parallel
```bash
   make openmp
```
from the directory where the source codes and the Makefile are located.

To verify a correct compilation you can apply the algorithm to the test images
'pelican.png' and 'roma.png'. This can be done by executing:
```bash
  make test
```

This should print the following message:
```
test on pelican.png
===================
./god pelican.png 128
number of meaningful windows 35 of 36 (97 %)
number of windows (meaningful votes) for each JPEG grid origin:
      0       0       0       0       0       0      35       0
      0       0       0       0       0       0       0       0
      0       0       0       0       0       0       0       0
      0       0       0       0       0       0       0       0
      0       0       0       0       0       0       0       0
      0       0       0       0       0       0       0       0
      0       0       0       0       0       0       0       0
      0       0       0       0       0       0       0       0
best log NFA for each JPEG grid origin:
      -       -       -       -       -       -   -92.7       -
      -       -       -       -       -       -       -       -
      -       -       -       -       -       -       -       -
      -       -       -       -       -       -       -       -
      -       -       -       -       -       -       -       -
      -       -       -       -       -       -       -       -
      -       -       -       -       -       -       -       -
      -       -       -       -       -       -       -       -
number of different meaningful grids: 1
most meaningful grid origin 6 0 with NFA: 10^-92.6767

most meaningful grid different from (0,0), the image may have been cropped!

test on roma.png
================
./god roma.png 128
number of meaningful windows 0 of 36 (0 %)
number of windows (meaningful votes) for each JPEG grid origin:
      0       0       0       0       0       0       0       0
      0       0       0       0       0       0       0       0
      0       0       0       0       0       0       0       0
      0       0       0       0       0       0       0       0
      0       0       0       0       0       0       0       0
      0       0       0       0       0       0       0       0
      0       0       0       0       0       0       0       0
      0       0       0       0       0       0       0       0
best log NFA for each JPEG grid origin:
      -       -       -       -       -       -       -       -
      -       -       -       -       -       -       -       -
      -       -       -       -       -       -       -       -
      -       -       -       -       -       -       -       -
      -       -       -       -       -       -       -       -
      -       -       -       -       -       -       -       -
      -       -       -       -       -       -       -       -
      -       -       -       -       -       -       -       -
number of different meaningful grids: 0
no meaningful grid found

no suspicious traces found in the image with the performed analysis.
```

Running
-------

The program is executed as:
```
    god
```

(use ./god if the command is not included in the current path).
That should print the following usage:

```
  Error: use: god <image> <window_size>
```

The command takes an input image as argument. Any image file format handled by
Enric Meinhardt-Llopis' IIO library can be used. This includes, for example,
PNG, JPG, TIFF, PPM, PGM.

A typical execution is as follows:
```
  ./god pelican.png 128
```



Ouputs
------
- luminance.png: Luminance image.

- cross_diff.png: Cross-difference filter applied to the image.

- list_windows.txt: Attributes for each window.
Each line is composed of eight numbers separated by spaces, that are:
top left coordinate x of the window, top left coordinate y of the window,
bottom right x coordinate of the window, bottom right y coordinate of the window,
log NFA x, log NFA y, most significant grid x coordinate, most significant grid y coordinate.
For example, the line:
```
1 1 200 200 -407.534 -330.733 0 0 

```






Copyright and License
---------------------

Copyright (c) 2018-2019 Rafael Grompone von Gioi grompone@gmail.com
Copyright (c) 2018-2019 Tina Nikoukhah tina.nikoukhah@gmail.com

GOD is free software: you can redistribute it and/or modify it under
the terms of the GNU Affero General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

GOD is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public
License for more details.

You should have received a copy of the GNU Affero General Public
License along with this program. If not, see
https://www.gnu.org/licenses/.

Thanks
------

We would be grateful to receive any comment, especially about errors, bugs,
or strange results.
