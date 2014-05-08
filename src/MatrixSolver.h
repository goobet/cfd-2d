#ifndef _MatrixSolver_
#define _MatrixSolver_

#include "CSR.h"

class MatrixSolver
{
public:
	virtual ~MatrixSolver();

	void init(int cellsCount, int blockDimension);
	
	void zero();
	
	void setMatrElement(int i, int j, double** matrDim);
	void setRightElement(int i, double* vectDim);
	void addMatrElement(int i, int j, double** matrDim);
	void addRightElement(int i, double* vectDim);
	void createMatrElement(int i, int j);

	virtual void solve(double eps, int& maxIter) = 0;

	void printToFile(const char* fileName);

	CSRMatrix	*a;
	int			 blockDim;
	double		*b;
	double		*x;
};

class SolverZeidel: public MatrixSolver 
{
	virtual void solve(double eps, int& maxIter);
};

class SolverJacobi: public MatrixSolver
{
public:
	SolverJacobi() {
		tempXAlloc = false;
	}
	~SolverJacobi() {
		if (tempXAlloc) {
			delete [] tempX;
			tempXAlloc = false;
		}
	}
	virtual void	solve(double eps, int& maxIter);
	double			*tempX;
	bool			tempXAlloc;
};

#endif