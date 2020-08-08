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
#ifndef RETACCESSORYINFO_H
#define RETACCESSORYINFO_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

struct SAccessoryInfo
{

    QString m_StringToSend;
};

namespace GeneralLingo
{

class CRetAccessoryInfo: public CIAPPacket
{
//private:
//    USE_LOG_UTIL
public:
    CRetAccessoryInfo(int transIdArgs,int infoType,SAccessoryInfo info);
    QByteArray marshall();
    void  unmarshall(QByteArray);
   enum EInfoTypes
   {
       CAPABILITIES,
       ACCESSORYNAME,
       ACCESSORYFIRMWAREVERSION,
       ACCESSORYHARDWAREVERSION,
       ACCESSORYMANUFACTURER,
       ACCESSORYMODELNUMBER,
   };

private:
	int m_TransId;
	int m_InfoType;
        SAccessoryInfo m_Info;
};

}
#endif // RETACCESSORYINFO_H
