#include "sc_file.h"

SONDER_COMMON_BEGIN
COMMON_FILE_BEGIN

SCFile::SCFile()
    :_pFile (NULL)
{

}
SCFile::~SCFile()
{
	SCAutoLock autoLock(_locker);
	if (_pFile)
		fclose( _pFile );
	_pFile = NULL;
}

bool SCFile::open(const char *path, const char *mode)
{
	SCAutoLock autoLock(_locker);
    if ( _pFile )
        fclose(_pFile);
    _pFile = fopen( path, mode);
    if( _pFile )
        return true;
    return false;
}
void SCFile::close()
{
	SCAutoLock autoLock(_locker);
	if (NULL != _pFile)
	{
		fflush(_pFile);
		fclose(_pFile);
	}
	_pFile = NULL;
}
bool SCFile::isopen()
{
	SCAutoLock autoLock(_locker);
    return (NULL != _pFile) ? true : false;
}

unsigned SCFile::write(const char* buf , size_t len)
{
	SCAutoLock autoLock(_locker);
    if(NULL == _pFile || len <=0 || NULL == buf)
        return 0;
	fseek(_pFile, 0, SEEK_END);
    size_t wLen = fwrite(buf, strlen(buf), 1, _pFile);
	flush();
	if (wLen != 1)
		fclose(_pFile);
    return static_cast<unsigned>(len);
}
void SCFile::flush()
{
	SCAutoLock autoLock(_locker);
    if(!_pFile )
        return ;
    fflush( _pFile );
}

bool SCFile::readline(char* buf, size_t len)
{
	SCAutoLock autoLock(_locker);
    if (!_pFile)
        return false;
    if ( NULL == fgets(buf, len, _pFile))
        return false;
    return true;
}

void SCFile::readcontent(char* &pBuf , size_t & len)
{
	SCAutoLock autoLock(_locker);
    if(!_pFile)
        return ;
    fseek(_pFile, 0, SEEK_SET);
    long beginPos = ftell(_pFile);
    fseek(_pFile, 0, SEEK_END);
    long endPos = ftell(_pFile);
    fseek(_pFile, 0, SEEK_SET);
    len = endPos - beginPos;
    if ( len <= 0 )
        return;
    pBuf = new char[len+1]();
    if ( len != fread(pBuf, 1, len, _pFile ) )
    {
        delete [] pBuf;
        len = 0;
    }
}
unsigned long long SCFile::length() 
{
	SCAutoLock autoLock(_locker);
	if ( !_pFile )
		return 0;
	fseek(_pFile, 0, SEEK_SET);
	long beginPos = ftell(_pFile);
	fseek(_pFile, 0, SEEK_END);
	long endPos = ftell(_pFile);
	fseek(_pFile, 0, SEEK_SET);
	unsigned long long len = endPos - beginPos;
	return len;
}

COMMON_FILE_END
SONDER_COMMON_END
