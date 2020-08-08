/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Neelima Kasam                                      *
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
#ifndef REQUESTSOFTWAREVERSION_H
#define REQUESTSOFTWAREVERSION_H


#include <DHAVN_IPodController_iAPPacket.h>

namespace GeneralLingo
{
	class CRequestSoftwareVersion: public CIAPPacket
	{
                public:
                        CRequestSoftwareVersion(int transId, bool isTidEnabled);
			QByteArray marshall()        ;
			void  unmarshall(QByteArray)        ;
    };

}

#endif //REQUESTSOFTWAREVERSION_H
