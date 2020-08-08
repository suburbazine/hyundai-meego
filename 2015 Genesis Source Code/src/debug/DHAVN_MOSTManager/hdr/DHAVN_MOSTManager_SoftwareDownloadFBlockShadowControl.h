/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Shiva Kumar                                      *
 * DATE       :  06 Sep 2011                                      *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 * MODULE SUMMARY : MOST Manager is a middle ware component which *
 *                  talks to the applications and controller over *
 *                  QtDbus.The data sent over QtDbus is further   *
 *                  communicated to other MOST devices through ACS*
 *                  daemon.This module is also responsible for    *
 *                  commands from ACS daemon.                     *
 *                  This file contains the class definition for   *
 *                  Software download Fblock and Shadow control,  *
 *                  responsible to create instance of             *
 *                  Fblock(SWDL 0x01) and Shadows                 *
 *                  (SWDL 0x02 and SWDL 0x03) configure it and    *
 *                  add to most stack. Listeners and handleres    *
 *                  will be part of this class.                   *
 *                  Communicates with ACS daemon                  *
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER                   COMMENT              *
 ******************************************************************
 * 06 Sep 2011  Shiva Kumar              Draft version            *
 * 09 Sep 2011  Shiva Kumar              Changed the parameters of functions SetReadModuleVersion, SetSWDLProgressInfo
 *                                       and signals ReadModuleVersion, SWDLProgressInfo. Removed struct SReadNumofModules
 *                                       definition
 * 15 Sep 2011  Amit Koparde             Added Log_Tag define     *
 * 17 Jul 2012  Shiva Kumar              Modified for FCAT 1.18.2
 * 19 Sep 2012  Shiva Kumar              Modified for FCAT 1.19.0, 1.20.1, 1.20.2
 ******************************************************************
 */


#ifndef DHAVN_MOSTMANAGER_SOFTWAREDOWNLOADFBLOCKCONTROL_H
#define DHAVN_MOSTMANAGER_SOFTWAREDOWNLOADFBLOCKCONTROL_H

#include <QtCore>
#include <QtDBus/QtDBus>
#include <stdlib.h>
#include <sys/time.h>
#include <qfile.h>
#include <qtextstream.h>

#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_ACSApplication.h"
#include "DHAVN_MOSTManager_SoftwareDownloadFBlock.h"
#include "DHAVN_MOSTManager_SoftwareDownloadShadow.h"
#include "DHAVN_MOSTManager_MOSTStructures.h"

/** macros to identify device */
#define SWDLID_HU 1
#define SWDLID_IBOX 2
#define SWDLID_AMP 3

/** macros to identify state SET and GET*/
#define STATE_SET 1
#define STATE_GET 2

#define FILE_TRANSFER_AMS 2
#define FILE_TRANSFER_CMS 1



#define LOG_TAG_SWDLCONTROL "SoftwareDownloadControl"

//using namespace acs::generated::fblock::SoftwareDownload;
using namespace k2l::acs::protocols::most;


class CSWDLFBlockShadowControl;

/** Hanlder for Reprogramming mode and Listener for HWIdentifier From head unit */
class CRepmodeHandlerHWIdentifierListenerHU : public IFunctionBlockMethodHandler, public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLFBlockControlContext;
    SReporgrammingMode m_sreprogrammingmodeHU;
public:
    CRepmodeHandlerHWIdentifierListenerHU(CSWDLFBlockShadowControl *context);
    ~CRepmodeHandlerHWIdentifierListenerHU();
    /** Virtual functions of IFunctionBlockMethodHandler and IACSMOSTEventListener*/
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void OnChange();
};

