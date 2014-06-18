#include "k_eps_model.h"


const double KEpsModel::C_mu = 0.09;
const double KEpsModel::C_eps1 = 1.44;
const double KEpsModel::C_eps2 = 1.92;
const double KEpsModel::Sigma_K = 1.0;
const double KEpsModel::Sigma_Eps = 1.3;

const double KEpsModel::It_Start = 0.01;
const double KEpsModel::Lt_Start = 1.4e-5;


KEpsModel::KEpsModel(void)
{
}


KEpsModel::~KEpsModel(void)
{
}

void KEpsModel::init( Grid * grid, double * ro, double *ru, double * rv, double * ro_m, double * u_m, double * v_m, Vector * gradU, Vector * gradV, double * Txx, double * Tyy, double * Txy, const double mu )
{
	this->grid = grid;
	this->ro = ro;
	this->ru = ru;
	this->rv = rv;

	this->ro_m = ro_m;
	this->u_m = u_m;
	this->v_m = v_m;

	this->gradU = gradU;
	this->gradV = gradV;

	this->Txx = Txx;
	this->Tyy = Tyy;
	this->Txy = Txy;

	this->mu = mu;

	this->muT = new double[grid->cCount];
	this->rk = new double[grid->cCount];
	this->reps = new double[grid->cCount];

	this->rk_int = new double[grid->cCount];
	this->reps_int = new double[grid->cCount];

	this->gradK = new Vector[grid->cCount];
	this->gradEps = new Vector[grid->cCount];

	/*
	memset(muT, 0, grid->cCount*sizeof(double));
	memset(rk, 0, grid->cCount*sizeof(double));
	memset(reps, 0, grid->cCount*sizeof(double));
	*/

	startCond();
}


void KEpsModel::startCond()
{
	int nc = grid->cCount;
	
	for (int iCell = 0; iCell < nc; iCell++)
	{
		KEpsParam par;
		kEpsConvertConsToPar(iCell, par);
		
		double q = sqrt( par.u * par.u + par.v * par.v );
		
		par.k = 3.0 / 2.0 * ( It_Start * q ) * ( It_Start * q );
		par.eps = pow(C_mu, 3.0 / 4.0) * pow(par.k, 3.0 / 2.0) / Lt_Start;
		par.muT = par.r * C_mu * ( par.k * par.k / par.eps );

		kEpsConvertParToCons(iCell, par);
	}
}

inline double KEpsModel::getMuT( const int iCell )
{
	return muT[iCell];
}

void KEpsModel::done()
{
	delete [] muT;
	delete [] rk;
	delete [] reps;

	delete [] gradK;
	delete [] gradEps;
}

void KEpsModel::calcMuT( double * cTau )
{
	int nc = grid->cCount;
	int ne = grid->eCount;

	for (int iTau = 1; iTau <= 100; iTau++)
	{
		memset(rk_int, 0, nc*sizeof(double));
		memset(reps_int, 0, nc*sizeof(double));

		calcGrad();

		for (int iEdge = 0; iEdge < ne; iEdge++)
		{
			int c1	= grid->edges[iEdge].c1;
			int c2	= grid->edges[iEdge].c2;

			Vector n = grid->edges[iEdge].n;
			double l = grid->edges[iEdge].l;

			KEpsParam pL, pR;
			kEpsReconstruct(iEdge, pL, pR);

			if (c2 <= -1)
				continue;

			double ro_m = (pL.r + pR.r) / 2.0;
			double u_m = (pL.u + pR.u) / 2.0;
			double v_m = (pL.v + pR.v) / 2.0;
			double k_m = (pL.k + pR.k) / 2.0;
			double eps_m = (pL.eps + pR.eps) / 2.0;
			double muT_m = (pL.muT + pR.muT) / 2.0;

			double Txx_m = (Txx[c1] + Txx[c2]) / 2.0;
			double Tyy_m = (Tyy[c1] + Tyy[c2]) / 2.0;
			double Txy_m = (Txy[c1] + Txy[c2]) / 2.0;

			Vector gradK_m;
			gradK_m.x = (gradK[c1].x + gradK[c2].x) / 2.0;
			gradK_m.y = (gradK[c1].y + gradK[c2].y) / 2.0;

			Vector gradEps_m;
			gradEps_m.x = (gradEps[c1].x + gradEps[c2].x) / 2.0;
			gradEps_m.y = (gradEps[c1].y + gradEps[c2].y) / 2.0;


			// ������ ������
			register double tmp1 = ro_m * (u_m * n.x + v_m * n.y) * l;
			rk_int[c1] -= k_m * tmp1;
			rk_int[c2] += k_m * tmp1;

			reps_int[c1] -= eps_m * tmp1;
			reps_int[c2] += eps_m * tmp1;

			// ������ ������
			tmp1 = (mu + muT_m / Sigma_K) * ( gradK_m.x * u_m + gradK_m.y * v_m ) * l;
			rk_int[c1] += tmp1;
			rk_int[c2] -= tmp1;

			tmp1 = (mu + muT_m / Sigma_Eps) * ( gradEps_m.x * u_m + gradEps_m.y * v_m ) * l;
			reps_int[c1] += tmp1;
			reps_int[c2] -= tmp1;
		}

		for (int iCell = 0; iCell < nc; iCell++)
		{
			register double si = grid->cells[iCell].S;
			// TODO: ���������
			double rPk = Txx[iCell] * gradU[iCell].x + Txy[iCell] * ( gradU[iCell].y + gradV[iCell].x ) + Tyy[iCell] * gradV[iCell].y;

			// ����������� ������!
			rPk += 2.0 / 3.0 * ( ( ( rk[iCell] + muT[iCell] * gradU->x ) * gradU->x ) + ( ( rk[iCell] + muT[iCell] * gradV->y ) * gradV->y ) );

			// TODO: �����, �����!
			rk_int[iCell] += si * rPk - si * reps[iCell];
			reps_int[iCell] += -si * rPk * C_eps1 * reps[iCell] / rk[iCell] - si * C_eps2 * reps[iCell] * reps[iCell] / rk[iCell];
		}

		for (int iCell = 0; iCell < nc; iCell++)
		{
			// TODO: �����, �����!
			register double cfl = cTau[iCell] / 100.0 / grid->cells[iCell].S;
			rk[iCell] += cfl * rk_int[iCell];
			reps[iCell] += cfl * reps_int[iCell];

			muT[iCell] = C_mu * rk[iCell] * rk[iCell] / reps[iCell];
		}
	}
}

