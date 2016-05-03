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
#include "CreateDenseMatrixSymmetricRowLine.h"

int CreateDenseMatrixSymmetricRowLine(int argc, char *argv[]) {
//Options: numRows numCols fileName seed

	FILE *output;
	//long nz;   
	int i, j;
	
	unsigned long int numRows = 0;
	unsigned long int numCols = 0;
	unsigned int seed = 0;

	MM_typecode outputmatcode;
	
	mm_initialize_typecode(&outputmatcode);
	mm_set_matrix(&outputmatcode);
	//mm_set_coordinate(&outputmatcode);
	mm_set_dense(&outputmatcode);
	mm_set_real(&outputmatcode);
	mm_set_symmetric(&outputmatcode);

	//double valor = 0.0;

	//int ret_code;

	if (argc < 5)
	{
		fprintf(stderr, "[%s] Usage: %s [num-rows] [num-cols] [output-filename] [seed]\n",__func__, argv[0]);
		return 0;
	}

	if ((output = fopen(argv[3], "w")) == NULL){
		return 0;
	}
			
	numRows = atoi(argv[1]);
	numCols = atoi(argv[2]);
	seed = atoi(argv[4]);
	
	unsigned long long int nnz = numRows*numCols;
	
	mm_write_banner(output, outputmatcode);
	mm_write_mtx_crd_size(output, numRows, numCols, numRows*numCols);
	
	double *values =  (double *) calloc(nnz,sizeof(double));
	double value = 0.0;
	
	srand (seed);
	
	for(i = 0;i < numRows; i++){
	
		for(j = 0; j<=i; j++){

			value = ((double)rand() / (double)RAND_MAX)/100;
			
			values[i*numCols+j] = value;
			
			if(i!=j){
				values[j*numCols+i] = value;
			}
			
		}

	}
	
	
	for(i = 0;i < numRows; i++){
	
		fprintf(output, "%d:",i);
	
		for(j = 0; j<numCols; j++){

			fprintf(output, "%f,",values[i*numCols+j] );
		}

		fprintf(output,"\n");

	}
	

	fclose(output);

	return 1;
}

