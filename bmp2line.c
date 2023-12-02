
void RGB_BMP2line45290(uint16_t xstart, uint16_t xend, uint16_t ystart, uint16_t yend, uint8_t red, uint8_t green, uint8_t blue, BMP2image_t *image) {  
  int32_t dx, dy, x, y;
  x = xstart;
  y = ystart;
  double invgrad, invgrad1, invgrad2, diff1, diff2;
  if (yend == ystart) return;
  invgrad = (double)(xend - xstart)/(yend - ystart);
  // dx in {0,1}, dy = 1
  while ((y < yend) && (x < image->width) && (y < image->height)) {
    invgrad1 = (double)(x - xstart)/(y + 1 - ystart);
    invgrad2 = (double)(x + 1 - xstart)/(y + 1 - ystart);
    diff1 = fabs(invgrad1 - invgrad);
    diff2 = fabs(invgrad2 - invgrad);
    if (diff1 < diff2) {
      dx = 0;
      dy = 1;
    } else {
      dx = 1;
      dy = 1;
    }
    x += dx;
    y += dy;
    RGB_BMP2plot(x, y, red, green, blue, image);
  }
}

void RGB_BMP2linem452m90(uint16_t xstart, uint16_t xend, uint16_t ystart, uint16_t yend, uint8_t red, uint8_t green, uint8_t blue, BMP2image_t *image) {  
  int32_t dx, dy, x, y;
  x = xstart;
  y = ystart;
  double invgrad, invgrad1, invgrad2, diff1, diff2;
  if (yend == ystart) return;
  invgrad = (double)(xend - xstart)/(yend - ystart);
  // dx in {0,1}, dy = -1;
  while ((y > yend) && (x < image->width) && (y < image->height)) {
    invgrad1 = (double)(x - xstart)/(y - 1 - ystart);
    invgrad2 = (double)(x + 1 - xstart)/(y - 1 - ystart);
    diff1 = fabs(invgrad1 - invgrad);
    diff2 = fabs(invgrad2 - invgrad);
    if (diff1 < diff2) {
      dx = 0;
      dy = -1;
    } else {
      dx = 1;
      dy = -1;
    }
    x += dx;
    y += dy;
    RGB_BMP2plot(x, y, red, green, blue, image);
  }
}

void RGB_BMP2line0245(uint16_t xstart, uint16_t xend, uint16_t ystart, uint16_t yend, uint8_t red, uint8_t green, uint8_t blue, BMP2image_t *image) {  
  int32_t dx, dy, x, y;
  x = xstart;
  y = ystart;
  double grad, grad2, grad3, diff2, diff3;
  if (xend == xstart) return;
  grad = (double)(yend - ystart)/(xend - xstart);
  // dx = 1, dy in {0,1}
  while (((x < xend) || (y < yend)) && (x < image->width) && (y < image->height)) {
    grad2 = (double)(y + 1 - ystart)/(x + 1 - xstart);
    grad3 = (double)(y - ystart)/(x + 1 - xstart);
    diff2 = fabs(grad2 - grad);
    diff3 = fabs(grad3 - grad);
    if (diff2 < diff3) {
      dx = 1;
      dy = 1;
    } else {
      dx = 1;
      dy = 0;
    }
    x += dx;
    y += dy;
    RGB_BMP2plot(x, y, red, green, blue, image);
  }
}

void RGB_BMP2line02m45(uint16_t xstart, uint16_t xend, uint16_t ystart, uint16_t yend, uint8_t red, uint8_t green, uint8_t blue, BMP2image_t *image) {  
  int32_t dx, dy, x, y;
  x = xstart;
  y = ystart;
  double grad, grad2, grad3, diff2, diff3;
  if (xend == xstart) return;
  grad = (double)(yend - ystart)/(xend - xstart);
  // dx in {0,1}, dy in {0,-1}
  // dx = 1, dy in {0,1}
  while (((x < xend) || (y < yend)) && (x < image->width) && (y < image->height)) {
    grad2 = (double)(y - 1 - ystart)/(x + 1 - xstart);
    grad3 = (double)(y - ystart)/(x + 1 - xstart);
    diff2 = fabs(grad2 - grad);
    diff3 = fabs(grad3 - grad);
    if (diff2 < diff3) {
      dx = 1;
      dy = -1;
    } else {
      dx = 1;
      dy = 0;
    }
    x += dx;
    y += dy;
    RGB_BMP2plot(x, y, red, green, blue, image);
  }
}

void RGB_BMP2line(uint16_t xstart, uint16_t xend, uint16_t ystart, uint16_t yend, uint8_t red, uint8_t green, uint8_t blue, BMP2image_t *image) {  
  int32_t x, y;
  x = xstart;
  y = ystart;
  if (xstart > xend) {
    xstart = xend;
    xend = x;
    ystart = yend;
    yend = y;
    x = xstart;
    y = ystart;    
  }
  // xstart <= xend
  RGB_BMP2plot(xstart, ystart, red, green, blue, image);
  if ((xend - xstart) <= (yend - ystart)) {
    RGB_BMP2line45290(xstart, xend, ystart, yend, red, green, blue, image);
  } else {
    if ((yend - ystart) >= 0) {
      RGB_BMP2line0245(xstart, xend, ystart, yend, red, green, blue, image);
    } else {
      if ((xend - xstart) < (ystart - yend)) {
        RGB_BMP2linem452m90(xstart, xend, ystart, yend, red, green, blue, image);
      } else {
        RGB_BMP2line02m45(xstart, xend, ystart, yend, red, green, blue, image);
      }
    }
  }
}

