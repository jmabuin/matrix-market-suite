#include <stdio.h>
#include <string.h>
#include "utils/utils.h"

#include "io/CreateDenseMatrixSymmetric.h"
#include "io/CreateDenseVector.h"


#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "0.0.1"
#endif


static int usage()
{
	fprintf(stderr, "\n");
	fprintf(stderr, "Program: MM-Suite (perform operations to create and operate over matrix market files)\n");
	fprintf(stderr, "Version: %s\n", PACKAGE_VERSION);
	fprintf(stderr, "Contact: José M. Abuín <josemanuel.abuin@usc.es>\n\n");
	fprintf(stderr, "Usage:   MM-Suite <command> [options]\n\n");
	fprintf(stderr, "--------------------------------Input/Output--------------------------------\n");
	fprintf(stderr, "Command: CreateDenseMatrixSymmetric         creates a dense symmetric matrix\n");
	fprintf(stderr, "         CreateDenseVector                  creates a dense vector\n");
	fprintf(stderr, "------------------------------Basic operations------------------------------\n");
	fprintf(stderr, "Command: DMxV         Dense matrix dot vector operation\n");
//	fprintf(stderr, "         fastmap       identify super-maximal exact matches\n");
//	fprintf(stderr, "         pemerge       merge overlapping paired ends (EXPERIMENTAL)\n");
//	fprintf(stderr, "         aln           gapped/ungapped alignment\n");
//	fprintf(stderr, "         samse         generate alignment (single ended)\n");
//	fprintf(stderr, "         sampe         generate alignment (paired ended)\n");
//	fprintf(stderr, "         bwasw         BWA-SW for long queries\n");
	fprintf(stderr, "\n");
//	fprintf(stderr, "         shm           manage indices in shared memory\n");
//	fprintf(stderr, "         fa2pac        convert FASTA to PAC format\n");
//	fprintf(stderr, "         pac2bwt       generate BWT from PAC\n");
//	fprintf(stderr, "         pac2bwtgen    alternative algorithm for generating BWT\n");
//	fprintf(stderr, "         bwtupdate     update .bwt to the new format\n");
//	fprintf(stderr, "         bwt2sa        generate SA from BWT and Occ\n");
//	fprintf(stderr, "\n");
//	fprintf(stderr,
//"Note: To use BWA, you need to first index the genome with `bwa index'.\n"
//"      There are three alignment algorithms in BWA: `mem', `bwasw', and\n"
//"      `aln/samse/sampe'. If you are not sure which to use, try `bwa mem'\n"
//"      first. Please `man ./bwa.1' for the manual.\n\n");
	return 1;
}


int main(int argc, char *argv[])
{

	int i, ret;
	double t_real;

	t_real = realtime();

	
	fprintf(stderr, "MM-Suite\tVN:%s\tCL:%s", PACKAGE_VERSION, argv[0]);
	
	for (i = 1; i < argc; ++i) fprintf(stderr, " %s", argv[i]);
	
	fprintf(stderr, "\n\n");

	if (argc < 2) return usage();

	if (strcmp(argv[1], "CreateDenseMatrixSymmetric") == 0) ret = CreateDenseMatrixSymmetric(argc-1, argv+1);
	else if (strcmp(argv[1], "CreateDenseVector") == 0) ret = CreateDenseVector(argc-1, argv+1);
/*	else if (strcmp(argv[1], "pac2bwtgen") == 0) ret = bwt_bwtgen_main(argc-1, argv+1);
	else if (strcmp(argv[1], "bwtupdate") == 0) ret = bwa_bwtupdate(argc-1, argv+1);
	else if (strcmp(argv[1], "bwt2sa") == 0) ret = bwa_bwt2sa(argc-1, argv+1);
	else if (strcmp(argv[1], "index") == 0) ret = bwa_index(argc-1, argv+1);
	else if (strcmp(argv[1], "aln") == 0) ret = bwa_aln(argc-1, argv+1);
	else if (strcmp(argv[1], "samse") == 0) ret = bwa_sai2sam_se(argc-1, argv+1);
	else if (strcmp(argv[1], "sampe") == 0) ret = bwa_sai2sam_pe(argc-1, argv+1);
	else if (strcmp(argv[1], "bwtsw2") == 0) ret = bwa_bwtsw2(argc-1, argv+1);
	else if (strcmp(argv[1], "dbwtsw") == 0) ret = bwa_bwtsw2(argc-1, argv+1);
	else if (strcmp(argv[1], "bwasw") == 0) ret = bwa_bwtsw2(argc-1, argv+1);
	else if (strcmp(argv[1], "fastmap") == 0) ret = main_fastmap(argc-1, argv+1);
	else if (strcmp(argv[1], "mem") == 0) ret = main_mem(argc-1, argv+1);
	else if (strcmp(argv[1], "shm") == 0) ret = main_shm(argc-1, argv+1);
	else if (strcmp(argv[1], "pemerge") == 0) ret = main_pemerge(argc-1, argv+1);
*/	else {
		fprintf(stderr, "[main] unrecognized command '%s'\n", argv[1]);
		return 1;
	}
	
	
	if (ret == 0) {
		fprintf(stderr, "[%s] Version: %s\n", __func__, PACKAGE_VERSION);
		fprintf(stderr, "[%s] CMD:", __func__);
		for (i = 0; i < argc; ++i)
			fprintf(stderr, " %s", argv[i]);
		fprintf(stderr, "\n[%s] Real time: %.3f sec; CPU: %.3f sec; User: %.3f sec; Sys: %3f sec\n", __func__, realtime() - t_real, cputime(),usertime(),systemtime());
	}

	return ret;
}
