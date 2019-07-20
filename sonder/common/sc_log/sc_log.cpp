#include "../include/sc_log.h"
#ifdef _cpluscplus
#include <map>
#include <vector>
#include <list>
#include <cstring>
#else
#include <vector>
#include <map>
#include <list>
#include <string.h>
#endif
#include<iostream>
using namespace std;
#include "sc_file.h"
#include "sc_util.h"
#include "sc_sem.h"
#include "sc_console.h"
using namespace sc::file ;
using namespace sc::util ;
using namespace sc::sem;
using namespace sc::console;
using sc::console::CConsole;
SONDER_COMMON_BEGIN
COMMON_LOG_BEGIN
struct SLogInfo{
    char* _fileName;  //log file name 
    char* _proccessId;// proccess id 
    char* _logPath;	  // log path dir 	
    unsigned _nlogLevel;
    char _bDispConsole:2,
         _bDispFile:2,
         _bEnableLog:3,
         _bRecv:1;
    char _bMonthDir;
    unsigned _nLimitSize;
    time_t _tProccessTime;
    unsigned _nCurWritedLen;
    unsigned _nCurFileSuffix;
    sc::file::SCFile*  _pFileHandle;

    SLogInfo():_fileName(NULL),_proccessId(NULL),_logPath(NULL)
      ,_nlogLevel(0),_bDispConsole(0),_bDispFile(0),_bEnableLog(0),_bRecv(0)
      ,_bMonthDir(0),_nLimitSize(0),_tProccessTime(sc::util::sc_get_time()),_nCurWritedLen(0)
      ,_nCurFileSuffix(1),_pFileHandle(NULL)
    { }
    ~SLogInfo()
    {
        delete [] _fileName;
        delete [] _proccessId;
        delete [] _logPath;
        if( _pFileHandle )
            _pFileHandle->close();
        delete _pFileHandle;
    }
    void setDefault()
    {
#ifdef SONDER_LOG_DEFAULT_PATH
        _logPath = new char[strlen(SONDER_LOG_DEFAULT_PATH)+1]();
        strcpy(_logPath, SONDER_LOG_DEFAULT_PATH);
#endif
#ifdef SONDER_LOG_LEVEL_INFO
        _nlogLevel |= SONDER_LOG_LEVEL_INFO;
#endif // !SONDER_LOG_LEVEL_INFO

#ifdef SONDER_LOG_LEVEL_WARN
        _nlogLevel |= SONDER_LOG_LEVEL_WARN;
#endif // !SONDER_LOG_LEVEL_WARN

#ifdef SONDER_LOG_LEVEL_DEBUG
        _nlogLevel |= SONDER_LOG_LEVEL_DEBUG;
#endif // !SONDER_LOG_LEVEL_DEBUG

#ifdef SONDER_LOG_LEVEL_ASSERT
        _nlogLevel |= SONDER_LOG_LEVEL_ASSERT;
#endif // !SONDER_LOG_LEVEL_ASSERT

#ifdef SONDER_LOG_LEVEL_ERROR
        _nlogLevel |= SONDER_LOG_LEVEL_ERROR;
#endif // !SONDER_LOG_LEVEL_ERROR

#ifdef SONDER_LOG_LEVEL_FATAL
        _nlogLevel |= SONDER_LOG_LEVEL_FATAL;
#endif // !SONDER_LOG_LEVEL_FATAL
#ifdef SONDER_ALL_SYNCHRONOUS_DISPLAY
        _bDispConsole = 1;
        _bEnableLog |= 0x01;
#else
        _bDispConsole = 0;
        _bEnableLog  &= ~0x01
#endif
#ifdef SONDER_ALL_WRITE_TO_FILE
        _bDispFile = 1;
        _bEnableLog |= 0x02;
#else
        _bDispFile = 0 ;
        _bEnableLog &= ~0x02;
#endif

#ifdef SONDER_DEFAULT_MONTH_DIR
        _bMonthDir = 1;
#else
        _bMonthDir = 0;
#endif
#ifdef SONDER_LOG_DEFAULT_LIMIT_SIZE
        _nLimitSize = SONDER_LOG_DEFAULT_LIMIT_SIZE*1024*1024;
#else
        _nLimitSize = 4*1024*1024;
#endif
    }

