#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "mmio.h"

double cputime();
double systemtime();
double usertime();
double realtime();

int readDenseCoordinateMatrix(char *fileName,unsigned long **I,unsigned long **J, double **values,unsigned long *M,unsigned long *N, unsigned long long *nz);
int readDenseVector(char *fileName, double **values,unsigned long *M,unsigned long *N, unsigned long long *nz);

