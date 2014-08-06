#include "global.h"
#include <math.h>

FILE * hLog;




void Material::URS(Param &par, int flag)
{
	double Cv = Cp-gR/M;
	double gam = Cp/Cv; 
	switch (flag)
	{
	case 0:		// p=p(r,e)
		par.p = par.r*par.e*(gam-1);
		par.cz = sqrt(gam*par.p/par.r);
		break;
	
	case 1:		// e=e(r,p)
		par.e = par.p/(par.r*(gam-1));
		par.T = par.e/Cv;				// TODO: ��������� ������������
		break;
	
	case 2:		// r=r(T,p)
		par.r = par.p*M/(par.T*gR);
		par.cz = sqrt(gam*par.p/par.r);
		break;
	}
}





void log(char * format, ...)
{
	va_list arglist;

    va_start(arglist,format);

        vprintf(format, arglist);
        vfprintf(hLog, format, arglist);

    va_end(arglist);
	
	fflush(stdout);
	fflush(hLog);
}

void EXIT(int err)
{
	fclose(hLog);
	exit(err);
}



/**
 *	������� ������ � ������� ������������� �������
 *	(�) ��� ��. �.�.������� ���, ������, �������, ���������
 *
 *	c==========================================================
 *	C    Nikichine
 *	C    module for tube.for /Zmitrenko/
 *	c==========================================================
 */
