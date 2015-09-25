import sys
from PIL import Image

if len(sys.argv) != 2:
	exit("No file specified")

filename = sys.argv[1]

im = Image.open(filename) #Can be many different formats.
pix = im.load()

if im.size != (8, 8):
	exit("Image with incorrect Dimensions")

print(pix[0,0]) #Get the RGBA Value of the a pixel of an image
# pix[x,y] = value # Set the RGBA Value of the image (tuple)