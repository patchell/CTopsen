#include "doubleMatrix.h"

CdoubleMatrix::CdoubleMatrix()
{
	m_Rows = 0;
	m_Columns = 0;
	m_pDoubles = 0;
	m_TotalItems = 0;
	m_pDoubles = 0;
}

CdoubleMatrix::CdoubleMatrix(int r, int c)
{
	m_TotalItems = r * c;
	m_Rows = r;
	m_Columns = c;
	m_pDoubles = new double[m_TotalItems];
	int i;
	for (i = 0; i < m_TotalItems; ++i) m_pDoubles[i] = 0.0;
}

CdoubleMatrix::~CdoubleMatrix()
{
	if (m_pDoubles) delete[] m_pDoubles;
}

double& CdoubleMatrix::operator()(int i, int j)
{
	int index = j + i * m_Columns;
	if (index >= m_TotalItems) fprintf(stderr, "Error:Index exceeds bounds [%d]\n",index);
	return m_pDoubles[index];
}

const double& CdoubleMatrix::operator()(int i, int j) const
{
	int index = j * m_Rows + i;
	if (index >= m_TotalItems) fprintf(stderr, "Error:Index exceeds bounds[%d]\n", index);
	return m_pDoubles[index];
}

void CdoubleMatrix::Print(FILE* pOut)
{
	int i, j;

	for (i = 0; i < m_Rows; ++i)	//colums
	{
		printf("ROW %3d: ", i);
		for (j = 0; j < m_Columns; ++j)	// rows
		{
			printf("%10.2lf  ", m_pDoubles[i * m_Columns +  j]);
		}
		printf("\n");
	}
}
