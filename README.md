# Matrix Market Suite

# 1. Description
**Matrix Market Suite** is a tool that creates and operates over matrices with the [Matrix Market][1] format.

# 2. Installation
## 2.1 Dependencies
**Matrix Market Suite** only depends on the [blas][2] library. More preciselly, the [cblas][3] library.

To install it in Debian:

	sudo aptitude install libblas-dev libblas3
	
In Debian stretch, **blas** include the **cblas** routines.

## 2.2 Compiling
In order to compile **Matrix Market Suite**, the user only has to execute:

	make
	
This command creates the *MM-Suite* executable.

# 3. Commands

Available commands are:

* Input/Output
- **CreateDenseMatrixSymmetric		creates a dense symmetric matrix
- **CreateDenseVector			creates a dense vector

* Basic operations
- **DMxV					Dense matrix dot vector operation

* Solvers
- **ConjugateGradient			Solves a system by using the conjugate gradient method


# 4. Examples



[1]: http://math.nist.gov/MatrixMarket/
[2]: http://www.netlib.org/blas/
[3]: http://www.netlib.org/blas/#_cblas
