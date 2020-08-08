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
 * DATE         PROGRAMMER               COMMENT                  *
 ******************************************************************
 * 06 Sep 2011  Shiva Kumar              Draft version            *
 * 06 Sep 2011  Shiva Kumar              Changed the definition of functions SetSWDLProgressInfo,
 *                                       SetReadNumberofModule, ReadNumberofModule and SWDLProgressInfo.
 * 15 Sep 2011  Amit Koparde             Added Debug Statement    *
 * 12 Jan 2012  Shiva Kumar              Added LOG Statements     *
 * 25 Jan 2012  Shiva Kumar              Initialized all structure elements of listeners. *
 * 06 Feb 2012  Shiva Kumar              Updated QByteArray data extraction
 * 08 Feb 2011  Neelima Khedad           Using the latest Version of LOG_INIT function Logger module
 * 17 Jul 2012  Shiva Kumar              Modified for FCAT 1.18.2
 * 19 Sep 2012  Shiva Kumar              Modified for FCAT 1.19.0, 1.20.1, 1.20.2
 ******************************************************************
 */

#include "DHAVN_MOSTManager_SoftwareDownloadFBlockShadowControl.h"
#include "MOSTErrorInfo.h"

bool CheckTransferExit = false;     // hyunyong.sung, 2014.3.12 for re-transfer
/*
 *******************************************************************
 * Function:CSWDLFBlockShadowControl
 *
 * Description :This is constructor of CSWDLFBlockShadowControl class.
 *              Creates instances of all defined parameters
 *
 * Parameters :
 *    context    rw   It holds the instance of CMarshaledACSApplication.
 *
 * Returns : none
 *
 *******************************************************************
 */

CSWDLFBlockShadowControl::CSWDLFBlockShadowControl(CMarshaledACSApplication* context)
    : CControlThreadBase(context, LOG_TAG_SWDLCONTROL)
{

    LOG_INIT_V2(LOG_PREFIX_MOST)

    /** SWDL for Head Unit */
    m_SWDLFBlockHU = new acs::generated::fblock::SoftwareDownload::CSoftwareDownload(0x01);
    /** SWDL for IBOX */
    m_SWDLShadowIbox = new acs::generated::shadow::SoftwareDownload::CSoftwareDownload(0x02);
    /** SWDL for Amplifier */
    m_SWDLShadowAmp = new acs::generated::shadow::SoftwareDownload::CSoftwareDownload(0x03);

    /** Handlers and Listeners to Reprogramming mode and HWIdentifier for HU,IBOX and AMP*/
    m_RepmodeHandlerHWIdentifierListenerHU = new CRepmodeHandlerHWIdentifierListenerHU(this);
    m_RepmodeHandlerHWIdentifierListenerIbox = new CRepmodeHandlerHWIdentifierListenerIBOX(this);
    m_RepmodeHandlerHWIdentifierListenerAmp = new CRepmodeHandlerHWIdentifierListenerAMP(this);

    /** Handlers and Listeners to InitUpdate and Devicename for HU,IBOX and AMP*/
    m_IUpdateHandlerDevNameListenerHU = new CIUpdateHandlerDevNameListenerHU(this);
    m_IUpdateHandlerDevNameListenerIbox = new CIUpdateHandlerDevNameListenerIBOX(this);
    m_IUpdateHandlerDevNameListenerAmp = new CIUpdateHandlerDevNameListenerAMP(this);

    /** Handlers and Listeners to Erase Memory and HWVersion for HU,IBOX and AMP*/
    m_EraseMemoryHandlerHWVersionListenerHU = new CEraseMemoryHandlerHWVersionListenerHU(this);
    m_EraseMemoryHandlerHWVersionListenerIbox = new CEraseMemoryHandlerHWVersionListenerIBOX(this);
    m_EraseMemoryHandlerHWVersionListenerAmp = new CEraseMemoryHandlerHWVersionListenerAMP(this);

    /** Handlers and Listeners to Request Download and NumberofModules for HU,IBOX and AMP*/
    m_RDownloadHandlerNumofmoduleListenerHU = new CRDownloadHandlerNumofModuleListenerHU(this);
    m_RDownloadHandlerNumofmoduleListenerIbox = new CRDownloadHandlerNumofModuleListenerIBOX(this);
    m_RDownloadHandlerNumofmoduleListenerAmp = new CRDownloadHandlerNumofModuleListenerAMP(this);

    /** Handlers and Listeners to PrepareFileTransfer and mode version for HU,IBOX and AMP*/
    m_PFTransferHandlerModeVersionListenerHU = new CPFTransferHandlerModVersionListenerHU(this);
    m_PFTransferHandlerModeVersionListenerIbox = new CPFTransferHandlerModVersionListenerIBOX(this);
    m_PFTransferHandlerModeVersionListenerAmp = new CPFTransferHandlerModVersionListenerAMP(this);

    /** Handlers to TransferDataCMS for HU,IBOX and AMP*/
    m_TransferDataCMSHandlerHU = new CTransferDataCMSHandlerHU(this);
    m_TransferDataCMSHandlerIbox = new CTransferDataCMSHandlerIBOX(this);
    m_TransferDataCMSHandlerAmp = new CTransferDataCMSHandlerAMP(this);

    /** Handlers to TransferDataAMS for HU,IBOX and AMP*/
    m_TransferDataAMSHandlerHU = new CTransferDataAMSHandlerHU(this);
    m_TransferDataAMSHandlerIbox = new CTransferDataAMSHandlerIBOX(this);
    m_TransferDataAMSHandlerAmp = new CTransferDataAMSHandlerAMP(this);

    /** Handlers to Checkactivatemodule for HU,IBOX and AMP*/
    m_CheckActivateModuleHandlerHU = new CCheckActivateModuleHandlerHU(this);
    m_CheckActivateModuleHandlerIbox = new CCheckActivateModuleHandlerIBOX(this);
    m_CheckActivateModuleHandlerAmp = new CCheckActivateModuleHandlerAMP(this);

    /** Handlers to requesttransferexit for HU,IBOX and AMP*/
    m_RequestTransferExitHandlerHU = new CRequestTransferExitHandlerHU(this);
    m_RequestTransferExitHandlerIbox = new CRequestTransferExitHandlerIBOX(this);
    m_RequestTransferExitHandlerAmp = new CRequestTransferExitHandlerAMP(this);

    /** Listeners to SWDLProgressInfo for IBOX and AMP*/
    m_SWDLProgressInfoListenerIbox = new CSWDLProgressInfoListenerIBOX(this);
    m_SWDLProgressInfoListenerAmp = new CSWDLProgressInfoListenerAMP(this);

    /** Listeners to UpdateProgressingInfo from IBOX and Amp*/
    m_UpdateProgressingInfoListenerIBOX = new CUpdateProgressingInfoListenerIBOX(this);
    m_UpdateProgressingInfoListenerAmp = new CUpdateProgressingInfoListenerAMP(this);

    /** Instances of properties for HU, IBOX and AMP*/
    m_ReadHWIdentifierHU = new acs::generated::fblock::SoftwareDownload::CReadHWIdentifier(m_SWDLFBlockHU);
    m_ReadHWIdentifierIbox = new acs::generated::shadow::SoftwareDownload::CReadHWIdentifier(m_SWDLShadowIbox);
    m_ReadHWIdentifierAmp = new acs::generated::shadow::SoftwareDownload::CReadHWIdentifier(m_SWDLShadowAmp);

    m_ReadDevNameHU = new acs::generated::fblock::SoftwareDownload::CReadDevName(m_SWDLFBlockHU);
    m_ReadDevNameIbox = new acs::generated::shadow::SoftwareDownload::CReadDevName(m_SWDLShadowIbox);
    m_ReadDevNameAmp = new acs::generated::shadow::SoftwareDownload::CReadDevName(m_SWDLShadowAmp);

    m_ReadHWVersionHU = new acs::generated::fblock::SoftwareDownload::CReadHWVersion(m_SWDLFBlockHU);
    m_ReadHWVersionIbox = new acs::generated::shadow::SoftwareDownload::CReadHWVersion(m_SWDLShadowIbox);
    m_ReadHWVersionAmp = new acs::generated::shadow::SoftwareDownload::CReadHWVersion(m_SWDLShadowAmp);

    m_ReadNumberofModulesHU = new acs::generated::fblock::SoftwareDownload::CReadNumberOfModule(m_SWDLFBlockHU);
    m_ReadNumberofModulesIbox = new acs::generated::shadow::SoftwareDownload::CReadNumberOfModule(m_SWDLShadowIbox);
    m_ReadNumberofModulesAmp = new acs::generated::shadow::SoftwareDownload::CReadNumberOfModule(m_SWDLShadowAmp);

    m_ReadModuleVersionHU = new acs::generated::fblock::SoftwareDownload::CReadModuleVersion(m_SWDLFBlockHU);
    m_ReadModuleVersionIbox = new acs::generated::shadow::SoftwareDownload::CReadModuleVersion(m_SWDLShadowIbox);
    m_ReadModuleVersionAmp = new acs::generated::shadow::SoftwareDownload::CReadModuleVersion(m_SWDLShadowAmp);

    m_ReprogrammingModeHU = new acs::generated::fblock::SoftwareDownload::CReprogrammingMode(m_SWDLFBlockHU);
    m_ReprogrammingModeIbox = new acs::generated::shadow::SoftwareDownload::CReprogrammingMode(m_SWDLShadowIbox);
    m_ReprogrammingModeAmp = new acs::generated::shadow::SoftwareDownload::CReprogrammingMode(m_SWDLShadowAmp);

    m_InitUpdateHU = new acs::generated::fblock::SoftwareDownload::CInitUpdate(m_SWDLFBlockHU);
    m_InitUpdateIbox = new acs::generated::shadow::SoftwareDownload::CInitUpdate(m_SWDLShadowIbox);
    m_InitUpdateAmp = new acs::generated::shadow::SoftwareDownload::CInitUpdate(m_SWDLShadowAmp);

    m_EraseMemoryHU = new acs::generated::fblock::SoftwareDownload::CEraseMemory(m_SWDLFBlockHU);
    m_EraseMemoryIbox = new acs::generated::shadow::SoftwareDownload::CEraseMemory(m_SWDLShadowIbox);
    m_EraseMemoryAmp = new acs::generated::shadow::SoftwareDownload::CEraseMemory(m_SWDLShadowAmp);

    m_RequestDownloadHU = new acs::generated::fblock::SoftwareDownload::CRequestDownload(m_SWDLFBlockHU);
    m_RequestDownloadIbox = new acs::generated::shadow::SoftwareDownload::CRequestDownload(m_SWDLShadowIbox);
    m_RequestDownloadAmp = new acs::generated::shadow::SoftwareDownload::CRequestDownload(m_SWDLShadowAmp);

    m_PrepareFileTransferHU = new acs::generated::fblock::SoftwareDownload::CPrepareFileTransfer(m_SWDLFBlockHU);
    m_PrepareFileTransferIbox = new acs::generated::shadow::SoftwareDownload::CPrepareFileTransfer(m_SWDLShadowIbox);
    m_PrepareFileTransferAmp = new acs::generated::shadow::SoftwareDownload::CPrepareFileTransfer(m_SWDLShadowAmp);

    m_TransferDataCMSHU = new acs::generated::fblock::SoftwareDownload::CTransferDataCMS(m_SWDLFBlockHU);
    m_TransferDataCMSIbox = new acs::generated::shadow::SoftwareDownload::CTransferDataCMS(m_SWDLShadowIbox);
    m_TransferDataCMSAmp = new acs::generated::shadow::SoftwareDownload::CTransferDataCMS(m_SWDLShadowAmp);

    m_TransferDataAMSHU = new acs::generated::fblock::SoftwareDownload::CTransferDataAMS(m_SWDLFBlockHU);
    m_TransferDataAMSIbox = new acs::generated::shadow::SoftwareDownload::CTransferDataAMS(m_SWDLShadowIbox);
    m_TransferDataAMSAmp = new acs::generated::shadow::SoftwareDownload::CTransferDataAMS(m_SWDLShadowAmp);

    m_CheckActivateModuleHU = new acs::generated::fblock::SoftwareDownload::CCheckActivateModule(m_SWDLFBlockHU);
    m_CheckActivateModuleIbox = new acs::generated::shadow::SoftwareDownload::CCheckActivateModule(m_SWDLShadowIbox);
    m_CheckActivateModuleAmp = new acs::generated::shadow::SoftwareDownload::CCheckActivateModule(m_SWDLShadowAmp);

    m_RequestTransferExitHU = new acs::generated::fblock::SoftwareDownload::CRequestTransferExit(m_SWDLFBlockHU);
    m_RequestTransferExitIbox = new acs::generated::shadow::SoftwareDownload::CRequestTransferExit(m_SWDLShadowIbox);
    m_RequestTransferExitAmp = new acs::generated::shadow::SoftwareDownload::CRequestTransferExit(m_SWDLShadowAmp);

    m_SWDLProgressInfoHU = new acs::generated::fblock::SoftwareDownload::CSWDLProgressInfo(m_SWDLFBlockHU);
    m_SWDLProgressInfoIbox = new acs::generated::shadow::SoftwareDownload::CSWDLProgressInfo(m_SWDLShadowIbox);
    m_SWDLProgressInfoAmp = new acs::generated::shadow::SoftwareDownload::CSWDLProgressInfo(m_SWDLShadowAmp);

    m_UpdateProgressingInfoHU = new acs::generated::fblock::SoftwareDownload::CUpdateProcessingInfo(m_SWDLFBlockHU);
    m_UpdateProgressingInfoIBOX = new acs::generated::shadow::SoftwareDownload::CUpdateProcessingInfo(m_SWDLShadowIbox);
    m_UpdateProgressingInfoAmp = new acs::generated::shadow::SoftwareDownload::CUpdateProcessingInfo(m_SWDLShadowAmp);

    // iBox SWDL FBlock checkling for iBox Update, 2014.2.24
    m_IBOXSWDLFblockStateListener = new CIBOXSWDLFblockStateListener(m_SWDLShadowIbox, this);

    /** Intializing the structure */
    fileTransferInfo.blocksize = 0;
    fileTransferInfo.fileName = (const char *)0;
    fileTransferInfo.filePath = (const char *)0;
    fileTransferInfo.fileSize = 0;
    fileTransferInfo.transfertype = 0;

    m_fileTransferIBOX = false;
    m_blockCounter = 0;
    m_filearray = 0;
    m_EndOfFile = false;

    m_IBOXUpgradefile = 0;

    //timem = 0;
    //memset(&tval, 0, sizeof(timeval));
    //memset(&tzone, 0, sizeof(timezone));
}

/*
 *******************************************************************
 * Function:~CSWDLFBlockShadowControl
 *
 * Description : Is a Destructor,removes all the registered listeners.
 *              and destroys all the instances.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */

CSWDLFBlockShadowControl::~CSWDLFBlockShadowControl(){

    RemoveListener();

    delete m_SWDLFBlockHU;
    delete m_SWDLShadowIbox;
    delete m_SWDLShadowAmp;

    delete m_RepmodeHandlerHWIdentifierListenerHU;
    delete m_RepmodeHandlerHWIdentifierListenerIbox;
    delete m_RepmodeHandlerHWIdentifierListenerAmp;
    delete m_ReprogrammingModeIbox;
    delete m_ReprogrammingModeHU;
    delete m_ReprogrammingModeAmp;
    delete m_ReadHWIdentifierHU;
    delete m_ReadHWIdentifierIbox;
    delete m_ReadHWIdentifierAmp;

    delete m_IUpdateHandlerDevNameListenerHU;
    delete m_IUpdateHandlerDevNameListenerIbox;
    delete m_IUpdateHandlerDevNameListenerAmp;
    delete m_InitUpdateHU;
    delete m_InitUpdateIbox;
    delete m_InitUpdateAmp;
    delete m_ReadDevNameHU;
    delete m_ReadDevNameIbox;
    delete m_ReadDevNameAmp;

    delete m_EraseMemoryHandlerHWVersionListenerHU;
    delete m_EraseMemoryHandlerHWVersionListenerIbox;
    delete m_EraseMemoryHandlerHWVersionListenerAmp;
    delete m_EraseMemoryHU;
    delete m_EraseMemoryIbox;
    delete m_EraseMemoryAmp;
    delete m_ReadHWVersionHU;
    delete m_ReadHWVersionIbox;
    delete m_ReadHWVersionAmp;

    delete m_RDownloadHandlerNumofmoduleListenerHU;
    delete m_RDownloadHandlerNumofmoduleListenerIbox;
    delete m_RDownloadHandlerNumofmoduleListenerAmp;
    delete m_RequestDownloadHU;
    delete m_RequestDownloadIbox;
    delete m_RequestDownloadAmp;
    delete m_ReadNumberofModulesHU;
    delete m_ReadNumberofModulesIbox;
    delete m_ReadNumberofModulesAmp;

    delete m_PFTransferHandlerModeVersionListenerHU;
    delete m_PFTransferHandlerModeVersionListenerIbox;
    delete m_PFTransferHandlerModeVersionListenerAmp;
    delete m_PrepareFileTransferHU;
    delete m_PrepareFileTransferIbox;
    delete m_PrepareFileTransferAmp;
    delete m_ReadModuleVersionHU;
    delete m_ReadModuleVersionIbox;
    delete m_ReadModuleVersionAmp;

    delete m_TransferDataCMSHandlerHU;
    delete m_TransferDataCMSHandlerIbox;
    delete m_TransferDataCMSHandlerAmp;
    delete m_TransferDataCMSHU;
    delete m_TransferDataCMSIbox;
    delete m_TransferDataCMSAmp;

    delete m_TransferDataAMSHandlerHU;
    delete m_TransferDataAMSHandlerIbox;
    delete m_TransferDataAMSHandlerAmp;
    delete m_TransferDataAMSHU;
    delete m_TransferDataAMSIbox;
    delete m_TransferDataAMSAmp;

    delete m_CheckActivateModuleHandlerHU;
    delete m_CheckActivateModuleHandlerIbox;
    delete m_CheckActivateModuleHandlerAmp;
    delete m_CheckActivateModuleIbox;
    delete m_CheckActivateModuleHU;
    delete m_CheckActivateModuleAmp;

    delete m_RequestTransferExitHandlerHU;
    delete m_RequestTransferExitHandlerIbox;
    delete m_RequestTransferExitHandlerAmp;
    delete m_RequestTransferExitHU;
    delete m_RequestTransferExitIbox;
    delete m_RequestTransferExitAmp;

    delete m_SWDLProgressInfoListenerIbox;
    delete m_SWDLProgressInfoListenerAmp;
    delete m_SWDLProgressInfoHU;
    delete m_SWDLProgressInfoIbox;
    delete m_SWDLProgressInfoAmp;

    delete m_UpdateProgressingInfoHU;
    delete m_UpdateProgressingInfoIBOX;
    delete m_UpdateProgressingInfoAmp;
    delete m_UpdateProgressingInfoListenerIBOX;
    delete m_UpdateProgressingInfoListenerAmp;

    delete m_IBOXUpgradefile;
    
	// iBox SWDL FBlock checkling for iBox Update, 2014.2.24
	delete m_IBOXSWDLFblockStateListener;

}

/*
 *******************************************************************
 * Function:Initialize
 *
 * Description :Function initializes the MOST elements to SWDL
 *              FBlock,shadow and also adds to MOST stack.
 *
 * Parameters : none
 *
 *
 * Returns :true or false
 *
 *******************************************************************
 */

bool CSWDLFBlockShadowControl::Initialize()
{
    /** Initialize the properties of software downlaod and set the notification for properties required */
    if(m_SWDLFBlockHU->InitializeMOSTElement() && m_SWDLShadowIbox->InitializeMOSTElement() && m_SWDLShadowAmp->InitializeMOSTElement())
    {
        SetHandlerListener();

        /** Initializing Notification values with default values */       
        SetSWDLProgressInfo(0xFF,0x00);
        SetUpdateProgressingInfo(0xFF);

        m_Initialized = true;
    }

    return m_Initialized;
}

bool CSWDLFBlockShadowControl::Register()
{
    bool result = false;

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " Register API got called " << MOST_LOG_ENDL;

    //if(m_Initialized)
    {
        if(!m_Registered)
        {
            result = m_AppContext->MOST()->AddFunctionBlock(m_SWDLFBlockHU);
            result &= m_AppContext->MOST()->AddShadow(m_SWDLShadowIbox);
            result &= m_AppContext->MOST()->AddShadow(m_SWDLShadowAmp);

            if(result)
            {
                MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " Register FBlock successfully " << MOST_LOG_ENDL;
                m_Registered = true;
            }
            else
            {
                MOST_LOG_CRITICAL << LOG_TAG_SWDLCONTROL << " Failed to Register FBlock " << MOST_LOG_ENDL;
            }
        }
        else
        {
            MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " FBlock was already registered " << MOST_LOG_ENDL;
            // A case already registered
            result = true;
        }
    }

    return result;
}

bool CSWDLFBlockShadowControl::UnRegister()
{
    bool result = false;

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " UnRegister API got called " << MOST_LOG_ENDL;

    if(m_Registered)
    {
        result = m_AppContext->MOST()->RemoveFunctionBlock(m_SWDLFBlockHU);
        result &= m_AppContext->MOST()->RemoveShadow(m_SWDLShadowIbox);
        result &= m_AppContext->MOST()->RemoveShadow(m_SWDLShadowAmp);

        if(result)
        {
            MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " UnRegister FBlock successfully " << MOST_LOG_ENDL;
            m_Registered = false;
        }
        else
        {
            MOST_LOG_CRITICAL << LOG_TAG_SWDLCONTROL << " Failed to Unregister FBlock " << MOST_LOG_ENDL;
        }
    }
    else
    {
        MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " FBlock was already Unregistered " << MOST_LOG_ENDL;
        result = true;
    }

    return result;
}

