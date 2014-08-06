#pragma once
#include "method.h"
#include "MatrixSolver.h"


class FEM_DG_IMPLICIT :	public Method
{
public:
	FEM_DG_IMPLICIT();
	~FEM_DG_IMPLICIT();
public:
	void init(char * xmlFileName);
	void run();
	void done();

private:
	void memAlloc();
	void memFree();

	void calcTimeStep();
	
	Region & getRegionByCellType(int type);

	Region   &	getRegion(int iCell);
	Material &	getMaterial(int iCell);

	void convertParToCons(int iCell, Param & par); //!< �������������� ����������� ���������� � ��������������

	void convertConsToPar(int iCell, Param & par); //!< �������������� �������������� ���������� � �����������

	double getField(int fld, int iCell, Point p);
	double getField(int fld, int iCell, double x, double y);

	double getF(int id, int iCell, Point p);
	double getF(int id, int iCell, double x, double y);

	double getDfDx(int id, int iCell, Point p);
	double getDfDx(int id, int iCell, double x, double y);

	double getDfDy(int id, int iCell, Point p);
	double getDfDy(int id, int iCell, double x, double y);

	void save(int step);

	inline void setCellFlagLim(int iCell){ grid.cells[iCell].flag |= CELL_FLAG_LIM; }
	inline bool cellIsLim(int iCell)		{ return (grid.cells[iCell].flag & CELL_FLAG_LIM) > 0; }
	int getLimitedCellsCount();
	void remediateLimCells();

	void incCFL();
	void decCFL();

	void calcIntegral();		//!< ��������� �������� ��(dF / dU)*deltaU*dFi / dx
	void calcMatrWithTau();		//!< ��������� ������� ����� ����������� �� �������
	void calcMatrFlux();		//!< ��������� ��������� �������� 
	void calcRHS();				//!< ��������� ������� ������ ������

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
	bool			SMOOTHING;
	double			SMOOTHING_PAR;
	int				FLUX;


	int				matCount;
	int				regCount;
	int				bCount;
	Material	   *materials;
	Region		   *regions;
	Boundary	   *boundaries;

	//! �������������� ���������� �� ������� ��������� ����.
	double			**ro;
	double			**ru;
	double			**rv;
	double			**re;

	double			***fields;

	double			*tmpArr;
	int				*tmpArrInt;

	//! ���������.
	Vector			*gradR;
	Vector			*gradP;
	Vector			*gradU;
	Vector			*gradV;

	//! ������
	double			limitRmin;
	double			limitRmax;
	double			limitPmin;
	double			limitPmax;
	double			limitUmax;

	int				limCells;

	//! ��������� ����.
	double			Fx;
	double			Fy;

	//! ���� � ������������ ���������
	Point			**cellGP;
	Point			**edgeGP;
	double			**cellGW;
	double			*cellJ;
	double			**edgeGW;
	double			*edgeJ;

	// ������� ����
	double			***matrA;
	double			***matrInvA;

	MatrixSolver	*solverMtx;

protected:
	const static int FLUX_GODUNOV = 0;
	const static int FLUX_LAX = 1;

	const static int BASE_FUNC_COUNT = 3;
	const static int GP_CELL_COUNT = 3;
	const static int GP_EDGE_COUNT = 2;

	const static int FIELD_RO = 0;
	const static int FIELD_RU = 1;
	const static int FIELD_RV = 2;
	const static int FIELD_RE = 3;
};

