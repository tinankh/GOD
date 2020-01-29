#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import sys
from PIL import Image
import numpy as np


im = Image.open(sys.argv[1])
width, height = im.size

print('<?xml version="1.0" standalone="no"?>')
print('<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">')
print('<svg width="{}px" height="{}px" version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">'.format(width, height))

print('<image x="0" y="0" width="{}" height="{}" xlink:href="{}"> </image>'.format(width, height, sys.argv[1]))

lnfa = []
zblue = []
zred = []

result_file = open(sys.argv[2], 'r').readlines()

# get most meaningful grid origin
for line in result_file:
    window = line.split(' ')
    lnfa.append(max(list(map(float,window[4:6]))))

meaningful = result_file[np.argmin(np.array(lnfa))]
gridx = meaningful.split(' ')[6]
gridy = meaningful.split(' ')[7]


# get list of significant (for foreign grids) and none significant windows
for line in result_file:
    window = line.split(' ')

    if (window[6] == '-1'):
        zblue.append(list(map(int, window[0:4])))

    else:
        if (window[6] != gridx or window[7] != gridy):
            zred.append(list(map(int, window[0:4])))

# draw blue windows
for z in zblue:
    print('<rect x="{}" y="{}" width="{}" height="{}" fill="blue" fill-opacity="0.2"></rect>'
          .format(z[0],z[1],z[2]-z[0],z[3]-z[1]))

# draw red windows over the blue windows
for z in zred:
    print('<rect x="{}" y="{}" width="{}" height="{}" fill="red" fill-opacity="0.2"></rect>'
          .format(z[0],z[1],z[2]-z[0],z[3]-z[1]))

print('</svg>')