bool CSWDLFBlockShadowControl::FBlockVisible(const uint deviceID)
{
    bool result = false;

    if(deviceID == HU_DEVICE_ADDRESS)
    {
        result = GetIsRegistered();
    }
    else if(deviceID == IBOX_DEVICE_ADDRESS)
    {
        result = m_SWDLShadowIbox->FunctionBlockVisible();
    }
    else if(deviceID == AMP_DEVICE_ADDRESS)
    {
        result = m_SWDLShadowAmp->FunctionBlockVisible();
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetHandlerListener
 *
 * Description : Sets the listeners and handlers of all properties defined.
 *
 * Parameters : none
 *
 * Returns :none
 *
 *******************************************************************
 */

void CSWDLFBlockShadowControl::SetHandlerListener()
{
    m_ReprogrammingModeHU->SetHandler(m_RepmodeHandlerHWIdentifierListenerHU);
    m_ReprogrammingModeIbox->SetHandler(m_RepmodeHandlerHWIdentifierListenerIbox);
    m_ReprogrammingModeAmp->SetHandler(m_RepmodeHandlerHWIdentifierListenerAmp);

    m_ReadHWIdentifierHU->SetListener(m_RepmodeHandlerHWIdentifierListenerHU);
    m_ReadHWIdentifierIbox->SetListener(m_RepmodeHandlerHWIdentifierListenerIbox);
    m_ReadHWIdentifierAmp->SetListener(m_RepmodeHandlerHWIdentifierListenerAmp);

    m_InitUpdateHU->SetHandler(m_IUpdateHandlerDevNameListenerHU);
    m_InitUpdateIbox->SetHandler(m_IUpdateHandlerDevNameListenerIbox);
    m_InitUpdateAmp->SetHandler(m_IUpdateHandlerDevNameListenerAmp);
    m_ReadDevNameHU->SetListener(m_IUpdateHandlerDevNameListenerHU);
    m_ReadDevNameIbox->SetListener(m_IUpdateHandlerDevNameListenerIbox);
    m_ReadDevNameAmp->SetListener(m_IUpdateHandlerDevNameListenerAmp);

    m_EraseMemoryHU->SetHandler(m_EraseMemoryHandlerHWVersionListenerHU);
    m_EraseMemoryIbox->SetHandler(m_EraseMemoryHandlerHWVersionListenerIbox);
    m_EraseMemoryAmp->SetHandler(m_EraseMemoryHandlerHWVersionListenerAmp);
    m_ReadHWVersionHU->SetListener(m_EraseMemoryHandlerHWVersionListenerHU);
    m_ReadHWVersionIbox->SetListener(m_EraseMemoryHandlerHWVersionListenerIbox);
    m_ReadHWVersionAmp->SetListener(m_EraseMemoryHandlerHWVersionListenerAmp);

    m_RequestDownloadHU->SetHandler(m_RDownloadHandlerNumofmoduleListenerHU);
    m_RequestDownloadIbox->SetHandler(m_RDownloadHandlerNumofmoduleListenerIbox);
    m_RequestDownloadAmp->SetHandler(m_RDownloadHandlerNumofmoduleListenerAmp);
    m_ReadNumberofModulesHU->SetListener(m_RDownloadHandlerNumofmoduleListenerHU);
    m_ReadNumberofModulesIbox->SetListener(m_RDownloadHandlerNumofmoduleListenerIbox);
    m_ReadNumberofModulesAmp->SetListener(m_RDownloadHandlerNumofmoduleListenerAmp);

    m_PrepareFileTransferHU->SetHandler(m_PFTransferHandlerModeVersionListenerHU);
    m_PrepareFileTransferIbox->SetHandler(m_PFTransferHandlerModeVersionListenerIbox);
    m_PrepareFileTransferAmp->SetHandler(m_PFTransferHandlerModeVersionListenerAmp);
    m_ReadModuleVersionHU->SetListener(m_PFTransferHandlerModeVersionListenerHU);
    m_ReadModuleVersionIbox->SetListener(m_PFTransferHandlerModeVersionListenerIbox);
    m_ReadModuleVersionAmp->SetListener(m_PFTransferHandlerModeVersionListenerAmp);

    m_TransferDataCMSHU->SetHandler(m_TransferDataCMSHandlerHU);
    m_TransferDataCMSIbox->SetHandler(m_TransferDataCMSHandlerIbox);
    m_TransferDataCMSAmp->SetHandler(m_TransferDataCMSHandlerAmp);

    m_TransferDataAMSHU->SetHandler(m_TransferDataAMSHandlerHU);
    m_TransferDataAMSIbox->SetHandler(m_TransferDataAMSHandlerIbox);
    m_TransferDataAMSAmp->SetHandler(m_TransferDataAMSHandlerAmp);

    m_CheckActivateModuleHU->SetHandler(m_CheckActivateModuleHandlerHU);
    m_CheckActivateModuleIbox->SetHandler(m_CheckActivateModuleHandlerIbox);
    m_CheckActivateModuleAmp->SetHandler(m_CheckActivateModuleHandlerAmp);

    m_RequestTransferExitHU->SetHandler(m_RequestTransferExitHandlerHU);
    m_RequestTransferExitIbox->SetHandler(m_RequestTransferExitHandlerIbox);
    m_RequestTransferExitAmp->SetHandler(m_RequestTransferExitHandlerAmp);

    m_SWDLProgressInfoIbox->SetListener(m_SWDLProgressInfoListenerIbox);
    m_SWDLProgressInfoAmp->SetListener(m_SWDLProgressInfoListenerAmp);

    m_UpdateProgressingInfoIBOX->SetListener(m_UpdateProgressingInfoListenerIBOX);
    m_UpdateProgressingInfoAmp->SetListener(m_UpdateProgressingInfoListenerAmp);

	// iBox SWDL FBlock checkling for iBox Update, 2014.2.24
	m_SWDLShadowIbox->SetListener(m_IBOXSWDLFblockStateListener);
    
}

/*
 *******************************************************************
 * Function:RemoveListener
 *
 * Description :Removes the listeners for all the properties registered.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::RemoveListener()
{
    m_ReadHWIdentifierHU->RemoveListener(m_RepmodeHandlerHWIdentifierListenerHU);
    m_ReadHWIdentifierIbox->RemoveListener(m_RepmodeHandlerHWIdentifierListenerIbox);
    m_ReadHWIdentifierAmp->RemoveListener(m_RepmodeHandlerHWIdentifierListenerAmp);

    m_ReadDevNameHU->RemoveListener(m_IUpdateHandlerDevNameListenerHU);
    m_ReadDevNameIbox->RemoveListener(m_IUpdateHandlerDevNameListenerIbox);
    m_ReadDevNameAmp->RemoveListener(m_IUpdateHandlerDevNameListenerAmp);

    m_ReadHWVersionHU->RemoveListener(m_EraseMemoryHandlerHWVersionListenerHU);
    m_ReadHWVersionIbox->RemoveListener(m_EraseMemoryHandlerHWVersionListenerIbox);
    m_ReadHWVersionAmp->RemoveListener(m_EraseMemoryHandlerHWVersionListenerAmp);

    m_ReadNumberofModulesHU->RemoveListener(m_RDownloadHandlerNumofmoduleListenerHU);
    m_ReadNumberofModulesIbox->RemoveListener(m_RDownloadHandlerNumofmoduleListenerIbox);
    m_ReadNumberofModulesAmp->RemoveListener(m_RDownloadHandlerNumofmoduleListenerAmp);

    m_ReadModuleVersionHU->RemoveListener(m_PFTransferHandlerModeVersionListenerHU);
    m_ReadModuleVersionIbox->RemoveListener(m_PFTransferHandlerModeVersionListenerIbox);
    m_ReadModuleVersionAmp->RemoveListener(m_PFTransferHandlerModeVersionListenerAmp);
    m_SWDLProgressInfoIbox->RemoveListener(m_SWDLProgressInfoListenerIbox);
    m_SWDLProgressInfoAmp->RemoveListener(m_SWDLProgressInfoListenerAmp);

    m_UpdateProgressingInfoIBOX->RemoveListener(m_UpdateProgressingInfoListenerIBOX);
    m_UpdateProgressingInfoAmp->RemoveListener(m_UpdateProgressingInfoListenerAmp);

	// iBox SWDL FBlock checkling for iBox Update, 2014.2.24
	m_SWDLShadowIbox->SetListener(m_IBOXSWDLFblockStateListener);	
}
/*
 *******************************************************************
 * Function:SetReadHWIdentifier
 *
 * Description :Function called by the SWDL adaptor to update or request hardware identifier
 *              over ACS Daemon and send it to IBOX and AMP. Is a part of the device
 *              identification process of software download.
 *
 * Parameters :
 *    swdlid         rw   It holds data which represents FBlock and Shadows of IBOX and Amplifier
 *                        in order to update the respective properties.
 *    hwidentifier   rw   Is a string of that contains hardware identifier of MOST devices.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetReadHWIdentifier(const uint swdlid, const uint state, const QString hwidentifier)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetReadHWIdentifier API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReadHWIdentifier:swdlid " << swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReadHWIdentifier:state " << state << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReadHWIdentifier:hwidentifier " << hwidentifier.toAscii().data() << MOST_LOG_ENDL;

    bool retVal = false;
    const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding = NULL;
    uint8_t* hwidentifierString = NULL;
    uint32_t hwidentifierStringLength = 0;

    switch(swdlid)
    {
        case SWDLID_HU:
            {
                if(state == STATE_SET)
                {
                    /** Update the hwidentifier string from Head Unit to IBOX and AMP*/
                    m_ReadHWIdentifierHU->HWIdentifierString(
                        &k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                reinterpret_cast<const uint8_t*>(hwidentifier.toUtf8().data()),
                                hwidentifier.length()+1);
                    retVal = m_ReadHWIdentifierHU->UpdateShadows();
                 }
                else if(state == STATE_GET)
                {
                    /** request for hwidentifier string from HU*/
                    m_ReadHWIdentifierHU->HWIdentifierString(&pStringEncoding,&hwidentifierString,&hwidentifierStringLength);
                    emit ReadHWIdentifier(swdlid,reinterpret_cast<const char*>(hwidentifierString));
                }
            }
            break;

         case SWDLID_IBOX:
            /** request for hwidentifier string from IBOX*/
            retVal = m_ReadHWIdentifierIbox->Get();
            break;

         case SWDLID_AMP:
            /** request for hwidentifier string from AMP*/
            retVal = m_ReadHWIdentifierAmp->Get();
            break;

         default:
            break;
    }
    return retVal;
}
/*
 *******************************************************************
 * Function:SetReadDevName
 *
 * Description :Function called by the SWDL adaptor to update or request Device name
 *              over ACS Daemon and send it to IBOX and AMP. Is a part of the device
 *              identification process of software update.
 *
 * Parameters :
 *    swdlid       rw   It holds data which represents FBlock and Shadows of IBOX and Amplifier
 *                      in order to update the respective properties.
 *    deviceName   rw   Is a string of that contains device name.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetReadDevName(const uint swdlid, const QString deviceName)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetReadDevName API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReadDevName:swdlid " << swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReadDevName:deviceName " << deviceName.toAscii().data() << MOST_LOG_ENDL;

    bool retVal = false;

    switch(swdlid)
    {
        case SWDLID_HU:
            /** Update the deviceName string from Head Unit to IBOX and AMP*/
            m_ReadDevNameHU->DeviceNameString(
                &k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                        reinterpret_cast<const uint8_t*>(deviceName.toUtf8().data()),
                        deviceName.length()+1);
            retVal = m_ReadDevNameHU->UpdateShadows();
            break;

        case SWDLID_IBOX:
            /** request for deviceName string from IBOX*/
            retVal = m_ReadDevNameIbox->Get();
            break;
        case SWDLID_AMP:
            /** request for deviceName string from AMP*/
            retVal = m_ReadDevNameAmp->Get();
            break;
        default:
            break;
    }

    return retVal;
}
/*
 *******************************************************************
 * Function:SetReadHWVersion
 *
 * Description :Function called by the SWDL adaptor to update or request Device name
 *              over ACS Daemon and send it to IBOX and AMP. Is a part of the device
 *              identification process of software update.
 *
 * Parameters :
 *    swdlid       rw   It holds data which represents FBlock and Shadows of IBOX and Amplifier
 *                      in order to update the respective properties.
 *    hwversion   rw   Is a string of that contains hardware version.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetReadHWVersion(const uint swdlid, const QString hwversion)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetReadHWVersion API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReadHWVersion:swdlid " << swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReadHWVersion:hwversion " << hwversion.toAscii().data() << MOST_LOG_ENDL;

    bool retVal = false;

    switch(swdlid)
    {
        case SWDLID_HU:
            /** Update the hwversion string from Head Unit to IBOX and AMP*/
            m_ReadHWVersionHU->HWVersionString(
                &k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                        reinterpret_cast<const uint8_t*>(hwversion.toUtf8().data()),
                        hwversion.length()+1);
            retVal = m_ReadHWVersionHU->UpdateShadows();
            break;
        case SWDLID_IBOX:
            /** request for hwversion string from IBOX*/
            retVal = m_ReadHWVersionIbox->Get();
            break;
        case SWDLID_AMP:
            /** request for hwversion string from AMP*/
            retVal = m_ReadHWVersionAmp->Get();
            break;
        default:
            break;
    }

    return retVal;
}
/*
 *******************************************************************
 * Function:SetReadNumberofModule
 *
 * Description :Function called by the SWDL adaptor to update or request number of modules
 *              over ACS Daemon and send it to IBOX and AMP. Is a part of the device
 *              identification process of software update.
 *
 * Parameters :
 *    swdlid        rw   It holds data which represents FBlock and Shadows of IBOX and Amplifier
 *                       in order to update the respective properties.
 *    modulevalue   rw   Is a string of that contains code,data and boot value.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetReadNumberofModule(const uint swdlid, QByteArray modulevalue)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetReadNumberofModule API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReadNumberofModule:swdlid " << swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReadNumberofModule:modulevalue " << modulevalue.length() << MOST_LOG_ENDL;
    //MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReadNumberofModule:modulevalue " << modulevalue.toHex().data() << MOST_LOG_ENDL;

    bool retVal = false;

    switch(swdlid)
    {
        case SWDLID_HU:
            {
                    /** Update the modulevalue string from Head Unit to IBOX and AMP*/
                    QByteArray data((const char *)modulevalue,modulevalue.length());	//hyunyong.sung 2013.2.18
                    m_ReadNumberofModulesHU->ModuleCountValue(reinterpret_cast<uint8_t*>(data.data()),
                                                        data.length());
                    retVal = m_ReadNumberofModulesHU->UpdateShadows();
            }
            break;
        case SWDLID_IBOX:
            /** request for modulevalue string from IBOX*/
            retVal = m_ReadNumberofModulesIbox->Get();
            break;
        case SWDLID_AMP:
             /** request for modulevalue string from AMP*/
            retVal = m_ReadNumberofModulesAmp->Get();
            break;
        default:
            break;
    }
    return retVal;
}
/*
 *******************************************************************
 * Function:SetReadModuleVersion
 *
 * Description :Function called by the SWDL adaptor to update or request module version
 *              over ACS Daemon and send it to IBOX and AMP. Is a part of the device
 *              identification process of software update.
 *
 * Parameters :
 *    swdlid          rw   It holds data which represents FBlock and Shadows of IBOX and Amplifier
 *                         in order to update the respective properties.
 *    moduleversion   rw   Is a string of that contains module version.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetReadModuleVersion(const uint swdlid, const QDBusVariant &moduleversion)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetReadModuleVersion API got called " << MOST_LOG_ENDL;
    bool retVal = false;

    /** Unpacking structure form arugument of type QDBusVariant  */   
    struct SModuleVersionListInfo sreadmoduleversion_wr;
    moduleversion.variant().value<QDBusArgument>() >> sreadmoduleversion_wr;

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReadModuleVersion:swdlid " << swdlid << MOST_LOG_ENDL;


    switch(swdlid)
    {
        case SWDLID_HU:
            /** Update the module versions from Head Unit to IBOX and AMP*/
            m_ReadModuleVersionHU->Size(sreadmoduleversion_wr.infosize);
            for(uint count = 0; count < sreadmoduleversion_wr.infosize; count++)
            {
                MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReadModuleVersion:moduleType " << sreadmoduleversion_wr.moduleversionlistinfo.at(count).moduletype << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReadModuleVersion:moduleVersion " << sreadmoduleversion_wr.moduleversionlistinfo.at(count).moduleversion.toAscii().data() << MOST_LOG_ENDL;

                m_ReadModuleVersionHU->DataModuleTypeValue(count,
                       (acs::generated::fblock::SoftwareDownload::CReadModuleVersion::TDataModuleType)sreadmoduleversion_wr.moduleversionlistinfo.at(count).moduletype);



                m_ReadModuleVersionHU->DataModuleVersionString(count,&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                                                 reinterpret_cast<const uint8_t*>(sreadmoduleversion_wr.moduleversionlistinfo.at(count).moduleversion.toUtf8().data()),
                                                                 sreadmoduleversion_wr.moduleversionlistinfo.at(count).moduleversion.length()+1);

            }
            retVal = m_ReadModuleVersionHU->UpdateShadows();
            break;
        case SWDLID_IBOX:
             /** request for module versions from IBOX*/
            retVal = m_ReadModuleVersionIbox->Get();
            break;
        case SWDLID_AMP:
             /** request for module versions from AMP*/
            retVal = m_ReadModuleVersionAmp->Get();
            break;
        default:
            break;
    }
    return retVal;
}

/*
 *******************************************************************
 * Function:SetSWDLProgressInfo
 *
 * Description :Function called by the SWDL adaptor to set SWDL Progress Info.
 *
 * Parameters :
 *    swdlOpMode    rw   Is unsigned byte which holds the SWDL operation mode set by the user.
 *    swdlProgress  rw   Is unsigned byte which holds the progress status.
 *
 * Returns :true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetSWDLProgressInfo(const uint swdlOpMode, const uint swdlProgress)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetSWDLProgressInfo API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetSWDLProgressInfo:swdlOpMode " << swdlOpMode << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetSWDLProgressInfo:swdlProgress " << swdlProgress << MOST_LOG_ENDL;

    /** Update the swdlProgress string from Head Unit to IBOX and AMP*/

    m_SWDLProgressInfoHU->SWDLProgressDataDataType().ResetWriter();
    m_SWDLProgressInfoHU->SWDLOpModeValue((acs::generated::fblock::SoftwareDownload::CSWDLProgressInfo::TSWDLOpMode)swdlOpMode);
    m_SWDLProgressInfoHU->SWDLProgressValue(swdlProgress);
    m_SWDLProgressInfoHU->SWDLProgressDataSaveNext();
    return m_SWDLProgressInfoHU->UpdateShadows();
}
/*
 *******************************************************************
 * Function:SetReprogrammingMode
 *
 * Description :Function called by the SWDL adaptor to activate and deactivate reprogramming
 *              mode.
 *
 * Parameters :
 *    reprogrammingmode      rw   It is QDBusVariant datatype which holds the structure
 *                                SReporgrammingMode containing software download mode
 *                                (normal mode or reprogramming mode), software download id,
 *                                device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetReprogrammingMode(const SReporgrammingMode &reprogrammingmode_wr)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetReprogrammingMode API got called " << MOST_LOG_ENDL;
    bool retVal = false;   

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReprogrammingMode:status " << reprogrammingmode_wr.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReprogrammingMode:Senderhandle " << reprogrammingmode_wr.senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReprogrammingMode:DeviceId " << reprogrammingmode_wr.DeviceID << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReprogrammingMode:SWDLmode " << reprogrammingmode_wr.SWDLmode << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetReprogrammingMode:swdlid " << reprogrammingmode_wr.swdlid << MOST_LOG_ENDL;

    switch(reprogrammingmode_wr.status)
    {
        case STATUS_RESULTACK:
            /** Updates reprogramming mode result ack to IBOX/AMP based on the request*/
            m_ReprogrammingModeHU->SenderHandleValue(reprogrammingmode_wr.senderhandle);
            m_ReprogrammingModeHU->SWDLModeValue((acs::generated::fblock::SoftwareDownload::CReprogrammingMode::TSWDLMode)reprogrammingmode_wr.SWDLmode);
            retVal = m_ReprogrammingModeHU->ResultAck(reprogrammingmode_wr.DeviceID);
            break;
        case STATUS_PROCESSINGACK:
            {
                /** Updates reprogramming mode processing ack to IBOX/AMP based on the request*/
                uint16_t senderhandle = reprogrammingmode_wr.senderhandle;
                m_ReprogrammingModeHU->SenderHandleValue(reprogrammingmode_wr.senderhandle);
                retVal = m_ReprogrammingModeHU->ProcessingAck(reprogrammingmode_wr.DeviceID,senderhandle);
            }
            break;
        case STATUS_STARTRESULTACK:
            {
                if(reprogrammingmode_wr.swdlid == SWDLID_IBOX)
                {
                    /** Updates the start result ack request for reprogramming mode to IBOX */
                    m_ReprogrammingModeIbox->SenderHandleValue(reprogrammingmode_wr.senderhandle);
                    m_ReprogrammingModeIbox->SWDLModeValue((acs::generated::shadow::SoftwareDownload::CReprogrammingMode::TSWDLMode)reprogrammingmode_wr.SWDLmode);
                    retVal = m_ReprogrammingModeIbox->StartResultAck();
                }
                else if(reprogrammingmode_wr.swdlid == SWDLID_AMP)
                {
                    /** Updates the start result ack request for reprogramming mode to AMP */
                    m_ReprogrammingModeAmp->SenderHandleValue(reprogrammingmode_wr.senderhandle);
                    m_ReprogrammingModeAmp->SWDLModeValue((acs::generated::shadow::SoftwareDownload::CReprogrammingMode::TSWDLMode)reprogrammingmode_wr.SWDLmode);
                    retVal = m_ReprogrammingModeAmp->StartResultAck();
                }
            }
            break;
        default:
            break;
    }

    return retVal;
}
/*
 *******************************************************************
 * Function:SetInitUpdateResult
 *
 * Description :Function called by the SWDL adaptor to initiates the update and
 *              set the date of the update and the DiscID of the update medium
 *              mode.
 *
 * Parameters :
 *    initUpdate      rw   It is QDBusVariant datatype which holds the structure
 *                         SInitUpdate containing diskid, date of the update,
 *                         software download id, device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetInitUpdateResult(const SInitUpdate &sinitupdate_wr)
{
    bool retVal = false;

    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetInitUpdateResult API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetInitUpdateResult:status " << sinitupdate_wr.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetInitUpdateResult:swdlid " << sinitupdate_wr.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetInitUpdateResult:DeviceId " << sinitupdate_wr.DeviceID << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetInitUpdateResult:senderhandle " << sinitupdate_wr.senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetInitUpdateResult:day " << sinitupdate_wr.day << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetInitUpdateResult:month " << sinitupdate_wr.month << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetInitUpdateResult:year " << sinitupdate_wr.year << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetInitUpdateResult:DiscID " << sinitupdate_wr.diskID.toAscii().data() << MOST_LOG_ENDL;

    switch(sinitupdate_wr.status)
    {
        case STATUS_RESULTACK:
            /** Updates Init Update result ack to IBOX/AMP based on the request*/
            m_InitUpdateHU->SenderHandleValue(sinitupdate_wr.senderhandle);
            m_InitUpdateHU->DayValue(sinitupdate_wr.day);
            m_InitUpdateHU->MonthValue(sinitupdate_wr.month);
            m_InitUpdateHU->YearValue(sinitupdate_wr.year);
            m_InitUpdateHU->DiscIDString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                           reinterpret_cast<const uint8_t*>(sinitupdate_wr.diskID.toUtf8().data()),
                                           sinitupdate_wr.diskID.length()+1);
            retVal = m_InitUpdateHU->ResultAck(sinitupdate_wr.DeviceID);
            break;
        case STATUS_PROCESSINGACK:
            {
                /** Updates Init Update processing ack to IBOX/AMP based on the request*/
                uint16_t senderhandle = sinitupdate_wr.senderhandle;
                m_InitUpdateHU->SenderHandleValue(sinitupdate_wr.senderhandle);
                retVal = m_InitUpdateHU->ProcessingAck(sinitupdate_wr.DeviceID,senderhandle);
            }
            break;
        case STATUS_STARTRESULTACK:
            {
                if(sinitupdate_wr.swdlid == SWDLID_IBOX)
                {
                    /** Updates the start result ack request for Init Update to IBOX */
                    m_InitUpdateIbox->SenderHandleValue(sinitupdate_wr.senderhandle);
                    m_InitUpdateIbox->DayValue(sinitupdate_wr.day);
                    m_InitUpdateIbox->MonthValue(sinitupdate_wr.month);
                    m_InitUpdateIbox->YearValue(sinitupdate_wr.year);
                    m_InitUpdateIbox->DiscIDString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                                    reinterpret_cast<const uint8_t*>(sinitupdate_wr.diskID.toUtf8().data()),
                                                    sinitupdate_wr.diskID.length()+1);
                    retVal = m_InitUpdateIbox->StartResultAck();

                }
				else if(sinitupdate_wr.swdlid == SWDLID_AMP)
                {
                    /** Updates the start result ack request for Init Update to AMP */
                    m_InitUpdateAmp->SenderHandleValue(sinitupdate_wr.senderhandle);
                    m_InitUpdateAmp->DayValue(sinitupdate_wr.day);
                    m_InitUpdateAmp->MonthValue(sinitupdate_wr.month);
                    m_InitUpdateAmp->YearValue(sinitupdate_wr.year);
                    m_InitUpdateAmp->DiscIDString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                                   reinterpret_cast<const uint8_t*>(sinitupdate_wr.diskID.toUtf8().data()),
                                                   sinitupdate_wr.diskID.length()+1);
                    retVal = m_InitUpdateAmp->StartResultAck();
                }
            }
            break;
        default:
            break;
    }
    return retVal;
}
/*
 *******************************************************************
 * Function:SetEraseMemory
 *
 * Description :Function called by the SWDL adaptor to erase the flash for
 *              special module to update.
 *
 * Parameters :
 *    eraseMemory      rw   It is QDBusVariant datatype which holds the structure
 *                         SEraseMemory containing module type, module index
 *                         software download id, device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetEraseMemory(const SEraseMemory &serasememory_wr)
{
    bool retVal = false;

    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetEraseMemory API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetEraseMemory:status " << serasememory_wr.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetEraseMemory:senderhandle " << serasememory_wr.Senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetEraseMemory:deviceId " << serasememory_wr.DeviceID << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetEraseMemory:swdlid " << serasememory_wr.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetEraseMemory:moduleType " << serasememory_wr.moduletype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetEraseMemory:moduleindex " << serasememory_wr.moduleindex << MOST_LOG_ENDL;

    switch(serasememory_wr.status)
    {
        case STATUS_RESULTACK:
            /** Updates Erase memory result ack to IBOX/AMP based on the request*/
            m_EraseMemoryHU->SenderHandleValue(serasememory_wr.Senderhandle);
            m_EraseMemoryHU->ModuleTypeValue((acs::generated::fblock::SoftwareDownload::CEraseMemory::TModuleType)serasememory_wr.moduletype);
            m_EraseMemoryHU->ModuleIndexValue(serasememory_wr.moduleindex);
            retVal = m_EraseMemoryHU->ResultAck(serasememory_wr.DeviceID);
            break;
        case STATUS_PROCESSINGACK:
            {
                /** Updates Erase memory processing ack to IBOX/AMP based on the request*/
                uint16_t senderhandle = serasememory_wr.Senderhandle;
                m_EraseMemoryHU->SenderHandleValue(serasememory_wr.Senderhandle);
                retVal = m_EraseMemoryHU->ProcessingAck(serasememory_wr.DeviceID,senderhandle);
            }
            break;
        case STATUS_STARTRESULTACK:
            {
                if(serasememory_wr.swdlid == SWDLID_IBOX)
                {
                    /** Updates the start result ack request for Erase memory to IBOX */
                    m_EraseMemoryIbox->SenderHandleValue(serasememory_wr.Senderhandle);
                    m_EraseMemoryIbox->ModuleTypeValue((acs::generated::shadow::SoftwareDownload::CEraseMemory::TModuleType)serasememory_wr.moduletype);
                    m_EraseMemoryIbox->ModuleIndexValue(serasememory_wr.moduleindex);
                    retVal = m_EraseMemoryIbox->StartResultAck();
                }
                else if(serasememory_wr.swdlid == SWDLID_AMP)
                {
                    /** Updates the start result ack request for Erase memory to AMP */
                    m_EraseMemoryAmp->SenderHandleValue(serasememory_wr.Senderhandle);
                    m_EraseMemoryAmp->ModuleTypeValue((acs::generated::shadow::SoftwareDownload::CEraseMemory::TModuleType)serasememory_wr.moduletype);
                    m_EraseMemoryAmp->ModuleIndexValue(serasememory_wr.moduleindex);
                    retVal = m_EraseMemoryAmp->StartResultAck();
                }
            }
            break;
        default:
            break;
    }
    return retVal;
}
/*
 *******************************************************************
 * Function:SetRequestDownload
 *
 * Description :Function called by the SWDL adaptor to initialize the Download
 *              for the given module
 *
 * Parameters :
 *    requestdownload     rw    It is QDBusVariant datatype which holds the structure
 *                              SRequestDownload containing blocksize and channel to be
 *                              used for transmission of data,
 *                              software download id, device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetRequestDownload(const SRequestDownload &srequestdownload_wr)
{
    bool retVal = false;

    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetRequestDownload API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetRequestDownload:status " << srequestdownload_wr.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetRequestDownload:deviceID " << srequestdownload_wr.DeviceID << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetRequestDownload:swdlid " << srequestdownload_wr.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetRequestDownload:senderhandle " << srequestdownload_wr.senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetRequestDownload:moduleType " << srequestdownload_wr.moduletype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetRequestDownload:moduleIndex " << srequestdownload_wr.moduleindex << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetRequestDownload:blockSize " << srequestdownload_wr.blocksize << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetRequestDownload:SWDLChannel " << srequestdownload_wr.swdlchannel << MOST_LOG_ENDL;

    switch(srequestdownload_wr.status)
    {
        case STATUS_RESULTACK:
            /** Updates Request download result ack to IBOX/AMP based on the request*/
            m_RequestDownloadHU->SenderHandleValue(srequestdownload_wr.senderhandle);
            m_RequestDownloadHU->BlockSizeValue(srequestdownload_wr.blocksize);
            m_RequestDownloadHU->SWDLChannelValue((acs::generated::fblock::SoftwareDownload::CRequestDownload::TSWDLChannel)srequestdownload_wr.swdlchannel);
            retVal = m_RequestDownloadHU->ResultAck(srequestdownload_wr.DeviceID);
            break;
        case STATUS_PROCESSINGACK:
            {
                /** Updates Request download processing ack to IBOX/AMP based on the request*/
                uint16_t senderhandle = srequestdownload_wr.senderhandle;
                m_RequestDownloadHU->SenderHandleValue(srequestdownload_wr.senderhandle);
                retVal = m_RequestDownloadHU->ProcessingAck(srequestdownload_wr.DeviceID,senderhandle);
            }
            break;
        case STATUS_STARTRESULTACK:
            {
                if(srequestdownload_wr.swdlid == SWDLID_IBOX)
                {
                    /** Updates the start result ack request for Request download to IBOX */
                    m_RequestDownloadIbox->SenderHandleValue(srequestdownload_wr.senderhandle);
                    m_RequestDownloadIbox->ModuleTypeValue((acs::generated::shadow::SoftwareDownload::CRequestDownload::TModuleType)srequestdownload_wr.moduletype);
                    m_RequestDownloadIbox->ModuleIndexValue(srequestdownload_wr.moduleindex);
                    retVal = m_RequestDownloadIbox->StartResultAck();
                }else if(srequestdownload_wr.swdlid == SWDLID_AMP)
                {
                    /** Updates the start result ack request for Request download to AMP */
                    m_RequestDownloadAmp->SenderHandleValue(srequestdownload_wr.senderhandle);
                    m_RequestDownloadAmp->ModuleTypeValue((acs::generated::shadow::SoftwareDownload::CRequestDownload::TModuleType)srequestdownload_wr.moduletype);
                    m_RequestDownloadAmp->ModuleIndexValue(srequestdownload_wr.moduleindex);
                    retVal = m_RequestDownloadAmp->StartResultAck();
                }
            }
            break;
        default:
            break;
    }
    return retVal;
}
/*
 *******************************************************************
 * Function:SetPrepareFileTransfer
 *
 * Description :Function called by the SWDL adaptor tused to transfer or request Meta
 *              information of the flashware file to the target device.
 *
 * Parameters :
 *    PrepareFileTransfer     rw    It is QDBusVariant datatype which holds the structure
 *                                  SPrepareFileTransfer containing file size, file name,
 *                                  software download id, device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetPrepareFileTransfer(const SPrepareFileTransfer &spreparefiletransfer_wr)
{
    bool retVal = false;

    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetPrepareFileTransfer API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetPrepareFileTransfer:status " << spreparefiletransfer_wr.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetPrepareFileTransfer:swdlid " << spreparefiletransfer_wr.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetPrepareFileTransfer:deviceID " << spreparefiletransfer_wr.DeviceID << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetPrepareFileTransfer:senderHandle " << spreparefiletransfer_wr.senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetPrepareFileTransfer:FileSize " << spreparefiletransfer_wr.filesize << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetPrepareFileTransfer:SWDLFileName " << spreparefiletransfer_wr.swdlFilename.toAscii().data() << MOST_LOG_ENDL;


    switch(spreparefiletransfer_wr.status)
    {
        case STATUS_RESULTACK:
            /** Updates Prepare File transfer result ack to IBOX/AMP based on the request*/
            m_PrepareFileTransferHU->SenderHandleValue(spreparefiletransfer_wr.senderhandle);
            m_PrepareFileTransferHU->FileSizeValue(spreparefiletransfer_wr.filesize);
            m_PrepareFileTransferHU->SWDLFileNameString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                                        reinterpret_cast<const uint8_t*>(spreparefiletransfer_wr.swdlFilename.toUtf8().data()),
                                                        spreparefiletransfer_wr.swdlFilename.length()+1);
            retVal = m_PrepareFileTransferHU->ResultAck(spreparefiletransfer_wr.DeviceID);
            break;
        case STATUS_PROCESSINGACK:
            {
                /** Updates Prepare File transfer processing ack to IBOX/AMP based on the request*/
                uint16_t senderhandle = spreparefiletransfer_wr.senderhandle;
                m_PrepareFileTransferHU->SenderHandleValue(spreparefiletransfer_wr.senderhandle);
                retVal = m_PrepareFileTransferHU->ProcessingAck(spreparefiletransfer_wr.DeviceID,senderhandle);
            }
            break;
        case STATUS_STARTRESULTACK:
            {
                if(spreparefiletransfer_wr.swdlid == SWDLID_IBOX)
                {
                    /** Updates the start result ack request for Prepare File transfer to IBOX */
                    m_PrepareFileTransferIbox->SenderHandleValue(spreparefiletransfer_wr.senderhandle);
                    m_PrepareFileTransferIbox->FileSizeValue(spreparefiletransfer_wr.filesize);
                    m_PrepareFileTransferIbox->SWDLFileNameString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                                                  reinterpret_cast<const uint8_t*>(spreparefiletransfer_wr.swdlFilename.toUtf8().data()),
                                                                  spreparefiletransfer_wr.swdlFilename.length()+1);
                    retVal = m_PrepareFileTransferIbox->StartResultAck();
                }
                else if(spreparefiletransfer_wr.swdlid == SWDLID_AMP)
                {
                    /** Updates the start result ack request for Prepare File transfer to AMP */
                    m_PrepareFileTransferAmp->SenderHandleValue(spreparefiletransfer_wr.senderhandle);
                    m_PrepareFileTransferAmp->FileSizeValue(spreparefiletransfer_wr.filesize);
                    m_PrepareFileTransferAmp->SWDLFileNameString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                                                 reinterpret_cast<const uint8_t*>(spreparefiletransfer_wr.swdlFilename.toUtf8().data()),
                                                                 spreparefiletransfer_wr.swdlFilename.length()+1);
                    retVal = m_PrepareFileTransferAmp->StartResultAck();
                }
            }
            break;
        default:
            break;
    }

    return retVal;
}
/*
 *******************************************************************
 * Function:SetPrepareFileTransferIBOX
 *
 * Description :Function called by the AppUpgrade used to transfer
 *              information of the flashware file to the IBOX
 *
 * Parameters :
 *    PrepareFileTransfer     rw    It is QDBusVariant datatype which holds the structure
 *                                  SPrepareFileTransfer containing file size, file name,
 *                                  filepath, transfer type (channel),blocksize.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetPrepareFileTransferIBOX(const SFileInfoforSWDL &fileInfo_wr)
{
    bool retVal = false;
    m_EndOfFile = false;

    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetPrepareFileTransferIBOX API got called " << MOST_LOG_ENDL;

    (void)memcpy(&fileTransferInfo, &fileInfo_wr, sizeof(SFileInfoforSWDL));

    QString fileName = (const char *)0;
    fileName.append(fileTransferInfo.filePath);
    m_IBOXUpgradefile = new QFile(fileName);

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " File Name is : " << fileName.toAscii().data() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " File path is : " << fileTransferInfo.filePath.toAscii().data() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " File size : " << fileTransferInfo.fileSize << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " File block size : " << fileTransferInfo.blocksize << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " File Transfer Type : " << fileTransferInfo.transfertype << MOST_LOG_ENDL;

    if (!m_IBOXUpgradefile->open(QFile::ReadOnly))
    {
        MOST_LOG_CRITICAL << LOG_TAG_SWDLCONTROL << " FAILED TO OPEN THE FILE : " << fileName.toAscii().data() << MOST_LOG_ENDL;
    }
    else
    {
        m_filearray = m_IBOXUpgradefile->read(fileTransferInfo.blocksize);

        MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " FileArray size : " << m_filearray.length() << MOST_LOG_ENDL;

        /** Updates the start result ack request for Prepare File transfer to IBOX */
        m_PrepareFileTransferIbox->SenderHandleValue((uint)0x01);
        m_PrepareFileTransferIbox->FileSizeValue(fileTransferInfo.fileSize);
        m_PrepareFileTransferIbox->SWDLFileNameString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                                      reinterpret_cast<const uint8_t *>(fileTransferInfo.fileName.toUtf8().data()),
                                                      fileTransferInfo.fileName.length()+1);

        retVal = m_PrepareFileTransferIbox->StartResultAck();

        if(retVal)
        {
            m_fileTransferIBOX = true;
        }
    }

    return retVal;
}

