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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <cuda_runtime.h>
#include <cublas.h>
#include <device_launch_parameters.h>
#include <cublas_v2.h>

#include "DMxV-CUDA.h"

void usageDMxV(){

    fprintf(stderr, "\n");
    fprintf(stderr, "Usage: MM-Suite DMxV [options] <input-matrix> <input-vector>\n");
    fprintf(stderr, "\nInput/output options:\n\n");
    fprintf(stderr, "       -o STR        Output file name. Default: stdout\n");
    fprintf(stderr, "       -r            Input format is row per line. Default: False\n");
    fprintf(stderr, "\nParameters options:\n\n");
    fprintf(stderr, "       -a DOUBLE     Alpha. Default: 1.0\n");
    fprintf(stderr, "       -b DOUBLE     Beta. Default: 0.0\n");
    fprintf(stderr, "\nPerformance options:\n\n");
    fprintf(stderr, "       -t INT        Number of threads to use in OpenBLAS. Default: 1\n");
    fprintf(stderr, "\n");

}

int DMxVCUDA(int argc, char *argv[]) {

    int 			ret_code = 1;
    int 			option;

    unsigned long 		*II;
    unsigned long 		*J;
    double 			*values;
    double          *d_values;

    unsigned long 		M;
    unsigned long 		N;
    unsigned long long 	nz;


    double 			*vectorValues;
    double 			*d_vectorValues;
    unsigned long 		M_Vector;
    unsigned long 		N_Vector;
    unsigned long long 	nz_vector;

    char			*outputFileName = NULL;

    char			*inputMatrixFile = NULL;
    char			*inputVectorFile = NULL;
    char			*outputVectorFile = NULL;

    int			inputFormatRow = 0;
    int			basicOps = 0;
    int			numThreads = 1;

    double			alpha = 1.0;
    double			beta = 0.0;

    cudaError_t  cudaStat;  //  cudaMalloc  status
    cublasStatus_t  stat;   //  CUBLAS  functions  status
    cublasHandle_t  handle;

    while ((option = getopt(argc, argv,"ero:b:a:t:")) >= 0) {
        switch (option) {
            case 'o' :
                //free(outputFileName);

                outputFileName = (char *) malloc(sizeof(char)*strlen(optarg)+1);
                strcpy(outputFileName,optarg);

                break;

            case 'r':
                inputFormatRow = 1;
                break;

            case 'e':
                basicOps = 1;
                break;

            case 'b':
                beta = atof(optarg);
                break;

            case 'a':
                alpha = atof(optarg);
                break;

            case 't':
                numThreads = atoi(optarg);
                break;

            default: break;
        }

    }

    if ((optind + 3 != argc) && (optind + 2 != argc)) {
        usageDMxV();
        return 0;
    }

    //openblas_set_num_threads(numThreads);

    if(optind + 3 == argc) { //We have an output vector

        outputVectorFile = (char *)malloc(sizeof(char)*strlen(argv[optind+2])+1);
        strcpy(outputVectorFile,argv[optind+2]);
    }

    if(outputFileName == NULL) {
        outputFileName = (char *) malloc(sizeof(char)*7);
        sprintf(outputFileName,"stdout");
    }

    inputMatrixFile = (char *)malloc(sizeof(char)*strlen(argv[optind])+1);

    if(inputMatrixFile == NULL) {
        fprintf(stderr, "[%s] Error reserving memory for input matrix file name\n",__func__);
        return 0;
    }

    inputVectorFile = (char *)malloc(sizeof(char)*strlen(argv[optind+1])+1);

    if(inputVectorFile == NULL) {
        fprintf(stderr, "[%s] Error reserving memory for input vector file name\n",__func__);
        return 0;
    }

    strcpy(inputMatrixFile,argv[optind]);
    strcpy(inputVectorFile,argv[optind+1]);

    //Read matrix
    if(inputFormatRow){

        if(!readDenseCoordinateMatrixRowLine(inputMatrixFile,&II,&J,&values,&M,&N,&nz)){
            usageDMxV();
            fprintf(stderr, "[%s] Can not read Matrix\n",__func__);
            return 0;
        }

    }
    else {
        if(!readDenseCoordinateMatrix(inputMatrixFile,&II,&J,&values,&M,&N,&nz)){
            usageDMxV();
            fprintf(stderr, "[%s] Can not read Matrix\n",__func__);
            return 0;
        }
    }

    //Read vector
    if(!readDenseVector(inputVectorFile, &vectorValues,&M_Vector,&N_Vector,&nz_vector)){
        usageDMxV();
        fprintf(stderr, "[%s] Can not read Vector\n",__func__);
        return 0;
    }

    cudaStat = cudaMalloc ((void **)&d_vectorValues , nz_vector * sizeof(double));     // device

    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cudaMalloc for vector values\n",__func__);
        return 0;
    }

    cudaStat = cudaMalloc ((void **)&d_values , nz * sizeof (double));     // device

    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cudaMalloc for matrix values\n",__func__);
        return 0;
    }

    stat = cublasCreate (& handle );   //  initialize  CUBLAS  context

    if (stat != CUBLAS_STATUS_SUCCESS) {
        fprintf(stderr, "[%s] Error in cublasCreate\n",__func__);
        return 0;

    }

    //stat = cublasSetVector(nz_vector,sizeof (*vectorValues),vectorValues,1,d_vectorValues ,1);// cp x->d_x
    cudaStat = cudaMemcpy( d_vectorValues, vectorValues, nz_vector* sizeof(double), cudaMemcpyHostToDevice );
    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cublasSetVector for vector values\n",__func__);
        return 0;

    }

    //stat = cublasSetVector(nz,sizeof (*values),values,1,d_values ,1);// cp x->d_x
    cudaStat = cudaMemcpy( d_values, values, nz* sizeof(double), cudaMemcpyHostToDevice );
    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cublasSetVector for matrix values\n",__func__);
        return 0;

    }


    double *result=(double *) malloc(nz_vector * sizeof(double));
    double *d_result;

    //Read output vector if any
    if(outputVectorFile != NULL) {
        if(!readDenseVector(outputVectorFile, &result,&M_Vector,&N_Vector,&nz_vector)){
            usageDMxV();
            fprintf(stderr, "[%s] Can not read Vector %s\n",__func__, outputVectorFile);
            return 0;
        }
    }

    cudaStat = cudaMalloc ((void **)&d_result , nz_vector * sizeof (double));     // device
    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cudaMalloc for result values\n",__func__);
        return 0;
    }

    //stat = cublasSetVector(nz_vector,sizeof (*result),result,1,d_result ,1);// cp x->d_x
    cudaStat = cudaMemcpy( d_result, result, nz_vector* sizeof(double), cudaMemcpyHostToDevice );
    if (cudaStat != cudaSuccess) {
        fprintf(stderr, "[%s] Error in cublasSetVector for result vector values\n",__func__);
        return 0;

    }

    double t_real = realtime();

    /*
    cublasStatus_t cublasDgemv(cublasHandle_t handle, cublasOperation_t trans,
                           int m, int n,
                           const double          *alpha,
                           const double          *A, int lda,
                           const double          *x, int incx,
                           const double          *beta,
                           double          *y, int incy)
                 */


    stat = cublasDgemv(handle, CUBLAS_OP_N, M, N, &alpha, d_values, M, d_vectorValues, 1, &beta, d_result, 1);

    // Copy array back to host
    cudaMemcpy( result, d_result, nz_vector* sizeof(double), cudaMemcpyDeviceToHost );

    cudaFree(d_result);                             // free  device  memory
    cudaFree(d_values);                             // free  device  memory
    cudaFree(d_vectorValues);                             // free  device  memory
    cublasDestroy(handle);

    fprintf(stderr, "\n[%s] Time spent in cublasDgemv: %.6f sec\n", __func__, realtime() - t_real);

    writeDenseVector(outputFileName, result,M_Vector,N_Vector,nz_vector);


    return ret_code;
}

