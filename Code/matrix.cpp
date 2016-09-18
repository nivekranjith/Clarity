#include "EasyBMP/EasyBMP.h"
#include "matrix.h"

Matrix::Matrix(int** matrix_in, int width, int height) {}
Matrix::~Matrix() {}

RGBApixel* Matrix::kernel(Matrix* matrix, BMP* source, int x, int y) {}
BMP* Matrix::convolution(Matrix* matrix, BMP* source) {}
void Matrix::edge_extrapolate_source(BMP* source) {}
RGBApixel* Matrix::edge_extrapolate_pixel(BMP* source, int x, int y) {
  if ((x>=0)&&(x<(*source).TellWidth())&&(y>=0)&&(y<(*source).TellHeight())) {
    return (*source)(x,y);
  }
  else {
    //On the left
    if (x<0) {
      //Top-left.
      if (y<0) {
	return (*source)(0,0);
      }
      //Bottom-left
      if (y>(*source).TellHeight()) {
	return (*source)(0,(*source).TellHeight()-1);
      }
      //Left side somewhere
      return (*source)(0,y);
    }
    //On the right
    if (x>(*source).TellWidth()) {
      //Top-right
      if (y<0) {
	return (*source)(source->TellWidth()-1,0);
      }
      //Bottom-right
      if (y>source->TellHeight()) {
	return (*source)(source->TellWidth()-1,source->TellHeight()-1);
      }
      //Right side somewhere
      return (*source)(source->TellWidth()-1,y);
    }
    //On the top
    if (y<0) {
      //Can only be on the top somewhere
      return (*source)(x,0);
    }
    //Can only be on the bottom somewhere
    return (*source)(x,source->TellHeight()-1);
  }
  return nullptr;
}