/** Hanlder for Reprogramming mode and Listener for HWIdentifier From IBOX */
class CRepmodeHandlerHWIdentifierListenerIBOX : public IShadowMethodHandler, public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    SReporgrammingMode m_sreprogrammingmodeIbox;
public:
    CRepmodeHandlerHWIdentifierListenerIBOX(CSWDLFBlockShadowControl* context);
    ~CRepmodeHandlerHWIdentifierListenerIBOX();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void OnChange();
};
/** Hanlder for Reprogramming mode and Listener for HWIdentifier From Amplifier */
class CRepmodeHandlerHWIdentifierListenerAMP : public IShadowMethodHandler, public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    SReporgrammingMode m_sreprogrammingmodeAmp;
public:
    CRepmodeHandlerHWIdentifierListenerAMP(CSWDLFBlockShadowControl* context);
    ~CRepmodeHandlerHWIdentifierListenerAMP();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void OnChange();
};
/** Handler for InitUpdate and Listener for DeviceName From Head Unit */
class CIUpdateHandlerDevNameListenerHU : public IFunctionBlockMethodHandler, public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLFBlockControlContext;
    SInitUpdate m_sinitupdateHU;
public:
    CIUpdateHandlerDevNameListenerHU(CSWDLFBlockShadowControl *context);
    ~CIUpdateHandlerDevNameListenerHU();
    /** Virtual functions of IFunctionBlockMethodHandler and IACSMOSTEventListener*/
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void OnChange();
};
/** Handler for InitUpdate and Listener for DeviceName From IBOX */
class CIUpdateHandlerDevNameListenerIBOX: public IShadowMethodHandler, public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    SInitUpdate m_sinitupdateIbox;
public:
    CIUpdateHandlerDevNameListenerIBOX(CSWDLFBlockShadowControl* context);
    ~CIUpdateHandlerDevNameListenerIBOX();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void OnChange();
};
/** Handler for InitUpdate and Listener for DeviceName From Amplifier */
class CIUpdateHandlerDevNameListenerAMP: public IShadowMethodHandler, public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    SInitUpdate m_sinitupdateAmp;
public:
    CIUpdateHandlerDevNameListenerAMP(CSWDLFBlockShadowControl* context);
    ~CIUpdateHandlerDevNameListenerAMP();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void OnChange();
};
/** Handler for EraseMemory and Listener for HWversion From Head Uint */
class CEraseMemoryHandlerHWVersionListenerHU : public IFunctionBlockMethodHandler, public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLFBlockControlContext;
    SEraseMemory m_serasememoryHU;
public:
    CEraseMemoryHandlerHWVersionListenerHU(CSWDLFBlockShadowControl *context);
    ~CEraseMemoryHandlerHWVersionListenerHU();
    /** Virtual functions of IFunctionBlockMethodHandler and IACSMOSTEventListener*/
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void OnChange();
};
/** Handler for EraseMemory and Listener for HWversion From IBOX */
class CEraseMemoryHandlerHWVersionListenerIBOX : public IShadowMethodHandler, public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    SEraseMemory m_serasememoryIbox;
public:
    CEraseMemoryHandlerHWVersionListenerIBOX(CSWDLFBlockShadowControl* context);
    ~CEraseMemoryHandlerHWVersionListenerIBOX();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void OnChange();
};
/** Handler for EraseMemory and Listener for HWversion From Amplifier */
class CEraseMemoryHandlerHWVersionListenerAMP : public IShadowMethodHandler, public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    SEraseMemory m_serasememoryAmp;
public:
    CEraseMemoryHandlerHWVersionListenerAMP(CSWDLFBlockShadowControl* context);
    ~CEraseMemoryHandlerHWVersionListenerAMP();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void OnChange();
};
/** Handler for Request Download  and Listener for NumberofModule From Head Unit */
class CRDownloadHandlerNumofModuleListenerHU : public IFunctionBlockMethodHandler, public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLFBlockControlContext;
    SRequestDownload m_srequestdownloadHU;
public:
    CRDownloadHandlerNumofModuleListenerHU(CSWDLFBlockShadowControl *context);
    ~CRDownloadHandlerNumofModuleListenerHU();
    /** Virtual functions of IFunctionBlockMethodHandler and IACSMOSTEventListener*/
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void OnChange();
};
/** Handler for Request Download  and Listener for NumberofModule From IBOX */
class CRDownloadHandlerNumofModuleListenerIBOX : public IShadowMethodHandler, public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    SRequestDownload m_srequestdownloadIbox;
public:
    CRDownloadHandlerNumofModuleListenerIBOX(CSWDLFBlockShadowControl* context);
    ~CRDownloadHandlerNumofModuleListenerIBOX();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void OnChange();
};
/** Handler for Request Download  and Listener for NumberofModule From Amplifier */
class CRDownloadHandlerNumofModuleListenerAMP : public IShadowMethodHandler, public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    SRequestDownload m_srequestdownloadAmp;
public:
    CRDownloadHandlerNumofModuleListenerAMP(CSWDLFBlockShadowControl* context);
    ~CRDownloadHandlerNumofModuleListenerAMP();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void OnChange();
};
/** Handler for PrepareFileTransfer  and Listener for Moduleversion From Head Unit */
class CPFTransferHandlerModVersionListenerHU : public IFunctionBlockMethodHandler, public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLFBlockControlContext;
    SPrepareFileTransfer m_spreparefiletransferHU;
