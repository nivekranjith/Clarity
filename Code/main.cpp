#include "EasyBMP/EasyBMP.h"
#include "matrix.h"
using namespace std;

//Note that BMP's apparently automatically delete.
int main( int argc, char* argv[] ) 
{ 
  BMP* Background = new BMP(); 
  Background->ReadFromFile(argv[1]); 

  //Create Matrix
  int rows = 3;
  int cols = 3;
  int** kern = new int*[rows];
  for(int i=0; i < rows; ++i) {
    kern[i] = new int[cols];
    for (int j=0; j < cols; ++j) {
      kern[i][j] = 0;
    }
  }
  //Let's try a "identity" matrix
  kern[1][1] = 1;
 
  //Convolution of Background to Output
  Matrix* mat = new Matrix(kern,rows,cols);
  BMP* Output = Matrix::convolution(mat,Background);
  delete mat;
    
  Output->WriteToFile(argv[2]);
return 0; 
} 
