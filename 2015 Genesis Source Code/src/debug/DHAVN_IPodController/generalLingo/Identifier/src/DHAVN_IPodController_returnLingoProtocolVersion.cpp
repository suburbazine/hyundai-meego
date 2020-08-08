/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Sowmini Philip       *
 * DATE       :  12 November 2011                                   *
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
#include <DHAVN_IPodController_returnLingoProtocolVersion.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace ExtendedLingo;

CReturnLingoProtocolVersion::CReturnLingoProtocolVersion( bool isTidEnabled )
    :CIAPPacket(CIAPCmds::GeneralLingoId,CIAPCmds::L0_10_ReturnLingoProtocolVersion,-1, isTidEnabled ),m_MajorProtocolVersion(-1),m_MinorProtocolVersion(-1), m_reqLingoId(-1)
{
//    LOG_INIT_V2(gControllerLogName);
}

QByteArray CReturnLingoProtocolVersion::marshall()
{
    QByteArray command;
//    //LOG_TRACE << "CReturnLingoProtocolVersion::marshall shall never be reached" << LOG_ENDL;
    return command;
}

int CReturnLingoProtocolVersion::returnMajorProtocolVersion()
{
	return m_MajorProtocolVersion;
}

int CReturnLingoProtocolVersion::returnMinorProtocolVersion()
{
	return m_MinorProtocolVersion;
}

void  CReturnLingoProtocolVersion::unmarshall(QByteArray returnCommand)
{
    if( ((int)returnCommand[0] == 0x55) &&
        ((int)returnCommand[2] == 0x00) &&
        ((int)returnCommand[3] == 0x10))
    {
        if( m_isTidEnabled )
	    {
            m_reqLingoId = (int) returnCommand[6];
		    m_MajorProtocolVersion = (int)returnCommand[7];
            m_MinorProtocolVersion = (int)returnCommand[8];
	    }
	    else
	    {
            m_reqLingoId = (int) returnCommand[4];
		    m_MajorProtocolVersion = (int)returnCommand[5];
            m_MinorProtocolVersion = (int)returnCommand[6];
	    }
    }
}
