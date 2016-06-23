/**
  * Copyright 2016 José Manuel Abuín Mosquera <josemanuel.abuin@usc.es>
  * Copyright 2016 Guillermo Indalecio <guillermo.indalecio.fernandez@usc.es>
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
#include "CreateSparseMatrix.h"
#include "CreateSparseMatrixGeneral.h"
#include "CreateSparseMatrixGeneralGap.h"
#include "../utils/utils.h"


void usageCreateSparseMatrix(){

	fprintf(stderr, "\n");
	fprintf(stderr, "Usage: MM-Suite CreateSparseMatrix [options] <num-rows> <num-cols> <seed>\n");
	fprintf(stderr, "\nInput/output options:\n\n");
	fprintf(stderr, "       -o STR        Output file name. Default: stdout\n");
	fprintf(stderr, "       -n INT        Number of non zeros in the output matrix. Default: numRows * numCols\n");
	fprintf(stderr, "       -s            Output will be a symmetric matrix.  Default: False\n");
	fprintf(stderr, "       -g            Uses a gap to determine distance to next element.  Default: False\n");
	fprintf(stderr, "\n");

}

int CreateSparseMatrix(int argc, char *argv[]) {


	/*  Options:
	 *		-s 		=> Symmetric matrix
	 
	 *		-o [STR] 	=> Output file name
	 *		-n [INT]	=> Number of non zeros
	 */

	int			symmetric	= 0;
	int 			gapVersion	= 0;
	char			*outputFileName = NULL;
	unsigned long long 	nnz 		= 0;
	int 			option;
	
	while ((option = getopt(argc, argv,"sgo:n:")) >= 0) {
		switch (option) {
			case 'o' : 
				//free(outputFileName);
				
				outputFileName = (char *) malloc(sizeof(char)*strlen(optarg)+1);
				strcpy(outputFileName,optarg);
				
				break;
			
			case 's' :
				symmetric = 1;
				break;
				
			case 'n':
				nnz = atol(optarg);
				break;
				
			case 'g':
				gapVersion = 1;
				break;
				
			default: break;
		}
	
	}
	
	if ((optind + 3 > argc) || (optind + 4 <= argc)) {
		usageCreateSparseMatrix();
		return 0;
	}
	
	if(outputFileName == NULL) {
		outputFileName = (char *) malloc(sizeof(char)*7);
		sprintf(outputFileName,"stdout");
	}
	
	unsigned long int numRows 	= atol(argv[optind]);
	unsigned long int numCols 	= atol(argv[optind+1]);
	unsigned int seed		= atoi(argv[optind+2]);
	
	if (nnz == 0){
		nnz = numRows * numCols;
	}
/*	
	if (symmetric) { //Case of symmetric matrix and row per line
		CreateSparseMatrixSymmetric(outputFileName, numRows, numCols, seed);
	}
	else if (!symmetric) { //Case of general matrix and row per line
		CreateSparseMatrixGeneral(outputFileName, numRows, numCols, seed);
	}
*/
	if (!symmetric && !gapVersion) { //Case of general matrix and row per line
		CreateSparseMatrixGeneral(outputFileName, numRows, numCols, nnz, seed);
	}
	else if (!symmetric && gapVersion) {
		CreateSparseMatrixGeneralGap(outputFileName, numRows, numCols, nnz, seed);
	}

	return 1;
}
