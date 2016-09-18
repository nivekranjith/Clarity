#include "EasyBMP/EasyBMP.h"
#include "matrix.h"

Matrix(int** matrix_in, int width, int height) {}
~Matrix() {}

static RGBAPixel*  Matrix::kernel(Matrix* matrix, BMP* source, int x, int y) {}

static BMP* Matrix::convolution(Matrix* matrix, BMP* source) {
  //Make output canvas
  BMP output;

  //Get dimensions fom source
  int picWidth = source.TellWidth(); 
  int picHeight = source.TellHeight(); 

  source.SetSize( picWidth , picHeight); 
  //set outputs bit depth to 24 since we're using RGB 8bit+8bit+8bit=24bit
  output.SetBitDepth(24); 

   for (int i = 0; i < picWidth-1; ++i)
   {
     for (int j = 0; j < picHeight-1; ++j) 
     { 
        //Get RGBA value from kernel function
        RGBAPixel kernelReturn;
        kernelReturn = kernel(matrix,source,i,j);

        output(i,j)->Red = kernelReturn->Red;
        output(i,j)->Green = kernelReturn->Green;
        output(i,j)->Blue = kernelReturn->Blue;

     } 

   } //end for
 
    //Return the final BMP

   return output;
   //Output.WriteToFile("output.BMP"); 


}

static void Matrix::edge_extrapolate_source(BMP* source) {}
static RGBAPixel* Matrix::edge_extrapolate_pixel(BMP* source, int x, int y) {
  if ((x>=0)&&(x<source->tellWidth())&&(y>=0)&&(y<source->tellHeight())) {
    return source->(x,y);
  }
  else {
    //On the left
    if (x<0) {
      //Top-left.
      if (y<0) {
	return source->(0,0);
      }
      //Bottom-left
      if (y>source->tellHeight()) {
	return source->(0,source->tellHeight()-1);
      }
      //Left side somewhere
      return source->(0,y);
    }
    //On the right
    if (x>source->tellWidth()) {
      //Top-right
      if (y<0) {
	return source(source->tellWidth()-1,0);
      }
      //Bottom-right
      if (y>source->tellHeight()) {
	return source(source->tellWidth()-1,source->tellHeight()-1);
      }
      //Right side somewhere
      return source(tellWidth()-1,y)
    }
    //On the top
    if (y<0) {
      //Can only be on the top somewhere
      return source->(x,0);
    }
    //Can only be on the bottom somewhere
    return source->(x,source->tellHeight()-1);
  }
  return nullptr;
}

#endif
