#include "../include/sc_thread.h"

#ifdef WIN32
#include <io.h>
#include <Shlwapi.h>
#include <process.h>
#pragma comment(lib, "shlwapi")
#pragma warning(disable:4996)
#else
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <semaphore.h>
#endif

SONDER_COMMON_BEGIN
COMMON_THREAD_BEGIN

#ifdef WIN32
static unsigned int WINAPI ThreadProc(LPVOID lpParam);
#else
static void* ThreadProc(void pParam);
#endif

#ifdef WIN32
unsigned int WINAPI ThreadProc(LPVOID lpParam)
{
    SCThread* pThread = static_cast<SCThread*>(lpParam);
    pThread->run();
    _endthreadex(0);
    return 0;
}
#else
void* ThreadProc(void* pParam)
{
    SCThread* pThread = static_cast<SCThread*>(lpParam);
    pThread->run();
    _endthreadex(0);
}
#endif

SCThread::SCThread()
    :_id(0)
{
}
SCThread::~SCThread()
{

}

bool SCThread::start()
{
#ifdef WIN32
    unsigned long long ret = _beginthreadex(NULL, 0, ThreadProc, static_cast<void*>(this), 0, NULL);
    if (-1 == ret || 1 == ret || 0 == ret )
    {
        return false;
    }
    _id = static_cast<unsigned> (ret);
#else
    int ret = pthread_create(&_id, NULL, ThreadProc, static_cast<void*>(this));
    if (0 != ret )
    {
        return false;
    }
#endif

	return true;
}

bool SCThread::wait()
{
#ifdef WIN32
    if (WaitForSingleObject(reinterpret_cast<HANDLE>(_id), INFINITE) != WAIT_OBJECT_0 )
    {
        return false;
    }
#else
    if (pthread_join(_id, NULL) != 0 )
        return false;
#endif
	return true;
}

COMMON_THREAD_END
SONDER_COMMON_END