public:
    CPFTransferHandlerModVersionListenerHU(CSWDLFBlockShadowControl *context);
    ~CPFTransferHandlerModVersionListenerHU();
    /** Virtual functions of IFunctionBlockMethodHandler and IACSMOSTEventListener*/
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void OnChange();
};
/** Handler for PrepareFileTransfer  and Listener for Moduleversion From IBOX */
class CPFTransferHandlerModVersionListenerIBOX : public IShadowMethodHandler, public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    SPrepareFileTransfer m_spreparefiletransferIbox;
public:
    CPFTransferHandlerModVersionListenerIBOX(CSWDLFBlockShadowControl* context);
    ~CPFTransferHandlerModVersionListenerIBOX();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void OnChange();
};
/** Handler for PrepareFileTransfer  and Listener for Moduleversion From Amplifier */
class CPFTransferHandlerModVersionListenerAMP : public IShadowMethodHandler, public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    SPrepareFileTransfer m_spreparefiletransferAmp;
public:
    CPFTransferHandlerModVersionListenerAMP(CSWDLFBlockShadowControl* context);
    ~CPFTransferHandlerModVersionListenerAMP();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void OnChange();
};
/** Hanlder for TransferDataCMS From Head Unit */
class CTransferDataCMSHandlerHU : public IFunctionBlockMethodHandler
{

    CSWDLFBlockShadowControl *m_SWDLFBlockControlContext;
    STransferDataCMSAMS m_stransferdataCMSHU;
public:
    CTransferDataCMSHandlerHU(CSWDLFBlockShadowControl *context);
    ~CTransferDataCMSHandlerHU();
    /** Virtual functions of IFunctionBlockMethodHandler and IACSMOSTEventListener*/
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};
/** Hanlder for TransferDataCMS From IBOX */
class CTransferDataCMSHandlerIBOX : public IShadowMethodHandler
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    STransferDataCMSAMS m_stransferdataCMSIbox;
public:
    CTransferDataCMSHandlerIBOX(CSWDLFBlockShadowControl* context);
    ~CTransferDataCMSHandlerIBOX();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};
/** Hanlder for TransferDataCMS From Amplifier */
class CTransferDataCMSHandlerAMP : public IShadowMethodHandler
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    STransferDataCMSAMS m_stransferdataCMSAmp;
public:
    CTransferDataCMSHandlerAMP(CSWDLFBlockShadowControl* context);
    ~CTransferDataCMSHandlerAMP();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};
/** Hanlder for TransferDataAMS From Head Unit */
class CTransferDataAMSHandlerHU : public IFunctionBlockMethodHandler
{

    CSWDLFBlockShadowControl *m_SWDLFBlockControlContext;
    STransferDataCMSAMS m_stransferdataAMSHU;
public:
    CTransferDataAMSHandlerHU(CSWDLFBlockShadowControl *context);
    ~CTransferDataAMSHandlerHU();
    /** Virtual functions of IFunctionBlockMethodHandler and IACSMOSTEventListener*/
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};
/** Hanlder for TransferDataAMS From IBOX */
class CTransferDataAMSHandlerIBOX : public IShadowMethodHandler
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    STransferDataCMSAMS m_stransferdataAMSIbox;
    USE_LOG_UTIL

    //struct timeval tval;
    //struct timezone tzone;
    //struct tm *timem;
public:
    CTransferDataAMSHandlerIBOX(CSWDLFBlockShadowControl* context);
    ~CTransferDataAMSHandlerIBOX();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};
