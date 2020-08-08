#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_remoteEventNotification.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace DisplayRemoteLingo;

CRemoteEventNotification::CRemoteEventNotification( int transId, bool isTidEnabled )
    :CIAPPacket()
{
    LOG_INIT_V2(gControllerLogName);
    m_TransId = transId;
    m_isTidEnabled = isTidEnabled;

    m_Index = 0;
    m_NotificationType = 0;
    m_NotificationValue = 0;
    //LOG_TRACE << "In RemotEventNotification constructor" << LOG_ENDL;
}

QByteArray CRemoteEventNotification::marshall()
{
    return NULL;
}

void  CRemoteEventNotification::unmarshall(QByteArray returnCommand)
{
    if( ((int)returnCommand[0] == 0x55) &&    // Command ID
        ((int)returnCommand[2] == 0x03) &&   // Display Command ID
        ((int)returnCommand[3] == 0x09) ) {   // Remote Event Notification Data

        if( m_isTidEnabled == true ) {
            m_NotificationType =  (int)returnCommand[6];
            m_Index = 7;
        }
        else {
            m_NotificationType =  (int)returnCommand[4];
            m_Index = 5;
        }

        //LOG_TRACE << "CRemoteEventNotification::unmarshell: m_NotificationType " << m_NotificationType << LOG_ENDL;
        switch( m_NotificationType ) {
            case 0x03 :  // Play Status ( 1 Byte )
                m_NotificationValue = (int)returnCommand[m_Index++];
                break;
            case 0x07 :  // Shuffle Status ( 1 Byte )
                m_NotificationValue = (int)returnCommand[m_Index++];
                break;
            case 0x08 :  // Repeat Status  ( 1 Byte )
                m_NotificationValue = (int)returnCommand[m_Index++];
                break;
            case 0x0E :  // Audiobok playback speed ( 1Byte )
                m_NotificationValue = (int)returnCommand[m_Index++];
                break;
            case 0x00 :  // Track Position      ( 4 Byte )
            case 0x01 :  // Track Index         ( 4 Byte )
            case 0x02 :  // Chapter Information ( 8 Byte )
            case 0x04 :  // Mute/UI Volume     ( 2 Byte )
            case 0x05 :  // Power/Battery      ( 2 Byte )
            case 0x06 :  // Equalize state       ( 4 Byte )
            case 0x09 :  // Date/Time           ( 6 Byte )
            case 0x0A :  // Alarm               ( 3 Byte )
            case 0x0B :  // Back Light           ( 1 Byte )
            case 0x0C :  // Hold Switch          ( 1 Byte )
            case 0x0D :  // Sound Check         ( 1 Byte )
            case 0x0F :  // Track Position in Second ( 2 Byte )
            case 0x10 :  // Mute / UI / Absolute Volume ( 3 Byte )
            case 0x11 :  // Track capabilitites     ( 4 Byte )
            case 0x12 :  // Playback engine change  ( 4 Byte )
                         // number of tracks in new playlist
            default :
                //LOG_TRACE << "CRemoteEventNotification::unmarshell: Unsupport not yet = " << m_NotificationType << LOG_ENDL;
                break;
        }
    }
}
