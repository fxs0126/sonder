#include "sc_util.h"
#ifdef WIN32
#include <Shlwapi.h>
#endif
SONDER_COMMON_BEGIN
COMMON_UTIL_BEGIN

void sc_sleep(unsigned int ms)
{
#ifdef WIN32
    ::Sleep(ms);
#else
    usleep(ms*1000);
#endif
}

bool sc_time2tm(const time_t& t , tm* tt)
{
#ifdef WIN32
#if _MSC_VER < 1400
     tt = localtime(&t);
     return true;
#else
    return localtime_s(tt, &t) == 0 ? true: false;
#endif
#else
    return localtime_s(&t, tt) != NULL ? true : false;
#endif
}

bool sc_is_same_day(const time_t& t1, const time_t& t2)
{
    tm tm1, tm2;
    memset(&tm1, 0, sizeof (tm));
    memset(&tm2, 0, sizeof (tm));
    sc::util::sc_time2tm(t1, &tm1);
    sc::util::sc_time2tm(t2, &tm2);
    if ( tm1.tm_year == tm2.tm_year
         && tm1.tm_yday == tm2.tm_yday)
    {
        return true;
    }
    return false;
}
bool sc_is_same_month(const time_t& t1, const time_t& t2)
{
    tm tm1, tm2;
    memset(&tm1, 0, sizeof (tm));
    memset(&tm2, 0, sizeof (tm));
    sc::util::sc_time2tm(t1, &tm1);
    sc::util::sc_time2tm(t2, &tm2);
    if ( tm1.tm_year == tm2.tm_year
         && tm1.tm_mon == tm2.tm_mon)
    {
        return true;
    }
    return false;
}
bool sc_is_same_year(const time_t& t1, const time_t& t2)
{
    tm tm1, tm2;
    memset(&tm1, 0, sizeof (tm));
    memset(&tm2, 0, sizeof (tm));
    sc::util::sc_time2tm(t1, &tm1);
    sc::util::sc_time2tm(t2, &tm2);
    if ( tm1.tm_year == tm2.tm_year)
    {
        return true;
    }
    return false;
}
time_t sc_get_time()
{
	time_t currentTime;
	currentTime = time(&currentTime);
	return currentTime;
}
void sc_tolower(char* pStr)
{
    if ( NULL == pStr)
        return ;
    char* pCurrent = pStr;
    while(*pCurrent != '\0')
    {
        if (*pCurrent >= 'A' && *pCurrent <= 'Z' )
            *pCurrent +=  ('a' - 'A');
        ++pCurrent;
    }
}

void sc_touper(char* pStr)
{
    if (NULL == pStr)
        return ;
    char* pCurrent = pStr;
    while(pCurrent != '\0')
    {
        if (*pCurrent >= 'a' && *pCurrent <='z')
            *pCurrent -= ('a' - 'A');
        ++pCurrent;
    }
}

bool sc_is_path_dir(const char* pStrPath)
{
#ifdef WIN32
    return PathIsDirectoryA( pStrPath );
#else
    DIR* pDir = opendir( pStrPath );
    if ( NULL != pDir )
    {
        closedir(pDir);
        pDir = NULL;
        return  true;
    }else {
        return false;
    }
#endif
}
void sc_path_dir(std::string& path)
{
    if ( path.empty() )
        return ;
    for(std::string::iterator ite = path.begin();
        ite != path.end(); ++ite)
    {
        if ('\\' == (*ite) )
        (*ite) = '/';
    }
    if ('/' != path.at(path.length()-1))
        path.append("/");
}

bool sc_create_dir(const char* pStrPath)
{
    if (NULL == pStrPath )
        return false;
    std::string strPath(pStrPath);
    std::string sub;
    sc::util::sc_path_dir(strPath);
    std::string::size_type pos = strPath.find('/');
    while( std::string::npos != pos )
    {
        std::string cur = strPath.substr(0, pos);
        if (cur.length() > 0 && !sc_is_path_dir(cur.c_str()))
        {
            bool ret = false;
#ifdef WIN32
            ret = CreateDirectoryA(cur.c_str(), NULL);
#else
            ret = (mkdir(cur.c_str(), S_IRWXU|S_IRWXG|S_IRWXO)==0)
#endif
            if ( !ret )
                    return false;
        }
        pos = strPath.find('/',pos + 1 );
    }
    return  true;
}
COMMON_UTIL_END
SONDER_COMMON_END
