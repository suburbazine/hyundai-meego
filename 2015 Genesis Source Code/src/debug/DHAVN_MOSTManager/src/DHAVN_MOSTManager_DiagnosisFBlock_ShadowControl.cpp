/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     : MOSTManager                                       *
 * PROGRAMMER : Amit Koparde                                      *
 * DATE       : 13 th September 2011                              *
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
 *                  Diagnosis FBlock_Shadow control,responsible to*
 *                  create instance of Fblock, configure it and   *
 *                  add to most stack. if any listeners will      *
 *                  be part of this class.Communicates with ACS   *
 *                  daemon.                                       *
 *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS                                           *
 *
 *   DATE         PROGRAMMER               COMMENT                *
 ******************************************************************
 * 13 Sep 2011   Amit Koparde         Draft version
 * 15 Sep 2011   Amit Koparde         Added Debug Statement       *
 * 08 Feb 2011  Neelima Khedad           Using the latest Version of LOG_INIT function Logger module
 * 19 Sep 2012  Shiva Kumar              Modified for FCAT 1.19.0, 1.20.1, 1.20.2
 ******************************************************************
 */

#include "DHAVN_MOSTManager_DiagnosisFBlock_ShadowControl.h"
#include "DHAVN_MOSTManager_DiagnosisFBlock.h"
#include "DHAVN_MOSTManager_DiagnosisShadow.h"
#include "DHAVN_MOSTManager_MOSTStructures.h"


/*
 *******************************************************************
 * Function:      CDiagnosisFBlockShadowControl
 *
 * Description :  This is constructor of CDiagnosisFBlockShadowControl class.
 *
 * Parameters :
 *    context     rw   It holds the instance of CMarshaledACSApplication.
 *
 * Returns :      none
 *
 *******************************************************************
 */

CDiagnosisFBlockShadowControl::CDiagnosisFBlockShadowControl(CMarshaledACSApplication *context)
    : CControlThreadBase(context, LOG_TAG_DIAG_CNT)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)

    /** Head unit Diagnosis */
    m_DiagnosisFBlock = new acs::generated::fblock::Diagnosis::CDiagnosis(0x01);
    /** IBox Diagnosis */
    m_DiagnosisShadow = new acs::generated::shadow::Diagnosis::CDiagnosis(0x02);

    /** Head Unit Listeners */
    m_VariantCodeHU = new acs::generated::fblock::Diagnosis::CVariantCode(m_DiagnosisFBlock);
    m_VariantCodeListenerHU = new CVariantCodeStatusListenerHU(this);

    /** Ibox Listeners */
    m_MainSWVersionIBOX = new acs::generated::shadow::Diagnosis::CSWVersion(m_DiagnosisShadow);
    m_MainSWVersionListenerIBOX = new CMainSWVersionStatusListenerIBOX(this);

    m_DiagnosisFBlockStateListener = new CDiagnosisFBlockStateListener(m_DiagnosisShadow, this);
}

/*
 *******************************************************************
 * Function:      ~CDiagnosisFBlockShadowControl
 *
 * Description :  Is a Destructor.
 *
 * Parameters :   none
 *
 *
 * Returns :      none
 *
 *******************************************************************
 */

CDiagnosisFBlockShadowControl::~CDiagnosisFBlockShadowControl()
{
    delete m_DiagnosisFBlock;
    delete m_DiagnosisShadow;

    delete m_VariantCodeHU;
    delete m_VariantCodeListenerHU;

    delete m_MainSWVersionIBOX;
    delete m_MainSWVersionListenerIBOX;

    delete m_DiagnosisFBlockStateListener;
}

/*
 *******************************************************************
 * Function:     Initialize
 *
 * Description : Function initializes the MOST elements to Diagnosis
 *              FBlock_Shadow and adds to MOST.
 *
 * Parameters :none
 *
 *
 * Returns :    true or false
 *
 *******************************************************************
 */

