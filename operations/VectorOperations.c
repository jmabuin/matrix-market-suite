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

#include "VectorOperations.h"

void vector_substract(double* a,double* b, double* c, unsigned long n) {

	unsigned long int i = 0;

	for(i =0;i<n;i++){
		c[i] = a[i] - b[i];
	}

}

//Sum of all elements of a vector
double vectorSumElements(double *values, unsigned int dim) {

	double result = 0.0;
	
	unsigned int i = 0;
	
	for (i = 0; i< dim; i++){
	
		result = result + values[i];
	
	}
	
	return result;
	

}
