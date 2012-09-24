#include "global.h"

#ifndef _FEM_RKDG_H_
#define _FEM_RKDG_H_

#include <method.h>


class FEM_RKDG: public Method {
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
	void convertConsToPar(int iCell, Point pt, Param & par);
	
	/**
	 *	���������� ���������� ������ � ����� �� ������� ������
	 */
	void reconstruct(int iEdge, Point pt, Param& pL, Param& pR);

	/**
	 *	���������� ���������� � ������� ������� �� ������� ������ �������� ��������� ��������
	 */
	void boundaryCond(int iEdge, Point pt, Param& pL, Param& pR);

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

	void calcLimiters();

	void calcConvectionVol();

	void calcConvectionSurf();
	
	void calcDiffusionVol();

	void calcDiffusionSurf();
	
	void calcNewFields();

	void calcGP();

	void calcMatr();
	
	/**
	 *	��������� �������� ������� ������� �������
	 */
	VECTOR getF(int iCell, Point pt);

	VECTOR getDFDX(int iCell, Point pt);

	VECTOR getDFDY(int iCell, Point pt);

	inline double getRO(int iCell, Point pt) { return VECTOR::SCALAR_PROD(ro[iCell], getF(iCell, pt)); }
	inline double getRU(int iCell, Point pt) { return VECTOR::SCALAR_PROD(ru[iCell], getF(iCell, pt)); }
	inline double getRV(int iCell, Point pt) { return VECTOR::SCALAR_PROD(rv[iCell], getF(iCell, pt)); }
	inline double getRE(int iCell, Point pt) { return VECTOR::SCALAR_PROD(re[iCell], getF(iCell, pt)); }

private:
	double TMAX;
	double TAU;
	double CFL;
	int FILE_SAVE_STEP;
	int PRINT_STEP;
	int EDGE_GP_COUNT;
	int CELL_GP_COUNT;
	int FUNC_COUNT;

	int				matCount;
	int				regCount;
	int				bCount;
	Material	*	materials;
	Region		*	regions;
	Boundary	*	boundaries;

	//! �������������� ���������� �� ������� ��������� ���� (����. ������. �� �������� ��������)
	VECTOR * ro;			 
	VECTOR * ru;			
	VECTOR * rv;			
	VECTOR * re;			

	//! �������������� ���������� �� ���������� ��������� ���� (����. ������. �� �������� ��������)
	VECTOR * ro_old;
	VECTOR * ru_old;
	VECTOR * rv_old;
	VECTOR * re_old;

	//! ������ ����� ������� ��������� ���������� �����
	VECTOR * ro_int;
	VECTOR * ru_int;
	VECTOR * rv_int;
	VECTOR * re_int;
	
	Point ** edgeGP;
	Point ** cellGP;

	double * edgeGW;
	double * cellGW;

	double * edgeJ;
	double * cellJ;

	MATRIX * cellA;
	MATRIX * cellInvA;

};

#endif