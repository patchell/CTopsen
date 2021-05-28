#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "Topsen.h"
#include "TopsenMain.h"


CConsol* pConsol;

static const char* Cmds[4] = {
	"LT",	//1
	"TF",	//2
	"SE",	//3
	NULL	//0
};
enum {
	PREMENU_NONE,
	PREMENU_LISTTREES,
	PREMENU_TRANSFERFUNCTION,
	PREMENU_TF_AND_SENSITIVITY
};

static const char* CmdComment[] = {
	"List Trees",
	"Voltage Gain",
	"Voltage Gain and Sensitivity",
	NULL
};

static const char* Pcc[9] = {	// Post Command Comments
	"End Run",
	"Eval Resp",
	"Analyze",
	"Components",
	"In/Out Nodes",
	"Outputs",
	"New Freq",
	"Print MAT",
	NULL
};

static const char* ActiveTypeString[4] = {
	"XXXX",
	" Gm ",
	"Gm/s",
	"Gm*s"
};

static const char* PassiveTypeStrings[4] = {
	"?",
	"R",
	"L",
	"C"
};

enum {
	PMENU_NONE,		//0
	PMENU_ENDRUN,	//1
	PMENU_EVALRESP,	//2
	PMENU_ANALYZE,	//3
	PMENU_MODCOMPS,	//4
	PMENU_INOUTNODES,//5
	PMENU_OUTPUTS,	//6
	PMENU_NEWFREQS,	//7
	PMENU_PRINTMATS	//8
};
static const char* PC[9] = {	//post commands
	"ER",		//end run
	"EV",		//Evaluate response
	"GO",		//Analyze
	"NC",		//Mod Components
	"NN",		//Input Output Nodes
	"NO",		//Outputs
	"NF",		//New Frequencies
	"MP",		//Print Matrixes
	NULL
};

CTopsenMain::CTopsenMain()
{
	InputNode = 0;
	OutputNode = 0;
	InputNodeMirror = 0;
	m_pRes = 0;
	m_pInd = 0;
	m_pCap = 0;
	m_pGm = 0;
	NumberOfResistors = 0;
	NumberOfInductors = 0;
	NumberOfCapacitors = 0;
	NumberOfVCCS = 0;
	NumberOfNodes = 0;
	NumberOfPassives = 0;
	TotalComponents = 0;
	CmdPre = 0;
	CmdPost = 0;
	T3 = 0;
	NodeBranchMatrix = 0;
	pTransferFunction = 0;
	G6 = 0;
	G7 = 0;
	NumberOfReactiveComponents = 0;
	W = 0;
	pBM = 0;
	Pi = 8.0 * atan(1);
	P1 = 0;
	Q1 = 0.0;
	pE = 0;
	pF = 0;
	T4 = 0;
	T8 = 0;
}

CTopsenMain::~CTopsenMain()
{
	if (m_pRes) delete[] m_pRes;
	if (m_pInd) delete[] m_pInd;
	if (m_pCap) delete[] m_pCap;
	if (m_pGm) delete[] m_pGm;
}

int CTopsenMain::MenuStringPre(char* s)
{
	int l;

	l = sprintf_s(s, 1024, "OUTPUTS:%s %s, %s %s\n", Cmds[0], CmdComment[0], Cmds[1], CmdComment[1]);
	l += sprintf_s(&s[l], 1024, "       :%s %s\n", Cmds[2], CmdComment[2]);
	return l;
}

int CTopsenMain::DoMenu(int MenuID, const char** Cmds)
{
	char* Menu = new char[1024];
	char* UsrIn = new char[256];
	int rv = 0;
	int l, i;

	switch (MenuID)
	{
	case 0:
		MenuStringPre(Menu);
		break;
	case 1:
		MenuStringPost(Menu);
		break;
	}
	while (rv == 0)
	{
		printf("%s", Menu);
		pConsol->InputString(">", UsrIn, 1);
		l = strlen(UsrIn);
		for (i = 0; i < l; ++i)
			UsrIn[i] = toupper(UsrIn[i]);
		rv = ParseCommand(Cmds, UsrIn);
		if (rv == 0)
			printf("%s:unknown command\n", UsrIn);
	}
	delete[] Menu;
	return rv;
}