/** Hanlder for TransferDataAMS From Amplifier */
class CTransferDataAMSHandlerAMP : public IShadowMethodHandler
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    STransferDataCMSAMS m_stransferdataAMSAmp;
public:
    CTransferDataAMSHandlerAMP(CSWDLFBlockShadowControl* context);
    ~CTransferDataAMSHandlerAMP();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};
/** Hanlder for CheckActivateModule From Head Unit */
class CCheckActivateModuleHandlerHU : public IFunctionBlockMethodHandler
{

    CSWDLFBlockShadowControl *m_SWDLFBlockControlContext;
    SCheckActivateModule m_scheckactivemoduleHU;
public:
    CCheckActivateModuleHandlerHU(CSWDLFBlockShadowControl *context);
    ~CCheckActivateModuleHandlerHU();
    /** Virtual functions of IFunctionBlockMethodHandler and IACSMOSTEventListener*/
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};
/** Hanlder for CheckActivateModule From IBOX */
class CCheckActivateModuleHandlerIBOX : public IShadowMethodHandler
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    SCheckActivateModule m_scheckactivemoduleIbox;
public:
    CCheckActivateModuleHandlerIBOX(CSWDLFBlockShadowControl* context);
    ~CCheckActivateModuleHandlerIBOX();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};
/** Hanlder for CheckActivateModule From Amplifier */
class CCheckActivateModuleHandlerAMP : public IShadowMethodHandler
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    SCheckActivateModule m_scheckactivemoduleAmp;
public:
    CCheckActivateModuleHandlerAMP(CSWDLFBlockShadowControl* context);
    ~CCheckActivateModuleHandlerAMP();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};
/** Hanlder for RequestTransferExit From Head Unit */
class CRequestTransferExitHandlerHU : public IFunctionBlockMethodHandler
{

    CSWDLFBlockShadowControl *m_SWDLFBlockControlContext;
    SRequestTransferExit m_srequesttransferexitHU;
public:
    CRequestTransferExitHandlerHU(CSWDLFBlockShadowControl *context);
    ~CRequestTransferExitHandlerHU();
    /** Virtual functions of IFunctionBlockMethodHandler and IACSMOSTEventListener*/
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};
/** Hanlder for RequestTransferExit From IBOX */
class CRequestTransferExitHandlerIBOX : public IShadowMethodHandler
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    SRequestTransferExit m_srequesttransferexitIbox;
public:
    CRequestTransferExitHandlerIBOX(CSWDLFBlockShadowControl* context);
    ~CRequestTransferExitHandlerIBOX();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};
/** Hanlder for RequestTransferExit From Amplifier */
class CRequestTransferExitHandlerAMP : public IShadowMethodHandler
{

    CSWDLFBlockShadowControl *m_SWDLShadowControlContext;
    SRequestTransferExit m_srequesttransferexitAmp;
public:
    CRequestTransferExitHandlerAMP(CSWDLFBlockShadowControl* context);
    ~CRequestTransferExitHandlerAMP();
    /** Virtual functions of IShadowMethodHandler and IACSMOSTEventListener*/
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};
/** Listener for property SWDLProgressInfo of IBOX*/
class CSWDLProgressInfoListenerIBOX : public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLFBlockControlContext;
 public:
    CSWDLProgressInfoListenerIBOX(CSWDLFBlockShadowControl *context);
    ~CSWDLProgressInfoListenerIBOX();
    virtual void OnChange();
};
/** Listener for property SWDLProgressInfo of Amplifier*/
class CSWDLProgressInfoListenerAMP : public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLFBlockControlContext;
 public:
    CSWDLProgressInfoListenerAMP(CSWDLFBlockShadowControl *context);
    ~CSWDLProgressInfoListenerAMP();
    virtual void OnChange();
};
/** Listener for property UpdateProgressingInfo of IBOX*/
class CUpdateProgressingInfoListenerIBOX : public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLFBlockControlContext;
 public:
    CUpdateProgressingInfoListenerIBOX(CSWDLFBlockShadowControl *context);
    ~CUpdateProgressingInfoListenerIBOX();
    virtual void OnChange();
};
/** Listener for property UpdateProgressingInfo of Amplifier*/
class CUpdateProgressingInfoListenerAMP : public IACSMOSTEventListener
{

