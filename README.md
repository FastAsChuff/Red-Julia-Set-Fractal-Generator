# Red-Julia-Set-Fractal-Generator
This program generates an image file juliaset.bmp based on input parameters.

Usage ./redjulia.bin creal cimag xstart xend ystart height width

e.g. ./redjulia.bin .33 .4 -2.1 2.1 -1.2 900 1600

e.g. ./redjulia.bin .33 .4 -.3 .4 -1 900 1600


creal and cimag are the real and imaginary components used in the z^2 + c equation use to generate Julia set fractals.

xstart and xend are the real horizontal limits of the image to be produced.

ystart is the imaginary limit at the bottom of the image to be produced. yend is calculated internally from the aspect ratio.

height and width are the vertical and horizontal pixel lengths of the image to be produced.
