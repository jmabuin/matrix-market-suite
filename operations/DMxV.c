
#include <stdio.h>
#include <stdlib.h>
#include "DMxV.h"


int DMxV(int argc, char *argv[]) {
//Options: InputMatrixFile InputVectorFile

	FILE *inputMatrix;
	FILE *inputVector;

	int i;
	int ret_code;
	
	if (argc < 3)
	{
		fprintf(stderr, "[%s] Usage: %s [input-matrix-file] [input-vector-file]\n",__func__, argv[0]);
		return 0;
	}
	
	if ((inputMatrix = fopen(argv[1], "r")) == NULL){
		fprintf(stderr,"[%s] The input matrix file could not be readed\n",__func__);
		return 0;
	}
	
	if ((inputVector = fopen(argv[2], "r")) == NULL){
		fprintf(stderr,"[%s] The input vector file could not be readed\n",__func__);
		return 0;
	}
	
	MM_typecode inputmatcode;
	MM_typecode inputveccode;
	
	mm_read_banner(inputMatrix, inputmatcode);
	mm_read_banner(inputVector, inputveccode);
	
	if (mm_read_banner(inputMatrix, &inputmatcode) != 0){
		fprintf(stderr,"[%s] Could not read banner in the Matrix file\n",__func__);
		return 0;
	}
	
	if (mm_read_banner(inputVector, &inputveccode) != 0){
		fprintf(stderr,"[%s] Could not read banner in the Vector file\n",__func__);
		return 0;
	}
	
	if(!mm_is_dense(inputmatcode)){
		fprintf(stderr,"[%s] The matrix is not dense\n",__func__);
		return 0;
	}
	
	if(!mm_is_array(inputveccode)){
		fprintf(stderr,"[%s] The vector is not an array\n",__func__);
		return 0;
	}
	/*
	if ((ret_code = mm_read_mtx_crd_size(inputMatrix, &M, &N, &nz)) !=0)
        exit(1);
	
	unsigned int seed = 0;

	//int ret_code;

	

	
			
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
*/
	return 1;
}

