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

#include "utils.h"

/*********
 * Timer *
 *********/

double cputime() {
	struct rusage r;
	getrusage(RUSAGE_SELF, &r);
	return r.ru_utime.tv_sec + r.ru_stime.tv_sec + 1e-6 * (r.ru_utime.tv_usec + r.ru_stime.tv_usec);
}

double systemtime() {
	struct rusage r;
	getrusage(RUSAGE_SELF, &r);
	return r.ru_stime.tv_sec + 1e-6 * (r.ru_stime.tv_usec);
}

double usertime() {
	struct rusage r;
	getrusage(RUSAGE_SELF, &r);
	return r.ru_utime.tv_sec + 1e-6 * (r.ru_utime.tv_usec);
}

double realtime() {
	struct timeval tp;
	struct timezone tzp;
	gettimeofday(&tp, &tzp);
	return tp.tv_sec + tp.tv_usec * 1e-6;
}


/* Read matrices
 *
 */
 
int readDenseCoordinateMatrix(char *fileName,unsigned long **I,unsigned long **J, double **values,unsigned long *M,unsigned long *N, unsigned long long *nz) {

	FILE *inputMatrix;
	
	int ret_code;
	
	MM_typecode inputmatcode;
	
	if ((inputMatrix = fopen(fileName, "r")) == NULL) {
		fprintf(stderr,"[%s] Error opening matrix file\n",__func__);
		return 0;
	}
	
	//mm_read_banner(inputMatrix, &inputmatcode);
	
	if (mm_read_banner(inputMatrix, &inputmatcode) != 0){
		fprintf(stderr,"[%s] Could not read banner in the Matrix file\n",__func__);
		return 0;
	}
	
	
	if(!mm_is_coordinate(inputmatcode)){
		fprintf(stderr,"[%s] The matrix is not coordinate\n",__func__);
		return 0;
	}
	
	
	if ((ret_code = mm_read_mtx_crd_size(inputMatrix, M, N, nz)) !=0) {
		fprintf(stderr,"[%s] Could not read matrix size\n",__func__);
		return 0;
	}

	*I = NULL;
	*J = NULL;
	*values=NULL;

	*I = (unsigned long *)  malloc(*nz * sizeof(unsigned long));
	*J = (unsigned long *)  malloc(*nz * sizeof(unsigned long));
	*values = (double *)  malloc(*nz * sizeof(double));
	
	if(mm_read_mtx_crd_data(inputMatrix,*M,*N,*nz,*I,*J,*values,inputmatcode) != 0){
		fprintf(stderr,"[%s] Error reading matrix values\n",__func__);
		return 0;
	}
	

	fclose(inputMatrix);

	//mm_write_mtx_crd("stdout",*M,*N,*nz,I,J,values,inputmatcode);

	return 1;
}


