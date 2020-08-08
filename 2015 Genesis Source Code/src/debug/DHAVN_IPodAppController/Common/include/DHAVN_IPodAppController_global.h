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
#ifndef DHAVN_IPODAppCONTROLLER_GLOBAL_H
#define DHAVN_IPODAppCONTROLLER_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QString>
#include <DHAVN_LogUtil.h>
#include <DHAVN_IPodAppController_CommonVars.h>
#if defined(DHAVN_IPODAppCONTROLLER_LIBRARY)
#  define DHAVN_IPODAppCONTROLLERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DHAVN_IPODAppCONTROLLERSHARED_EXPORT Q_DECL_IMPORT
#endif

const QString gIAPRawPcktHandler("IAPRawPcktHandler");
const QString gIAPReader("IAPReader");
const QString gIAPWriter("IAPWriter");
const int BYTESTOBEREAD = 2048;
const int TimeToWaitForSendCommand = 500; //ms
const int MAXSENDCOMMANDTRAIL = 9;

// Modified by jonghwan.cho@lge.com	2013.04.03.
// for changing filesystem read only
/*
const QString gIPodHidDevicePath ("/tmp/tempIPodData/ipodhiddevpath.txt");
const QString gIPodHidDevicePathPort1 ("/tmp/tempIPodData/ipodhiddevpath1.txt");
const QString gIPodHidDevicePathPort2 ("/tmp/tempIPodData/ipodhiddevpath2.txt");
const QString gIPodAppSessionIdPath ("/tmp/tempIPodData/");
*/
const QString gIPodHidDevicePath ("/app/data/AppMedia/tmp/tempIPodData/ipodhiddevpath.txt");
const QString gIPodHidDevicePathPort1 ("/app/data/AppMedia/tmp/tempIPodData/ipodhiddevpath1.txt");
const QString gIPodHidDevicePathPort2 ("/app/data/AppMedia/tmp/tempIPodData/ipodhiddevpath2.txt");
const QString gIPodAppSessionIdPath ("/app/data/AppMedia/tmp/tempIPodData/");

/**
  * IPOD app handle structure
  * This structure will store the ipod application details
  * to which the library is dealing with
  */
struct SIPodAppHandle
{
    QString m_AppId; //Something like "com.pandora.link.v1"
    QString m_SeedId; //something like "6KVQ6HHK5F"
    QString m_BundleID; //somethinglike "com.pandora"
    int m_SessionID; //session id of the current app session
    qint16 m_MaxPayloadSize; //maximum payload size to be written
    int m_DeviceNumber;
    int m_ProtocolIndex; // added by Tan 2013.01.03
};

class CIpodAppControllerGlobal
{
public:
static char checkSum(QByteArray data, int len)
{
    char sum=0;
    for(int i=1; i<=len; i++)
    {
        sum +=data[i];
    }
    return -sum;
}
static void printArray(QByteArray array)
{
    USE_LOG_UTIL;
    LOG_INIT_V2(gIPodAppControllerLogs);
    //LOG_TRACE << "Printing Array of command:" << LOG_ENDL;
    int count = array.count()/8;
    count++;
    /*for(int i=0;i<count;i++)
    {
        //LOG_TRACE << "[" <<  i << "]" <<  array.mid(i*8, 8).toHex().data() << LOG_ENDL;
    }*/

}
};
#endif // DHAVN_IPODAppCONTROLLER_GLOBAL_H
