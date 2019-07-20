#include "../include/sc_sem.h"

SONDER_COMMON_BEGIN
COMMON_SEM_BEGIN

SCSem::SCSem()
{
#ifdef WIN32
    _hSem = NULL;
#else
    _bCreate = false;
    _hSem = 0;
#endif
}

SCSem::~SCSem()
{

}
bool SCSem::create(int count)
{
    if (count < 0 || count > 64 ) count = 0;
#ifdef WIN32
    _hSem =CreateSemaphore(NULL, count, 64, NULL);
    if (NULL == _hSem )
        return false;
#else
    if (sem_init(&_hSem, 0 , count) != 0 )
    {
        return false;
    }
    _bCreate = true;
#endif
    return true;
}

bool SCSem::wait(int timeout /*= 0*/)
{
#ifdef WIN32
    if ( timeout <=0 ) timeout = INFINITE;
    if ( WaitForSingleObject(_hSem, timeout) != WAIT_OBJECT_0 )
    {
        return false;
    }
#else
    if (timeout <=0 )
        return sem_wait(&_hSem) == 0 ? true: false ;
    else
    {
        timespec ts;
        ts.tv_sec = time(NULL) + timeout/1000;
        ts.tv_nsec = (timeout%1000)*1000000;
        return sem_timedwait(&m_semid, &ts) == 0 ? true : false;
    }
#endif
    return true;
}

bool SCSem::post()
{
#ifdef WIN32
    return ReleaseSemaphore(_hSem, 1, NULL) ? true : false;
#else
    return (sem_post(&_hSem) == 0 ? true : false );
#endif
}


SCLock::SCLock()
{
#ifdef WIN32
	InitializeCriticalSection(&_locker);
#else
	//m_crit = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&_locker, &attr);
	pthread_mutexattr_destroy(&attr);
#endif
}
SCLock::~SCLock()
{
#ifdef WIN32
	DeleteCriticalSection(&_locker);
#else
	pthread_mutex_destroy(&_locker);
#endif
}

void SCLock::lock()
{
#ifdef WIN32
	EnterCriticalSection(&_locker);
#else
	pthread_mutex_lock(&_locker);
#endif
}
void SCLock::unlock()
{
#ifdef WIN32
	LeaveCriticalSection(&_locker);
#else
	pthread_mutex_unlock(&_locker);
#endif
}

COMMON_SEM_END
SONDER_COMMON_END