/*
 *******************************************************************
 * Function:SetTransferDataCMS
 *
 * Description :Function called by the SWDL adaptor used is used for sending flashware data
 *              from the control device to the target device via control channel
 *
 * Parameters :
 *    transferdataCMS     rw    It is QDBusVariant datatype which holds the structure
 *                              STransferDataCMSAMS containing block counter and data,
 *                              software download id, device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetTransferDataCMS(const STransferDataCMSAMS &stransferdataCMS_wr)
{
    bool retVal = false;

    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetTransferDataCMS API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetTransferDataCMS:status " << stransferdataCMS_wr.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetTransferDataCMS:swdlid " << stransferdataCMS_wr.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetTransferDataCMS:DeviceId " << stransferdataCMS_wr.DeviceID << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetTransferDataCMS:senderhandle " << stransferdataCMS_wr.senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetTransferDataCMS:BlockCounter " << stransferdataCMS_wr.blockcounter << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetTransferDataCMS:SWDLData " << stransferdataCMS_wr.swdldata.length() << MOST_LOG_ENDL;

    switch(stransferdataCMS_wr.status)
    {
        case STATUS_RESULTACK:
            /** Updates Transfer data CMS result ack to IBOX/AMP based on the request*/
            m_TransferDataCMSHU->SenderHandleValue(stransferdataCMS_wr.senderhandle);
            m_TransferDataCMSHU->BlockCounterValue(stransferdataCMS_wr.blockcounter);
            retVal = m_TransferDataCMSHU->ResultAck(stransferdataCMS_wr.DeviceID);
            break;
        case STATUS_PROCESSINGACK:
            {
                /** Updates Transfer data CMS processing ack to IBOX/AMP based on the request*/
                uint16_t senderhandle = stransferdataCMS_wr.senderhandle;
                m_TransferDataCMSHU->SenderHandleValue(stransferdataCMS_wr.senderhandle);
                retVal = m_TransferDataCMSHU->ProcessingAck(stransferdataCMS_wr.DeviceID,senderhandle);
            }
            break;
        case STATUS_STARTRESULTACK:
            {
                if(stransferdataCMS_wr.swdlid == SWDLID_IBOX)
                {
                    /** Updates the start result ack request for Transfer data CMS to IBOX */
                    m_TransferDataCMSIbox->SenderHandleValue(stransferdataCMS_wr.senderhandle);
                    m_TransferDataCMSIbox->BlockCounterValue(stransferdataCMS_wr.blockcounter);
                    m_TransferDataCMSIbox->SWDLDataValue(reinterpret_cast<uint8_t *>(const_cast<char *>(stransferdataCMS_wr.swdldata.data())),
                                                         stransferdataCMS_wr.swdldata.length());
                    retVal = m_TransferDataCMSIbox->StartResultAck();
                }
                else if(stransferdataCMS_wr.swdlid == SWDLID_AMP)
                {
                    /** Updates the start result ack request for Transfer data CMS to AMP */
                    m_TransferDataCMSAmp->SenderHandleValue(stransferdataCMS_wr.senderhandle);
                    m_TransferDataCMSAmp->BlockCounterValue(stransferdataCMS_wr.blockcounter);
                    m_TransferDataCMSAmp->SWDLDataValue(reinterpret_cast<uint8_t *>(const_cast<char *>(stransferdataCMS_wr.swdldata.data())),
                                                        stransferdataCMS_wr.swdldata.length());
                    retVal = m_TransferDataCMSAmp->StartResultAck();
                }
            }
            break;
        default:
            break;
    }

    return retVal;
}
/*
 *******************************************************************
 * Function:SetTransferDataAMS
 *
 * Description :Function called by the SWDL adaptor used is used for sending flashware data
 *              from the control device to the target device via asynchronous channel
 *
 * Parameters :
 *    transferdataAMS     rw    It is QDBusVariant datatype which holds the structure
 *                              STransferDataCMSAMS containing block counter and data,
 *                              software download id, device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetTransferDataAMS(const STransferDataCMSAMS &stransferdataAMS_wr)
{    
    bool retVal = false;

    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetTransferDataAMS API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetTransferDataAMS:status " << stransferdataAMS_wr.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetTransferDataAMS:swdlid " << stransferdataAMS_wr.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetTransferDataAMS:DeviceId " << stransferdataAMS_wr.DeviceID << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetTransferDataAMS:senderhandle " << stransferdataAMS_wr.senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetTransferDataAMS:BlockCounter " << stransferdataAMS_wr.blockcounter << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetTransferDataAMS:SWDLData " << stransferdataAMS_wr.swdldata.size() << MOST_LOG_ENDL;

    switch(stransferdataAMS_wr.status)
    {
        case STATUS_RESULTACK:
            /** Updates Transfer data AMS result ack to IBOX/AMP based on the request*/
            m_TransferDataAMSHU->SenderHandleValue(stransferdataAMS_wr.senderhandle);
            m_TransferDataAMSHU->BlockCounterValue(stransferdataAMS_wr.blockcounter);
            retVal = m_TransferDataAMSHU->ResultAck(stransferdataAMS_wr.DeviceID);
            break;

        case STATUS_PROCESSINGACK:
            {
                /** Updates Transfer data AMS processing ack to IBOX/AMP based on the request*/
                uint16_t senderhandle = stransferdataAMS_wr.senderhandle;
                m_TransferDataAMSHU->SenderHandleValue(stransferdataAMS_wr.senderhandle);
                retVal = m_TransferDataAMSHU->ProcessingAck(stransferdataAMS_wr.DeviceID, senderhandle);
            }
            break;

        case STATUS_STARTRESULTACK:
            {
                if(stransferdataAMS_wr.swdlid == SWDLID_IBOX)
                {
                    /** Updates the start result ack request for Transfer data AMS to IBOX */
                    m_TransferDataAMSIbox->SenderHandleValue(stransferdataAMS_wr.senderhandle);
                    m_TransferDataAMSIbox->BlockCounterValue(stransferdataAMS_wr.blockcounter);
                    m_TransferDataAMSIbox->SWDLDataValue(reinterpret_cast<uint8_t *>(const_cast<char *>(stransferdataAMS_wr.swdldata.data())),
                                                         stransferdataAMS_wr.swdldata.length());
                    retVal = m_TransferDataAMSIbox->StartResultAck();
                }
                else if(stransferdataAMS_wr.swdlid == SWDLID_AMP)
                {
                    /** Updates the start result ack request for Transfer data AMS to AMP */
                    m_TransferDataAMSAmp->SenderHandleValue(stransferdataAMS_wr.senderhandle);
                    m_TransferDataAMSAmp->BlockCounterValue(stransferdataAMS_wr.blockcounter);
                    m_TransferDataAMSAmp->SWDLDataValue(reinterpret_cast<uint8_t *>(const_cast<char *>(stransferdataAMS_wr.swdldata.data())),
                                                        stransferdataAMS_wr.swdldata.length());
                    retVal = m_TransferDataAMSAmp->StartResultAck();
                }
            }
            break;

        default:
            break;
    }

    return retVal;
}
/*
 *******************************************************************
 * Function:SetCheckActivateModule
 *
 * Description :Function called by the SWDL adaptor used to activate the new
 *              modules after download. Before it can activate the received module
 *              it check and validate the received data with the given checksum parameter
 *
 * Parameters :
 *    checkactivatemodule     rw    It is QDBusVariant datatype which holds the structure
 *                                  SCheckActivateModule containing checksum,
 *                                  software download id, device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetCheckActivateModule(const SCheckActivateModule &checkactivationmodule_wr)
{
    bool retVal = false;

    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetCheckActivateModule API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetCheckActivateModule:status " << checkactivationmodule_wr.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetCheckActivateModule:swdlid " << checkactivationmodule_wr.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetCheckActivateModule:DeviceId " << checkactivationmodule_wr.DeviceID << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetCheckActivateModule:senderHandle " << checkactivationmodule_wr.senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetCheckActivateModule:checksum " << checkactivationmodule_wr.checksum.length() << MOST_LOG_ENDL;

    switch(checkactivationmodule_wr.status)
    {
        case STATUS_RESULTACK:
            /** Updates check activate module result ack to IBOX/AMP based on the request*/
            m_CheckActivateModuleHU->SenderHandleValue(checkactivationmodule_wr.senderhandle);
            m_CheckActivateModuleHU->CheckSumValue(reinterpret_cast<uint8_t *>(const_cast<char *>(checkactivationmodule_wr.checksum.data())),
                                                   checkactivationmodule_wr.checksum.length());
            retVal = m_CheckActivateModuleHU->ResultAck(checkactivationmodule_wr.DeviceID);
            break;
        case STATUS_PROCESSINGACK:
            {
                /** Updates check activate module processing ack to IBOX/AMP based on the request*/
                uint16_t senderhandle = checkactivationmodule_wr.senderhandle;
                m_CheckActivateModuleHU->SenderHandleValue(checkactivationmodule_wr.senderhandle);
                retVal = m_CheckActivateModuleHU->ProcessingAck(checkactivationmodule_wr.DeviceID, senderhandle);
            }
            break;
        case STATUS_STARTRESULTACK:
            {
                if(checkactivationmodule_wr.swdlid == SWDLID_IBOX)
                {
                    /** Updates the start result ack request for check activate module to IBOX */
                    m_CheckActivateModuleIbox->SenderHandleValue(checkactivationmodule_wr.senderhandle);
                    m_CheckActivateModuleIbox->CheckSumValue(reinterpret_cast<uint8_t *>(const_cast<char *>(checkactivationmodule_wr.checksum.data())),
                                                             checkactivationmodule_wr.checksum.length());
                    retVal = m_CheckActivateModuleIbox->StartResultAck();
                }
                else if(checkactivationmodule_wr.swdlid == SWDLID_AMP)
                {
                    /** Updates the start result ack request for check activate module to AMP */
                    m_CheckActivateModuleAmp->SenderHandleValue(checkactivationmodule_wr.senderhandle);
                    m_CheckActivateModuleAmp->CheckSumValue(reinterpret_cast<uint8_t *>(const_cast<char *>(checkactivationmodule_wr.checksum.data())),
                                                            checkactivationmodule_wr.checksum.length());
                    retVal = m_CheckActivateModuleAmp->StartResultAck();
                }
            }
            break;
        default:
            break;
    }
    return retVal;
}
/*
 *******************************************************************
 * Function:SetRequestTransferExit
 *
 * Description :Function called by the SWDL adaptor used to indicate the end of the data and
 *              filetransfer for the active module.
 *
 * Parameters :
 *    requesttransferexit   rw    It is QDBusVariant datatype which holds the structure
 *                                SRequestTransferExit software download id, device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetRequestTransferExit(const SRequestTransferExit &requesttransferexit_wr)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetRequestTransferExit API got called " << MOST_LOG_ENDL;
    bool retVal = false;

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetRequestTransferExit:status " << requesttransferexit_wr.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetRequestTransferExit:swdlid " << requesttransferexit_wr.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetRequestTransferExit:DeviceID " << requesttransferexit_wr.DeviceID << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetRequestTransferExit:senderhandle " << requesttransferexit_wr.senderhandle << MOST_LOG_ENDL;

    switch(requesttransferexit_wr.status)
    {
    case STATUS_RESULTACK:
        /** Updates request transfer exit result ack to IBOX/AMP based on the request*/
        m_RequestTransferExitHU->SenderHandleValue(requesttransferexit_wr.senderhandle);
        retVal = m_RequestTransferExitHU->ResultAck(requesttransferexit_wr.DeviceID);
        break;
    case STATUS_PROCESSINGACK:
        {
            /** Updates request transfer exit processing ack to IBOX/AMP based on the request*/
            uint16_t senderhandle = requesttransferexit_wr.senderhandle;
            m_RequestTransferExitHU->SenderHandleValue(requesttransferexit_wr.senderhandle);
            retVal = m_RequestTransferExitHU->ProcessingAck(requesttransferexit_wr.DeviceID,senderhandle);
        }
        break;
    case STATUS_STARTRESULTACK:
        {
            if(requesttransferexit_wr.swdlid == SWDLID_IBOX)
            {
                /** Updates the start result ack request for request transfer exit to IBOX */
                m_RequestTransferExitIbox->SenderHandleValue(requesttransferexit_wr.senderhandle);
                retVal = m_RequestTransferExitIbox->StartResultAck();
            }else if(requesttransferexit_wr.swdlid == SWDLID_AMP)
            {
                /** Updates the start result ack request for request transfer exit to AMP */
                m_RequestTransferExitAmp->SenderHandleValue(requesttransferexit_wr.senderhandle);
                retVal = m_RequestTransferExitAmp->StartResultAck();
            }
        }
        break;
    default:
        break;
    }
    CheckTransferExit = true;     // hyunyong.sung, 2014.3.12 for re-transfer
    //MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " RETRANSFER _ Value init_ 1" << MOST_LOG_ENDL;
    return retVal;
}
/*
 *******************************************************************
 * Function:SetUpdateProgressingInfo
 *
 * Description :Function called by the SWDL adaptor used to indicate update progressing Info.
 *
 * Parameters :
 *    updateStatus   rw    It holds the update progress information
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLFBlockShadowControl::SetUpdateProgressingInfo(const uint updateStatus)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SetUpdateProgressingInfo API has been called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SetUpdateProgressingInfo : updateStatus " << updateStatus << MOST_LOG_ENDL;
    m_UpdateProgressingInfoHU->Value((acs::generated::fblock::SoftwareDownload::CUpdateProcessingInfo::TUpdateStatus)updateStatus);
    return m_UpdateProgressingInfoHU->UpdateShadows();
}

/*
 *******************************************************************
 * Function:ReadHWIdentifier
 *
 * Description :Invoked by Listener for Hardware identifier of Head Unit, IBOX or Ampilfier
 *              sends the result signal to SWDL Adaptor with all required data.
 *
 * Parameters :
 *    swdlId          rw   It holds the swdlID for HU or IBOX or AMP.
 *
 * Returns : void
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::ReadHWIdentifier(uint swdlid)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " ReadHWIdentifier signal got received " << MOST_LOG_ENDL;

    const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding = NULL;
    uint8_t* hwidentifierString = NULL;
    uint32_t hwidentifierStringLength = 0;

    switch(swdlid)
    {
    case SWDLID_HU:
        /** retrieveing hwidentifier string form HU Shadow on IBOX/AMP */
        m_ReadHWIdentifierHU->HWIdentifierString(&pStringEncoding,&hwidentifierString,&hwidentifierStringLength);
        break;
    case SWDLID_IBOX:
        /** retrieveing hwidentifier string form IBOX */
        m_ReadHWIdentifierIbox->HWIdentifierString(&pStringEncoding,&hwidentifierString,&hwidentifierStringLength);
        break;
    case SWDLID_AMP:
        /** retrieveing hwidentifier string form AMP */
        m_ReadHWIdentifierAmp->HWIdentifierString(&pStringEncoding,&hwidentifierString,&hwidentifierStringLength);
        break;
    default:
        break;
    }

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadHWIdentifier:hwidentifier " << reinterpret_cast<const char*>(hwidentifierString) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadHWIdentifier:swdlid " << swdlid << MOST_LOG_ENDL;
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " ReadHWIdentifier signal emitted " << MOST_LOG_ENDL;
    emit ReadHWIdentifier(swdlid,reinterpret_cast<const char*>(hwidentifierString));
}
/*
 *******************************************************************
 * Function:ReadDevName
 *
 * Description :Invoked by Listener for Device Name of Head Unit, IBOX or Ampilfier
 *              sends the result signal to SWDL Adaptor with all required data.
 *
 * Parameters :
 *    swdlId          rw   It holds the swdlID for HU or IBOX or AMP.
 *
 * Returns : void
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::ReadDevName(uint swdlid)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " ReadDevName signal got received " << MOST_LOG_ENDL;

    const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding = NULL;
    uint8_t* devicenameString = NULL;
    uint32_t devicenameStringLength = 0;
    switch(swdlid)
    {
    case SWDLID_HU:
        /** retrieveing devicename string form HU Shadow on IBOX/AMP */
        m_ReadDevNameHU->DeviceNameString(&pStringEncoding,&devicenameString,&devicenameStringLength);
        break;

    case SWDLID_IBOX:
        /** retrieveing devicename string form IBOX */
        m_ReadDevNameIbox->DeviceNameString(&pStringEncoding,&devicenameString,&devicenameStringLength);
        break;
    case SWDLID_AMP:
        /** retrieveing devicename string form AMP */
        m_ReadDevNameAmp->DeviceNameString(&pStringEncoding,&devicenameString,&devicenameStringLength);
        break;
    default:
        break;
    }

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadDevName:deviceName " << reinterpret_cast<const char*>(devicenameString) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadDevName:swdlid " << swdlid << MOST_LOG_ENDL;
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " ReadDevName signal emitted " << MOST_LOG_ENDL;
    emit ReadDevName(swdlid,reinterpret_cast<const char*>(devicenameString));
}
/*
 *******************************************************************
 * Function:ReadHWVersion
 *
 * Description :Invoked by Listener for hardware version of Head Unit, IBOX or Ampilfier
 *              sends the result signal to SWDL Adaptor with all required data.
 *
 * Parameters :
 *    swdlId          rw   It holds the swdlID for HU or IBOX or AMP.
 *
 * Returns : void
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::ReadHWVersion(uint swdlid)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " ReadHWVersion signal got received " << MOST_LOG_ENDL;

    const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding = NULL;
    uint8_t* hwversionString = NULL;
    uint32_t hwversionStringLength = 0;
    switch(swdlid)
    {
    case SWDLID_HU:
        /** retrieveing hwversion string form HU Shadow on IBOX/AMP */
        m_ReadHWVersionHU->HWVersionString(&pStringEncoding,&hwversionString,&hwversionStringLength);
        break;
    case SWDLID_IBOX:
        /** retrieveing hwversion string form IBOX */
        m_ReadHWVersionIbox->HWVersionString(&pStringEncoding,&hwversionString,&hwversionStringLength);
        break;
    case SWDLID_AMP:
        /** retrieveing hwversion string form AMP */
        m_ReadHWVersionAmp->HWVersionString(&pStringEncoding,&hwversionString,&hwversionStringLength);
        break;
    default:
        break;
    }

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadHWVersion:HWVersion " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadHWVersion:swdlid " << swdlid << MOST_LOG_ENDL;
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " ReadHWVersion signal emitted " << MOST_LOG_ENDL;
    emit ReadHWVersion(swdlid,reinterpret_cast<const char*>(hwversionString));
}
/*
 *******************************************************************
 * Function:ReadNumberofModule
 *
 * Description :Invoked by Listener for number of modules of Head Unit, IBOX or Ampilfier
 *              sends the result signal to SWDL Adaptor with all required data.
 *
 * Parameters :
 *    swdlId          rw   It holds the swdlID for HU or IBOX or AMP.
 *
 * Returns : void
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::ReadNumberofModule(uint swdlid)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " ReadNumberofModule signal got received " << MOST_LOG_ENDL;

    //uint datalength = 0;
    //QString modvalue;
    QByteArray modval = 0;

    switch(swdlid)
    {
    case SWDLID_HU:
        {
            /** retrieveing module value string form HU Shadow on IBOX/AMP */
            //datalength = m_ReadNumberofModulesHU->ModuleCountStreamLength();
            //const uint8_t *datastrHU = m_ReadNumberofModulesHU->ModuleCountValue();
            QByteArray temp((char *)m_ReadNumberofModulesHU->ModuleCountValue(), m_ReadNumberofModulesHU->ModuleCountStreamLength());
            modval = temp;
        }
        break;

    case SWDLID_IBOX:
        {
            /** retrieveing module value string form IBOX */
            //datalength = m_ReadNumberofModulesIbox->ModuleCountStreamLength();
            //const uint8_t *datastribox = m_ReadNumberofModulesIbox->ModuleCountValue();
            //QByteArray modval((char *)datastribox,datalength);	//hyunyong.sung 2013.2.18
            QByteArray temp((char *)m_ReadNumberofModulesIbox->ModuleCountValue(), m_ReadNumberofModulesIbox->ModuleCountStreamLength());
            modval = temp;
        }
        break;

    case SWDLID_AMP:
        {
            /** retrieveing module value string form AMP */
            //datalength = m_ReadNumberofModulesAmp->ModuleCountStreamLength();
            //const uint8_t *datastramp = m_ReadNumberofModulesAmp->ModuleCountValue();
            //QByteArray modval((const char *)datastramp,datalength);	//hyunyong.sung 2013.2.18
            QByteArray temp((char *)m_ReadNumberofModulesAmp->ModuleCountValue(), m_ReadNumberofModulesAmp->ModuleCountStreamLength());
            modval = temp;
        }
        break;
    default:		
        break;
    }

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadNumberofModule:swdlid " << swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadNumberofModule:modulevaluelength " << modval.length() << MOST_LOG_ENDL;
    //MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadNumberofModule:modulevalue " << modval.toHex().data() << MOST_LOG_ENDL;
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " ReadNumberofModule signal emitted " << MOST_LOG_ENDL;
    emit ReadNumberofModule(swdlid,modval);
}
/*
 *******************************************************************
 * Function:ReadModuleVersion
 *
 * Description :Invoked by Listener for module version of Head Unit, IBOX or Ampilfier
 *              sends the result signal to SWDL Adaptor with all required data.
 *
 * Parameters :
 *    swdlId          rw   It holds the swdlID for HU or IBOX or AMP.
 *
 * Returns : void
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::ReadModuleVersion(uint swdlid)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " ReadModuleVersion signal got received " << MOST_LOG_ENDL;

    const k2l::acs::protocols::most::string::IStringEncoding* encoding = NULL;
    uint8_t* data = NULL;
    uint32_t length = 0;
    struct SModuleVersionListInfo smoduleversionlistinfo;
    smoduleversionlistinfo.infosize = 0;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadModuleVersion:swdlid " << swdlid << MOST_LOG_ENDL;
    switch(swdlid)
    {
        case SWDLID_HU:
            /** retrieveing module versions info form HU Shadow on IBOX/AMP */
            for(uint index = 0; index < m_ReadModuleVersionHU->Size();index++)
            {
                struct SModuleVerisonList smoduleversionlist;
                smoduleversionlist.moduletype = m_ReadModuleVersionHU->DataModuleTypeValue(index);
                if(m_ReadModuleVersionHU->DataModuleVersionString(index,&encoding,&data,&length))
                smoduleversionlist.moduleversion.append(reinterpret_cast<const char*>(data));

                smoduleversionlistinfo.moduleversionlistinfo.append(smoduleversionlist);
                smoduleversionlistinfo.infosize++;
                MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadModuleVersion:infosize " << smoduleversionlistinfo.infosize << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadModuleVersion:moduleType " << smoduleversionlist.moduletype << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadModuleVersion:moduleversion " << smoduleversionlist.moduleversion.toAscii().data() << MOST_LOG_ENDL;
            }
            break;
        case SWDLID_IBOX:
            for(uint index = 0; index < m_ReadModuleVersionIbox->Size();index++)
            {
                /** retrieveing module versions info form IBOX */
                struct SModuleVerisonList smoduleversionlist;
                smoduleversionlist.moduletype = m_ReadModuleVersionIbox->DataModuleTypeValue(index);
                if(m_ReadModuleVersionIbox->DataModuleVersionString(index,&encoding,&data,&length))
                smoduleversionlist.moduleversion.append(reinterpret_cast<const char*>(data));

                smoduleversionlistinfo.moduleversionlistinfo.append(smoduleversionlist);
                smoduleversionlistinfo.infosize++;
                MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadModuleVersion:infosize " << smoduleversionlistinfo.infosize << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadModuleVersion:moduleType " << smoduleversionlist.moduletype << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadModuleVersion:moduleversion " << smoduleversionlist.moduleversion.toAscii().data() << MOST_LOG_ENDL;
            }
            break;
        case SWDLID_AMP:
            for(uint index = 0; index < m_ReadModuleVersionAmp->Size();index++)
            {
                /** retrieveing module versions info form AMP */
                struct SModuleVerisonList smoduleversionlist;
                smoduleversionlist.moduletype = m_ReadModuleVersionAmp->DataModuleTypeValue(index);
                if(m_ReadModuleVersionAmp->DataModuleVersionString(index,&encoding,&data,&length))
                smoduleversionlist.moduleversion.append(reinterpret_cast<const char*>(data));

                smoduleversionlistinfo.moduleversionlistinfo.append(smoduleversionlist);
                smoduleversionlistinfo.infosize++;
                MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadModuleVersion:infosize " << smoduleversionlistinfo.infosize << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadModuleVersion:moduleType " << smoduleversionlist.moduletype << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReadModuleVersion:moduleversion " << smoduleversionlist.moduleversion.toAscii().data() << MOST_LOG_ENDL;
            }
            break;
        default:
            break;
    }
    /** Packing structure to type QDBusVariant  */
    QVariant moduleversionVar;
    QDBusVariant moduleversiondVar;

    QDBusArgument moduleversionArg;
    moduleversionArg << smoduleversionlistinfo;
    moduleversionVar.setValue(moduleversionArg);

    moduleversiondVar.setVariant(moduleversionVar);
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " ReadModuleVersion signal emitted " << MOST_LOG_ENDL;
    emit ReadModuleVersion(swdlid,moduleversiondVar);
}

