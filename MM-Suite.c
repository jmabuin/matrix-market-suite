#include <stdio.h>
#include <string.h>
#include "utils/utils.h"

#include "io/CreateDenseMatrixSymmetric.h"
#include "io/CreateDenseVector.h"

#include "operations/DMxV.h"

#include "solvers/ConjugateGradient.h"

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "0.0.2"
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
	fprintf(stderr, "         CreateDenseVector                  creates a dense vector\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Basic operations:\n");
	fprintf(stderr, "Command: DMxV                               Dense matrix dot vector operation\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Solvers:\n");
	fprintf(stderr, "Command: ConjugateGradient                  Solves a system by using the conjugate gradient method\n");
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

	if (strcmp(argv[1], "CreateDenseMatrixSymmetric") == 0) ret = CreateDenseMatrixSymmetric(argc-1, argv+1);
	else if (strcmp(argv[1], "CreateDenseVector") == 0) 	ret = CreateDenseVector(argc-1, argv+1);
	else if (strcmp(argv[1], "DMxV") == 0) 			ret = DMxV(argc-1, argv+1);
	else if (strcmp(argv[1], "ConjugateGradient") == 0)	ret = ConjugateGradient(argc-1, argv+1);
	else {
		fprintf(stderr, "[%s] unrecognized command '%s'\n",__func__, argv[1]);
		return 1;
	}
	
	
	if (ret == 0) {
		fprintf(stderr, "[%s] Version: %s\n", __func__, PACKAGE_VERSION);
		fprintf(stderr, "[%s] CMD:", __func__);
		for (i = 0; i < argc; ++i)
			fprintf(stderr, " %s", argv[i]);
		
	}

	fprintf(stderr, "\n[%s] Real time: %.3f sec; CPU: %.3f sec; User: %.3f sec; Sys: %3f sec\n", __func__, realtime() - t_real, cputime(),usertime(),systemtime());

	return ret;
}
