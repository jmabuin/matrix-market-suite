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

#include "CreateSparseMatrixGeneral.h"

/* Fill the already allocated array (list) with (ammount) **different** random
 * numbers from [1, max], to be used as random columns
 */
static void RandomColumns(unsigned long *list, size_t ammount, unsigned long max) {
	size_t i = 0, j;
	unsigned long candidate;

	while (i < ammount) {
		candidate = 1 + (double) rand() * max / (double) RAND_MAX;

		/* rand() can return RAND_MAX, I don't want that case */
		if (candidate == max + 1) continue;

		/* only add this candidate if it was not added before */
		for (j = 0; j < i; j++) {
			if (list[j] == candidate) break;
		}

		if (j == i) {
			list[j] = candidate;
			i++;
		}
	}
};

int CreateSparseMatrixGeneral(char *fileName, unsigned long int numRows, unsigned long int numCols, unsigned long long nnz, unsigned int seed) {

	FILE *output;
	unsigned long int i, j;
	MM_typecode outputmatcode;

	mm_initialize_typecode(&outputmatcode);
	mm_set_matrix(&outputmatcode);
	mm_set_coordinate(&outputmatcode);
	mm_set_real(&outputmatcode);
	mm_set_general(&outputmatcode);

	//unsigned long int numRows = 0;
	//unsigned long int numCols = 0;
	unsigned long long int numNonzero = nnz;
	//unsigned int seed = 0;
	unsigned long int * columns;
	size_t elementsPerRow;
	double value;

	//if (argc < 6) {
	//	fprintf(stderr, "[%s] Usage: %s [num-rows] [num-cols] [num-nonzero] [output-filename] [seed]\n",__func__, argv[0]);
	//	return 0;
	//}

	//if ((output = fopen(argv[4], "w")) == NULL) {
	//	return 0;
	//}

	//numRows = atoi(argv[1]);
	//numCols = atoi(argv[2]);
	//numNonzero = atoi(argv[3]);
	//seed = atoi(argv[5]);

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

	elementsPerRow = numNonzero / numRows;

	if (elementsPerRow == 0) {
		fprintf(stderr, "[%s] Too few non-zero elements. Provide at least as much as rows\n", __func__);
		return 0;
	} else if (elementsPerRow > numCols) {
		fprintf(stderr, "[%s] Too many non-zero elements. More elements required than matrix size\n", __func__);
		return 0;
	}
	else if(numNonzero%numRows != 0) {
		numNonzero = elementsPerRow * numRows;
	}

	columns = malloc(elementsPerRow * sizeof(unsigned long));
	
	if (columns == NULL)
		return 0;

	srand (seed);

	mm_write_banner(output, outputmatcode);
	mm_write_mtx_crd_size(output, numRows, numCols, numNonzero);

	for(i = 0;i < numRows; i++){
		RandomColumns(columns, elementsPerRow, numCols);
		for (j = 0; j < elementsPerRow; j++) {
			value = ((double)rand() / (double)RAND_MAX);
			fprintf(output, "%lu %lu %lg\n",i+1,columns[j],value);
		}
	}
	
	free(columns);

	fclose(output);

	return 1;
}
