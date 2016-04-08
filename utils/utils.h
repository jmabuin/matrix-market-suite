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

#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mmio.h"

double cputime();
double systemtime();
double usertime();
double realtime();

int readDenseCoordinateMatrix(char *fileName,unsigned long **I,unsigned long **J, double **values,unsigned long *M,unsigned long *N, unsigned long long *nz);
int readDenseVector(char *fileName, double **values,unsigned long *M,unsigned long *N, unsigned long long *nz);
int writeDenseVector(char *fileName, double *values,unsigned long M,unsigned long N, unsigned long long nz);