void KEpsModel::calcGrad()
{
	int nc = grid->cCount;
	int ne = grid->eCount;

	memset(gradK, 0, nc * sizeof(Vector));
	memset(gradEps, 0, nc * sizeof(Vector));

	for (int iEdge = 0; iEdge < ne; iEdge++)
	{

		int c1 = grid->edges[iEdge].c1;
		int c2 = grid->edges[iEdge].c2;

        KEpsParam pL, pR;
		kEpsReconstruct(iEdge, pL, pR);

		Vector n = grid->edges[iEdge].n;
		double l = grid->edges[iEdge].l;

		gradK[c1].x += ( pL.k+pR.k ) / 2.0 * n.x * l;
		gradK[c1].y += ( pL.k+pR.k ) / 2.0 * n.y * l;
		gradEps[c1].x += ( pL.eps+pR.eps ) / 2.0 * n.x * l;
		gradEps[c1].y += ( pL.eps+pR.eps ) / 2.0 * n.y * l;

		if (c2 > -1)
		{
			gradK[c2].x -= ( pL.k+pR.k ) / 2.0 * n.x * l;
			gradK[c2].y -= ( pL.k+pR.k ) / 2.0 * n.y * l;
			gradEps[c2].x -= ( pL.eps+pR.eps ) / 2.0 * n.x * l;
			gradEps[c2].y -= ( pL.eps+pR.eps ) / 2.0 * n.y * l;
		}

	}

	for (int iCell = 0; iCell < nc; iCell++)
	{
#ifdef _DEBUG
		KEpsParam par;
		kEpsConvertConsToPar(iCell, par);
#endif

		register double si = grid->cells[iCell].S;
		gradK[iCell].x /= si;
		gradK[iCell].y /= si;
		gradEps[iCell].x /= si;
		gradEps[iCell].y /= si;
	}
}

void KEpsModel::kEpsReconstruct( int iEdge, KEpsParam& pL, KEpsParam& pR )
{
	if (grid->edges[iEdge].type == Edge::TYPE_INNER) 
	{
		int c1	= grid->edges[iEdge].c1;
		int c2	= grid->edges[iEdge].c2;
		kEpsConvertConsToPar(c1, pL);
		kEpsConvertConsToPar(c2, pR);
	} else {
		int c1	= grid->edges[iEdge].c1;
		kEpsConvertConsToPar(c1, pL);
		boundaryCond(iEdge, pL, pR);
	}
}

void KEpsModel::kEpsConvertConsToPar( int iCell, KEpsParam& par )
{
	par.r = ro[iCell];
	par.u = ru[iCell]/ro[iCell];
	par.v = rv[iCell]/ro[iCell];
	
	par.k = rk[iCell]/ro[iCell];
	par.eps = reps[iCell]/ro[iCell];

	par.muT = muT[iCell];
}

void KEpsModel::kEpsConvertParToCons( int iCell, KEpsParam& par )
{
	ro[iCell] = par.r;
	ru[iCell] = par.u * par.r;
	rv[iCell] = par.v * par.r;

	rk[iCell] = par.k * par.r;
	reps[iCell] = par.eps * par.r;

	muT[iCell] = par.muT;
}

void KEpsModel::boundaryCond( int iEdge, KEpsParam& pL, KEpsParam& pR )
{
	// TODO: ������, ��������� �� ���
	pR = pL;
}
