#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
	"Gm",
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

	l = sprintf_s(s, 1024,     "OUTPUTS:%s %s, %s %s\n", Cmds[0], CmdComment[0], Cmds[1], CmdComment[1]);
	l += sprintf_s(&s[l], 1024, "       :%s %s\n", Cmds[2], CmdComment[2]);
	return l;
}

int CTopsenMain::DoMenu(int MenuID, const char** Cmds)
{
	char* Menu = new char[1024];
	char* UsrIn = new char[256];
	int rv = 0;
	int l,i;

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
			rv = i+1;
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

void CTopsenMain::InputComponents(const char *prompt, int n, passiveComponent* parts)
{
	char* p = new char[256];
	printf("%s\n", prompt);
	int i;
	int PartNum;

	for (i = 1; i <= n; ++i)
	{
		sprintf_s(p, 256, "%s(?)", parts->GetTypeString(parts->GetType()));
		PartNum = pConsol->InputINT(p,0);
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
	int nR,int nL,int nC,int nG)
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
	cmd =  DoMenu(0, Cmds);
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
	return cmd;
}

void CTopsenMain::Run()
{	
	PrintTitle();	//Line 10
	pConsol = new CConsol;
	InputDetails();	//Line 140
	m_pRes = CreateComponents(NumberOfResistors,COMP_R_TYPE);	//line 180
	m_pInd = CreateComponents(NumberOfInductors,COMP_L_TYPE);
	m_pCap = CreateComponents(NumberOfCapacitors,COMP_C_TYPE);
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
	NumberOfPassives =	NumberOfResistors +
						NumberOfInductors +
						NumberOfCapacitors;
	TotalComponents = NumberOfPassives + NumberOfVCCS;
	pBM = new BranchMatrix[TotalComponents + 1];
	pBM->SetNumbeerOfElements(TotalComponents);

	pBM->Fill(NumberOfNodes, NumberOfPassives,
		TotalComponents, NumberOfResistors,
		NumberOfCapacitors,
		NumberOfInductors,
		NumberOfVCCS,
		m_pRes, m_pCap, m_pInd, m_pGm,
		InputNode, OutputNode);
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

