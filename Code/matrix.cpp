#include "EasyBMP/EasyBMP.h"
#include "matrix.h"
#include <omp.h>

Matrix::Matrix(int **matrix_in, int wid, int hei, int div) {
  mat = matrix_in;
  width = wid;
  height = hei;
  divisor = div;
}

Matrix::~Matrix() {
  for (int i=0; i<width; i++) {
    delete mat[i];
  }
  delete mat;
}

//From Mark Ransom at http://codereview.stackexchange.com/questions/6502/fastest-way-to-clamp-an-integer-to-the-range-0-255
inline
int fastClamp(int n)
{
    n &= -(n >= 0);
    return n | ((255 - n) >> 31);
}

void Matrix::kernel2(int mat[][7], int div, int width, BMP* source, BMP* output, int x, int y) {
  //Need to use temporary variables because result of kernel may exceed 255 or go below 0.
  int Red = 0;
  int Green = 0;
  int Blue = 0;
  int border = (width-1)/2;
  for(int i = -border; i<=border; i++) {
    for(int j = -border; j<=border; j++) {
      Red = Red + Matrix::edge_extrapolate_pixel(source,x+i,y+j)->Red * mat[i+border][j+border];
      Blue = Blue + Matrix::edge_extrapolate_pixel(source,x+i,y+j)->Blue * mat[i+border][j+border];
      Green = Green + Matrix::edge_extrapolate_pixel(source,x+i,y+j)->Green * mat[i+border][j+border];
    }
  }

  (*output)(x,y)->Red = Red/div; //divide by divisor
  (*output)(x,y)->Blue = Blue/div;
  (*output)(x,y)->Green = Green/div;
}

RGBApixel* Matrix::kernel(Matrix* matrix, BMP* source, int x, int y) {
  RGBApixel* out = new RGBApixel();
  //Need to use temporary variables because result of kernel may exceed 255 or go below 0.
  int Red = 0;
  int Green = 0;
  int Blue = 0;
  int border = (matrix->width-1)/2;
  for(int i = -border; i<=border; i++) {
    for(int j = -border; j<=border; j++) {
      Red = Red + Matrix::edge_extrapolate_pixel(source,x+i,y+j)->Red * matrix->mat[i+border][j+border];
      Blue = Blue + Matrix::edge_extrapolate_pixel(source,x+i,y+j)->Blue * matrix->mat[i+border][j+border];
      Green = Green + Matrix::edge_extrapolate_pixel(source,x+i,y+j)->Green * matrix->mat[i+border][j+border];
    }
  }

  out->Red = fastClamp(Red/matrix->divisor); //divide by divisor
  out->Blue = fastClamp(Blue/matrix->divisor);
  out->Green = fastClamp(Green/matrix->divisor);
  //Dummy for now
  out ;
}

void Matrix::kernel3(Matrix* matrix, BMP* source, BMP* output, int x, int y) {
  //Need to use temporary variables because result of kernel may exceed 255 or go below 0.
  int Red = 0;
  int Green = 0;
  int Blue = 0;
  int border = (matrix->width-1)/2;
  for(int i = -border; i<=border; i++) {
    for(int j = -border; j<=border; j++) {
      Red = Red + Matrix::edge_extrapolate_pixel(source,x+i,y+j)->Red * matrix->mat[i+border][j+border];
      Blue = Blue + Matrix::edge_extrapolate_pixel(source,x+i,y+j)->Blue * matrix->mat[i+border][j+border];
      Green = Green + Matrix::edge_extrapolate_pixel(source,x+i,y+j)->Green * matrix->mat[i+border][j+border];
    }
  }

  (*output)(x,y)->Red = fastClamp(Red/matrix->divisor); //divide by divisor
  (*output)(x,y)->Blue = fastClamp(Blue/matrix->divisor);
  (*output)(x,y)->Green = fastClamp(Green/matrix->divisor);
}

void Matrix::edge_extrapolate_source(BMP* source) {}

//Liam says: I changed output to be a pointer to match definition
//If we return a normal BMP then it will have to clone the whole image
//Better to just pass a reference which is quick, no?
BMP* Matrix::convolution(Matrix* matrix, BMP* source) {
  //Make output canvas
  BMP* output = new BMP();

  //Get dimensions fom source
  int picWidth = source->TellWidth(); 
  int picHeight = source->TellHeight(); 

  output->SetSize( picWidth , picHeight); 
  //set outputs bit depth to 24 since we're using RGB 8bit+8bit+8bit=24bit
  output->SetBitDepth(24); 

   for (int i = 0; i < picWidth; ++i)
   {
     for (int j = 0; j < picHeight; ++j) 
     { 
        kernel3(matrix,source,output,i,j);
     } 
   } //end for
 
    //Return the final BMP

   return output;
   //Output.WriteToFile("output.BMP"); 

}

BMP* Matrix::convolution_parallel(Matrix* matrix, BMP* source, int n) {
  int tempmat[7][7];
  for (int l=0; l<matrix->width; l++) {
    for (int o=0; o<matrix->height; o++) {
      tempmat[l][o] = matrix->mat[l][o];
    }
  }
  //Make output canvas
  BMP* output = new BMP();

  //Get dimensions fom source
  int picWidth = source->TellWidth(); 
  int picHeight = source->TellHeight(); 

  output->SetSize(picWidth, picHeight); 
  //set outputs bit depth to 24 since we're using RGB 8bit+8bit+8bit=24bit
  output->SetBitDepth(24);

  int i, j, k, max;
  int width = matrix->width;
  int div = matrix->divisor;
  omp_set_num_threads(n);
#pragma omp parallel for schedule(dynamic,1), private(k,i,max), firstprivate(tempmat,width,div,picWidth), shared(source,output)
  for (j=0; j<picWidth/(width*2); j++) {
    if ((j+1)*width*2>picWidth) max = picWidth;
    else max = (j+1)*width*2;
    for (i=j*width*2; i<max; i++) {
    //j=((i%n)*(picWidth/n))+(i/n);
      for (k=0; k<picHeight; k++) {
	//if (k==0) printf("hi %d\n",i);
	kernel2(tempmat,div,width,source,output,i,k);
      }
    }
  }

  return output;
  //Output.WriteToFile("output.BMP"); 
}

RGBApixel* Matrix::edge_extrapolate_pixel(BMP* source, int x, int y) {
  if ((x>=0)&&(x<source->TellWidth())&&(y>=0)&&(y<source->TellHeight())) {
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
      if (y>=(*source).TellHeight()) {
	return (*source)(0,(*source).TellHeight()-1);
      }
      //Left side somewhere
      return (*source)(0,y);
    }
    //On the right
    if (x>=(*source).TellWidth()) {
      //Top-right
      if (y<0) {
	return (*source)(source->TellWidth()-1,0);
      }
      //Bottom-right
      if (y>=source->TellHeight()) {
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