    void setLogFileName(std::string& logFile)
    {
        delete [] _fileName;
        _fileName = new char[logFile.length()+1]();
        strcpy(_fileName, logFile.c_str());
    }
    void setConsoleShow(bool bShow)
    {
        bShow ? _bDispConsole = 1: _bDispConsole = 0;
        bShow ? _bEnableLog |= 0x01: _bEnableLog  &= ~0x01;
    }
    void setFileShow(bool bShow)
    {
        bShow ?  _bDispFile = 1 : _bDispFile = 0;
        bShow ?  _bEnableLog |= 0x02: _bEnableLog &= ~0x02;
    }
    void setLogLevel(unsigned level , bool bShow)
    {
        bShow ? _nlogLevel |= level : _nlogLevel &= (~level);
    }
    void setLogDirPath(const char* path)
    {
        if ( !path )
            return;
        delete [] _logPath;
        _logPath = new char[strlen(path)+1]();
        strcpy(_logPath, path);
    }
    void setLogMonthPathEnable(bool enable)
    {
        _bMonthDir = enable ? 1: 0;
    }
};

struct  SLogContent
{
	unsigned _nLogId;
	unsigned _nLogLvl;
	time_t   _tLogTime;
	char*    _pLogContent;
	SLogContent()
		:_nLogId(-1), _nLogLvl(0),_tLogTime(sc::util::sc_get_time()) ,_pLogContent(NULL)
	{}
	~SLogContent()
	{
		delete [] _pLogContent;
		_pLogContent = NULL;
	}
};
class SCLogDataImp
{
public:
    typedef std::map<std::string, unsigned> LogFileIdMap;
    typedef std::vector<SLogInfo*>			IdLogInfoVec;
	typedef std::list<SLogContent*>			LogContentList;
    SCLogDataImp();
    ~SCLogDataImp();
    int createlog(std::string& logfile);
	void message(const unsigned& logid, const unsigned& loglevel, const char* content);
	void release(SLogContent* &pContent);
	bool getcontent(SLogContent* &pContent);
	bool contentempty();
    void setConsoleShow(const unsigned& logid, bool bShow);
    void setFileShow(const unsigned& logid, bool bShow);
    void setlogLevelShow(const unsigned& logid, unsigned level, bool bShow);
	void synclog();
	void printlog();
protected:
private:
	SLogInfo* findLogInfo(const unsigned& logid)const;
	SCFile*   openfilehandle(const unsigned& logid , bool bSameDay, bool bCreateNew);
private:
    LogFileIdMap _logFile2Id;
    IdLogInfoVec _id2LogInfo;
	SCLock         _contentLock;
	LogContentList _logContentList;

	SLogContent* _pContent ;
	SLogInfo*	 _pLogInfo ;
	char _messageBuffer[1024];
	bool _bNewCreated ;
	char* _message ;
	char* _pNext ;

};

SCLogDataImp::SCLogDataImp()
    :_logFile2Id()
    ,_id2LogInfo()
{}

