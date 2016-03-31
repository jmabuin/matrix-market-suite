
#include <stdio.h>
#include <stdlib.h>
#include "CreateDenseMatrixSymmetric.h"

//int CreateDenseMatrixSymmetric(unsigned long numRows,unsigned long numCols, char *fileName, unsigned int seed) {
int CreateDenseMatrixSymmetric(int argc, char *argv[]) {
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
	mm_set_symmetric(&outputmatcode);
	
	unsigned long int numRows = 0;
	unsigned long int numCols = 0;
	unsigned int seed = 0;

	double valor = 0.0;

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
	
	double *values =  (double *) calloc(nnz,sizeof(double));
	
	//nz = numRows * numCols;
	//sscanf (argv[3],"%d",&blockSize);
	
	srand (seed);

	mm_write_banner(output, outputmatcode);
	mm_write_mtx_crd_size(output, numRows, numCols, numRows*numCols);
	//ret_code = fprintf(output,"\%\%MatrixMarket matrix coordinate real symmetric\n");
	
	unsigned long long int val1 = 0;
	unsigned long long int val2 = 0;
	

	
	for(i = 0;i < numRows; i++){
	
		//This loop is for assigning values to places that are no in the diagonal
		for(j = 0; j<i; j++){

			//valor = ((double)rand() / (double)RAND_MAX)/100;
			valor = ((double)rand() / (double)RAND_MAX);
			val1 = j+(i*numCols);
			values[val1] = valor;
			
			val2 = i+(j*numCols);
			values[val2] = valor;


		}

		//Elements in the diagonal, i = j
		
		//valor = ((double)rand() / (double)RAND_MAX)/100;
		valor = ((double)rand() / (double)RAND_MAX);
		values[j+(i*numCols)] = valor;
	}
	
	//Write to file
	for(i = 0;i < numRows; i++){

		for(j = 0; j<numCols; j++){
			
			fprintf(output, "%d %d %f\n",i+1,j+1,values[j+(i*numCols)]);

		}

	}

	fclose(output);

	return 1;
}

