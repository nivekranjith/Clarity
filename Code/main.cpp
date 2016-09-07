#include "EasyBMP/EasyBMP.h"
#include "matrix.h"
using namespace std; 
int main( int argc, char* argv[] ) 
{ 
  BMP Background; 
  Background.ReadFromFile(argv[1]); 

  BMP Output; 
  Output.SetSize( Background.TellWidth() , Background.TellHeight() );

  //Convolution of Background to Output
  
  Output.WriteToFile(argv[2]); 
return 0; 
} 
