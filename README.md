GOD
===

Version 1 - December 2017
Automatic JPEG Grid Detection with Controlled False Alarms, and Its
Image Forensic Applications

Version 2 - July 2019
Local JPEG Grid Detector via Blocking Artifacts


by rafael grompone von gioi <grompone@gmail.com>
and tina nikoukhah <tina.nikoukhah@gmail.com>,


Introduction
------------

This program implements a local a-contrario detector of the JPEG block origin.


Requirements
------------

The following libraries are required for image input/output:

  - libpng
  - LibTIFF
  - libjpeg


Files
-----

- src/main.c: Main code.

- src/acontrario.c and include/acontrario.h: A contrario formulation.

- src/jpegblocks.c and include/jpegblocks.h: Functions used for the voting process.

- src/misc.c and include/misc.h: Useful functions.

- README.md: this file.

- LICENSE: GNU AFFERO GENERAL PUBLIC LICENSE Version 3.

- Makefile: Compilation instructions.

- src/iio.c and include/iio.h: [iio](https://github.com/mnhrdt/iio) code and header.

- *.{ppm,pgm}: Test images.

- create_svg.py: Creates final masks from txt files to svg files.


Compiling
---------

The compiling instruction is

  make

To verify a correct compilation you can apply the algorithm to the test images
'pelican.ppm' and 'roma.pgm'. This can be done by executing:

  make test

This should print the following message:

test on pelican.ppm
===================
./bin/main pelican.ppm 128 > temp
cat global_result.txt

============================================================================================
number of blocks: 100
       0/0        0/0        0/0        0/0        0/0        0/0       99/100        0/0
       0/0        0/0        0/0        0/0        0/0        0/0        0/0        0/0
       0/0        0/0        0/0        0/0        0/0        0/0        0/0        0/0
       0/0        0/0        0/0        0/0        0/0        0/0        0/0        0/0
       0/0        0/0        0/0        0/0        0/0        0/0        0/0        0/0
       0/0        0/0        0/0        0/0        0/0        0/0        0/0        0/0
       0/0        0/0        0/0        0/0        0/0        0/0        0/0        0/0
       0/0        0/0        0/0        0/0        0/0        0/0        0/0        0/0
============================================================================================
worst meaningful NFA = 10^-1.73232
best NFA = 10^-213.367

mean NFA = 10^-47.9746

detected grid origin 6 0


test on roma.pgm
================
./bin/main roma.pgm 128 >> temp
cat global_result.txt

============================================================================================
number of blocks: 100
       0/0        0/3        0/0        0/2        0/1        0/0        0/1        0/1
       0/0        0/0        0/0        0/0        0/1        0/0        0/0        0/3
       0/0        0/0        0/0        0/0        0/2        0/2        0/2        0/0
       0/0        0/0        0/0        0/0        0/1        0/0        0/0        0/2
       0/0        0/0        0/0        0/0        0/5        0/0        0/1        0/3
       0/0        0/0        0/6        0/1        0/16        0/4        0/4        0/12
       0/0        0/2        0/0        0/0        0/1        0/0        0/0        0/7
       0/0        0/0        0/1        0/0        0/0        0/1        0/0        0/15
============================================================================================
best NFA = 10^1.91207

mean NFA = 10^2.31863

grid origin NOT found


Running
-------

The program is executed as:

    bin/main

(use ./bin/main if the command is not included in the current path).
That should print the following usage:

  Error: use: main <image> <block_size>

The command takes an input image as argument. Any image file format handled by
Enric Meinhardt-Llopis' IIO library can be used. This includes, for example,
PNG, JPG, TIFF, PPM, PGM.

A typical execution is as follows:

  ./bin/main pelican.ppm 128


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
