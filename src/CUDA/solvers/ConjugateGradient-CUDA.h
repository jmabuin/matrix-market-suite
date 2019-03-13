//
// Created by chema on 3/13/19.
//

#ifndef MATRIX_MARKET_SUITE_CONJUGATEGRADIENT_CUDA_H_H
#define MATRIX_MARKET_SUITE_CONJUGATEGRADIENT_CUDA_H_H

extern "C" {
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include "../../utils/mmio.h"
    #include "../../utils/utils.h"
}

int ConjugateGradientCUDA(int argc, char *argv[]);

#endif //MATRIX_MARKET_SUITE_CONJUGATEGRADIENT_CUDA_H_H
