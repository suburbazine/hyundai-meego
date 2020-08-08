/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS:  Neelima Kasam                                     *
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
#include <QByteArray>
#include <DHAVN_IPodController_returnSoftwareVersion.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>


using namespace GeneralLingo;

CReturnSoftwareVersion::CReturnSoftwareVersion( bool isTidEnabled )
        :CIAPPacket(0x00,0x000A,-1, isTidEnabled),m_MajorSoftwareVersion(-1),m_MinorSoftwareVersion(-1),m_DeviceRevisionSoftwareVersion(-1)
{
}

QByteArray CReturnSoftwareVersion::marshall()
{
    QByteArray command;
//    //LOG_TRACE << "CReturnSoftwareVersion::marshall shall never be reached" << LOG_ENDL;
    return command;
}

int CReturnSoftwareVersion::returnMajorSoftwareVersion()
{
	return m_MajorSoftwareVersion;
}

int CReturnSoftwareVersion::returnMinorSoftwareVersion()
{
	return m_MinorSoftwareVersion;
}
int CReturnSoftwareVersion::returnDeviceRevisionSoftwareVersion()
{
	return m_DeviceRevisionSoftwareVersion;
}
void  CReturnSoftwareVersion::unmarshall(QByteArray returnCommand)
{
//    //LOG_TRACE << "CReturnSoftwareVersion::unmarshall shall never be reached" << LOG_ENDL;
    if( ((int)returnCommand[0] == 0x55) &&
        ((int)returnCommand[2] == 0x00) &&
        ((int)returnCommand[3] == 0x0A))
    {
            if( m_isTidEnabled )
	    {
                    m_MajorSoftwareVersion = 0 | (int)returnCommand[6];
                    m_MinorSoftwareVersion = 0 | (int)returnCommand[7];
                    m_DeviceRevisionSoftwareVersion = 0 | (int)returnCommand[8];

            }
	    else
	    {
		    m_MajorSoftwareVersion = (int)returnCommand[4];
		    m_MinorSoftwareVersion = (int)returnCommand[5];
                    m_DeviceRevisionSoftwareVersion = (int)returnCommand[6];
            }
    }
}
