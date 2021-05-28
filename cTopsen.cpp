// cTopsen.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#include "Topsen.h"
#include "TopsenMain.h"
#include "passiveComponet.h"
#include "activeComponent.h"
#include "BranchMatrix.h"
#include "IntMatrix.h"
#include "doubleMatrix.h"


int main()
{
	CTopsenMain* pTop;
	/*
	int i, j,v;
	CIntMatrix* testMatrix = new CIntMatrix(4, 6);
	CdoubleMatrix* doubleMat = new CdoubleMatrix(3, 7);
	for (j = 0; j < 3; ++j)
		for (i = 0; i < 7; ++i)
			(*doubleMat)(j, i) = (double)i +(double)j * 7.0;
	v = 0;
	for (i = 0; i < 4; ++i)
		for (j = 0; j < 6; ++j)
			(*testMatrix)(i, j) = v++;
	testMatrix->Print(stdout);
	doubleMat->Print(stdout);
	exit(0);
	*/


/*
	NumberOfPassives = NumberOfCapacitors + NumberOfInductors + NumberOfResistors;
	TotalComponents = NumberOfVCCS + NumberOfPassives;
	BranchMatrix* Branches = new BranchMatrix[TotalComponents + 1];
	Branches->SetNumbeerOfElements(TotalComponents);

	int InputNode, OutputNode;
	InputNode = InputINT("Input Node: ");
	OutputNode = InputINT("Output Node: ");

	Branches->Fill(Branches,
					NumberOfNodes,
					NumberOfPassives,
					TotalComponents,
					NumberOfResistors,
					NumberOfCapacitors,
					NumberOfInductors,
					NumberOfVCCS,
					Resistors,
					Capacitors,
					Inductors,
					gM,
					InputNode,
					OutputNode
		);
		*/
	pTop = new CTopsenMain();
	pTop->Run();
	printf("%s\n","All Done\n");
}

