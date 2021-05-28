#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "Consol.h"

CConsol::CConsol()
{

}

CConsol::~CConsol()
{

}

int CConsol::GetAchr(void)
{
	int a;
	while (0 == _kbhit());
	a = _getch();
	return a;
}

int CConsol::InputString(const char* Prompt, char* out, int crFlag)
{
	int len = 0;
	int loop = 1;
	int ch;
	int index;
	const char* s = Prompt;
	index = 0;
	while (ch = *s++)
	{
		_putch(ch);
	}
	while (loop)
	{
		ch = GetAchr();
		switch (ch)
		{
		case '\t':
		case '\r':
			out[index] = 0;
			if (crFlag)
			{
				_putch('\r');
				_putch('\n');
			}
			loop = 0;
			break;
		case '\b':
			index--;
			_putch('\b');
			_putch(' ');
			_putch('\b');
			break;
		default:
			out[index++] = ch;
			++len;
			_putch(ch);
			break;
		}
	}

	return len;
}

int CConsol::InputINT(const char* prompt, int crFlag)
{
	char s[256];
	InputString(prompt, s, crFlag);
	return(atoi(s));
}
