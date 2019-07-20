#ifndef SONDER_COMMON_FILE_H_
#define SONDER_COMMON_FILE_H_

#include "sc_common.h"
#include "sc_sem.h"
using namespace sc::sem;

#ifndef WIN32
        #define SC_FILE_API
#else
        #ifndef  SC_FILE_API
                #define SC_FILE_API __declspec(dllexport)
                #pragma warning (disable: 4251)
        #else
                #define SC_FILE_API __declspec(dllimport)
                #pragma warning (disable: 4251)
        #endif
#endif

#ifndef SONDER_COMMON_BEGIN
#define SONDER_COMMON_BEGIN namespace sc {
#endif

#ifndef SONDER_COMMON_END
#define SONDER_COMMON_END }
#endif

#ifndef COMMON_FILE_BEGIN
#define COMMON_FILE_BEGIN namespace file {
#endif

#ifndef COMMON_FILE_END
#define COMMON_FILE_END }
#endif

SONDER_COMMON_BEGIN
COMMON_FILE_BEGIN
#ifdef __cplusplus
extern "C" {
#endif
class SC_FILE_API SCFile
{
public:
    SCFile();
    virtual ~SCFile();
    bool open(const char* path, const char* mode);
    void close();
    bool isopen();
    unsigned write(const char* buf , size_t len);
    void flush();
    bool readline(char* buf, size_t len);
    void readcontent(char* &pBuf , size_t & len);
	unsigned long long length();
private:
    FILE* _pFile;
	SCLock _locker;
};
#ifdef __cplusplus
}
#endif

COMMON_FILE_END
SONDER_COMMON_END

#endif
