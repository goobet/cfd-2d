#ifndef _GRID_H_
#define _GRID_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "global.h"

class Cell 
{
public:
	Cell(): nodesInd(NULL), edgesInd(NULL), nCount(0), eCount(0) {};
	~Cell();

	int   nCount;
	int   eCount;
	int*  nodesInd;
	int*  edgesInd;
	int	  neigh[3];
	//Point   side[3][6]; //�������� ����� �������������� ������ ������������, ������ ����� ���������� ��������������
	//int sideType; //0 - ������������� ����� ������, 1 - ������� �� ������� ��������������� �������
	int	  type;
	double  S;
	double R; //������ ��������� ����������
	Point c; // ����� ������
	double HX;
	double HY;
	friend class Grid;
};

class Edge 
{
public:
	Edge(): c(NULL), cCount(0) {};
	~Edge();

	int      n1;        // ���� � ������
	int      n2;        // ���� � �����
	int      c1;        // ������ �����
	int      c2;        // ������ ������, ������� �� �1 � �2
	Vector   n;         // ������� � �����
	double     l;         // ����� ����� 
	double     cnl1;       // ����� ���� �������� �������� �� ������ ����� �� ������ ������� ������
	double     cnl2;       // ����� ���� �������� �������� �� ������ ����� �� ������ ������� ������
	int      cCount;    // ���������� ����� �� �����
	Point*   c;         // ����� �� �����
	int      type;      // ��� ����� (�����., ������.)
	Point otr[6]; //���������� 6-�� �����, ������� �� �������������� � �����
	int seq_tri[6]; //������������������ �������������, � ������� ����� ����� ��������������
	int wenoType; // ����� weno ������������� ��� ��� (0 - �����, 1 - �� �����)
	friend class Grid;
public:
	static const int TYPE_INNER		= 0;  
	static const int TYPE_INLET		= 1;  
	static const int TYPE_OUTLET	= 2;  
	static const int TYPE_WALL		= 3;  
};

class Grid 
{
public:
	Grid(): nodes(NULL), cells(NULL), edges(NULL),
		nCount(0), cCount(0), eCount(0) {};
	~Grid();

	void initFromFiles(char* fName);
	inline Point& getNode(int i) { return nodes[i]; };
	inline Cell&  getCell(int i) { return cells[i]; };
	inline Edge&  getEdge(int i) { return edges[i]; };
	void replaceEdges(int if1, int if2);
	virtual void reorderEdges();
	int findEdge(int n1, int n2);
	int location_node (int possible_tri,double x0,double y0); //��������� � ����� ������������ ����� �����
	int inArea (double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4); //����������, ������������ �� ������� � ������������ (x1,y1);(x2,y2) � �������� � ������������ (x3,y3);(x4,y4)   //0 - �� ������������, 1 - ������������
	void WENOforEdge (int node1, int node2, int cellL, int cellR, double *uwx, double *uwy); //���� � ������, ���� � �����, ������ �����, ������ ������

	Point* nodes;
	Cell*  cells;
	Edge*  edges;
	int    nCount;
	int    cCount;
	int    eCount;
};

inline double _max_(double a, double b)
{
	if (a>b) { return a; } else {return b; }
}

inline double _min_(double a, double b)
{
	if (a<b) { return a; } else {return b; }
}

inline double _max_(double a, double b, double c)
{
	return _max_(a, _max_(b, c));
}

inline double _min_(double a, double b, double c)
{
	return _min_(a, _min_(b, c));
}

inline double _sqr_(double a)
{
	return a*a;
}

#endif