SCLogDataImp::~SCLogDataImp()
{
    _logFile2Id.clear();

    for (unsigned index = 0; index < _id2LogInfo.size(); ++index)
        delete (_id2LogInfo[index]);
    _id2LogInfo.clear();
}
int SCLogDataImp::createlog(std::string &logfile)
{
#ifdef SONDER_LOG_DEFAULT_MAX_NUM
        if ( _id2LogInfo.size() >= SONDER_LOG_DEFAULT_MAX_NUM )
            return -1;
#endif
    LogFileIdMap::iterator iteFileId = _logFile2Id.find(logfile);
    if (iteFileId == _logFile2Id.end())
    {
        unsigned nId = static_cast<unsigned>(_id2LogInfo.size());
        _logFile2Id.insert(std::pair<std::string, unsigned>(logfile, nId));
        SLogInfo* info = new SLogInfo();
        _id2LogInfo.push_back(info);
        info->setLogFileName(logfile);
        info->setDefault();
		return nId;
    }
    return -1;
}
void SCLogDataImp::message(const unsigned& logid, const unsigned& loglevel, const char* content)
{
	if (logid >= 0 && loglevel >= 0 && NULL != content)
	{
		SCAutoLock autoLock(_contentLock);
		SLogContent* pContent = new SLogContent();
		pContent->_nLogId = logid;
		pContent->_nLogLvl = loglevel;
		pContent->_pLogContent = new char[ strlen(content) + 1]();
		strcpy(pContent->_pLogContent, content);
		_logContentList.push_back(pContent);
	}
}

void SCLogDataImp::release(SLogContent* &pContent)
{
	delete pContent;
	pContent = NULL;
}
bool SCLogDataImp::getcontent(SLogContent* &pContent)
{
	SCAutoLock autoLock(_contentLock);
	if ( _logContentList.empty() )
		return false;
	pContent = _logContentList.front();
	_logContentList.pop_front();
	
	return true;
}
bool SCLogDataImp::contentempty()
{
	SCAutoLock autoLock(_contentLock);
	return _logContentList.empty();
}
void SCLogDataImp::setConsoleShow(const unsigned int &logid, bool bShow)
{
    SLogInfo* pInfo = findLogInfo(logid);
    if ( !pInfo )
        return ;
    pInfo->setConsoleShow(bShow);
}
void SCLogDataImp::setFileShow(const unsigned int &logid, bool bShow)
{
    SLogInfo* pInfo = findLogInfo(logid);
    if (!pInfo)
        return ;
    pInfo->setFileShow(bShow);
}
void SCLogDataImp::setlogLevelShow(const unsigned int &logid, unsigned level, bool bShow)
{
    SLogInfo* pInfo = findLogInfo(logid);
    if (!pInfo)
        return ;
    pInfo->setLogLevel( level, bShow);
}
void SCLogDataImp::synclog()
{
	while (getcontent(_pContent))
	{
		_pLogInfo = findLogInfo(_pContent->_nLogId);
		if (NULL == _pLogInfo || 0 == _pLogInfo->_bEnableLog)
		{
			release(_pContent);
			continue;
		}

		if (!(_pContent->_nLogLvl & _pLogInfo->_nlogLevel))
		{
			release(_pContent);
			continue;
		}
		if (strlen(_pContent->_pLogContent) + 32 > 1022)
		{
			_bNewCreated = true;
			_message = new char[strlen(_pContent->_pLogContent) + 32]();
		}
		else
		{
			_bNewCreated = false;
			_message = _messageBuffer;
		}
		_pNext = _message;
		switch (_pContent->_nLogLvl & _pLogInfo->_nlogLevel)
		{
		case SONDER_LOG_LEVEL_INFO:
			strcpy(_pNext, "[I].");
			_pNext += strlen("[I].");
			break;
		case SONDER_LOG_LEVEL_WARN:
			strcpy(_pNext, "[W].");
			_pNext += strlen("[W].");
			break;
		case SONDER_LOG_LEVEL_DEBUG:
			strcpy(_pNext, "[D].");
			_pNext += strlen("[D].");
			break;
		case SONDER_LOG_LEVEL_ASSERT:
			strcpy(_pNext, "[A].");
			_pNext += strlen("[A].");
			break;
		case SONDER_LOG_LEVEL_ERROR:
			strcpy(_pNext, "[E].");
			_pNext += strlen("[E].");
			break;
		case SONDER_LOG_LEVEL_FATAL:
			strcpy(_pNext, "[F].");
			_pNext += strlen("[F].");
			break;
		default:
			break;
		}
		tm logTime;
		sc::util::sc_time2tm(_pContent->_tLogTime, &logTime);
		sprintf(_pNext, "%04d_%02d%02d %02d:%02d:%02d.%03d ", logTime.tm_year + 1900,
			logTime.tm_mon + 1, logTime.tm_mday, logTime.tm_hour, logTime.tm_min, logTime.tm_sec,
			logTime.tm_isdst);
		_pNext += 23;
		strcpy(_pNext, _pContent->_pLogContent);
		if (_pLogInfo->_bDispFile)
		{
			bool bSameDay = sc::util::sc_is_same_day(_pContent->_tLogTime, _pLogInfo->_tProccessTime);
			bool needCreateNew =_pLogInfo->_nCurWritedLen >= _pLogInfo->_nLimitSize;
			if (NULL == _pLogInfo->_pFileHandle
				|| !bSameDay
				|| needCreateNew
				)
			{
				openfilehandle(_pContent->_nLogId, bSameDay, needCreateNew);
			}
			if ( _pLogInfo->_pFileHandle->isopen())
			{
				_pLogInfo->_pFileHandle->write(_message, strlen(_message));
				_pLogInfo->_nCurWritedLen += strlen(_message);
			}
		}
		if (_pLogInfo->_bDispConsole)
		{
			switch (_pContent->_nLogLvl & _pLogInfo->_nlogLevel)
			{
			case SONDER_LOG_LEVEL_INFO:
				CConsole::instance()->setcolor(CONSOLE_TEXT_BLUE | CONSOLE_TEXT_GREEN,0);
				break;
			case SONDER_LOG_LEVEL_WARN:
				CConsole::instance()->setcolor(CONSOLE_TEXT_RED | CONSOLE_TEXT_BLUE, 0);
				break;
			case SONDER_LOG_LEVEL_DEBUG:
				CConsole::instance()->setcolor(CONSOLE_TEXT_GREEN | CONSOLE_TEXT_RED, 0);
				break;
			case SONDER_LOG_LEVEL_ASSERT:
				CConsole::instance()->setcolor(CONSOLE_TEXT_BLUE | CONSOLE_TEXT_RED | CONSOLE_TEXT_INTENSITY, 0);
				break;
			case SONDER_LOG_LEVEL_ERROR:
				CConsole::instance()->setcolor(CONSOLE_TEXT_BLUE| CONSOLE_TEXT_INTENSITY, 0);
				break;
			case SONDER_LOG_LEVEL_FATAL:
				CConsole::instance()->setcolor(CONSOLE_TEXT_RED | CONSOLE_TEXT_INTENSITY, 0);
				break;
			default:
				break;
			}
			CConsole::instance()->print( _message );
			CConsole::instance()->reset();
			sc::util::sc_sleep(1);
		}
		if ( _bNewCreated )
			delete[] _message;
		_message = NULL;
		release(_pContent);
	}
}
void SCLogDataImp::printlog()
{
	if (_id2LogInfo.empty())
		return;
	for (size_t index = 0; index < _id2LogInfo.size(); ++index)
	{
		std::cout << "log name:" << _id2LogInfo[index]->_fileName << std::endl;
	}
}

