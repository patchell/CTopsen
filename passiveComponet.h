#pragma once

class passiveComponent
{
	int mNumber;
	double mValue;
	int mNode1;
	int mNode2;
	int mType;	//1=R,2=L,3=C
public:
	passiveComponent();
	virtual ~passiveComponent();
	inline int GetNumber(void) { return mNumber; }
	inline void SetNumber(int n) { mNumber = n; }
	inline double GetValue(void) { return mValue; }
	inline void SetValue(double v) { mValue = v; }
	inline int GetNode1(void) { return mNode1; }
	void SetNode1(int n1) { mNode1 = n1; }
	int GetNode2(void) { return mNode2; }
	void SetNode2(int n2) { mNode2 = n2; }
	int GetType(void) { return mType; }
	void SetType(int t) { mType = t; }
	void Print();
	void Input();
	static const char* GetTypeString(int CompType);
};

