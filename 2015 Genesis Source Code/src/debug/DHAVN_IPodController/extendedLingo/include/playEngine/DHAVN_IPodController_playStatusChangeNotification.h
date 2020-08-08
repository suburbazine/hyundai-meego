/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  BaseMediaPlay                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
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
#ifndef PLAYSTATUSCHANGENOTIFICATION_H
#define PLAYSTATUSCHANGENOTIFICATION_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>
namespace ExtendedLingo
{
    class CPlayStatusChangeNotification : public CIAPPacket
    {
#ifdef DBUGLOGS
        private:
            USE_LOG_UTIL
#endif
        public:
           CPlayStatusChangeNotification( bool isTidEnabled);
           QByteArray marshall();
           void unmarshall(QByteArray);
           int getNotificationType();
           int getNotificationValue();
	   quint64 getUID();

        public:  // Modified by pawan 2013.05.28.
           int m_NotificationType;
           int m_NotificationValue;
           quint64 m_UID;
    };

} // end of namespace ExtendedLingo
#endif // PLAYSTATUSCHANGENOTIFICATION_H
