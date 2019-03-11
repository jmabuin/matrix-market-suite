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

#include "utils-MPI.h"



int mm_read_mtx_crd_data_mpi(FILE *f, unsigned long M, unsigned long N, unsigned long long nz, unsigned long I[], unsigned long J[], double val[], MM_typecode matcode, unsigned long startRow, unsigned long endRow, unsigned long long totalNz )
{
	
	unsigned long long i;
	unsigned long tmpI;
	unsigned long tmpJ;
	double tmpValue;
	if (mm_is_complex(matcode))
	{
		fprintf(stderr,"[%s] Reading complex matrix...\n",__func__);
		for (i=0; i<totalNz; i++)
			if (fscanf(f, "%lu %lu %lg %lg", &I[i], &J[i], &val[2*i], &val[2*i+1])
				!= 4) return MM_PREMATURE_EOF;
	}
	else if (mm_is_real(matcode))
	{
	
		if(!mm_is_symmetric(matcode)) {
			fprintf(stderr,"[%s] Reading NON symmetric real matrix...\n",__func__);
			for (i=0; i<totalNz; i++)
			{
				if (fscanf(f, "%lu %lu %lg\n", &I[i], &J[i], &val[i])
					!= 3) return MM_PREMATURE_EOF;

			}
		}
		else{
			fprintf(stderr,"[%s] Reading symmetric real matrix...\n",__func__);
			unsigned long long j;
			
			//unsigned long modifier;
			
			
			//if(startRow !=0) {
			//	modifier = startRow - 1;
			//}
			//else{
			//	modifier = startRow;
			//}
			
			for (i=0; i<M; i++) {
				
				for(j = 0; j<=i; j++) {
					
					if (fscanf(f, "%lu %lu %lg\n", &tmpI, &tmpJ, &tmpValue) != 3) {
						return MM_PREMATURE_EOF;
					}
					
					//If the value is in the same split, the row is the obtained from the file minus the startRow and the col is the same.
					//In the case of the symmetric value, the values are the same, but we swap row and col.
					if ( (i >= startRow) && (i <= endRow) && (j >= startRow) && (j <= endRow) && (i!=j)) {
						
						
						I[(i-startRow)*N+j] = tmpI - startRow;
						J[(i-startRow)*N+j] = tmpJ;
						val[(i-startRow)*N+j] = tmpValue;
						
						//fprintf(stderr,"[%s] %lu %lu %lg\n",__func__,startRow+I[(i-startRow)*N+j], J[(i-startRow)*N+j], val[(i-startRow)*N+j]);
						
						I[(j-startRow)*N+i] = J[(i-startRow)*N+j];
						J[(j-startRow)*N+i] = I[(i-startRow)*N+j];
						val[(j-startRow)*N+i] = val[(i-startRow)*N+j];
						
						//fprintf(stderr,"[%s] %lu %lu %lg\n",__func__,I[(j-startRow)*N+i], startRow+J[(j-startRow)*N+i], val[(j-startRow)*N+i]);
					}
					//Only the current value is in this split
					else if ( (i >= startRow) && (i <= endRow)) {
						
						I[(i-startRow)*N+j] = tmpI - startRow;
						J[(i-startRow)*N+j] = tmpJ;
						val[(i-startRow)*N+j] = tmpValue;
						
						//fprintf(stderr,"[%s] %lu %lu %lg\n",__func__,startRow+I[(i-startRow)*N+j], J[(i-startRow)*N+j], val[(i-startRow)*N+j]);
					}
					//The symmetric value is in this split. The row value is the current col minus the startRow and the col value is the currentrow value
					else if ( (j >= startRow) && (j <= endRow)) {
						
						I[(j-startRow)*N+i] = tmpJ - startRow;
						J[(j-startRow)*N+i] = tmpI;
						val[(j-startRow)*N+i] = tmpValue;
						
						//fprintf(stderr,"[%s] %lu %lu %lg\n",__func__,startRow+I[(j-startRow)*N+i], J[(j-startRow)*N+i], val[(j-startRow)*N+i]);
					}
					
				}
			}
		}
	}

	else if (mm_is_pattern(matcode))
	{
		fprintf(stderr,"[%s] Reading pattern matrix...\n",__func__);
		for (i=0; i<totalNz; i++)
			if (fscanf(f, "%lu %lu", &I[i], &J[i])
				!= 2) return MM_PREMATURE_EOF;
	}

	else {
		fprintf(stderr,"[%s] Unsupported type\n",__func__);
		return MM_UNSUPPORTED_TYPE;
	}
	
	return 0;
		
}



