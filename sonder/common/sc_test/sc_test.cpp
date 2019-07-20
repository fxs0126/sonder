#include<sc_log.h>
#include<sc_common.h>
using namespace sc::log;
using namespace sc::sem;
int main(int argc,char** argv)
{
    char tmpBuf[128]={0};
    unsigned logId[1000]={0};
    for (int index = 0; index < 1000; ++index)
    {
        sprintf(tmpBuf,"file_%d",index);
        logId[index] = SCLogManager::Instance()->createlog(tmpBuf);
        SCLogManager::Instance()->setConsoleShow(logId[index], index%2);
        SCLogManager::Instance()->setFileLogShow(logId[index], index%2+1);
    }
    SCLogManager::Instance()->start();
    while(true)
    {
        for (int index = 0; index < 1000; ++index)
        {
            sprintf(tmpBuf,"message%d_%d",logId[index],index%6);
            SCLogManager::Instance()->message(logId[index],index%6, tmpBuf);
        }
    }
    return 0;
}
