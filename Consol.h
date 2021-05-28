#pragma once
class CConsol
{
public:
	CConsol();
	~CConsol();
	int InputINT(const char* Prompt, int crFlag = 0);
	int GetAchr(void);
	int InputString(const char* Prompt, char* out, int crFlag = 0);
};

