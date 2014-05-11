#ifndef _FVM_TVD_IMPLICIT_H_
#define _FVM_TVD_IMPLICIT_H_

#include <method.h>
#include "MatrixSolver.h"
#include <algorithm>

class FVM_TVD_IMPLICIT: public Method
{
public:
	virtual void init(char * xmlFileName);
	virtual void run();
	virtual void done();
protected:
	Region & getRegionByCellType(int type);

	Region   &	getRegion	(int iCell);
	Material &	getMaterial	(int iCell);
	
	/**
	 *	�������������� ����������� ���������� � ��������������
	 */
	void convertParToCons(int iCell, Param & par);
	
	/**
	 *	�������������� �������������� ���������� � �����������
	 */
	void convertConsToPar(int iCell, Param & par);
	
	/**
	 *	���������� ���������� � ������� ������� �� ������� ������ �������� ��������� ��������
	 */
	void boundaryCond(int iFace, Param& pL, Param& pR);

	/**
	 *	���������� ���� �� ������� �� �������� CFL, ���� �������� TAU �� XML 
	 *	������ ������������, �� ������������ ��������, �������� � XML
	 */
	void calcTimeStep();

	/**
	 *	������ �������� ���������������� ���������� � ����
	 */
	void save(int);

	/**
	 *	���������� ���������� ������
	 */
	void calcFlux(double& fr, double& fu, double& fv, double& fe, Param pL, Param pR, Vector n, double GAM);

private:
	double **allocMtx4();													//+.
	void freeMtx4(double **mtx4);											//+.
	void multMtx4(double **dst4, double **srcA4, double **srcB4);			//+.
	void clearMtx4(double **mtx4);											//+.
	void printMtx4(double **mtx4, char *msg = 0);							//+.
	
	// ��������� � mtx4 ����������� �������� ������� ����� ������� ������.
	void eigenValues(double **dst4, double c, double u, double nx, double v, double ny);					//+.
	void rightEigenVector(double **dst4, double c, double u, double nx, double v, double ny, double H);		//+.
	void leftEigenVector(double **dst4, double c, double GAM, double u, double nx, double v, double ny);	//+.
	
	// ��������� � mtx4 A+.
	void calcAP(double **dst4, double **rightEgnVecl4, double **egnVal4, double **leftEgnVecl4);			//+.
	// ��������� � mtx4 A-.
	void calcAM(double **dst4, double **rightEgnVecl4, double **egnVal4, double **leftEgnVecl4);			//+.

	void calcRoeAverage(Param& average, Param pL, Param pR, double GAM, Vector n);	//+.
	
	void setCellFlagLim(int iCell)	{ grid.cells[iCell].flag |= CELL_FLAG_LIM; }
	bool cellIsLim(int iCell)		{ return (grid.cells[iCell].flag & CELL_FLAG_LIM) > 0; }
	int getLimitedCellsCount();
	void remediateLimCells();

	void incCFL();
	void decCFL();

private:
	double			TMAX;
	int				STEP_MAX;
	double			TAU;
	double			CFL;
	double			scaleCFL;
	double			maxCFL;
	int				stepCFL;
	double			maxLimCells;
	int				FILE_SAVE_STEP;
	int				PRINT_STEP;

	double			TAU_MIN;

	bool			STEADY;	// false - �������������� �������, true - ������������� �������.
	double			*cTau;  // ��������� ��� �� ������� � ������.

	int				matCount;
	int				regCount;
	int				bCount;
	Material	   *materials;
	Region		   *regions;
	Boundary	   *boundaries;

	//! �������������� ���������� �� ������� ��������� ����.
	double		   *ro;			 
	double		   *ru;			
	double		   *rv;			
	double		   *re;

	double			*tmpArr;

	//! ������
	double limitRmin;
	double limitRmax;
	double limitPmin;
	double limitPmax;
	double limitUmax;
};

#endif