int mm_read_mtx_crd_data_mpi_rowperline(FILE *f, unsigned long M, unsigned long N, unsigned long long nz, unsigned long II[], unsigned long J[], double val[], MM_typecode matcode, unsigned long startRow, unsigned long endRow, unsigned long long totalNz ) {
	
	unsigned long long i;
	
	
	char * line 			= NULL;
	size_t len 			= 0;
	ssize_t read;
	
	char *token 			= NULL;
	char *valuesLine 		= NULL;
	
	int currentRow 			= -1;
	unsigned long currentCol 	= 0;
	
	if (mm_is_dense(matcode))
	{
		
		fprintf(stderr,"[%s] Reading dense real matrix...\n",__func__);
		
		
		for (i=0; i<M; i++) {
			
			while ((read = getline(&line, &len, f)) != -1) {
				token = strtok(line, ":");
				valuesLine = NULL;
	
	
				currentRow = -1;
	
				while(token){
	
					if(currentRow == -1){ //Parse row index
						currentRow = atoi(token);
						//fprintf(stderr,"[%s] Current row reading: %d\n",__func__,currentRow);
					}
		
					else{ //Get values
		
						valuesLine = (char *) malloc(sizeof(char)*strlen(token)+1);
						
						if (valuesLine == NULL) {
							fprintf(stderr,"[%s] Error reserving memory with malloc at row %d\n",__func__, currentRow);
							return -1;
						}
						
						strcpy(valuesLine,token);
			
					}
		
	
					token = strtok(0, ":");
				}
				

				free(token);

				
				//If the value is in the same split, the row is the obtained from the file minus the startRow and the col is the same.

				if ( (currentRow >= startRow) && (currentRow <= endRow) ) {
					
					//Parse values
					//fprintf(stderr,"[%s][%lu] Reading row %d\n",__func__, startRow, currentRow);
					token = strtok(valuesLine, ",");	
					
					currentCol = 0;
					
		
					while(token){

						if(currentCol < N){

							II[(currentRow-startRow)*N+currentCol] = currentRow-startRow;
							J[(currentRow-startRow)*N+currentCol] = currentCol;
							val[(currentRow-startRow)*N+currentCol] = atof(token);
		
							currentCol++;
			
						}
						
						token = strtok(0, ",");
					}


				}
				

				
				free(valuesLine);
			
				free(token);

				//free(valuesLine);
				//free(token);
			}
				
		}
	}
	
	
	return 0;
		
}


/* Read matrices
 *
 */
 
int readDenseCoordinateMatrixMPI(char *fileName,unsigned long **II,unsigned long **J, double **values,unsigned long *M,unsigned long *local_M,unsigned long *N, unsigned long long *nz, int myid, int numProcs) {

	FILE *inputMatrix;
	
	int ret_code;
	
	unsigned long startRow;
	unsigned long endRow;
	unsigned long long totalNz;
	
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

	totalNz = *nz;

	//The distribution is going to be made by rows
	
	*local_M = (*M)/(unsigned long int)numProcs;
	
	//fprintf(stderr,"[%s] Number of non zeros 1: %llu\n",__func__,*nz);
	if ( myid < (numProcs - 1) ){
	
		startRow = myid * (*local_M);
		endRow = startRow + (*local_M) - 1;
		*nz = (*local_M)*(*N);
	}
	else{
		
		startRow = myid * (*local_M);
		endRow = *M - 1;
		
		*nz = *nz - startRow * (*N);
	}

	*II = NULL;
	*J = NULL;
	*values=NULL;

	*II = (unsigned long *)  malloc(*nz * sizeof(unsigned long));
	*J = (unsigned long *)  malloc(*nz * sizeof(unsigned long));
	*values = (double *)  malloc(*nz * sizeof(double));
	
	if(mm_read_mtx_crd_data_mpi(inputMatrix,*M,*N,*nz,*II,*J,*values,inputmatcode,startRow, endRow, totalNz) != 0){
		fprintf(stderr,"[%s] Error reading matrix values\n",__func__);
		return 0;
	}
	

	fclose(inputMatrix);


	//fprintf(stderr,"[%s] Number of non zeros 2: %llu\n",__func__,*nz);
	//fprintf(stderr,"[%s] Start: %lu, end: %lu\n",__func__,startRow, endRow);
	//mm_write_mtx_crd("stdout",*M,*N,*nz,I,J,values,inputmatcode);

	return 1;
}


int readDenseCoordinateMatrixMPIRowLine(char *fileName,unsigned long **II,unsigned long **J, double **values,unsigned long *M,unsigned long *local_M,unsigned long *N, unsigned long long *nz, int myid, int numProcs) {

	FILE *inputMatrix;
	
	int ret_code;
	
	unsigned long startRow;
	unsigned long endRow;
	unsigned long long totalNz;
	
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
		fprintf(stderr,"[%s] The matrix is not coordinate\n",__func__);
		return 0;
	}
	
	if ((ret_code = mm_read_mtx_crd_size(inputMatrix, M, N, nz)) !=0) {
		fprintf(stderr,"[%s] Could not read matrix size\n",__func__);
		return 0;
	}

	totalNz = *nz;

	//The distribution is going to be made by rows
	
	*local_M = (*M)/(unsigned long int)numProcs;
	
	//fprintf(stderr,"[%s] Number of non zeros 1: %llu\n",__func__,*nz);
	if ( myid < (numProcs - 1) ){
	
		startRow = myid * (*local_M);
		endRow = startRow + (*local_M) - 1;
		*nz = (*local_M)*(*N);
	}
	else{
		
		startRow = myid * (*local_M);
		endRow = *M - 1;
		
		*nz = *nz - startRow * (*N);
	}

	*II = NULL;
	*J = NULL;
	*values=NULL;

	*II = (unsigned long *)  malloc(*nz * sizeof(unsigned long));
	*J = (unsigned long *)  malloc(*nz * sizeof(unsigned long));
	*values = (double *)  malloc(*nz * sizeof(double));
	
	if(mm_read_mtx_crd_data_mpi_rowperline(inputMatrix,*M,*N,*nz,*II,*J,*values,inputmatcode,startRow, endRow, totalNz) != 0){
		fprintf(stderr,"[%s] Error reading matrix values\n",__func__);
		return 0;
	}
	

	fclose(inputMatrix);


	//fprintf(stderr,"[%s] Number of non zeros 2: %llu\n",__func__,*nz);
	//fprintf(stderr,"[%s] Start: %lu, end: %lu\n",__func__,startRow, endRow);
	//mm_write_mtx_crd("stdout",*M,*N,*nz,I,J,values,inputmatcode);

	return 1;
}





