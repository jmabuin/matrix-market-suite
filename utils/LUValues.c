/**
  * Copyright 2017 José Manuel Abuín Mosquera <josemanuel.abuin@usc.es>
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
  
#include "LUValues.h"

/**
 * \brief Function that gets the lower values from a matrix. Diagonal not included
 * \param L Return matrix L where lower values will be stored
 * \param values Input matrix
 * \param M Number of rows for input and output matrices
 * \param N Number of columns for input and output matrices
 * \param nz Number of non zero items in the matrix
 */
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

/**
 * \brief Function that gets the upper values from a matrix. Diagonal not included
 * \param U Return matrix U where upper values will be stored
 * \param values Input matrix
 * \param M Number of rows for input and output matrices
 * \param N Number of columns for input and output matrices
 * \param nz Number of non zero items in the matrix
 */
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

/**
 * \brief Function that gets the upper and lower values from a matrix. Diagonal not included
 * \param LU Return matrix LU where upper and lower values will be stored
 * \param values Input matrix
 * \param M Number of rows for input and output matrices
 * \param N Number of columns for input and output matrices
 * \param nz Number of non zero items in the matrix
 */
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

/**
 * \brief Function that gets the diagonal values from a matrix
 * \param D Return matrix D where diagonal values will be stored
 * \param values Input matrix
 * \param M Number of rows for input and output matrices
 * \param N Number of columns for input and output matrices
 * \param nz Number of non zero items in the matrix
 */
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

/**
 * \brief Function that gets the inverse of the diagonal values from a matrix
 * \param D Return matrix D where the inverse of the diagonal values will be stored
 * \param values Input matrix
 * \param M Number of rows for input and output matrices
 * \param N Number of columns for input and output matrices
 * \param nz Number of non zero items in the matrix
 */
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

/**
 * \brief Function that gets the negative inverse of the diagonal values from a matrix
 * \param D Return matrix D where the negative inverse of the diagonal values will be stored
 * \param values Input matrix
 * \param M Number of rows for input and output matrices
 * \param N Number of columns for input and output matrices
 * \param nz Number of non zero items in the matrix
 */
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
