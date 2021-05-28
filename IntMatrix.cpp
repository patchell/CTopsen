#include <stdio.h>
#include <stdlib.h>
#include "IntMatrix.h"

CIntMatrix::CIntMatrix()
{
	m_Rows = 0;
	m_Columns = 0;
	m_pInts = 0;
	m_TotalItems = 0;
}

CIntMatrix::CIntMatrix(int row,int col)
{
	m_Rows = row;
	m_Columns = col;
	m_TotalItems = row * col;
	m_pInts = new int[m_TotalItems];
	int i;
	for (i = 0; i < m_TotalItems; ++i) m_pInts[i] = 0;
}

CIntMatrix::~CIntMatrix()
{
	if (m_pInts)
		delete[] m_pInts;
}

int& CIntMatrix::operator()(int r, int c)
{
	///------------------------------------------------------------
	/// Operator()
	///		parameters:
	///			r........row
	///			c........column
	///		returns:
	///			the (i,j) element of the matrix
	///---------------------------------------------------------------
	if (r >= m_Rows || r < 0)
	{
		printf("Bad Row Number %d\n", r);
		exit(1);
	}
	if (c >= m_Columns || c < 0)
	{
		printf("Bad Column Number %d\n", c);
		exit(1);
	}
	int index = c + r * m_Columns;
	if (index >= m_TotalItems) fprintf(stderr, "ERROR:Matrix Index row=%d  col=%d m=%d  n=%d max=%d Index=%d\n", r, c, m_Columns, m_Rows, m_TotalItems, index);
	return m_pInts[index];
}

const int& CIntMatrix::operator()(int r, int c) const
{
	///------------------------------------------------------------
	/// Operator()
	///		parameters:
	///			r........row
	///			c........column
	///		returns:
	///			the (i,j) element of the matrix
	///---------------------------------------------------------------
	if (r >= m_Rows || r < 0)
	{
		printf("Bad Row Number %d\n", r);
		exit(1);
	}
	if (c >= m_Columns || c < 0)
	{
		printf("Bad Column Number %d\n", c);
		exit(1);
	}
	int index = c + r * m_Columns;
	if (index >= m_TotalItems) fprintf(stderr, "ERROR:Matrix Index i=%d  j=%d m=%d  n=%d max=%d Index=%d\n", r, c, m_Columns, m_Rows, m_TotalItems, index);
	return m_pInts[index];
}

void CIntMatrix::Print(FILE* out, const char *title)
{
	int i, j;
	
	for (i = 0; i < m_Rows; ++i)	// rows
	{
		fprintf(out,"ROW %3d: ", i);
		for (j = 0; j < m_Columns; ++j)	// columns
		{
			fprintf(out,"%6d  ", m_pInts[m_Columns * i +  j]);
		}
		fprintf(out,"\n");
	}
}
