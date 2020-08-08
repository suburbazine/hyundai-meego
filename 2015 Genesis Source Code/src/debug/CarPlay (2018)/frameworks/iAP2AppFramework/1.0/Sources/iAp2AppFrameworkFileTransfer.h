/*
 *     iAp2AppFrameworkNowPlaying.h
 *
 *    Created on: 7/22, 2015
 *    Author: yongil park
 */

#ifndef IAP2APPFRAMEWORKFILETRANSFER_H_
#define IAP2APPFRAMEWORKFILETRANSFER_H_
#include <stdint.h>
#include <stddef.h>

#include "iAp2AppFrameworkCommon.h"
#include "iAp2AppFrameworkExternal.h"
#include "iAp2AppFrameworkExternal_command.h"
#include "iAp2AppFrameworkExternal_event.h"

#ifdef __cplusplus
extern "C" {
#endif

//===========================================================================================================================
//    Definitions
//===========================================================================================================================

/*!
 * @brief This function is callback to send iAp2AppFileTransferUpdate_f data to Application.
 * @param inContext handle value
 * @param inCbArg TransferAlbumArtwork data, Refer @ref FileTrnaferAlbumArtworkArg type
 * @return 0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
                                                                                
typedef int (*iAp2AppFileTransferUpdate_f)(void* inContext, FileTransferUpdateArg* inCbArg);


/*!
 * @brief     Send FileTransfer Cmd to Apple Device.
 * @param     inRef Application handle value  
 * @param   mFileTransferCMD is CMD to request about FileTransfer workding.
 * @param   bufferID is used to confirm FileTransfer session. 
 * @return     0 : No error occurred.\n -6700 ~ -6779 : Generic error codes.
 */
// int iAp2AppFileTransferRequestCMD(struct iAp2AppPrivate inRef, FileTransferCMD mFileTransferCMD, uint8_t bufferID);
 
int iAp2ConectionHandleEventFileTransferUpdate(iAp2AppRef inRef, unsigned char* pRxData);

#ifdef __cplusplus
}
#endif

#endif /* IAP2APPFRAMEWORKFILETRANSFER_H_ */