    CSWDLFBlockShadowControl *m_SWDLFBlockControlContext;
 public:
    CUpdateProgressingInfoListenerAMP(CSWDLFBlockShadowControl *context);
    ~CUpdateProgressingInfoListenerAMP();
    virtual void OnChange();
};


/** Listener for checking visibility of iBox SWDL FBlock */
// iBox SWDL FBlock checkling for iBox Update, 2014.2.24
class CIBOXSWDLFblockStateListener : public QObject, public IACSMOSTEventListener
{
    Q_OBJECT

    QTimer *m_Timer;
	acs::generated::shadow::SoftwareDownload::CSoftwareDownload *m_SoftwareDownload;
	CSWDLFBlockShadowControl *m_SWDLFBlockShadowControl;

    USE_LOG_UTIL
    //struct timeval tv;

public:
    CIBOXSWDLFblockStateListener(acs::generated::shadow::SoftwareDownload::CSoftwareDownload *SoftwareDownload, CSWDLFBlockShadowControl* context);
    virtual void OnChange();

private slots:
    void OnWaitForTimeout();
};
//==========================================

class CSWDLFBlockShadowControl : public QObject, public CControlThreadBase
{
    Q_OBJECT

private:

    /** Instance of Software download(0x01) on Head unit side hence Function Block */
    acs::generated::fblock::SoftwareDownload::CSoftwareDownload *m_SWDLFBlockHU;
     /** Instance of Software download(0x02) on IBOX side hence shadow Block */
    acs::generated::shadow::SoftwareDownload::CSoftwareDownload *m_SWDLShadowIbox;
    /** Instance of Software download(0x03) on Amplifier side hence shadow Block */
    acs::generated::shadow::SoftwareDownload::CSoftwareDownload *m_SWDLShadowAmp;

    /** Instances of properties ,handlers and Listeners of software download on HU, IBOX and Amplifier */
    acs::generated::fblock::SoftwareDownload::CReprogrammingMode *m_ReprogrammingModeHU;
    acs::generated::shadow::SoftwareDownload::CReprogrammingMode *m_ReprogrammingModeIbox;
    acs::generated::shadow::SoftwareDownload::CReprogrammingMode *m_ReprogrammingModeAmp;
    acs::generated::fblock::SoftwareDownload::CReadHWIdentifier* m_ReadHWIdentifierHU;
    acs::generated::shadow::SoftwareDownload::CReadHWIdentifier* m_ReadHWIdentifierIbox;
    acs::generated::shadow::SoftwareDownload::CReadHWIdentifier* m_ReadHWIdentifierAmp;
    CRepmodeHandlerHWIdentifierListenerHU *m_RepmodeHandlerHWIdentifierListenerHU;
    CRepmodeHandlerHWIdentifierListenerIBOX *m_RepmodeHandlerHWIdentifierListenerIbox;
    CRepmodeHandlerHWIdentifierListenerAMP *m_RepmodeHandlerHWIdentifierListenerAmp;

    acs::generated::fblock::SoftwareDownload::CInitUpdate* m_InitUpdateHU;
    acs::generated::shadow::SoftwareDownload::CInitUpdate* m_InitUpdateIbox;
    acs::generated::shadow::SoftwareDownload::CInitUpdate* m_InitUpdateAmp;
    acs::generated::fblock::SoftwareDownload::CReadDevName* m_ReadDevNameHU;
    acs::generated::shadow::SoftwareDownload::CReadDevName* m_ReadDevNameIbox;
    acs::generated::shadow::SoftwareDownload::CReadDevName* m_ReadDevNameAmp;
    CIUpdateHandlerDevNameListenerHU* m_IUpdateHandlerDevNameListenerHU;
    CIUpdateHandlerDevNameListenerIBOX* m_IUpdateHandlerDevNameListenerIbox;
    CIUpdateHandlerDevNameListenerAMP* m_IUpdateHandlerDevNameListenerAmp;

