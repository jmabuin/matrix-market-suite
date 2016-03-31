
#include <stdio.h>
#include <stdlib.h>
#include "CreateDenseVector.h"

//int CreateDenseMatrixSymmetric(unsigned long numRows,unsigned long numCols, char *fileName, unsigned int seed) {
int CreateDenseVector(int argc, char *argv[]) {
//Options: numItems fileName seed

	FILE *output;
	//long nz;   
	int i;
	MM_typecode outputmatcode;
	
	mm_initialize_typecode(&outputmatcode);
	mm_set_array(&outputmatcode);
	mm_set_dense(&outputmatcode);
	//mm_set_coordinate(&outputmatcode);
	mm_set_real(&outputmatcode);
	//mm_set_symmetric(&outputmatcode);
	
	unsigned long int numItems = 0;
	unsigned int seed = 0;

	//int ret_code;

	if (argc < 4)
	{
		fprintf(stderr, "[%s] Usage: %s [num-items] [output-filename] [seed]\n",__func__, argv[0]);
		return 0;
	}

	if ((output = fopen(argv[2], "w")) == NULL){
		return 0;
	}
			
	numItems = atoi(argv[1]);
	seed = atoi(argv[3]);
	
	//unsigned long long int nnz = numItems;
	
	
	srand (seed);

	mm_write_banner(output, outputmatcode);
	//mm_write_mtx_crd_size(output, numRows, numCols, numRows*numCols);
	mm_write_mtx_array_size(output, numItems, 1);
	//ret_code = fprintf(output,"\%\%MatrixMarket matrix coordinate real symmetric\n");

	
	for(i = 0;i < numItems; i++){

		fprintf(output, "%f\n",((double)rand() / (double)RAND_MAX));
	}

	fclose(output);

	return 1;
}