bool CDiagnosisFBlockShadowControl::Initialize()
{
    /** Initialze notification for all parameters on FBlock and Shadow */
    if(m_DiagnosisFBlock->InitializeMOSTElement() && m_DiagnosisShadow->InitializeMOSTElement())
    {
        SetListeners();

	//========= variant value init =========== //
	QDBusInterface micom("com.lge.car.micom", "/SysInfo", "", QDBusConnection::sessionBus(), this);
	QDBusPendingReply<QByteArray> reply = micom.asyncCall(QLatin1String("LgeDBus_AskSysOption"));
	QDBusPendingReply<QByteArray> reply2 = micom.asyncCall(QLatin1String("LgeDBus_AskSysVersion"));		

	QByteArray result = reply.value();	
	if(reply.isError())
	{
		m_VariantCodeHU->VehicleVariantValue((acs::generated::fblock::Diagnosis::CVariantCode::TVehicleVariant)0x00);
		m_VariantCodeHU->CountryVariantValue((acs::generated::fblock::Diagnosis::CVariantCode::TCountryVariant)0x00);
		m_VariantCodeHU->SystemVariantValue(0x0000008F);
		m_VariantCodeHU->DVDRegionValue((acs::generated::fblock::Diagnosis::CVariantCode::TDVDRegion)0x00);
		m_VariantCodeHU->DVDMenuLanguageValue(0x6B6F);
		m_VariantCodeHU->UpdateShadows();

            MOST_LOG_HIGH << LOG_TAG_DIAG_CNT << " Boot :: VariantCode_Status --> Default value "<< MOST_LOG_ENDL;			
	}
	else
	{
		uchar vehicleVariant = result.at(0);
		uchar countryVariant = result.at(1);
		uint systemVariant = ((uchar)result.at(2)) << 8*3 | ((uchar)result.at(3)) << 8*2 | ((uchar)result.at(4)) << 8*1 | (uchar)result.at(5);
		uchar dvdRegion = result.at(6);
		ushort dvdMenuLang = ((uchar)result.at(7)) << 8 | (uchar)result.at(8);

		MOST_LOG_HIGH << LOG_TAG_DIAG_CNT << " Boot :: VariantCode_Status --> : Vehicle = " << vehicleVariant
            << " Country = " << countryVariant << " System = " << systemVariant << " DVDRegion = " <<  dvdRegion
            << " DVDMenuLanguage = " << dvdMenuLang << MOST_LOG_ENDL;

		m_VariantCodeHU->VehicleVariantValue((acs::generated::fblock::Diagnosis::CVariantCode::TVehicleVariant)vehicleVariant);
		m_VariantCodeHU->CountryVariantValue((acs::generated::fblock::Diagnosis::CVariantCode::TCountryVariant)countryVariant);
		m_VariantCodeHU->SystemVariantValue(systemVariant);
		m_VariantCodeHU->DVDRegionValue((acs::generated::fblock::Diagnosis::CVariantCode::TDVDRegion)dvdRegion);
		m_VariantCodeHU->DVDMenuLanguageValue(dvdMenuLang);
		m_VariantCodeHU->UpdateShadows();
	}

	//*===================== add 2014.09.24 =====================*//
	QString MainSWversion, FCatVersion, CANDBVersion = "";
	uint8_t line_index = 0;
	uchar Variant_value = result.at(1);

	// ------MAIN SW Version -----------//	
	switch ( Variant_value )
	{
		// 0 :kr, 1 :n.a, 2 :cn, 3 :gen, 4 :m.e, 5 :eu, 6 :canada, 7 :rus
		case 0:			line_index = 0; break;
		case 1:			line_index = 1; break;
		case 2:			line_index = 2; break;
		case 3:			line_index = 3; break;
		case 4:			line_index = 4; break;
		case 5:			line_index = 5; break;
		case 6:			line_index = 6; break;
		case 7:			line_index = 7; break;
		default:			line_index = 0; break;
	}
	QFile swVerFile( QString("/etc/version.txt") );
	if ( !swVerFile.open(QIODevice::ReadOnly | QIODevice::Text) )
	{
		MOST_LOG_HIGH << LOG_TAG_DIAG_CNT << " can't open file /etc/version.txt " << MOST_LOG_ENDL;
	}
	else
	{
		QTextStream swVerFileStream(&swVerFile);
		do
		{
			QString string = swVerFileStream.readLine();

			if ( !string.isNull() )
			{
				MainSWversion = string;
			}
		}
		while( line_index-- );
	}
	swVerFile.close();

	// ------MOST FCat Version-----------//
	FCatVersion = QString::number(MOST_VERSION_YY) + "." + QString::number(MOST_VERSION_MM) + "." + QString::number(MOST_VERSION_DD);

	// ------CAN DB Version-----------//
	QByteArray result_candb = reply2.value();
	if(reply2.isError())
	{
		CANDBVersion = "00.00";		
	}
	else
	{
		QByteArray can_major = QByteArray(1, result_candb.at(22)).toHex();
		QByteArray can_minor = QByteArray(1, result_candb.at(23)).toHex();		
		CANDBVersion = can_major.data() + QString::fromUtf8(".") + can_minor.data();				
	}

	acs::generated::fblock::Diagnosis::CSWVersion swversion(m_DiagnosisFBlock);

	swversion.MainSWString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(), 
	reinterpret_cast<const uint8_t*>(MainSWversion.toUtf8().data()), MainSWversion.length()+1);
	swversion.FCatString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(), 
	reinterpret_cast<const uint8_t*>(FCatVersion.toUtf8().data()), FCatVersion.length()+1);
	swversion.CANDBString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(), 
	reinterpret_cast<const uint8_t*>(CANDBVersion.toUtf8().data()), CANDBVersion.length()+1);
	swversion.UpdateShadows();

	MOST_LOG_HIGH << LOG_TAG_DIAG_CNT << " Boot :: SWVersion_Status --> : MainSW = " << MainSWversion
	<< " FCat = " << FCatVersion << " CANDB = " << CANDBVersion << MOST_LOG_ENDL;
	
	//*================== add 2014.09.24 ================= */

	m_Initialized = true;
    }

    return m_Initialized;
}

