#include "EasyBMP/EasyBMP.h"
#include "matrix.h"
using namespace std;

//Note that BMP's apparently automatically delete.
int main( int argc, char* argv[] ) 
{ 
  BMP* Background = new BMP(); 
  Background->ReadFromFile(argv[1]); 

  //Create Matrix
  int rows = 5;
  int cols = 5;
  int** kern = new int*[rows];
  for(int i=0; i < rows; ++i) {
    kern[i] = new int[cols];
    for (int j=0; j < cols; ++j) {
      kern[i][j] = 0;
    }
  }
  //Let's try a "identity" matrix
  kern[0][0] = 1; kern[0][1] = 4; kern[0][2] = 6; kern[0][3] = 4; kern[0][4] = 1;
  kern[1][0] = 4; kern[1][1] = 16; kern[1][2] = 24; kern[1][3] = 16; kern[1][4] = 4;
  kern[2][0] = 6; kern[2][1] = 24; kern[2][2] = 36; kern[2][3] = 24; kern[2][4] = 6;
  kern[3][0] = 4; kern[1][1] = 16; kern[3][2] = 24; kern[1][3] = 16; kern[3][4] = 4;
  kern[4][0] = 1; kern[4][1] = 4; kern[4][2] = 6; kern[4][3] = 4; kern[4][4] = 1;
  //Convolution of Background to Output
  //Last parameter is divisor
  Matrix* mat = new Matrix(kern,rows,cols,256);
  BMP* Output = Matrix::convolution(mat,Background);
  delete mat;
    
  Output->WriteToFile(argv[2]);
return 0; 
} 