int CTopsenMain::MenuStringPost(char* s)
{
	int l;

	l = sprintf_s(s, 1024, "Next Command:  %s: %s\t%s: %s\n", PC[0], Pcc[0], PC[1], Pcc[1]);
	l += sprintf_s(&s[l], 1024, "Modification:  %s: %s\t%s: %s\n", PC[3], Pcc[3], PC[4], Pcc[4]);
	l += sprintf_s(&s[l], 1024, "            :  %s: %s\t%s: %s\n", PC[5], Pcc[5], PC[6], Pcc[6]);
	l += sprintf_s(&s[l], 1024, "     OUTPUTS:  %s: %s\t%s: %s\n", PC[2], Pcc[2], PC[7], Pcc[7]);
	return l;
}

int CTopsenMain::ParseCommand(const char** Cm, const char* userinput)
{
	int rv = 0;
	int i;

	for (i = 0; Cm[i] != NULL; ++i)
	{
		if (strcmp(Cm[i], userinput) == 0)
		{
			rv = i + 1;
			break;
		}
	}
	return rv;
}

void CTopsenMain::InputVCCS(int n, const char* Prompt, activeComponent* parts)
{
	int i;
	int PartNum;

	printf("%s\n", Prompt);
	for (i = 1; i <= n; ++i)
	{
		PartNum = pConsol->InputINT("gM(?):", 0);
		parts[PartNum].SetNumber(PartNum);
		parts[PartNum].Input();
	}
}

void CTopsenMain::InputComponents(const char* prompt, int n, passiveComponent* parts)
{
	char* p = new char[256];
	printf("%s\n", prompt);
	int i;
	int PartNum;

	for (i = 1; i <= n; ++i)
	{
		sprintf_s(p, 256, "%s(?)", parts->GetTypeString(parts->GetType()));
		PartNum = pConsol->InputINT(p, 0);
		parts[PartNum].SetNumber(PartNum);
		parts[PartNum].Input();
	}
	delete[] p;
}

void CTopsenMain::PrintComponents(int n, const char* Title, int type, passiveComponent* parts)
{
	int i;
	char* s = new char[256];

	printf("** %s **\n", Title);
	for (i = 1; i <= n; ++i)
	{
		parts[i].Print();
	}
	delete[]s;
}

void CTopsenMain::PrintActives(int n, const char* Title, activeComponent* parts)
{
	int i;
	printf("%s\n", Title);
	for (i = 1; i <= n; ++i)
	{
		parts[i].Print();
	}
}

void CTopsenMain::InputAllComponents(
	passiveComponent* R,
	passiveComponent* L,
	passiveComponent* C,
	activeComponent* G,
	int nR, int nL, int nC, int nG)
{
	if (R)InputComponents("Input Resistors", nR, R);
	if (L)InputComponents("Input Inductors", nL, L);
	if (C)InputComponents("Input Capacitors", nC, C);
	if (G) InputVCCS(nG, "Input VCCS", G);
}

void CTopsenMain::PrintAllParts()
{
	if (m_pRes) PrintComponents(NumberOfResistors, "Resistors", COMP_R_TYPE, m_pRes);
	if (m_pInd) PrintComponents(NumberOfInductors, "Inductors", COMP_L_TYPE, m_pInd);
	if (m_pCap) PrintComponents(NumberOfCapacitors, "Capacitors", COMP_C_TYPE, m_pCap);
	if (m_pGm) PrintActives(NumberOfVCCS, "Active Components", m_pGm);
}

void CTopsenMain::InputDetails()
{
	NumberOfNodes = pConsol->InputINT("Number of Nodes:", 1);
	NumberOfResistors = pConsol->InputINT("Number Of Resistors:", 1);
	NumberOfInductors = pConsol->InputINT("Number Of Inductors:", 1);
	NumberOfCapacitors = pConsol->InputINT("Number Of Capacitors:", 1);
	NumberOfVCCS = pConsol->InputINT("Number of VCCSs:", 1);
}

