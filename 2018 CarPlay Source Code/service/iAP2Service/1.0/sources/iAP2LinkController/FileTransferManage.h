/*
    File:       FileTransferManage.h
    Package:    LG iAP2 Service
    Author:     Yongil Park (yongil.park@lge.com)
    Copyright © [2014-2016] by LG Electronics Inc.

    This program or software including the accompanying associated documentation
    (“Software”) is the proprietary software of LG Electronics Inc. and or its
    licensors, and may only be used, duplicated, modified or distributed pursuant
    to the terms and conditions of a separate written license agreement between you
    and LG Electronics Inc. (“Authorized License”). Except as set forth in an
    Authorized License, LG Electronics Inc. grants no license (express or implied),
    rights to use, or waiver of any kind with respect to the Software, and LG
    Electronics Inc. expressly reserves all rights in and to the Software and all
    intellectual property therein. If you have no Authorized License, then you have
    no rights to use the Software in any ways, and should immediately notify LG
    Electronics Inc. and discontinue all use of the Software.
*/

#ifndef FILETRANSFERMANAGE_H_
#define FILETRANSFERMANAGE_H_

#include "iAP2LinkRunLoop.h"
//#include "Certificate.h"
#include "DataDecoder.h"
#include "DataEncoder.h"
#include "Transport.h"
#include "utils.h"
#include "Connection.h"
#include <pthread.h>
#include <iAP2FileTransfer.h>

namespace IAP2
{

    struct stFileTransferInform {
                ConnectionPath connPath;
                uint8_t bufferID;
                iAP2FileTransfer_t* m_piAP2FileTransfer;
                FILETRANSFERMODE mFiletransferMode;
    };    
    class FileTransferManage
    {
        public :    
            dnode_t* pFileTransferManageQHead;
            dnode_t* pFileTransferManageQTail;
            pthread_mutex_t FileTransferManageMutex;
            
        public:
             FileTransferManage();
                ~FileTransferManage();

            int RegisterFileTransferInfo(stFileTransferInform* mstFileTransferInform);            
            int UnRegisterFileTransferInfo(stFileTransferInform* mstFileTransferInform);            
            stFileTransferInform* searchDatafrombufferID(uint8_t buuferID);
            stFileTransferInform* searchDatafromiAP2FileTransfer(iAP2FileTransfer_t* piAP2FileTransfer);
            int searchDatafromConnPathAndUnRegisterFileTransfer(ConnectionPath connPath);

    };


} // namespace IAP2

#endif // #ifndef FILETRANSFERMANAGE_H_


