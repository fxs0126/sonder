#include "sc_console.h"
#ifdef __cplusplus
#include <iostream>
using namespace std;
#else 
#include <stdlib.h>
#endif // __cplusplus

#ifdef WIN32
#include <Windows.h>
#endif // WIN32

SONDER_COMMON_BEGIN
COMMON_CONSOLE_BEGIN

CConsole* CConsole::_pInstance = 0;
CConsole::CAutoRelease CConsole::_autoRelease;

CConsole::CConsole()
{
#ifdef WIN32
	hStdOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdOutHandle, &csbiInfo);
	wOldColorAttrs = csbiInfo.wAttributes;
#endif // WIN32
}

CConsole* CConsole::instance()
{
    if( !_pInstance )
        _pInstance = new CConsole();
    return _pInstance;
}
CConsole::~CConsole()
{

}

void CConsole::setcolor(int textColor, int backColor)
{
#ifdef WIN32
	SetConsoleTextAttribute(hStdOutHandle, textColor | backColor);
#endif // WIN32
}
void CConsole::print(const char* messsage)
{
#ifdef __cplusplus
	std::cout << messsage;
#else
	printf(messsage);
#endif // __cplusplus

}

void CConsole::reset()
{
#ifdef WIN32
	SetConsoleTextAttribute(hStdOutHandle, wOldColorAttrs);
#endif // WIN32
}
void CConsole::settitle(const char* title)
{
#ifdef WIN32
	SetConsoleTitleA(title);
#endif // WIN32

}
CConsole::CAutoRelease::CAutoRelease()
{

}
CConsole::CAutoRelease::~CAutoRelease()
{
    delete CConsole::_pInstance;
}
COMMON_CONSOLE_END
SONDER_COMMON_END
