/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  26 Septmber 2012                                 *
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
#ifndef DHAVN_IPODCONTROLLER_IDENTIFIER_IDPS_H
#define DHAVN_IPODCONTROLLER_IDENTIFIER_IDPS_H
#include <DHAVN_IPodController_Identifier.h>
#include <DHAVN_IPodController_getIPODOptionsForLingo.h>
#include <QVariantMap>
#include <QBitArray>
#include <QList>
#include <DHAVN_LogUtil.h>

using namespace GeneralLingo;

class CIdentifierIDPSTaskHandler : public CIdentifierTaskHandler
{
    Q_OBJECT
private:
    USE_LOG_UTIL
public:
    CIdentifierIDPSTaskHandler(QObject *parent);
    ~CIdentifierIDPSTaskHandler();
    void identify(QBitArray supportedLingoes, QList<FIDTokenValue> fidTokenValues);
    void SetFIDTokenValues(bool isInterleaved, bool bUsVariant);
private:
    void GetIPODOptionsForGeneralLingo();
    void GetIPODOptionsForStorageLingo();
    void EndIDPS();
public slots:
    virtual void packetAvailable(int lingoID,int commandID,const QByteArray& buf );
    void identify_restart_slot();               // for identify retry(CR 13200)
private:
    QBitArray m_SupportedLingoes;
    QList<FIDTokenValue> m_FidTokenValues;
    QBitArray m_SupportedLingoes_org;           // for identify retry(CR 13200)
    QList<FIDTokenValue> m_FidTokenValues_org;  // for identify retry(CR 13200)
    QBitArray m_SupportedDeviceOptions;
//    QString m_IPodPandoraAppSessionID;
};
#endif // DHAVN_IPODCONTROLLER_IDENTIFIER_IDPS_H
