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

#include <lapacke.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "LUDecomposition.h"

void usageLUDecomposition(){

	fprintf(stderr, "\n");
	fprintf(stderr, "Usage: MM-Suite LUDecomposition [options] <input-matrix>\n");
	fprintf(stderr, "\nInput/output options:\n\n");
	fprintf(stderr, "       -o STR        Output file name. Default: stdout\n");
	fprintf(stderr, "       -r            Input format is row per line. Default: False\n");
	fprintf(stderr, "\n");

}

int LUDecomposition(int argc, char *argv[]) {

	int 			ret_code = 1;
	int 			option;

	unsigned long 		*II;
	
	unsigned long 		*J;
	double 			*values;
	
	unsigned long 		M;
	unsigned long 		N;
	unsigned long long 	nz;
	
	int			_M;
	int			_N;
	
	
	char			*outputFileName = NULL;
	
	char			*inputMatrixFile = NULL;
	
	int			inputFormatRow = 0;
	
	
	while ((option = getopt(argc, argv,"ro:")) >= 0) {
		switch (option) {
			case 'o' : 
				//free(outputFileName);
				
				outputFileName = (char *) malloc(sizeof(char)*strlen(optarg)+1);
				strcpy(outputFileName,optarg);
				
				break;
			
			case 'r':
				inputFormatRow = 1;
				break;
			
			default: break;
		}
	
	}
	
	if ((optind + 1 > argc) || (optind + 2 <= argc)) {
		usageLUDecomposition();
		return 0;
	}
	
	if(outputFileName == NULL) {
		outputFileName = (char *) malloc(sizeof(char)*7);
		sprintf(outputFileName,"stdout");
	}
	
	inputMatrixFile = (char *)malloc(sizeof(char)*strlen(argv[optind])+1);
	
	strcpy(inputMatrixFile,argv[optind]);

	
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

	
	_M = (int) M;
	_N = (int) N;
	
	int ipiv[_M];
	
	/*
	lapack_int LAPACKE_dgetrf( int matrix_layout, lapack_int m, lapack_int n,
                           double* a, lapack_int lda, lapack_int* ipiv );
	*/
	
        ret_code = LAPACKE_dgetrf(LAPACK_ROW_MAJOR,_M,_N, values,_N, ipiv);
	
	if(inputFormatRow){
		writeLUCoordinateMatrixRowLine(outputFileName, values,M,N,nz, ipiv);
	}
	else{
		writeLUCoordinateMatrix(outputFileName, values,M,N,nz, ipiv);
	}
	
	
	//writeDenseCoordinateMatrix("stdout",values,M,N,nz);
	if(!ret_code)
		return 1;
	else
		return 0;
	

}
