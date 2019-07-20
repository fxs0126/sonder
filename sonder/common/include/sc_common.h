#ifndef _SONDER_COMMON_H_
#define _SONDER_COMMON_H_

#ifdef WIN32
#include<Windows.h>
#endif

#ifdef __cplusplus
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstddef>
#include <ctime>
#include <cmath>
#else
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <time.h>
#include <math.h>
#endif // __cplusplus

#ifndef WIN32
        #define SC_API
#else
        #ifdef  SC_DLL
                #define SC_API __declspec(dllexport)
                #pragma warning (disable: 4251)
        #else
                #define SC_API __declspec(dllimport)
                #pragma warning (disable: 4251)
        #endif
#endif

#endif