/*
 *******************************************************************
 * Function:ReprogrammingMode
 *
 * Description :Invoked by handler for reprogramming mode of Head Unit, IBOX or Ampilfier
 *              sends the result signal to SWDL Adaptor with all required data.
 *
 * Parameters :
 *    payloadLength         rw   length of the payload data from hanlder.
 *    payload               rw   payload data from handler.
 *    sreprogrammingmode    rw   structure of type SReporgrammingMode.
 *
 * Returns : none
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::ReprogrammingMode(uint32_t payloadLength, const uint8_t *payload, SReporgrammingMode &sreprogrammingmode )
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " ReprogrammingMode signal got received " << MOST_LOG_ENDL;

    QVariant reprogrammingmodeQVar;
    QDBusVariant reprogrammingmodeQDBVar;
    QDBusArgument reprogrammingmodeQDBArg;
    uint16_t senderhandle = 0;

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReprogrammingMode:status " << sreprogrammingmode.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReprogrammingMode:swdlid " << sreprogrammingmode.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReprogrammingMode:DeviceId " << sreprogrammingmode.DeviceID << MOST_LOG_ENDL;

    switch(sreprogrammingmode.status)
    {
        case STATUS_STARTRESULTACK:
            /** retrieveing start result ack request for reprogramming mode form IBOX/AMP */
            m_ReprogrammingModeHU->ParseStartResultAck(payloadLength,payload);
            sreprogrammingmode.senderhandle = m_ReprogrammingModeHU->SenderHandleValue();
            sreprogrammingmode.SWDLmode = m_ReprogrammingModeHU->SWDLModeValue();
            break;

        case STATUS_PROCESSINGACK:
            {
                if(sreprogrammingmode.swdlid == SWDLID_IBOX)
                {
                    /** retrieveing processing ack request for reprogramming mode form IBOX */
                   m_ReprogrammingModeIbox->ParseSenderHandle(payloadLength,payload,&senderhandle);
                   sreprogrammingmode.senderhandle = senderhandle;

                }else if(sreprogrammingmode.swdlid == SWDLID_AMP)
                {
                    /** retrieveing processing ack request for reprogramming mode form AMP */
                    m_ReprogrammingModeAmp->ParseSenderHandle(payloadLength,payload,&senderhandle);
                    sreprogrammingmode.senderhandle = senderhandle;
                }
            }
            break;
        case STATUS_RESULTACK:
            {
                if(sreprogrammingmode.swdlid == SWDLID_IBOX)
                {
                    /** retrieveing result ack request for reprogramming mode form IBOX */
                    m_ReprogrammingModeIbox->ParseResultAck(payloadLength,payload);
                    sreprogrammingmode.senderhandle = m_ReprogrammingModeIbox->SenderHandleValue();
                    sreprogrammingmode.SWDLmode = m_ReprogrammingModeIbox->SWDLModeValue();

                }else if(sreprogrammingmode.swdlid == SWDLID_AMP)
                {
                    /** retrieveing result ack request for reprogramming mode form AMP */
                    m_ReprogrammingModeAmp->ParseResultAck(payloadLength,payload);
                    sreprogrammingmode.senderhandle = m_ReprogrammingModeAmp->SenderHandleValue();
                    sreprogrammingmode.SWDLmode = m_ReprogrammingModeAmp->SWDLModeValue();
                }
            }
            break;
        case STATUS_ERROR:
            break;
        case STATUS_ERRORACK:
            break;
        case STATUS_ABORTACK:
            break;
        case STATUS_UNDEFINED:
             break;
        default:
            break;
    }

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReprogrammingMode:senderhandle " << sreprogrammingmode.senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " ReprogrammingMode:SWDLmode " << sreprogrammingmode.SWDLmode << MOST_LOG_ENDL;

    /** Packing structure to type QDBusVariant  */
    reprogrammingmodeQDBArg << sreprogrammingmode;
    reprogrammingmodeQVar.setValue(reprogrammingmodeQDBArg);
    reprogrammingmodeQDBVar.setVariant(reprogrammingmodeQVar);
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " ReprogrammingMode signal emitted " << MOST_LOG_ENDL;
    emit ReprogrammingMode(reprogrammingmodeQDBVar);

}
/*
 *******************************************************************
 * Function:InitUpdateResult
 *
 * Description :Invoked by handler for Initializing Update of Head Unit, IBOX or Ampilfier
 *              sends the result signal to SWDL Adaptor with all required data.
 *
 * Parameters :
 *    payloadLength         rw   length of the payload data from hanlder.
 *    payload               rw   payload data from handler.
 *    sinitupdate           rw   structure of type SInitUpdate.
 *
 * Returns : none
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::InitUpdateResult(uint32_t payloadLength, const uint8_t *payload, SInitUpdate &sinitupdate)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " InitUpdateResult signal got received " << MOST_LOG_ENDL;

    QVariant initupdateQVar;
    QDBusVariant initupdateQDBVar;
    QDBusArgument initupdateQDBArg;
    const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding = NULL;
    uint8_t *diskidstring = NULL;
    uint32_t diskidlength = 0;
    uint16_t senderhandle = 0;

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " InitUpdateResult:status " << sinitupdate.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " InitUpdateResult:swdlid " << sinitupdate.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " InitUpdateResult:DeviceID " << sinitupdate.DeviceID << MOST_LOG_ENDL;

    switch(sinitupdate.status)
    {
        case STATUS_STARTRESULTACK:
            /** retrieveing start result ack request for init update info form IBOX/AMP */
            m_InitUpdateHU->ParseStartResultAck(payloadLength,payload);

            m_InitUpdateHU->DiscIDString(&pStringEncoding,&diskidstring,&diskidlength);
            sinitupdate.senderhandle = m_InitUpdateHU->SenderHandleValue();
            sinitupdate.day = (uint)m_InitUpdateHU->DayValue();
            sinitupdate.month = (uint)m_InitUpdateHU->MonthValue();
            sinitupdate.year = m_InitUpdateHU->YearValue();
            sinitupdate.diskID = reinterpret_cast<const char*>(diskidstring);
            break;
        case STATUS_PROCESSINGACK:
            {
                if(sinitupdate.swdlid == SWDLID_IBOX)
                {
                    /** retrieveing processing ack request for init update form IBOX */
                    m_InitUpdateIbox->ParseSenderHandle(payloadLength,payload,&senderhandle);
                    sinitupdate.senderhandle = senderhandle;
                }else if(sinitupdate.swdlid == SWDLID_AMP)
                {
                    /** retrieveing processing ack request for init update form AMP */
                    m_InitUpdateAmp->ParseSenderHandle(payloadLength,payload,&senderhandle);
                    sinitupdate.senderhandle = senderhandle;
                }
            }
            break;
        case STATUS_RESULTACK:
            {
                if(sinitupdate.swdlid == SWDLID_IBOX)
                {
                    /** retrieveing result ack request for init update info form IBOX */
                    m_InitUpdateIbox->ParseResultAck(payloadLength,payload);
                    m_InitUpdateIbox->DiscIDString(&pStringEncoding,&diskidstring,&diskidlength);
                    sinitupdate.senderhandle = m_InitUpdateIbox->SenderHandleValue();
                    sinitupdate.day = (uint)m_InitUpdateIbox->DayValue();
                    sinitupdate.month = (uint)m_InitUpdateIbox->MonthValue();
                    sinitupdate.year = m_InitUpdateIbox->YearValue();
                    sinitupdate.diskID = reinterpret_cast<const char*>(diskidstring);
                }else if(sinitupdate.swdlid == SWDLID_AMP)
                {
                    /** retrieveing result ack request for init update info form AMP */
                    m_InitUpdateAmp->ParseResultAck(payloadLength,payload);
                    m_InitUpdateAmp->DiscIDString(&pStringEncoding,&diskidstring,&diskidlength);
                    sinitupdate.senderhandle = m_InitUpdateAmp->SenderHandleValue();
                    sinitupdate.day = (uint)m_InitUpdateAmp->DayValue();
                    sinitupdate.month = (uint)m_InitUpdateAmp->MonthValue();
                    sinitupdate.year = m_InitUpdateAmp->YearValue();
                    sinitupdate.diskID = reinterpret_cast<const char*>(diskidstring);
                }
            }
            break;
        case STATUS_ERROR:
            break;
        case STATUS_ERRORACK:
            break;
        case STATUS_ABORTACK:
            break;
        case STATUS_UNDEFINED:
             break;
        default:
            break;
    }
    /** Packing structure to type QDBusVariant  */

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " InitUpdateResult:senderhandle " << sinitupdate.senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " InitUpdateResult:day " << sinitupdate.day << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " InitUpdateResult:month " << sinitupdate.month << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " InitUpdateResult:year " << sinitupdate.year << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " InitUpdateResult:diskID " << sinitupdate.diskID.toAscii().data() << MOST_LOG_ENDL;

    initupdateQDBArg << sinitupdate;
    initupdateQVar.setValue(initupdateQDBArg);
    initupdateQDBVar.setVariant(initupdateQVar);
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " InitUpdateResult signal emitted " << MOST_LOG_ENDL;
    emit InitUpdateResult(initupdateQDBVar);
}
/*
 *******************************************************************
 * Function:EraseMemory
 *
 * Description :Invoked by handler for Erase memory of Head Unit, IBOX or Ampilfier
 *              sends the result signal to SWDL Adaptor with all required data.
 *
 * Parameters :
 *    payloadLength         rw   length of the payload data from hanlder.
 *    payload               rw   payload data from handler.
 *    serasememory           rw   structure of type SEraseMemory.
 *
 * Returns : none
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::EraseMemory(uint32_t payloadLength, const uint8_t *payload, SEraseMemory &serasememory)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " EraseMemory signal got received " << MOST_LOG_ENDL;

    QVariant erasememoryQVar;
    QDBusVariant erasememoryQDBVar;
    QDBusArgument erasememoryQDBArg;
    uint16_t senderhandle = 0;

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " EraseMemory:status " << serasememory.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " EraseMemory:swdlid " << serasememory.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " EraseMemory:DeviceID " << serasememory.DeviceID << MOST_LOG_ENDL;

    switch(serasememory.status)
    {
        case STATUS_STARTRESULTACK:
            /** retrieveing start result ack request for erase memory info form IBOX/AMP */
            m_EraseMemoryHU->ParseStartResultAck(payloadLength,payload);

            serasememory.Senderhandle = m_EraseMemoryHU->SenderHandleValue();
            serasememory.moduletype = m_EraseMemoryHU->ModuleTypeValue();
            serasememory.moduleindex = (uint)m_EraseMemoryHU->ModuleIndexValue();
            break;
        case STATUS_PROCESSINGACK:
            {
                if(serasememory.swdlid == SWDLID_IBOX)
                {
                    /** retrieveing processing ack request for erase memory info form IBOX */
                    m_EraseMemoryIbox->ParseSenderHandle(payloadLength,payload,&senderhandle);
                    serasememory.Senderhandle = senderhandle;
                }else if(serasememory.swdlid == SWDLID_AMP)
                {
                    /** retrieveing processing ack request for erase memory info form AMP */
                    m_EraseMemoryAmp->ParseSenderHandle(payloadLength,payload,&senderhandle);
                    serasememory.Senderhandle = senderhandle;
                }
            }
            break;
        case STATUS_RESULTACK:
            {
                if(serasememory.swdlid == SWDLID_IBOX)
                {
                    /** retrieveing result ack request for erase memory info form IBOX */
                    m_EraseMemoryIbox->ParseResultAck(payloadLength,payload);

                    serasememory.Senderhandle = m_EraseMemoryIbox->SenderHandleValue();
                    serasememory.moduletype = m_EraseMemoryIbox->ModuleTypeValue();
                    serasememory.moduleindex = (uint)m_EraseMemoryIbox->ModuleIndexValue();

                }else if(serasememory.swdlid == SWDLID_AMP)
                {
                    /** retrieveing result ack request for erase memory info form AMP */
                    m_EraseMemoryAmp->ParseResultAck(payloadLength,payload);

                    serasememory.Senderhandle = m_EraseMemoryAmp->SenderHandleValue();
                    serasememory.moduletype = m_EraseMemoryAmp->ModuleTypeValue();
                    serasememory.moduleindex = m_EraseMemoryAmp->ModuleIndexValue();
                }
            }
            break;
        case STATUS_ERROR:
            break;
        case STATUS_ERRORACK:
            break;
        case STATUS_ABORTACK:
            break;
        case STATUS_UNDEFINED:
             break;
        default:
            break;
    }
    /** Packing structure to type QDBusVariant  */
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " EraseMemory:senderhandle " << serasememory.Senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " EraseMemory:moduletype " << serasememory.moduletype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " EraseMemory:moduleindex " << serasememory.moduleindex << MOST_LOG_ENDL;

    erasememoryQDBArg << serasememory;
    erasememoryQVar.setValue(erasememoryQDBArg);
    erasememoryQDBVar.setVariant(erasememoryQVar);
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " EraseMemory signal emitted " << MOST_LOG_ENDL;
    emit EraseMemory(erasememoryQDBVar);
}
/*
 *******************************************************************
 * Function:RequestDownload
 *
 * Description :Invoked by handler for Request download of Head Unit, IBOX or Ampilfier
 *              sends the result signal to SWDL Adaptor with all required data.
 *
 * Parameters :
 *    payloadLength         rw   length of the payload data from hanlder.
 *    payload               rw   payload data from handler.
 *    srequestdownload      rw   structure of type SRequestDownload.
 *
 * Returns : none
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::RequestDownload(uint32_t payloadLength, const uint8_t *payload, SRequestDownload &srequestdownload)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " RequestDownload signal got received " << MOST_LOG_ENDL;

    QVariant requestdownloadQVar;
    QDBusVariant requestdownloadQDBVar;
    QDBusArgument requestdownloadQDBArg;
    uint16_t senderhandle = 0;

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " RequestDownload:status " << srequestdownload.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " RequestDownload:swdlid " << srequestdownload.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " RequestDownload:DeviceId " << srequestdownload.DeviceID << MOST_LOG_ENDL;

    switch(srequestdownload.status)
    {
        case STATUS_STARTRESULTACK:
            /** retrieveing start result ack request for request download info form IBOX/AMP */
            m_RequestDownloadHU->ParseStartResultAck(payloadLength,payload);

            srequestdownload.senderhandle = m_RequestDownloadHU->SenderHandleValue();
            srequestdownload.blocksize = 0;
            srequestdownload.moduletype = m_RequestDownloadHU->ModuleTypeValue();
            srequestdownload.moduleindex = (uint)m_RequestDownloadHU->ModuleIndexValue();
            srequestdownload.swdlchannel = 0;
            break;

        case STATUS_PROCESSINGACK:
            {
                if(srequestdownload.swdlid == SWDLID_IBOX)
                {
                    /** retrieveing processing ack request for request download info form IBOX */
                    m_RequestDownloadIbox->ParseSenderHandle(payloadLength,payload,&senderhandle);
                    srequestdownload.senderhandle = senderhandle;
                }else if(srequestdownload.swdlid == SWDLID_AMP)
                {
                    /** retrieveing processing ack request for request download info form AMP */
                    m_RequestDownloadAmp->ParseSenderHandle(payloadLength,payload,&senderhandle);
                    srequestdownload.senderhandle = senderhandle;
                }
            }
            break;
        case STATUS_RESULTACK:
            {
                if(srequestdownload.swdlid == SWDLID_IBOX)
                {
                    /** retrieveing result ack request for request download info form IBOX */
                    m_RequestDownloadIbox->ParseResultAck(payloadLength,payload);
                    srequestdownload.senderhandle = m_RequestDownloadIbox->SenderHandleValue();
                    srequestdownload.blocksize = m_RequestDownloadIbox->BlockSizeValue();
                    srequestdownload.moduletype = m_RequestDownloadIbox->ModuleTypeValue();
                    srequestdownload.moduleindex = (uint)m_RequestDownloadIbox->ModuleIndexValue();
                    srequestdownload.swdlchannel = m_RequestDownloadIbox->SWDLChannelValue();
                }else if(srequestdownload.swdlid == SWDLID_AMP)
                {
                    /** retrieveing result ack request for request download info form AMP */
                    m_RequestDownloadAmp->ParseResultAck(payloadLength,payload);
                    srequestdownload.senderhandle = m_RequestDownloadAmp->SenderHandleValue();
                    srequestdownload.blocksize = m_RequestDownloadAmp->BlockSizeValue();
                    srequestdownload.moduletype = m_RequestDownloadAmp->ModuleTypeValue();
                    srequestdownload.moduleindex = (uint)m_RequestDownloadAmp->ModuleIndexValue();
                    srequestdownload.swdlchannel = m_RequestDownloadAmp->SWDLChannelValue();
                }
            }
            break;
        case STATUS_ERROR:
            break;
        case STATUS_ERRORACK:
            break;
        case STATUS_ABORTACK:
            break;
        case STATUS_UNDEFINED:
             break;
        default:
            break;
    }
    /** Packing structure to type QDBusVariant  */

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " RequestDownload:senderHandle " << srequestdownload.senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " RequestDownload:blocksize " << srequestdownload.blocksize << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " RequestDownload:moduletype " << srequestdownload.moduletype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " RequestDownload:moduleindex " << srequestdownload.moduleindex << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " RequestDownload:swdlchannel " << srequestdownload.swdlchannel << MOST_LOG_ENDL;

    requestdownloadQDBArg << srequestdownload;
    requestdownloadQVar.setValue(requestdownloadQDBArg);
    requestdownloadQDBVar.setVariant(requestdownloadQVar);

    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " RequestDownload signal emitted " << MOST_LOG_ENDL;
    emit RequestDownload(requestdownloadQDBVar);
}
/*
 *******************************************************************
 * Function:PrepareFileTransfer
 *
 * Description :Invoked by handler for Prepare File Transfer of Head Unit, IBOX or Ampilfier
 *              sends the result signal to SWDL Adaptor with all required data.
 *
 * Parameters :
 *    payloadLength         rw   length of the payload data from hanlder.
 *    payload               rw   payload data from handler.
 *    spreparefiletransfer  rw   structure of type SPrepareFileTransfer.
 *
 * Returns : none
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::PrepareFileTransfer(uint32_t payloadLength, const uint8_t *payload, SPrepareFileTransfer &spreparefiletransfer)
{   
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " PrepareFileTransfer signal got received " << MOST_LOG_ENDL;
    QVariant preparefiletransferQVar;
    QDBusVariant rpreparefiletransferQDBVar;
    QDBusArgument preparefiletransferQDBArg;
    uint16_t senderhandle = 0;
    const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding = NULL;
    uint8_t *filenamestring = NULL;
    uint32_t filenamelength = 0;

    //gettimeofday(&tval, &tzone);
    //timem=localtime(&tval.tv_sec);

    //MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SystemTime when PrepareFileTransfer got invoked : " << timem->tm_hour << ":"
    //          << timem->tm_min << ":" << timem->tm_sec << ":" << tval.tv_usec << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " PrepareFileTransfer:status " << spreparefiletransfer.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " PrepareFileTransfer:swdlid " << spreparefiletransfer.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " PrepareFileTransfer:deviceId " << spreparefiletransfer.DeviceID << MOST_LOG_ENDL;

    switch(spreparefiletransfer.status)
    {
        case STATUS_STARTRESULTACK:
            /** retrieveing start result ack request for prepare file transfer info form IBOX/AMP */
            m_PrepareFileTransferHU->ParseStartResultAck(payloadLength,payload);

            m_PrepareFileTransferHU->SWDLFileNameString(&pStringEncoding,&filenamestring,&filenamelength);
            spreparefiletransfer.senderhandle = m_PrepareFileTransferHU->SenderHandleValue();
            spreparefiletransfer.filesize = m_PrepareFileTransferHU->FileSizeValue();
            spreparefiletransfer.swdlFilename = reinterpret_cast<const char*>(filenamestring);
            break;

        case STATUS_PROCESSINGACK:
            {
                if(spreparefiletransfer.swdlid == SWDLID_IBOX)
                {
                    /** retrieveing processing ack request for prepare file transfer info form IBOX */
                    m_PrepareFileTransferIbox->ParseSenderHandle(payloadLength,payload,&senderhandle);
                    spreparefiletransfer.senderhandle = senderhandle;
                }else if(spreparefiletransfer.swdlid == SWDLID_AMP)
                {
                    /** retrieveing processing ack request for prepare file transfer info form AMP */
                    m_PrepareFileTransferAmp->ParseSenderHandle(payloadLength,payload,&senderhandle);
                    spreparefiletransfer.senderhandle = senderhandle;
                }
            }
            break;
        case STATUS_RESULTACK:
            {
                if(spreparefiletransfer.swdlid == SWDLID_IBOX)
                {
                    /** retrieveing result ack request for prepare file transfer info form IBOX */
                    m_PrepareFileTransferIbox->ParseResultAck(payloadLength,payload);

                    m_PrepareFileTransferIbox->SWDLFileNameString(&pStringEncoding,&filenamestring,&filenamelength);
                    spreparefiletransfer.senderhandle = m_PrepareFileTransferIbox->SenderHandleValue();
                    spreparefiletransfer.filesize = m_PrepareFileTransferIbox->FileSizeValue();
                    spreparefiletransfer.swdlFilename = reinterpret_cast<const char*>(filenamestring);

                    if(m_fileTransferIBOX)
                    {
                        TransferFileIBOXAMSCMS();
                    }

                }else if(spreparefiletransfer.swdlid == SWDLID_AMP)
                {
                    /** retrieveing result ack request for prepare file transfer info form AMP */
                    m_PrepareFileTransferAmp->ParseResultAck(payloadLength,payload);

                    m_PrepareFileTransferAmp->SWDLFileNameString(&pStringEncoding,&filenamestring,&filenamelength);
                    spreparefiletransfer.senderhandle = m_PrepareFileTransferAmp->SenderHandleValue();
                    spreparefiletransfer.filesize = m_PrepareFileTransferAmp->FileSizeValue();
                    spreparefiletransfer.swdlFilename = reinterpret_cast<const char*>(filenamestring);
                }
            }
            break;
        case STATUS_ERROR:
            break;
        case STATUS_ERRORACK:
            break;
        case STATUS_ABORTACK:
            break;
        case STATUS_UNDEFINED:
             break;
        default:
            break;
    }
    /** Packing structure to type QDBusVariant  */
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " PrepareFileTransfer:senderHandle " << spreparefiletransfer.senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " PrepareFileTransfer:filesize " << spreparefiletransfer.filesize << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " PrepareFileTransfer:swdlFilename " << spreparefiletransfer.swdlFilename.toAscii().data() << MOST_LOG_ENDL;

    preparefiletransferQDBArg << spreparefiletransfer;
    preparefiletransferQVar.setValue(preparefiletransferQDBArg);
    rpreparefiletransferQDBVar.setVariant(preparefiletransferQVar);
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " PrepareFileTransfer signal emitted " << MOST_LOG_ENDL;
    emit PrepareFileTransfer(rpreparefiletransferQDBVar);
}
/*
 *******************************************************************
 * Function:TransferDataCMS
 *
 * Description :Invoked by handler for TransferDataCMS of Head Unit, IBOX or Ampilfier
 *              sends the result signal to SWDL Adaptor with all required data.
 *
 * Parameters :
 *    payloadLength         rw   length of the payload data from hanlder.
 *    payload               rw   payload data from handler.
 *    stransferdataCMS      rw   structure of type STransferDataCMSAMS.
 *
 * Returns : none
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::TransferDataCMS(uint32_t payloadLength, const uint8_t *payload, STransferDataCMSAMS &stransferdataCMS)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " TransferDataCMS signal got received " << MOST_LOG_ENDL;

    QVariant transferdataCMSQVar;
    QDBusVariant transferdataCMSQDBVar;
    QDBusArgument transferdataCMSQDBArg;
    uint16_t senderhandle = 0;
    //uint swdldatalength = 0;
    //uint8_t *swdldataptr = NULL;

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " TransferDataCMS:status " << stransferdataCMS.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " TransferDataCMS:swdlid " << stransferdataCMS.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " TransferDataCMS:deviceId " << stransferdataCMS.DeviceID << MOST_LOG_ENDL;

    switch(stransferdataCMS.status)
    {
        case STATUS_STARTRESULTACK:
        {
            /** retrieveing start result ack request for Transfer data CMS info form IBOX/AMP */
            m_TransferDataCMSHU->ParseStartResultAck(payloadLength,payload);
            //swdldatalength = m_TransferDataCMSHU->SWDLDataStreamLength();
            //swdldataptr = m_TransferDataCMSHU->SWDLDataValue();
            stransferdataCMS.senderhandle = m_TransferDataCMSHU->SenderHandleValue();
            stransferdataCMS.blockcounter = m_TransferDataCMSHU->BlockCounterValue();
            //stransferdataCMS.swdldata = QByteArray::fromRawData((const char *)swdldataptr,swdldatalength);
            QByteArray swdldata((const char *)m_TransferDataCMSIbox->SWDLDataValue(), m_TransferDataCMSIbox->SWDLDataStreamLength());
            stransferdataCMS.swdldata = swdldata;

        }
        break;
        case STATUS_PROCESSINGACK:
        {
            if(stransferdataCMS.swdlid == SWDLID_IBOX)
            {
                /** retrieveing processing ack request for Transfer data CMS info form IBOX */
                m_TransferDataCMSIbox->ParseSenderHandle(payloadLength,payload,&senderhandle);
                stransferdataCMS.senderhandle = senderhandle;
            }
            else if(stransferdataCMS.swdlid == SWDLID_AMP)
            {
                /** retrieveing processing ack request for Transfer data CMS info form IBOX */
                m_TransferDataCMSAmp->ParseSenderHandle(payloadLength,payload,&senderhandle);
                stransferdataCMS.senderhandle = senderhandle;
            }
        }
        break;
        case STATUS_RESULTACK:
        {
            if(stransferdataCMS.swdlid == SWDLID_IBOX)
            {
                /** retrieveing result ack request for Transfer data CMS info form IBOX */
                m_TransferDataCMSIbox->ParseResultAck(payloadLength,payload);

                stransferdataCMS.senderhandle = m_TransferDataCMSIbox->SenderHandleValue();
                stransferdataCMS.blockcounter = m_TransferDataCMSIbox->BlockCounterValue();
                //swdldatalength = m_TransferDataCMSIbox->SWDLDataStreamLength();
                //swdldataptr = m_TransferDataCMSIbox->SWDLDataValue();
                //stransferdataCMS.swdldata = QByteArray::fromRawData((const char *)swdldataptr,swdldatalength);
                QByteArray swdldata((const char *)m_TransferDataCMSIbox->SWDLDataValue(), m_TransferDataCMSIbox->SWDLDataStreamLength());
                stransferdataCMS.swdldata = swdldata;

                if(m_fileTransferIBOX)
                {
                    TransferFileIBOXAMSCMS();
                }

            }
            else if(stransferdataCMS.swdlid == SWDLID_AMP)
            {
                /** retrieveing result ack request for Transfer data CMS info form AMP */
                m_TransferDataCMSAmp->ParseResultAck(payloadLength,payload);
                //swdldatalength = m_TransferDataCMSAmp->SWDLDataStreamLength();
                //swdldataptr = m_TransferDataCMSAmp->SWDLDataValue();
                stransferdataCMS.senderhandle = m_TransferDataCMSAmp->SenderHandleValue();
                stransferdataCMS.blockcounter = m_TransferDataCMSAmp->BlockCounterValue();
                //stransferdataCMS.swdldata = QByteArray::fromRawData((const char *)swdldataptr,swdldatalength);
                QByteArray swdldata((const char *)m_TransferDataCMSIbox->SWDLDataValue(), m_TransferDataCMSIbox->SWDLDataStreamLength());
                stransferdataCMS.swdldata = swdldata;
            }
        }
        break;
        case STATUS_ERRORACK:
        break;
        default:
        break;
    }

    /** Packing structure to type QDBusVariant  */
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " TransferDataCMS:senderhandle " << stransferdataCMS.senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " TransferDataCMS:blockcounter " << stransferdataCMS.blockcounter << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " TransferDataCMS:swdldata length" << stransferdataCMS.swdldata.length() << MOST_LOG_ENDL;

    transferdataCMSQDBArg << stransferdataCMS;
    transferdataCMSQVar.setValue(transferdataCMSQDBArg);
    transferdataCMSQDBVar.setVariant(transferdataCMSQVar);

    emit TransferDataCMS(transferdataCMSQDBVar);
}
/*
 *******************************************************************
 * Function:TransferDataAMS
 *
 * Description :Invoked by handler for TransferDataAMS of Head Unit, IBOX or Ampilfier
 *              sends the result signal to SWDL Adaptor with all required data.
 *
 * Parameters :
 *    payloadLength         rw   length of the payload data from hanlder.
 *    payload               rw   payload data from handler.
 *    stransferdataAMS      rw   structure of type STransferDataCMSAMS.
 *
 * Returns : none
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::TransferDataAMS(uint32_t payloadLength, const uint8_t *payload, STransferDataCMSAMS &stransferdataAMS)
{
    QVariant transferdataAMSQVar;
    QDBusVariant transferdataAMSQDBVar;
    QDBusArgument transferdataAMSQDBArg;

    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " TransferDataAMS signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " TransferDataAMS:Status = " << stransferdataAMS.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " TransferDataAMS:SWDL ID = " << stransferdataAMS.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " TransferDataAMS:Device ID = " << stransferdataAMS.DeviceID << MOST_LOG_ENDL;

    switch(stransferdataAMS.status)
    {
        case STATUS_STARTRESULTACK:
        {
            /** retrieveing start result ack request for transfer data AMS info form IBOX/AMP */
            m_TransferDataAMSHU->ParseStartResultAck(payloadLength,payload);
            stransferdataAMS.senderhandle = m_TransferDataAMSHU->SenderHandleValue();
            stransferdataAMS.blockcounter = m_TransferDataAMSHU->BlockCounterValue();
            QByteArray swdldata((const char *)m_TransferDataAMSHU->SWDLDataValue(), m_TransferDataAMSHU->SWDLDataStreamLength());
            stransferdataAMS.swdldata = swdldata;

            /** Packing structure to type QDBusVariant  */
            MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " TransferDataAMS:Sender Handle = " << stransferdataAMS.senderhandle << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " TransferDataAMS:Block Counter = " << stransferdataAMS.blockcounter << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " TransferDataAMS:SWDL Data Length = " << stransferdataAMS.swdldata.length() << MOST_LOG_ENDL;
        }
        break;
        case STATUS_PROCESSINGACK:
        {
            if(stransferdataAMS.swdlid == SWDLID_IBOX)
            {
                /** retrieveing processing ack request for Transfer data AMS info form IBOX */
                m_TransferDataAMSIbox->ParseSenderHandle(payloadLength, payload, reinterpret_cast<ushort *>(&stransferdataAMS.senderhandle));
            }
            else if(stransferdataAMS.swdlid == SWDLID_AMP)
            {
                /** retrieveing processing ack request for Transfer data AMS info form AMP */
                m_TransferDataAMSAmp->ParseSenderHandle(payloadLength, payload, reinterpret_cast<ushort *>(&stransferdataAMS.senderhandle));
            }            

            MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " TransferDataAMS:Sender Handle = " << stransferdataAMS.senderhandle << MOST_LOG_ENDL;
        }
        break;
        case STATUS_RESULTACK:
        {
            if(stransferdataAMS.swdlid == SWDLID_IBOX)
            {
                /** retrieveing result ack request for Transfer data AMS info form IBOX */
                m_TransferDataAMSIbox->ParseResultAck(payloadLength,payload);
                stransferdataAMS.senderhandle = m_TransferDataAMSIbox->SenderHandleValue();
                stransferdataAMS.blockcounter = m_TransferDataAMSIbox->BlockCounterValue();
                QByteArray swdldata((const char *)m_TransferDataAMSIbox->SWDLDataValue(), m_TransferDataAMSIbox->SWDLDataStreamLength());
                stransferdataAMS.swdldata = swdldata;

                if(m_fileTransferIBOX)
                {
                    TransferFileIBOXAMSCMS();
                }

            }
            else if(stransferdataAMS.swdlid == SWDLID_AMP)
            {
                /** retrieveing result ack request for Transfer data AMS info form AMP */
                m_TransferDataAMSAmp->ParseResultAck(payloadLength,payload);
                stransferdataAMS.senderhandle = m_TransferDataAMSAmp->SenderHandleValue();
                stransferdataAMS.blockcounter = m_TransferDataAMSAmp->BlockCounterValue();
                QByteArray swdldata((const char *)m_TransferDataAMSAmp->SWDLDataValue(), m_TransferDataAMSAmp->SWDLDataStreamLength());
                stransferdataAMS.swdldata = swdldata;
            }

            /** Packing structure to type QDBusVariant  */
            MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " TransferDataAMS:Sender Handle = " << stransferdataAMS.senderhandle << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " TransferDataAMS:Block Counter = " << stransferdataAMS.blockcounter << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " TransferDataAMS:SWDL Data Length = " << stransferdataAMS.swdldata.length() << MOST_LOG_ENDL;
        }
        break;
        case STATUS_ERRORACK:
        {
            uint8_t errorCode = payload[0];
            QByteArray errorInfo(reinterpret_cast<const char *>(payload[1]), payloadLength-1);

            //Retrieved error information correctly.
            MOST_LOG_CRITICAL << LOG_TAG_SWDLCONTROL << " Error code = " << errorCode << MOST_LOG_ENDL;
            MOST_LOG_CRITICAL << LOG_TAG_SWDLCONTROL << " Error Info Length = " << errorInfo.length() << MOST_LOG_ENDL;
            MOST_LOG_CRITICAL << LOG_TAG_SWDLCONTROL << " Error Info Payload = " << errorInfo.data() << MOST_LOG_ENDL;

            emit TransferDataAMSError(stransferdataAMS.swdlid, errorCode, errorInfo);
            return;
        }
        break;
        default:
        break;
    }

    transferdataAMSQDBArg << stransferdataAMS;
    transferdataAMSQVar.setValue(transferdataAMSQDBArg);
    transferdataAMSQDBVar.setVariant(transferdataAMSQVar);

    emit TransferDataAMS(transferdataAMSQDBVar);
}
/*
 *******************************************************************
 * Function:CheckActivateModule
 *
 * Description :Invoked by handler for TransferDataAMS of Head Unit, IBOX or Ampilfier
 *              sends the result signal to SWDL Adaptor with all required data.
 *
 * Parameters :
 *    payloadLength         rw   length of the payload data from hanlder.
 *    payload               rw   payload data from handler.
 *    scheckactivemodule    rw   structure of type SCheckActivateModule.
 *
 * Returns : none
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::CheckActivateModule(uint32_t payloadLength, const uint8_t *payload, SCheckActivateModule &scheckactivemodule)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " CheckActivateModule signal got received " << MOST_LOG_ENDL;

    QVariant checkactivatemoduleQVar;
    QDBusVariant checkactivatemoduleQDBVar;
    QDBusArgument checkactivatemoduleQDBArg;
    //uint8_t *checksumptr = NULL;
    //uint32_t checksumlength = 0;

    uint16_t senderhandle = 0;

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " CheckActivateModule:status " << scheckactivemodule.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " CheckActivateModule:swdlid " << scheckactivemodule.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " CheckActivateModule:deviceID " << scheckactivemodule.DeviceID << MOST_LOG_ENDL;

    switch(scheckactivemodule.status)
    {
        case STATUS_STARTRESULTACK:
            {
                /** retrieveing start result ack request for check activate module info form IBOX/AMP */
                m_CheckActivateModuleHU->ParseStartResultAck(payloadLength,payload);

                //checksumlength = m_CheckActivateModuleHU->CheckSumStreamLength();
                //checksumptr = m_CheckActivateModuleHU->CheckSumValue();

                scheckactivemodule.senderhandle = m_CheckActivateModuleHU->SenderHandleValue();
                //scheckactivemodule.checksum = QByteArray::fromRawData((const char *)checksumptr,checksumlength);
                            QByteArray checksum((const char *)m_CheckActivateModuleHU->CheckSumValue(), m_CheckActivateModuleHU->CheckSumStreamLength());
                            scheckactivemodule.checksum = checksum;
            }
            break;
        case STATUS_PROCESSINGACK:
            {
                if(scheckactivemodule.swdlid == SWDLID_IBOX)
                {
                    /** retrieveing processing ack request for check activate module info form IBOX */
                    m_CheckActivateModuleIbox->ParseSenderHandle(payloadLength,payload,&senderhandle);
                    scheckactivemodule.senderhandle = senderhandle;
                }else if(scheckactivemodule.swdlid == SWDLID_AMP)
                {
                    /** retrieveing processing ack request for check activate module info form AMP */
                    m_CheckActivateModuleAmp->ParseSenderHandle(payloadLength,payload,&senderhandle);
                    scheckactivemodule.senderhandle = senderhandle;
                }
            }
            break;
        case STATUS_RESULTACK:
            {
                if(scheckactivemodule.swdlid == SWDLID_IBOX)
                {
                    /** retrieveing result ack request for check activate module info form IBOX */
                    m_CheckActivateModuleIbox->ParseResultAck(payloadLength,payload);

                    //checksumlength = m_CheckActivateModuleIbox->CheckSumStreamLength();
                    //checksumptr = m_CheckActivateModuleIbox->CheckSumValue();

                    scheckactivemodule.senderhandle = m_CheckActivateModuleIbox->SenderHandleValue();
                    //scheckactivemodule.checksum = QByteArray::fromRawData((const char *)checksumptr,checksumlength);
                                    QByteArray checksum((const char *)m_CheckActivateModuleIbox->CheckSumValue(), m_CheckActivateModuleIbox->CheckSumStreamLength());
                                    scheckactivemodule.checksum = checksum;

                }else if(scheckactivemodule.swdlid == SWDLID_AMP)
                {
                    /** retrieveing result ack request for check activate module info form AMP */
                    m_CheckActivateModuleAmp->ParseResultAck(payloadLength,payload);

                    //checksumlength = m_CheckActivateModuleAmp->CheckSumStreamLength();
                    //checksumptr = m_CheckActivateModuleAmp->CheckSumValue();

                    scheckactivemodule.senderhandle = m_CheckActivateModuleAmp->SenderHandleValue();
                    //scheckactivemodule.checksum = QByteArray::fromRawData((const char *)checksumptr,checksumlength);
                                    QByteArray checksum((const char *)m_CheckActivateModuleAmp->CheckSumValue(), m_CheckActivateModuleAmp->CheckSumStreamLength());
                                    scheckactivemodule.checksum = checksum;
                }
            }
            break;
        case STATUS_ERROR:
            break;
        case STATUS_ERRORACK:
            break;
        case STATUS_ABORTACK:
            break;
        case STATUS_UNDEFINED:
             break;
        default:
            break;
    }
    /** Packing structure to type QDBusVariant  */
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " CheckActivateModule:senderhandle " << scheckactivemodule.senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " CheckActivateModule:checksum length" << scheckactivemodule.checksum.length() << MOST_LOG_ENDL;

    checkactivatemoduleQDBArg << scheckactivemodule;
    checkactivatemoduleQVar.setValue(checkactivatemoduleQDBArg);
    checkactivatemoduleQDBVar.setVariant(checkactivatemoduleQVar);
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " CheckActivateModule signal emitted " << MOST_LOG_ENDL;
    emit CheckActivateModule(checkactivatemoduleQDBVar);
}
/*
 *******************************************************************
 * Function:RequestTransferExit
 *
 * Description :Invoked by handler for RequestTransferExit of Head Unit, IBOX or Ampilfier
 *              sends the result signal to SWDL Adaptor with all required data.
 *
 * Parameters :
 *    payloadLength         rw   length of the payload data from hanlder.
 *    payload               rw   payload data from handler.
 *    srequesttransferexit  rw   structure of type SRequestTransferExit.
 *
 * Returns : none
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::RequestTransferExit(uint32_t payloadLength, const uint8_t *payload, SRequestTransferExit &srequesttransferexit)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " RequestTransferExit signal got received " << MOST_LOG_ENDL;

    QVariant requesttransferexitQVar;
    QDBusVariant requesttransferexitQDBVar;
    QDBusArgument requesttransferexitQDBArg;
    uint16_t senderhandle = 0;

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " RequestTransferExit:status " << srequesttransferexit.status << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " RequestTransferExit:swdlid " << srequesttransferexit.swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " RequestTransferExit:deviceID " << srequesttransferexit.DeviceID << MOST_LOG_ENDL;

    switch(srequesttransferexit.status)
    {
        case STATUS_STARTRESULTACK:
            /** retrieveing start result ack request for request transfer exit info form IBOX/AMP */
            m_RequestTransferExitHU->ParseStartResultAck(payloadLength,payload);

            srequesttransferexit.senderhandle = m_RequestTransferExitHU->SenderHandleValue();
            break;
        case STATUS_PROCESSINGACK:
            {
                if(srequesttransferexit.swdlid == SWDLID_IBOX)
                {
                    /** retrieveing processing ack request for request transfer exit info form IBOX */
                    m_RequestTransferExitIbox->ParseSenderHandle(payloadLength,payload,&senderhandle);
                    srequesttransferexit.senderhandle = senderhandle;
                }else if(srequesttransferexit.swdlid == SWDLID_AMP)
                {
                    /** retrieveing processing ack request for request transfer exit info form AMP */
                    m_RequestTransferExitAmp->ParseSenderHandle(payloadLength,payload,&senderhandle);
                    srequesttransferexit.senderhandle = senderhandle;
                }
            }
            break;
        case STATUS_RESULTACK:
            {
                if(srequesttransferexit.swdlid == SWDLID_IBOX)
                {
                    /** retrieveing result ack request for request transfer exit info form AMP */
                    m_RequestTransferExitIbox->ParseResultAck(payloadLength,payload);

                    srequesttransferexit.senderhandle = m_RequestTransferExitIbox->SenderHandleValue();
                }else if(srequesttransferexit.swdlid == SWDLID_AMP)
                {
                    /** retrieveing result ack request for request transfer exit info form AMP */
                    m_RequestTransferExitAmp->ParseResultAck(payloadLength,payload);

                    srequesttransferexit.senderhandle = m_RequestTransferExitAmp->SenderHandleValue();
                }
            }
            break;
        case STATUS_ERROR:
            break;
        case STATUS_ERRORACK:
            break;
        case STATUS_ABORTACK:
            break;
        case STATUS_UNDEFINED:
             break;
        default:
            break;
    }
    /** Packing structure to type QDBusVariant  */
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " RequestTransferExit:senderhandle " << srequesttransferexit.senderhandle << MOST_LOG_ENDL;

    requesttransferexitQDBArg << srequesttransferexit;
    requesttransferexitQVar.setValue(requesttransferexitQDBArg);
    requesttransferexitQDBVar.setVariant(requesttransferexitQVar);
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " RequestTransferExit signal emitted " << MOST_LOG_ENDL;
    emit RequestTransferExit(requesttransferexitQDBVar);
}
/*
 *******************************************************************
 * Function:SWDLProgressInfo
 *
 * Description :Invoked by Listener for Software download ProgressInfo of Head Unit, IBOX or Ampilfier
 *              sends the result signal to SWDL Adaptor with all required data.
 *
 * Parameters :
 *    swdlId          rw   It holds the swdlID for IBOX or AMP.
 *
 * Returns : none
 *
 *******************************************************************
 */

