/*
 ******************************************************************
 *        COPYRIGHT © Teleca AB                                   *
 *----------------------------------------------------------------*
 * MODULE     :  IPodAppController                                *
 *
 * PROGRAMMERS : Pawan Gupta                                      *
 * DATE       :  16 jan 2012                                      *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :

 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef IAPPACKET_H
#define IAPPACKET_H

#include <QByteArray>
#include <QObject>
#include <QDebug>
//#include <DHAVN_IPodController_Def.h>

class CIAPCmds : public QObject
{
public:
    enum CIAPLingoId{
        GeneralLingoId = 0x00,
        ExtenededLingoId = 0x04,
        DigitalLingoId = 0x0A
    };
    enum CIAPExtendedCmds{
        ACK                         = 0x0001,
        GetIndexedPlayingTrackInfo  = 0x000C,
        RetIndexedPlayingTrackInfo  = 0x000D,
        GetArtworkFormats           = 0x000E,
        RetArtworkFormats           = 0x000F,
        GetTrackArtworkData         = 0x0010,
        RetTrackArtworkData         = 0x0011,
        GetTrackArtworkTimes        = 0x002A,
        RetTrackArtworkTimes        = 0x002B
    };
};


class CIAPPacket : public QObject
{
public:
    CIAPPacket():m_TransactionID(-1), m_LingoID(-1), m_CommandID(-1) {}// added by jungae 2012.11.10, Static_Test 13235 {}
    CIAPPacket(QObject* parent):QObject(parent), m_TransactionID(-1), m_LingoID(-1), m_CommandID(-1) {} // added by jungae 2012.11.10, Static_Test 13234{}
    CIAPPacket(CIAPPacket& args):QObject(&args){m_CommandBytes = args.getCommandBytes();}
    CIAPPacket(QByteArray array): m_TransactionID(-1), m_LingoID(-1), m_CommandID(-1) {m_CommandBytes = array;} // added by jungae 2012.11.10,, Static_Test 13233
    CIAPPacket(int lingo,int command,int transactionId,QObject* parent):QObject(parent),
       m_TransactionID(transactionId),  m_LingoID(lingo), m_CommandID (command){

    }
    virtual QByteArray marshall() {return NULL;}
    virtual void unmarshall(const QByteArray&) {}
    QByteArray getCommandBytes()
    {
        return m_CommandBytes;
    }
    int getTransactionId()
    {
        return m_TransactionID;
    }
    int getLingoID()
    {
        return m_LingoID;
    }
    int getCommandID()
    {
        return m_CommandID;
    }

protected:
    QByteArray m_CommandBytes;
    int m_TransactionID;
public:
    int m_LingoID;
    int m_CommandID;
};
#endif //IAPPACKET_H
