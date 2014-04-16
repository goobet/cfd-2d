#include "MatrixSolver.h"

void MatrixSolver::init(int cellsCount, int blockDimension)
{
	blockDim = blockDimension;
	int n = cellsCount*blockDim;
	a = new CSRMatrix(n);
	b = new double[n];
	x = new double[n];
}

void MatrixSolver::zero() {
	memset(x, 0, sizeof(double)*a->n);
	memset(b, 0, sizeof(double)*a->n);
	a->zero();
}

MatrixSolver::~MatrixSolver()
{
	delete a;
	delete[] b;
	delete[] x;
}

void MatrixSolver::setMatrElement(int i, int j, double** matrDim)
{
	for (int ii = 0; ii < blockDim; ii++)
	{
		for (int jj = 0; jj < blockDim; jj++)
		{
			a->set(ii+i*blockDim, ii+j*blockDim, matrDim[ii][jj]);
		}
	}
}

void MatrixSolver::setRightElement(int i, double* vectDim)
{
	for (int ii = 0; ii < blockDim; ii++)
	{
		b[ii+i*blockDim] = vectDim[ii];
	}
}


void MatrixSolver::addMatrElement(int i, int j, double** matrDim)
{
	for (int ii = 0; ii < blockDim; ii++)
	{
		for (int jj = 0; jj < blockDim; jj++)
		{
			a->add(ii+i*blockDim, ii+j*blockDim, matrDim[ii][jj]);
		}
	}
}

void MatrixSolver::addRightElement(int i, double* vectDim)
{
	for (int ii = 0; ii < blockDim; ii++)
	{
		b[ii+i*blockDim] += vectDim[ii];
	}
}

void SolverZeidel::solve(double eps, int& maxIter)
{
	double	aii;
	double	err = 1.0;
	int		step = 0;
	double	tmp;
	//memset(x, 0, sizeof(double)*a->n);
	while(err > eps && step < maxIter)
	{
		step++;
		for (int i = 0; i < a->n; i++)
		{
			tmp = 0.0;
			aii = 0;
			for (int k = a->ia[i]; k < a->ia[i+1]; k++)
			{
				if (i == a->ja[k])
				{
					aii = a->a[k];
				} else {
					tmp += a->a[k]*x[a->ja[k]];
				}
			}
			if (aii == 0) 
			{
				printf("ZEIDEL_SOLVER: error: a[%d, %d] = 0\n", i, i);

			}
			x[i] = (-tmp+b[i])/aii;
		}
		err = 0.0;
		for (int i = 0; i < a->n; i++)
		{
			tmp = 0.0;
			for (int k = a->ia[i]; k < a->ia[i+1]; k++)
			{
				tmp += a->a[k]*x[a->ja[k]];
			}
			err += fabs(tmp-b[i]);
		}
		int qqqqq = 0; // ZHRV_WARN
	}
	if (step >= maxIter)
	{
		printf("ZEIDEL_SOLVER: (warning) maximum iterations done (%d); error: %e\n", step, err);
	}
	maxIter = step;
}
