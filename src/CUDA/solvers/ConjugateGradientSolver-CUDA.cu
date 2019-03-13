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

#include <cuda_runtime.h>
#include <cublas.h>
#include <device_launch_parameters.h>
#include <cublas_v2.h>
#include "ConjugateGradientSolver-CUDA.h"

int ConjugateGradientSolverCUDA(unsigned long *II, unsigned long *J, double *A, unsigned long M, unsigned long N, unsigned long long nz, double *b, unsigned long M_Vector, unsigned long N_Vector, unsigned long long nz_vector, double *x, int iterationNumber) {

    //A*x=b

    cudaError_t  cudaStat;  //  cudaMalloc  status
    cublasStatus_t  stat;   //  CUBLAS  functions  status
    cublasHandle_t  handle;

    double *d_A;
    double *d_x;
    double *d_b;
    double *Ap; //=(double *) malloc(nz_vector * sizeof(double));
    double *r; //=(double *) malloc(nz_vector * sizeof(double));
    double *p; //=(double *) malloc(nz_vector * sizeof(double));
    //double *x=(double *) calloc(nz_vector,sizeof(double));

    double			alpha = 1.0;
    double			beta = 0.0;

    // ================= Reserve memory and copy data for matrix A on device =================
    cudaStat = cudaMalloc ((void **)&d_A , nz * sizeof(double));     // device

    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cudaMalloc for matrix values\n",__func__);
        return 0;
    }

    cudaStat = cudaMemcpy( d_A, A, nz * sizeof(double), cudaMemcpyHostToDevice );
    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cudaMemcpy for matrix values\n",__func__);
        return 0;

    }
    // =======================================================================================

    // ================= Reserve memory and copy data for vector b on device =================
    cudaStat = cudaMalloc ((void **)&d_b , nz_vector * sizeof(double));     // device

    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cudaMalloc for vector values\n",__func__);
        return 0;
    }

    cudaStat = cudaMemcpy( d_b, b, nz_vector* sizeof(double), cudaMemcpyHostToDevice );
    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cublasSetVector for matrix values\n",__func__);
        return 0;

    }
    // =======================================================================================

    // ================= Reserve memory and copy data for vector x on device =================
    cudaStat = cudaMalloc ((void **)&d_x , nz_vector * sizeof(double));     // device

    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cudaMalloc for vector values\n",__func__);
        return 0;
    }

    cudaStat = cudaMemcpy( d_x, x, nz_vector* sizeof(double), cudaMemcpyHostToDevice );
    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cublasSetVector for matrix values\n",__func__);
        return 0;

    }
    // =======================================================================================

    // ================= Reserve memory for vector Ap on device =================
    cudaStat = cudaMalloc ((void **)&Ap , nz_vector * sizeof(double));     // device

    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cudaMalloc for vector values\n",__func__);
        return 0;
    }
    // ==========================================================================

    // ================= Reserve memory for vector r on device =================
    cudaStat = cudaMalloc ((void **)&r , nz_vector * sizeof(double));     // device

    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cudaMalloc for vector values\n",__func__);
        return 0;
    }
    // ==========================================================================

    // ================= Reserve memory for vector p on device =================
    cudaStat = cudaMalloc ((void **)&p , nz_vector * sizeof(double));     // device

    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cudaMalloc for vector values\n",__func__);
        return 0;
    }
    // ==========================================================================

    stat = cublasCreate (& handle );   //  initialize  CUBLAS  context

    if (stat != CUBLAS_STATUS_SUCCESS) {
        fprintf(stderr, "[%s] Error in cublasCreate\n",__func__);
        return 0;

    }

    //r = b-A*x
    //If we take x=0 the init multiplication is avoided and r=b

    //memcpy(r, b, N*sizeof(double));
    cudaStat = cudaMemcpy( r, b, nz_vector* sizeof(double), cudaMemcpyHostToDevice );
    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cublasSetVector for vector values\n",__func__);
        return 0;

    }

    //p=r

    //memcpy(p, r, N*sizeof(double));
    cudaStat = cudaMemcpy( p, b, nz_vector* sizeof(double), cudaMemcpyHostToDevice );
    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cublasSetVector for vector values\n",__func__);
        return 0;

    }

    //rsold = r*r
    double rsold;// = cblas_ddot(N,r,1,r,1);

    cublasDdot(handle, nz_vector, r, 1, r, 1, &rsold);

    int stop = 0;

    double alphaCG = 0.0;

    double rsnew = 0.0;
    unsigned long k = 0;

    unsigned long maxIterations = M*2;

    if(iterationNumber != 0 ){
        maxIterations = iterationNumber;
    }

    //int i  = 0;

    double tmp_result;

    while(!stop){

        //Ap=A*p
        // for(i=0; i<M; i++){
        //	Ap[i] = 0.0;
        // }
        //cblas_dgemv(CblasRowMajor, CblasNoTrans, M,N , 1.0, A, N, p, 1, 0.0, Ap, 1);
        stat = cublasDgemv(handle, CUBLAS_OP_N, M, N, &alpha, d_A, M, p, 1, &beta, Ap, 1);

        //alphaCG=rsold/(p'*Ap)
        //alphaCG = rsold/cblas_ddot(N,p,1,Ap,1);
        cublasDdot(handle, N, p, 1, Ap, 1, &tmp_result);
        alphaCG = rsold/tmp_result;

        //x=x+alphaCG*p
        cublasDaxpy( handle, N, &alphaCG, p, 1, d_x, 1);
        //cblas_daxpy(N,alphaCG,p,1,x,1);

        //r=r-alphaCG*Ap
        //cblas_daxpy(N,-alphaCG,Ap,1,r,1);
        tmp_result = alphaCG * -1;
        cublasDaxpy( handle, N, &tmp_result, Ap, 1, r, 1);

        //rsnew = r'*r
        //rsnew = cblas_ddot(N,r,1,r,1);
        cublasDdot(handle, N, r, 1, r, 1, &rsnew);

        if((sqrt(rsnew)<=EPSILON)||(k == maxIterations)){
            stop = 1;
        }

        //p=r+rsnew/rsold*p
        //cblas_dscal(N, rsnew/rsold, p, 1);
        tmp_result = (rsnew/rsold);
        cublasDscal(handle, N, &tmp_result, p, 1);
        //cblas_daxpy(N,1.0,r,1,p,1);
        cublasDaxpy( handle, N, &alpha, r, 1, p, 1);

        rsold = rsnew;

        k++;
    }

    //memcpy(b, x, N*sizeof(double));
    cudaStat = cudaMemcpy( x, d_x, nz_vector* sizeof(double), cudaMemcpyDeviceToHost );
    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error copying results back to host\n",__func__);
        return 0;

    }


    cudaFree(d_x);                             // free  device  memory
    cudaFree(d_A);                             // free  device  memory
    cudaFree(d_b);                             // free  device  memory
    cudaFree(Ap);                             // free  device  memory
    cudaFree(r);                             // free  device  memory
    cudaFree(p);                             // free  device  memory
    cublasDestroy(handle);

    fprintf(stderr, "[%s] Number of iterations %lu\n",__func__,k);

    return 1;
}
