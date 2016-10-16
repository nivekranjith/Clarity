/*
 * This file contains the main() function which is executed.
 */
#include "EasyBMP/EasyBMP.h"
#include "matrix.h"
#include "kernels.h"
#include "omp.h"
#include <string.h>
using namespace std;

//Note that BMP* deletes itself at the end of scope.
void images_test(int argc, char* argv[]) {
  double start,end;
  double time;
  //Loop 10 times for accuracy
  int loop=10; 
  //Do Parallel
  if(*argv[3]=='p' || *argv[3]=='P') {
    BMP* Background = new BMP(); 
    BMP* Output;
    Background->ReadFromFile(argv[1]); 

    Matrix* mat = Kernels::genMatrix(5,"gaussian_blur");

    for(int i=0;i<loop;i++)
    {
      start = omp_get_wtime();
      Output = Matrix::convolution_parallel(mat,Background,4);
      end = omp_get_wtime();

      time +=(end-start);
      cout<<i <<" : "<<time <<"(+"<<end-start<<")"<<endl;
    }

    delete mat;
		  
    Output->WriteToFile(argv[2]);
    cout<<"Execution Time (Parallel) : "<<(time/loop)<<endl;

  }
  //Do Serial
  else if(*argv[3]=='s' || *argv[3]=='S'){

    BMP* Background = new BMP(); 
    BMP* Output;
    Background->ReadFromFile(argv[1]); 
		
    Matrix* mat = Kernels::genMatrix(5,"gaussian_blur");

    for(int i=0;i<loop;i++)
      {
	start = omp_get_wtime();
	Output = Matrix::convolution(mat,Background);
	end = omp_get_wtime();

	time+=(end-start);
	cout<<i <<" : "<<time <<"(+"<<end-start<<")"<<endl;
      }

    delete mat;
		  
    Output->WriteToFile(argv[2]);
    cout<<"Execution Time (Serial) : "<<(time/loop)<<endl;
  }
}

void thread_test(int argc, char* argv[]) {
  double start,end;
  double time;
  //Loop 10 times for accuracy
  int loop=10;
  int threads_max=24;
  //Do Parallel
  BMP* Background = new BMP(); 
  BMP* Output;
  Background->ReadFromFile(argv[1]); 

  Matrix* mat = Kernels::genMatrix(5,"gaussian_blur");

  //"0" threads is serial implementation
  for (int j=0; j<loop; j++) {
    start = omp_get_wtime();
    Output = Matrix::convolution(mat,Background);
    end = omp_get_wtime();

    time = (end-start);
    cout << 0 << ' ' << time << endl;
  }

  //Parallel implementation
  for(int i=1;i<threads_max;i++)
  {
    for (int j=0; j<loop; j++) {
      start = omp_get_wtime();
      Output = Matrix::convolution_parallel(mat,Background,i);
      end = omp_get_wtime();

      time = (end-start);
          cout << i << ' ' << time << endl;
    }
  }

  delete mat;
		  
  //Output->WriteToFile(argv[2]);
  //cout<<"Execution Time (Parallel) : "<<(time/loop)<<endl;
}

//Arguments to the executable are described in section 3 of the report.
int main( int argc, char* argv[] ) 
{ 	
  //images_test(argc,argv);
  thread_test(argc,argv);
  return 0; 
} 