int CTopsenMain::PreMenu(void)
{
	CmdPre = DoMenu(0, Cmds);
	switch (CmdPre)
	{
	case PREMENU_LISTTREES:		//1
		printf("Listing Trees\n");
		break;
	case PREMENU_TRANSFERFUNCTION:		//2
		printf("Tranfer Function\n");
		break;
	case PREMENU_TF_AND_SENSITIVITY:	//
		printf("Transfer Function and Sensitivity\n");
		break;
	default:
		printf("Internal Error\n");
		exit(1);
		break;
	}
	return CmdPre;
}

void CTopsenMain::Run()
{
	PrintTitle();	//Line 10
	pConsol = new CConsol;
	InputDetails();	//Line 140
	m_pRes = CreateComponents(NumberOfResistors, COMP_R_TYPE);	//line 180
	m_pInd = CreateComponents(NumberOfInductors, COMP_L_TYPE);
	m_pCap = CreateComponents(NumberOfCapacitors, COMP_C_TYPE);
	m_pGm = CreateActiveComponent(NumberOfVCCS);
	InputAllComponents(				// Line 270
		m_pRes,
		m_pInd,
		m_pCap,
		m_pGm,
		NumberOfResistors,
		NumberOfInductors,
		NumberOfCapacitors,
		NumberOfVCCS);
	PrintAllParts();
	int CommandLoop = 1;
	CmdPre = PreMenu();
	SetInputOutputNodes();	//Line 600
	NumberOfPassives = NumberOfResistors +
		NumberOfInductors +
		NumberOfCapacitors;
	TotalComponents = NumberOfPassives + NumberOfVCCS;
	pBM = new BranchMatrix[TotalComponents + 1];
	pBM->SetNumbeerOfElements(TotalComponents);

	FillBranchMatrix();
	CmdPost = DoMenu(1, PC);	//Line 2770

	switch (CmdPost)
	{
	case PMENU_NONE:		//error
		break;
	case PMENU_ENDRUN:		//goto Lin 2920
		break;
	case PMENU_EVALRESP:	//goto line 2670
		break;
	case PMENU_ANALYZE:		//goto line  2030
		break;
	case PMENU_MODCOMPS:	//goto line 240
		break;
	case PMENU_INOUTNODES:	//goto line 600
		break;
	case PMENU_OUTPUTS:		//goto line 480
		break;
	case PMENU_NEWFREQS:	//goto0 line 450
		break;
	case PMENU_PRINTMATS:	//goto line 1960
		break;
	}
}

void CTopsenMain::PrintTitle(void)
{
	printf("----------------------------------------------\n");
	printf("Topological Transfer Function Analysis Program\n");
	printf("Translated from the Basic Program in\n");
	printf("the book \"Applied Circuit Theory\"\n");
	printf("----------------------------------------------\n");
	printf("\n\n\n\n");

}

passiveComponent* CTopsenMain::CreateComponents(int n, int t)
{
	passiveComponent* pC;
	if (n)
	{
		n++;
		pC = new passiveComponent[n];
		int i;
		for (i = 0; i < n; ++i)
			pC[i].SetType(t);
	}
	else
		pC = 0;
	return pC;
}

activeComponent* CTopsenMain::CreateActiveComponent(int n)
{
	activeComponent* pAC = 0;
	if (n)
	{
		n++;
		pAC = new activeComponent[n];
	}
	return pAC;
}

void CTopsenMain::SetInputOutputNodes()
{
	InputNode = pConsol->InputINT("Input Node:", 1);
	InputNodeMirror = InputNode;
	OutputNode = pConsol->InputINT("Output Node:", 1);
}

void CTopsenMain::ModifyComponents(
	passiveComponent* R,
	passiveComponent* L,
	passiveComponent* C,
	activeComponent* G
)
{
	int nR = 0;
	int nL = 0;
	int nC = 0;
	int nG = 0;

	if (R)
	{
		printf("There are %d Resistors\n", NumberOfResistors);
		nR = pConsol->InputINT("Modify How Many?", 1);
	}
	if (L)
	{
		printf("There are %d Inductors\n", NumberOfInductors);
		nL = pConsol->InputINT("Modify How Many?", 1);
	}
	if (C)
	{
		printf("There are %d Capacitors\n", NumberOfCapacitors);
		nC = pConsol->InputINT("Modify How Many", 1);
	}
	if (G)
	{
		printf("There are %d VCCSs", NumberOfVCCS);
		nG = pConsol->InputINT("Modiy How Many", 1);
	}
	InputAllComponents(R, L, C, G, nR, nL, nC, nG);
}

