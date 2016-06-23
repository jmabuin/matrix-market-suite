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
#include <string.h>

#include "CreateSparseMatrixGeneralGap.h"

int CreateSparseMatrixGeneralGap(char *fileName, unsigned long int numRows, unsigned long int numCols, unsigned long long nnz, unsigned int seed) {

	FILE *output;
	unsigned long int i;
	MM_typecode outputmatcode;

	mm_initialize_typecode(&outputmatcode);
	mm_set_matrix(&outputmatcode);
	mm_set_coordinate(&outputmatcode);
	mm_set_real(&outputmatcode);
	mm_set_general(&outputmatcode);


	unsigned long long int numNonzero = nnz;

	size_t gap;
	double value;

	if(strcmp(fileName,"stdout")==0){
		output = stdout;
	}
	else{
		if ((output = fopen(fileName, "w")) == NULL){
			fprintf(stderr,"[%s] Unable to open file for writing\n",__func__);
			return 0;
		}
	}

	if (numRows == 0) {
		fprintf(stderr, "[%s] The number of rows cannot be zero.\n", __func__);
		return 0;
	}

	gap = (numRows * numCols) / numNonzero;

	fprintf(stderr, "[%s] Gap is: %lu\n", __func__,gap);

	srand (seed);

	mm_write_banner(output, outputmatcode);
	mm_write_mtx_crd_size(output, numRows, numCols, numNonzero);

	unsigned long int currentIndex = 0;
	unsigned long int currentRow = 0;
	unsigned long int currentCol = 0;
	
	for(i = 0;i < numNonzero; i++){
		
		currentIndex = currentIndex + ((unsigned long int)rand() % gap) + 1;
		
		
		
		// If we are in the first row
		if(currentIndex < numCols) {
			currentRow = 0;
			currentCol = currentIndex;
		}
		else { // Otherwise
			currentRow = (currentIndex/numCols);
			currentCol = currentIndex-(currentRow*numCols);
		}
		
		value = ((double)rand() / (double)RAND_MAX);
		fprintf(output, "%lu %lu %lg\n",currentRow+1,currentCol,value);
		
	}
	

	fclose(output);

	return 1;
}
