
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
	//Do Parallel
	if(*argv[3]=='p' || *argv[3]=='P') {

		BMP* Background = new BMP(); 
		Background->ReadFromFile(argv[1]); 

		Matrix* mat = Kernels::genMatrix(5,"gaussian_blur");

		start = omp_get_wtime();
		BMP* Output = Matrix::convolution_parallel(mat,Background,4);
		end = omp_get_wtime();

		delete mat;
		  
		Output->WriteToFile(argv[2]);
		cout<<"Exection Time (Parallel) : "<<(end-start)<<endl;

	}
	//Do Serial
    else if(*argv[3]=='s' || *argv[3]=='S'){

		BMP* Background = new BMP(); 
		Background->ReadFromFile(argv[1]); 
		
		Matrix* mat = Kernels::genMatrix(5,"gaussian_blur");

		start = omp_get_wtime();
		BMP* Output = Matrix::convolution(mat,Background);
		end = omp_get_wtime();

		delete mat;
		  
		Output->WriteToFile(argv[2]);
		cout<<"Exection Time (Serial) : "<<(end-start)<<endl;

	}


return 0; 
} 