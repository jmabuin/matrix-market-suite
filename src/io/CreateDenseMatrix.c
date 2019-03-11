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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "CreateDenseMatrix.h"
#include "CreateDenseMatrixSymmetric.h"
#include "CreateDenseMatrixGeneral.h"
#include "CreateDenseMatrixGeneralRowLine.h"
#include "CreateDenseMatrixSymmetricRowLine.h"
#include "CreateDenseMatrixGeneralDiagonallyDominant.h"
#include "CreateDenseMatrixGeneralDiagonallyDominantRowLine.h"
#include "CreateDenseMatrixSymmetricDiagonallyDominantRowLine.h"
#include "CreateDenseMatrixSymmetricDiagonallyDominant.h"
#include "../utils/utils.h"

void usageCreateDenseMatrix(){

	fprintf(stderr, "\n");
	fprintf(stderr, "Usage: MM-Suite CreateDenseMatrix [options] <num-rows> <num-cols> <seed>\n");
	fprintf(stderr, "\nInput/output options:\n\n");
	fprintf(stderr, "       -o STR        Output file name. Default: stdout\n");
	fprintf(stderr, "       -r            Output format will be row per line. Default: False\n");
	fprintf(stderr, "       -s            Output will be a symmetric matrix.  Default: False\n");
	fprintf(stderr, "       -d            Output will be a diagonally dominant matrix.  Default: False\n");
    fprintf(stderr, "       -l            Lower value for data to put in the matrix.  Default: 0.0\n");
    fprintf(stderr, "       -m           Max value for data to put in the matrix.  Default: 1.0\n");
	fprintf(stderr, "\n");

}

int CreateDenseMatrix(int argc, char *argv[]) {
//Options: numRows numCols fileName seed

	
	
	/*  Options:
	 *		-s 		=> Symmetric matrix
	 *		-r 		=> Row per line matrix
	 *		-o [STR] 	=> Output file name
	 *		-d		=> Diagonally dominant matrix
	 */
	
	int	inputFormatRow		= 0;
	int	symmetric			= 0;
	int diagonallyDominant 	= 0;

	char	*outputFileName = NULL;
	
    double min = 0.0;
    double max = 1.0;
    
	int 	option;
	
	while ((option = getopt(argc, argv,"rso:dl:m:")) >= 0) {
		switch (option) {
			case 'o' : 
				//free(outputFileName);
				
				outputFileName = (char *) malloc(sizeof(char)*strlen(optarg)+1);
				strcpy(outputFileName,optarg);
				
				break;
			
			case 's' :
				symmetric = 1;
				break;
				
			case 'r':
				inputFormatRow = 1;
				break;

			case 'd':
				diagonallyDominant = 1;
				break;
            
            case 'l':
                min = (double) atof(optarg);
                break;
                
            case 'm':
                max = (double) atof(optarg);
                break;
                
			default: break;
		}
	
	}
	
	if ((optind + 3 > argc) || (optind + 4 <= argc)) {
		usageCreateDenseMatrix();
		return 0;
	}
	
	if(outputFileName == NULL) {
		outputFileName = (char *) malloc(sizeof(char)*7);
		sprintf(outputFileName,"stdout");
	}
	
	unsigned long int numRows 	= atol(argv[optind]);
	unsigned long int numCols 	= atol(argv[optind+1]);
	unsigned int seed			= atoi(argv[optind+2]);
	
	if (symmetric && inputFormatRow && !diagonallyDominant) { //Case of symmetric matrix and row per line
		CreateDenseMatrixSymmetricRowLine(outputFileName, numRows, numCols, seed, min, max);
	}
	else if (!symmetric && inputFormatRow && !diagonallyDominant) { //Case of general matrix and row per line
		CreateDenseMatrixGeneralRowLine(outputFileName, numRows, numCols, seed, min, max);
	}
	else if (symmetric && !inputFormatRow && !diagonallyDominant) { //Case of symmetric matrix and coordinate
		CreateDenseMatrixSymmetric(outputFileName, numRows, numCols, seed, min, max);
	}
	else if (!symmetric && !inputFormatRow && !diagonallyDominant) { //Case of general matrix and coordinate
		CreateDenseMatrixGeneral(outputFileName, numRows, numCols, seed, min, max);
	}
	else if (symmetric && inputFormatRow && diagonallyDominant) { //Case of symmetric matrix and row per line
		CreateDenseMatrixSymmetricDiagonallyDominantRowLine(outputFileName, numRows, numCols, seed, min, max);
	}
	else if (!symmetric && inputFormatRow && diagonallyDominant) { //Case of general matrix and row per line
		CreateDenseMatrixGeneralDiagonallyDominantRowLine(outputFileName, numRows, numCols, seed, min, max);
	}
	else if (symmetric && !inputFormatRow && diagonallyDominant) { //Case of symmetric matrix and coordinate
		CreateDenseMatrixSymmetricDiagonallyDominant(outputFileName, numRows, numCols, seed, min, max);
	}
	else if (!symmetric && !inputFormatRow && diagonallyDominant) { //Case of general matrix and coordinate
		CreateDenseMatrixGeneralDiagonallyDominant(outputFileName, numRows, numCols, seed, min, max);
	}

	return 1;
}

