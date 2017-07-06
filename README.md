# Matrix Market Suite

# 1. Description
The [Matrix Market][1] format is a widely used format when working with dense and sparse matrices. Typically, when measuring performance of linear algebra programs or testing scientific simulation software, matrices from collections such as [The University of Florida Sparse Matrix Collection][7] or [The Harwell-Boeing Collection][8]. These collections use the [Matrix Market][1] format as one of their formats.

But sometimes, these matrices are not enough, or don't adapt to certain kind of problems. So, in order to create and test these data, we have created **Matrix Market Suite**. **Matrix Market Suite** is a tool that creates and operates over matrices with the [Matrix Market][1] format.

# 2. Installation
## 2.1 Dependencies
**Matrix Market Suite** depends on the [openblas][2] and [lapacke][3] libraries.

To install them Debian:

	sudo aptitude install libopenblas-base libopenblas-dev liblapacke liblapacke-dev

Also, the compiler used to build the tool is [gcc][4].

## 2.2 Compiling
In order to compile **Matrix Market Suite**, the user only has to execute:

	make

This command creates the *MM-Suite* executable.

# 3. Commands

Available commands are:

## Input/Output
* **CreateDenseMatrix** - creates a dense matrix. It can be a symmetric, generic or diagonally dominant matrix, or a combination of all of them, depending on the arguments.
* **CreateDenseVector** - creates a dense vector.
* **CreateSparseMatrix** - creates a sparse matrix.

## Basic operations
* **DMxV** - Dense matrix dot vector operation.
* **DMxDM** - Dense matrix dot dense matrix operation.
* **LUDecomposition** - LU factorization of a matrix using partial pivoting.

## Solvers
* **ConjugateGradient** - Solves a system by using the conjugate gradient method.
* **Jacobi** - Solves a system by using the Jacobi method.

To check the available options for a given command, execute:

    MM-Suite <Command-name>

# 4. Versions
## 4.1. MPI version
This version is a parallel implementation of the methods from **Matrix Market Suite** that allows parallelization. Nowadays, these methods are:

* **DMxV**
* **ConjugateGradient**

The **DMxDM** is not yet implemented with MPI.

### 4.1.2. Build MPI version
To build the parallel MPI version, the user has to enter the MPI directory and run:

	make
	
Dependencies for this versions are only [openmpi][9]

	sudo aptitude install libopenmpi-dev openmpi-common openmpi-bin libopenmpi2


# 5. Examples
## 5.1. Create a dense symmetrix matrix
In this example a dense symmetrix matrix of dimensions 10x10 is created.

	./MM-Suite CreateDenseMatrix -o Matrix-Example-10x10.mtx -s 10 10  87
	
Where:
* *-o Matrix-Example-10x10.mtx* Is the option and the file name of the result matrix.
* *-s* Indicates that we want to create a symmetric matrix.
* *10* Is the number of rows.
* *10* Is the number of cols.
* *87* Is the seed used to fill the matrix.

The result is:

	%%MatrixMarket matrix coordinate real symmetric
	10 10 100
	1 1 0.163865
	1 2 0.921726
	1 3 0.831328
	...
	10 7 0.084717
	10 8 0.240895
	10 9 0.700352
	10 10 0.318448

## 5.2. Create a dense vector
In this example a vector of 10 elements is created.

	./MM-Suite CreateDenseVector 10 Vector-B-10.mtx 55
	
Where:
* *10* Is the number of elements.
* *Vector-B-10.mtx* Is the file name of the result vector.
* *55* Is the seed used to fill the vector.

The result is:

	%%MatrixMarket matrix array real general
	10 1
	0.193523
	0.200518
	0.625800
	0.585233
	0.849371
	0.121675
	0.331428
	0.199876
	0.774032
	0.127585


## 5.3. Solve a system by using the conjugate gradient method
In this example we will use the matrix and vector created previouslly to solve a system. For that:

	./MM-Suite ConjugateGradient Matrix-Example-10x10.mtx Vector-B-10.mtx
	
Where:
* *Matrix-Example-10x10.mtx* Is the file name of the matrix.
* *Vector-B-10.mtx* Is the file name of the vector

The result is:

	%%MatrixMarket matrix array real general
	10 1
	0.159064
	-1.07431
	1.60824
	3.59025
	-1.91339
	1.62213
	1.11456
	-1.29864
	-0.0936935
	-2.45013
	
## 5.4. Multiply a dense matrix and a vector.
To check if the result obtained from the conjugate gradient method is correct, we can perform a multiplication with the input matrix and the result obtained in the previous example. The result must be the vector used also in the previous example.

	./MM-Suite DMxV Matrix-Example-10x10.mtx Result-CG.mtx
	
Where:
* *Matrix-Example-10x10.mtx* Is the same matrix used in these examples.
* *Result-CG.mtx* Is the result from the previous example.

The result is:

	%%MatrixMarket matrix array real general
	10 1
	0.193531
	0.200523
	0.625799
	0.585231
	0.849374
	0.121682
	0.331433
	0.199879
	0.774031
	0.127583
	
Which is the input vector from the conjugate gradient example.

[1]: http://math.nist.gov/MatrixMarket/
[2]: http://www.openblas.net/
[3]: http://www.netlib.org/lapack/lapacke.html
[4]: https://gcc.gnu.org/
[5]: https://gcc.gnu.org/fortran/
[6]: https://www.mpich.org/
[7]: https://www.cise.ufl.edu/research/sparse/matrices/
[8]: http://math.nist.gov/MatrixMarket/data/Harwell-Boeing/
[9]: https://www.open-mpi.org/
