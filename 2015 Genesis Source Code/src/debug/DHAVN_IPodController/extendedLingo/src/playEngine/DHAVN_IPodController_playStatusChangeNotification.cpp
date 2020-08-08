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
#include <DHAVN_IPodController_playStatusChangeNotification.h>
#include <QByteArray>
#include <DHAVN_IPodController_CommonVars.h>
using namespace ExtendedLingo;

CPlayStatusChangeNotification::CPlayStatusChangeNotification( bool isTidEnabled)
    :CIAPPacket(0x04,0x0027,-1, isTidEnabled), m_NotificationType(0),m_NotificationValue(0),m_UID(0)
{
#ifdef DBUGLOGS
    LOG_INIT_V2(gControllerLogName);
#endif
}

QByteArray CPlayStatusChangeNotification::marshall()
{
        QByteArray command;
        return command;
}

int CPlayStatusChangeNotification::getNotificationType()
{
        return QString::number(m_NotificationType,10).toInt();
}

int CPlayStatusChangeNotification::getNotificationValue()
{
        return QString::number(m_NotificationValue,10).toInt();
}

quint64 CPlayStatusChangeNotification::getUID()
{
        return m_UID;
}

void  CPlayStatusChangeNotification::unmarshall(QByteArray returnCommand)
{
    if( ((int)returnCommand[0] == 0x55) &&
            ((int)returnCommand[2] == 0x04) &&
            ((int)returnCommand[3] == 0x00) &&
            ((int)returnCommand[4] == 0x27) ){
        if( m_isTidEnabled  )
	    {
		    m_NotificationType =  (int)returnCommand[7];
	    }
	    else
	    {
		    m_NotificationType =  (int)returnCommand[5];
        }

#ifdef DBUGLOGS
        //LOG_TRACE << "CPlayStatusChangeNotification::unmarshell: m_NotificationType " << m_NotificationType << LOG_ENDL;
#endif
	    //qDebug()<< " \n\n type is \n\n" << m_NotificationType;
        switch(m_NotificationType)
        {
        // track index change
        case 0x01:
        {
            int trackIndex = 0;
            int sh = 24;
            if( m_isTidEnabled )
            {
                for(int i = 8;i<12;i++)
                {
                    trackIndex  = trackIndex | (0x000000FF & returnCommand[i]);
                    trackIndex = trackIndex << sh;
                    sh = sh - 8;
                }
            }
            else
            {
                for(int i = 6;i<10;i++)
                {
                    trackIndex  = trackIndex | (0x000000FF & returnCommand[i]);
                    trackIndex = trackIndex << sh;
                    sh = sh - 8;
                }

            }
            m_NotificationValue = trackIndex;
            break;
        }
            // { Added by pawan 2013.05.04.
        case 0x0E:
        {
            int trackCount = 0;
            int sh = 24;
            if( m_isTidEnabled )
            {
                for(int i = 8;i<12;i++)
                {
                    trackCount  = trackCount | (0x000000FF & returnCommand[i]);
                    trackCount = trackCount << sh;
                    sh = sh - 8;
                }
            }
            else
            {
                for(int i = 6;i<10;i++)
                {
                    trackCount  = trackCount | (0x000000FF & returnCommand[i]);
                    trackCount = trackCount << sh;
                    sh = sh - 8;
                }

            }
            m_NotificationValue = trackCount;
        }
            break;
            // } Added by pawan 2013.05.04.
            //position change in mseconds
        case 0x07:
        {
#ifdef DBUGLOGS
            //LOG_TRACE << "CPlayStatusChangeNotification::unmarshell: Position change " << LOG_ENDL;
#endif
            //int trackOffsetSec = 0;
            //int sh = 24;
            int nTrackInfoDetails = 0;
            if( m_isTidEnabled )
            {
                for(int i = 8;i<12;i++)
                {
                    nTrackInfoDetails <<= 8;     nTrackInfoDetails |= (int)returnCommand[i] & 0xFF;
                }
            }
            else
            {
                for(int i = 6;i<10;i++)
                {
                    nTrackInfoDetails <<= 8;     nTrackInfoDetails |= (int)returnCommand[i] & 0xFF;
                }
            }
            /*for(int i = 8;i<12;i++)
                {
                        trackOffsetSec  = trackOffsetSec | (0x000000FF & returnCommand[i]);
                        trackOffsetSec = trackOffsetSec << sh;
                        sh = sh - 8;
                }*/
            m_NotificationValue = nTrackInfoDetails;//trackOffsetSec;
            break;
        }
        case 0x04:
        {
#ifdef DBUGLOGS
            //LOG_TRACE << "CPlayStatusChangeNotification::unmarshell: Position change in ms" << LOG_ENDL;
#endif
		    //int trackOffsetSec = 0;
		    //int sh = 24;
            int nTrackInfoDetails = 0;
            if( !m_isTidEnabled )
            {
			    for(int i = 6;i<10;i++)
			    {
				    nTrackInfoDetails <<= 8;     nTrackInfoDetails |= (int)returnCommand[i] & 0xFF;
			    }
            }
            m_NotificationValue = nTrackInfoDetails;//trackOffsetSec;
		    break;
        }
            //play state changed - play pause FF etc
        case 0x06:
        {
            if( m_isTidEnabled )
            {
                m_NotificationValue = (int)returnCommand[8];
            }
            else
            {
                m_NotificationValue = (int)returnCommand[6];
            }
            //                qDebug()<< " \n\n value is \n\n" << m_NotificationValue;
            break;
        }
        case 0x0A:
        {
            int startIndex =  -1;
            int endIndex =  -1;
            if( m_isTidEnabled )
            {
                startIndex = 8;
                endIndex = 16;
            }
            else
            {
                startIndex = 6;
                endIndex = 14;
            }
            for(int i = startIndex;i< endIndex ;i++)
            {
                m_UID <<= 8;     m_UID |= (quint8)returnCommand[i] & 0xFF;
            }
#ifdef DBUGLOGS
            //LOG_TRACE << "CPlayStatusChangeNotification::unmarshall: 0A change " << m_UID <<  LOG_ENDL;
#endif
            break;
        }
        }
    }
}