void rim(double& RI, double& EI, double& PI, double& UN, double& UI, double& VI,
         double RB, double PB, double UNB, double UB, double VB,
         double RE, double PE, double UNE, double UE, double VE, Vector n, double GAM) {

	double AGAM  =  (GAM-1.0);
	double BGAM  =  (2.0*sqrt(GAM/AGAM));
	double CGAM  =  (1.0/GAM);
	double DGAM  =  (2.0/AGAM);
	double EGAM  =  (AGAM/(GAM+1.0));
	double GGAM  =  (sqrt(GAM*AGAM));
	double HGAM  =  (AGAM/2.0);
	double FGAM  =  (3.0*GAM-1.0);
	double OGAM  =  (AGAM/(2.0*GAM));
	double QGAM  =  (GAM+1.0);
	double PGAM  =  (QGAM/(2.0*GAM));
	double RGAM  =  (4.0*GAM);
	double SGAM  =  (GAM*AGAM);
	double TGAM  =  (QGAM/2.0);
	double UGAM  =  (sqrt(AGAM/GAM));

	double RF,RS,EF,ES,SBL,SFL,SSL,SEL,D;
	double    eps=1.0e-5;
	double    CB= sqrt(GAM*PB/RB);
	double    CE= sqrt(GAM*PE/RE);
	double    EB= CB*CB/SGAM;
	double    EE= CE*CE/SGAM;
	double    RCB=RB*CB;
	double    RCE=RE*CE;
	double    DU=UNB-UNE;
      if (DU < -2.0*(CB+CE)/AGAM) {
         //printf(" ATTENTION!!!  VACUUM \n");
         RF=0.0;
         RS=0.0;
         EF=0.0;
         ES=0.0;
         SBL=UNB-CB;
         SFL=UNB+2.0*CB/AGAM;
         SSL=UNE-2.0*CE/AGAM;
         SEL=UNE+CE;
         goto lbl9;
      }
      double P=(PB*RCE+PE*RCB+DU*RCB*RCE)/(RCB+RCE);
    lbl5:
           if (P<eps) P=eps;

           double PPB=P/PB;
           if(PB>P) goto lbl1;
           double PKB=PGAM*PPB+OGAM;
           double ZNB=RCB*sqrt(PKB);
           double F1=(P-PB)/ZNB ;
           double FS1=(QGAM*PPB+FGAM)/(RGAM*ZNB*PKB);
           goto lbl2;
    lbl1:      
           double ZFB=CB*exp(log(PPB)*OGAM) ;
           F1=DGAM*(ZFB-CB);
           FS1=ZFB/(GAM*P);
    lbl2:      
           double PPE=P/PE;
           if(PE>P) goto lbl3;
           double PKE=PGAM*PPE+OGAM;
           double ZNE=RCE*sqrt(PKE);
           double F2=(P-PE)/ZNE;
           double FS2=(QGAM*PPE+FGAM)/(RGAM*ZNE*PKE);
           goto lbl4;
    lbl3:      
           double ZFE=CE*exp(log(PPE)*OGAM);
           F2=DGAM*(ZFE-CE);
           FS2=ZFE/(GAM*P);
    lbl4:      
           double DP=(DU-F1-F2)/(FS1+FS2);
           P=P+DP;
       if(fabs(DU-F1-F2)>eps) goto lbl5;
 

      PPB=P/PB;
      PPE=P/PE;

//       ZFB=CB*PPB**OGAM;
//       ZFE=CE*PPE**OGAM;
      ZFB=CB*exp(log(PPB)*OGAM);
      ZFE=CE*exp(log(PPE)*OGAM);
      if (PB>P) goto lbl6 ;
      D=UNB-sqrt((TGAM*P+HGAM*PB)/RB);
      double UBD=UNB-D;
      double RUBD=RB*UBD;
      RF=RUBD*RUBD/(PB-P+RUBD*UBD);
      double UF=D+RUBD/RF;
      EF=P/(AGAM*RF);
      SBL=D;
      SFL=D;
      goto lbl7;
    lbl6: 
      EF=ZFB*ZFB/SGAM;
      UF=UNB+DGAM*(CB-ZFB);
      RF=P/(AGAM*EF);
      SBL=UNB-CB;
      SFL=UF-ZFB;
    lbl7: 
      if (PE>P) goto lbl8;
      D=UNE+sqrt((TGAM*P+HGAM*PE)/RE);
      double UED=UNE-D;
      double RUED=RE*UED;
      RS=RUED*RUED/(PE-P+RUED*UED);
      double US=D+RUED/RS;
      ES=P/(AGAM*RS);
      SEL=D;
      SSL=D;
      goto lbl9;
    lbl8: 
      ES=ZFE*ZFE/SGAM;
      US=UNE-DGAM*(CE-ZFE);
      RS=P/(AGAM*ES);
      SSL=US+ZFE ;
      SEL=UNE+CE ;
    lbl9: 
// 
// C     compute the interpolation value
      if (SEL<=0.0) {
         RI= RE;
         EI= EE;
         UN= UNE;
         goto lbl157;
      }

      if (SBL>=0.0) {
         RI= RB;
         EI= EB;
         UN= UNB;
         goto lbl157;
      }

      if ((SSL>=0.0)&&(SFL<=0.0)) {
         if (US>=0.0) {
            RI= RF;
            EI= EF;
            UN= UF;
         } else {
            RI= RS;
            EI= ES;
            UN= US;
         }
         goto lbl157;
      }

      if (SFL>0.0) {
         UN= (UNB+DGAM*GGAM*sqrt(EB))/(1+DGAM);
         EI= (UN*UN)/SGAM;
         RI= RB*exp(log(EI/EB)*(1/AGAM));
       } else {
         UN= (UNE-DGAM*GGAM*sqrt(EE))/(1+DGAM);
         EI= (UN*UN)/SGAM;
         RI= RE*exp(log(EI/EE)*(1/AGAM)) ;
      }

  lbl157:
      PI= AGAM*EI*RI;
 
	  if (UN > 0)
	  {
		  UI = UB + (UN-UNB)*n.x;
		  VI = VB + (UN-UNB)*n.y;
	  } else {
		  UI = UE + (UN-UNE)*n.x;
		  VI = VE + (UN-UNE)*n.y;
	  }
} 
void rim_orig(double& RI, double& EI, double& PI, double& UI, double& VI, double& WI,
         double RB, double PB, double UB, double VB, double WB,
         double RE, double PE, double UE, double VE, double WE, double GAM) {

	double AGAM  =  (GAM-1.0);
	double BGAM  =  (2.0*sqrt(GAM/AGAM));
	double CGAM  =  (1.0/GAM);
	double DGAM  =  (2.0/AGAM);
	double EGAM  =  (AGAM/(GAM+1.0));
	double GGAM  =  (sqrt(GAM*AGAM));
	double HGAM  =  (AGAM/2.0);
	double FGAM  =  (3.0*GAM-1.0);
	double OGAM  =  (AGAM/(2.0*GAM));
	double QGAM  =  (GAM+1.0);
	double PGAM  =  (QGAM/(2.0*GAM));
	double RGAM  =  (4.0*GAM);
	double SGAM  =  (GAM*AGAM);
	double TGAM  =  (QGAM/2.0);
	double UGAM  =  (sqrt(AGAM/GAM));

	double US = 0.0, UF = 0.0;

	double RF,RS,EF,ES,SBL,SFL,SSL,SEL,D;
	double    eps=1.0e-5;
	double    CB= sqrt(GAM*PB/RB);
	double    CE= sqrt(GAM*PE/RE);
	double    EB= CB*CB/SGAM;
	double    EE= CE*CE/SGAM;
	double    RCB=RB*CB;
	double    RCE=RE*CE;
	double    DU=UB-UE;
      if (DU < -2.0*(CB+CE)/AGAM) {
         //printf(" ATTENTION!!!  VACUUM \n");
         RF=0.0;
         RS=0.0;
         EF=0.0;
         ES=0.0;
         SBL=UB-CB;
         SFL=UB+2.0*CB/AGAM;
         SSL=UE-2.0*CE/AGAM;
         SEL=UE+CE;
         goto lbl9;
      }
      double P=(PB*RCE+PE*RCB+DU*RCB*RCE)/(RCB+RCE);
    lbl5:
           if (P<eps) P=eps;

           double PPB=P/PB;
           if(PB>P) goto lbl1;
           double PKB=PGAM*PPB+OGAM;
           double ZNB=RCB*sqrt(PKB);
           double F1=(P-PB)/ZNB ;
           double FS1=(QGAM*PPB+FGAM)/(RGAM*ZNB*PKB);
           goto lbl2;
    lbl1:      
           double ZFB=CB*exp(log(PPB)*OGAM) ;
           F1=DGAM*(ZFB-CB);
           FS1=ZFB/(GAM*P);
    lbl2:      
           double PPE=P/PE;
           if(PE>P) goto lbl3;
           double PKE=PGAM*PPE+OGAM;
           double ZNE=RCE*sqrt(PKE);
           double F2=(P-PE)/ZNE;
           double FS2=(QGAM*PPE+FGAM)/(RGAM*ZNE*PKE);
           goto lbl4;
    lbl3:      
           double ZFE=CE*exp(log(PPE)*OGAM);
           F2=DGAM*(ZFE-CE);
           FS2=ZFE/(GAM*P);
    lbl4:      
           double DP=(DU-F1-F2)/(FS1+FS2);
           P=P+DP;
       if(fabs(DU-F1-F2)>eps) goto lbl5;
 

      PPB=P/PB;
      PPE=P/PE;

//       ZFB=CB*PPB**OGAM;
//       ZFE=CE*PPE**OGAM;
      ZFB=CB*exp(log(PPB)*OGAM);
      ZFE=CE*exp(log(PPE)*OGAM);
      if (PB>P) goto lbl6 ;
      D=UB-sqrt((TGAM*P+HGAM*PB)/RB);
      double UBD=UB-D;
      double RUBD=RB*UBD;
      RF=RUBD*RUBD/(PB-P+RUBD*UBD);
      UF=D+RUBD/RF;
      EF=P/(AGAM*RF);
      SBL=D;
      SFL=D;
      goto lbl7;
    lbl6: 
      EF=ZFB*ZFB/SGAM;
      UF=UB+DGAM*(CB-ZFB);
      RF=P/(AGAM*EF);
      SBL=UB-CB;
      SFL=UF-ZFB;
    lbl7: 
      if (PE>P) goto lbl8;
      D=UE+sqrt((TGAM*P+HGAM*PE)/RE);
      double UED=UE-D;
      double RUED=RE*UED;
      RS=RUED*RUED/(PE-P+RUED*UED);
      US=D+RUED/RS;
      ES=P/(AGAM*RS);
      SEL=D;
      SSL=D;
      goto lbl9;
    lbl8: 
      ES=ZFE*ZFE/SGAM;
      US=UE-DGAM*(CE-ZFE);
      RS=P/(AGAM*ES);
      SSL=US+ZFE ;
      SEL=UE+CE ;
    lbl9: 
// 
// C     compute the interpolation value
      if (SEL<=0.0) {
         RI= RE;
         EI= EE;
         UI= UE;
         VI= VE;
         WI= WE;
         goto lbl157;
      }

      if (SBL>=0.0) {
         RI= RB;
         EI= EB;
         UI= UB;
         VI= VB;
         WI= WB;
         goto lbl157;
      }

      if ((SSL>=0.0)&&(SFL<=0.0)) {
         if (US>=0.0) {
            RI= RF;
            EI= EF;
            UI= UF;
            VI= VB;
            WI= WB;
         } else {
            RI= RS;
            EI= ES;
            UI= US;
            VI= VE;
            WI= WE;
         }
         goto lbl157;
      }

      if (SFL>0.0) {
         UI= (UB+DGAM*GGAM*sqrt(EB))/(1+DGAM);
         VI= VB;
		 WI= WB;
		 EI= (UI*UI)/SGAM;
         RI= RB*exp(log(EI/EB)*(1/AGAM));
       } else {
         UI= (UE-DGAM*GGAM*sqrt(EE))/(1+DGAM);
         VI= VE;
		 WI= WE;
		 EI= (UI*UI)/SGAM;
         RI= RE*exp(log(EI/EE)*(1/AGAM)) ;
      }

  lbl157:
      PI= AGAM*EI*RI;
 
} 

