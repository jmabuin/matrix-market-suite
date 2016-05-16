#include <stdio.h>
#include <string.h>
#include "utils/utils.h"

#include "io/CreateDenseMatrixSymmetric.h"
#include "io/CreateDenseMatrixSymmetricRowLine.h"
#include "io/CreateDenseVector.h"
#include "io/CreateDenseMatrixGeneral.h"
#include "io/CreateSparseMatrixGeneral.h"
#include "io/CreateDenseMatrixGeneralRowLine.h"

#include "operations/DMxV.h"
#include "operations/LUDecomposition.h"
#include "operations/DMxDM.h"

#include "solvers/ConjugateGradient.h"
#include "solvers/Jacobi.h"

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "0.2.0"
#endif


static int usage()
{
	fprintf(stderr, "\n");
	fprintf(stderr, "Program: MM-Suite (perform operations to create and operate over matrix market files)\n");
	fprintf(stderr, "Version: %s\n", PACKAGE_VERSION);
	fprintf(stderr, "Contact: José M. Abuín <josemanuel.abuin@usc.es>\n\n");
	fprintf(stderr, "Usage:   MM-Suite <command> [options]\n\n");
	fprintf(stderr, "Input/Output:\n");
	fprintf(stderr, "Command: CreateDenseMatrixSymmetric         creates a dense symmetric matrix\n");
	fprintf(stderr, "         CreateDenseMatrixSymmetricRowLine  creates a dense symmetric matrix where each line represents a matrix row\n");
	fprintf(stderr, "         CreateDenseVector                  creates a dense vector\n");
	fprintf(stderr, "         CreateDenseMatrixGeneral           creates a dense general matrix\n");
	fprintf(stderr, "         CreateSparseMatrixGeneral          creates a sparse general matrix\n");
	fprintf(stderr, "         CreateDenseMatrixGeneralRowLine    creates a dense general matrix where each line represents a matrix row\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Basic operations:\n");
	fprintf(stderr, "Command: DMxV                               Dense matrix dot vector operation\n");
	fprintf(stderr, "         DMxDM                              Dense matrix dot dense matrix operation\n");
	fprintf(stderr, "         LUDecomposition                    LU factorization of a matrix\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Solvers:\n");
	fprintf(stderr, "Command: ConjugateGradient                  Solves a system by using the conjugate gradient method\n");
	fprintf(stderr, "         Jacobi                             Solves a system by using the Jacobi method\n");
	fprintf(stderr, "\n");
	return 1;
}


int main(int argc, char *argv[])
{

	int i, ret;
	double t_real;

	t_real = realtime();

	
	fprintf(stderr, "MM-Suite\tVN:%s\tCL:%s", PACKAGE_VERSION, argv[0]);
	
	for (i = 1; i < argc; ++i) fprintf(stderr, " %s", argv[i]);
	
	fprintf(stderr, "\n");

	if (argc < 2) return usage();

	if (strcmp(argv[1], "CreateDenseMatrixSymmetric") == 0) 		ret = CreateDenseMatrixSymmetric(argc-1, argv+1);
	else if (strcmp(argv[1], "CreateDenseMatrixSymmetricRowLine") == 0) 	ret = CreateDenseMatrixSymmetricRowLine(argc-1, argv+1);
	else if (strcmp(argv[1], "CreateDenseVector") == 0) 			ret = CreateDenseVector(argc-1, argv+1);
	else if (strcmp(argv[1], "DMxV") == 0) 					ret = DMxV(argc-1, argv+1);
	else if (strcmp(argv[1], "ConjugateGradient") == 0)			ret = ConjugateGradient(argc-1, argv+1);
	else if (strcmp(argv[1], "LUDecomposition") == 0)			ret = LUDecomposition(argc-1, argv+1);
	else if (strcmp(argv[1], "DMxDM") == 0)					ret = DMxDM(argc-1, argv+1);
	else if (strcmp(argv[1], "CreateDenseMatrixGeneral") == 0)		ret = CreateDenseMatrixGeneral(argc-1, argv+1);
	else if (strcmp(argv[1], "CreateSparseMatrixGeneral") == 0)		ret = CreateSparseMatrixGeneral(argc-1, argv+1);
	else if (strcmp(argv[1], "CreateDenseMatrixGeneralRowLine") == 0)	ret = CreateDenseMatrixGeneralRowLine(argc-1, argv+1);
	else if (strcmp(argv[1], "Jacobi") == 0)				ret = Jacobi(argc-1, argv+1);
	else {
		fprintf(stderr, "[%s] unrecognized command '%s'\n",__func__, argv[1]);
		return 1;
	}
	
	
	if (ret == 0) {
		fprintf(stderr, "[%s] ERROR!\n", __func__);
		fprintf(stderr, "[%s] Version: %s\n", __func__, PACKAGE_VERSION);
		fprintf(stderr, "[%s] CMD:", __func__);
		for (i = 0; i < argc; ++i)
			fprintf(stderr, " %s", argv[i]);
		
	}
	else{
		fprintf(stderr, "[%s] DONE!", __func__);
	}

	fprintf(stderr, "\n[%s] Real time: %.3f sec; CPU: %.3f sec; User: %.3f sec; Sys: %3f sec\n", __func__, realtime() - t_real, cputime(),usertime(),systemtime());

	return ret;
}