    acs::generated::fblock::SoftwareDownload::CEraseMemory* m_EraseMemoryHU;
    acs::generated::shadow::SoftwareDownload::CEraseMemory* m_EraseMemoryIbox;
    acs::generated::shadow::SoftwareDownload::CEraseMemory* m_EraseMemoryAmp;
    acs::generated::fblock::SoftwareDownload::CReadHWVersion* m_ReadHWVersionHU;
    acs::generated::shadow::SoftwareDownload::CReadHWVersion* m_ReadHWVersionIbox;
    acs::generated::shadow::SoftwareDownload::CReadHWVersion* m_ReadHWVersionAmp;
    CEraseMemoryHandlerHWVersionListenerHU* m_EraseMemoryHandlerHWVersionListenerHU;
    CEraseMemoryHandlerHWVersionListenerIBOX* m_EraseMemoryHandlerHWVersionListenerIbox;
    CEraseMemoryHandlerHWVersionListenerAMP* m_EraseMemoryHandlerHWVersionListenerAmp;

    acs::generated::fblock::SoftwareDownload::CRequestDownload* m_RequestDownloadHU;
    acs::generated::shadow::SoftwareDownload::CRequestDownload* m_RequestDownloadIbox;
    acs::generated::shadow::SoftwareDownload::CRequestDownload* m_RequestDownloadAmp;
    acs::generated::fblock::SoftwareDownload::CReadNumberOfModule* m_ReadNumberofModulesHU;
    acs::generated::shadow::SoftwareDownload::CReadNumberOfModule* m_ReadNumberofModulesIbox;
    acs::generated::shadow::SoftwareDownload::CReadNumberOfModule* m_ReadNumberofModulesAmp;
    CRDownloadHandlerNumofModuleListenerHU* m_RDownloadHandlerNumofmoduleListenerHU;
    CRDownloadHandlerNumofModuleListenerIBOX* m_RDownloadHandlerNumofmoduleListenerIbox;
    CRDownloadHandlerNumofModuleListenerAMP* m_RDownloadHandlerNumofmoduleListenerAmp;


    acs::generated::fblock::SoftwareDownload::CPrepareFileTransfer* m_PrepareFileTransferHU;
    acs::generated::shadow::SoftwareDownload::CPrepareFileTransfer* m_PrepareFileTransferIbox;
    acs::generated::shadow::SoftwareDownload::CPrepareFileTransfer* m_PrepareFileTransferAmp;
    acs::generated::fblock::SoftwareDownload::CReadModuleVersion* m_ReadModuleVersionHU;
    acs::generated::shadow::SoftwareDownload::CReadModuleVersion* m_ReadModuleVersionIbox;
    acs::generated::shadow::SoftwareDownload::CReadModuleVersion* m_ReadModuleVersionAmp;
    CPFTransferHandlerModVersionListenerHU* m_PFTransferHandlerModeVersionListenerHU;
    CPFTransferHandlerModVersionListenerIBOX* m_PFTransferHandlerModeVersionListenerIbox;
    CPFTransferHandlerModVersionListenerAMP* m_PFTransferHandlerModeVersionListenerAmp;

    acs::generated::fblock::SoftwareDownload::CTransferDataCMS* m_TransferDataCMSHU;
    acs::generated::shadow::SoftwareDownload::CTransferDataCMS* m_TransferDataCMSIbox;
    acs::generated::shadow::SoftwareDownload::CTransferDataCMS* m_TransferDataCMSAmp;
    CTransferDataCMSHandlerHU* m_TransferDataCMSHandlerHU;
    CTransferDataCMSHandlerIBOX* m_TransferDataCMSHandlerIbox;
    CTransferDataCMSHandlerAMP* m_TransferDataCMSHandlerAmp;

    acs::generated::fblock::SoftwareDownload::CTransferDataAMS* m_TransferDataAMSHU;
    acs::generated::shadow::SoftwareDownload::CTransferDataAMS* m_TransferDataAMSIbox;
    acs::generated::shadow::SoftwareDownload::CTransferDataAMS* m_TransferDataAMSAmp;
    CTransferDataAMSHandlerHU* m_TransferDataAMSHandlerHU;
    CTransferDataAMSHandlerIBOX* m_TransferDataAMSHandlerIbox;
    CTransferDataAMSHandlerAMP* m_TransferDataAMSHandlerAmp;

