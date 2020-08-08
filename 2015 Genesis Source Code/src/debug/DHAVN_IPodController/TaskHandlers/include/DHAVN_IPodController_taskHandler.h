/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
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
#ifndef TASKHANDLER_H
#define TASKHANDLER_H

#include <QObject>
#include <QByteArray>
#include <DHAVN_IPodController_IAPPacketReader.h>
#include <DHAVN_IPodController_IAPPacketWriter.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_PrivateData.h>

class CTaskHandler : public QObject
{
Q_OBJECT
public:
    CTaskHandler(QObject* parent);

/* Stops the current task */
    virtual void stopCurrentTask();
/* Stops the entair taskhandler. This will disconnect the taskhandler form iAP channel*
 * This can be called if the task handler is not required to operate.
 * This will improve the performance.
*/
    virtual void stopTaskHandler();

public slots:
    virtual void packetAvailable(int lingoID,int commandID,const QByteArray& buf );
signals:
    void sendiAPPacket(QByteArray packetBytes);
};

#endif // TASKHANDLER_H