void CSWDLFBlockShadowControl::SWDLProgressInfo(uint swdlid)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " SWDLProgressInfo signal got received " << MOST_LOG_ENDL;

    uint swdlprogress = 0;
    uint opmodeval = 0;

    switch(swdlid)
    {
        case SWDLID_IBOX:
            {
                /** retrieveing SWDLProgressInfo from IBOX */
                m_SWDLProgressInfoIbox->SWDLProgressDataLoadNext();
                opmodeval = (uint)m_SWDLProgressInfoIbox->SWDLOpModeValue();
                swdlprogress = (uint)m_SWDLProgressInfoIbox->SWDLProgressValue();
                m_SWDLProgressInfoIbox->SWDLProgressDataDataType().ResetReader();
            }
            break;

        case SWDLID_AMP:
            {
                m_SWDLProgressInfoAmp->SWDLProgressDataLoadNext();
                opmodeval = (uint)m_SWDLProgressInfoAmp->SWDLOpModeValue();
                swdlprogress = (uint)m_SWDLProgressInfoAmp->SWDLProgressValue();
                m_SWDLProgressInfoAmp->SWDLProgressDataDataType().ResetReader();
            }
            break;

        default:
            break;
    }

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SWDLProgressInfo:swdlid = " << swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SWDLProgressInfo:opmodeval = " << opmodeval << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SWDLProgressInfo:swdlprogress = " << swdlprogress << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SWDLProgressInfo signal emitted " << MOST_LOG_ENDL;


    emit SWDLProgressInfo(swdlid, opmodeval, swdlprogress);
}
/*
 *******************************************************************
 * Function:UpdateProgressingInfoStatusSlot
 *
 * Description :Invoked by Listener for Software download UpdateProgressingInfo of Head Unit, IBOX or Ampilfier
 *              sends the result signal to SWDL Adaptor with all required data.
 *
 * Parameters :
 *    swdlId          rw   It holds the swdlID for IBOX or AMP.
 *
 * Returns : none
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::UpdateProgressingInfoStatusSlot(const uint swdlid)
{
    MOST_LOG_SIGNAL << LOG_TAG_SWDLCONTROL << " UpdateProggressingInfo signal got received " << MOST_LOG_ENDL;

    uint updateProgress = 0;

    switch(swdlid)
    {
        case SWDLID_IBOX:
             /** Retriving UpdateProgressingInfo from IBOX */
            updateProgress = m_UpdateProgressingInfoIBOX->Value();
            break;
        case SWDLID_AMP:
            updateProgress = m_UpdateProgressingInfoAmp->Value();
            break;
        default:
            break;
    }

    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " swdlid : " << swdlid << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " updateProgress value : " << updateProgress << MOST_LOG_ENDL;

    emit UpdateProgressingInfoStatus(swdlid,updateProgress);
}
/*
 *******************************************************************
 * Function:TransferFileIBOXAMSCMS
 *
 * Description :Invoked by SetPrepareFileTransferIBOX method to transfer the IBOXUpgrade file
 *              to IBOX.
 *
 * Parameters :
 *    none
 *
 * Returns : none
 *
 *******************************************************************
 */
void CSWDLFBlockShadowControl::TransferFileIBOXAMSCMS()
{
   bool retVal = false;

   MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " LENGTH OF ARRAY DATA : " << m_filearray.length() << MOST_LOG_ENDL;
   MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " BlockCounter : " << m_blockCounter << MOST_LOG_ENDL;

   //gettimeofday(&tval, &tzone);
   //timem=localtime(&tval.tv_sec);

   //MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " SystemTime when TransferDataAMS/CMS Data Transmitted : " << timem->tm_hour << ":"
   //          << timem->tm_min << ":" << timem->tm_sec << ":" << tval.tv_usec << MOST_LOG_ENDL;

   if(!m_EndOfFile)
   {
       if(fileTransferInfo.transfertype == FILE_TRANSFER_AMS)
       {
           m_TransferDataAMSIbox->SenderHandleValue((uint)0x01);
           if(CheckTransferExit == true)        // hyunyong.sung, 2014.3.12 for re-transfer
           {
                m_blockCounter = 0;
                CheckTransferExit = false;
           }
           m_TransferDataAMSIbox->BlockCounterValue(m_blockCounter);
           //MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " RETRANSFER_Blockcounter_2 : " << m_blockCounter << MOST_LOG_ENDL;
           m_TransferDataAMSIbox->SWDLDataValue(reinterpret_cast<uint8_t*>(m_filearray.data()),
                                                       m_filearray.length());
           retVal = m_TransferDataAMSIbox->StartResultAck();

       }
       else if(fileTransferInfo.transfertype == FILE_TRANSFER_CMS)
       {
           m_TransferDataCMSIbox->SenderHandleValue((uint)0x01);
           m_TransferDataCMSIbox->BlockCounterValue(m_blockCounter);
           m_TransferDataCMSIbox->SWDLDataValue(reinterpret_cast<uint8_t*>(m_filearray.data()),
                                                       m_filearray.length());
           retVal = m_TransferDataCMSIbox->StartResultAck();
       }
   }

   if( retVal && !m_EndOfFile && m_fileTransferIBOX)
   {
       if(!m_IBOXUpgradefile->atEnd())
       {
           m_filearray = 0;
           m_filearray = m_IBOXUpgradefile->read(fileTransferInfo.blocksize);
           MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " fileReader size : " << m_filearray.length() << MOST_LOG_ENDL;
           m_blockCounter++;

       }
       else if(m_IBOXUpgradefile->atEnd())
       {
           m_EndOfFile = true;
           m_fileTransferIBOX = false;
           m_blockCounter = 0;
           MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " END of FILE " << MOST_LOG_ENDL;
           m_IBOXUpgradefile->close();

       }
       else
       {
           MOST_LOG_INFO << LOG_TAG_SWDLCONTROL << " Unable to read the next block " << MOST_LOG_ENDL;
       }
   }
}


// iBox SWDL FBlock checkling for iBox Update, 2014.2.24
void CSWDLFBlockShadowControl::IBOXSWDLFblockStateListener(const bool state)
{    
    emit iBoxSWDLFBlockVisible(state);
}

/*
 *******************************************************************
 * Function: CRepmodeHandlerHWIdentifierListenerHU
 *
 * Description :This is constructor of CRepmodeHandlerHWIdentifierListenerHU class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CRepmodeHandlerHWIdentifierListenerHU::CRepmodeHandlerHWIdentifierListenerHU(CSWDLFBlockShadowControl *context)
{
    m_SWDLFBlockControlContext = context;
    m_sreprogrammingmodeHU.DeviceID = 0x0000;
    m_sreprogrammingmodeHU.senderhandle = 0x0000;
    m_sreprogrammingmodeHU.status = 0;
    m_sreprogrammingmodeHU.swdlid = 0;
    m_sreprogrammingmodeHU.SWDLmode = 0;
}
/*
 *******************************************************************
 * Function: ~CRepmodeHandlerHWIdentifierListenerHU
 *
 * Description :This is destructor of CRepmodeHandlerHWIdentifierListenerHU class.
 *
 * Parameters :none
 *
 * Returns : none
 *
 *******************************************************************
 */
CRepmodeHandlerHWIdentifierListenerHU::~CRepmodeHandlerHWIdentifierListenerHU()
{
}
/*
 *******************************************************************
 * Function:GetInterface
 *
 * Description :Callback for received GetInterface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */

void CRepmodeHandlerHWIdentifierListenerHU::GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeHU.status = STATUS_UNDEFINED;
    m_sreprogrammingmodeHU.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeHU);
}
/*
 *******************************************************************
 * Function:Start
 *
 * Description :Callback for received Start operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */

void CRepmodeHandlerHWIdentifierListenerHU::Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeHU.status = STATUS_UNDEFINED;
    m_sreprogrammingmodeHU.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeHU);
}
/*
 *******************************************************************
 * Function:Abort
 *
 * Description :Callback for received Abort operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */

void CRepmodeHandlerHWIdentifierListenerHU::Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeHU.status = STATUS_UNDEFINED;
    m_sreprogrammingmodeHU.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeHU);
}
/*
 *******************************************************************
 * Function:StartResult
 *
 * Description :Callback for received StartResult operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */

void CRepmodeHandlerHWIdentifierListenerHU::StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeHU.status = STATUS_UNDEFINED;
    m_sreprogrammingmodeHU.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeHU);
}
/*
 *******************************************************************
 * Function: StartResultAck
 *
 * Description :Callback for received StartResultAck operation.
 *              Updates Start Result Ack to SWDL FBlock Control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload.
 *
 * Returns : void
 *
 *******************************************************************
 */

void CRepmodeHandlerHWIdentifierListenerHU::StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeHU.status = STATUS_STARTRESULTACK;
    m_sreprogrammingmodeHU.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeHU);
}
/*
 *******************************************************************
 * Function:AbortAck
 *
 * Description :Callback for received AbortAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */

