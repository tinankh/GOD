GOD
===

Version 1 - December 2017
Automatic JPEG Grid Detection with Controlled False Alarms, and Its
Image Forensic Applications

Version 2 - July 2019
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

- src/jpegblocks.c and src/jpegblocks.h: Functions used for the voting process.

- src/misc.c and src/misc.h: Useful functions.

- README.md: this file.

- LICENSE: GNU AFFERO GENERAL PUBLIC LICENSE Version 3.

- Makefile: Compilation instructions.

- src/iio.c and src/iio.h: [iio](https://github.com/mnhrdt/iio) code and header.

- *.png: Test images.

- create_svg.py: Creates final masks from txt files to svg files. A typical execution is as follows:
```
python create_svg.py luminance.png nonmeaningful.txt meaningful_n0.txt > <output.svg>
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
