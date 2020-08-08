#ifndef DHAVN_IPODCONTROLLER_GETACCESSORYINFO_H
#define DHAVN_IPODCONTROLLER_GETACCESSORYINFO_H

/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPOdcontroller                                    *
 *
 * PROGRAMMERS :  Sowmini / Venkat                    *
 * DATE       :  Oct 12 2011                                   *
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
#ifndef GETACCESSORYINFO_H
#define GETACCESSORYINFO_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace GeneralLingo
{

class CGetAccessoryInfo: public CIAPPacket
{
//private:
//    USE_LOG_UTIL
public:
    CGetAccessoryInfo(int transIdArgs);
    QByteArray marshall();
    void  unmarshall(QByteArray);
    int getInfoType();
private:
        int m_TransId;
        int m_InFoType;
};

}
#endif // RETACCESSORYINFO_H

#endif // DHAVN_IPODCONTROLLER_GETACCESSORYINFO_H
