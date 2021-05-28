#pragma once

#define ACTIVE_TYPE_GM		1
#define ACTIVE_TYPE_GM_D_S	2
#define ACTIVE_TYPE_GM_T_S	3

class activeComponent
{
	int mNumber;
	int mType;
	double mValue;
	int mSend_P;	//U[N,2]
	int mSend_N;	//U[N,3]
	int mRec_P;		//U[N,4]
	int mRec_N;		//U[N,5]
public:
	activeComponent();
	virtual ~activeComponent();
	inline double GetValue() { return mValue; }
	inline void SetValue(double v) { mValue = v; }
	inline void SetSendP(int n) { mSend_P = n; }
	inline int GetSendP(void) { return mSend_P; }
	inline void SetSendN(int n) { mSend_N = n; }
	inline int GetSendN(void) { return mSend_N; }
	inline void SetRecP(int n) { mRec_P = n; }
	inline int GetRecP(void) { return mRec_P; }
	inline void SetRecN(int n) { mRec_N = n; }
	inline int GetRecN(void) { return mRec_N; }
	inline void SetNumber(int n) { mNumber = n; }
	inline int GetNumber(void) { return mNumber; }
	inline void SetType(int t) { mType = t; }
	inline int GetType(void) { return mType; }
	void Print();
	void Input();
};


