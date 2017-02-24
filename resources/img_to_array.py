#!/usr/bin/python
import os,sys
import Image

format = 'L'
file = "ffont.png"

image_name = os.path.splitext(file)[0]


def print_help():
    print("-f\n\tSet the format. One of '1','L','RGB'.'RGBA','CMYK','YCbCr'. \n\tIf none is set the input files format is used!")

def print_help():
    print("-if\n\tSet the input file.")

for i in range(1,len(sys.argv)):
    if '-h' in sys.argv[i]:
        print_help()
        exit(0)
    elif '-f' in sys.argv[i]:
        format = sys.argv[i + 1]
        i += 1
    elif '-if' in sys.argv[i]:
        file = sys.argv[i + 1]
        i += 1


img = Image.open(file)
img.transpose(Image.FLIP_TOP_BOTTOM)
if format is not '':
    img = img.convert(format)

width, height = img.size
channels = len(img.getbands())
imgsize = width*height*channels

sys.stdout.write("static const unsigned int {}_width = {};\n".format(image_name,width))
sys.stdout.write("static const unsigned int {}_height = {};\n".format(image_name,height))
sys.stdout.write("static const unsigned int {}_channels = {};\n".format(image_name,channels))
sys.stdout.write("static const unsigned int {}_size = {};\n".format(image_name,imgsize))

first = True
for p in img.getdata():
    if channels is 1:
            if (first):
                sys.stdout.write("static const unsigned char {}_data[] = {{ \n{}".format(image_name,p))
                first = False
            else:
                sys.stdout.write(",{}".format(p))
    else:
        for c in range(0,channels):
            if(first):
                sys.stdout.write("static const unsigned char[] {}_data = ,{}".format(image_name,p[c]))
                first = False
            else:
                sys.stdout.write(",{}".format(p[c]))
sys.stdout.write("};\n")
