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
#include <string.h>
#include <math.h>

#include "CreateDenseMatrixGeneralDiagonallyDominant.h"

int CreateDenseMatrixGeneralDiagonallyDominant(char *fileName, unsigned long int numRows, unsigned long int numCols, unsigned int seed) {

//Options: numRows numCols fileName seed

	FILE *output;
	//long nz;
	int i, j;
	MM_typecode outputmatcode;

	mm_initialize_typecode(&outputmatcode);
	mm_set_matrix(&outputmatcode);
	mm_set_coordinate(&outputmatcode);
	//mm_set_dense(&outputmatcode);
	mm_set_real(&outputmatcode);
	//mm_set_symmetric(&outputmatcode);

	if(strcmp(fileName,"stdout")==0){
		output = stdout;
	}
	else{
		if ((output = fopen(fileName, "w")) == NULL){
			fprintf(stderr,"[%s] Unable to open file for writing\n",__func__);
			return 0;
		}
	}


	double value = 0.0;

	srand (seed);

	mm_write_banner(output, outputmatcode);
	mm_write_mtx_crd_size(output, numRows, numCols, numRows*numCols);
	//ret_code = fprintf(output,"\%\%MatrixMarket matrix coordinate real symmetric\n");

	//unsigned long long int val1 = 0;
	//unsigned long long int val2 = 0;

	double *tmpValues = (double *) calloc (numCols,sizeof(double));
	double diagonalValue = 0.0;

	for(i = 0;i < numRows; i++){

		diagonalValue = 0.0;

		for(j = 0 ; j < numCols; j++){

			if(i != j) {
				value = ((double)rand() / (double)RAND_MAX)/100;
				//fprintf(output, "%d %d %f\n",i+1,j+1,value);
				tmpValues[j] = value;

				diagonalValue = diagonalValue + fabs(value);
			}
		}

		diagonalValue = diagonalValue + 1.0;

		tmpValues[ i] = diagonalValue;

		for(j = 0 ; j < numCols; j++){

			fprintf(output, "%d %d %lg\n",i+1,j+1,tmpValues[j]);

		}

	}


	fclose(output);

	return 1;
}