/*
 *******************************************************************
 * Function:SetListeners
 *
 * Description : Sets the listeners for all the properties defined
 *
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CDiagnosisFBlockShadowControl::SetListeners()
{
    /** Setting the listeners for Properties on Head Unit */
    m_VariantCodeHU->SetListener(m_VariantCodeListenerHU);

    /** Setting the listeners for Properties on Ibox */
    m_MainSWVersionIBOX->SetListener(m_MainSWVersionListenerIBOX);

    m_DiagnosisShadow->SetListener(m_DiagnosisFBlockStateListener);
}

/*
 *******************************************************************
 * Function:RemoveListeners
 *
 * Description : Removes the listeners for all the properties registered.
 *
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CDiagnosisFBlockShadowControl::RemoveListeners()
{
    /** Removing the listeners for Properties on Head Unit */
    m_VariantCodeHU->RemoveListener(m_VariantCodeListenerHU);

    /** Removing the listeners for Properties on Ibox*/
    m_MainSWVersionIBOX->RemoveListener(m_MainSWVersionListenerIBOX);

    m_DiagnosisShadow->RemoveListener(m_DiagnosisFBlockStateListener);
}

/*
 *******************************************************************
 * Function:CVariantCodeStatusListenerHU
 *
 * Description :Constructor for CVariantCodeStatusListenerHU.
 *              Stores the context of CDiagnosisFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

CVariantCodeStatusListenerHU::CVariantCodeStatusListenerHU(CDiagnosisFBlockShadowControl *context)
{
    m_DiagnosisFBlock_ShadowControl = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property VariantCodeStatus
 *               changed.
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CVariantCodeStatusListenerHU::OnChange()
{   
    m_DiagnosisFBlock_ShadowControl->GetVariantCodeStatus_emit();
}

/*
void CVariantCodeStatusListenerHU::OnGetReceived()
{
    //m_DiagnosisFBlock_ShadowControl->GetVariantCodeToHU();
}
*/

/*
 *******************************************************************
 * Function:CMainSWVersionStatusListenerIBOX
 *
 * Description :Constructor for CMainSWVersionStatusListenerIBOX.
 *              Stores the context of CDiagnosisFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

CMainSWVersionStatusListenerIBOX::CMainSWVersionStatusListenerIBOX(CDiagnosisFBlockShadowControl *context)
{
    m_DiagnosisFBlock_ShadowControl = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property MainSWVersionStatus
 *               changed.
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CMainSWVersionStatusListenerIBOX::OnChange()
{    
    m_DiagnosisFBlock_ShadowControl->MainSWVersionStatus();
}

/*
 *******************************************************************
 * Function:CMainSWVersionStatusListenerAMP
 *
 * Description :Constructor for CMainSWVersionStatusListenerAMP.
 *              Stores the context of CDiagnosisFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */


/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property MainSWVersionStatus
 *               changed.
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */


/*
 *******************************************************************
 * Function: GetVariantCodeStatus_emit
 *
 * Description : Emits the signal GetVariantCodeStatus.
 *
 * Parameters :
 *      none
 *
 * Emits : GetVariantCodeStatus Signal
 *
 *******************************************************************
 */

void CDiagnosisFBlockShadowControl::GetVariantCodeStatus_emit()
{
//	MOST_LOG_SIGNAL << LOG_TAG_DIAG_CNT << " GetVariantCodeStatus_emit Signal got received " << MOST_LOG_ENDL;
// 2014.07.22, for log, hyunyong.sung
	MOST_LOG_SIGNAL << LOG_TAG_DIAG_CNT << " VariantCode_Get <-- " << MOST_LOG_ENDL;	

    emit GetVariantCodeStatus();
}

