//
// Created by chema on 3/13/19.
//

#ifndef MATRIX_MARKET_SUITE_CONJUGATEGRADIENTSOLVER_CUDA_H
#define MATRIX_MARKET_SUITE_CONJUGATEGRADIENTSOLVER_CUDA_H

extern "C" {
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
}

#define EPSILON 1.0e-5       /* Convergence criterion */

int ConjugateGradientSolverCUDA(unsigned long *II, unsigned long *J, double *A, unsigned long M, unsigned long N, unsigned long long nz, double *b, unsigned long M_Vector, unsigned long N_Vector, unsigned long long nz_vector, double *x, int iterationNumber);

#endif //MATRIX_MARKET_SUITE_CONJUGATEGRADIENTSOLVER_CUDA_H
