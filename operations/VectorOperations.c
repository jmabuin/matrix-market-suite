#include "VectorOperations.h"

void vector_substract(double* a,double* b, double* c, unsigned long n) {

	unsigned long int i = 0;

	for(i =0;i<n;i++){
		c[i] = a[i] - b[i];
	}

}
