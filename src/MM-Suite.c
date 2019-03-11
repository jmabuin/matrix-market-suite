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

#include <stdio.h>
#include <string.h>
#include "utils/utils.h"

#include "io/CreateDenseMatrix.h"
#include "io/CreateDenseVector.h"
#include "io/CreateSparseMatrix.h"


#include "operations/DMxV.h"
#include "operations/LUDecomposition.h"
#include "operations/DMxDM.h"

#include "solvers/ConjugateGradient.h"
#include "solvers/Jacobi.h"

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "0.3.2"
#endif

typedef enum {TYPECOM, TYPEOP, TYPESOLVER} commandtype;

struct st_command {
	const char * name;
	const char * description;
	commandtype type;
	int (*function)(int, char*[]);
};

#define NEW_COMMAND(name, description) {#name, description, TYPECOM, name}
#define NEW_OPERATION(name, description) {#name, description, TYPEOP, name}
#define NEW_SOLVER(name, description) {#name, description, TYPESOLVER, name}

const struct st_command commands[] = {
	NEW_COMMAND(CreateDenseMatrix,		"Creates a dense matrix"),
	NEW_COMMAND(CreateDenseVector,		"Creates a dense vector"),
	NEW_COMMAND(CreateSparseMatrix,		"Creates a sparse matrix"),
	NEW_OPERATION(DMxV,			"Dense matrix dot vector operation"),
	NEW_OPERATION(DMxDM,			"Dense matrix dot dense matrix operation"),
	NEW_OPERATION(LUDecomposition,		"LU factorization of a matrix"),
	NEW_SOLVER(ConjugateGradient,		"Solves a system by using the conjugate gradient method"),
	NEW_SOLVER(Jacobi,		"Solves a system by using the Jacobi method"),
};

#define SHOW_COMMANDS(mytype) \
	for (i = 0, imax = sizeof(commands) / sizeof(com); i < imax; i++) { \
		com = commands[i]; \
		if (com.type != mytype) continue; \
		fprintf(stderr, "   %-34s  %s\n", com.name, com.description); \
	}

static int usage()
{
	fprintf(stderr, "\n");
	fprintf(stderr, "Program: MM-Suite (perform operations to create and operate over matrix market files)\n");
	fprintf(stderr, "Version: %s\n", PACKAGE_VERSION);
	fprintf(stderr, "Contact: José M. Abuín <josemanuel.abuin@usc.es>\n\n");
	fprintf(stderr, "Usage:   MM-Suite <command> [options]\n\n");
	fprintf(stderr, "Available commands:\n");

	size_t i, imax;
	struct st_command com;

	fprintf(stderr, "Input/Output:\n");
	SHOW_COMMANDS(TYPECOM);

	fprintf(stderr, "\nBasic operations:\n");
	SHOW_COMMANDS(TYPEOP);

	fprintf(stderr, "\nSolvers:\n");
	SHOW_COMMANDS(TYPESOLVER);

	fprintf(stderr, "\n");
	return 1;
}


int main(int argc, char *argv[])
{
	int ret;
	double t_real;
	struct st_command com;
	size_t i, imax;

	t_real = realtime();

	fprintf(stderr, "MM-Suite\tVN:%s\tCL:%s", PACKAGE_VERSION, argv[0]);
	
	//for (i = 1; i < argc; ++i) fprintf(stderr, " %s", argv[i]);
	
	fprintf(stderr, "\n");

	if (argc < 2) return usage();

	for (i = 0, imax = sizeof(commands) / sizeof(com); i < imax; i++) {
		com = commands[i];
		if (strcmp(com.name, argv[1]) == 0) {
			ret = com.function(argc-1, argv+1);
			break;
		}
	}
	if (i == imax) {
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

	fprintf(stderr, "\n[%s] Real time: %.6f sec; CPU: %.6f sec; User: %.6f sec; Sys: %6f sec\n", __func__, realtime() - t_real, cputime(),usertime(),systemtime());

	return ret;
}
