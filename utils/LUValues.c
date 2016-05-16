

#include "LUValues.h"

void getLValues(double *L, double *values,unsigned long M,unsigned long N, unsigned long long nz) {

	for(int i= 0; i< M; i++){
	
		for(int j = 0; j<N;j++){
		
			if(i<j){
				L[i*N+j] = values[i*N+j];
			}
			else{
				L[i*N+j] = 0.0;
			}
		
		}
	}

}

void getUValues(double *U, double *values,unsigned long M,unsigned long N, unsigned long long nz) {

	for(int i= 0; i< M; i++){
	
		for(int j = 0; j<N;j++){
		
			if(i>j){
				U[i*N+j] = values[i*N+j];
			}
			else{
				U[i*N+j] = 0.0;
			}
		
		}
	}

}

void getLUValues(double *LU, double *values,unsigned long M,unsigned long N, unsigned long long nz) {

	for(int i= 0; i< M; i++){
	
		for(int j = 0; j<N;j++){
		
			if(i!=j){
				LU[i*N+j] = values[i*N+j];
			}
			else{
				LU[i*N+j] = 0.0;
			}
		
		}
	}

}

void getDValues(double *D, double *values,unsigned long M,unsigned long N, unsigned long long nz) {

	for(int i= 0; i< M; i++){
	
		for(int j = 0; j<N;j++){
		
			if(i==j){
				D[i*N+j] = values[i*N+j];
			}
			else{
				D[i*N+j] = 0.0;
			}
		
		}
	}

}

void getDInvValues(double *D, double *values,unsigned long M,unsigned long N, unsigned long long nz) {

	for(int i= 0; i< M; i++){
	
		for(int j = 0; j<N;j++){
		
			if(i==j){
				D[i*N+j] = 1.0/values[i*N+j];
			}
			else{
				D[i*N+j] = 0.0;
			}
		
		}
	}

}

void getDInvNegValues(double *D, double *values,unsigned long M,unsigned long N, unsigned long long nz) {

	for(int i= 0; i< M; i++){
	
		for(int j = 0; j<N;j++){
		
			if(i==j){
				D[i*N+j] = -1.0/values[i*N+j];
			}
			else{
				D[i*N+j] = 0.0;
			}
		
		}
	}

}
