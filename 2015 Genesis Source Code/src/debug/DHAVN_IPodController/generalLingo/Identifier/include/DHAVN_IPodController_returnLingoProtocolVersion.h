
/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Sowmini Philip                    *
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
#ifndef RETURNLINGOPROTOCOLVERSION_H
#define RETURNLINGOPROTOCOLVERSION_H


#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace ExtendedLingo
{

class CReturnLingoProtocolVersion: public CIAPPacket
{
public:
    CReturnLingoProtocolVersion( bool isTidEnabled);
    QByteArray marshall()        ;
    void  unmarshall(QByteArray)        ;
    int returnMajorProtocolVersion();
    int returnMinorProtocolVersion();
    int returnReqLingoId(){return m_reqLingoId;}
private:
	int m_MajorProtocolVersion;
	int m_MinorProtocolVersion;
        int m_reqLingoId;
};

}

#endif //RETURNLINGOPROTOCOLVERSION_H
