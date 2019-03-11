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

void usageDMxDM(){

	fprintf(stderr, "\n");
	fprintf(stderr, "Usage: MM-Suite DMxDM [options] <input-matrix-A> <input-matrix-B>\n");
	fprintf(stderr, "\nInput/output options:\n\n");
	fprintf(stderr, "       -o STR        Output file name. Default: stdout\n");
	fprintf(stderr, "       -r            Input format is row per line. Default: False\n");
	fprintf(stderr, "\nPerformance options:\n\n");
	fprintf(stderr, "       -t INT        Number of threads to use in OpenBLAS. Default: 1\n");
	fprintf(stderr, "\n");

}

int DMxDM(int argc, char *argv[]) {
	
	int 			ret_code = 1;
	int 			option;
	
	unsigned long 		*IA;
	unsigned long 		*JA;
	double 			*valuesA;
	
	unsigned long 		MA;
	unsigned long 		NA;
	unsigned long long 	nzA;
	
	
	unsigned long 		*IB;
	unsigned long 		*JB;
	double 			*valuesB;
	
	unsigned long 		MB;
	unsigned long 		NB;
	unsigned long long 	nzB;
	
	char			*outputFileName = NULL;
	
	char			*inputMatrixFileA = NULL;
	char			*inputMatrixFileB= NULL;
	
	int			inputFormatRow = 0;
	
	int			numThreads = 1;
	
	while ((option = getopt(argc, argv,"ro:t:")) >= 0) {
		switch (option) {
			case 'o' : 
				//free(outputFileName);
				
				outputFileName = (char *) malloc(sizeof(char)*strlen(optarg)+1);
				strcpy(outputFileName,optarg);
				
				break;
			
			case 'r':
				inputFormatRow = 1;
				break;
				
			case 't':
				numThreads = atoi(optarg);
				break;
			
			default: break;
		}
	
	}
	
	if ((optind + 2 > argc) || (optind + 3 <= argc)) {
		usageDMxDM();
		return 0;
	}
	
	openblas_set_num_threads(numThreads);
	
	if(outputFileName == NULL) {
		outputFileName = (char *) malloc(sizeof(char)*7);
		sprintf(outputFileName,"stdout");
	}
	
	inputMatrixFileA = (char *)malloc(sizeof(char)*strlen(argv[optind])+1);
	inputMatrixFileB = (char *)malloc(sizeof(char)*strlen(argv[optind+1])+1);
	
	strcpy(inputMatrixFileA,argv[optind]);
	strcpy(inputMatrixFileB,argv[optind+1]);
	
	//Read matrices
	if(inputFormatRow){
	
		if(!readDenseCoordinateMatrixRowLine(inputMatrixFileA,&IA,&JA,&valuesA,&MA,&NA,&nzA)){
			fprintf(stderr, "[%s] Can not read Matrix\n",__func__);
			return 0;
		}
		
		if(!readDenseCoordinateMatrixRowLine(inputMatrixFileB,&IB,&JB,&valuesB,&MB,&NB,&nzB)){
			fprintf(stderr, "[%s] Can not read Matrix\n",__func__);
			return 0;
		}
	
	}
	else {
		if(!readDenseCoordinateMatrix(inputMatrixFileA,&IA,&JA,&valuesA,&MA,&NA,&nzA)){
			fprintf(stderr, "[%s] Can not read Matrix\n",__func__);
			return 0;
		}
		
		if(!readDenseCoordinateMatrix(inputMatrixFileB,&IB,&JB,&valuesB,&MB,&NB,&nzB)){
			fprintf(stderr, "[%s] Can not read Matrix\n",__func__);
			return 0;
		}
	}
	
	/*
	void cblas_dgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA,
                 const enum CBLAS_TRANSPOSE TransB, const int M, const int N,
                 const int K, const double alpha, const double *A,
                 const int lda, const double *B, const int ldb,
                 const double beta, double *C, const int ldc);
                 */
        
        double *result=(double *) malloc(MA * NB * sizeof(double));
        
	//cblas_dgemv(CblasColMajor,CblasNoTrans,M,N,1.0,values,N,vectorValues,1,0.0,result,1);
	cblas_dgemm(CblasRowMajor,CblasNoTrans,CblasNoTrans, MA, NB, NA, 1.0,valuesA,NA,valuesB,NB,0.0,result,NB);
	
	if(inputFormatRow){
		writeDenseCoordinateMatrixRowLine(outputFileName, result,MA,NB,MA * NB);
	}
	else{
		writeDenseCoordinateMatrix(outputFileName, result,MA,NB,MA * NB);
	}
	
	
	
	return ret_code;
}