void roe_orig(double& RI, double& EI, double& PI, double& UI, double& VI, double& WI,
         double RB, double PB, double UB, double VB, double WB,
         double RE, double PE, double UE, double VE, double WE, double GAM) {

	double AGAM  =  (GAM-1.0);

	// ����� ROE
	
	double fG = GAM;

	double fSB = sqrt( RB );
	double fSE = sqrt( RE );
	double fS_ = 1.0 / ( fSB + fSE );

	RI = fSB * fSE;
		
	UI = ( fSB * UB + fSE * UE ) * fS_;
	VI = ( fSB * VB + fSE * VE ) * fS_;
	WI = ( fSB * WB + fSE * WE ) * fS_;

	double EB = PB/(RB*AGAM);
	double EE = PE/(RE*AGAM);
	//EI = ( fSB * EB + fSE * EE ) * fS_;
	//TI = ( fSB * TB + fSE * TE ) * fS_;
		
		
	double HB = EB + (UB*UB+VB*VB+WB*WB)*0.5 + PB / RB;
	double HE = EE + (UE*UE+VE*VE+WE*WE)*0.5 + PE / RE;
		
	double HI = ( fSB * HB + fSE * HE ) * fS_;

	PI = ( HI - (UI*UI+VI*VI+WI*WI)*0.5 ) * RI * ( fG - 1.0 ) / fG;
	EI = PI/(RI*AGAM);

} 



