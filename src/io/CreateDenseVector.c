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


#include "../utils/utils.h"
#include "CreateDenseVector.h"

void usageCreateDenseVector() {

	fprintf(stderr, "\n");
	fprintf(stderr, "Usage: MM-Suite CreateDenseVector [options] <num-items> <seed>\n");
	fprintf(stderr, "\nInput/output options:\n\n");
	fprintf(stderr, "       -o STR        Output file name. Default: stdout\n");
	fprintf(stderr, "\n");

}

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

	char	*outputFileName = NULL;

	//int ret_code;

	int 	option;
	
	while ((option = getopt(argc, argv,"o:")) >= 0) {
		switch (option) {
			case 'o' : 
				//free(outputFileName);
				
				outputFileName = (char *) malloc(sizeof(char)*strlen(optarg)+1);
				strcpy(outputFileName,optarg);
				
				break;
				
			default: break;
		}
	
	}
	
	if (optind + 2 != argc) {
		usageCreateDenseVector();
		return 0;
	}
	

	if(outputFileName == NULL) {
		output = stderr;
		
	}
	else {
		if ((output = fopen(outputFileName, "w")) == NULL){
			return 0;
		} 
	}
	
	numItems = atol(argv[optind]);
	seed = atoi(argv[optind+1]);
	
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

