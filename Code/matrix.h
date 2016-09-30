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
  static void kernel3(Matrix* matrix, BMP* source, BMP* output, int x, int y);
  static void kernel2(int mat[][7], int div, int width, BMP* source, BMP* output, int x, int y);
  static BMP* convolution_parallel(Matrix* matrix, BMP* source, int n);
  static BMP* convolution(Matrix* matrix, BMP* source);
  static void edge_extrapolate_source(BMP* source);
  static RGBApixel* edge_extrapolate_pixel(BMP* source, int x, int y);
};

#endif
