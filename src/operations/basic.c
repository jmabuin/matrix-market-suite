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

#include "basic.h"

void mms_dgemv(int m, int n,double alpha, double *matrix, double *vector,double beta, double *result){

	//double *result = (double *) calloc(m,sizeof(double));

	int i = 0;
	int j = 0;
	for(i=0; i<m; i++){
		
		result[i] = 0.0;
		
		for(j = 0; j< n; j++){
		
			result[i] = result[i] + matrix[i*n+j]*vector[j];
		}
	
	}
	
	//return result;

}

void mms_dscal(int m,double value, double *vector){

	int i = 0;
	
	for(i = 0; i<m; i++){
		vector[i] = value*vector[i];
	}
}

//val =
double mms_ddot(int m,double *x, double *y) {

	int i = 0;
	
	double result = 0.0;
	
	for(i = 0; i<m; i++){
		result = result + x[i] * y[i];
	}
	
	return result;
}

//y = alpha*x+y
void mms_daxpy(int m, double alpha,double *x,double *y) {

	int i = 0;
	
	for(i=0; i< m; i++) {
	
		y[i] = alpha*x[i] + y [i];
	}
	

}
