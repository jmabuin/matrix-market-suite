#include <stdio.h>
#include <string.h>

#include <mpich/mpi.h>

#include "utils/utils-MPI.h"
#include "../utils/utils.h"

#include "operations/DMxV-MPI.h"

#include "solvers/ConjugateGradient-MPI.h"

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "0.2.0"
#endif


static int usage()
{
	fprintf(stderr, "\n");
	fprintf(stderr, "Program: MM-Suite-MPI (perform operations to operate over matrix market files using MPI)\n");
	fprintf(stderr, "Version: %s\n", PACKAGE_VERSION);
	fprintf(stderr, "Contact: José M. Abuín <josemanuel.abuin@usc.es>\n\n");
	fprintf(stderr, "Usage:   MM-Suite-MPI <command> [options]\n\n");
	fprintf(stderr, "Basic operations:\n");
	fprintf(stderr, "Command: DMxV                               Dense matrix dot vector operation\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Solvers:\n");
	fprintf(stderr, "Command: ConjugateGradient                  Solves a system by using the conjugate gradient method\n");
	fprintf(stderr, "\n");
	return 1;
}


int main(int argc, char *argv[]) {

	int numProcs; 
	int myid;
	
	
	// Start the MPI engine
	MPI_Init(&argc, &argv);

	// Find out number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

	// Find out process rank  
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	
	int i, ret;
	double t_real;

	t_real = realtime();

	if(myid == 0){
		fprintf(stderr, "MM-Suite-MPI\tVN:%s\tCL:%s", PACKAGE_VERSION, argv[0]);
	
		for (i = 1; i < argc; ++i) fprintf(stderr, " %s", argv[i]);
	
		fprintf(stderr, "\n");

		if (argc < 2) return usage();
	}
	else{
		if (argc < 2) return 1;
	}
	
	if (strcmp(argv[1], "DMxV") == 0) 			ret = DMxVMPI(argc-1, argv+1,numProcs,myid);
	else if (strcmp(argv[1], "ConjugateGradient") == 0)	ret = ConjugateGradientMPI(argc-1, argv+1,numProcs,myid);
	else {
		fprintf(stderr, "[%s] unrecognized command '%s'\n",__func__, argv[1]);
		return 1;
	}
	
	if(myid == 0){
		if (ret == 0) {
			fprintf(stderr, "[%s] Version: %s\n", __func__, PACKAGE_VERSION);
			fprintf(stderr, "[%s] CMD:", __func__);
			for (i = 0; i < argc; ++i)
				fprintf(stderr, " %s", argv[i]);
		
		}
		
	}
	
	fprintf(stderr, "\n[%s] Real time: %.3f sec; CPU: %.3f sec; User: %.3f sec; Sys: %3f sec\n", __func__, realtime() - t_real, cputime(),usertime(),systemtime());
	
	if(ret == 1){
		ret = 0;
	}
	else if(ret == 0){
		ret = 1;
	}
	
	// Shut down MPI 
	MPI_Finalize();
	
	return ret;
}
