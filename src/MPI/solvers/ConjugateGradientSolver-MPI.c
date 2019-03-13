/**
  * Copyright 2019 José Manuel Abuín Mosquera <josemanuel.abuin@usc.es>
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
#include "ConjugateGradientSolver-MPI.h"

#include <mpi.h>

int ConjugateGradientSolverMPI(unsigned long *II, unsigned long *J, double *A, unsigned long M,unsigned long local_M, unsigned long N, unsigned long long nz, double *b, unsigned long M_Vector, unsigned long N_Vector, unsigned long long nz_vector, int numIterations) {

    //A*x=b

    double *Ax=(double *) malloc(nz_vector * sizeof(double));
    double *Ap=(double *) malloc(nz_vector * sizeof(double));
    double *r=(double *) malloc(nz_vector * sizeof(double));
    double *p=(double *) malloc(nz_vector * sizeof(double));
    double *x=(double *) calloc(nz_vector,sizeof(double));

    double *Ap_partial=(double *) malloc(local_M * sizeof(double));

    //r = b-A*x
    //If we take x=0 the init multiplication is avoided and r=b

    memcpy(r, b, N*sizeof(double));

    //p=r

    memcpy(p, r, N*sizeof(double));

    //rsold = r*r
    double rsold = cblas_ddot(N,r,1,r,1);

    int stop = 0;

    double alphaCG = 0.0;

    double rsnew = 0.0;
    unsigned long k = 0;

    unsigned long maxIterations = M*2;
    if ( numIterations != 0) {
        maxIterations = numIterations;
    }


    while(!stop){

        //Ap=A*p
        //cblas_dgemv(CblasColMajor, CblasNoTrans, M,N , 1.0, A, N, p, 1, 0.0, Ap, 1);
        cblas_dgemv(CblasRowMajor, CblasNoTrans, local_M,N , 1.0, A, N, p, 1, 0.0, Ap_partial, 1);

        MPI_Allgather (Ap_partial,local_M,MPI_DOUBLE,Ap,local_M,MPI_DOUBLE,MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);

        //alphaCG=rsold/(p'*Ap)
        alphaCG = rsold/cblas_ddot(N,p,1,Ap,1);

        //x=x+alphaCG*p
        cblas_daxpy(N,alphaCG,p,1,x,1);

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
