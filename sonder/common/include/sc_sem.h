
#ifndef _SONDER_COMMON_SEM_H_
#define _SONDER_COMMON_SEM_H_

#include "sc_common.h"
#ifndef WIN32
        #define SC_SEM_API
#else
        #ifndef  SC_SEM_API
                #define SC_SEM_API __declspec(dllexport)
                #pragma warning (disable: 4251)
        #else
                #define SC_SEM_API __declspec(dllimport)
                #pragma warning (disable: 4251)
        #endif
#endif
#ifdef WIN32
#include <io.h>
#include <shlwapi.h>
#include <process.h>
#pragma comment(lib, "shlwapi")
#pragma warning(disable:4996)
#else
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<pthread.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <semaphore.h>
#endif


#ifndef SONDER_COMMON_BEGIN
#define SONDER_COMMON_BEGIN namespace sc {
#endif
#ifndef SONDER_COMMON_END
#define SONDER_COMMON_END }
#endif

#ifndef COMMON_SEM_BEGIN
#define COMMON_SEM_BEGIN namespace sem {
#endif
#ifndef COMMON_SEM_END
#define COMMON_SEM_END }
#endif

SONDER_COMMON_BEGIN
COMMON_SEM_BEGIN
#ifdef __cplusplus
extern "C" {
#endif
class SC_SEM_API SCSem
{
public:
    SCSem();
    virtual ~SCSem();
    bool create(int);
    bool wait(int timeout = 0);
    bool post();
protected:
private:
#ifdef WIN32
    HANDLE _hSem;
#else
    sem_t _hSem;
    bool  _bCreate;
#endif
};
class SC_SEM_API SCLock
{
public:
	SCLock();
	virtual ~SCLock();
	void lock();
	void unlock();
private:
#ifdef  WIN32
	CRITICAL_SECTION _locker;
#else 
	pthread_mutex_t  _locker;
#endif //  WIN32

};
class SC_SEM_API SCAutoLock
{
public:
	explicit SCAutoLock(SCLock& lock)
		:_locker(lock)
	{
		_locker.lock();
	}
	~SCAutoLock()
	{
		_locker.unlock();
	}
private:
	SCLock& _locker;
};

#ifdef __cplusplus
}
#endif

COMMON_SEM_END
SONDER_COMMON_END

#endif //!_SONDER_COMMON_SEM_H_