void CTopsenMain::Analyze()	//Line 2030
{
	//--------------------------------
	// Make Tree List
	//--------------------------------
	int T6 = 0;	//line 2040
	int T5 = 0;	//line 2070
	int T4 = 0;	//line 2060

	(*NodeBranchMatrix)(0, 0) = 0;	//line 2050

}

void CTopsenMain::FillBranchMatrix()
{
	int i;
	int index;
	int j;
	int Node1, Node2;

	index = 1;
	for (i = 1; i <= NumberOfResistors; ++i)
	{
		pBM[i].SetType(COMP_R_TYPE);
		pBM[i].SetNumber(i);
		pBM[i].SetNode1(m_pRes[i].GetNode1());
		pBM[i].SetNode2(m_pRes[i].GetNode2());
		pBM[i].SetValue(m_pRes[i].GetValue());
	}
	index += NumberOfResistors;
	j = index;
	for (i = 1; i <= NumberOfInductors; ++i)
	{
		pBM[j].SetType(COMP_L_TYPE);
		pBM[j].SetNumber(i);
		pBM[j].SetNode1(m_pInd[i].GetNode1());
		pBM[j].SetNode2(m_pInd[i].GetNode2());
		pBM[j].SetValue(m_pInd[i].GetValue());
		j += 1;
	}
	index += NumberOfInductors;
	j = index;
	for (i = 1; i <= NumberOfCapacitors; ++i)
	{
		pBM[j].SetType(COMP_C_TYPE);
		pBM[j].SetNumber(i);
		pBM[j].SetNode1(m_pCap[i].GetNode1());
		pBM[j].SetNode2(m_pCap[i].GetNode2());
		pBM[j].SetValue(m_pCap[i].GetValue());
		j += 1;
	}
	j = index + NumberOfCapacitors;
	G6 = 0;
	G7 = 0;
	for (i = 1; i <= NumberOfVCCS; ++i)
	{
		pBM[j].SetType(-m_pGm[i].GetType());
		pBM[j].SetNumber(i);
		pBM[j].SetNode1(m_pGm[i].GetRecP());
		pBM[j].SetNode2(m_pGm[i].GetRecN());
		pBM[j].SetValue(m_pGm[i].GetValue());
		switch (m_pGm[i].GetType())
		{
		case 1:		//gM
			break;
		case 2:		//gM/s
			G6++;
			break;
		case 3:		//gM*s
			G7++;
			break;
		default:
			printf("%s\n", "Error:Unexpected VCCS type");
			break;
		}
	}
	//----------------------------------
	//Parrallel Branch List
	//---------------------------------
	printf("Parrallel Branch List\n");
	NumberOfReactiveComponents =  NumberOfCapacitors +
								NumberOfInductors +
								G6 + G7;
	int loop;
	for (i = 1; i <= TotalComponents - 1; i++)
	{
		//		printf("Parrallel Branch List Iteration:i=%d\n", i);
		if (pBM[i].GetParBranch() == 0)	//Line 1080 <= ?
		{
			loop = 1;
			for (j = i + 1; j <= TotalComponents && loop; j++)	//line 1090
			{
				if (pBM[j].GetParBranch() == 0)	//Line 1100
				{
					if (pBM[j].GetNode1() == pBM[i].GetNode1())	//line 1110
					{
						if (pBM[j].GetNode2() == pBM[i].GetNode2()) //line 1120
						{
							pBM[i].SetParBranch(j);		// Line 1130
							pBM[j].SetParBranch(i);		// Line 1140
//							printf("Break\n");
							loop = 0;
						}
					}
					else
					{
						if (pBM[i].GetNode1() == pBM[j].GetNode2())	// Line 1160
						{
							if (pBM[i].GetNode2() == pBM[j].GetNode1())	// Line 1070
							{
								pBM[i].SetParBranch(j);
								pBM[j].SetParBranch(i);
							}
						}
					}
				}
			}

		}
	}
	pBM->Print(stdout, "BM");
	//-----------------------------------------
	//	Fill in Node Branch Matrix
	// (*BranchMatrix::NodeBranchMatrix) is the
	// Same as V in the Basic Program
	//------------------------------------------
	printf("Fill in Node Branch Matrix\n");
	T8 = 2;	// Line 1210
	W = new int[NumberOfNodes + 2];	// Line 1220
//	printf("W[n] n=%d\n",nN + 2);
	for (i = 1; i <= NumberOfNodes + 1; ++i)		//???
	{
		//		printf("Iteration:i=%d\n", i);
		W[i] = 0;
		for (j = 1; j <= TotalComponents; ++j)	// Line 1250
		{
			if (pBM[j].GetNode1() == i - 1)
				W[i]++;
			else if (pBM[j].GetNode2() == i - 1)
				W[i]++;
			//			printf("BM:W[%d]=%d\n", i, BranchMatrix::W[i]);
		}
		if (W[i] > T8 - 2)
			T8 = W[i] + 2;	// Line 1310
	}	// Line 1320
	NodeBranchMatrix = new CIntMatrix(T8 + 1, NumberOfNodes + 1);	//line 1330
	for (i = 0; i <= NumberOfNodes; ++i)	// Line 1340
	{
		//		printf("Iteration:i:%d i+1:%d T8-1:%d\n", i, i + 1, T8 - 1);
		(*NodeBranchMatrix)(T8 - 1, i) = W[i + 1];
		(*NodeBranchMatrix)(0, i) = 0;
		(*NodeBranchMatrix)(T8, i) = 0;
	}		// Line 1380
	for (i = 1; i <= NumberOfNodes; ++i)	//line 1390
	{
		Node1 = pBM[i].GetNode1();
		Node2 = pBM[i].GetNode2();
		(*NodeBranchMatrix)(T8, Node1) = (*NodeBranchMatrix)(T8, Node1) + 1;
		(*NodeBranchMatrix)(T8, Node2) = (*NodeBranchMatrix)(T8, Node2) + 1;
		(*NodeBranchMatrix)((*NodeBranchMatrix)(T8, Node1), Node1) = i;
		(*NodeBranchMatrix)((*NodeBranchMatrix)(T8, Node2), Node2) = i;
	}	// line 1460
	//----------------------------------------------------------
	// Rearange Branch Order in BranchMatrix::NodeBranchMatrix
	//-----------------------------------------------------------
	printf("Rearange Branch Order in Node Branch Matrix\n");
	int R8;
	R8 = 0;

	//	printf("T8 = %d LoopLimit=%d\n", T8, (*BranchMatrix::NodeBranchMatrix)(T8 - 1, 0));
	for (j = 0; j <= (*NodeBranchMatrix)(T8 - 1, 0); ++j)	// Line 1490
	{
		Node1 = pBM[(*NodeBranchMatrix)(j, 0)].GetNode1();	//Line 1500 V1 = U[V[S9,0],2]
		Node2 = pBM[(*NodeBranchMatrix)(j, 0)].GetNode2();	//Line 1510 V1 = U[V[S9,0],3]
//		printf("Node1=%d  Node2=%d\n", Node1, Node2);
		if (Node1 == 0) Node1 = Node2;
		for (i = 1; i <= (*NodeBranchMatrix)(T8 - 1, Node1); ++i)
		{
			if ((*NodeBranchMatrix)(i, Node1) == (*NodeBranchMatrix)(j, 0))
			{
				if ((*NodeBranchMatrix)(0, Node1) <= 0)
				{
					(*NodeBranchMatrix)(0, Node1) = i;
					R8++;	// Line 1570
//					printf("Increment R8\n");
				}
			}

		}	//Line 1580
	}	// Line 1590
	//--------------------------------------------
	// The folling code needed to be
	// redone due to the fact that the
	// Basic Version was very badly
	// writen
	//----------------------------------------------
//	printf("Nasty for loops\n");
	do {
		int i;
		int x;
		loop = 0;
		//		printf("Number of nodes %d  T8 = %d\n", nN,T8);
		for (i = 1, x = 1; (i <= NumberOfNodes) && x; ++i)	//Line 1600
		{
			if ((*NodeBranchMatrix)(0, i) != 0)	//line 1610
			{
				if ((*NodeBranchMatrix)(T8 - 1, i) != 1)	//line 1620
				{
					x = SubJLoop(i, R8);	// line 1630
					if (x == 0)
						loop = 1;
				}
			}
		}	//Line 1770
	} while (loop);
	//	printf("R8=%d  nN=%d\n", R8, nN);
	if (R8 != NumberOfNodes)
	{
		printf("Could Not find a Node Branch Matrix\n");
		printf("The original Basic Version allowed\n");
		printf("To manually Enter a new Node Matrix\n");
		printf("They could not be serious!\n");
		printf("Terminating....\n");
		exit(1);
	}
	for (i = 1; i <= NumberOfNodes; ++i)	// Line 1860
	{
		if ((*NodeBranchMatrix)(0, i) != 1)
		{
			Node1 = (*NodeBranchMatrix)(1, i);
			Node2 = (*NodeBranchMatrix)((*NodeBranchMatrix)(0, i), i);
			(*NodeBranchMatrix)((*NodeBranchMatrix)(0, i), i) = Node1;
			(*NodeBranchMatrix)(1, i) = Node2;
			(*NodeBranchMatrix)(0, i) = 1;	// Line 1920
		}	// Line 1930
	}
	(*NodeBranchMatrix)(0, InputNode) = 0;	// Line 1940
	pBM->Print(stdout, "U");
	return;	// Line 1950
}
int CTopsenMain::SubJLoop(int i, int& R8)
{
	int j;
	int Node1, Node2;

	//	printf(" J sub loop LINE=1630 i=%d T8=%d R8=%d\n",i,T8,R8);
	//	printf(" J Loop Iterations:%d\n", (*BranchMatrix::NodeBranchMatrix)(T8 - 1, i));
	for (j = 1; j <= (*NodeBranchMatrix)(T8 - 1, i); ++j)
	{
		//		printf("   Iteration:%d\n",j);
		//		printf(" V DIM [%d,%d]\n", (*BranchMatrix::NodeBranchMatrix).GetRows(), (*BranchMatrix::NodeBranchMatrix).GetCols());
		if ((*NodeBranchMatrix)(0, i) != j)	// line 1640
		{
			//			printf("Line 1650 j = %d  i = %d\n",j,i);
			Node1 = pBM[(*NodeBranchMatrix)(j, i)].GetNode1();
			Node2 = pBM[(*NodeBranchMatrix)(j, i)].GetNode2();
			//			printf("   Line 1660  Node1:%d  Node2:%d", Node1, Node2);
			if (Node1 == i) Node1 = Node2;
			if (Node1 != 0)
			{
				//				printf("Line 1680\n");
				if ((*NodeBranchMatrix)(0, Node1) <= 0)
				{
					SubKLoop(Node1, i, j);
					//					printf("increment R8=%d->%d\n", R8,R8+1);
					R8++;
					//					printf("Line 1740\n");
					return 0;	//break from for loop (i)
				}
			}
		}
	}	// Line 1760

	return 1;	// keep looping for (i)
}

