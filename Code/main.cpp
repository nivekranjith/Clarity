#include "EasyBMP/EasyBMP.h"
#include "matrix.h"
#include "kernels.h"
#include "omp.h"
#include <string.h>
using namespace std;

//Note that BMP's apparently automatically delete.
int main( int argc, char* argv[] ) 
{ 
	double start,end;
	double time;
	int loop=10;


	if(*argv[3]=='p' || *argv[3]=='P') {

	  BMP* Background = new BMP(); 
	  BMP* Output;
	  Background->ReadFromFile(argv[1]); 

	  Matrix* mat = Kernels::genMatrix(3,"box_blur");

	  for(int i=0;i<loop;i++){
	  	start = omp_get_wtime();
	  	Output = Matrix::convolution_parallel(mat,Background,4);
	  	end = omp_get_wtime();

	  	time+=(end-start);
	  }

	  delete mat;
	    
	  Output->WriteToFile(argv[2]);
	  cout<<"Exection Time (Parallel) : "<<(time/loop)<<endl;

	}

    else if(*argv[3]=='s' || *argv[3]=='S'){

		BMP* Background = new BMP(); 
		BMP* Output;
		Background->ReadFromFile(argv[1]); 
		
		Matrix* mat = Kernels::genMatrix(3,"box_blur");

		for(int i=0;i<loop;i++){
			start = omp_get_wtime();
			Output = Matrix::convolution(mat,Background);
			end = omp_get_wtime();

			time+=(end-start);
			//cout<<i <<" : "<<time <<"(+"<<end-start<<")"<<endl;
		}



		delete mat;
		  
		Output->WriteToFile(argv[2]);
		cout<<"Exection Time (Serial) : "<<(time/loop)<<endl;
	}



return 0; 
} 