#pragma once

#include "activeComponent.h"
#include "passiveComponet.h"
#include "BranchMatrix.h"
#include "IntMatrix.h"
#include "doubleMatrix.h"
#include "Consol.h"
#include "polyMember.h"

extern CConsol* pConsol;

class CTopsenMain
{
	CpolyMember* pTransferFunction;
	int NumberOfNodes;		// N
	int InputNode;			// N1
	int InputNodeMirror;	// N0
	int OutputNode;			// N2
	int NumberOfResistors;	// R9
	int NumberOfInductors;	// L9
	int NumberOfCapacitors;	// C9
	int NumberOfVCCS;		// G9
	int NumberOfPassives;	// P9
	int TotalComponents;	// T9
	passiveComponent* m_pRes;	//resistors S[]
	passiveComponent* m_pInd;	//inductors M[]
	passiveComponent* m_pCap;	//capacitors A[]
	activeComponent* m_pGm;	//voltage controlled current source G[]
	int CmdPre;		// I7
	int CmdPost;	// I6
	int T3;	//?
	BranchMatrix* pBM;	// U[]
	int* W;				// W[]
	CIntMatrix* NodeBranchMatrix;	// V
	int G6, G7;		// G6,G7
	int NumberOfReactiveComponents;	//S3
	double Pi;	//P2
	int P1;		//?
	double Q1;	//?
	CdoubleMatrix* pE;	//?
	CdoubleMatrix* pF;	//?
	int T4;				//?
	int T8;
public:
	CTopsenMain();
	~CTopsenMain();
	int MenuStringPre(char* s);
	void SetInputOutputNodes();
	int MenuStringPost(char* s);
	int DoMenu(int MenuID, const char** Cmds);
	int PreMenu(void);
	int ParseCommand(const char** Cm, const char* userinput);
	void InputDetails();
	void InputVCCS(int n, const char* Prompt, activeComponent* parts);
	void InputComponents(const char *prompt, int n, passiveComponent* parts);
	void InputAllComponents(
		passiveComponent* R,
		passiveComponent* L,
		passiveComponent* c,
		activeComponent* G,
		int nR, int nL, int nC, int nG);
	void ModifyComponents(
		passiveComponent* R,
		passiveComponent* L,
		passiveComponent* c,
		activeComponent* G
	);
	void PrintComponents(int n, const char* Title, int type, passiveComponent* parts);
	void PrintActives(int n, const char* Title, activeComponent* part);
	void PrintAllParts();
	void PrintTitle(void);
	passiveComponent* CreateComponents(int n, int type);
	activeComponent* CreateActiveComponent(int n);
	void Run();
	void Analyze();
	void FillBranchMatrix();
	int SubJLoop(int i, int& R8);
	int SubKLoop(int Node1, int i, int j);
	void GenNextTree();			// Line 3000
	void LoopTest();			//Line 4000
	void ActiveLoopTest();		//Line 5000
	void UpdateTransferFunction();	//Line 7000
	inline int Odd(int n) { return n & 1; }
	inline double MinusOneTo(int power) { return Odd(power) ? -1.0 : 1.0; }
};

