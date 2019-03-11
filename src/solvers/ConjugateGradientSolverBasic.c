/**
  * Copyright 2016 José Manuel Abuín Mosquera <josemanuel.abuin@usc.es>
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

#include "ConjugateGradientSolverBasic.h"


int ConjugateGradientSolverBasic(unsigned long *II, unsigned long *J, double *A, unsigned long M, unsigned long N, unsigned long long nz, double *b, unsigned long M_Vector, unsigned long N_Vector, unsigned long long nz_vector,double *x, int iterationNumber) {
	
	//A*x=b

        double *Ap=(double *) malloc(nz_vector * sizeof(double));
        double *r=(double *) malloc(nz_vector * sizeof(double));
        double *p=(double *) malloc(nz_vector * sizeof(double));
	//double *x=(double *) calloc(nz_vector,sizeof(double));
	
	//r = b-A*x
	//If we take x=0 the init multiplication is avoided and r=b
	
	memcpy(r, b, N*sizeof(double));
	
	
	//p=r

	memcpy(p, r, N*sizeof(double));
	
	//rsold = r*r
	double rsold = mms_ddot(N,r,r);
	
	int stop = 0;
		
	double alphaCG = 0.0;
		
	double rsnew = 0.0;
	unsigned long k = 0;
	
	unsigned long maxIterations = M*2;
	
	if(iterationNumber != 0 ){
		maxIterations = iterationNumber;
	}
		

	while(!stop){
	
		//Ap=A*p
		mms_dgemv( M,N , 1.0, A, p, 0.0, Ap);

		//alphaCG=rsold/(p'*Ap)
		alphaCG = rsold/mms_ddot(N,p,Ap);
		
		//x=x+alphaCG*p
		mms_daxpy(N,alphaCG,p,x);

		//r=r-alphaCG*Ap
		mms_daxpy(N,-alphaCG,Ap,r);
	
		//rsnew = r'*r
		rsnew = mms_ddot(N,r,r);

		if((sqrt(rsnew)<=EPSILON)||(k == maxIterations)){
			stop = 1;
		}
		
		//p=r+rsnew/rsold*p
		mms_dscal(N, rsnew/rsold, p);
		mms_daxpy(N,1.0,r,p);
		
		
		rsold = rsnew;
		
		k++;
	}
	
	//memcpy(b, x, N*sizeof(double));

        free(Ap);
        free(r);
        free(p);
	//free(x);

	fprintf(stderr, "[%s] Number of iterations %lu\n",__func__,k);

	return 1;
}
