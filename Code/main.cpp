#include "EasyBMP/EasyBMP.h"
#include "matrix.h"
#include "kernels.h"
#include <string.h>
using namespace std;

//Note that BMP's apparently automatically delete.
int main( int argc, char* argv[] ) 
{ 
  BMP* Background = new BMP(); 
  Background->ReadFromFile(argv[1]); 

  Matrix* mat = Kernels::genMatrix(5,"gaussian_blur");
  BMP* Output = Matrix::convolution_parallel(mat,Background,4);
  delete mat;
    
  Output->WriteToFile(argv[2]);
return 0; 
} 