int readDenseCoordinateMatrixRowLine(char *fileName,unsigned long **I,unsigned long **J, double **values,unsigned long *M,unsigned long *N, unsigned long long *nz) {

	FILE *inputMatrix;
	
	int ret_code;
	int i,j;
	
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	
	MM_typecode inputmatcode;
	
	if ((inputMatrix = fopen(fileName, "r")) == NULL) {
		fprintf(stderr,"[%s] Error opening matrix file\n",__func__);
		return 0;
	}
	
	//mm_read_banner(inputMatrix, &inputmatcode);
	
	if (mm_read_banner(inputMatrix, &inputmatcode) != 0){
		fprintf(stderr,"[%s] Could not read banner in the Matrix file\n",__func__);
		return 0;
	}
	
	
	if(!mm_is_dense(inputmatcode)){
		fprintf(stderr,"[%s] The matrix is not a row per line matrix\n",__func__);
		return 0;
	}
	
	
	if ((ret_code = mm_read_mtx_crd_size(inputMatrix, M, N, nz)) !=0) {
		fprintf(stderr,"[%s] Could not read matrix size\n",__func__);
		return 0;
	}

	*I = NULL;
	*J = NULL;
	*values=NULL;

	*I = (unsigned long *)  malloc(*nz * sizeof(unsigned long));
	*J = (unsigned long *)  malloc(*nz * sizeof(unsigned long));
	*values = (double *)  malloc(*nz * sizeof(double));
	
	unsigned int *tmpI = *I;
	unsigned int *tmpJ = *J;
	double *tmpValues = *values;
	
	//Read rows
	
	//for (i = 0; i< numRows, i++){
	
	unsigned long currentValue = 0;
	
	while ((read = getline(&line, &len, inputMatrix)) != -1) {
		
		char* token = strtok(line, ":");
		char *valuesLine;
		
		
		int currentRow = -1;
		
		while(token){
		
			if(currentRow == -1){ //Parse row index
				currentRow = atoi(token);
				//fprintf(stderr,"[%s] Current row reading: %d\n",__func__,currentRow);
			}
			
			else{ //Get values
			
				valuesLine = (char *) malloc(sizeof(char)*strlen(token));
				strcpy(valuesLine,token);
				
				//fprintf(stderr,"[%s] Current string for values: %s\n",__func__,valuesLine);
				
			
			}
			
		
			token = strtok(0, ":");
		
		}
		
		
		token = strtok(valuesLine, ",");
		
		unsigned long currentCol = 0;
		
		
		while(token){

			
			if((currentRow < *M) && (currentCol < *N) && (currentValue < *nz)){
			
			
				tmpI[currentValue] = (unsigned int)currentRow;
				tmpJ[currentValue] = currentCol;
			
				tmpValues[currentValue] = atof(token);
		
				currentValue++;
				currentCol++;
			
			}

			token = strtok(0, ",");

		}
		
		free(valuesLine);
		
	}
	
	

	fclose(inputMatrix);

	//mm_write_mtx_crd("stdout",*M,*N,*nz,I,J,values,inputmatcode);

	return 1;
}


int readDenseVector(char *fileName, double **values,unsigned long *M,unsigned long *N, unsigned long long *nz) {

	FILE *inputVector;
	
	int ret_code;
	
	MM_typecode inputmatcode;
	
	if ((inputVector = fopen(fileName, "r")) == NULL) {
		fprintf(stderr,"[%s] Error opening vector file\n",__func__);
		return 0;
	}
	
	
	if (mm_read_banner(inputVector, &inputmatcode) != 0){
		fprintf(stderr,"[%s] Could not read banner in the Vector file\n",__func__);
		return 0;
	}
	
	/*
	if(!mm_is_dense(inputmatcode)){
		fprintf(stderr,"[%s] The matrix is not dense\n",__func__);
		return 0;
	}
	*/
	
	if ((ret_code = mm_read_mtx_array_size(inputVector, M, N)) !=0) {
		fprintf(stderr,"[%s] Could not read matrix size\n",__func__);
		return 0;
	}

	*nz = (unsigned long long)(*M)*(unsigned long long)(*N);

	*values=NULL;

	*values = (double *)  malloc(*nz * sizeof(double));
	

	if(mm_read_vector_data(inputVector,*nz,*values,inputmatcode) != 0){
		fprintf(stderr,"[%s] Error reading vector values\n",__func__);
		return 0;
	}
	

	fclose(inputVector);


	return 1;

}


int writeDenseVector(char *fileName, double *values,unsigned long M,unsigned long N, unsigned long long nz) {

	FILE *f;
	unsigned long i = 0;
	
	if(strcmp(fileName,"stdout")==0){
		f = stdout;
	}
	else{
		if ((f = fopen(fileName, "w")) == NULL){
			fprintf(stderr,"[%s] Unable to open file for writing\n",__func__);
			return 0;
		}
	}
	
	MM_typecode outputmatcode;
	
	mm_initialize_typecode(&outputmatcode);
	mm_set_array(&outputmatcode);
	mm_set_dense(&outputmatcode);
	mm_set_real(&outputmatcode);

	
	mm_write_banner(f, outputmatcode);
	mm_write_mtx_array_size(f, M, N);
	
	for(i = 0;i < M; i++){

		fprintf(f, "%lg\n",values[i]);
	}
	
	if(f!=stdout){
		fclose(f);
	}
	
	return 1;
}



