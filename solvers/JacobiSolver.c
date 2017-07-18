/**
  * Copyright 2017 José Manuel Abuín Mosquera <josemanuel.abuin@usc.es>
  * 
  * This file is part of Matrix Market Suite.
  *
  * Matrix Market Suite is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * Matrix Market Suite is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  * GNU General Public License for more details.
  * 
  * You should have received a copy of the GNU General Public License
  * along with Matrix Market Suite. If not, see <http://www.gnu.org/licenses/>.
  */

#include <cblas.h>
#include "JacobiSolver.h"

int JacobiSolver(unsigned long *II, unsigned long *J, double *A, unsigned long M, unsigned long N, unsigned long long nz, double *b, unsigned long M_Vector, unsigned long N_Vector, unsigned long long nz_vector,double *x2, int iterationNumber) {
	
	//Jacobi Method as shown in the example from https://en.wikipedia.org/wiki/Jacobi_method
	
	unsigned long 	k 	= 0;
	int 		stop 	= 0;
	
	unsigned int	i	= 0;
	
	double		result	= 0.0;
	
	//Initial solution
	double *x1=(double *) calloc(nz_vector,sizeof(double));
	//double *x2=(double *) calloc(nz_vector,sizeof(double));
	
	double *res=(double *) calloc(nz_vector,sizeof(double));
	
	double *LU=(double *) calloc(nz,sizeof(double));
	double *Dinv=(double *) calloc(nz,sizeof(double));
	
	double *T=(double *) calloc(nz,sizeof(double));
	double *C=(double *) calloc(nz_vector,sizeof(double));
	
	
	if(!isDiagonallyDominant(A,M,N,nz)){
		fprintf(stderr, "[%s] The matrix is not diagonally dominant\n",__func__);
		//return 0;
	}
	
	getLUValues(LU, A,M,N,nz);

	getDInvValues(Dinv, A,M,N,nz);
	
	for(i = 0; i< N; i++){
		x1[i] = 1.0;
	}
	
	//T=-D^{-1}(L+U)
	/*
	void cblas_dgemm (const CBLAS_LAYOUT Layout, const CBLAS_TRANSPOSE transa, const CBLAS_TRANSPOSE transb, const MKL_INT m, const MKL_INT n, const MKL_INT k, const double alpha, const double *a, const MKL_INT lda, const double *b, const MKL_INT ldb, const double beta, double *c, const MKL_INT ldc);
	*/
	cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans, M, N, N, -1.0, Dinv, N, LU, N, 0.0, T, N);
	//writeDenseCoordinateMatrixRowLine("stdout", T,M,N,nz);
	
	//C=-D^{-1}b
	cblas_dgemv(CblasRowMajor, CblasNoTrans, M,N , 1.0, Dinv, N, b, 1, 0.0, C, 1);
	
	//writeDenseVector("stdout", C,nz_vector,1, nz_vector);
	unsigned long maxIterations = M*2;
	
	if(iterationNumber != 0 ){
		maxIterations = iterationNumber;
	}
	
	while(!stop) {
	
		// x^{(1)}= Tx^{(0)}+C
		//x2 = T*x1
		cblas_dgemv(CblasRowMajor, CblasNoTrans, M,N , 1.0, T, N, x1, 1, 0.0, x2, 1);
		
		//x2 = x2+C
		cblas_daxpy(nz_vector,1.0,C, 1, x2, 1);
		
		//res = A*x - b
		memcpy(res, b, nz_vector*sizeof(double));
		cblas_dgemv(CblasRowMajor, CblasNoTrans, M,N , 1.0, A, N, x2, 1, -1.0, res, 1);
	
		
		result = vectorSumElements(res,N);
		if((fabs(result)<=EPSILON)||(k == maxIterations)){
			//fprintf(stderr,"Sum vector res is %lg\n",result);
			stop = 1;
		}
	
		memcpy(x1, x2, nz_vector*sizeof(double));
		//writeDenseVector("stdout", x1,nz_vector,1, nz_vector);
		k++;
	}
	
	//memcpy(b, x2, N*sizeof(double));

	free(x1);
	//free(x2);
	free(res);

	fprintf(stderr, "[%s] Number of iterations %lu\n",__func__,k);
	
	return 1;
	
}
