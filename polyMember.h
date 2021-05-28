#pragma once
class CpolyMember
{
	double mNumerator;
	double mDenominator;
public:
	CpolyMember() { mNumerator = 0.0; mDenominator = 0.0; }
	~CpolyMember() {}
	void SetNumerator(double n) { mNumerator = n; }
	double GetNumerator() { return mNumerator; }
	void SetDenominator(double d) { mDenominator = d; }
	double GetDenominator() { return mDenominator; }
};

