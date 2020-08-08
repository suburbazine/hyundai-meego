/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  BaseMediaPlay                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                                                                *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodController_retDBiTunesInfo.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace ExtendedLingo;

CRetDBiTunesInfo::CRetDBiTunesInfo( bool isTidEnabled)
    :CIAPPacket(0x04,0x0041,-1, isTidEnabled),m_InfoType(-1)
{
#ifdef DBUGLOGS
    LOG_INIT_V2(gControllerLogName);
#endif
    sec = 0;
    min = 0;
    hours = 0;
    day = 0;
    month = 0;
    year = 0;
    m_iTunesUid = 0; //uninitialized variable - coverity fix
}

QByteArray CRetDBiTunesInfo::marshall()
{
    QByteArray command;
    return command;
}


void  CRetDBiTunesInfo::unmarshall(QByteArray returnCommand)
{
#ifdef DBUGLOGS
//LOG_TRACE<<"CRetDBiTunesInfo::unmarshall:Entry"<<LOG_ENDL;
#endif
    if( ((int)returnCommand[0] == 0x55) &&
            ((int)returnCommand[2] == 0x04) &&
            ((int)returnCommand[4] == 0x3D) ){
#ifdef DBUGLOGS
        //LOG_TRACE<<"CRetDBiTunesInfo::unmarshall:1"<<LOG_ENDL;
#endif
        int i = 5;
        if( m_isTidEnabled){
            i = 7;
        }

        m_InfoType = returnCommand[i++];
#ifdef DBUGLOGS
        //LOG_TRACE<<"CRetDBiTunesInfo::unmarshall:infoType= "<<m_InfoType<<LOG_ENDL;
#endif
        switch(m_InfoType)
        {
        case 0x00:
        {
            uint64_t UID=0;

            for(int k =0; k< i+8; k++)
            {
                UID <<= 8;
                UID |= (int)returnCommand[i+k] & 0x00000000000000FF;
            }

            m_iTunesUid = UID;
            break;
        }
        case 0x01:
        {
            sec = (int)returnCommand[i++];
            min = (int)returnCommand[i++];
            hours = (int)returnCommand[i++];
            day = (int)returnCommand[i++];
            month = (int)returnCommand[i++];
            year = ((returnCommand[i]& 0x000000FF)<< 8) | (returnCommand[i+1]& 0x000000FF);
#ifdef DBUGLOGS
            //LOG_TRACE<<"CRetDBiTunesInfo::unmarshall:sec="<<sec<<",min"<<min<<",hour="<<hours<<",day="<<day<<", month="<<month<<", year="<<year<<LOG_ENDL;
#endif

        }

        default:
        {
                        break;
        }
        }
    }
}

int CRetDBiTunesInfo::getInfoType()
{
    return m_InfoType;
}

uint64_t CRetDBiTunesInfo::getiTunesUid()
{
    return m_iTunesUid;
}

QTime CRetDBiTunesInfo::getLastSyncTime()
{
    QTime time(hours,min,sec);
    return time;
}

QDate CRetDBiTunesInfo::getLastSyncDate()
{
    QDate date(year,month,day);
    return date;
}


