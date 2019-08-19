import sys
from PIL import Image

im = Image.open(sys.argv[2])
width, height = im.size

print('<svg width="{}" height="{}">'.format(width, height))
print('<image x="0" y="0" width="{}" height="{}" xlink:href="{}"> </image>'.format(width, height, sys.argv[2]))

for line in open(sys.argv[1], 'r').readlines():
    a,b,c,d = map(int, line.split(','))
    print('<rect x="{}" y="{}" width="{}" height="{}" fill="blue" fill-opacity="0.2"></rect>'.format(a,b,c-a,d-b))
print('</svg>')