/*
 *******************************************************************
 * Function: MainSWVersionStatusIBOX
 *
 * Description : Emits the signal MainSWVersionStatus.
 *
 * Parameters :
 *      none
 *
 * Emits : MainSWVersionStatus Signal
 *
 *******************************************************************
 */

void CDiagnosisFBlockShadowControl::MainSWVersionStatus()
{
//    MOST_LOG_SIGNAL << LOG_TAG_DIAG_CNT << " MainSWVersionStatus Signal got received " << MOST_LOG_ENDL;

    const k2l::acs::protocols::most::string::IStringEncoding* stringEncoding = NULL;
    uint8_t* MainSWVersionString = NULL;
    uint32_t stringLength = 0;
    QStringList mainSWVersion;

    m_MainSWVersionIBOX->MainSWString(&stringEncoding,&MainSWVersionString,&stringLength);
    mainSWVersion.append(QString::fromUtf8((const char *)MainSWVersionString,stringLength));
    m_MainSWVersionIBOX->FCatString(&stringEncoding,&MainSWVersionString,&stringLength);
    mainSWVersion.append(QString::fromUtf8((const char *)MainSWVersionString,stringLength));
    m_MainSWVersionIBOX->CANDBString(&stringEncoding,&MainSWVersionString,&stringLength);
    mainSWVersion.append(QString::fromUtf8((const char *)MainSWVersionString,stringLength));
/*
    MOST_LOG_INFO << LOG_TAG_DIAG_CNT << " MainSWVersionStatus:SWVersion = " << mainSWVersion.at(0).toAscii().data() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DIAG_CNT << " MainSWVersionStatus:Fcat = " << mainSWVersion.at(1).toAscii().data() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DIAG_CNT << " MainSWVersionStatus:CANDB = " << mainSWVersion.at(2).toAscii().data() << MOST_LOG_ENDL;
*/
    MOST_LOG_SIGNAL << LOG_TAG_DIAG_CNT << " SWVersion_Status <-- : MainSW = " <<  mainSWVersion.at(0).toAscii().data()
	<< " Fcat = " << mainSWVersion.at(1).toAscii().data() << " CANDB = " << mainSWVersion.at(2).toAscii().data() << MOST_LOG_ENDL;

    emit MainSWVersionStatus(mainSWVersion);
}

/*
 *******************************************************************
 * Function: GetMainSWVersionIBOX
 *
 * Description : Function called by adaptor to get the MainSWVersion.
 *
 * Parameters :
 *      none
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CDiagnosisFBlockShadowControl::GetMainSWVersion()
{   
//    MOST_LOG_SIGNAL << LOG_TAG_DIAG_CNT << " GetMainSWVersion API got called " << MOST_LOG_ENDL;
// 2014.07.22, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_DIAG_CNT << " SWVersion_Get --> " << MOST_LOG_ENDL;

    return m_MainSWVersionIBOX->Get();
}

/*
 *******************************************************************
 * Function: VariantCode
 *
 * Description : Function called by the Diagnosis controller to set the VariantCode.
 *
 * Parameters :
 *      variantcode   rw    It holds the information about the VariantCode as QList<uint>
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CDiagnosisFBlockShadowControl::VariantCode(const QList<uint> &variantcode)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_DIAG_CNT << " VariantCode API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DIAG_CNT << " VariantCode:VehicleVariant = " << variantcode.at(0) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DIAG_CNT << " VariantCode:CountryVariant = " << variantcode.at(1) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DIAG_CNT << " VariantCode:SystemVariant = " << variantcode.at(2) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DIAG_CNT << " VariantCode:DVDRegion = " << variantcode.at(3) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DIAG_CNT << " VariantCode:DVDMenuLanguage = " << variantcode.at(4) << MOST_LOG_ENDL;
*/
// 2014.07.22, for log, hyunyong.sung
     MOST_LOG_SIGNAL << LOG_TAG_DIAG_CNT << " VariantCode_Status --> : Vehicle = " << variantcode.at(0)
	 << " Country = " << variantcode.at(1) << " System = " << variantcode.at(2) << " DVDRegion = " <<  variantcode.at(3)
	 << " DVDMenuLanguage = " << variantcode.at(4) << MOST_LOG_ENDL;	

    m_VariantCodeHU->VehicleVariantValue(acs::generated::fblock::Diagnosis::CVariantCode::TVehicleVariant (variantcode.at(0)));
    m_VariantCodeHU->CountryVariantValue(acs::generated::fblock::Diagnosis::CVariantCode::TCountryVariant (variantcode.at(1)));
    m_VariantCodeHU->SystemVariantValue(variantcode.at(2));
    m_VariantCodeHU->DVDRegionValue(acs::generated::fblock::Diagnosis::CVariantCode::TDVDRegion (variantcode.at(3)));
    m_VariantCodeHU->DVDMenuLanguageValue(variantcode.at(4));

    return m_VariantCodeHU->UpdateShadows();
}

