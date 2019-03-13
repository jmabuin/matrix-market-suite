cmake_minimum_required(VERSION 3.13)


if(${USE_CUDA})
    project(matrix-market-suite LANGUAGES C CUDA)
else(${USE_CUDA})
    project(matrix-market-suite LANGUAGES C)
endif(${USE_CUDA})

set(CMAKE_C_STANDARD 11)

SET(USE_MPI false CACHE BOOLEAN "Use MPI parallelization for examples")
SET(USE_CUDA false CACHE BOOLEAN "Use CUDA parallelization for examples")
#SET(USE_BLAS false CACHE BOOLEAN "Use BLAS libraries for examples")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(THREADS_PREFER_PTHREAD_FLAG ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB io_SRC  "src/io/*.h"  "src/io/*.c")
file(GLOB operations_SRC  "src/operations/*.h"  "src/operations/*.c")
file(GLOB solvers_SRC  "src/solvers/*.h"  "src/solvers/*.c")
file(GLOB utils_SRC  "src/utils/*.h"  "src/utils/*.c")

file(GLOB operations_MPI_SRC  "src/MPI//operations/*.h"  "src/MPI/operations/*.c")
file(GLOB solvers_MPI_SRC  "src/MPI/solvers/*.h"  "src/MPI/solvers/*.c")
file(GLOB utils_MPI_SRC  "src/MPI/utils/*.h"  "src/MPI/utils/*.c")

file(GLOB operations_CUDA_SRC  "src/CUDA/operations/*.h"  "src/CUDA/operations/*.cu")
file(GLOB solvers_CUDA_SRC  "src/CUDA/solvers/*.h"  "src/CUDA/solvers/*.cu")

set(CMAKE_C_FLAGS "-g -Wall -Wno-unused-function -DUSE_MALLOC_WRAPPERS")
set(CMAKE_C_FLAGS_DEBUG "-g -O3")
set(CMAKE_C_FLAGS_RELEASE "-O3")

if(${USE_MPI})
    find_package(BLAS REQUIRED)
    find_package(LAPACKE REQUIRED)
    find_package(MPI REQUIRED)
    include_directories(${MPI_C_INCLUDE_DIRS})
    add_executable(MM-Suite ${io_SRC} ${operations_MPI_SRC} ${solvers_MPI_SRC} ${utils_MPI_SRC} src/MPI/MM-Suite-MPI.c
            src/utils/utils.c src/utils/utils.h src/operations/VectorOperations.c src/operations/VectorOperations.h
            src/utils/mmio.h src/utils/mmio.c)
    target_link_libraries(MM-Suite ${MPI_LIBRARIES})
elseif(${USE_CUDA})
    find_package(CUDA)
    add_executable(MM-Suite ${operations_CUDA_SRC} ${solvers_CUDA_SRC} src/CUDA/MM-Suite-CUDA.cu
            src/utils/utils.c src/utils/utils.h src/utils/mmio.h src/utils/mmio.c)
    CUDA_ADD_CUBLAS_TO_TARGET( MM-Suite )
else(${USE_MPI})
    find_package(BLAS REQUIRED)
    find_package(LAPACKE REQUIRED)
    add_executable(MM-Suite ${io_SRC} ${operations_SRC} ${solvers_SRC} ${utils_SRC} src/MM-Suite.c)

endif(${USE_MPI})


target_link_libraries(MM-Suite m)
target_link_libraries(MM-Suite ${LAPACKE_LIBRARIES})