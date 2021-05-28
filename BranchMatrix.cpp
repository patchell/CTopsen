#include <stdio.h>
#include <stdlib.h>
#include "Topsen.h"
#include "BranchMatrix.h"


BranchMatrix::BranchMatrix()
{
	mComponentNumber = 0;
	mNode1 = 0;
	mNode2 = 0;
	mScratch = 0.0;
	mParrallelBranch = 0;
	mValue = 0.0;
	mType = 0;
	W = 0;
	nElements = 0;
}

BranchMatrix::~BranchMatrix()
{
}



const char* PassiveTypes[] = {
	"  ERROR! ",
	" Resistor",
	" Inductor",
	"Capacitor"
};

const char* ActiveTypes[] = {
	"ERROR!",
	"  gM  ",
	" gM/S ",
	" S gM "
};

const char* TypeString(int type)
{
	const char* s;

	if (type < 0)
	{
		type = -type;
		s = ActiveTypes[type];
	}
	else
		s = PassiveTypes[type];
	return s;
}

void BranchMatrix::Print(FILE* out, const char* title)
{
	int i;
	printf("Number of Elements:%d\n", BranchMatrix::nElements);
	fprintf(out, "\t\tBranch Matrix %s\n", title);
	fprintf(out,"   Type\t\tNumber'\tNode 1\tNode 2\t|| Branch\tValue\n");
	for (i = 1; i <= BranchMatrix::nElements; ++i)
	{
		fprintf(out, "%s\t%d\t%d\t%d\t%d\t%15.6lf\n",
			TypeString(this[i].GetType()),
			this[i].GetNumber(),
			this[i].GetNode1(),
			this[i].GetNode2(),
			this[i].GetParBranch(),
			this[i].GetValue());
	}
}