int CTopsenMain::SubKLoop(int Node1, int i, int j)
{
	int k;

	//	printf("K sub loop\n");
	for (k = 1; k <= (*NodeBranchMatrix)(T8 - 1, Node1); ++k)
	{
		if ((*NodeBranchMatrix)(k, Node1) == (*NodeBranchMatrix)(j, i))
			(*NodeBranchMatrix)(0, Node1) = k;
	}
	return 0;
}

void CTopsenMain::GenNextTree()			// Line 3000
{

}

void CTopsenMain::LoopTest()			//Line 4000
{
	int i,j;
	int V1;
	int loop;
	int Node1, Node2;
	CIntMatrix* pV = NodeBranchMatrix;	// a lot easir to type
	T4++;		// Line 4010
	for (i = 1; i <= NumberOfNodes; ++i)		// Line 4020
	{
		(*pV)(T8, i) = 0;	// line 4030
		W[i + 1] = 0;					//line 4040
	}
	V1 = 1;	// line 4060
	switch ((*pV)(0, 0))
	{
	case 2:		//gptp ;ome 4430
		break;
	case 1:
		if (InputNodeMirror == 1)
			V1 = 2;		// Line 4080
		(*pV)(T8, V1) = 1;	// Line 4090
		W[1] = pBM[(*pV)((*pV)(0,V1),V1)].GetNode1();	// Line 4100
		if (W[1] == InputNodeMirror)
			W[1] = 0;			// Line 4110
		W[2] = pBM[(*pV)((*pV)(0, V1), V1)].GetNode2();
		if (W[2] == InputNodeMirror)
			W[2] = 0;	// Line 4130
		V1 = 1;			// Line 4140
		while (loop)
		{
			V1 += 1;	//Line 4150
			for (i = 1; i <= NumberOfNodes; ++i)
			{
				if (i != InputNodeMirror)	// Line 4170
				{
					if ((*pV)(T8, i) != 1)	// Line 4180
					{
						Node1 = pBM[(*pV)((*pV)(0, i), )].GetNode1();	// Line 4190
						Node2 = pBM[(*pV)((*pV)(0, i), i)].GetNode2();	// Line 4200
						if (Node1 == InputNodeMirror)
							Node1 = 0;	// Line 4210
						if (Node2 == InputNodeMirror)
							Node2 = 0;	//Line 4220
					}
				}

			}
		}
		break;
	default:
		fprintf(stderr, "Hit Default in Basic Line 4070  V(0,0)=%d\n", (*NodeBranchMatrix)(0, 0));
		exit(1);
		break;
	}
}

