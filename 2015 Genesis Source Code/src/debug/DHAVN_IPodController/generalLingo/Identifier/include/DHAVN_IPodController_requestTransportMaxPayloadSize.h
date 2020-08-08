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
#ifndef REQUESTTRANSPORTMAXPAYLOADSIZE_H
#define REQUESTTRANSPORTMAXPAYLOADSIZE_H


#include <DHAVN_IPodController_iAPPacket.h>

namespace GeneralLingo
{
        class CRequestTransportMaxPayloadSize: public CIAPPacket
	{
		public:

                        CRequestTransportMaxPayloadSize(int transId, bool isTidEnabled);
			QByteArray marshall()        ;
			void  unmarshall(QByteArray)        ;

	};
}

#endif //REQUESTTRANSPORTMAXPAYLOADSIZE_H
