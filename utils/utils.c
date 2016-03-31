#include "utils.h"

/*********
 * Timer *
 *********/

double cputime() {
	struct rusage r;
	getrusage(RUSAGE_SELF, &r);
	return r.ru_utime.tv_sec + r.ru_stime.tv_sec + 1e-6 * (r.ru_utime.tv_usec + r.ru_stime.tv_usec);
}

double systemtime() {
	struct rusage r;
	getrusage(RUSAGE_SELF, &r);
	return r.ru_stime.tv_sec + 1e-6 * (r.ru_stime.tv_usec);
}

double usertime() {
	struct rusage r;
	getrusage(RUSAGE_SELF, &r);
	return r.ru_utime.tv_sec + 1e-6 * (r.ru_utime.tv_usec);
}

double realtime() {
	struct timeval tp;
	struct timezone tzp;
	gettimeofday(&tp, &tzp);
	return tp.tv_sec + tp.tv_usec * 1e-6;
}


/* Read matrices
 *
 */
 
int readDenseCoordinateMatrix(char *fileName,unsigned long **I,unsigned long **J, double **values,unsigned long *M,unsigned long *N, unsigned long long *nz) {

	FILE *inputMatrix;
	
	int ret_code;
	
	MM_typecode inputmatcode;
	
	if ((inputMatrix = fopen(fileName, "r")) == NULL) {
		fprintf(stderr,"[%s] Error opening matrix file\n",__func__);
		return 0;
	}
	
	//mm_read_banner(inputMatrix, &inputmatcode);
	
	if (mm_read_banner(inputMatrix, &inputmatcode) != 0){
		fprintf(stderr,"[%s] Could not read banner in the Matrix file\n",__func__);
		return 0;
	}
	
	/*
	if(!mm_is_dense(inputmatcode)){
		fprintf(stderr,"[%s] The matrix is not dense\n",__func__);
		return 0;
	}
	*/
	
	if ((ret_code = mm_read_mtx_crd_size(inputMatrix, M, N, nz)) !=0) {
		fprintf(stderr,"[%s] Could not read matrix size\n",__func__);
		return 0;
	}

	*I = NULL;
	*J = NULL;
	*values=NULL;

	*I = (unsigned long *)  malloc(*nz * sizeof(unsigned long));
	*J = (unsigned long *)  malloc(*nz * sizeof(unsigned long));
	*values = (double *)  malloc(*nz * sizeof(double));
	
	if(mm_read_mtx_crd_data(inputMatrix,*M,*N,*nz,*I,*J,*values,inputmatcode) != 0){
		fprintf(stderr,"[%s] Error reading matrix values\n",__func__);
		return 0;
	}
	

	fclose(inputMatrix);

	//mm_write_mtx_crd("stdout",*M,*N,*nz,I,J,values,inputmatcode);

	return 1;
}



int readDenseVector(char *fileName, double **values,unsigned long *M,unsigned long *N, unsigned long long *nz) {

	FILE *inputVector;
	
	int ret_code;
	
	MM_typecode inputmatcode;
	
	if ((inputVector = fopen(fileName, "r")) == NULL) {
		fprintf(stderr,"[%s] Error opening vector file\n",__func__);
		return 0;
	}
	
	
	if (mm_read_banner(inputVector, &inputmatcode) != 0){
		fprintf(stderr,"[%s] Could not read banner in the Vector file\n",__func__);
		return 0;
	}
	
	/*
	if(!mm_is_dense(inputmatcode)){
		fprintf(stderr,"[%s] The matrix is not dense\n",__func__);
		return 0;
	}
	*/
	
	if ((ret_code = mm_read_mtx_array_size(inputVector, M, N)) !=0) {
		fprintf(stderr,"[%s] Could not read matrix size\n",__func__);
		return 0;
	}

	*nz = (unsigned long long)(*M)*(unsigned long long)(*N);

	*values=NULL;

	*values = (double *)  malloc(*nz * sizeof(double));
	

	if(mm_read_vector_data(inputVector,*nz,*values,inputmatcode) != 0){
		fprintf(stderr,"[%s] Error reading vector values\n",__func__);
		return 0;
	}
	

	fclose(inputVector);


	return 1;

}


int writeDenseVector(char *fileName, double *values,unsigned long M,unsigned long N, unsigned long long nz) {

	FILE *f;

	if(strcmp(fileName,"stdout")==0){
		f = stdout;
	}
	else{
		if ((f = fopen(fileName, "w")) == NULL){
			fprintf(stderr,"[%s] Unable to open file for writing\n",__func__);
			return 0;
		}
	}
	
	MM_typecode outputmatcode;
	
	mm_initialize_typecode(&outputmatcode);
	mm_set_array(&outputmatcode);
	mm_set_dense(&outputmatcode);
	mm_set_real(&outputmatcode);

	
	mm_write_banner(f, outputmatcode);
	mm_write_mtx_array_size(f, M, N);
	
	for(unsigned long i = 0;i < M; i++){

		fprintf(f, "%lg\n",values[i]);
	}
	
	if(f!=stdout){
		fclose(f);
	}
	
	return 1;
}



