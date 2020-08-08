#include "DHAVN_QMLLogUtil.h"


CQMLLogUtil::CQMLLogUtil()
{
    LOG_INIT_V2("Pandora");
}

void CQMLLogUtil::log(QString inMessage , quint8 inLevel)
{

    switch(inLevel)
    {
        case 0:
        {
            LOG_LOW<<inMessage<<LOG_ENDL;
        }
        break;
        case 1:
        {
            LOG_TRACE<<inMessage<<LOG_ENDL;
        }
        break;
        case 2:
        {
            LOG_MEDIUM<<inMessage<<LOG_ENDL;
        }
        break;
        case 3:
        {
            LOG_INFO<<inMessage<<LOG_ENDL;
        }
        break;
        case 4:
        {
            LOG_HIGH<<inMessage<<LOG_ENDL;
        }
        break;
        case 5:
        {
            LOG_SIGNAL<<inMessage<<LOG_ENDL;
        }
        break;
        case 6:
        {
            LOG_SLOT<<inMessage<<LOG_ENDL;
        }
        break;
        case 7:
        {
            LOG_TRANSITION<<inMessage<<LOG_ENDL;
        }
        break;
        case 8:
        {
            LOG_CRITICAL<<inMessage<<LOG_ENDL;
        }
        break;
    default:
        break;

    }
}
