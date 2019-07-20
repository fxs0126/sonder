#ifndef _SONDER_COMMON_THREAD_H_
#define _SONDER_COMMON_THREAD_H_

#ifndef WIN32
        #define SC_THREAD_API
#else
        #ifndef  SC_THREAD_API
                #define SC_THREAD_API __declspec(dllexport)
                #pragma warning (disable: 4251)
        #else
                #define SC_THREAD_API __declspec(dllimport)
                #pragma warning (disable: 4251)
        #endif
#endif
#ifndef SONDER_COMMON_BEGIN
#define SONDER_COMMON_BEGIN namespace sc {
#endif // !SONDER_COMMON_BEGIN
#ifndef SONDER_COMMON_END 
#define SONDER_COMMON_END }
#endif // !SONDER_COMMON_END 

#ifndef COMMON_THREAD_BEGIN
#define COMMON_THREAD_BEGIN namespace thread {
#endif // !COMMON_THREAD_BEGIN
#ifndef  COMMON_THREAD_END
#define COMMON_THREAD_END }
#endif // ! COMMON_THREAD_END


SONDER_COMMON_BEGIN
COMMON_THREAD_BEGIN
#ifdef __cplusplus
extern "C" {
#endif

class SC_THREAD_API SCThread
{
public:
	SCThread();
	virtual ~SCThread();
	bool start();
	bool wait();
	virtual void run() = 0;
private:
#ifdef WIN32
	unsigned int _id;
#else
    pthread_t _id;
#endif
};

#ifdef __cplusplus
}
#endif

COMMON_THREAD_END
SONDER_COMMON_END


#endif // !_SONDER_COMMON_THREAD_H_
