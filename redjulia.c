#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

// gcc redjulia.c -o redjulia.bin -O3 -Wall -march=native

typedef struct {
  uint64_t pixels_bytes;
  uint16_t height;
  uint16_t width;
  uint32_t hres;
  uint32_t vres;
  uint16_t bits_per_pixel;
  uint16_t rowpadding;
  uint8_t *pixels;
} BMP2image_t;

void little_endian32(unsigned char* arr, uint32_t num) {
  arr[0] = num & 0xff;
  arr[1] = (num >> 8) & 0xff;
  arr[2] = (num >> 16) & 0xff;
  arr[3] = (num >> 24) & 0xff;
}

void little_endian16(unsigned char* arr, uint16_t num) {
  arr[0] = num & 0xff;
  arr[1] = (num >> 8) & 0xff;
}
/*
void RGB_BMP2colour765(uint8_t *red, uint8_t *green, uint8_t *blue, uint16_t colour765) {  
  if (colour765 < 256) {
    *red = colour765;
    *green = 0;
    *blue = 0;
    return ;
  }
  if (colour765 < 511) {
    *red = 255;
    *green = colour765 - 255;
    *blue = 0;
    return ;
  }  
  *red = 255;
  *green = 255;
  *blue = colour765 - 510;
  return ;
}
*/
void RGB_BMP2colour765(uint8_t *red, uint8_t *green, uint8_t *blue, uint16_t colour765) {  
  // Red and black only
    *green = 0;
    *blue = 0;
    *red = 255;
  if (colour765 < 256) {
    *red = colour765;
    return ;
  }
  return ;
}


void RGB_BMP2plot(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue, BMP2image_t *image) {  
  uint32_t ix = 3*x + (3*image->width + image->rowpadding)*y;
  image->pixels[ix++] = blue;
  image->pixels[ix++] = green;
  image->pixels[ix] = red;
}

uint32_t julia_iterations(double creal, double cimag, double preal, double pimag, double oob) {
  // z_{n+1} = (z_n)^2 + c
  uint32_t iterations;
  uint32_t maxiterations = 255;
  double temp, tempreal, tempimag;
  tempreal = preal;
  tempimag = pimag;
  for (iterations=0; iterations<maxiterations; iterations++) {
    if (fabs(tempreal - preal) > oob) return iterations;
    if (fabs(tempimag - pimag) > oob) return iterations;
    temp = tempreal*tempreal - tempimag*tempimag + creal;
    tempimag = 2*tempreal*tempimag + cimag;
    tempreal = temp;
  }
  return iterations;
}

void RGB_BMP2julia(double creal, double cimag, double xstart, double xend, double ystart, BMP2image_t *image) {  
  double oob, preal, pimag, yend = ystart + image->height*(xend - xstart)/image->width;
  uint64_t x, y;
  uint32_t iterations;
  uint8_t red, green, blue;
  oob = 30;
  for (y = 0; y < image->height; y++) {
    for (x = 0; x < image->width; x++) {
      preal = xstart + (xend - xstart)*x/(image->width - 1);
      pimag = ystart + (yend - ystart)*y/(image->height - 1);
      iterations = julia_iterations(creal, cimag, preal, pimag, oob);
      RGB_BMP2colour765(&red, &green, &blue, iterations);
      RGB_BMP2plot(x, y, red, green, blue, image);
    }
  }
}

uint32_t RGB_BMP2create(uint16_t height, uint16_t width, uint32_t hres, uint32_t vres, uint8_t bgred, uint8_t bggreen, uint8_t bgblue, BMP2image_t *image) {  
  if (image == NULL) return 1;
  image->rowpadding = ((3*width) % 4);
  if (image->rowpadding > 0) image->rowpadding = 4 - image->rowpadding;
  image->pixels_bytes = (uint64_t)(3*width + image->rowpadding)*height;
  image->pixels = malloc(image->pixels_bytes);
  image->height = height;
  image->width = width;
  image->hres = hres;
  image->vres = vres;
  image->bits_per_pixel = 24;
  uint64_t i, j, rowstart = 0;
  for (j = 0; j < height; j++) {
    for (i = 0; i < 3*width; ) {
      image->pixels[rowstart + i++] = bgblue;
      image->pixels[rowstart + i++] = bggreen;
      image->pixels[rowstart + i++] = bgred;
    }
    i+=image->rowpadding;
    rowstart += i;
  }
  return 0;
}

uint32_t BMP2save(BMP2image_t *image, char *filename) {
  uint8_t headers[52];
  little_endian32(headers, 54 + image->pixels_bytes);
  little_endian32(headers + 4, 0);
  little_endian32(headers + 8, 54);
  little_endian32(headers + 12, 40);
  little_endian32(headers + 16, image->width);
  little_endian32(headers + 20, image->height);
  little_endian16(headers + 24, 1);
  little_endian16(headers + 26, image->bits_per_pixel);
  little_endian32(headers + 28, 0);
  little_endian32(headers + 32, image->pixels_bytes);
  little_endian32(headers + 36, image->hres);
  little_endian32(headers + 40, image->vres);
  little_endian32(headers + 44, 0);
  little_endian32(headers + 48, 0);
  size_t bytes_written;
  FILE *fp;
  if (filename == NULL) return 1;
  if (image == NULL) return 1;
  fp = fopen(filename, "wb");
  if (fp == NULL) return 1;
  bytes_written = fwrite("BM", 1, 2, fp);
  bytes_written = fwrite((char*)headers, 1, 52, fp);
  bytes_written = fwrite((char*)image->pixels, 1, image->pixels_bytes, fp);
  fclose(fp);
  return 0;
}

int main(int argc, char **argv){
  BMP2image_t myimage;
  uint16_t height = 900;
  uint16_t width = 1600;
  uint32_t hres = 250;
  uint32_t vres = 250;
  uint8_t bgred = 255;
  uint8_t bggreen = 0;
  uint8_t bgblue = 0;
  double creal, cimag, xstart, xend, ystart;
  creal = 0.33;
  cimag = 0.4;
  xstart = -2.1;
  xend = 2.1;
  ystart = -1.2;
  if (argc < 2) {
    printf("This program generates an image file juliaset.bmp based on input parameters.\n");
    printf("Usage %s creal cimag xstart xend ystart height width\n", argv[0]);
    printf("e.g. %s .33 .4 -2.1 2.1 -1.2 900 1600\n", argv[0]);
    printf("e.g. %s .33 .4 -.3 .4 -1 900 1600\n", argv[0]);
    exit(0);
  }
  if (argc >= 2) creal = atof(argv[1]);
  if (argc >= 3) cimag = atof(argv[2]);
  if (argc >= 4) xstart = atof(argv[3]);
  if (argc >= 5) xend = atof(argv[4]);
  if (argc >= 6) ystart = atof(argv[5]);
  if (argc >= 7) height = atoi(argv[6]);
  if (argc >= 8) width = atoi(argv[7]);
  RGB_BMP2create(height, width, hres, vres, bgred, bggreen, bgblue, &myimage); 
  RGB_BMP2julia(creal, cimag, xstart, xend, ystart, &myimage);
  BMP2save(&myimage, "juliaset.bmp");
}