SLogInfo* SCLogDataImp::findLogInfo(const unsigned& logid)const
{
	if (_id2LogInfo.empty() || logid < 0 || logid >= _id2LogInfo.size())
		return NULL;
	return _id2LogInfo[logid];
}
SCFile* SCLogDataImp::openfilehandle(const unsigned& logid, bool bSameDay, bool bCreateNew)
{
	SLogInfo* pLogInfo = findLogInfo(logid);
	if ( NULL == pLogInfo )
		return NULL;

	bool bMonthDir = pLogInfo->_bMonthDir;
	std::string strLogName = pLogInfo->_logPath;

	if (bMonthDir)
	{
		char strYMonth[32] = {0};
		tm t;
		time_t & tFileTime = _id2LogInfo[logid]->_tProccessTime;
		sc::util::sc_time2tm(tFileTime,&t);
		sprintf(strYMonth, "/%04d_%02d/", t.tm_year + 1900, t.tm_mon + 1);
		strLogName += strYMonth;
	}
	if (!sc::util::sc_is_path_dir(strLogName.c_str()))
	{
		sc::util::sc_create_dir(strLogName.c_str());
	}
	if (!bSameDay)
	{
		pLogInfo->_nCurWritedLen  = 0;
		pLogInfo->_nCurFileSuffix = 0;
		pLogInfo->_tProccessTime = sc::util::sc_get_time();
	}
	else if (bCreateNew)
	{
		pLogInfo->_nCurFileSuffix++;
		pLogInfo->_nCurWritedLen = 0;
	}
	char logSuffix[64] = { 0 };
	tm fileTm;
	sc::util::sc_time2tm(pLogInfo->_tProccessTime, &fileTm);
	if (bMonthDir)
		sprintf(logSuffix, "_%02d_%03d", fileTm.tm_mday, pLogInfo->_nCurFileSuffix);
	else
		sprintf(logSuffix, "_%04d_%02d%02d_%03d", fileTm.tm_year + 1900, fileTm.tm_mon + 1, fileTm.tm_mday,
			pLogInfo->_nCurFileSuffix);
	strLogName += pLogInfo->_fileName;
	strLogName += logSuffix;
	strLogName += ".log";
	if (NULL == pLogInfo->_pFileHandle )
		pLogInfo->_pFileHandle = new SCFile();
	if (pLogInfo->_pFileHandle && pLogInfo->_pFileHandle->isopen())
		pLogInfo->_pFileHandle->close();
	pLogInfo->_pFileHandle->open(strLogName.c_str(), "a+");
	pLogInfo->_nCurWritedLen = pLogInfo->_pFileHandle->length();
	return pLogInfo->_pFileHandle;
}