void CTopsenMain::ActiveLoopTest()		//Line 5000
{

}

void CTopsenMain::UpdateTransferFunction()	//Line 7000
{
	int i;
	int S1;
	double J1 = 1.0;

	for (i = 1; i <= NumberOfResistors + NumberOfInductors; ++i)
		pBM[i].SetScratch(1.0);	//Line 7030
	for (i = NumberOfResistors + NumberOfInductors + 1; i <= TotalComponents; ++i)
		pBM[i].SetScratch(0.0);	//Line 7060
	S1 = NumberOfInductors + G6;	//Line 7080
	for (i = 1; i <= NumberOfNodes; ++i)
		pBM[(*NodeBranchMatrix)((*NodeBranchMatrix)(0,i),i)].SetScratch(1.0 - pBM[(*NodeBranchMatrix)((*NodeBranchMatrix)(0, i), i)].GetScratch	());	//Line 7100
	for (i = 1; i <= TotalComponents; ++i)	//Line 7120
	{
		if (pBM[i].GetScratch() != 0.0)
		{
			J1 = J1 * pBM[i].GetValue();
			if (abs(pBM[i].GetType()) == COMP_C_TYPE)
				S1++;
			if (pBM[i].GetType() == -GM_S)
				S1--;
		}
		else if (pBM[i].GetType() == COMP_L_TYPE)	//Line 7180
			S1--;
	}
	if ((*NodeBranchMatrix)(0, 0) != 5)		// Line 7200
		pTransferFunction[S1].SetDenominator(pTransferFunction[S1].GetDenominator() + P1 * J1);	// Line 7210
	switch ((*NodeBranchMatrix)(0, 0) - 1)	//Line 7220
	{
	case 2:
	case 4:
	case 5:
		pTransferFunction[S1].SetNumerator(pTransferFunction[S1].GetNumerator() + Q1 * J1 * MinusOneTo(InputNode + OutputNode));	//Line 7230
	case 1:
	case 3:
		if (CmdPre == PREMENU_TRANSFERFUNCTION) return;	//Line 7240
		break;
	default:
		printf("Invalid Case #1 in UpdateTransferFunction\n");
		exit(1);
		break;
	}
	for (i = 1; i <= TotalComponents; ++i)
	{
		if (pBM[i].GetScratch() != 0.0)
		{
			if ((*NodeBranchMatrix)(0, 0) != 5)
				(*pF)(i, S1) += P1 * J1 / pBM[i].GetValue();
			switch ((*NodeBranchMatrix)(0, 0) - 1)	//Line 7290
			{
			case 1:
			case 3:
			case 5:
				break;
			case 2:
			case 4:
				(*pE)(i, S1) += Q1 * J1 * MinusOneTo(InputNode + OutputNode) / pBM[i].GetValue();	//Line 7300
				break;
			default:
				printf("Invalid Case #2 in UpdateTransferFunction\n");
				exit(1);
				break;
			}
		}
	}	// Line 7310
	return;	//Line 7320
}
