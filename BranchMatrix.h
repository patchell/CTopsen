#pragma once

#include "passiveComponet.h"
#include "activeComponent.h"
#include "IntMatrix.h"


class BranchMatrix
{
	int mType;
	int mComponentNumber;
	int mNode1;
	int mNode2;
	double  mScratch;
	int mParrallelBranch;
	double mValue;
	int* W;
	int nElements;
public:
	BranchMatrix();
	virtual ~BranchMatrix();
	inline void SetNumbeerOfElements(int n) { nElements = n; }
	inline void SetNode1(int N1) { mNode1 = N1; }
	inline int GetNode1() { return mNode1; }
	inline void SetNode2(int N2) { mNode2 = N2; }
	inline int GetNode2() { return mNode2; }
	inline void SetNumber(int n) { mComponentNumber = n; }
	inline int GetNumber() { return mComponentNumber; }
	inline void SetScratch(double scratch) { mScratch = scratch; }
	inline double GetScratch() { return mScratch; }
	inline void SetParBranch(int b) { mParrallelBranch = b; }
	inline int GetParBranch(void) { return mParrallelBranch; }
	inline double GetValue() { return mValue; }
	inline void SetValue(double v) { mValue = v; }
	inline void SetType(int t) { mType = t; }
	inline int GetType(void) { return mType; }
	void Print(FILE* out, const char* title);
};


