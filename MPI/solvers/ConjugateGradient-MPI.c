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
#include <unistd.h>

#include "ConjugateGradient-MPI.h"
#include "ConjugateGradientSolver-MPI.h"

void usageConjugateGradientMPI(){

	fprintf(stderr, "\n");
	fprintf(stderr, "Usage: MM-Suite ConjugateGradient [options] <input-matrix> <input-vector>\n");
	fprintf(stderr, "Algorithm options:\n\n");
	fprintf(stderr, "       -i INT        Iteration number. Default: number of matrix rows * 2\n");
	fprintf(stderr, "\nInput/output options:\n\n");
	fprintf(stderr, "       -o STR        Output file name. Default: stdout\n");
	fprintf(stderr, "\n");

}

int ConjugateGradientMPI(int argc, char *argv[],int numProcs, int myid) {

	int 			ret_code;
	int 			option;
	
	unsigned long 		*I;
	unsigned long 		*J;
	double 			*A;
	
	unsigned long 		M;
	unsigned long 		local_M;
	unsigned long 		N;
	unsigned long long 	nz;
	
	
	double 			*b;
	unsigned long 		M_Vector;
	unsigned long 		N_Vector;
	unsigned long long 	nz_vector;
	
	char			*outputFileName = NULL;
	int			iterationNumber = 0;
	
	char			*inputMatrixFile = NULL;
	char			*inputVectorFile = NULL;
	
	int			inputFormatRow = 0;
	
	while ((option = getopt(argc, argv,"ro:i:")) >= 0) {
		switch (option) {
			case 'o' : 
				//free(outputFileName);
				
				outputFileName = (char *) malloc(sizeof(char)*strlen(optarg)+1);
				strcpy(outputFileName,optarg);
				break;
				
			case 'i' :
				iterationNumber = atoi(optarg);
				break;
				
			case 'r':
				inputFormatRow = 1;
				break;
				
			default: break;
		}
	
	}
	
	if ((optind + 2 > argc) || (optind + 3 <= argc)) {
		usageConjugateGradientMPI();
		return 0;
	}
	
	if(outputFileName == NULL) {
		outputFileName = (char *) malloc(sizeof(char)*6);
		sprintf(outputFileName,"stdout");
	}
	
	inputMatrixFile = (char *)malloc(sizeof(char)*strlen(argv[optind])+1);
	inputVectorFile = (char *)malloc(sizeof(char)*strlen(argv[optind+1])+1);
	
	strcpy(inputMatrixFile,argv[optind]);
	strcpy(inputVectorFile,argv[optind+1]);
	
	//Read matrix
	if(inputFormatRow) {
		if(!readDenseCoordinateMatrixMPIRowLine(inputMatrixFile,&I,&J,&A,&M,&local_M,&N,&nz,myid, numProcs)){
			fprintf(stderr, "[%s] Can not read Matrix\n",__func__);
			return 0;
		}
	}
	else{
		if(!readDenseCoordinateMatrixMPI(inputMatrixFile,&I,&J,&A,&M,&local_M,&N,&nz,myid, numProcs)){
			fprintf(stderr, "[%s] Can not read Matrix\n",__func__);
			return 0;
		}
	}
	
	
	//Read vector
	if(!readDenseVector(inputVectorFile, &b,&M_Vector,&N_Vector,&nz_vector)){
		fprintf(stderr, "[%s] Can not read Vector\n",__func__);
		return 0;
	}
	
	
        
        //double *y=(double *) malloc(nz_vector * sizeof(double));
        fprintf(stderr,"[%s] Solving system using conjugate gradient method...\n",__func__);
	ret_code = ConjugateGradientSolverMPI(I,J,A,M,local_M,N,nz,b,M_Vector,N_Vector,nz_vector, iterationNumber);
	
	if (myid == 0) {
		if(ret_code){

			writeDenseVector(outputFileName, b,M_Vector,N_Vector,nz_vector);
		}
		else{
			fprintf(stderr,"[%s] Error executing ConjugateGradientSolver\n",__func__);
	
		}
	}
	return ret_code;
}

