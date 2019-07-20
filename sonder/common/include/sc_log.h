#ifndef _SONDER_COMMON_LOG_H_
#define _SONDER_COMMON_LOG_H_ 1

#include "sc_common.h"

#include "sc_thread.h"
#include "sc_sem.h"

using namespace sc::thread;
using namespace sc::sem;

#ifndef WIN32
        #define SC_LOG_API
#else
        #ifndef  SC_LOG_API
                #define SC_LOG_API __declspec(dllexport)
                #pragma warning (disable: 4251)
        #else
                #define SC_LOG_API __declspec(dllimport)
                #pragma warning (disable: 4251)
        #endif
#endif

typedef unsigned LoggerID;
// log level 
#ifndef SONDER_LOG_LEVEL_INFO
#define SONDER_LOG_LEVEL_INFO	0x01
#endif // !SONDER_LOG_LEVEL_INFO

#ifndef SONDER_LOG_LEVEL_WARN
#define SONDER_LOG_LEVEL_WARN	0x02
#endif // !SONDER_LOG_LEVEL_WARN

#ifndef SONDER_LOG_LEVEL_DEBUG
#define SONDER_LOG_LEVEL_DEBUG	0x04
#endif // !SONDER_LOG_LEVEL_DEBUG

#ifndef SONDER_LOG_LEVEL_ASSERT
#define SONDER_LOG_LEVEL_ASSERT 0x08
#endif // !SONDER_LOG_LEVEL_ASSERT

#ifndef SONDER_LOG_LEVEL_ERROR
#define SONDER_LOG_LEVEL_ERROR  0x10
#endif // !SONDER_LOG_LEVEL_ERROR

#ifndef SONDER_LOG_LEVEL_FATAL
#define SONDER_LOG_LEVEL_FATAL	0x20 
#endif // !SONDER_LOG_LEVEL_FATAL

#define SONDER_MAIN_LOGGER_ID  0

//! all log synchronous display to the screen or not
#ifndef SONDER_ALL_SYNCHRONOUS_DISPLAY
#define SONDER_ALL_SYNCHRONOUS_DISPLAY true
#endif // !SONDER_ALL_SYNCHRONOUS_DISPLAY

//! all log write log to file or not
#ifndef SONDER_ALL_WRITE_TO_FILE
#define SONDER_ALL_WRITE_TO_FILE true
#endif // !SONDER_ALL_WRITE_TO_FILE

#ifndef  SONDER_LOG_DEFAULT_PATH
#define SONDER_LOG_DEFAULT_PATH "E:/log/"
#endif // ! SONDER_LOG_DEFAULT_PATH

#ifndef SONDER_DEFAULT_MONTH_DIR
#define SONDER_DEFAULT_MONTH_DIR true 
#endif // !SONDER_DEFAULT_MONTH_DIR

#ifndef  SONDER_LOG_DEFAULT_LIMIT_SIZE
#define SONDER_LOG_DEFAULT_LIMIT_SIZE 1
#endif // ! SONDER_LOG_DEFAULT_LIMIT_SIZE

#ifndef  SONDER_LOG_DEFAULT_MAX_NUM
#define  SONDER_LOG_DEFAULT_MAX_NUM 1024
#endif // !

#ifndef SONDER_COMMON_BEGIN
#define SONDER_COMMON_BEGIN namespace sc {
#endif // !SONDER_COMMON_BEGIN

#ifndef SONDER_COMMON_END
#define SONDER_COMMON_END }
#endif // !SONDER_COMMON_END

#ifndef COMMON_LOG_BEGIN
#define COMMON_LOG_BEGIN namespace log {
#endif

#ifndef COMMON_LOG_END
#define COMMON_LOG_END }
#endif // !COMMON_LOG_END


SONDER_COMMON_BEGIN
COMMON_LOG_BEGIN
#ifdef __cplusplus
extern "C" {
#endif

class SCLogDataImp;

class SC_LOG_API SCLogManager:public SCThread
{
public:
	~SCLogManager();
    static SCLogManager* Instance();
    // input log file name, output log id
    unsigned createlog(std::string& logFile);
    unsigned createlog(const char* logfile);
	void message(const unsigned& logid,const unsigned& loglevel,const char* content);
    void setConsoleShow(const unsigned& logid, bool bShow = false);
    void setFileLogShow(const unsigned& logid, bool bShow = false);
    void setLogLevelEnable(const unsigned& logid, unsigned level , bool bShow=false);
	void run();
	void stop();
private:
	SCLogManager();
	class CAutoRelease
	{
	public:
        CAutoRelease() {}
        ~CAutoRelease()
        {
            if (_pSCLogMgrInstance)
                delete _pSCLogMgrInstance;
            _pSCLogMgrInstance = NULL;
        }
	private:
	};


	static SCLogManager* _pSCLogMgrInstance;
	static CAutoRelease  _autoRelease;

	SCLock				 _ImpLock;
    SCLogDataImp*        _pImp;
	char				_bRun  : 2,
						_bRecv : 6;
};
#ifdef __cplusplus
}
#endif
COMMON_LOG_END
SONDER_COMMON_END


#endif // !_SONDER_LOG_H_
