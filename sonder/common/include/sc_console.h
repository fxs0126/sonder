#ifndef SONDER_COMMON_CONSOLE_H
#define SONDER_COMMON_CONSOLE_H

#include "sc_common.h"

#ifndef WIN32
        #define SC_CONSOLE_API
#else
        #ifndef  SC_CONSOLE_API
                #define SC_CONSOLE_API __declspec(dllexport)
                #pragma warning (disable: 4251)
        #else
                #define SC_CONSOLE_API __declspec(dllimport)
                #pragma warning (disable: 4251)
        #endif
#endif

#ifndef SONDER_COMMON_BEGIN
#define SONDER_COMMON_BEGIN namespace sc {
#endif

#ifndef SONDER_COMMON_END
#define SONDER_COMMON_END }
#endif

#ifndef COMMON_CONSOLE_BEGIN
#define COMMON_CONSOLE_BEGIN namespace console {
#endif

#ifndef COMMON_CONSOLE_END
#define COMMON_CONSOLE_END }
#endif

SONDER_COMMON_BEGIN
COMMON_CONSOLE_BEGIN
#ifdef __cplusplus
extern "C" {
#endif

#define CONSOLE_TEXT_BLUE		0x0001 // text color contains blue.
#define CONSOLE_TEXT_GREEN		0x0002 // text color contains green.
#define CONSOLE_TEXT_RED		0x0004 // text color contains red.
#define CONSOLE_TEXT_INTENSITY  0x0008 // text color is intensified.
#define CONSOLE_BKGRD_BLUE      0x0010 // background color contains blue.
#define CONSOLE_BKGRD_GREEN     0x0020 // background color contains green.
#define CONSOLE_BKGRD_RED       0x0040 // background color contains red.
#define CONSOLE_BKGRD_INTENSITY 0x0080 // background color is intensified.
class SC_CONSOLE_API CConsole
{
public:
    static CConsole* instance();
    virtual ~CConsole();
	void setcolor(int textColor,int backColor);
	void print(const char* messsage);
	void reset();
	void settitle(const char* title);
protected:
private:
    class CAutoRelease
    {
    public:
        CAutoRelease();
        ~CAutoRelease();
    };
    CConsole();
    static CConsole* _pInstance;
    static CAutoRelease _autoRelease;

#ifdef WIN32
	HANDLE  hStdOutHandle;
	WORD    wOldColorAttrs;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
#endif // WIN32

};

#ifdef __cplusplus
}
#endif
COMMON_CONSOLE_END
SONDER_COMMON_END
#endif