void CRepmodeHandlerHWIdentifierListenerHU::AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeHU.status = STATUS_ABORTACK;
    m_sreprogrammingmodeHU.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeHU);
}
/*
 *******************************************************************
 * Function:StartAck
 *
 * Description :Callback for received StartAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerHU::StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeHU.status = STATUS_UNDEFINED;
    m_sreprogrammingmodeHU.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeHU);
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property hwidentifier
 *               is changed by IBox or Amplifier
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerHU::OnChange()
{
    m_SWDLFBlockControlContext->ReadHWIdentifier(SWDLID_HU);
}
/*
 *******************************************************************
 * Function: CRepmodeHandlerHWIdentifierListenerIBOX
 *
 * Description :This is constructor of CRepmodeHandlerHWIdentifierListenerIBOX class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CRepmodeHandlerHWIdentifierListenerIBOX::CRepmodeHandlerHWIdentifierListenerIBOX(CSWDLFBlockShadowControl* context)
{
    m_SWDLShadowControlContext = context;
    m_sreprogrammingmodeIbox.DeviceID = 0x0000;
    m_sreprogrammingmodeIbox.senderhandle = 0x0000;
    m_sreprogrammingmodeIbox.status = 0;
    m_sreprogrammingmodeIbox.swdlid = 0;
    m_sreprogrammingmodeIbox.SWDLmode = 0;
}
/*
 *******************************************************************
 * Function: ~CRepmodeHandlerHWIdentifierListenerIBOX
 *
 * Description :This is destructor of CRepmodeHandlerHWIdentifierListenerIBOX class.
 *
 * Parameters :none
 *
 * Returns : none
 *
 *******************************************************************
 */
CRepmodeHandlerHWIdentifierListenerIBOX::~CRepmodeHandlerHWIdentifierListenerIBOX()
{
}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerIBOX::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeIbox.status = STATUS_ERRORACK;
    m_sreprogrammingmodeIbox.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeIbox);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation.
 *              Updates the Processing Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerIBOX::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeIbox.status = STATUS_PROCESSINGACK;
    m_sreprogrammingmodeIbox.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeIbox);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerIBOX::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeIbox.status = STATUS_UNDEFINED;
    m_sreprogrammingmodeIbox.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeIbox);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerIBOX::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeIbox.status = STATUS_UNDEFINED;
    m_sreprogrammingmodeIbox.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeIbox);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation
 *              Updates the Result Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerIBOX::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeIbox.status = STATUS_RESULTACK;
    m_sreprogrammingmodeIbox.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeIbox);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerIBOX::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeIbox.status = STATUS_UNDEFINED;
    m_sreprogrammingmodeIbox.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeIbox);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerIBOX::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeIbox.status = STATUS_ERROR;
    m_sreprogrammingmodeIbox.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeIbox);
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property hwidentifier
 *               is changed by Ibox
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerIBOX::OnChange()
{
    m_SWDLShadowControlContext->ReadHWIdentifier(SWDLID_IBOX);
}
/*
 *******************************************************************
 * Function: CRepmodeHandlerHWIdentifierListenerAMP
 *
 * Description :This is constructor of CRepmodeHandlerHWIdentifierListenerAMP class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */

CRepmodeHandlerHWIdentifierListenerAMP::CRepmodeHandlerHWIdentifierListenerAMP(CSWDLFBlockShadowControl* context)
{
    m_SWDLShadowControlContext = context;
    m_sreprogrammingmodeAmp.DeviceID = 0x0000;
    m_sreprogrammingmodeAmp.senderhandle = 0x0000;
    m_sreprogrammingmodeAmp.status = 0;
    m_sreprogrammingmodeAmp.swdlid = 0;
    m_sreprogrammingmodeAmp.SWDLmode = 0;
}
/*
 *******************************************************************
 * Function: ~CRepmodeHandlerHWIdentifierListenerAMP
 *
 * Description :This is destructor of CRepmodeHandlerHWIdentifierListenerAMP class.
 *
 * Parameters :none
 *
 * Returns : none
 *
 *******************************************************************
 */
CRepmodeHandlerHWIdentifierListenerAMP::~CRepmodeHandlerHWIdentifierListenerAMP()
{
}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerAMP::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeAmp.status = STATUS_ERRORACK;
    m_sreprogrammingmodeAmp.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeAmp);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation
 *              updates the processing Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerAMP::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeAmp.status = STATUS_PROCESSINGACK;
    m_sreprogrammingmodeAmp.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeAmp);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerAMP::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeAmp.status = STATUS_UNDEFINED;
    m_sreprogrammingmodeAmp.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeAmp);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerAMP::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeAmp.status = STATUS_UNDEFINED;
    m_sreprogrammingmodeAmp.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeAmp);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation
 *              Updates the Result Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerAMP::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeAmp.status = STATUS_RESULTACK;
    m_sreprogrammingmodeAmp.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeAmp);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerAMP::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeAmp.status = STATUS_UNDEFINED;
    m_sreprogrammingmodeAmp.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeAmp);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerAMP::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sreprogrammingmodeAmp.status = STATUS_ERROR;
    m_sreprogrammingmodeAmp.DeviceID = sourceDeviceID;
    m_sreprogrammingmodeAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->ReprogrammingMode(payloadLength,payload,m_sreprogrammingmodeAmp);
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property hwidentifier
 *               is changed by Amplifier
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CRepmodeHandlerHWIdentifierListenerAMP::OnChange()
{
    m_SWDLShadowControlContext->ReadHWIdentifier(SWDLID_AMP);
}

/*
 *******************************************************************
 * Function: CIUpdateHandlerDevNameListenerHU
 *
 * Description :This is constructor of CIUpdateHandlerDevNameListenerHU class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CIUpdateHandlerDevNameListenerHU::CIUpdateHandlerDevNameListenerHU(CSWDLFBlockShadowControl *context)
{
    m_SWDLFBlockControlContext = context;
    m_sinitupdateHU.DeviceID = 0x0000;
    m_sinitupdateHU.senderhandle = 0x0000;
    m_sinitupdateHU.status = 0;
    m_sinitupdateHU.swdlid = 0;
    m_sinitupdateHU.day = 0;
    m_sinitupdateHU.month = 0;
    m_sinitupdateHU.year = 0;
    m_sinitupdateHU.diskID = (const char *)0;
}
/*
 *******************************************************************
 * Function: ~CIUpdateHandlerDevNameListenerHU
 *
 * Description :This is destructor of CIUpdateHandlerDevNameListenerHU class.
 *
 * Parameters :none
 *
 * Returns : none
 *
 *******************************************************************
 */
CIUpdateHandlerDevNameListenerHU::~CIUpdateHandlerDevNameListenerHU()
{
}
/*
 *******************************************************************
 * Function:GetInterface
 *
 * Description :Callback for received GetInterface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerHU::GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateHU.DeviceID = sourceDeviceID;
    m_sinitupdateHU.status = STATUS_UNDEFINED;
    m_sinitupdateHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateHU);
}
/*
 *******************************************************************
 * Function:Start
 *
 * Description :Callback for received Start operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerHU::Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateHU.DeviceID = sourceDeviceID;
    m_sinitupdateHU.status = STATUS_UNDEFINED;
    m_sinitupdateHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateHU);
}
/*
 *******************************************************************
 * Function:Abort
 *
 * Description :Callback for received Abort operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerHU::Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateHU.DeviceID = sourceDeviceID;
    m_sinitupdateHU.status = STATUS_UNDEFINED;
    m_sinitupdateHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateHU);
}
/*
 *******************************************************************
 * Function:StartResult
 *
 * Description :Callback for received StartResult operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerHU::StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateHU.DeviceID = sourceDeviceID;
    m_sinitupdateHU.status = STATUS_UNDEFINED;
    m_sinitupdateHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateHU);
}
/*
 *******************************************************************
 * Function:StartResultAck
 *
 * Description :Callback for received StartResultAck operation.
 *              Updates Start Result Ack to SWDL FBlock control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerHU::StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateHU.DeviceID = sourceDeviceID;
    m_sinitupdateHU.status = STATUS_STARTRESULTACK;
    m_sinitupdateHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateHU);
}
/*
 *******************************************************************
 * Function:AbortAck
 *
 * Description :Callback for received AbortAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerHU::AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateHU.DeviceID = sourceDeviceID;
    m_sinitupdateHU.status = STATUS_ABORTACK;
    m_sinitupdateHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateHU);
}
/*
 *******************************************************************
 * Function:StartAck
 *
 * Description :Callback for received StartAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerHU::StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateHU.DeviceID = sourceDeviceID;
    m_sinitupdateHU.status = STATUS_UNDEFINED;
    m_sinitupdateHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateHU);
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property device name
 *               is changed by IBox or Amplifier
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerHU::OnChange()
{

    m_SWDLFBlockControlContext->ReadDevName(SWDLID_HU);
}
/*
 *******************************************************************
 * Function: CIUpdateHandlerDevNameListenerIBOX
 *
 * Description :This is constructor of CIUpdateHandlerDevNameListenerIBOX class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */

CIUpdateHandlerDevNameListenerIBOX::CIUpdateHandlerDevNameListenerIBOX(CSWDLFBlockShadowControl* context)
{
    m_SWDLShadowControlContext = context;
    m_sinitupdateIbox.DeviceID = 0x0000;
    m_sinitupdateIbox.senderhandle = 0x0000;
    m_sinitupdateIbox.status = 0;
    m_sinitupdateIbox.swdlid = 0;
    m_sinitupdateIbox.day = 0;
    m_sinitupdateIbox.month = 0;
    m_sinitupdateIbox.year = 0;
    m_sinitupdateIbox.diskID = (const char *)0;
}

/*
 *******************************************************************
 * Function: ~CIUpdateHandlerDevNameListenerIBOX
 *
 * Description :This is destructor of CIUpdateHandlerDevNameListenerIBOX class.
 *
 * Parameters :none
 *
 * Returns : none
 *
 *******************************************************************
 */
CIUpdateHandlerDevNameListenerIBOX::~CIUpdateHandlerDevNameListenerIBOX()
{
}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerIBOX::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateIbox.status = STATUS_ERRORACK;
    m_sinitupdateIbox.DeviceID = sourceDeviceID;
    m_sinitupdateIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateIbox);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation.
 *              Updates Processing Ack to SWDL shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerIBOX::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateIbox.status = STATUS_PROCESSINGACK;
    m_sinitupdateIbox.DeviceID = sourceDeviceID;
    m_sinitupdateIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateIbox);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerIBOX::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateIbox.status = STATUS_UNDEFINED;
    m_sinitupdateIbox.DeviceID = sourceDeviceID;
    m_sinitupdateIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateIbox);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerIBOX::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateIbox.status = STATUS_UNDEFINED;
    m_sinitupdateIbox.DeviceID = sourceDeviceID;
    m_sinitupdateIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateIbox);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation.
 *              Updates Result Ack to SWDL shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerIBOX::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateIbox.status = STATUS_RESULTACK;
    m_sinitupdateIbox.DeviceID = sourceDeviceID;
    m_sinitupdateIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateIbox);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerIBOX::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateIbox.status = STATUS_UNDEFINED;
    m_sinitupdateIbox.DeviceID = sourceDeviceID;
    m_sinitupdateIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateIbox);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerIBOX::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateIbox.status = STATUS_ERROR;
    m_sinitupdateIbox.DeviceID = sourceDeviceID;
    m_sinitupdateIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateIbox);
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property DeviceName
 *               is changed by IBox.
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerIBOX::OnChange()
{
    m_SWDLShadowControlContext->ReadDevName(SWDLID_IBOX);
}
/*
 *******************************************************************
 * Function: CIUpdateHandlerDevNameListenerAMP
 *
 * Description :This is constructor of CIUpdateHandlerDevNameListenerAMP class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */

CIUpdateHandlerDevNameListenerAMP::CIUpdateHandlerDevNameListenerAMP(CSWDLFBlockShadowControl* context)
{
    m_SWDLShadowControlContext = context;
    m_sinitupdateAmp.DeviceID = 0x0000;
    m_sinitupdateAmp.senderhandle = 0x0000;
    m_sinitupdateAmp.status = 0;
    m_sinitupdateAmp.swdlid = 0;
    m_sinitupdateAmp.day = 0;
    m_sinitupdateAmp.month = 0;
    m_sinitupdateAmp.year = 0;
    m_sinitupdateAmp.diskID = (const char *)0;
}

/*
 *******************************************************************
 * Function: ~CIUpdateHandlerDevNameListenerAMP
 *
 * Description :This is destructor of CIUpdateHandlerDevNameListenerAMP class.
 *
 * Parameters :none
 *
 * Returns : none
 *
 *******************************************************************
 */
CIUpdateHandlerDevNameListenerAMP::~CIUpdateHandlerDevNameListenerAMP()
{

}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerAMP::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateAmp.status = STATUS_ERRORACK;
    m_sinitupdateAmp.DeviceID = sourceDeviceID;
    m_sinitupdateAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateAmp);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation.
 *              Updates Processing Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerAMP::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateAmp.status = STATUS_PROCESSINGACK;
    m_sinitupdateAmp.DeviceID = sourceDeviceID;
    m_sinitupdateAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateAmp);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerAMP::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateAmp.status = STATUS_UNDEFINED;
    m_sinitupdateAmp.DeviceID = sourceDeviceID;
    m_sinitupdateAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateAmp);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerAMP::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{    
    m_sinitupdateAmp.status = STATUS_UNDEFINED;
    m_sinitupdateAmp.DeviceID = sourceDeviceID;
    m_sinitupdateAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateAmp);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation.
 *              Updates Result Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerAMP::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateAmp.status = STATUS_RESULTACK;
    m_sinitupdateAmp.DeviceID = sourceDeviceID;
    m_sinitupdateAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateAmp);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerAMP::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateAmp.status = STATUS_UNDEFINED;
    m_sinitupdateAmp.DeviceID = sourceDeviceID;
    m_sinitupdateAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateAmp);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerAMP::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_sinitupdateAmp.status = STATUS_ERROR;
    m_sinitupdateAmp.DeviceID = sourceDeviceID;
    m_sinitupdateAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->InitUpdateResult(payloadLength,payload,m_sinitupdateAmp);
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property device name
 *               is changed by Amplifier
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CIUpdateHandlerDevNameListenerAMP::OnChange()
{
    m_SWDLShadowControlContext->ReadDevName(SWDLID_AMP);
}

/*
 *******************************************************************
 * Function: CEraseMemoryHandlerHWVersionListenerHU
 *
 * Description :This is constructor of CEraseMemoryHandlerHWVersionListenerHU class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CEraseMemoryHandlerHWVersionListenerHU::CEraseMemoryHandlerHWVersionListenerHU(CSWDLFBlockShadowControl *context)
{
    m_SWDLFBlockControlContext = context;
    m_serasememoryHU.DeviceID = 0x0000;
    m_serasememoryHU.Senderhandle = 0x0000;
    m_serasememoryHU.status = 0;
    m_serasememoryHU.swdlid = 0;
    m_serasememoryHU.moduleindex = 0;
    m_serasememoryHU.moduletype = 0;
}
/*
 *******************************************************************
 * Function: ~CEraseMemoryHandlerHWVersionListenerHU
 *
 * Description :This is destructor of CEraseMemoryHandlerHWVersionListenerHU class.
 *
 * Parameters :none
 *
 * Returns : none
 *
 *******************************************************************
 */
CEraseMemoryHandlerHWVersionListenerHU::~CEraseMemoryHandlerHWVersionListenerHU()
{
}
/*
 *******************************************************************
 * Function:GetInterface
 *
 * Description :Callback for received GetInterface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerHU::GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryHU.DeviceID = sourceDeviceID;
    m_serasememoryHU.status = STATUS_UNDEFINED;
    m_serasememoryHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->EraseMemory(payloadLength,payload,m_serasememoryHU);
}
/*
 *******************************************************************
 * Function:Start
 *
 * Description :Callback for received Start operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerHU::Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryHU.DeviceID = sourceDeviceID;
    m_serasememoryHU.status = STATUS_UNDEFINED;
    m_serasememoryHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->EraseMemory(payloadLength,payload,m_serasememoryHU);
}
/*
 *******************************************************************
 * Function:Abort
 *
 * Description :Callback for received Abort operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerHU::Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryHU.DeviceID = sourceDeviceID;
    m_serasememoryHU.status = STATUS_UNDEFINED;
    m_serasememoryHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->EraseMemory(payloadLength,payload,m_serasememoryHU);
}
/*
 *******************************************************************
 * Function:StartResult
 *
 * Description :Callback for received StartResult operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerHU::StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryHU.DeviceID = sourceDeviceID;
    m_serasememoryHU.status = STATUS_UNDEFINED;
    m_serasememoryHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->EraseMemory(payloadLength,payload,m_serasememoryHU);
}
/*
 *******************************************************************
 * Function:StartResultAck
 *
 * Description :Callback for received StartResultAck operation.
 *              Updates Start Result Ack to SWDL FBlock control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerHU::StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryHU.DeviceID = sourceDeviceID;
    m_serasememoryHU.status = STATUS_STARTRESULTACK;
    m_serasememoryHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->EraseMemory(payloadLength,payload,m_serasememoryHU);
}
/*
 *******************************************************************
 * Function:AbortAck
 *
 * Description :Callback for received AbortAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerHU::AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryHU.DeviceID = sourceDeviceID;
    m_serasememoryHU.status = STATUS_ABORTACK;
    m_serasememoryHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->EraseMemory(payloadLength,payload,m_serasememoryHU);
}
/*
 *******************************************************************
 * Function:StartAck
 *
 * Description :Callback for received StartAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerHU::StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryHU.DeviceID = sourceDeviceID;
    m_serasememoryHU.status = STATUS_UNDEFINED;
    m_serasememoryHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->EraseMemory(payloadLength,payload,m_serasememoryHU);
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property hwversion
 *               is changed by IBox or Amplifier
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerHU::OnChange()
{
    m_SWDLFBlockControlContext->ReadHWVersion(SWDLID_HU);
}
/*
 *******************************************************************
 * Function: CEraseMemoryHandlerHWVersionListenerIBOX
 *
 * Description :This is constructor of CEraseMemoryHandlerHWVersionListenerIBOX class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */

CEraseMemoryHandlerHWVersionListenerIBOX::CEraseMemoryHandlerHWVersionListenerIBOX(CSWDLFBlockShadowControl* context)
{
    m_SWDLShadowControlContext = context;
    m_serasememoryIbox.DeviceID = 0x0000;
    m_serasememoryIbox.Senderhandle = 0x0000;
    m_serasememoryIbox.status = 0;
    m_serasememoryIbox.swdlid = 0;
    m_serasememoryIbox.moduleindex = 0;
    m_serasememoryIbox.moduletype = 0;
}

/*
 *******************************************************************
 * Function: ~CEraseMemoryHandlerHWVersionListenerIBOX
 *
 * Description :This is destructor of CEraseMemoryHandlerHWVersionListenerIBOX class.
 *
 * Parameters :none
 *
 * Returns : none
 *
 *******************************************************************
 */
CEraseMemoryHandlerHWVersionListenerIBOX::~CEraseMemoryHandlerHWVersionListenerIBOX()
{
}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerIBOX::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryIbox.status = STATUS_ERRORACK;
    m_serasememoryIbox.DeviceID = sourceDeviceID;
    m_serasememoryIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->EraseMemory(payloadLength,payload,m_serasememoryIbox);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation.
 *              Updates Processing Ack to SWDL shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerIBOX::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryIbox.status = STATUS_PROCESSINGACK;
    m_serasememoryIbox.DeviceID = sourceDeviceID;
    m_serasememoryIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->EraseMemory(payloadLength,payload,m_serasememoryIbox);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerIBOX::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryIbox.status = STATUS_UNDEFINED;
    m_serasememoryIbox.DeviceID = sourceDeviceID;
    m_serasememoryIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->EraseMemory(payloadLength,payload,m_serasememoryIbox);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerIBOX::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryIbox.status = STATUS_UNDEFINED;
    m_serasememoryIbox.DeviceID = sourceDeviceID;
    m_serasememoryIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->EraseMemory(payloadLength,payload,m_serasememoryIbox);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation.
 *              Updates Result Ack to SWDL shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerIBOX::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryIbox.status = STATUS_RESULTACK;
    m_serasememoryIbox.DeviceID = sourceDeviceID;
    m_serasememoryIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->EraseMemory(payloadLength,payload,m_serasememoryIbox);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerIBOX::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryIbox.status = STATUS_UNDEFINED;
    m_serasememoryIbox.DeviceID = sourceDeviceID;
    m_serasememoryIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->EraseMemory(payloadLength,payload,m_serasememoryIbox);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerIBOX::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryIbox.status = STATUS_ERROR;
    m_serasememoryIbox.DeviceID = sourceDeviceID;
    m_serasememoryIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->EraseMemory(payloadLength,payload,m_serasememoryIbox);
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property hwversion
 *               is changed by IBox.
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerIBOX::OnChange()
{
    m_SWDLShadowControlContext->ReadHWVersion(SWDLID_IBOX);
}
/*
 *******************************************************************
 * Function: CEraseMemoryHandlerHWVersionListenerAMP
 *
 * Description :This is constructor of CEraseMemoryHandlerHWVersionListenerAMP class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */

CEraseMemoryHandlerHWVersionListenerAMP::CEraseMemoryHandlerHWVersionListenerAMP(CSWDLFBlockShadowControl* context)
{
    m_SWDLShadowControlContext = context;
    m_serasememoryAmp.DeviceID = 0x0000;
    m_serasememoryAmp.Senderhandle = 0x0000;
    m_serasememoryAmp.status = 0;
    m_serasememoryAmp.swdlid = 0;
    m_serasememoryAmp.moduleindex = 0;
    m_serasememoryAmp.moduletype = 0;
}

/*
 *******************************************************************
 * Function: ~CEraseMemoryHandlerHWVersionListenerAMP
 *
 * Description :This is destructor of CEraseMemoryHandlerHWVersionListenerAMP class.
 *
 * Parameters :none
 *
 * Returns : none
 *
 *******************************************************************
 */
CEraseMemoryHandlerHWVersionListenerAMP::~CEraseMemoryHandlerHWVersionListenerAMP()
{
}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerAMP::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryAmp.status = STATUS_ERRORACK;
    m_serasememoryAmp.DeviceID = sourceDeviceID;
    m_serasememoryAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->EraseMemory(payloadLength,payload,m_serasememoryAmp);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation.
 *              Updates Processing Ack to SWDL shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerAMP::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryAmp.status = STATUS_PROCESSINGACK;
    m_serasememoryAmp.DeviceID = sourceDeviceID;
    m_serasememoryAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->EraseMemory(payloadLength,payload,m_serasememoryAmp);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerAMP::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryAmp.status = STATUS_UNDEFINED;
    m_serasememoryAmp.DeviceID = sourceDeviceID;
    m_serasememoryAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->EraseMemory(payloadLength,payload,m_serasememoryAmp);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerAMP::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryAmp.status = STATUS_UNDEFINED;
    m_serasememoryAmp.DeviceID = sourceDeviceID;
    m_serasememoryAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->EraseMemory(payloadLength,payload,m_serasememoryAmp);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation.
 *              Updates Result Ack to SWDL shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerAMP::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryAmp.status = STATUS_RESULTACK;
    m_serasememoryAmp.DeviceID = sourceDeviceID;
    m_serasememoryAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->EraseMemory(payloadLength,payload,m_serasememoryAmp);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerAMP::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryAmp.status = STATUS_UNDEFINED;
    m_serasememoryAmp.DeviceID = sourceDeviceID;
    m_serasememoryAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->EraseMemory(payloadLength,payload,m_serasememoryAmp);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerAMP::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_serasememoryAmp.status = STATUS_ERROR;
    m_serasememoryAmp.DeviceID = sourceDeviceID;
    m_serasememoryAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->EraseMemory(payloadLength,payload,m_serasememoryAmp);
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property hwversion
 *               is changed by Amplifier
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CEraseMemoryHandlerHWVersionListenerAMP::OnChange()
{
    m_SWDLShadowControlContext->ReadHWVersion(SWDLID_AMP);
}
/*
 *******************************************************************
 * Function: CRDownloadHandlerNumofModuleListenerHU
 *
 * Description :This is constructor of CRDownloadHandlerNumofModuleListenerHU class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CRDownloadHandlerNumofModuleListenerHU::CRDownloadHandlerNumofModuleListenerHU(CSWDLFBlockShadowControl *context)
{
    m_SWDLFBlockControlContext = context;
    m_srequestdownloadHU.DeviceID = 0x0000;
    m_srequestdownloadHU.senderhandle = 0x0000;
    m_srequestdownloadHU.status = 0;
    m_srequestdownloadHU.swdlid = 0;
    m_srequestdownloadHU.moduleindex = 0;
    m_srequestdownloadHU.moduletype = 0;
    m_srequestdownloadHU.blocksize = 0;
    m_srequestdownloadHU.swdlchannel = 0;
}
/*
 *******************************************************************
 * Function: ~CRDownloadHandlerNumofModuleListenerHU
 *
 * Description :This is destructor of CRDownloadHandlerNumofModuleListenerHU class.
 *
 * Parameters :none
 *
 * Returns : none
 *
 *******************************************************************
 */
CRDownloadHandlerNumofModuleListenerHU::~CRDownloadHandlerNumofModuleListenerHU()
{
}
/*
 *******************************************************************
 * Function:GetInterface
 *
 * Description :Callback for received GetInterface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerHU::GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_srequestdownloadHU.DeviceID = sourceDeviceID;
    m_srequestdownloadHU.status = STATUS_UNDEFINED;
    m_srequestdownloadHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadHU);
}
/*
 *******************************************************************
 * Function:Start
 *
 * Description :Callback for received Start operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerHU::Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_srequestdownloadHU.DeviceID = sourceDeviceID;
    m_srequestdownloadHU.status = STATUS_UNDEFINED;
    m_srequestdownloadHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadHU);
}
/*
 *******************************************************************
 * Function:Abort
 *
 * Description :Callback for received Abort operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerHU::Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_srequestdownloadHU.DeviceID = sourceDeviceID;
    m_srequestdownloadHU.status = STATUS_UNDEFINED;
    m_srequestdownloadHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadHU);
}
/*
 *******************************************************************
 * Function:StartResult
 *
 * Description :Callback for received StartResult operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerHU::StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_srequestdownloadHU.DeviceID = sourceDeviceID;
    m_srequestdownloadHU.status = STATUS_UNDEFINED;
    m_srequestdownloadHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadHU);
}
/*
 *******************************************************************
 * Function:StartResultAck
 *
 * Description :Callback for received StartResultAck operation.
 *              Updates Start Result Ack to SWDL FBlock control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerHU::StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_srequestdownloadHU.DeviceID = sourceDeviceID;
    m_srequestdownloadHU.status = STATUS_STARTRESULTACK;
    m_srequestdownloadHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadHU);
}
/*
 *******************************************************************
 * Function:AbortAck
 *
 * Description :Callback for received AbortAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerHU::AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_srequestdownloadHU.DeviceID = sourceDeviceID;
    m_srequestdownloadHU.status = STATUS_ABORTACK;
    m_srequestdownloadHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadHU);
}
/*
 *******************************************************************
 * Function:StartAck
 *
 * Description :Callback for received StartAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerHU::StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_srequestdownloadHU.DeviceID = sourceDeviceID;
    m_srequestdownloadHU.status = STATUS_UNDEFINED;
    m_srequestdownloadHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadHU);
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property Number of modules
 *               is changed by IBox or Amplifier
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerHU::OnChange()
{
    m_SWDLFBlockControlContext->ReadNumberofModule(SWDLID_HU);
}
/*
 *******************************************************************
 * Function: CRDownloadHandlerNumofModuleListenerIBOX
 *
 * Description :This is constructor of CRDownloadHandlerNumofModuleListenerIBOX class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */

