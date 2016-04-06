#include <math.h>

#include "../utils/utils.h"
#include "../operations/VectorOperations.h"

#define EPSILON 1.0e-5       /* Convergence criterion */

int ConjugateGradientSolver(unsigned long *I, unsigned long *J, double *A, unsigned long M, unsigned long N, unsigned long long nz, double *b, unsigned long M_Vector, unsigned long N_Vector, unsigned long long nz_vector);