bool CDiagnosisFBlockShadowControl::Register()
{
    bool result = false;

    MOST_LOG_INFO << LOG_TAG_DIAG_CNT << " Register API got called " << MOST_LOG_ENDL;

    if(!m_Registered)
    {
        result = m_AppContext->MOST()->AddShadow(m_DiagnosisShadow);
        result &= m_AppContext->MOST()->AddFunctionBlock(m_DiagnosisFBlock);

        /*
        result = m_AppContext->MOST()->AddFunctionBlock(m_DiagnosisHUFBlock);
        result &= m_AppContext->MOST()->AddShadow(m_DiagnosisIBOXShadow);
        result &= m_AppContext->MOST()->AddShadow(m_DiagnosisAMPShadow);
        */

        if(result)
        {
            MOST_LOG_INFO << LOG_TAG_DIAG_CNT << " Register FBlock successfully " << MOST_LOG_ENDL;
            m_Registered = true;
        }
        else
        {
            MOST_LOG_CRITICAL << LOG_TAG_DIAG_CNT << " Failed to Register FBlock " << MOST_LOG_ENDL;
        }
    }
    else
    {
        MOST_LOG_INFO << LOG_TAG_DIAG_CNT << " FBlock was already registered " << MOST_LOG_ENDL;
        // A case already registered
        result = true;
    }

    return result;
}

bool CDiagnosisFBlockShadowControl::UnRegister()
{
    bool result = false;

    MOST_LOG_INFO << LOG_TAG_DIAG_CNT << " UnRegister API got called " << MOST_LOG_ENDL;

    if(m_Registered)
    {
        result = m_AppContext->MOST()->RemoveShadow(m_DiagnosisShadow);
        result &= m_AppContext->MOST()->RemoveFunctionBlock(m_DiagnosisFBlock);

        /*
        result = m_AppContext->MOST()->RemoveFunctionBlock(m_DiagnosisHUFBlock);
        result &= m_AppContext->MOST()->RemoveShadow(m_DiagnosisIBOXShadow);
        result &= m_AppContext->MOST()->RemoveShadow(m_DiagnosisAMPShadow);
        */

        if(result)
        {
            MOST_LOG_INFO << LOG_TAG_DIAG_CNT << " UnRegister FBlock successfully " << MOST_LOG_ENDL;
            m_Registered = false;
        }
        else
        {
            MOST_LOG_CRITICAL << LOG_TAG_DIAG_CNT << " Failed to Unregister FBlock " << MOST_LOG_ENDL;
        }
    }
    else
    {
        MOST_LOG_INFO << LOG_TAG_DIAG_CNT << " FBlock was already Unregistered " << MOST_LOG_ENDL;
        result = true;
    }

    return result;
}

bool CDiagnosisFBlockShadowControl::FBlockVisible(const uint deviceID)
{
    /*
    bool result = false;

    if(deviceID == HU_DEVICE_ADDRESS)
    {
        result = GetIsRegistered();
    }
    else if(deviceID == IBOX_DEVICE_ADDRESS)
    {
        result = m_DiagnosisIBOXShadow->FunctionBlockVisible();
    }
    else if(deviceID == AMP_DEVICE_ADDRESS)
    {
        result = m_DiagnosisAMPShadow->FunctionBlockVisible();
    }

    return result;
    */
    return true;
}

CDiagnosisFBlockStateListener::CDiagnosisFBlockStateListener(acs::generated::shadow::Diagnosis::CDiagnosis *diagContext,
                                                             CDiagnosisFBlockShadowControl *context)
{
    m_DiagnosisShadow = diagContext;
    m_DiagnosisFBlockShadowControl = context;
}

void CDiagnosisFBlockStateListener::OnChange()
{
    if(m_DiagnosisShadow->FunctionBlockVisible())
    {
        m_DiagnosisFBlockShadowControl->GetMainSWVersion();
    }
}