    acs::generated::fblock::SoftwareDownload::CCheckActivateModule* m_CheckActivateModuleHU;
    acs::generated::shadow::SoftwareDownload::CCheckActivateModule* m_CheckActivateModuleIbox;
    acs::generated::shadow::SoftwareDownload::CCheckActivateModule* m_CheckActivateModuleAmp;
    CCheckActivateModuleHandlerHU* m_CheckActivateModuleHandlerHU;
    CCheckActivateModuleHandlerIBOX* m_CheckActivateModuleHandlerIbox;
    CCheckActivateModuleHandlerAMP* m_CheckActivateModuleHandlerAmp;

    acs::generated::fblock::SoftwareDownload::CRequestTransferExit* m_RequestTransferExitHU;
    acs::generated::shadow::SoftwareDownload::CRequestTransferExit* m_RequestTransferExitIbox;
    acs::generated::shadow::SoftwareDownload::CRequestTransferExit* m_RequestTransferExitAmp;
    CRequestTransferExitHandlerHU* m_RequestTransferExitHandlerHU;
    CRequestTransferExitHandlerIBOX* m_RequestTransferExitHandlerIbox;
    CRequestTransferExitHandlerAMP* m_RequestTransferExitHandlerAmp;


    acs::generated::fblock::SoftwareDownload::CSWDLProgressInfo* m_SWDLProgressInfoHU;
    acs::generated::shadow::SoftwareDownload::CSWDLProgressInfo* m_SWDLProgressInfoIbox;
    acs::generated::shadow::SoftwareDownload::CSWDLProgressInfo* m_SWDLProgressInfoAmp;
    CSWDLProgressInfoListenerIBOX* m_SWDLProgressInfoListenerIbox;
    CSWDLProgressInfoListenerAMP* m_SWDLProgressInfoListenerAmp;

    acs::generated::fblock::SoftwareDownload::CUpdateProcessingInfo *m_UpdateProgressingInfoHU;
    acs::generated::shadow::SoftwareDownload::CUpdateProcessingInfo *m_UpdateProgressingInfoIBOX;
    acs::generated::shadow::SoftwareDownload::CUpdateProcessingInfo *m_UpdateProgressingInfoAmp;
    CUpdateProgressingInfoListenerIBOX *m_UpdateProgressingInfoListenerIBOX;
    CUpdateProgressingInfoListenerAMP *m_UpdateProgressingInfoListenerAmp;

	// iBox SWDL FBlock checkling for iBox Update, 2014.2.24
	CIBOXSWDLFblockStateListener *m_IBOXSWDLFblockStateListener;

    //struct timeval tval;
    //struct timezone tzone;
    //struct tm *timem;

    bool m_fileTransferIBOX;
    SFileInfoforSWDL fileTransferInfo;

    QFile *m_IBOXUpgradefile;
    uint m_blockCounter;
    QByteArray m_filearray;
    bool m_EndOfFile;

public:

    CSWDLFBlockShadowControl(CMarshaledACSApplication* context);
    ~CSWDLFBlockShadowControl();

    virtual bool Initialize();
    virtual bool Register();
    virtual bool UnRegister();

    bool FBlockVisible(const uint deviceID);

    void SetHandlerListener();

    void RemoveListener();

    /** Methods invoked by the Adaptor API's */
    bool SetReadHWIdentifier(const uint swdlid, const uint state, const QString hwidentifier);

    bool SetReadDevName(const uint swdlid, const QString deviceName);

    bool SetReadHWVersion(const uint swdlid, const QString hwversion);

    bool SetReadNumberofModule(const uint swdlid, QByteArray modulevalue);

    bool SetReadModuleVersion(const uint swdlid, const QDBusVariant &moduleversion);

    bool SetSWDLProgressInfo(const uint swdlOpMode, const uint swdlProgress);

    bool SetReprogrammingMode(const SReporgrammingMode &reprogrammingmode_wr);

    bool SetInitUpdateResult(const SInitUpdate &sinitupdate_wr);

    bool SetEraseMemory(const SEraseMemory &serasememory_wr);

    bool SetRequestDownload(const SRequestDownload &srequestdownload_wr);

