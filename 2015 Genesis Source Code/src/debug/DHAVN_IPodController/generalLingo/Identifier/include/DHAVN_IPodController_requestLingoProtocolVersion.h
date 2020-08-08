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
#ifndef REQUESTLINGOPROTOCOLVERSION_H
#define REQUESTLINGOPROTOCOLVERSION_H


#include <DHAVN_IPodController_iAPPacket.h>

namespace GeneralLingo
{
	class CRequestLingoProtocolVersion: public CIAPPacket
	{
		public:

                        CRequestLingoProtocolVersion(int lingo,int transId, bool isTidEnabled);
			QByteArray marshall()        ;
			void  unmarshall(QByteArray)        ;
		private:
                        int m_ReqLingoId;
	};

}

#endif //REQUESTLINGOPROTOCOLVERSION_H
