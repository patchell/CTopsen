#include <stdio.h>
#include <stdlib.h>
#include "Topsen.h"
#include "TopsenMain.h"
#include "passiveComponet.h"
#include "Consol.h"

static const char* typeString[] = {
	"?",
	"R",
	"L",
	"C"
};

const char *passiveComponent::GetTypeString(int CompType)
{
	return typeString[CompType];
}

passiveComponent::passiveComponent()
{
	mNode1 = 0;
	mNode2 = 0;
	mNumber = 0;
	mValue = 0.0;
	mType = 0;
}

passiveComponent::~passiveComponent()
{

}

void passiveComponent::Input()
{
	char* s = new char[256];
	pConsol->InputString(" Value:", s, 0);
	SetValue(atof(s));
	pConsol->InputString(" Node1:", s, 0);
	SetNode1(atoi(s));
	pConsol->InputString(" Node2:", s, 1);
	SetNode2(atoi(s));
	delete[] s;
}

void passiveComponent::Print()
{
	printf("%s%d:%10.2lf %d %d\n", typeString[mType], mNumber, GetValue(), GetNode1(), GetNode2());
}
