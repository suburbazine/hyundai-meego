
/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS:  Neelima kasam                    *
 * DATE       :  08 February 2012                                   *
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
#ifndef RETURNSOFTWAREVERSION_H
#define RETURNSOFTWAREVERSION_H


#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace GeneralLingo
{

class CReturnSoftwareVersion: public CIAPPacket
{
public:
    CReturnSoftwareVersion( bool isTidEnabled);
    QByteArray marshall()        ;
    void  unmarshall(QByteArray)        ;
    int returnMajorSoftwareVersion();
    int returnMinorSoftwareVersion();
    int returnDeviceRevisionSoftwareVersion();
private:
	int m_MajorSoftwareVersion;
	int m_MinorSoftwareVersion;
	int m_DeviceRevisionSoftwareVersion;

};

}

#endif //RETURNSOFTWAREVERSION_H
