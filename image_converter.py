import sys
from PIL import Image

files = sys.argv[1:len(sys.argv)]
print("Frame Count: {}".format(len(files)))
data = []

for f in files:
    im = Image.open(f)  # Can be many different formats.
    pix = im.load()

    if im.size != (8, 8):
        exit("Image ({}) with incorrect dimensions".format(f))

    for y in range(0, 8):
        for x in range(0, 8):
            for c in range(0, 3):
                data.append(pix[x, y][c])
# make file
with open("img.dat", "wb") as out:
    # write to file
    pixelBytes = bytearray(data)
    out.write(pixelBytes)
