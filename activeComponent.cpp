#include <stdio.h>
#include <stdlib.h>
#include "Consol.h"
#include "Topsen.h"
#include "TopsenMain.h"
#include "activeComponent.h"

static const char* typeStrings[] = {
    "----",
    "  gM",
    "gM/S",
    " SgM"
};

activeComponent::activeComponent()
{
    mValue = 0.0;
    mSend_P = 0;
    mSend_N = 0;
    mRec_N = 0;
    mRec_P = 0;
    mType = 0;
    mNumber = 0;
}

activeComponent::~activeComponent()
{

}

void activeComponent::Print()
{
    printf("%s(%d):%10.6lf %d %d %d %d\n", typeStrings[GetType()], GetNumber(), GetValue(), GetSendP(), GetSendN(), GetRecP(), GetRecN());
}

void activeComponent::Input()
{
	char* s = new char[256];
	pConsol->InputString(" Type:1-gM 2-gM/s 3-sgM:", s);
	SetType(atoi(s));
	pConsol->InputString(" Value (gM):", s, 0);
	SetValue(atof(s));
	pConsol->InputString(" Send Node+:", s, 0);
	SetSendP(atoi(s));
	pConsol->InputString(" Send Node-:", s, 0);
	SetSendN(atoi(s));
	pConsol->InputString(" Rec Node +:", s, 0);
	SetRecP(atoi(s));
	pConsol->InputString(" Rec Node -:", s, 1);
	SetRecN(atoi(s));
	delete[]s;
}
