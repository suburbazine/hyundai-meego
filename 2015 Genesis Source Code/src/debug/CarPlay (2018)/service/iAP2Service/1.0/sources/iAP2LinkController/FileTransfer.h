/*
    File:       FileTransfer.h
    Package:    LG iAP2 Service
    Author:     Yongil Park (yongil.park@lge.com)
    Copyright ¨Ï [2014-2016] by LG Electronics Inc.

    This program or software including the accompanying associated documentation
    (¡°Software¡±) is the proprietary software of LG Electronics Inc. and or its
    licensors, and may only be used, duplicated, modified or distributed pursuant
    to the terms and conditions of a separate written license agreement between you
    and LG Electronics Inc. (¡°Authorized License¡±). Except as set forth in an
    Authorized License, LG Electronics Inc. grants no license (express or implied),
    rights to use, or waiver of any kind with respect to the Software, and LG
    Electronics Inc. expressly reserves all rights in and to the Software and all
    intellectual property therein. If you have no Authorized License, then you have
    no rights to use the Software in any ways, and should immediately notify LG
    Electronics Inc. and discontinue all use of the Software.
*/

#ifndef FILETRANSFER_H_
#define FILETRANSFER_H_

#include "iAP2LinkRunLoop.h"
#include "DataDecoder.h"
#include "DataEncoder.h"
#include "Transport.h"
// #ifdef iAP2SERVICE_FILETRANSFER
#include "iAP2FileTransfer.h"
// #endif
#include <iAp2Marshal.h>
namespace IAP2
{
    
    class LinkController;
    class FileTransfer {

        public :             
            FileTransfer();
            ~FileTransfer();
            
        public :
            static BOOL iAP2FileTransferGotDataCB_Caller(struct iAP2FileTransfer_st* fileXfer, void* userInfo);
            static BOOL iAP2FileTransferEndCB_Caller(struct iAP2FileTransfer_st* fileXfer, void* userInfo);
            
            void createIAP2FileTransferCreate(LinkController* pLnkCtrler,uint8_t bufferID, ConnectionPath RecvPath, FILETRANSFERMODE UserInfo );
            void handleFileTransferUpdate(iAP2FileTransfer_st* fileXfer,FILETRANSFERMODE mFileTransferMode);
            //int handleiAP2FileTransferRequestCMD(LinkController* pLnkCtrler, uint8_t bufferID, ConnectionPath SendPath, FileTransferCMD eFileTransferCMD);

        private:    
            
            void decodeFileTransferUpdate(iAP2FileTransfer_st* fileXfer, FileTransferUpdateEvt* pEvt);
            int handleFileTransferUpdateEvent(LinkController* pLnkCtrler, FileTransferUpdateEvt* pEvent, ConnectionPath RecvPath);

    };


} // namespace IAP2

#endif // #ifndef NOWPLAYING_H_