    bool SetPrepareFileTransfer(const SPrepareFileTransfer &spreparefiletransfer_wr);

    bool SetPrepareFileTransferIBOX(const SFileInfoforSWDL &fileInfo_wr);

    bool SetTransferDataCMS(const STransferDataCMSAMS &stransferdataCMS_wr);

    bool SetTransferDataAMS(const STransferDataCMSAMS &stransferdataAMS_wr);

    bool SetCheckActivateModule(const SCheckActivateModule &checkactivationmodule_wr);

    bool SetRequestTransferExit(const SRequestTransferExit &requesttransferexit_wr);

    bool SetUpdateProgressingInfo(const uint updateStatus);

    /** Methods invoked by handlers and listeners */
    void ReadHWIdentifier(const uint swdlid);

    void ReadDevName(const uint swdlid);

    void ReadHWVersion(const uint swdlid);

    void ReadNumberofModule(const uint swdlid);

    void ReadModuleVersion(const uint swdlid);

    void ReprogrammingMode(const uint32_t payloadLength, const uint8_t *payload, SReporgrammingMode &sreprogrammingmode);

    void InitUpdateResult(const uint32_t payloadLength, const uint8_t *payload, SInitUpdate &sinitupdate);

    void EraseMemory(const uint32_t payloadLength, const uint8_t *payload, SEraseMemory &serasememory);

    void RequestDownload(const uint32_t payloadLength, const uint8_t *payload, SRequestDownload &srequestdownload);

    void PrepareFileTransfer(const uint32_t payloadLength, const uint8_t *payload, SPrepareFileTransfer &spreparefiletransfer);

    void TransferDataCMS(const uint32_t payloadLength, const uint8_t *payload, STransferDataCMSAMS &stransferdataCMS);

    void TransferDataAMS(const uint32_t payloadLength, const uint8_t *payload, STransferDataCMSAMS &stransfersdataAMS);

    void CheckActivateModule(const uint32_t payloadLength, const uint8_t *payload, SCheckActivateModule &scheckactivemodule);

    void RequestTransferExit(const uint32_t payloadLength, const uint8_t *payload, SRequestTransferExit &srequesttransferexit);

    void SWDLProgressInfo(const uint swdlid);

    void UpdateProgressingInfoStatusSlot(const uint swdlid);

    void TransferFileIBOXAMSCMS();

	// iBox SWDL FBlock checkling for iBox Update, 2014.2.24
    void IBOXSWDLFblockStateListener(const bool state);


signals:
    /** Signals sent to SWDLAdaptor */
    void ReadHWIdentifier(const uint swdlid, const QString hwidentifier);

    void ReadDevName(const uint swdlid, const QString deviceName);

    void ReadHWVersion(const uint swdlid, const QString hwversion);

    void ReadNumberofModule(const uint swdlid, const QByteArray modulevalue);

    void ReadModuleVersion(const uint swdlid, const QDBusVariant &moduleversion);

    void ReprogrammingMode(const QDBusVariant &reprogrammingmode);

    void InitUpdateResult(const QDBusVariant &initUpdate);

    void EraseMemory(const QDBusVariant &eraseMemory);

    void RequestDownload(const QDBusVariant &requestdownload);

    void PrepareFileTransfer(const QDBusVariant &PrepareFileTransfer);

    void TransferDataCMS(const QDBusVariant &transferdataAMS);

    void TransferDataAMS(const QDBusVariant &transferdataCMS);
    void TransferDataAMSError(const uint swdlid, const uchar errorCode, const QByteArray errorInfo);

    void CheckActivateModule(const QDBusVariant &checkactivatemodule);

    void RequestTransferExit(const QDBusVariant &requesttransferexit);

    void SWDLProgressInfo(const uint swdlid, const uint swdlOpMode, const uint swdlProgress);

    void UpdateProgressingInfoStatus(const uint swdlid, const uint updateStatus);

	// iBox SWDL FBlock checkling for iBox Update, 2014.2.24
	void iBoxSWDLFBlockVisible(const bool state);
    
};

#endif // DHAVN_MOSTMANAGER_SOFTWAREDOWNLOADFBLOCKCONTROL_H