void inverseMatr(double** a_src, double **am, int N)
{
	int	*	mask;
	double	fmaxval;
	int		maxind;
	int		tmpi;
	double	tmp;
	//double	a[N][N];

	double	**a;

	mask = new int[N];
	a = new double*[N];
	for (int i = 0; i < N; i++)
	{
		a[i] = new double[N];
		for (int j = 0; j < N; j++)
		{
			a[i][j] = a_src[i][j];
		}
	}
	//::memcpy(a, a_src, sizeof(double)*N*N);

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (i == j)
			{
				am[i][j] = 1.0;
			}
			else {
				am[i][j] = 0.0;
			}
		}
	}
	for (int i = 0; i < N; i++)
	{
		mask[i] = i;
	}
	for (int i = 0; i < N; i++)
	{
		maxind = i;
		fmaxval = fabs(a[i][i]);
		for (int ni = i + 1; ni < N; ni++)
		{
			if (fabs(fmaxval) <= fabs(a[ni][i]))
			{
				fmaxval = fabs(a[ni][i]);
				maxind = ni;
			}
		}
		fmaxval = a[maxind][i];
		if (fmaxval == 0)
		{
			log("ERROR! Determinant of mass matrix is zero...\n");
			return;
		}
		if (i != maxind)
		{
			for (int nj = 0; nj < N; nj++)
			{
				tmp = a[i][nj];
				a[i][nj] = a[maxind][nj];
				a[maxind][nj] = tmp;

				tmp = am[i][nj];
				am[i][nj] = am[maxind][nj];
				am[maxind][nj] = tmp;
			}
			tmpi = mask[i];
			mask[i] = mask[maxind];
			mask[maxind] = tmpi;
		}
		double aii = a[i][i];
		for (int j = 0; j < N; j++)
		{
			a[i][j] = a[i][j] / aii;
			am[i][j] = am[i][j] / aii;
		}
		for (int ni = 0; ni < N; ni++)
		{
			if (ni != i)
			{
				double fconst = a[ni][i];
				for (int nj = 0; nj < N; nj++)
				{
					a[ni][nj] = a[ni][nj] - fconst *  a[i][nj];
					am[ni][nj] = am[ni][nj] - fconst * am[i][nj];
				}
			}
		}
	}
	//for (int i = 0; i < N; i++)
	//{
	//	if (mask[i] != i) 
	//	{
	//		for (int j = 0; j < N; j++) 
	//		{
	//			tmp				= a[i][j];
	//			a[i][j]			= a[mask[i]][j];
	//			a[mask[i]][j]	= tmp;
	//		}
	//	}
	//}
	for (int i = 0; i < N; i++)
	{
		delete[] a[i];
	}
	delete[] a;
	delete[] mask;
	return;
}

