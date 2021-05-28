#pragma once

class CIntMatrix
{
	int	*m_pInts;
	int	m_Columns;
	int m_Rows;
	int m_TotalItems;
public:
	CIntMatrix();
	CIntMatrix(int r, int c);
	~CIntMatrix();
	int& operator()(int r, int c);
	const int& operator()(int r, int c) const;
	inline int GetRows(void) { return m_Rows; }
	inline int GetCols(void) { return m_Columns; }
	void Print(FILE* pOut, const char *Title);
};