CRDownloadHandlerNumofModuleListenerIBOX::CRDownloadHandlerNumofModuleListenerIBOX(CSWDLFBlockShadowControl* context)
{
    m_SWDLShadowControlContext = context;
    m_srequestdownloadIbox.DeviceID = 0x0000;
    m_srequestdownloadIbox.senderhandle = 0x0000;
    m_srequestdownloadIbox.status = 0;
    m_srequestdownloadIbox.swdlid = 0;
    m_srequestdownloadIbox.moduleindex = 0;
    m_srequestdownloadIbox.moduletype = 0;
    m_srequestdownloadIbox.blocksize = 0;
    m_srequestdownloadIbox.swdlchannel = 0;
}
/*
 *******************************************************************
 * Function: ~CRDownloadHandlerNumofModuleListenerIBOX
 *
 * Description :This is destructor of CRDownloadHandlerNumofModuleListenerIBOX class.
 *
 * Parameters :none
 *
 * Returns : none
 *
 *******************************************************************
 */
CRDownloadHandlerNumofModuleListenerIBOX::~CRDownloadHandlerNumofModuleListenerIBOX()
{
}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerIBOX::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_srequestdownloadIbox.status = STATUS_ERRORACK;
    m_srequestdownloadIbox.DeviceID = sourceDeviceID;
    m_srequestdownloadIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadIbox);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation.
 *              Updates Processing Ack to SWDL shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerIBOX::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_srequestdownloadIbox.status = STATUS_PROCESSINGACK;
    m_srequestdownloadIbox.DeviceID = sourceDeviceID;
    m_srequestdownloadIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadIbox);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerIBOX::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_srequestdownloadIbox.status = STATUS_UNDEFINED;
    m_srequestdownloadIbox.DeviceID = sourceDeviceID;
    m_srequestdownloadIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadIbox);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerIBOX::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_srequestdownloadIbox.status = STATUS_UNDEFINED;
    m_srequestdownloadIbox.DeviceID = sourceDeviceID;
    m_srequestdownloadIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadIbox);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation.
 *              Updates Result Ack to SWDL shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerIBOX::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_srequestdownloadIbox.status = STATUS_RESULTACK;
    m_srequestdownloadIbox.DeviceID = sourceDeviceID;
    m_srequestdownloadIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadIbox);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerIBOX::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_srequestdownloadIbox.status = STATUS_UNDEFINED;
    m_srequestdownloadIbox.DeviceID = sourceDeviceID;
    m_srequestdownloadIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadIbox);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerIBOX::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_srequestdownloadIbox.status = STATUS_ERROR;
    m_srequestdownloadIbox.DeviceID = sourceDeviceID;
    m_srequestdownloadIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadIbox);
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property Number of modules
 *               is changed by IBox.
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerIBOX::OnChange()
{
    m_SWDLShadowControlContext->ReadNumberofModule(SWDLID_IBOX);
}
/*
 *******************************************************************
 * Function: CRDownloadHandlerNumofModuleListenerAMP
 *
 * Description :This is constructor of CRDownloadHandlerNumofModuleListenerAMP class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */

CRDownloadHandlerNumofModuleListenerAMP::CRDownloadHandlerNumofModuleListenerAMP(CSWDLFBlockShadowControl* context)
{
    m_SWDLShadowControlContext = context;
    m_srequestdownloadAmp.DeviceID = 0x0000;
    m_srequestdownloadAmp.senderhandle = 0x0000;
    m_srequestdownloadAmp.status = 0;
    m_srequestdownloadAmp.swdlid = 0;
    m_srequestdownloadAmp.moduleindex = 0;
    m_srequestdownloadAmp.moduletype = 0;
    m_srequestdownloadAmp.blocksize = 0;
    m_srequestdownloadAmp.swdlchannel = 0;
}

/*
 *******************************************************************
 * Function: ~CRDownloadHandlerNumofModuleListenerAMP
 *
 * Description :This is destructor of CRDownloadHandlerNumofModuleListenerAMP class.
 *
 * Parameters :none
 *
 * Returns : none
 *
 *******************************************************************
 */
CRDownloadHandlerNumofModuleListenerAMP::~CRDownloadHandlerNumofModuleListenerAMP()
{

}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerAMP::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequestdownloadAmp.status = STATUS_ERRORACK;
    m_srequestdownloadAmp.DeviceID = sourceDeviceID;
    m_srequestdownloadAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadAmp);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation.
 *              Updates Processing Ack to SWDL shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerAMP::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequestdownloadAmp.status = STATUS_PROCESSINGACK;
    m_srequestdownloadAmp.DeviceID = sourceDeviceID;
    m_srequestdownloadAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadAmp);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerAMP::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequestdownloadAmp.status = STATUS_UNDEFINED;
    m_srequestdownloadAmp.DeviceID = sourceDeviceID;
    m_srequestdownloadAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadAmp);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerAMP::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequestdownloadAmp.status = STATUS_UNDEFINED;
    m_srequestdownloadAmp.DeviceID = sourceDeviceID;
    m_srequestdownloadAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadAmp);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation.
 *              Updates Result Ack to SWDL shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerAMP::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequestdownloadAmp.status = STATUS_RESULTACK;
    m_srequestdownloadAmp.DeviceID = sourceDeviceID;
    m_srequestdownloadAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadAmp);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerAMP::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequestdownloadAmp.status = STATUS_UNDEFINED;
    m_srequestdownloadAmp.DeviceID = sourceDeviceID;
    m_srequestdownloadAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadAmp);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerAMP::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequestdownloadAmp.status = STATUS_ERROR;
    m_srequestdownloadAmp.DeviceID = sourceDeviceID;
    m_srequestdownloadAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->RequestDownload(payloadLength,payload,m_srequestdownloadAmp);
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property Number of modules
 *               is changed by Amplifier.
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CRDownloadHandlerNumofModuleListenerAMP::OnChange()
{

    m_SWDLShadowControlContext->ReadNumberofModule(SWDLID_AMP);
}
/*
 *******************************************************************
 * Function: CPFTransferHandlerModVersionListenerHU
 *
 * Description :This is constructor of CPFTransferHandlerModVersionListenerHU class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CPFTransferHandlerModVersionListenerHU::CPFTransferHandlerModVersionListenerHU(CSWDLFBlockShadowControl *context){

    m_SWDLFBlockControlContext = context;
    m_spreparefiletransferHU.DeviceID = 0x0000;
    m_spreparefiletransferHU.senderhandle = 0x0000;
    m_spreparefiletransferHU.status = 0;
    m_spreparefiletransferHU.swdlid = 0;
    m_spreparefiletransferHU.filesize = 0;
    m_spreparefiletransferHU.swdlFilename = (const char *)0;
}
/*
 *******************************************************************
 * Function: ~CPFTransferHandlerModVersionListenerHU
 *
 * Description :This is destructor of CPFTransferHandlerModVersionListenerHU class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CPFTransferHandlerModVersionListenerHU::~CPFTransferHandlerModVersionListenerHU(){

}
/*
 *******************************************************************
 * Function:GetInterface
 *
 * Description :Callback for received GetInterface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerHU::GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_spreparefiletransferHU.DeviceID = sourceDeviceID;
    m_spreparefiletransferHU.status = STATUS_UNDEFINED;
    m_spreparefiletransferHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferHU);
}
/*
 *******************************************************************
 * Function:Start
 *
 * Description :Callback for received Start operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerHU::Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_spreparefiletransferHU.DeviceID = sourceDeviceID;
    m_spreparefiletransferHU.status = STATUS_UNDEFINED;
    m_spreparefiletransferHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferHU);
}
/*
 *******************************************************************
 * Function:Abort
 *
 * Description :Callback for received Abort operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerHU::Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_spreparefiletransferHU.DeviceID = sourceDeviceID;
    m_spreparefiletransferHU.status = STATUS_UNDEFINED;
    m_spreparefiletransferHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferHU);
}
/*
 *******************************************************************
 * Function:StartResult
 *
 * Description :Callback for received StartResult operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerHU::StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_spreparefiletransferHU.DeviceID = sourceDeviceID;
    m_spreparefiletransferHU.status = STATUS_UNDEFINED;
    m_spreparefiletransferHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferHU);
}
/*
 *******************************************************************
 * Function:StartResultAck
 *
 * Description :Callback for received StartResultAck operation.
 *              Updates Start Result Ack to SWDL FBlock control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerHU::StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){


    m_spreparefiletransferHU.DeviceID = sourceDeviceID;
    m_spreparefiletransferHU.status = STATUS_STARTRESULTACK;
    m_spreparefiletransferHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferHU);
}
/*
 *******************************************************************
 * Function:AbortAck
 *
 * Description :Callback for received AbortAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerHU::AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_spreparefiletransferHU.DeviceID = sourceDeviceID;
    m_spreparefiletransferHU.status = STATUS_ABORTACK;
    m_spreparefiletransferHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferHU);
}
/*
 *******************************************************************
 * Function:StartAck
 *
 * Description :Callback for received StartAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerHU::StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_spreparefiletransferHU.DeviceID = sourceDeviceID;
    m_spreparefiletransferHU.status = STATUS_UNDEFINED;
    m_spreparefiletransferHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferHU);
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property module version
 *               is changed by IBox or Amplifier
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerHU::OnChange()
{

    m_SWDLFBlockControlContext->ReadModuleVersion(SWDLID_HU);
}
/*
 *******************************************************************
 * Function: CPFTransferHandlerModVersionListenerIBOX
 *
 * Description :This is constructor of CPFTransferHandlerModVersionListenerIBOX class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */


CPFTransferHandlerModVersionListenerIBOX::CPFTransferHandlerModVersionListenerIBOX(CSWDLFBlockShadowControl* context)
{

    m_SWDLShadowControlContext = context;
    m_spreparefiletransferIbox.DeviceID = 0x0000;
    m_spreparefiletransferIbox.senderhandle = 0x0000;
    m_spreparefiletransferIbox.status = 0;
    m_spreparefiletransferIbox.swdlid = 0;
    m_spreparefiletransferIbox.filesize = 0;
    m_spreparefiletransferIbox.swdlFilename = (const char *)0;
}
/*
 *******************************************************************
 * Function: ~CPFTransferHandlerModVersionListenerIBOX
 *
 * Description :This is destructor of CPFTransferHandlerModVersionListenerIBOX class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CPFTransferHandlerModVersionListenerIBOX::~CPFTransferHandlerModVersionListenerIBOX()
{

}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerIBOX::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_spreparefiletransferIbox.status = STATUS_ERRORACK;
    m_spreparefiletransferIbox.DeviceID = sourceDeviceID;
    m_spreparefiletransferIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferIbox);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation.
 *              Updates Processing Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerIBOX::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_spreparefiletransferIbox.status = STATUS_PROCESSINGACK;
    m_spreparefiletransferIbox.DeviceID = sourceDeviceID;
    m_spreparefiletransferIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferIbox);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerIBOX::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_spreparefiletransferIbox.status = STATUS_UNDEFINED;
    m_spreparefiletransferIbox.DeviceID = sourceDeviceID;
    m_spreparefiletransferIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferIbox);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerIBOX::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_spreparefiletransferIbox.status = STATUS_UNDEFINED;
    m_spreparefiletransferIbox.DeviceID = sourceDeviceID;
    m_spreparefiletransferIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferIbox);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation.
 *              Updates Result Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerIBOX::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_spreparefiletransferIbox.status = STATUS_RESULTACK;
    m_spreparefiletransferIbox.DeviceID = sourceDeviceID;
    m_spreparefiletransferIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferIbox);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerIBOX::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_spreparefiletransferIbox.status = STATUS_UNDEFINED;
    m_spreparefiletransferIbox.DeviceID = sourceDeviceID;
    m_spreparefiletransferIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferIbox);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerIBOX::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_spreparefiletransferIbox.status = STATUS_ERROR;
    m_spreparefiletransferIbox.DeviceID = sourceDeviceID;
    m_spreparefiletransferIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferIbox);
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property module version
 *               is changed by IBox.
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerIBOX::OnChange()
{

    m_SWDLShadowControlContext->ReadModuleVersion(SWDLID_IBOX);
}
/*
 *******************************************************************
 * Function: CPFTransferHandlerModVersionListenerAMP
 *
 * Description :This is constructor of CPFTransferHandlerModVersionListenerAMP class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : void
 *
 *******************************************************************
 */

CPFTransferHandlerModVersionListenerAMP::CPFTransferHandlerModVersionListenerAMP(CSWDLFBlockShadowControl* context)
{

    m_SWDLShadowControlContext = context;
    m_spreparefiletransferAmp.DeviceID = 0x0000;
    m_spreparefiletransferAmp.senderhandle = 0x0000;
    m_spreparefiletransferAmp.status = 0;
    m_spreparefiletransferAmp.swdlid = 0;
    m_spreparefiletransferAmp.filesize = 0;
    m_spreparefiletransferAmp.swdlFilename = (const char *)0;

}

/*
 *******************************************************************
 * Function: ~CPFTransferHandlerModVersionListenerAMP
 *
 * Description :This is destructor of CPFTransferHandlerModVersionListenerAMP class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CPFTransferHandlerModVersionListenerAMP::~CPFTransferHandlerModVersionListenerAMP()
{

}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerAMP::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_spreparefiletransferAmp.status = STATUS_ERRORACK;
    m_spreparefiletransferAmp.DeviceID = sourceDeviceID;
    m_spreparefiletransferAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferAmp);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation.
 *              Updates Processing Ack to SWDL shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerAMP::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_spreparefiletransferAmp.status = STATUS_PROCESSINGACK;
    m_spreparefiletransferAmp.DeviceID = sourceDeviceID;
    m_spreparefiletransferAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferAmp);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerAMP::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_spreparefiletransferAmp.status = STATUS_UNDEFINED;
    m_spreparefiletransferAmp.DeviceID = sourceDeviceID;
    m_spreparefiletransferAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferAmp);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerAMP::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_spreparefiletransferAmp.status = STATUS_UNDEFINED;
    m_spreparefiletransferAmp.DeviceID = sourceDeviceID;
    m_spreparefiletransferAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferAmp);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation.
 *              Updates Result Ack to SWDL shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerAMP::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_spreparefiletransferAmp.status = STATUS_RESULTACK;
    m_spreparefiletransferAmp.DeviceID = sourceDeviceID;
    m_spreparefiletransferAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferAmp);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerAMP::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_spreparefiletransferAmp.status = STATUS_UNDEFINED;
    m_spreparefiletransferAmp.DeviceID = sourceDeviceID;
    m_spreparefiletransferAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferAmp);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerAMP::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_spreparefiletransferAmp.status = STATUS_ERROR;
    m_spreparefiletransferAmp.DeviceID = sourceDeviceID;
    m_spreparefiletransferAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->PrepareFileTransfer(payloadLength,payload,m_spreparefiletransferAmp);
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property module version
 *               is changed by Amplifier.
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CPFTransferHandlerModVersionListenerAMP::OnChange()
{

    m_SWDLShadowControlContext->ReadModuleVersion(SWDLID_AMP);
}
/*
 *******************************************************************
 * Function: CTransferDataCMSHandlerHU
 *
 * Description :This is constructor of CTransferDataCMSHandlerHU class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CTransferDataCMSHandlerHU::CTransferDataCMSHandlerHU(CSWDLFBlockShadowControl *context){

    m_SWDLFBlockControlContext = context;
    m_stransferdataCMSHU.DeviceID = 0x0000;
    m_stransferdataCMSHU.senderhandle = 0x0000;
    m_stransferdataCMSHU.status = 0;
    m_stransferdataCMSHU.swdlid = 0;
    m_stransferdataCMSHU.blockcounter = 0;
    m_stransferdataCMSHU.swdldata = 0;
}
/*
 *******************************************************************
 * Function: ~CTransferDataCMSHandlerHU
 *
 * Description :This is destructor of CTransferDataCMSHandlerHU class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CTransferDataCMSHandlerHU::~CTransferDataCMSHandlerHU(){

}
/*
 *******************************************************************
 * Function:GetInterface
 *
 * Description :Callback for received GetInterface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerHU::GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_stransferdataCMSHU.DeviceID = sourceDeviceID;
    m_stransferdataCMSHU.status = STATUS_UNDEFINED;
    m_stransferdataCMSHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSHU);
}
/*
 *******************************************************************
 * Function:Start
 *
 * Description :Callback for received Start operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerHU::Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_stransferdataCMSHU.DeviceID = sourceDeviceID;
    m_stransferdataCMSHU.status = STATUS_UNDEFINED;
    m_stransferdataCMSHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSHU);
}
/*
 *******************************************************************
 * Function:Abort
 *
 * Description :Callback for received Abort operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerHU::Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_stransferdataCMSHU.DeviceID = sourceDeviceID;
    m_stransferdataCMSHU.status = STATUS_UNDEFINED;
    m_stransferdataCMSHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSHU);
}
/*
 *******************************************************************
 * Function:StartResult
 *
 * Description :Callback for received StartResult operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerHU::StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_stransferdataCMSHU.DeviceID = sourceDeviceID;
    m_stransferdataCMSHU.status = STATUS_UNDEFINED;
    m_stransferdataCMSHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSHU);
}
/*
 *******************************************************************
 * Function:StartResultAck
 *
 * Description :Callback for received StartResultAck operation.
 *              Updates Start Result Ack to SWDL FBlock control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerHU::StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_stransferdataCMSHU.DeviceID = sourceDeviceID;
    m_stransferdataCMSHU.status = STATUS_STARTRESULTACK;
    m_stransferdataCMSHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSHU);
}
/*
 *******************************************************************
 * Function:AbortAck
 *
 * Description :Callback for received AbortAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerHU::AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_stransferdataCMSHU.DeviceID = sourceDeviceID;
    m_stransferdataCMSHU.status = STATUS_ABORTACK;
    m_stransferdataCMSHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSHU);
}
/*
 *******************************************************************
 * Function:StartAck
 *
 * Description :Callback for received StartAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerHU::StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_stransferdataCMSHU.DeviceID = sourceDeviceID;
    m_stransferdataCMSHU.status = STATUS_UNDEFINED;
    m_stransferdataCMSHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSHU);
}
/*
 *******************************************************************
 * Function: CTransferDataCMSHandlerIBOX
 *
 * Description :This is constructor of CTransferDataCMSHandlerIBOX class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */

CTransferDataCMSHandlerIBOX::CTransferDataCMSHandlerIBOX(CSWDLFBlockShadowControl* context)
{
    m_SWDLShadowControlContext = context;
    m_stransferdataCMSIbox.DeviceID = 0x0000;
    m_stransferdataCMSIbox.senderhandle = 0x0000;
    m_stransferdataCMSIbox.status = 0;
    m_stransferdataCMSIbox.swdlid = 0;
    m_stransferdataCMSIbox.blockcounter = 0;
    m_stransferdataCMSIbox.swdldata = 0;
}
/*
 *******************************************************************
 * Function: ~CTransferDataCMSHandlerIBOX
 *
 * Description :This is destructor of CTransferDataCMSHandlerIBOX class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CTransferDataCMSHandlerIBOX::~CTransferDataCMSHandlerIBOX()
{

}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerIBOX::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataCMSIbox.status = STATUS_ERRORACK;
    m_stransferdataCMSIbox.DeviceID = sourceDeviceID;
    m_stransferdataCMSIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSIbox);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation.
 *              Updates Processing Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerIBOX::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataCMSIbox.status = STATUS_PROCESSINGACK;
    m_stransferdataCMSIbox.DeviceID = sourceDeviceID;
    m_stransferdataCMSIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSIbox);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerIBOX::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataCMSIbox.status = STATUS_UNDEFINED;
    m_stransferdataCMSIbox.DeviceID = sourceDeviceID;
    m_stransferdataCMSIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSIbox);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerIBOX::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataCMSIbox.status = STATUS_UNDEFINED;
    m_stransferdataCMSIbox.DeviceID = sourceDeviceID;
    m_stransferdataCMSIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSIbox);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation.
 *              Updates result Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerIBOX::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataCMSIbox.status = STATUS_RESULTACK;
    m_stransferdataCMSIbox.DeviceID = sourceDeviceID;
    m_stransferdataCMSIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSIbox);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerIBOX::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataCMSIbox.status = STATUS_UNDEFINED;
    m_stransferdataCMSIbox.DeviceID = sourceDeviceID;
    m_stransferdataCMSIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSIbox);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerIBOX::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataCMSIbox.status = STATUS_ERROR;
    m_stransferdataCMSIbox.DeviceID = sourceDeviceID;
    m_stransferdataCMSIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSIbox);
}
/*
 *******************************************************************
 * Function: CTransferDataCMSHandlerAMP
 *
 * Description :This is constructor of CTransferDataCMSHandlerAMP class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */

CTransferDataCMSHandlerAMP::CTransferDataCMSHandlerAMP(CSWDLFBlockShadowControl* context)
{

    m_SWDLShadowControlContext = context;
    m_stransferdataCMSAmp.DeviceID = 0x0000;
    m_stransferdataCMSAmp.senderhandle = 0x0000;
    m_stransferdataCMSAmp.status = 0;
    m_stransferdataCMSAmp.swdlid = 0;
    m_stransferdataCMSAmp.blockcounter = 0;
    m_stransferdataCMSAmp.swdldata = 0;
}

/*
 *******************************************************************
 * Function: ~CTransferDataCMSHandlerAMP
 *
 * Description :This is destructor of CTransferDataCMSHandlerAMP class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CTransferDataCMSHandlerAMP::~CTransferDataCMSHandlerAMP()
{

}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerAMP::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataCMSAmp.status = STATUS_ERRORACK;
    m_stransferdataCMSAmp.DeviceID = sourceDeviceID;
    m_stransferdataCMSAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSAmp);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation.
 *              Updates Processing Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerAMP::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataCMSAmp.status = STATUS_PROCESSINGACK;
    m_stransferdataCMSAmp.DeviceID = sourceDeviceID;
    m_stransferdataCMSAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSAmp);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerAMP::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataCMSAmp.status = STATUS_UNDEFINED;
    m_stransferdataCMSAmp.DeviceID = sourceDeviceID;
    m_stransferdataCMSAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSAmp);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerAMP::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataCMSAmp.status = STATUS_UNDEFINED;
    m_stransferdataCMSAmp.DeviceID = sourceDeviceID;
    m_stransferdataCMSAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSAmp);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerAMP::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataCMSAmp.status = STATUS_RESULTACK;
    m_stransferdataCMSAmp.DeviceID = sourceDeviceID;
    m_stransferdataCMSAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSAmp);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerAMP::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataCMSAmp.status = STATUS_UNDEFINED;
    m_stransferdataCMSAmp.DeviceID = sourceDeviceID;
    m_stransferdataCMSAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSAmp);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataCMSHandlerAMP::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataCMSAmp.status = STATUS_ERROR;
    m_stransferdataCMSAmp.DeviceID = sourceDeviceID;
    m_stransferdataCMSAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->TransferDataCMS(payloadLength,payload,m_stransferdataCMSAmp);
}
/*
 *******************************************************************
 * Function: CTransferDataAMSHandlerHU
 *
 * Description :This is constructor of CTransferDataAMSHandlerHU class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CTransferDataAMSHandlerHU::CTransferDataAMSHandlerHU(CSWDLFBlockShadowControl *context){

    m_SWDLFBlockControlContext = context;
    m_stransferdataAMSHU.DeviceID = 0x0000;
    m_stransferdataAMSHU.senderhandle = 0x0000;
    m_stransferdataAMSHU.status = 0;
    m_stransferdataAMSHU.swdlid = 0;
    m_stransferdataAMSHU.blockcounter = 0;
    m_stransferdataAMSHU.swdldata = 0;
}
/*
 *******************************************************************
 * Function: ~CTransferDataAMSHandlerHU
 *
 * Description :This is destructor of CTransferDataAMSHandlerHU class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CTransferDataAMSHandlerHU::~CTransferDataAMSHandlerHU(){

}
/*
 *******************************************************************
 * Function:GetInterface
 *
 * Description :Callback for received GetInterface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerHU::GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_stransferdataAMSHU.DeviceID = sourceDeviceID;
    m_stransferdataAMSHU.status = STATUS_UNDEFINED;
    m_stransferdataAMSHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSHU);
}
/*
 *******************************************************************
 * Function:Start
 *
 * Description :Callback for received Start operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerHU::Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_stransferdataAMSHU.DeviceID = sourceDeviceID;
    m_stransferdataAMSHU.status = STATUS_UNDEFINED;
    m_stransferdataAMSHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSHU);
}
/*
 *******************************************************************
 * Function:Abort
 *
 * Description :Callback for received Abort operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerHU::Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_stransferdataAMSHU.DeviceID = sourceDeviceID;
    m_stransferdataAMSHU.status = STATUS_UNDEFINED;
    m_stransferdataAMSHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSHU);
}
/*
 *******************************************************************
 * Function:StartResult
 *
 * Description :Callback for received StartResult operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerHU::StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_stransferdataAMSHU.DeviceID = sourceDeviceID;
    m_stransferdataAMSHU.status = STATUS_UNDEFINED;
    m_stransferdataAMSHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSHU);
}
/*
 *******************************************************************
 * Function:StartResultAck
 *
 * Description :Callback for received StartResultAck operation.
 *              Updates Start Result Ack to SWDL FBlock control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerHU::StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){


    m_stransferdataAMSHU.DeviceID = sourceDeviceID;
    m_stransferdataAMSHU.status = STATUS_STARTRESULTACK;
    m_stransferdataAMSHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSHU);
}
/*
 *******************************************************************
 * Function:AbortAck
 *
 * Description :Callback for received AbortAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerHU::AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_stransferdataAMSHU.DeviceID = sourceDeviceID;
    m_stransferdataAMSHU.status = STATUS_ABORTACK;
    m_stransferdataAMSHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSHU);
}
/*
 *******************************************************************
 * Function:StartAck
 *
 * Description :Callback for received StartAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerHU::StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_stransferdataAMSHU.DeviceID = sourceDeviceID;
    m_stransferdataAMSHU.status = STATUS_UNDEFINED;
    m_stransferdataAMSHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSHU);
}
/*
 *******************************************************************
 * Function: CTransferDataAMSHandlerIBOX
 *
 * Description :This is constructor of CTransferDataAMSHandlerIBOX class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */

CTransferDataAMSHandlerIBOX::CTransferDataAMSHandlerIBOX(CSWDLFBlockShadowControl* context)
{

    m_SWDLShadowControlContext = context;
    m_stransferdataAMSIbox.DeviceID = 0x0000;
    m_stransferdataAMSIbox.senderhandle = 0x0000;
    m_stransferdataAMSIbox.status = 0;
    m_stransferdataAMSIbox.swdlid = 0;
    m_stransferdataAMSIbox.blockcounter = 0;
    m_stransferdataAMSIbox.swdldata = 0;

    //timem = 0;
    //memset(&tval, 0, sizeof(timeval));
    //memset(&tzone, 0, sizeof(timezone));

    LOG_INIT_V2(LOG_PREFIX_MOST)
}
/*
 *******************************************************************
 * Function: ~CTransferDataAMSHandlerIBOX
 *
 * Description :This is destructor of CTransferDataAMSHandlerIBOX class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CTransferDataAMSHandlerIBOX::~CTransferDataAMSHandlerIBOX()
{

}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerIBOX::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataAMSIbox.status = STATUS_ERRORACK;
    m_stransferdataAMSIbox.DeviceID = sourceDeviceID;
    m_stransferdataAMSIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSIbox);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation.
 *              Updates Processing Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerIBOX::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataAMSIbox.status = STATUS_PROCESSINGACK;
    m_stransferdataAMSIbox.DeviceID = sourceDeviceID;
    m_stransferdataAMSIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSIbox);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerIBOX::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataAMSIbox.status = STATUS_UNDEFINED;
    m_stransferdataAMSIbox.DeviceID = sourceDeviceID;
    m_stransferdataAMSIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSIbox);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerIBOX::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_stransferdataAMSIbox.status = STATUS_UNDEFINED;
    m_stransferdataAMSIbox.DeviceID = sourceDeviceID;
    m_stransferdataAMSIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSIbox);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation.
 *              Updates Result Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerIBOX::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    //gettimeofday(&tval, &tzone);
    //timem=localtime(&tval.tv_sec);

    //LOG_INFO << LOG_TAG_SWDLCONTROL << " SystemTime when TransferDataASM Handler invoked for RESULTACK : " << timem->tm_hour << ":"
    //          << timem->tm_min << ":" << timem->tm_sec << ":" << tval.tv_usec << LOG_ENDL;

    m_stransferdataAMSIbox.status = STATUS_RESULTACK;
    m_stransferdataAMSIbox.DeviceID = sourceDeviceID;
    m_stransferdataAMSIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSIbox);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerIBOX::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_stransferdataAMSIbox.status = STATUS_UNDEFINED;
    m_stransferdataAMSIbox.DeviceID = sourceDeviceID;
    m_stransferdataAMSIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSIbox);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerIBOX::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataAMSIbox.status = STATUS_ERROR;
    m_stransferdataAMSIbox.DeviceID = sourceDeviceID;
    m_stransferdataAMSIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSIbox);
}
/*
 *******************************************************************
 * Function: CTransferDataAMSHandlerAMP
 *
 * Description :This is constructor of CTransferDataAMSHandlerAMP class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */

CTransferDataAMSHandlerAMP::CTransferDataAMSHandlerAMP(CSWDLFBlockShadowControl* context)
{

    m_SWDLShadowControlContext = context;
    m_stransferdataAMSAmp.DeviceID = 0x0000;
    m_stransferdataAMSAmp.senderhandle = 0x0000;
    m_stransferdataAMSAmp.status = 0;
    m_stransferdataAMSAmp.swdlid = 0;
    m_stransferdataAMSAmp.blockcounter = 0;
    m_stransferdataAMSAmp.swdldata = 0;
}

/*
 *******************************************************************
 * Function: ~CTransferDataAMSHandlerAMP
 *
 * Description :This is destructor of CTransferDataAMSHandlerAMP class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CTransferDataAMSHandlerAMP::~CTransferDataAMSHandlerAMP()
{

}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerAMP::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataAMSAmp.status = STATUS_ERRORACK;
    m_stransferdataAMSAmp.DeviceID = sourceDeviceID;
    m_stransferdataAMSAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSAmp);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation.
 *              Updates Processing Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerAMP::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataAMSAmp.status = STATUS_PROCESSINGACK;
    m_stransferdataAMSAmp.DeviceID = sourceDeviceID;
    m_stransferdataAMSAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSAmp);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerAMP::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataAMSAmp.status = STATUS_UNDEFINED;
    m_stransferdataAMSAmp.DeviceID = sourceDeviceID;
    m_stransferdataAMSAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSAmp);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerAMP::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataAMSAmp.status = STATUS_UNDEFINED;
    m_stransferdataAMSAmp.DeviceID = sourceDeviceID;
    m_stransferdataAMSAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSAmp);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation.
 *              Updates Result Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerAMP::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataAMSAmp.status = STATUS_RESULTACK;
    m_stransferdataAMSAmp.DeviceID = sourceDeviceID;
    m_stransferdataAMSAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSAmp);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerAMP::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataAMSAmp.status = STATUS_UNDEFINED;
    m_stransferdataAMSAmp.DeviceID = sourceDeviceID;
    m_stransferdataAMSAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSAmp);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CTransferDataAMSHandlerAMP::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_stransferdataAMSAmp.status = STATUS_ERROR;
    m_stransferdataAMSAmp.DeviceID = sourceDeviceID;
    m_stransferdataAMSAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->TransferDataAMS(payloadLength,payload,m_stransferdataAMSAmp);

}
/*
 *******************************************************************
 * Function: CCheckActivateModuleHandlerHU
 *
 * Description :This is constructor of CCheckActivateModuleHandlerHU class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CCheckActivateModuleHandlerHU::CCheckActivateModuleHandlerHU(CSWDLFBlockShadowControl *context){
    m_SWDLFBlockControlContext = context;
    m_scheckactivemoduleHU.DeviceID = 0x0000;
    m_scheckactivemoduleHU.senderhandle = 0x0000;
    m_scheckactivemoduleHU.status = 0;
    m_scheckactivemoduleHU.swdlid = 0;
    m_scheckactivemoduleHU.checksum = 0;
}
/*
 *******************************************************************
 * Function: ~CCheckActivateModuleHandlerHU
 *
 * Description :This is destructor of CCheckActivateModuleHandlerHU class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CCheckActivateModuleHandlerHU::~CCheckActivateModuleHandlerHU(){

}
/*
 *******************************************************************
 * Function:GetInterface
 *
 * Description :Callback for received GetInterface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerHU::GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_scheckactivemoduleHU.DeviceID = sourceDeviceID;
    m_scheckactivemoduleHU.status = STATUS_UNDEFINED;
    m_scheckactivemoduleHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleHU);
}
/*
 *******************************************************************
 * Function:Start
 *
 * Description :Callback for received Start operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerHU::Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_scheckactivemoduleHU.DeviceID = sourceDeviceID;
    m_scheckactivemoduleHU.status = STATUS_UNDEFINED;
    m_scheckactivemoduleHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleHU);
}
/*
 *******************************************************************
 * Function:Abort
 *
 * Description :Callback for received Abort operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerHU::Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_scheckactivemoduleHU.DeviceID = sourceDeviceID;
    m_scheckactivemoduleHU.status = STATUS_UNDEFINED;
    m_scheckactivemoduleHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleHU);
}
/*
 *******************************************************************
 * Function:StartResult
 *
 * Description :Callback for received StartResult operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerHU::StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_scheckactivemoduleHU.DeviceID = sourceDeviceID;
    m_scheckactivemoduleHU.status = STATUS_UNDEFINED;
    m_scheckactivemoduleHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleHU);
}
/*
 *******************************************************************
 * Function:StartResultAck
 *
 * Description :Callback for received StartResultAck operation.
 *              Updates Start Result Ack to SWDL FBlock control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerHU::StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){


    m_scheckactivemoduleHU.DeviceID = sourceDeviceID;
    m_scheckactivemoduleHU.status = STATUS_STARTRESULTACK;
    m_scheckactivemoduleHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleHU);
}
/*
 *******************************************************************
 * Function:AbortAck
 *
 * Description :Callback for received AbortAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerHU::AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_scheckactivemoduleHU.DeviceID = sourceDeviceID;
    m_scheckactivemoduleHU.status = STATUS_ABORTACK;
    m_scheckactivemoduleHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleHU);
}
/*
 *******************************************************************
 * Function:StartAck
 *
 * Description :Callback for received StartAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerHU::StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_scheckactivemoduleHU.DeviceID = sourceDeviceID;
    m_scheckactivemoduleHU.status = STATUS_UNDEFINED;
    m_scheckactivemoduleHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleHU);
}
/*
 *******************************************************************
 * Function: CCheckActivateModuleHandlerIBOX
 *
 * Description :This is constructor of CCheckActivateModuleHandlerIBOX class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */

CCheckActivateModuleHandlerIBOX::CCheckActivateModuleHandlerIBOX(CSWDLFBlockShadowControl* context)
{

    m_SWDLShadowControlContext = context;
    m_scheckactivemoduleIbox.DeviceID = 0x0000;
    m_scheckactivemoduleIbox.senderhandle = 0x0000;
    m_scheckactivemoduleIbox.status = 0;
    m_scheckactivemoduleIbox.swdlid = 0;
    m_scheckactivemoduleIbox.checksum = 0;
}
/*
 *******************************************************************
 * Function: ~CCheckActivateModuleHandlerIBOX
 *
 * Description :This is destructor of CCheckActivateModuleHandlerIBOX class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CCheckActivateModuleHandlerIBOX::~CCheckActivateModuleHandlerIBOX()
{

}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerIBOX::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_scheckactivemoduleIbox.status = STATUS_ERRORACK;
    m_scheckactivemoduleIbox.DeviceID = sourceDeviceID;
    m_scheckactivemoduleIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleIbox);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation.
 *              Updates Processing Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerIBOX::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_scheckactivemoduleIbox.status = STATUS_PROCESSINGACK;
    m_scheckactivemoduleIbox.DeviceID = sourceDeviceID;
    m_scheckactivemoduleIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleIbox);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerIBOX::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_scheckactivemoduleIbox.status = STATUS_UNDEFINED;
    m_scheckactivemoduleIbox.DeviceID = sourceDeviceID;
    m_scheckactivemoduleIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleIbox);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerIBOX::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_scheckactivemoduleIbox.status = STATUS_UNDEFINED;
    m_scheckactivemoduleIbox.DeviceID = sourceDeviceID;
    m_scheckactivemoduleIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleIbox);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation.
 *              Updates Result Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerIBOX::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_scheckactivemoduleIbox.status = STATUS_RESULTACK;
    m_scheckactivemoduleIbox.DeviceID = sourceDeviceID;
    m_scheckactivemoduleIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleIbox);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerIBOX::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_scheckactivemoduleIbox.status = STATUS_UNDEFINED;
    m_scheckactivemoduleIbox.DeviceID = sourceDeviceID;
    m_scheckactivemoduleIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleIbox);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerIBOX::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_scheckactivemoduleIbox.status = STATUS_ERROR;
    m_scheckactivemoduleIbox.DeviceID = sourceDeviceID;
    m_scheckactivemoduleIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleIbox);
}
/*
 *******************************************************************
 * Function: CCheckActivateModuleHandlerAMP
 *
 * Description :This is constructor of CCheckActivateModuleHandlerAMP class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */

CCheckActivateModuleHandlerAMP::CCheckActivateModuleHandlerAMP(CSWDLFBlockShadowControl* context)
{

    m_SWDLShadowControlContext = context;
    m_scheckactivemoduleAmp.DeviceID = 0x0000;
    m_scheckactivemoduleAmp.senderhandle = 0x0000;
    m_scheckactivemoduleAmp.status = 0;
    m_scheckactivemoduleAmp.swdlid = 0;
    m_scheckactivemoduleAmp.checksum = 0;
}

/*
 *******************************************************************
 * Function: ~CCheckActivateModuleHandlerAMP
 *
 * Description :This is destructor of CCheckActivateModuleHandlerAMP class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CCheckActivateModuleHandlerAMP::~CCheckActivateModuleHandlerAMP()
{

}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerAMP::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_scheckactivemoduleAmp.status = STATUS_ERRORACK;
    m_scheckactivemoduleAmp.DeviceID = sourceDeviceID;
    m_scheckactivemoduleAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleAmp);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation.
 *              Updates Processing Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerAMP::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_scheckactivemoduleAmp.status = STATUS_PROCESSINGACK;
    m_scheckactivemoduleAmp.DeviceID = sourceDeviceID;
    m_scheckactivemoduleAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleAmp);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerAMP::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_scheckactivemoduleAmp.status = STATUS_UNDEFINED;
    m_scheckactivemoduleAmp.DeviceID = sourceDeviceID;
    m_scheckactivemoduleAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleAmp);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerAMP::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_scheckactivemoduleAmp.status = STATUS_UNDEFINED;
    m_scheckactivemoduleAmp.DeviceID = sourceDeviceID;
    m_scheckactivemoduleAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleAmp);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation.
 *              Updates Result Ack to SWDL Shadow control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerAMP::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_scheckactivemoduleAmp.status = STATUS_RESULTACK;
    m_scheckactivemoduleAmp.DeviceID = sourceDeviceID;
    m_scheckactivemoduleAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleAmp);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerAMP::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_scheckactivemoduleAmp.status = STATUS_UNDEFINED;
    m_scheckactivemoduleAmp.DeviceID = sourceDeviceID;
    m_scheckactivemoduleAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleAmp);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CCheckActivateModuleHandlerAMP::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_scheckactivemoduleAmp.status = STATUS_ERROR;
    m_scheckactivemoduleAmp.DeviceID = sourceDeviceID;
    m_scheckactivemoduleAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->CheckActivateModule(payloadLength,payload,m_scheckactivemoduleAmp);
}
/*
 *******************************************************************
 * Function: CRequestTransferExitHandlerHU
 *
 * Description :This is constructor of CRequestTransferExitHandlerHU class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CRequestTransferExitHandlerHU::CRequestTransferExitHandlerHU(CSWDLFBlockShadowControl *context){

    m_SWDLFBlockControlContext = context;
    m_srequesttransferexitHU.DeviceID = 0x0000;
    m_srequesttransferexitHU.senderhandle = 0x0000;
    m_srequesttransferexitHU.status = 0;
    m_srequesttransferexitHU.swdlid = 0;
}
/*
 *******************************************************************
 * Function: ~CRequestTransferExitHandlerHU
 *
 * Description :This is destructor of CRequestTransferExitHandlerHU class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CRequestTransferExitHandlerHU::~CRequestTransferExitHandlerHU(){

}
/*
 *******************************************************************
 * Function:GetInterface
 *
 * Description :Callback for received GetInterface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */

void CRequestTransferExitHandlerHU::GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_srequesttransferexitHU.DeviceID = sourceDeviceID;
    m_srequesttransferexitHU.status = STATUS_UNDEFINED;
    m_srequesttransferexitHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitHU);
}
/*
 *******************************************************************
 * Function:Start
 *
 * Description :Callback for received Start operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerHU::Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_srequesttransferexitHU.DeviceID = sourceDeviceID;
    m_srequesttransferexitHU.status = STATUS_UNDEFINED;
    m_srequesttransferexitHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitHU);
}
/*
 *******************************************************************
 * Function:Abort
 *
 * Description :Callback for received Abort operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerHU::Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_srequesttransferexitHU.DeviceID = sourceDeviceID;
    m_srequesttransferexitHU.status = STATUS_UNDEFINED;
    m_srequesttransferexitHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitHU);
}
/*
 *******************************************************************
 * Function:StartResult
 *
 * Description :Callback for received StartResult operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerHU::StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_srequesttransferexitHU.DeviceID = sourceDeviceID;
    m_srequesttransferexitHU.status = STATUS_UNDEFINED;
    m_srequesttransferexitHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitHU);
}
/*
 *******************************************************************
 * Function:StartResultAck
 *
 * Description :Callback for received StartResultAck operation.
 *              Updates Start Result Ack to SWDL FBlock control.
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerHU::StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){


    m_srequesttransferexitHU.DeviceID = sourceDeviceID;
    m_srequesttransferexitHU.status = STATUS_STARTRESULTACK;
    m_srequesttransferexitHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitHU);
}
/*
 *******************************************************************
 * Function:AbortAck
 *
 * Description :Callback for received AbortAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerHU::AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_srequesttransferexitHU.DeviceID = sourceDeviceID;
    m_srequesttransferexitHU.status = STATUS_ABORTACK;
    m_srequesttransferexitHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitHU);
}
/*
 *******************************************************************
 * Function:StartAck
 *
 * Description :Callback for received StartAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerHU::StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

    m_srequesttransferexitHU.DeviceID = sourceDeviceID;
    m_srequesttransferexitHU.status = STATUS_UNDEFINED;
    m_srequesttransferexitHU.swdlid = SWDLID_HU;
    m_SWDLFBlockControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitHU);
}
/*
 *******************************************************************
 * Function: CRequestTransferExitHandlerIBOX
 *
 * Description :This is constructor of CRequestTransferExitHandlerIBOX class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */

CRequestTransferExitHandlerIBOX::CRequestTransferExitHandlerIBOX(CSWDLFBlockShadowControl* context)
{

    m_SWDLShadowControlContext = context;
    m_srequesttransferexitIbox.DeviceID = 0x0000;
    m_srequesttransferexitIbox.senderhandle = 0x0000;
    m_srequesttransferexitIbox.status = 0;
    m_srequesttransferexitIbox.swdlid = 0;
}
/*
 *******************************************************************
 * Function: ~CRequestTransferExitHandlerIBOX
 *
 * Description :This is destructor of CRequestTransferExitHandlerIBOX class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CRequestTransferExitHandlerIBOX::~CRequestTransferExitHandlerIBOX()
{

}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerIBOX::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequesttransferexitIbox.status = STATUS_ERRORACK;
    m_srequesttransferexitIbox.DeviceID = sourceDeviceID;
    m_srequesttransferexitIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitIbox);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation
 *              Updates Processing Ack to SWDL Shadow control
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerIBOX::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequesttransferexitIbox.status = STATUS_PROCESSINGACK;
    m_srequesttransferexitIbox.DeviceID = sourceDeviceID;
    m_srequesttransferexitIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitIbox);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerIBOX::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequesttransferexitIbox.status = STATUS_UNDEFINED;
    m_srequesttransferexitIbox.DeviceID = sourceDeviceID;
    m_srequesttransferexitIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitIbox);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerIBOX::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequesttransferexitIbox.status = STATUS_UNDEFINED;
    m_srequesttransferexitIbox.DeviceID = sourceDeviceID;
    m_srequesttransferexitIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitIbox);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerIBOX::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequesttransferexitIbox.status = STATUS_RESULTACK;
    m_srequesttransferexitIbox.DeviceID = sourceDeviceID;
    m_srequesttransferexitIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitIbox);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerIBOX::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequesttransferexitIbox.status = STATUS_UNDEFINED;
    m_srequesttransferexitIbox.DeviceID = sourceDeviceID;
    m_srequesttransferexitIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitIbox);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerIBOX::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequesttransferexitIbox.status = STATUS_ERROR;
    m_srequesttransferexitIbox.DeviceID = sourceDeviceID;
    m_srequesttransferexitIbox.swdlid = SWDLID_IBOX;
    m_SWDLShadowControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitIbox);
}
/*
 *******************************************************************
 * Function: CRequestTransferExitHandlerAMP
 *
 * Description :This is constructor of CRequestTransferExitHandlerAMP class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */

CRequestTransferExitHandlerAMP::CRequestTransferExitHandlerAMP(CSWDLFBlockShadowControl* context)
{

    m_SWDLShadowControlContext = context;
    m_srequesttransferexitAmp.DeviceID = 0x0000;
    m_srequesttransferexitAmp.senderhandle = 0x0000;
    m_srequesttransferexitAmp.status = 0;
    m_srequesttransferexitAmp.swdlid = 0;
}
/*
 *******************************************************************
 * Function: ~CRequestTransferExitHandlerAMP
 *
 * Description :This is destructor of CRequestTransferExitHandlerAMP class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CRequestTransferExitHandlerAMP::~CRequestTransferExitHandlerAMP()
{

}
/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerAMP::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequesttransferexitAmp.status = STATUS_ERRORACK;
    m_srequesttransferexitAmp.DeviceID = sourceDeviceID;
    m_srequesttransferexitAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitAmp);
}
/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerAMP::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequesttransferexitAmp.status = STATUS_PROCESSINGACK;
    m_srequesttransferexitAmp.DeviceID = sourceDeviceID;
    m_srequesttransferexitAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitAmp);
}
/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerAMP::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequesttransferexitAmp.status = STATUS_UNDEFINED;
    m_srequesttransferexitAmp.DeviceID = sourceDeviceID;
    m_srequesttransferexitAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitAmp);
}
/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerAMP::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequesttransferexitAmp.status = STATUS_UNDEFINED;
    m_srequesttransferexitAmp.DeviceID = sourceDeviceID;
    m_srequesttransferexitAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitAmp);
}
/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck operation
 *              Updates the Result Ack to SWDL Shadow control
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerAMP::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequesttransferexitAmp.status = STATUS_RESULTACK;
    m_srequesttransferexitAmp.DeviceID = sourceDeviceID;
    m_srequesttransferexitAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitAmp);
}
/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerAMP::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequesttransferexitAmp.status = STATUS_UNDEFINED;
    m_srequesttransferexitAmp.DeviceID = sourceDeviceID;
    m_srequesttransferexitAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitAmp);
}
/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 *
 * Returns :void
 *
 *******************************************************************
 */
void CRequestTransferExitHandlerAMP::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{

    m_srequesttransferexitAmp.status = STATUS_ERROR;
    m_srequesttransferexitAmp.DeviceID = sourceDeviceID;
    m_srequesttransferexitAmp.swdlid = SWDLID_AMP;
    m_SWDLShadowControlContext->RequestTransferExit(payloadLength,payload,m_srequesttransferexitAmp);
}
/*
 *******************************************************************
 * Function: CSWDLProgressInfoListenerIBOX
 *
 * Description :This is constructor of CSWDLProgressInfoListenerIBOX class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CSWDLProgressInfoListenerIBOX::CSWDLProgressInfoListenerIBOX(CSWDLFBlockShadowControl *context){

    m_SWDLFBlockControlContext = context;
}
/*
 *******************************************************************
 * Function: ~CSWDLProgressInfoListenerIBOX
 *
 * Description :This is destructor of CSWDLProgressInfoListenerIBOX class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CSWDLProgressInfoListenerIBOX::~CSWDLProgressInfoListenerIBOX(){

}
/*
 *******************************************************************
 * Function: OnChange
 *
 * Description : Method invoked by ACS Daemon when property SWDLProgressInfo
 *               is changed by IBOX.
 *
 * Parameters : none
 *
 * Returns : void
 *
 *******************************************************************
 */
void CSWDLProgressInfoListenerIBOX::OnChange(){

    m_SWDLFBlockControlContext->SWDLProgressInfo(SWDLID_IBOX);
}
/*
 *******************************************************************
 * Function: CSWDLProgressInfoListenerIBOX
 *
 * Description :This is constructor of CSWDLProgressInfoListenerIBOX class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CSWDLProgressInfoListenerAMP::CSWDLProgressInfoListenerAMP(CSWDLFBlockShadowControl *context){

    m_SWDLFBlockControlContext = context;
}
/*
 *******************************************************************
 * Function: ~CSWDLProgressInfoListenerIBOX
 *
 * Description :This is destructor of CSWDLProgressInfoListenerIBOX class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CSWDLProgressInfoListenerAMP::~CSWDLProgressInfoListenerAMP(){

}
/*
 *******************************************************************
 * Function: OnChange
 *
 * Description : Method invoked by ACS Daemon when property SWDLProgressInfo
 *               is changed by Amplifier.
 *
 * Parameters : none
 *
 * Returns : void
 *
 *******************************************************************
 */
void CSWDLProgressInfoListenerAMP::OnChange(){

    m_SWDLFBlockControlContext->SWDLProgressInfo(SWDLID_AMP);
}
/*
 *******************************************************************
 * Function: CUpdateProgressingInfoListenerIBOX
 *
 * Description :This is constructor of CUpdateProgressingInfoListenerIBOX class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CUpdateProgressingInfoListenerIBOX::CUpdateProgressingInfoListenerIBOX(CSWDLFBlockShadowControl *context)
{
    m_SWDLFBlockControlContext = context;
}
/*
 *******************************************************************
 * Function: ~CUpdateProgressingInfoListenerIBOX
 *
 * Description :This is destructor of CUpdateProgressingInfoListenerIBOX class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CUpdateProgressingInfoListenerIBOX::~CUpdateProgressingInfoListenerIBOX()
{

}
/*
 *******************************************************************
 * Function: OnChange
 *
 * Description : Method invoked by ACS Daemon when property UpdateProgressingInfo
 *               is changed by IBOX.
 *
 * Parameters : none
 *
 * Returns : void
 *
 *******************************************************************
 */
void CUpdateProgressingInfoListenerIBOX::OnChange()
{
    m_SWDLFBlockControlContext->UpdateProgressingInfoStatusSlot(SWDLID_IBOX);
}
/*
 *******************************************************************
 * Function: CUpdateProgressingInfoListenerAMP
 *
 * Description :This is constructor of CUpdateProgressingInfoListenerAMP class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CSWDLFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CUpdateProgressingInfoListenerAMP::CUpdateProgressingInfoListenerAMP(CSWDLFBlockShadowControl *context)
{
    m_SWDLFBlockControlContext = context;
}
/*
 *******************************************************************
 * Function: ~CUpdateProgressingInfoListenerIBOX
 *
 * Description :This is destructor of CUpdateProgressingInfoListenerIBOX class.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
CUpdateProgressingInfoListenerAMP::~CUpdateProgressingInfoListenerAMP()
{

}
/*
 *******************************************************************
 * Function: OnChange
 *
 * Description : Method invoked by ACS Daemon when property UpdateProgressingInfo
 *               is changed by IBOX.
 *
 * Parameters : none
 *
 * Returns : void
 *
 *******************************************************************
 */
void CUpdateProgressingInfoListenerAMP::OnChange()
{
    m_SWDLFBlockControlContext->UpdateProgressingInfoStatusSlot(SWDLID_AMP);
}


/*
 *******************************************************************
 * Function: CIBOXSWDLFblockStateListener
 *
 * Description : Method invoked by ACS Daemon when detect SWDLFBlock state by IBOX.
 *
 * Parameters : none
 *
 * Returns : void
 *
 *******************************************************************
 */
 // iBox SWDL FBlock checkling for iBox Update, 2014.2.24
CIBOXSWDLFblockStateListener::CIBOXSWDLFblockStateListener(acs::generated::shadow::SoftwareDownload::CSoftwareDownload *SoftwareDownload, CSWDLFBlockShadowControl* context)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)

    m_SoftwareDownload = SoftwareDownload;
    m_SWDLFBlockShadowControl = context;
    m_Timer = new QTimer(this);
    m_Timer->setSingleShot(true);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(OnWaitForTimeout()));
}

void CIBOXSWDLFblockStateListener::OnChange()
{
    bool state = m_SoftwareDownload->FunctionBlockVisible();

    LOG_SIGNAL << "CIBOXSWDLFblockStateListener" << "SWDL FBlock on iBox device is Visible? =" << state << LOG_ENDL;

/* send when the state is changed*/
/*
    if( !state )
    {
        // 2013.12.02 extend time from 1.5s to 2.5s
        m_Timer->start(2500);
        //gettimeofday(&tv, NULL);
    }
    else
    {
        if(m_Timer->isActive())
        {
            m_Timer->stop();
        }
        m_SWDLFBlockShadowControl->IBOXSWDLFblockStateListener(state);
    }
*/
	m_SWDLFBlockShadowControl->IBOXSWDLFblockStateListener(state);
}

void CIBOXSWDLFblockStateListener::OnWaitForTimeout()
{
    bool state = m_SoftwareDownload->FunctionBlockVisible();

#if 0
    struct timeval tv_;
    gettimeofday(&tv_, NULL);

    time_t sec = tv_.tv_sec - tv.tv_sec;
    long nsec = tv_.tv_usec - tv.tv_usec;

    LOG_SIGNAL << __FUNCTION__ << " Visible = " << state << " SEC: " << sec << " NSEC: " << nsec << LOG_ENDL;

    LOG_SIGNAL << __FUNCTION__ << " HMI FBlock (InstID 0x02) on iBox device is Visible in 1500ms? = " << state << LOG_ENDL;

    if( !state )
    {
        m_SWDLFBlockShadowControl->IBOXSWDLFblockStateListener(state);
    }
#endif    
}

