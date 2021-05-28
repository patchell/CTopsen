#pragma once

#include <stdio.h>

class CdoubleMatrix
{
	double* m_pDoubles;
	int	m_Columns;
	int m_Rows;
	int m_TotalItems;
public:
	CdoubleMatrix();
	CdoubleMatrix(int r, int c);
	~CdoubleMatrix();
	double& operator()(int i, int j);
	const double& operator()(int i, int j) const;
	void Print(FILE* pOut);
};

