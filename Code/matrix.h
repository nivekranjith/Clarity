#ifndef MATRIX
#define MATRIX

#include "EasyBMP/EasyBMP.h"

class Matrix {
 public:
  Matrix(int** matrix_in, int width, int height);
  ~Matrix();
  int width;
  int height;
  int[][] matrix;

  static RGBAPixel* kernel(Matrix* matrix, BMP* source, int x, int y);
  static BMP* convolution(Matrix* matrix, BMP* source);
  static void edge_extrapolate_source(BMP* source);
  static BMP* edge_extrapolate_pixel(RGB* source, int x, int y);
};

#endif
