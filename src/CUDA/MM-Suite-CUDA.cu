/**
  * Copyright 2019 José Manuel Abuín Mosquera <josemanuel.abuin@usc.es>
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

extern "C" {
    #include "../utils/utils.h"
}
#include "operations/DMxV-CUDA.h"

#include "solvers/ConjugateGradient-CUDA.h"

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "0.3.0"
#endif

typedef enum {TYPEOP, TYPESOLVER} commandtype;

struct st_command {
    const char * name;
    const char * description;
    commandtype type;
    int (*function)(int, char*[]);
};

#define NEW_OPERATION(name, description, function_name) {#name, description, TYPEOP, function_name}
#define NEW_SOLVER(name, description, function_name) {#name, description, TYPESOLVER, function_name}

const struct st_command commands[] = {
        NEW_OPERATION(DMxV,			"Dense matrix dot vector operation", DMxVCUDA),
        NEW_SOLVER(ConjugateGradient,		"Solves a system by using the conjugate gradient method", ConjugateGradientCUDA),
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
    fprintf(stderr, "Program: MM-Suite (perform operations to operate over matrix market files using CUDA)\n");
    fprintf(stderr, "Version: %s\n", PACKAGE_VERSION);
    fprintf(stderr, "Contact: José M. Abuín <josemanuel.abuin@usc.es>\n\n");
    fprintf(stderr, "Usage:   MM-Suite <command> [options]\n\n");
    fprintf(stderr, "Available commands:\n");

    size_t i, imax;
    struct st_command com;

    fprintf(stderr, "\nBasic operations:\n");
    SHOW_COMMANDS(TYPEOP);

    fprintf(stderr, "\nSolvers:\n");
    SHOW_COMMANDS(TYPESOLVER);

    fprintf(stderr, "\n");
    return 1;
}

void devicenfo(void)
{
    struct cudaDeviceProp capabilities;

    cudaGetDeviceProperties (&capabilities, 0);

    printf("->CUDA Platform & Capabilities\n");
    printf("Name: %s\n", capabilities.name);
    printf("totalGlobalMem: %.2f MB\n", capabilities.totalGlobalMem/1024.0f/1024.0f);
    printf("sharedMemPerBlock: %.2f KB\n", capabilities.sharedMemPerBlock/1024.0f);
    printf("regsPerBlock (32 bits): %d\n", capabilities.regsPerBlock);
    printf("warpSize: %d\n", capabilities.warpSize);
    printf("memPitch: %.2f KB\n", capabilities.memPitch/1024.0f);
    printf("maxThreadsPerBlock: %d\n", capabilities.maxThreadsPerBlock);
    printf("maxThreadsDim: %d x %d x %d\n", capabilities.maxThreadsDim[0],
           capabilities.maxThreadsDim[1], capabilities.maxThreadsDim[2]);
    printf("maxGridSize: %d x %d\n", capabilities.maxGridSize[0],
           capabilities.maxGridSize[1]);
    printf("totalConstMem: %.2f KB\n", capabilities.totalConstMem/1024.0f);
    printf("major.minor: %d.%d\n", capabilities.major, capabilities.minor);
    printf("clockRate: %.2f MHz\n", capabilities.clockRate/1024.0f);
    printf("textureAlignment: %d\n", capabilities.textureAlignment);
    printf("deviceOverlap: %d\n", capabilities.deviceOverlap);
    printf("multiProcessorCount: %d\n", capabilities.multiProcessorCount);
}

int main(int argc, char *argv[]) {

    int i, ret;
    double t_real;

    struct st_command com;

    size_t imax;

    t_real = realtime();


    fprintf(stderr, "MM-Suite\tVN:%s\tCL:%s", PACKAGE_VERSION, argv[0]);
    fprintf(stderr, "\n");
    devicenfo();
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


    fprintf(stderr, "\n[%s] Real time: %.6f sec; CPU: %.6f sec; User: %.6f sec; Sys: %6f sec\n", __func__, realtime() - t_real, cputime(),usertime(),systemtime());

    if(ret == 1){
        ret = 0;
    }
    else if(ret == 0){
        ret = 1;
    }

    return ret;
}