SCLogManager* SCLogManager::_pSCLogMgrInstance = NULL;
SCLogManager::CAutoRelease  SCLogManager::_autoRelease;
SCLogManager::SCLogManager()
    :_pImp( new SCLogDataImp())
{

}
SCLogManager::~SCLogManager()
{
	SCAutoLock autoLock(_ImpLock);
	delete _pImp;
	_pImp = NULL;
}

SCLogManager* SCLogManager::Instance()
{
    if ( NULL == _pSCLogMgrInstance )
        _pSCLogMgrInstance = new SCLogManager();
    return _pSCLogMgrInstance;
}
int SCLogManager::createlog(std::string &logFile)
{
	SCAutoLock autoLock(_ImpLock);
    if ( _pImp )
      return _pImp->createlog(logFile);
    return 0;
}
void SCLogManager::message(const unsigned& logid, const unsigned& loglevel, const char* content)
{
	SCAutoLock autoLock(_ImpLock);
	if (_pImp && logid >=0 && loglevel > 0 && NULL != content)
		_pImp->message(logid, loglevel, content);
}
void SCLogManager::setConsoleShow(const unsigned& logid, bool bShow /*= false*/)
{
    if ( _pImp )
        _pImp->setConsoleShow(logid, bShow);
}
void SCLogManager::setFileLogShow(const unsigned int &logid, bool bShow)
{
    if ( _pImp )
        _pImp->setFileShow(logid, bShow);
}
void SCLogManager::setLogLevelEnable(const unsigned int &logid, unsigned int level, bool bShow)
{
    if ( _pImp )
        _pImp->setlogLevelShow(logid, level, bShow);
}
void SCLogManager::run()
{
	_bRun = 1;

	{
		SCAutoLock autoLock(_ImpLock);
		_pImp->printlog();
	}
	while ( _bRun > 0)
	{	
		{
			SCAutoLock autoLock(_ImpLock);
			_pImp->synclog();
		}
		sc::util::sc_sleep(20);
	}
	_bRun = 0;
}
void SCLogManager::stop()
{
	_bRun = 0;
	SCThread::wait();
}
COMMON_LOG_END
SONDER_COMMON_END
