
#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>

#include "DMxV.h"


int DMxV(int argc, char *argv[]) {
//Options: InputMatrixFile InputVectorFile

	
	//int i;
	int ret_code;
	
	unsigned long *I;
	unsigned long *J;
	double *values;
	
	unsigned long M;
	unsigned long N;
	unsigned long long nz;
	
	
	double *vectorValues;
	unsigned long M_Vector;
	unsigned long N_Vector;
	unsigned long long nz_vector;
	
	int write2file = 0;
	
	if (argc < 3)
	{
		fprintf(stderr, "[%s] Usage: %s [input-matrix-file] [input-vector-file]\n",__func__, argv[0]);
		return 0;
	}
	
	if(argc == 4){
		write2file = 1;
	}
	
	//Read matrix
	if(!readDenseCoordinateMatrix(argv[1],&I,&J,&values,&M,&N,&nz)){
		fprintf(stderr, "[%s] Can not read Matrix\n",__func__);
		return 0;
	}
	
	//Read vector
	if(!readDenseVector(argv[2], &vectorValues,&M_Vector,&N_Vector,&nz_vector)){
		fprintf(stderr, "[%s] Can not read Vector\n",__func__);
		return 0;
	}
	
	/*
	void cblas_dgemv(const enum CBLAS_ORDER order,
                 const enum CBLAS_TRANSPOSE TransA, const int M, const int N,
                 const double alpha, const double *A, const int lda,
                 const double *X, const int incX, const double beta,
                 double *Y, const int incY);
                 */
        
        double *result=(double *) malloc(nz_vector * sizeof(double));
        
	cblas_dgemv(CblasColMajor,CblasNoTrans,M,N,1.0,values,N,vectorValues,1,0.0,result,1);
	
	if(write2file){
		writeDenseVector(argv[3], result,M_Vector,N_Vector,nz_vector);
	}
	else{
		writeDenseVector("stdout", result,M_Vector,N_Vector,nz_vector);
	}
	return 1;
}

