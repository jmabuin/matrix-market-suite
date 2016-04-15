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


#include "ConjugateGradient.h"
#include "ConjugateGradientSolver.h"

int ConjugateGradient(int argc, char *argv[]) {
//Options: InputMatrixFile InputVectorFile

	
	//int i;
	int ret_code;
	
	unsigned long *I;
	unsigned long *J;
	double *A;
	
	unsigned long M;
	unsigned long N;
	unsigned long long nz;
	
	
	double *b;
	unsigned long M_Vector;
	unsigned long N_Vector;
	unsigned long long nz_vector;
	
	int write2file = 0;
	
	if (argc < 3)
	{
		fprintf(stderr, "[%s] Usage: %s [input-matrix-file] [input-vector-file]\n",__func__, argv[0]);
		return 0;
	}
	
	if(argc == 4){
		write2file = 1;
	}
	
	//Read matrix
	if(!readDenseCoordinateMatrix(argv[1],&I,&J,&A,&M,&N,&nz)){
		fprintf(stderr, "[%s] Can not read Matrix\n",__func__);
		return 0;
	}
	
	//Read vector
	if(!readDenseVector(argv[2], &b,&M_Vector,&N_Vector,&nz_vector)){
		fprintf(stderr, "[%s] Can not read Vector\n",__func__);
		return 0;
	}
	
	
        
        //double *y=(double *) malloc(nz_vector * sizeof(double));
        fprintf(stderr,"[%s] Solving system using conjugate gradient method\n",__func__);
	ret_code = ConjugateGradientSolver(I,J,A,M,N,nz,b,M_Vector,N_Vector,nz_vector);
	
	if(ret_code){
	
		if(write2file){
			//writeDenseVector(argv[3], final_result,M_Vector,N_Vector,nz_vector);
			writeDenseVector(argv[3], b,M_Vector,N_Vector,nz_vector);
		}
		else{
			writeDenseVector("stdout", b,M_Vector,N_Vector,nz_vector);
		}
	
	}
	else{
		fprintf(stderr,"[%s] Error executing ConjugateGradientSolver\n",__func__);
	
	}
	return ret_code;
}

