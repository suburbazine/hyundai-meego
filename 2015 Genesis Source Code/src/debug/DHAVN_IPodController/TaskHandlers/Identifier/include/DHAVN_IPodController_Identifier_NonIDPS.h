/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  26 Sept 2012                                     *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                   This is the Identification TaskHandler       *
 *                   it Starts and handle the whole iden-         *
 *                   tification process                           *                                              *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef DHAVN_IPODCONTROLLER_IDENTIFIER_NONIDPS_H
#define DHAVN_IPODCONTROLLER_IDENTIFIER_NONIDPS_H
#include <DHAVN_IPodController_Identifier.h>
#include <QVariantMap>
#include <DHAVN_LogUtil.h>

//class Controller; //has to be removed once the controller class is up and running

using namespace GeneralLingo;

class CIdentifierNonIDPSTaskHandler : public CIdentifierTaskHandler
{
    Q_OBJECT
private:
    USE_LOG_UTIL
public:
    CIdentifierNonIDPSTaskHandler(QObject *parent);
    ~CIdentifierNonIDPSTaskHandler();
    void identify(QBitArray supportedLingoes, QList<FIDTokenValue> fidTokenValues);
public slots:
    virtual void packetAvailable(int lingoID,int commandID,const QByteArray& buf );
    void identify_restart_slot();		// for identify retry(CR 13200)
    void identifyDeviceLingoReset();
    void identifyDeviceLingo();
private:
    iPodControllerPrivateData* m_iPodPrivateData;
    bool m_EmitIdentify;
};
#endif // DHAVN_IPODCONTROLLER_IDENTIFIER_NONIDPS_H
