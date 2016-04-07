
#include "../operations/cblas.h"
#include "ConjugateGradientSolver.h"

int ConjugateGradientSolver(unsigned long *I, unsigned long *J, double *A, unsigned long M, unsigned long N, unsigned long long nz, double *b, unsigned long M_Vector, unsigned long N_Vector, unsigned long long nz_vector) {


	double *Ax=(double *) malloc(nz_vector * sizeof(double));
        double *Ap=(double *) malloc(nz_vector * sizeof(double));
        double *r=(double *) malloc(nz_vector * sizeof(double));
        double *p=(double *) malloc(nz_vector * sizeof(double));
	double *x=(double *) calloc(nz_vector,sizeof(double));
	
	/*
	void cblas_dgemv(const enum CBLAS_ORDER order,
                 const enum CBLAS_TRANSPOSE TransA, const int M, const int N,
                 const double alpha, const double *A, const int lda,
                 const double *X, const int incX, const double beta,
                 double *Y, const int incY);
                 */
	
	//r = b-A*x
	cblas_dgemv(CblasColMajor, CblasNoTrans, M,N , 1.0, A, N, x, 1, 0.0, Ax, 1);
	
	vector_substract(b,Ax, r,N);
	
	//writeDenseVector("stdout", r,M_Vector,N_Vector,nz_vector);
	
	//p=r

	memcpy(p, r, N*sizeof(double));
	
	//rsold = r*r
	double rsold = cblas_ddot(N,r,1,r,1);
	
	int stop = 0;
		
	double alphaCG = 0.0;
		
	double rsnew = 0.0;
	unsigned long k = 0;
	
	unsigned long maxIterations = M*2;
	//unsigned long maxIterations = 1;
	
	//double tmp = 0.0;
	//MM_typecode matcode;
	

	while(!stop){
	
		//Ap=A*p
		cblas_dgemv(CblasColMajor, CblasNoTrans, M,N , 1.0, A, N, p, 1, 0.0, Ap, 1);
	
		//alphaCG=rsold/(p'*Ap)
		alphaCG = rsold/cblas_ddot(N,p,1,Ap,1);
		
		//x=x+alphaCG*p
		cblas_daxpy(N,alphaCG,p,1,x,1);
		
		//writeDenseVector("stdout",x,M_Vector,N_Vector,nz_vector);

		//r=r-alphaCG*Ap
		cblas_daxpy(N,-alphaCG,Ap,1,r,1);
	
		//rsnew = r'*r
		rsnew = cblas_ddot(N,r,1,r,1);

		if((sqrt(rsnew)<=EPSILON)||(k == maxIterations)){
			stop = 1;
		}
		
		//p=r+rsnew/rsold*p
		cblas_dscal(N, rsnew/rsold, p, 1);
		cblas_daxpy(N,1.0,r,1,p,1);
		
		
		rsold = rsnew;
		
		k++;
	}
	
	memcpy(b, x, N*sizeof(double));

	free(Ax);
        free(Ap);
        free(r);
        free(p);
	free(x);

	fprintf(stderr, "[%s] Number of iterations %lu\n",__func__,k);

	return 1;
}