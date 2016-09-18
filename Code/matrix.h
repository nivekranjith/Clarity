#ifndef MATRIX
#define MATRIX

#include "EasyBMP/EasyBMP.h"

class Matrix {
 public:
  Matrix(int** matrix_in, int wid, int hei, int divisor);
  ~Matrix();
  int width;
  int height;
  int** mat;
  int divisor;

  static RGBApixel* kernel(Matrix* matrix, BMP* source, int x, int y);
  static BMP* convolution(Matrix* matrix, BMP* source);
  static void edge_extrapolate_source(BMP* source);
  static RGBApixel* edge_extrapolate_pixel(BMP* source, int x, int y);
};

#endif
