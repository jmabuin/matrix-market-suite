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

#include <cblas.h>

#include "DMxV.h"
#include "basic.h"

void usageDMxV(){

	fprintf(stderr, "\n");
	fprintf(stderr, "Usage: MM-Suite DMxV [options] <input-matrix> <input-vector>\n");
	fprintf(stderr, "\nInput/output options:\n\n");
	fprintf(stderr, "       -o STR        Output file name. Default: stdout\n");
	fprintf(stderr, "       -r            Input format is row per line. Default: False\n");
	fprintf(stderr, "\n");

}

int DMxV(int argc, char *argv[]) {
	
	int 			ret_code = 1;
	int 			option;
	
	unsigned long 		*II;
	unsigned long 		*J;
	double 			*values;
	
	unsigned long 		M;
	unsigned long 		N;
	unsigned long long 	nz;
	
	
	double 			*vectorValues;
	unsigned long 		M_Vector;
	unsigned long 		N_Vector;
	unsigned long long 	nz_vector;
	
	char			*outputFileName = NULL;
	
	char			*inputMatrixFile = NULL;
	char			*inputVectorFile = NULL;
	
	int			inputFormatRow = 0;
	int			basicOps = 0;
	
	while ((option = getopt(argc, argv,"bro:")) >= 0) {
		switch (option) {
			case 'o' : 
				//free(outputFileName);
				
				outputFileName = (char *) malloc(sizeof(char)*strlen(optarg)+1);
				strcpy(outputFileName,optarg);
				
				break;
			
			case 'r':
				inputFormatRow = 1;
				break;
				
			case 'b':
				basicOps = 1;
				break;
			
			default: break;
		}
	
	}
	
	if ((optind + 2 > argc) || (optind + 3 <= argc)) {
		usageDMxV();
		return 0;
	}
	
	if(outputFileName == NULL) {
		outputFileName = (char *) malloc(sizeof(char)*7);
		sprintf(outputFileName,"stdout");
	}
	
	inputMatrixFile = (char *)malloc(sizeof(char)*strlen(argv[optind])+1);
	inputVectorFile = (char *)malloc(sizeof(char)*strlen(argv[optind+1])+1);
	
	strcpy(inputMatrixFile,argv[optind]);
	strcpy(inputVectorFile,argv[optind+1]);
	
	//Read matrix
	if(inputFormatRow){
	
		if(!readDenseCoordinateMatrixRowLine(inputMatrixFile,&II,&J,&values,&M,&N,&nz)){
			fprintf(stderr, "[%s] Can not read Matrix\n",__func__);
			return 0;
		}
	
	}
	else {
		if(!readDenseCoordinateMatrix(inputMatrixFile,&II,&J,&values,&M,&N,&nz)){
			fprintf(stderr, "[%s] Can not read Matrix\n",__func__);
			return 0;
		}
	}
	
	
	
	//Read vector
	if(!readDenseVector(inputVectorFile, &vectorValues,&M_Vector,&N_Vector,&nz_vector)){
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
        
	//cblas_dgemv(CblasColMajor,CblasNoTrans,M,N,1.0,values,N,vectorValues,1,0.0,result,1);
	int t_real = realtime();
	if(basicOps){
		mms_dgemv(M, N, 1.0, values, vectorValues,0.0, result);
	}
	else{
		cblas_dgemv(CblasRowMajor,CblasNoTrans,M,N,1.0,values,N,vectorValues,1,0.0,result,1);
	}
	fprintf(stderr, "\n[%s] Time spent in cblas_dgemv: %.6f sec\n", __func__, realtime() - t_real);
	
	writeDenseVector(outputFileName, result,M_Vector,N_Vector,nz_vector);
	
	
	return ret_code;
}

