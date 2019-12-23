#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import sys
from PIL import Image

im = Image.open(sys.argv[1])
width, height = im.size

print('<?xml version="1.0" standalone="no"?>')
print('<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">')
print('<svg width="{}px" height="{}px" version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">'.format(width, height))

print('<image x="0" y="0" width="{}" height="{}" xlink:href="{}"> </image>'.format(width, height, sys.argv[1]))

for line in open(sys.argv[2], 'r').readlines():
    a,b,c,d = map(int, line.split(','))
    print('<rect x="{}" y="{}" width="{}" height="{}" fill="blue" fill-opacity="0.2"></rect>'.format(a,b,c-a,d-b))

for line in open(sys.argv[3], 'r').readlines():
    a,b,c,d = map(int, line.split(','))
    print('<rect x="{}" y="{}" width="{}" height="{}" fill="red" fill-opacity="0.2"></rect>'.format(a,b,c-a,d-b))

print('</svg>')
