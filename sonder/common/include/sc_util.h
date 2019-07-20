#ifndef SONDER_COMMON_UTIL_H_
#define SONDER_COMMON_UTIL_H_
#include "sc_common.h"
#pragma comment(lib,"shlwapi.lib") //加入链接库
#ifndef WIN32
        #define SC_UTIL_API
#else
        #ifndef  SC_UTIL_API
                #define SC_UTIL_API __declspec(dllexport)
                #pragma warning (disable: 4251)
        #else
                #define SC_UTIL_API __declspec(dllimport)
                #pragma warning (disable: 4251)
        #endif
#endif
#ifndef SONDER_COMMON_BEGIN
#define SONDER_COMMON_BEGIN namespace sc {
#endif

#ifndef SONDER_COMMON_END
#define SONDER_COMMON_END }
#endif

#ifndef COMMON_UTIL_BEGIN
#define COMMON_UTIL_BEGIN namespace util {
#endif
#ifndef COMMON_UTIL_END
#define COMMON_UTIL_END }
#endif

SONDER_COMMON_BEGIN
COMMON_UTIL_BEGIN
#ifdef __cplusplus
extern "C" {
#endif

SC_UTIL_API /*static*/ void sc_sleep(unsigned int ms);
SC_UTIL_API /*static*/ bool sc_time2tm(const time_t & t , tm* tt);
SC_UTIL_API /*static*/ bool sc_is_same_day(const time_t& t1, const time_t& t2);
SC_UTIL_API /*static*/ bool sc_is_same_month(const time_t& t1, const time_t& t2);
SC_UTIL_API /*static*/ bool sc_is_same_year(const time_t& t1, const time_t& t2);
SC_UTIL_API /*static*/ time_t sc_get_time();

SC_UTIL_API /*static*/ void sc_tolower(char* pStr);
SC_UTIL_API /*static*/ void sc_toupper(char* pStr);

SC_UTIL_API /*static*/ bool sc_is_path_dir(const char* pStrPath);
SC_UTIL_API /*static*/ void sc_path_dir(std::string& path);
SC_UTIL_API /*static*/ bool sc_create_dir(const char* pStrPath);
#ifdef __cplusplus
}
#endif

COMMON_UTIL_END
SONDER_COMMON_END
#endif
