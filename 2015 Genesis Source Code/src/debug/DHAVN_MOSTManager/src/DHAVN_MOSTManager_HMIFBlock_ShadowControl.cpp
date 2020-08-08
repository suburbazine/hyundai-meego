/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Neelima Khedad                                   *
 * DATE       :  22 Aug 2011                                      *
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
 *                  HMI Fblock and Shadow control, responsible to *
 *                  create instance of Fblock(HMI 0x01) and Shadow*
 *                  (HMI 0x02) configure it and add to most stack.*
 *                  If any listeners will be part of this class.  *
 *                  Communicates with ACS daemon                  *
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT                  *
 ******************************************************************
 * 22 Aug 2011  Neelima Khedad           Draft version            *
 * 7  Sep 2011  Neelima Khedad           Updated for comments     *
 * 15 Sep 2011  Amit Koparde             Added Debug Statement    *
 * 16 Sep 2011  Neelima Khedad           Added new API SetNotiModeStatusHU
 * 19 Sep 2011  Neelima Khedad           Added new parameter to ModeStatus of CHMIAdaptor
 * 12 Oct 2011  Neelima Khedad           Added API SetSystemVisibilityHUError
 * 27 Oct 2011  Neelima Khedad           Updated API for to use popupsize parameter of struct SPopUpRequestData
 * 27 Oct 2011  Neelima Khedad           Adedd API SetDisplayControl
 * 26 Jan 2012  Neelima Khedad           Fixed Coverity Error.Initialized all structure varibles before using.
 *                                      		 Use of only one Return statement.
 * 08 Feb 2011  Neelima Khedad           Using the latest Version of LOG_INIT function Logger module
 * 19 Mar 2012  Shiva Kumar              Modified SetLCDBrightness method to support changed parametrs.
 * 05 May 2012  Shiva Kumar              Added new signal DisplayOSD in HMI Adaptor
 * 07 May 2012  Shiva Kumar              Added new signals ShowStatusBar and LockController
 * 06 Jun 2012  Shiva Kumar              Added new signal MenuStatus for FCAT 1.17
 * 21 Jun 2012  Shiva Kumar              Modified for FCAT 1.17.1, FCAT 1.17.2 and FCAT 1.17.3
 * 17 Jul 2012  Shiva Kumar              Modified for FCAT 1.18.2
 * 26 Jul 2012  Shiva Kumar              Modified for CR 12089, 12357
 * 10 Aug 2012  Shiva Kumar              Modified for CR 11699
 * 19 Sep 2012  Shiva Kumar              Modified for FCAT 1.19.0, 1.20.1, 1.20.2
 * 27 Nov 2012  Shiva Kumar              Modified for CR 16038
 * 07 Dec 2012  Shiva Kumar              Modified for FCAT 1.22.1
 * 08 Jan 2013  Hanhong Keum             Modified for FCAT 1.22.5
 * 26 Feb 2014 hyunyong.sung		Added new enum value of language for system boot. 
 ******************************************************************
 */

#include "DHAVN_MOSTManager_HMIFBlock_ShadowControl.h"
#include "DHAVN_AppSettings_Shared.h"

#define SETTINGS_FILE_DB    "/app/data/app_settings/SettingsScreenSettings.db"
#define SETTINGS_HM_DB      "settings_screen_app"

using namespace AppSettingsShared;

/*
 *******************************************************************
 * Function:CHMIFBlockShadowControl
 *
 * Description :This is constructor of CHMIFBlockShadowControl class.
 *              creates the instances for all the defined parameters
 * Parameters :
 *    context    rw   It holds the instance of CMarshaledACSApplication.
 *
 * Returns :
 *
 *******************************************************************
 */

CHMIFBlockShadowControl::CHMIFBlockShadowControl(CMarshaledACSApplication *context)
    : CControlThreadBase(context, LOG_TAG_HMI_CNT)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)    

    /** Head unit HMI */
    m_HMIFBlock = new acs::generated::fblock::HMI::CHMI(HMI_HEADUNIT);
    /** IBox HMI */
    m_HMIShadow = new acs::generated::shadow::HMI::CHMI(HMI_IBOX);

    /** HeadUint Listeners */
    m_ModeStatusHU = new acs::generated::fblock::HMI::CModeStatus(m_HMIFBlock);

    m_SystemAudibilityHU = new acs::generated::fblock::HMI::CSystemAudibility(m_HMIFBlock);

    m_SystemVisibilityHU = new acs::generated::fblock::HMI::CSystemVisibility(m_HMIFBlock);

    m_SystemLanguage = new acs::generated::fblock::HMI::CSystemLanguage(m_HMIFBlock);

    m_keyBoardHU = new acs::generated::fblock::HMI::CKeyBoard(m_HMIFBlock);
    m_KeyBoardListenerHU = new CKeyBoardListenerHU(this);

    /** HeadUnit handlers */
    m_PopUpRequestHU = new acs::generated::fblock::HMI::CPopUpRequest(m_HMIFBlock);
    m_PopUpRequestHandlerHU = new CPopUpRequestHandlerHU(this);

    /** Ibox Handlers */
    m_PopUpRequestIBOX = new acs::generated::shadow::HMI::CPopUpRequest(m_HMIShadow);
    m_PopUpRequestHandlerIBOX = new CPopUpRequestHandlerIBOX(this);

    /** Ibox Listeners */
    m_ModeSatusIBOX = new acs::generated::shadow::HMI::CModeStatus(m_HMIShadow);    
    m_ModeStatusListenerIBOX = new CModeStatusListenerIBOX(this);

    m_PopUpProgressBarIBOX = new acs::generated::shadow::HMI::CPopUpProgressBar(m_HMIShadow);
    m_PopUpProgressBarListenerIBOX = new CPopupProgressbarListenerIBOX(this);

    m_LCDBrightnessHU = new acs::generated::fblock::HMI::CLCDBrightness(m_HMIFBlock);
    m_LCDBrightnessIBOXListener = new CLCDBrightnessIBOXListener(this);

    m_DisplayOSD = new acs::generated::fblock::HMI::CDisplayOSD(m_HMIFBlock);
    m_DisplayOSDListener = new CDisplayOSDListener(this);

    m_ShowStatusBar = new acs::generated::fblock::HMI::CShowStatusBar(m_HMIFBlock);
    m_ShowStatusBarListener = new CShowStatusBarListener(this);

    m_Emergencyset = new acs::generated::fblock::HMI::CEmergencySet(m_HMIFBlock);
    m_EmergencySetListener = new CEmergencySetListener(this);

    m_KeyInputStatus = new acs::generated::shadow::HMI::CKeyInputStatus(m_HMIShadow);
    m_KeyInputStatusListener = new CKeyInputStatusListener(this);

    m_Beepsetting = new acs::generated::fblock::HMI::CBeepSetting(m_HMIFBlock);
    m_BeepSettingListener = new CBeepSettingListener(this);

    m_DrivingRestriction = new acs::generated::shadow::HMI::CDrivingRestriction(m_HMIShadow);
    m_DrivingRestrictionListener = new CDrivingRestrictionListener(this);

    m_AdjustScreenIBOX = new acs::generated::fblock::HMI::CAdjustScreen(m_HMIFBlock);
    m_AdjustScreenIBOXListener = new CAdjustScreenIBOXListener(this);

    m_IlluminationSettingIBOX = new acs::generated::fblock::HMI::CIlluminationSetting(m_HMIFBlock);
    m_IlluminationSettingIBOXListener = new CIlluminationSettingIBOXListener(this);

    m_VideoRatioIBOX = new acs::generated::fblock::HMI::CVideoRatio(m_HMIFBlock);
    m_VideoRatioIBOXListener = new CVideoRatioIBOXListener(this);

    m_DisplayControlIBOX = new acs::generated::fblock::HMI::CDisplayControl(m_HMIFBlock);
    m_DisplayControlIBOXListener = new CDisplayControlIBOXListener(this);

    m_SoundStatus = new acs::generated::fblock::HMI::CSoundStatus(m_HMIFBlock);

    m_VideoDefaultSetHU = new acs::generated::fblock::HMI::CVideoDefaultSet(m_HMIFBlock);
    m_VideoDefaultSetHandlerHU = new CVideoDefaultSetHandlerHU(this);

    m_VideoDefaultSetIBOX = new acs::generated::shadow::HMI::CVideoDefaultSet(m_HMIShadow);
    m_VideoDefaultSetHandlerIBOX = new CVideoDefaultSetHandlerIBOX(this);

    m_SystemTime = new acs::generated::fblock::HMI::CSystemTime(m_HMIFBlock);
    m_SystemDate = new acs::generated::fblock::HMI::CSystemDate(m_HMIFBlock);

    m_PhonePriority = new acs::generated::fblock::HMI::CPhonePriority(m_HMIFBlock);

    m_HMIFblockStateListener = new CHMIFblockStateListener(m_HMIShadow, this);

    // Fcat v13.07.1
    m_SystemDefaultSet = new acs::generated::shadow::HMI::CSystemDefaultSet(m_HMIShadow);
    m_SystemDefaultSetHandler = new CSystemDefaultSetHandler(this);

    // Fcat v13.07.2
    m_MuteContorl = new acs::generated::fblock::HMI::CMuteControl(m_HMIFBlock);
    m_MuteControlListener = new CMuteControlListener(this);
}

/*
 *******************************************************************
 * Function:~CHMIFBlockShadowControl
 *
 * Description :Is a Destructor,removes all the registered listeners.
 *              and destroys all the instances
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */

CHMIFBlockShadowControl ::~CHMIFBlockShadowControl()
{
    RemoveListeners();

    delete m_SystemLanguage;

    delete m_PopUpProgressBarListenerIBOX;
    delete m_PopUpProgressBarIBOX;

    delete m_ModeStatusListenerIBOX;
    delete m_ModeSatusIBOX;

    delete m_PopUpRequestHandlerIBOX;
    delete m_PopUpRequestIBOX;

    delete m_PopUpRequestHandlerHU;
    delete m_PopUpRequestHU;

    delete m_KeyBoardListenerHU;
    delete m_keyBoardHU;

    delete m_SystemVisibilityHU;

    delete m_SystemAudibilityHU;

    delete m_ModeStatusHU;

    delete m_HMIShadow;
    delete m_HMIFBlock;

    delete m_LCDBrightnessHU;
    delete m_LCDBrightnessIBOXListener;

    delete m_DisplayOSD;
    delete m_DisplayOSDListener;

    delete m_ShowStatusBar;
    delete m_ShowStatusBarListener;

    delete m_Emergencyset;
    delete m_EmergencySetListener;

    delete m_KeyInputStatus;
    delete m_KeyInputStatusListener;

    delete m_Beepsetting;
    delete m_BeepSettingListener;

    delete m_DrivingRestriction;
    delete m_DrivingRestrictionListener;

    delete m_AdjustScreenIBOX;
    delete m_AdjustScreenIBOXListener;

    delete m_IlluminationSettingIBOX;
    delete m_IlluminationSettingIBOXListener;

    delete m_VideoRatioIBOX;
    delete m_VideoRatioIBOXListener;

    delete m_DisplayControlIBOX;
    delete m_DisplayControlIBOXListener;

    delete m_SoundStatus;

    delete m_VideoDefaultSetHU;
    delete m_VideoDefaultSetHandlerHU;

    delete m_VideoDefaultSetIBOX;
    delete m_VideoDefaultSetHandlerIBOX;

    delete m_SystemTime;
    delete m_SystemDate;

    delete m_PhonePriority;

    delete m_HMIFblockStateListener;

    // Fcat v13.07.1
    delete m_SystemDefaultSet;
    delete m_SystemDefaultSetHandler;

    // Fcat v13.07.2
    delete m_MuteContorl;
    delete m_MuteControlListener;
}

/*
 *******************************************************************
 * Function:Initialize
 *
 * Description :Function initializes the MOST elements of HMI Fblock and
 *             Shadow and adds to MOST stack.
 *
 * Parameters :
 *
 *
 * Returns :true or false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::Initialize()
{
    /** Initialze notification for all parameters on FBlock and Shadow */
    if(m_HMIFBlock->InitializeMOSTElement() && m_HMIShadow->InitializeMOSTElement())
    {     
        SetListeners();

        /** Set the handlers for PopUpRequest */
        m_PopUpRequestHU->SetHandler(m_PopUpRequestHandlerHU);
        m_PopUpRequestIBOX->SetHandler(m_PopUpRequestHandlerIBOX);
        m_VideoDefaultSetHU->SetHandler(m_VideoDefaultSetHandlerHU);
        m_VideoDefaultSetIBOX->SetHandler(m_VideoDefaultSetHandlerIBOX);


        /** Initializing ModeStatus Value */
        m_ModeStatusHU->Value(0,acs::generated::fblock::HMI::CModeStatus::DataHome);
        m_ModeStatusHU->Value(1,acs::generated::fblock::HMI::CModeStatus::DataHome);
        m_ModeStatusHU->Value(2,acs::generated::fblock::HMI::CModeStatus::DataInvalid);
        m_ModeStatusHU->Size(3);
        m_ModeStatusHU->FunctionInstance().UpdateShadows(0);

        /** Initializing SystemVisibility Value*/
        m_SystemVisibilityHU->Value(0,acs::generated::fblock::HMI::CSystemVisibility::DataInvalid);
        m_SystemVisibilityHU->Value(1,acs::generated::fblock::HMI::CSystemVisibility::DataInvalid);
        m_SystemVisibilityHU->Value(2,acs::generated::fblock::HMI::CSystemVisibility::DataInvalid);
        m_SystemVisibilityHU->Size(3);
        m_SystemVisibilityHU->FunctionInstance().UpdateShadows(0);

        /** Initializing Systemaudibility Values*/
        m_SystemAudibilityHU->Value(0,acs::generated::fblock::HMI::CSystemAudibility::DataHeadUnit);
        m_SystemAudibilityHU->Value(1,acs::generated::fblock::HMI::CSystemAudibility::DataHeadUnit);
        m_SystemAudibilityHU->Value(2,acs::generated::fblock::HMI::CSystemAudibility::DataInvalid);
        m_SystemAudibilityHU->Size(3);
        m_SystemAudibilityHU->FunctionInstance().UpdateShadows(0);

        /** Initializing BGSoundStatus Value */
        m_SoundStatus->BGSoundValue(acs::generated::fblock::HMI::CSoundStatus::BGSoundInvalid);
        m_SoundStatus->UpdateShadows();

        /** Initializing ShowStatusBar value to invalid */
        m_ShowStatusBar->StatusBarValue((acs::generated::fblock::HMI::CShowStatusBar::TStatusBar)0xFF);
        m_ShowStatusBar->UpdateShadows();

        /** Initializing EmergencySet value to invalid */
        m_Emergencyset->EMGCStateValue((acs::generated::fblock::HMI::CEmergencySet::TEMGCState)0xFF);
        m_Emergencyset->ControllerLockValue((acs::generated::fblock::HMI::CEmergencySet::TControllerLock)0xFF);
        m_Emergencyset->SoundLockValue((acs::generated::fblock::HMI::CEmergencySet::TSoundLock)0xFF);
        m_Emergencyset->UpdateShadows();

        /** Initializing SystemVolumeStatus to invalid */        
        SetVolumeStatus(0xFF, 0, 0);

        /** opening DB session for reading Setting values */
        int result_open_dbfile;
        PMHANDLE_T *pDBSession = NULL;

        result_open_dbfile = SIPMOpen( &pDBSession, SI_PM_SQLITE, SETTINGS_FILE_DB, SETTINGS_HM_DB, 0 , 0 );

        if( SI_PM_OK != result_open_dbfile )
        {
            MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to open DB file" << MOST_LOG_ENDL;
        }
        else if( NULL != pDBSession )
        {
            int result = SI_PM_OK;
            bool ret = false;
            int data = 0;
            int dataSize = 0;

            /** Getting the language Settings value from DB */
            result = SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_LANGUAGE_TYPE].keyString, (uchar *)&data, &dataSize );

            //if( (result == SI_PM_OK ) || ( data != -101 ))
            if( result == SI_PM_OK )
            {
                if((data == 1)||(data == 7))	// EN, EN_US
                {
                    ret = SetLanguage("EN1");
                }
                else if(data == 2)
                {
                    ret = SetLanguage("KO0");
                }
                else if((data == 3)||(data == 4))	//ZH_YUE, ZN_CN
                {
                    ret = SetLanguage("ZH1");
                }
                else if(data == 5)
                {
                    ret = SetLanguage("PT0");
                }  
                else if(data == 6)
                {
                    ret = SetLanguage("EN2");
                }  
                else if(data == 8)
                {
                    ret = SetLanguage("FR0");
                }  
                else if(data == 9)
                {
                    ret = SetLanguage("IT0");
                }  
                else if(data == 10)
                {
                    ret = SetLanguage("DE0");
                }  
                else if(data == 11)
                {
                    ret = SetLanguage("ES0");
                }  
                else if(data == 12)
                {
                    ret = SetLanguage("RU0");
                }  
                else if(data == 13)
                {
                    ret = SetLanguage("NL0");
                }  
                else if(data == 14)
                {
                    ret = SetLanguage("SV0");
                }  
                else if(data == 15)
                {
                    ret = SetLanguage("PL0");
                }                  
                else if(data == 16)
                {
                    ret = SetLanguage("TR0");
                }                  
                else if(data == 17)
                {
                    ret = SetLanguage("CS0");
                }                  
                else if(data == 18)
                {
                    ret = SetLanguage("DA0");
                }                  
                else if(data == 19)
                {
                    ret = SetLanguage("SK0");
                }                  
                else if(data == 20)
                {
                    ret = SetLanguage("AR0");
                }                  
                else if(data == 21)
                {
                    ret = SetLanguage("FR0");
                }                  
                else if(data == 22)
                {
                    ret = SetLanguage("ES0");
                }                  
                else
                {
                    MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Invalid value, Set default EN1" << MOST_LOG_ENDL;
                    ret = SetLanguage("EN1");
                }
                MOST_LOG_INFO << LOG_TAG_HMI_CNT << " Reading DB Values for language = " << data << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_HMI_CNT << " Data size = " << dataSize << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_HMI_CNT << " Language setting = " << ret << MOST_LOG_ENDL;
            }
            else
            {
                /** Setting default laungauge to IBOX */
                MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to read DB for Language setting, Set default EN1" << MOST_LOG_ENDL;

                ret = SetLanguage("EN1");                
            }

            /** Getting the TimeFormat value from DB */
            result = SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_TIME_TYPE].keyString, (uchar *)&data, &dataSize );

            //if( (result == SI_PM_OK ) || (data != -101) )
            if( result == SI_PM_OK )
            {
                if(data < 2)
                {
                    /** Set the received parameters to the property */
                    m_SystemTime->TimeTypeValue((acs::generated::fblock::HMI::CSystemTime::TTimeType)data);
                }
                else
                {
                    MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Invalid value, Set default EN1" << MOST_LOG_ENDL;
                    m_SystemTime->TimeTypeValue(acs::generated::fblock::HMI::CSystemTime::TimeTypeInvalid);
                }

                MOST_LOG_INFO << LOG_TAG_HMI_CNT << " Reading DB Values for time type = " << data << MOST_LOG_ENDL;
            }
            else
            {
                MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to Read DB for time format, Set default Invalid" << MOST_LOG_ENDL;

                m_SystemTime->TimeTypeValue(acs::generated::fblock::HMI::CSystemTime::TimeTypeInvalid);                               
            }

            result = SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_AUTO_TIMEZONE].keyString, (uchar *)&data, &dataSize );

            //if( (result == SI_PM_OK ) || (data != -101) )
            if( result == SI_PM_OK )
            {
                m_SystemTime->TimezoneValue((ushort)data);

                MOST_LOG_INFO << LOG_TAG_HMI_CNT << " Reading DB Values for time zone = " << data << MOST_LOG_ENDL;
            }
            else
            {
                MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to Read DB for time zone" << MOST_LOG_ENDL;
            }

            result = SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_SUMMER_TIME].keyString, (uchar *)&data, &dataSize );

            //if( (result == SI_PM_OK ) || (data != -101) )
            if( result == SI_PM_OK )
            {
                m_SystemTime->DSTValue((uint)data);

                MOST_LOG_INFO << LOG_TAG_HMI_CNT << " Reading DB Values for summer time = " << data << MOST_LOG_ENDL;
            }
            else
            {
                MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to Read DB for summer time" << MOST_LOG_ENDL;
            }

            /** Setting system time */
            QTime time = QTime::currentTime();
            m_SystemTime->CurrentHourValue((uchar)time.hour());
            m_SystemTime->CurrentMinValue((uchar)time.minute());
            m_SystemTime->CurrentSecValue((uchar)time.second());

            m_SystemTime->UpdateShadows();


            /** Getting the DateFormat value from DB */
            result = SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_DATEFORMAT_TYPE].keyString, (uchar *)&data, &dataSize );

            //if( (result == SI_PM_OK ) || (data != -101) )
            if( result == SI_PM_OK )
            {
                m_SystemDate->DateTypeValue((acs::generated::fblock::HMI::CSystemDate::TDateType)data);
            }
            else
            {
                MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to Read DB for date format, Set default Invalid" << MOST_LOG_ENDL;

                m_SystemDate->DateTypeValue(acs::generated::fblock::HMI::CSystemDate::DateTypeInvalid);                
            }

            QDate curDate = QDate::currentDate();
            m_SystemDate->CurrentYearValue((uint16_t)curDate.year());
            m_SystemDate->CurrentMonthValue((uint8_t)curDate.month());
            m_SystemDate->CurrentDayValue((uint8_t)curDate.day());
            m_SystemDate->UpdateShadows();


            /** Reading distance unit value from setting DB */
            result = SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_DISTANCE_UNIT].keyString, (uchar *)&data, &dataSize );

            //if( ( result == SI_PM_OK ) || ( data != -101 ) )
            if( result == SI_PM_OK )
            {
                ret = SetDistanceUnit(data);
                MOST_LOG_INFO << LOG_TAG_HMI_CNT << " Distance Unit = " << ret << MOST_LOG_ENDL;
            }
            else
            {
                /** Setting invalid value for Distance unit*/
                MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to Read DB for setting Distance Unit, Set default Invalid" << MOST_LOG_ENDL;

                ret = SetDistanceUnit(acs::generated::fblock::HMI::CDistanceUnit::DistanceUnitInvalid);
            }


            /** Initializing Screen Setting value from Setting DB */
            int lBrightness = 0;
            int lSaturation = 0;
            int lContrast = 0;
            int lHue = 0;

            /** Reading the brightness value from Setting DB */
            result = SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_VIDEO_BRIGHTNESS].keyString, (uchar *)&lBrightness, &dataSize );
            /** Reading the Saturation value from Setting DB */
            result &= SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_VIDEO_SATURATION].keyString, (uchar *)&lSaturation, &dataSize );
            /** Reading the Hue value from Setting DB */
            result &= SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_VIDEO_HUE].keyString, (uchar *)&lHue, &dataSize );
            /** Reading the contrast value from Setting DB */
            result &= SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_VIDEO_CONTRAST].keyString, (uchar *)&lContrast, &dataSize );

            //if( (result == SI_PM_OK) || (lBrightness != -101) || (lSaturation != -101) || (lContrast != -101) || (lHue != -101) )
            if( result == SI_PM_OK )
            {
                ret = SetScreen(lBrightness, lSaturation, lContrast, lHue);
                MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetScreen = " << ret << MOST_LOG_ENDL;
            }
            else
            {
                /** Setting default values for Adjust Screen */
                MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to Read DB for Screen setting, Set default" << MOST_LOG_ENDL;

                ret = SetVideoDefaulttoIBOX(0x0001, 0x01);                
            }

            /** Initializing LCDBrightness Setting value from Setting DB */
            int lTarget = 0;
            int lcdFrontBrightness = 0;
            int lcdRear1Brightness = 0;
            int lcdRear2Brightness = 0;

            /** Reading LCDBrightness for Front Display from Setting DB */
            result = SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_FRONT_SCREENBRIGHTNESS].keyString, (uchar *)&lcdFrontBrightness, &dataSize );
            /** Reading LCDBrightness for Rear Display from Setting DB */
            result &= SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_REAR_SCREENBRIGHTNESS].keyString, (uchar *)&lcdRear1Brightness, &dataSize );

            //if( (result == SI_PM_OK ) || (lcdFrontBrightness != -101) || (lcdRear1Brightness != -101) )
            if( result == SI_PM_OK )
            {
                ret = SetLCDBrightness(lTarget, lcdFrontBrightness, lcdRear1Brightness, lcdRear2Brightness);
                MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetLCDBrightness : " << ret << MOST_LOG_ENDL;
            }
            else
            {
                /** Setting default value for LCD Brightness */
                MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to Read DB for LCDBrightness, Set default" << MOST_LOG_ENDL;

                ret = SetVideoDefaulttoIBOX(0x0001, 0x02);                
            }


            /** Reading Illumination value from Setting DB */
            result = SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_EXPOSURE].keyString, (uchar *)&data, &dataSize );

            //if( (result == SI_PM_OK) || (data != -101) )
            if( result == SI_PM_OK )
            {
                ret = SetIllumination(data);
                MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetIllumination : " << ret << MOST_LOG_ENDL;
            }
            else
            {
                /** Setting default value for illumination */
                MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to read DB for Illumination Setting, Set default" << MOST_LOG_ENDL;

                ret = SetVideoDefaulttoIBOX(0x0001, 0x03);
            }


            /** Reading VideoRation value from Setting DB */
            result = SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_ASPECT_RADIO].keyString, (uchar *)&data, &dataSize );

            //if( (result == SI_PM_OK) || (data != -101) )
            if( result == SI_PM_OK )
            {
                ret = SetVideoRatio(data);
                MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetVideoRatio = " << ret << MOST_LOG_ENDL;
            }
            else
            {
                /** Setting default value for videoRatio */
                MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to read DB for videoRatio Setting, Set default" << MOST_LOG_ENDL;

                ret = SetVideoDefaulttoIBOX(0x0001, 0x04);
            }


            /** Reading BeepSetting value from Setting DB */
            result = SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_SOUND_BEEP].keyString, (uchar *)&data, &dataSize );

            //if( (result == SI_PM_OK) || (data != -101 ) )
            if( result == SI_PM_OK )
            {
                ret = SetBeepSetting(data);
                MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetBeepSetting = " << ret << MOST_LOG_ENDL;
            }
            else
            {
                /** Setting invalid value for BeepSetting */
                MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to read DB for beep Setting, Set default Invalid" << MOST_LOG_ENDL;

                ret = SetBeepSetting(acs::generated::fblock::HMI::CBeepSetting::BeepSettingInvaild);
            }

            /** Initializing keyboard setting value from setting DB */
            uint koreakeypad = 0;
            uint english_US = 0;
            uint english_EU = 0;
            uint chinese = 0;
            uint activeKeyboard = 0;
            uint region = 0;

            result = SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_ENGLISH_KEYPAD].keyString, (uchar *)&english_US, &dataSize );
            result &= SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_KOREAN_KEYPAD].keyString, (uchar *)&koreakeypad, &dataSize );
            result &= SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_CHINA_KEYPAD].keyString, (uchar *)&chinese, &dataSize );
            result &= SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_EUROPE_KEYPAD].keyString, (uchar *)&english_EU, &dataSize );
            result &= SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_CURRENT_REGION].keyString, (uchar *)&region, &dataSize );

            if( result == SI_PM_OK )
            {
                switch(region)
                {
                    case -1:
                    case 3:
                    case 1:
                        activeKeyboard = 0x03;
                        break;
                    case 2:
                        activeKeyboard = 0x0B;
                        break;
                    case 4:
                        activeKeyboard = 0x13;
                        break;
                    case 5:
                        activeKeyboard = 0x05;
                        break;
                    default:
                        activeKeyboard = 0x03;
                        break;
                }

                QList<uint> keyboardList;
                keyboardList.append(koreakeypad); // At 0 position Korean keyboard value.
                keyboardList.append(english_US);//At 1 position English US keyboard value.
                keyboardList.append(english_EU); //At 2 position English EU keyboard value.
                keyboardList.append(chinese); //At 3 position Chinese keyboard value.
                keyboardList.append(activeKeyboard); //At 4 position  keyboard  setting value.

                QVariant keyBoarddataSizeVar;
                QDBusVariant dBuskeyBoarddataSizeVar;
                QDBusArgument keyBoarddataSizeArg;
                keyBoarddataSizeArg << keyboardList;
                keyBoarddataSizeVar.setValue(keyBoarddataSizeArg);
                dBuskeyBoarddataSizeVar.setVariant(keyBoarddataSizeVar);

                ret = SetKeyBoard(dBuskeyBoarddataSizeVar);
                MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetKeyBoard : " << ret << MOST_LOG_ENDL;
            }
            else
            {
                MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to read DB for keyboard Setting " << MOST_LOG_ENDL;
            }


            /** Reading DisplayStatus from Setting DB */
            result = SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_REAR_ON].keyString, (uchar *)&data, &dataSize );

            //if( (result == SI_PM_OK ) || (data != -101) )
            if( result == SI_PM_OK )
            {
                ret = SetDisplayControl(data);
                MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetDisplayControl = " << ret << MOST_LOG_ENDL;
            }
            else
            {
                /** setting default value for display on/off */
                MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to read DB for displayControl Setting, Set default" << MOST_LOG_ENDL;

                ret = SetVideoDefaulttoIBOX(0x0001, 0x05);                
            }


            /** Reading Phone priority from Setting DB */
            result = SIPMGet( pDBSession, SETTINGS_DB_VARIABLES_KEYS[AppSettingsDef::DB_KEY_VR_PHONE_PRIORITY].keyString, (uchar *)&data, &dataSize );

            //if( (result == SI_PM_OK ) || (data != -101) )
            if( result == SI_PM_OK )
            {
                ret = SetPhonePriority(data);
                MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetPhonePriority = " << ret << MOST_LOG_ENDL;
            }
            else
            {
                /** setting default value for phone priority */
                MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to read DB for PhonePriority Setting, Set default Invalid" << MOST_LOG_ENDL;

                ret = SetPhonePriority(acs::generated::fblock::HMI::CPhonePriority::PhonePriorityInvalid);
            }            
        }

        if( SI_PM_OK == result_open_dbfile )
        {
            SIPMClose( pDBSession );
        }

        m_Initialized = true;
    }    

    return m_Initialized;

}

bool CHMIFBlockShadowControl::Register()
{
    bool result = false;

    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " Register API got called " << MOST_LOG_ENDL;

    //if(m_Initialized)
    {
        if(!m_Registered)
        {
            result = m_AppContext->MOST()->AddFunctionBlock(m_HMIFBlock);
            result &= m_AppContext->MOST()->AddShadow(m_HMIShadow);

            if(result)
            {
                MOST_LOG_INFO << LOG_TAG_HMI_CNT << " Register FBlock successfully " << MOST_LOG_ENDL;
                m_Registered = true;
            }
            else
            {
                MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to register FBlock " << MOST_LOG_ENDL;
            }
        }
        else
        {
            // A case already registered
            MOST_LOG_INFO << LOG_TAG_HMI_CNT << " FBlock was already registered " << MOST_LOG_ENDL;
            result = true;
        }
    }

    return result;
}

bool CHMIFBlockShadowControl::UnRegister()
{
    bool result = false;

    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " UnRegister API got called " << MOST_LOG_ENDL;

    if(m_Registered)
    {
        result = m_AppContext->MOST()->RemoveFunctionBlock(m_HMIFBlock);
        result &= m_AppContext->MOST()->RemoveShadow(m_HMIShadow);

        if(result)
        {
            MOST_LOG_INFO << LOG_TAG_HMI_CNT << " UnRegister FBlock successfully " << MOST_LOG_ENDL;
            m_Registered = false;
        }
        else
        {
            MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Failed to Unregister FBlock " << MOST_LOG_ENDL;
        }
    }
    else
    {
        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " FBlock was already Unregistered " << MOST_LOG_ENDL;
        result = true;
    }

    return result;
}


bool CHMIFBlockShadowControl::FBlockVisible(const uint deviceID)
{
    bool result = false;

    if(deviceID == HU_DEVICE_ADDRESS)
    {
        result = GetIsRegistered();
    }
    else if(deviceID == IBOX_DEVICE_ADDRESS)
    {
        result = m_HMIShadow->FunctionBlockVisible();
    }

    return result;
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

void CHMIFBlockShadowControl::SetListeners()
{
    /** Setting the listeners for Properties on Head Unit */

    m_keyBoardHU->SetListener(m_KeyBoardListenerHU);

    /** Setting the listeners for Properties on Ibox */
    m_ModeSatusIBOX->SetListener(m_ModeStatusListenerIBOX);   
    m_PopUpProgressBarIBOX->SetListener(m_PopUpProgressBarListenerIBOX);
    m_DisplayOSD->SetListener(m_DisplayOSDListener);
    m_ShowStatusBar->SetListener(m_ShowStatusBarListener);
    m_Emergencyset->SetListener(m_EmergencySetListener);
    m_KeyInputStatus->SetListener(m_KeyInputStatusListener);
    m_DrivingRestriction->SetListener(m_DrivingRestrictionListener);
    m_LCDBrightnessHU->SetListener(m_LCDBrightnessIBOXListener);
    m_AdjustScreenIBOX->SetListener(m_AdjustScreenIBOXListener);
    m_IlluminationSettingIBOX->SetListener(m_IlluminationSettingIBOXListener);
    m_VideoRatioIBOX->SetListener(m_VideoRatioIBOXListener);
    m_DisplayControlIBOX->SetListener(m_DisplayControlIBOXListener);
    m_Beepsetting->SetListener(m_BeepSettingListener);

    m_HMIShadow->SetListener(m_HMIFblockStateListener);

    // Fcat v13.10.2
    m_MuteContorl->SetListener(m_MuteControlListener);
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

void CHMIFBlockShadowControl::RemoveListeners()
{
    /** Removing the listeners for Properties on Head Unit */

    m_keyBoardHU->RemoveListener(m_KeyBoardListenerHU);

    /** Removing the listeners for Properties on Ibox*/
    m_ModeSatusIBOX->RemoveListener(m_ModeStatusListenerIBOX);  
    m_PopUpProgressBarIBOX->RemoveListener(m_PopUpProgressBarListenerIBOX);
    m_DisplayOSD->RemoveListener(m_DisplayOSDListener);
    m_ShowStatusBar->RemoveListener(m_ShowStatusBarListener);
    m_Emergencyset->RemoveListener(m_EmergencySetListener);
    m_KeyInputStatus->RemoveListener(m_KeyInputStatusListener);
    m_DrivingRestriction->RemoveListener(m_DrivingRestrictionListener);
    m_LCDBrightnessHU->RemoveListener(m_LCDBrightnessIBOXListener);
    m_AdjustScreenIBOX->RemoveListener(m_AdjustScreenIBOXListener);
    m_IlluminationSettingIBOX->RemoveListener(m_IlluminationSettingIBOXListener);
    m_VideoRatioIBOX->RemoveListener(m_VideoRatioIBOXListener);
    m_DisplayControlIBOX->RemoveListener(m_DisplayControlIBOXListener);
    m_Beepsetting->RemoveListener(m_BeepSettingListener);

    m_HMIShadow->RemoveListener(m_HMIFblockStateListener);

    // Fcat v13.10.2
    m_MuteContorl->RemoveListener(m_MuteControlListener);
}

/*
 *******************************************************************
 * Function:CModeStatusListenerIBOX
 *
 * Description :Constructor for CModeStatusListenerIBOX.
 *              Stores the context of CHMIFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

CModeStatusListenerIBOX::CModeStatusListenerIBOX(CHMIFBlockShadowControl *context)
{
    m_HMIFBlockShadowControl = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property ModeStatus
 *               changed by Ibox
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CModeStatusListenerIBOX::OnChange()
{   
    m_HMIFBlockShadowControl->ModeStatusIBOX();
}
/*
 *******************************************************************
 * Function:OnError
 *
 * Description : Method invoked by ACS Daemon when property ModeStatus
 *               error is received from Ibox
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CModeStatusListenerIBOX::OnError()
{
    m_HMIFBlockShadowControl->ModeStatusErrorIBOX();
}

/*
 *******************************************************************
 * Function:CPopupProgressbarListenerIBOX
 *
 * Description :Constructor for CPopupProgressbarListenerIBOX.
 *              Stores the context of CHMIFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

CPopupProgressbarListenerIBOX::CPopupProgressbarListenerIBOX(CHMIFBlockShadowControl *context)
{
    m_HMIFBlockShadowControl = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property PopupProgressbar
 *               is changed by Ibox
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CPopupProgressbarListenerIBOX::OnChange()
{    
    m_HMIFBlockShadowControl->Progressbar();
}

/*
 *******************************************************************
 * Function:CKeyBoardListenerHU
 *
 * Description :Constructor for CKeyBoardListenerHU.
 *              Stores the context of CHMIFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

CKeyBoardListenerHU::CKeyBoardListenerHU(CHMIFBlockShadowControl *context)
{
    m_HMIFBlockShadowControl = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property Visibility
 *               of HU is Set by Ibox
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CKeyBoardListenerHU::OnChange()
{
    m_HMIFBlockShadowControl->KeyBoardSetting();
}
/*
 *******************************************************************
 * Function:CDisplayOSDListener
 *
 * Description :Constructor for CDisplayOSDListener.
 *              Stores the context of CHMIFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
CDisplayOSDListener::CDisplayOSDListener(CHMIFBlockShadowControl *context)
{
    m_HMIFBockShadowControl = context;
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property DisplayOSD
 *               of HU is Set by Ibox
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CDisplayOSDListener::OnChange()
{
    m_HMIFBockShadowControl->DisplayOSDListener();
}
/*
 *******************************************************************
 * Function:CShowStatusBarListener
 *
 * Description :Constructor for CShowStatusBarListener.
 *              Stores the context of CHMIFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
CShowStatusBarListener::CShowStatusBarListener(CHMIFBlockShadowControl* context)
{
    m_HMIFBockShadowControl = context;
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property ShowStatusBar
 *               of HU is Set by Ibox
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CShowStatusBarListener::OnChange()
{
    m_HMIFBockShadowControl->ShowStatusBarSlot();
}
/*
 *******************************************************************
 * Function:CLockControllerListener
 *
 * Description :Constructor for CLockControllerListener.
 *              Stores the context of CHMIFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
CEmergencySetListener::CEmergencySetListener(CHMIFBlockShadowControl* context)
{
    m_HMIFBockShadowControl = context;
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property LockController
 *               of HU is Set by Ibox
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CEmergencySetListener::OnChange()
{
    m_HMIFBockShadowControl->EmergencySetSlot();
}
/*
 *******************************************************************
 * Function:CMenuStatusListener
 *
 * Description :Constructor for CMenuStatusListener.
 *              Stores the context of CHMIFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
CKeyInputStatusListener::CKeyInputStatusListener(CHMIFBlockShadowControl *context)
{
    m_HMIFBockShadowControl = context;
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property MenuStatus
 *               of HU is Set by Ibox
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CKeyInputStatusListener::OnChange()
{
    m_HMIFBockShadowControl->KeyInputStatusSlot();
}
/*
 *******************************************************************
 * Function:CDrivingRestrictionListener
 *
 * Description :Constructor for CDrivingRestrictionListener.
 *              Stores the context of CHMIFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
CDrivingRestrictionListener::CDrivingRestrictionListener(CHMIFBlockShadowControl* context)
{
    m_HMIFBockShadowControl = context;
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon for property DrivingRestriction
 *               status of Ibox
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CDrivingRestrictionListener::OnChange()
{
    m_HMIFBockShadowControl->DrivingRestrictionStatusSlot();
}
/*
 *******************************************************************
 * Function:CLCDBrightnessIBOXListener
 *
 * Description :Constructor for CLCDBrightnessIBOXListener.
 *              Stores the context of CHMIFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
CLCDBrightnessIBOXListener::CLCDBrightnessIBOXListener(CHMIFBlockShadowControl* context)
{
    m_HMIFBockShadowControl = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon for property LCDBrightness
 *               Set from Ibox
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CLCDBrightnessIBOXListener::OnChange()
{
    m_HMIFBockShadowControl->LCDBrightnessFromIBOXSlot();
}
/*
 *******************************************************************
 * Function:CAdjustScreenIBOXListener
 *
 * Description :Constructor for CAdjustScreenIBOXListener.
 *              Stores the context of CHMIFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
CAdjustScreenIBOXListener::CAdjustScreenIBOXListener(CHMIFBlockShadowControl* context)
{
    m_HMIFBockShadowControl = context;
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon for property AdjustScreen
 *               Set from Ibox
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CAdjustScreenIBOXListener::OnChange()
{
    m_HMIFBockShadowControl->AdjustScreenFromIBOXSlot();
}
/*
 *******************************************************************
 * Function:CIlluminationSettingIBOXListener
 *
 * Description :Constructor for CIlluminationSettingIBOXListener.
 *              Stores the context of CHMIFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
CIlluminationSettingIBOXListener::CIlluminationSettingIBOXListener(CHMIFBlockShadowControl* context)
{
    m_HMIFBockShadowControl = context;
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon for property Illumination
 *               Set from Ibox
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CIlluminationSettingIBOXListener::OnChange()
{
    m_HMIFBockShadowControl->IlluminationFromIBOXSlot();
}
/*
 *******************************************************************
 * Function:CVideoRatioIBOXListener
 *
 * Description :Constructor for CVideoRatioIBOXListener.
 *              Stores the context of CHMIFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
CVideoRatioIBOXListener::CVideoRatioIBOXListener(CHMIFBlockShadowControl* context)
{
    m_HMIFBockShadowControl = context;
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon for property VideoRatio
 *               Set from Ibox
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CVideoRatioIBOXListener::OnChange()
{
    m_HMIFBockShadowControl->VideoRatioFromIBOXSlot();
}
/*
 *******************************************************************
 * Function:CDisplayControlIBOXListener
 *
 * Description :Constructor for CDisplayControlIBOXListener.
 *              Stores the context of CHMIFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
CDisplayControlIBOXListener::CDisplayControlIBOXListener(CHMIFBlockShadowControl* context)
{
    m_HMIFBockShadowControl = context;
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon for property DisplayControl
 *               Set from Ibox
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CDisplayControlIBOXListener::OnChange()
{
    m_HMIFBockShadowControl->DisplayControlFromIBOXSlot();
}
/*
 *******************************************************************
 * Function:CBeepSettingListener
 *
 * Description :Constructor for CBeepSettingListener.
 *              Stores the context of CHMIFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
CBeepSettingListener::CBeepSettingListener(CHMIFBlockShadowControl* context)
{
    m_HMIFBockShadowControl = context;
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon for property BeepSetting
 *               Set from Ibox
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CBeepSettingListener::OnChange()
{
    m_HMIFBockShadowControl->BeepSettingFromIBOXSlot();
}

/*
 *******************************************************************
 * Function:CPopUpRequestHandlerIBOX
 *
 * Description :Constructor for CPopUpRequestHandlerIBOX.
 *              Stores the context of CHMIFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

CPopUpRequestHandlerIBOX::CPopUpRequestHandlerIBOX(CHMIFBlockShadowControl *context)
{
    m_HMIFBlockShadowControl = context;
}

/*
 *******************************************************************
 * Function:Error
 *
 * Description :Callback for received Error Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CPopUpRequestHandlerIBOX::Error(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_HMIFBlockShadowControl->PopUpRequestHandlerIBOX(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:ErrorAck
 *
 * Description :Callback for received ErrorAck Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CPopUpRequestHandlerIBOX::ErrorAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_HMIFBlockShadowControl->PopUpRequestHandlerIBOX(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:Interface
 *
 * Description :Callback for received Interface Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CPopUpRequestHandlerIBOX::Interface(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_HMIFBlockShadowControl->PopUpRequestHandlerIBOX(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:Processing
 *
 * Description :Callback for received Processing Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CPopUpRequestHandlerIBOX::Processing(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_HMIFBlockShadowControl->PopUpRequestHandlerIBOX(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck Operation.
 *              Updates the Processing acknowledgement to HU Controller
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CPopUpRequestHandlerIBOX::ProcessingAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{ 
    m_HMIFBlockShadowControl->PopUpRequestHandlerIBOX(sourceDeviceID,payloadLength,payload,STATUS_PROCESSINGACK);
}

/*
 *******************************************************************
 * Function:Result
 *
 * Description :Callback for received Result Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CPopUpRequestHandlerIBOX::Result(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_HMIFBlockShadowControl->PopUpRequestHandlerIBOX(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck Operation
 *              Updates the Result acknowledgement to HU Controller
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CPopUpRequestHandlerIBOX::ResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
     m_HMIFBlockShadowControl->PopUpRequestHandlerIBOX(sourceDeviceID,payloadLength,payload,STATUS_RESULTACK);
}

/*
 *******************************************************************
 * Function:CPopUpRequestHandlerHU
 *
 * Description :Constructor for CPopUpRequestHandlerHU.
 *              Stores the context of CHMIFBlockShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

CPopUpRequestHandlerHU::CPopUpRequestHandlerHU(CHMIFBlockShadowControl *context)
{
    m_HMIFBlockShadowControl = context;
}

/*
 *******************************************************************
 * Function:GetInterface
 *
 * Description :Callback for received GetInterface Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CPopUpRequestHandlerHU::GetInterface(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_HMIFBlockShadowControl->PopUpRequestHandlerHU(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:Abort
 *
 * Description :Callback for received Abort Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CPopUpRequestHandlerHU::Abort(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_HMIFBlockShadowControl->PopUpRequestHandlerHU(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:AbortAck
 *
 * Description :Callback for received AbortAck Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CPopUpRequestHandlerHU::AbortAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_HMIFBlockShadowControl->PopUpRequestHandlerHU(sourceDeviceID,payloadLength,payload,STATUS_ABORTACK);
}

/*
 *******************************************************************
 * Function:Start
 *
 * Description :Callback for received Start Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CPopUpRequestHandlerHU::Start(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_HMIFBlockShadowControl->PopUpRequestHandlerHU(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:StartAck
 *
 * Description :Callback for received StartAck Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CPopUpRequestHandlerHU::StartAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_HMIFBlockShadowControl->PopUpRequestHandlerHU(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:StartResult
 *
 * Description :Callback for received StartResult Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CPopUpRequestHandlerHU::StartResult(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_HMIFBlockShadowControl->PopUpRequestHandlerHU(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:StartResultAck
 *
 * Description :Callback for received StartResultAck Operation
 *              Sends the PopUpRequest to HU Contoroller
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CPopUpRequestHandlerHU::StartResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{ 
    m_HMIFBlockShadowControl->PopUpRequestHandlerHU(sourceDeviceID,payloadLength,payload,STATUS_STARTRESULTACK);
}
/*
 *******************************************************************
 * Function: CVideoDefaultSetHandlerHU
 *
 * Description :This is constructor of CVideoDefaultSetHandlerHU class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CHMIFBlockShadowControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CVideoDefaultSetHandlerHU::CVideoDefaultSetHandlerHU(CHMIFBlockShadowControl* context)
{
    m_HMIFBlockShadowControl = context;
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
void CVideoDefaultSetHandlerHU::GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->VideoDefaultSetHandlerHU(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
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
void CVideoDefaultSetHandlerHU::Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->VideoDefaultSetHandlerHU(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
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
void CVideoDefaultSetHandlerHU::Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->VideoDefaultSetHandlerHU(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
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
void CVideoDefaultSetHandlerHU::StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->VideoDefaultSetHandlerHU(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
/*
 *******************************************************************
 * Function:StartResultAck
 *
 * Description :Callback for received StartResultAck operation
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
void CVideoDefaultSetHandlerHU::StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->VideoDefaultSetHandlerHU(sourceDeviceID,payloadLength,payload,STATUS_STARTRESULTACK);
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
void CVideoDefaultSetHandlerHU::AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->VideoDefaultSetHandlerHU(sourceDeviceID,payloadLength,payload,STATUS_ABORTACK);
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
void CVideoDefaultSetHandlerHU::StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->VideoDefaultSetHandlerHU(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
/*
 *******************************************************************
 * Function:CVideoDefaultSetHandlerIBOX
 *
 * Description :Constructor for CVideoDefaultSetHandlerIBOX.
 *
 * Parameters :
 *    none
 *
 * Returns :void
 *
 *******************************************************************
 */
CVideoDefaultSetHandlerIBOX::CVideoDefaultSetHandlerIBOX(CHMIFBlockShadowControl* context)
{
    m_HMIFBlockShadowControl = context;
}

/** Virtual functions of IShadowMethodHandler for result message from IBOX*/
void CVideoDefaultSetHandlerIBOX::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->VideoDefaultSetHandlerIBOX(sourceDeviceID,payloadLength,payload,STATUS_ERRORACK);
}

void CVideoDefaultSetHandlerIBOX::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->VideoDefaultSetHandlerIBOX(sourceDeviceID,payloadLength,payload,STATUS_PROCESSINGACK);
}

void CVideoDefaultSetHandlerIBOX::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->VideoDefaultSetHandlerIBOX(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CVideoDefaultSetHandlerIBOX::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->VideoDefaultSetHandlerIBOX(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CVideoDefaultSetHandlerIBOX::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->VideoDefaultSetHandlerIBOX(sourceDeviceID,payloadLength,payload,STATUS_RESULTACK);
}

void CVideoDefaultSetHandlerIBOX::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->VideoDefaultSetHandlerIBOX(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CVideoDefaultSetHandlerIBOX::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->VideoDefaultSetHandlerIBOX(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}


CHMIFblockStateListener::CHMIFblockStateListener(acs::generated::shadow::HMI::CHMI *hmi, CHMIFBlockShadowControl* context)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)

    m_HMI = hmi;
    m_HMIFBlockShadowControl = context;
    m_Timer = new QTimer(this);
    m_Timer->setSingleShot(true);
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(OnWaitForTimeout()));
}

void CHMIFblockStateListener::OnChange()
{
    bool state = m_HMI->FunctionBlockVisible();

    LOG_SIGNAL << "CHMIFblockStateListener" << " HMI FBlock (InstID 0x02) on iBox device is Visible? = " << state << LOG_ENDL;

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

        m_HMIFBlockShadowControl->HMIFblockStateListener(state);
    }
}

void CHMIFblockStateListener::OnWaitForTimeout()
{
    bool state = m_HMI->FunctionBlockVisible();

#if 0
    struct timeval tv_;
    gettimeofday(&tv_, NULL);

    time_t sec = tv_.tv_sec - tv.tv_sec;
    long nsec = tv_.tv_usec - tv.tv_usec;

    LOG_SIGNAL << __FUNCTION__ << " Visible = " << state << " SEC: " << sec << " NSEC: " << nsec << LOG_ENDL;
#endif

    LOG_SIGNAL << __FUNCTION__ << " HMI FBlock (InstID 0x02) on iBox device is Visible in 1500ms? = " << state << LOG_ENDL;

    if( !state )
    {
        m_HMIFBlockShadowControl->HMIFblockStateListener(state);
    }
}

// Fcat v13.07.1
CSystemDefaultSetHandler::CSystemDefaultSetHandler(CHMIFBlockShadowControl* context)
{
    m_HMIFBlockShadowControl = context;
}

void CSystemDefaultSetHandler::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->SystemDefaultSetHandler(sourceDeviceID, payloadLength, payload, METHOD_ERRORACK);
}

void CSystemDefaultSetHandler::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->SystemDefaultSetHandler(sourceDeviceID, payloadLength, payload, METHOD_PROCESSINGACK);
}

void CSystemDefaultSetHandler::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_HMIFBlockShadowControl->SystemDefaultSetHandler(sourceDeviceID, payloadLength, payload, METHOD_RESULTACK);
}

// Fcat v13.07.2
CMuteControlListener::CMuteControlListener(CHMIFBlockShadowControl* context)
{
    m_HMIFBlockShadowControl = context;
}

void CMuteControlListener::OnChange()
{
    m_HMIFBlockShadowControl->MuteStatusListener();
}

/*
 *******************************************************************
 * Function:ModeStatusIBOX
 *
 * Description : Invoked by the Listener for ModeStatus of Ibox
 *               Sends the signal to HMIAdaptor
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CHMIFBlockShadowControl::ModeStatusIBOX()
{    
//    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " ModeStatusIBOX Signal got received " << MOST_LOG_ENDL;

    /** This position received is zero based */
    uint position = m_ModeSatusIBOX->FunctionInstance().LastPositionX();
    uint value = 0;

    if(position != 0)
    {
        /** Get the actual position */
        value = m_ModeSatusIBOX->Value(position -1);

        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " ModeStatusIBOX:Target = " << position << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " ModeStatusIBOX:Mode = " << value << MOST_LOG_ENDL;
        emit ModeStatusIBOX(position,value);
    }
    else if(position == 0)
    {
        value = m_ModeSatusIBOX->Value(0);
        emit ModeStatusIBOX(0,value);
    }
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " ModeStatus_Status <-- : Position = " << position
	<< " Data = " << value << MOST_LOG_ENDL;
	
}
/*
 *******************************************************************
 * Function:ModeStatusErrorIBOX
 *
 * Description : Invoked by the Listener for ModeStatus of Ibox
 *               Sends the error signal to HMIAdaptor
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CHMIFBlockShadowControl::ModeStatusErrorIBOX()
{    
//    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " ModeStatusError Signal got received " << MOST_LOG_ENDL;

    uint8_t errorCode = 0;
    uint32_t errorInfoLength = 0;
    const uint8_t* errorInfoPayload = NULL;
    QByteArray errorInfo = 0;
    if (k2l::acs::samples::CMOSTErrorInfo::GetMOSTError(
                    m_HMIShadow->Container(),
                    errorCode,
                    errorInfoLength,
                    errorInfoPayload
                    ))
    {
/*
        MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Error code " << errorCode << MOST_LOG_ENDL;
        MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Error Info Length " << errorInfoLength << MOST_LOG_ENDL;
        MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Error Info Payload " << errorInfoPayload << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
	MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " ModeStatus_Error <-- : errorcode = " << errorCode
	<< " errorinfo = " << errorInfoPayload << MOST_LOG_ENDL;

        QByteArray error((const char *)errorInfoPayload, errorInfoLength);
        errorInfo = error;
    }

    emit ModeStatusErrorIBOX(errorCode,errorInfo);
}

/*
 *******************************************************************
 * Function:KeyBoardSetting
 *
 * Description : Invoked by the Listener for KeyBoardSetting of Ibox
 *               Sends the signal to HMIAdaptor
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CHMIFBlockShadowControl::KeyBoardSetting()
{
    /** intialize the varaible */
    QList<uint> keyBoardSetting;
    keyBoardSetting.clear();

    keyBoardSetting.append(m_keyBoardHU->KeyBoardSettingValue());		// hyunyong.sung, 2015.2.10, add for keyboard type from ibox 
    keyBoardSetting.append(m_keyBoardHU->KoreanValue());
    keyBoardSetting.append(m_keyBoardHU->English_USValue());
    keyBoardSetting.append(m_keyBoardHU->English_EUValue());
    keyBoardSetting.append(m_keyBoardHU->ChineseValue());
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " KeyBoard.Set Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " KeyBoardSetting:Korean Keyboard = " << keyBoardSetting.at(0) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " KeyBoardSetting:English US Keyboard = " << keyBoardSetting.at(1) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " KeyBoardSetting:English EN Keyboard = " << keyBoardSetting.at(2) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " KeyBoardSetting:Chinese Keyboard = " << keyBoardSetting.at(3) << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " KeyBoard_Set <-- : KeyBoardSetting = " << keyBoardSetting.at(0) 
	<< " Korean = " << keyBoardSetting.at(1)	<< " EnglishUS = " << keyBoardSetting.at(2) 
	<< "EnglishEN" << keyBoardSetting.at(3) << "Chinese = "	<< keyBoardSetting.at(4) << MOST_LOG_ENDL;	

    /** Wrap the Structure in QDbusVariant */
    QVariant keyBoardDataVar;
    QDBusVariant dBuskeyBoardDataVar;

    QDBusArgument keyBoardDataArg;
    keyBoardDataArg << keyBoardSetting;
    keyBoardDataVar.setValue(keyBoardDataArg);

    dBuskeyBoardDataVar.setVariant(keyBoardDataVar);

    /** Send the Signal to HMI adaptor */
    emit KeyBoardSetting(dBuskeyBoardDataVar);
}

/*
 *******************************************************************
 * Function:Progressbar
 *
 * Description : Invoked by the Listener for Progressbar of Ibox
 *               Sends the signal to HMIAdaptor
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CHMIFBlockShadowControl::Progressbar()
{
// 2014.07.29, for log, hyunyong.sung
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " Progressbar Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " Progressbar:Status = " << m_PopUpProgressBarIBOX->ProgressValue() << MOST_LOG_ENDL;
*/    
    emit Progressbar(m_PopUpProgressBarIBOX->ProgressValue());
}
/*
 *******************************************************************
 * Function:DisplayOSDListener
 *
 * Description : Invoked by the Listener for DisplayOSD of Ibox
 *               Sends the signal to HMIAdaptor
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CHMIFBlockShadowControl::DisplayOSDListener()
{
    const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding = NULL;
    uint8_t* OSDTextString = NULL;
    uint32_t OSDTextStringLength = 0;

    struct SDisplayOSD displayOSD;
    displayOSD.target = m_DisplayOSD->TargetValue();
    displayOSD.osdId = m_DisplayOSD->OsdIdValue();
    displayOSD.osdControl = m_DisplayOSD->OsdControlValue();    
    displayOSD.osdMode = m_DisplayOSD->OsdModeValue();
    displayOSD.textColor = m_DisplayOSD->TextColorValue();
    m_DisplayOSD->OsdTextString(&pStringEncoding, &OSDTextString, &OSDTextStringLength);
    QString osd = QString::fromUtf8((const char *)OSDTextString, OSDTextStringLength);
    displayOSD.osdText = osd;
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " DisplayOSD Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " DisplayOSD: Target : " << displayOSD.target << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " DisplayOSD: OSD ID : " << displayOSD.osdId << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " DisplayOSD: OSD Control : " << displayOSD.osdControl << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " DisplayOSD: OSD Mode : " << displayOSD.osdMode << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " DisplayOSD: Text Color : " << displayOSD.textColor << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " DisplayOSD OSDtext " << displayOSD.osdText << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " DisplayOSD_Set <-- : Target = " << displayOSD.target
    << " OSD ID = " << displayOSD.osdId << "OSDControl = " << displayOSD.osdControl << " OSDMode = "
    << displayOSD.osdMode << " TextColor = "<< displayOSD.textColor << " OSDText = " << displayOSD.osdText << MOST_LOG_ENDL;


    QVariant displayOSDVar;
    QDBusVariant displayOSDdBusVar;

    QDBusArgument displayOSDDataArg;
    displayOSDDataArg << displayOSD;
    displayOSDVar.setValue(displayOSDDataArg);

    displayOSDdBusVar.setVariant(displayOSDVar);

    emit DisplayOSD(displayOSDdBusVar);
}
/*
 *******************************************************************
 * Function:ShowStatusBarSlot
 *
 * Description : Invoked by the Listener for ShowStatusBar of Ibox
 *               Sends the signal to HMIAdaptor
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CHMIFBlockShadowControl::ShowStatusBarSlot()
{
//    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " ShowStatusBar Signal got received " << MOST_LOG_ENDL;
//    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " ShowStatusBar: statusbar " << m_ShowStatusBar->StatusBarValue() << MOST_LOG_ENDL;

// 2014.07.29, for log, hyunyong.sung	
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " ShowStatusBar_Set <-- : statusbar = " 
    << m_ShowStatusBar->StatusBarValue() << MOST_LOG_ENDL;

    emit ShowStatusBar(m_ShowStatusBar->StatusBarValue());
}
/*
 *******************************************************************
 * Function:EmergencySetSlot
 *
 * Description : Invoked by the Listener for EmergencySet of Ibox
 *               Sends the signal to HMIAdaptor
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CHMIFBlockShadowControl::EmergencySetSlot()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " EmergencySet Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " EMGCState : " << m_Emergencyset->EMGCStateValue() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " ControllerLock : " << m_Emergencyset->ControllerLockValue() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SoundLock : " << m_Emergencyset->SoundLockValue() << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung	
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " EmergencySet_Set <-- : EMGCState = " 
    << m_Emergencyset->EMGCStateValue() << " ControllerLock = " << m_Emergencyset->ControllerLockValue() 
    << " SoundLock = " << m_Emergencyset->SoundLockValue() << MOST_LOG_ENDL;
	
    emit EmergencySetIBOX(m_Emergencyset->EMGCStateValue(),m_Emergencyset->ControllerLockValue(),m_Emergencyset->SoundLockValue());
}
/*
 *******************************************************************
 * Function:KeyInputStatusSlot
 *
 * Description :This slot is connected to signal KeyInputStatus
 *              from IBOX.Sends the same signal to HMI Adaptor
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
void CHMIFBlockShadowControl::KeyInputStatusSlot()
{
    uint hardKey;
    uint softKey;

    hardKey = m_KeyInputStatus->HardKeyValue();
    softKey = m_KeyInputStatus->SoftKeyValue();
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " KeyInputStatus Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " HardKey = " << hardKey << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SoftKey = " << softKey << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung	
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " KeyInputStatus_Set <-- : HardKey = " 
    << hardKey << " SoftKey = " << softKey << MOST_LOG_ENDL;

    emit KeyInputStatus(hardKey, softKey);
}
/*
 *******************************************************************
 * Function:DrivingRestrictionStatusSlot
 *
 * Description :This slot is connected to signal DrivingRestrictionStatus
 *              from IBOX.Sends the same signal to HMI Adaptor
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
void CHMIFBlockShadowControl::DrivingRestrictionStatusSlot()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " DrivingRestriction Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " DrivingRestriction: DrivingRestriction " << m_DrivingRestriction->Value() << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung	
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " DrivingRestriction_Status <-- : Need = " 
    << m_DrivingRestriction->Value() << MOST_LOG_ENDL;
	
    emit DrivingRestrictionStatus(m_DrivingRestriction->Value());
}
/*
 *******************************************************************
 * Function:LCDBrightnessFromIBOXSlot
 *
 * Description :This slot is connected to signal LCDBrightness
 *              from IBOX.Sends the same signal to HMI Adaptor
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
void CHMIFBlockShadowControl::LCDBrightnessFromIBOXSlot()
{    
    int position = m_LCDBrightnessHU->FunctionInstance().LastPositionX();
    int lcdFront = 0;
    int lcdRear1 = 0;
    int lcdRear2 = 0;

    switch(position)
    {
        case 0:
            lcdFront = m_LCDBrightnessHU->Value(0);
            lcdRear1 = m_LCDBrightnessHU->Value(1);
            lcdRear2 = m_LCDBrightnessHU->Value(2);
            break;
        case 1:
            lcdFront = m_LCDBrightnessHU->Value(0);
            break;
        case 2:
            lcdRear1 = m_LCDBrightnessHU->Value(1);
            break;
        case 3:
            lcdRear2 = m_LCDBrightnessHU->Value(2);
            break;
        default:
            break;
    }
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " LCDBrightness Signal got received from IBOX " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " LCDBrightnessIBOX : target " << position << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " LCDBrightnessIBOX : lcdFront " << lcdFront << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " LCDBrightnessIBOX : lcdRear1 " << lcdRear1 << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " LCDBrightnessIBOX : lcdRear2 " << lcdRear2 << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung	
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " LCDBrightness_Status <-- : Pos = " << position
    << " lcdFront = " << lcdFront << " lcdRear1 = " << lcdRear1 << " lcdRear2 = " << lcdRear2 << MOST_LOG_ENDL;

    emit LCDBrightnessFromIBOX(position,lcdFront,lcdRear1,lcdRear2);
}
/*
 *******************************************************************
 * Function:AdjustScreenFromIBOXSlot
 *
 * Description :This slot is connected to signal AdjustScreen
 *              from IBOX.Sends the same signal to HMI Adaptor
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
void CHMIFBlockShadowControl::AdjustScreenFromIBOXSlot()
{    
    int brightness = m_AdjustScreenIBOX->DataBrightnessValue();
    int saturation = m_AdjustScreenIBOX->DataSaturationValue();
    int contrast = m_AdjustScreenIBOX->DataContrastValue();
    int hue = m_AdjustScreenIBOX->DataHueValue();
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " AdjustScreen signal received from IBOX " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " AdjustScreen : brightness " << brightness << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " AdjustScreen : saturation " << saturation << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " AdjustScreen : contrast " << contrast << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " AdjustScreen : hue " << hue << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung	
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " AdjustScreen_Status <-- : brightness = " << brightness
    << " saturation = " << saturation << " contrast = " << contrast << " hue = " << hue << MOST_LOG_ENDL;

    emit AdjustScreenFromIBOX(brightness,saturation,contrast,hue);
}
/*
 *******************************************************************
 * Function:IlluminationFromIBOXSlot
 *
 * Description :This slot is connected to signal Illumination
 *              from IBOX.Sends the same signal to HMI Adaptor
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
void CHMIFBlockShadowControl::IlluminationFromIBOXSlot()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " IlluminationSetting signal received from IBOX " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " IlluminationSetting : Illumination " << m_IlluminationSettingIBOX->IlluminationValue();
*/
// 2014.07.29, for log, hyunyong.sung	
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " IlluminationSetting_Status <-- : Illumination = " 
    << m_IlluminationSettingIBOX->IlluminationValue() << MOST_LOG_ENDL;

    emit IlluminationFromIBOX(m_IlluminationSettingIBOX->IlluminationValue());
}
/*
 *******************************************************************
 * Function:VideoRatioFromIBOXSlot
 *
 * Description :This slot is connected to signal VideoRatio
 *              from IBOX.Sends the same signal to HMI Adaptor
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
void CHMIFBlockShadowControl::VideoRatioFromIBOXSlot()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " VideoRatio signal received from IBOX  " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " VideoRatio : VideoRatio " << m_VideoRatioIBOX->RatioValue() << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung	
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " VideoRatio_Status <-- : VideoRatio = " 
    << m_VideoRatioIBOX->RatioValue() << MOST_LOG_ENDL;	

    emit VideoRatioFromIBOX(m_VideoRatioIBOX->RatioValue());
}
/*
 *******************************************************************
 * Function:DisplayControlFromIBOXSlot
 *
 * Description :This slot is connected to signal DisplayControl
 *              from IBOX.Sends the same signal to HMI Adaptor
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
void CHMIFBlockShadowControl::DisplayControlFromIBOXSlot()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " DisplayControl signal received from IBOX " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " DisplayControl " << m_DisplayControlIBOX->Value() << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung	
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " DisplayControl_Set <-- : DisplayControl = " 
    << m_DisplayControlIBOX->Value() << MOST_LOG_ENDL;	

    emit DisplayControlFromIBOX(m_DisplayControlIBOX->Value());
}
/*
 *******************************************************************
 * Function:BeepSettingFromIBOXSlot
 *
 * Description :This slot is connected to signal BeepSetting
 *              from IBOX.Sends the same signal to HMI Adaptor
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */
void CHMIFBlockShadowControl::BeepSettingFromIBOXSlot()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " BeepSetting signal received from IBOX " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " BeepSetting " << m_Beepsetting->BeepSettingValue() << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung	
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " BeepSetting_Status <-- : BeepSetting = " 
    << m_Beepsetting->BeepSettingValue() << MOST_LOG_ENDL;	

    emit BeepSettingFromIBOX(m_Beepsetting->BeepSettingValue());
}

/*
 *******************************************************************
 * Function:PopUpRequestToHU
 *
 * Description : Invoked by the handler funtion for PopUpRequest To HU from IBOX
 *               Sends the signal to HMIAdaptor
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CHMIFBlockShadowControl::PopUpRequestToHU(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload)
{    
//    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " PopUpRequestToHU Signal got received " << MOST_LOG_ENDL;

    /** Parse the data  received in payload */
    if( !m_PopUpRequestHU->ParseStartResultAck(payloadLength,payload) )
    {
        if( payloadLength >= 2 )
        {
            ushort senderHandle = (ushort)payload[0] << 8 | payload[1];
            m_PopUpRequestHU->ErrorAck(sourceDeviceID, senderHandle, 0x05, 0, NULL);

//            MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " sourceDeviceID = " << sourceDeviceID << MOST_LOG_ENDL;
//            MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " senderHandle = " << senderHandle << MOST_LOG_ENDL;
        }

        return;
    }

    /** Update the Structure with PopUprequest Data  received*/
    struct SPopUpRequestData requestData;
    requestData.deviceID = sourceDeviceID;
    requestData.senderHandle = m_PopUpRequestHU->SenderHandleValue();
    requestData.target = m_PopUpRequestHU->TargetValue();
    requestData.displayHandle = m_PopUpRequestHU->DisplayHandleValue();
    requestData.popupSize = m_PopUpRequestHU->PopupSizeValue();
    requestData.popupType = m_PopUpRequestHU->PopUpTypeValue();
    requestData.titleNo = m_PopUpRequestHU->TitleNoValue();
    requestData.listNo = m_PopUpRequestHU->ListNoValue();
    requestData.textLineNo = m_PopUpRequestHU->TextLineNoValue();
    requestData.iconID = m_PopUpRequestHU->IconIDValue();
    requestData.iconPos = m_PopUpRequestHU->IconPositionValue();
    requestData.button1 = m_PopUpRequestHU->Button1Value();
    requestData.button2 = m_PopUpRequestHU->Button2Value();
    requestData.button3 = m_PopUpRequestHU->Button3Value();
    requestData.button4 = m_PopUpRequestHU->Button4Value();

    const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding = NULL;
    uint8_t* dataString = NULL;
    uint32_t dataStringLength = 0;

    m_PopUpRequestHU->PopUpDataString(&pStringEncoding, &dataString, &dataStringLength);
    requestData.popupData = QString::fromUtf8((const char *)dataString,dataStringLength);

    m_PopUpRequestHU->OsdTextString(&pStringEncoding, &dataString, &dataStringLength);
    requestData.OSDText = QString::fromUtf8((const char *)dataString,dataStringLength);
/*
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:senderHandle = " << (uint)requestData.senderHandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:target = " << requestData.target << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:displayHandle = " << requestData.displayHandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:popupSize = " << (uint)requestData.popupSize << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:popupType = " << (uint)requestData.popupType << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:TitleNo = " << (uint)requestData.titleNo << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:TextLineNo = " << (uint)requestData.textLineNo << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:ListNo = " << (uint)requestData.listNo << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:iconID = " << (uint)requestData.iconID << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:iconPos = " << (uint)requestData.iconPos << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:button1 = " << (uint)requestData.button1 << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:button2 = " << (uint)requestData.button2 << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:button3 = " << (uint)requestData.button3 << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:button4 = " << (uint)requestData.button4 << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:popupData = " << requestData.popupData.toAscii().data() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:OSDText = " << requestData.OSDText.toAscii().data() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpRequestToHU:Device ID = " << requestData.deviceID << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung	
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " PopUpRequest_StartResultAck <-- : senderHandle = " 
    << (uint)requestData.senderHandle << " target = " << requestData.target << " displayhandle = " << requestData.displayHandle
    << " PopupType = " << (uint)requestData.popupType << " popupdate = " << requestData.popupData.toAscii().data() 
    << " OSDText = " << requestData.OSDText.toAscii().data() << MOST_LOG_ENDL;	


    /** Wrap the Structure in QDbusVariant */
    QVariant popUpRequestDataVar;
    QDBusVariant dBuspopUpRequestDataVar;

    QDBusArgument popUpRequestDataArg;
    popUpRequestDataArg << requestData;
    popUpRequestDataVar.setValue(popUpRequestDataArg);

    dBuspopUpRequestDataVar.setVariant(popUpRequestDataVar);

    /** Send the Signal to HMI adaptor */
    emit PopUpRequestToHU(dBuspopUpRequestDataVar);
}

/*
 *******************************************************************
 * Function:PopUpRequestToHU
 *
 * Description : Invoked by the handler funtion for PopUpRequest To HU from IBOX
 *               Sends the signal to HMIAdaptor
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CHMIFBlockShadowControl::AbortPopUpRequestToHU(const uint32_t payloadLength, const uint8_t *payload)
{    
    ushort senderHandle = 0;

//    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " AbortPopUpRequestToHU Signal got received " << MOST_LOG_ENDL;

    /** Parse the data  received in payload */
    m_PopUpRequestHU->ParseAbortAck(payloadLength,payload);
    senderHandle = m_PopUpRequestHU->SenderHandleValue();
	
//    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " Sender Handle = " << senderHandle << MOST_LOG_ENDL;
// 2014.07.29, for log, hyunyong.sung	
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " PopUpRequest_AbortAck <-- : SenderHandle = " << senderHandle
    << MOST_LOG_ENDL;	

    emit AbortPopUpRequestToHU(senderHandle);
}

/*
 *******************************************************************
 * Function:PopUpResultFromIBOX
 *
 * Description : Invoked by the handler funtion for PopUpResult To HU from IBOX
 *               Sends the signal to HMIAdaptor
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CHMIFBlockShadowControl::PopUpResultFromIBOX(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uint state)
{  
    struct SPopUpResultData resultData;
    resultData.state = state;
    resultData.deviceId = sourceDeviceID;
    resultData.senderHandle = 0;
    resultData.target = 0xFF;
    resultData.displayHandle = 0xFF;
    resultData.reactionButton = 0;
    resultData.reactionList = 0;
    resultData.reactionText.clear();

    /** Check the result state and fill the structure data accordingly */
    if(state == HMI_RESULT_ACK)
    {
        m_PopUpRequestIBOX->ParseResultAck(payloadLength,payload);
        resultData.senderHandle = m_PopUpRequestIBOX->SenderHandleValue();
        resultData.target = m_PopUpRequestIBOX->TargetValue();
        resultData.displayHandle = m_PopUpRequestIBOX->DisplayHandleValue();
        resultData.reactionButton = m_PopUpRequestIBOX->ReactionButtonValue();
        resultData.reactionList = m_PopUpRequestIBOX->ReactionListValue();

        const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding = NULL;
        uint8_t* reactionTextString = NULL;
        uint32_t reactionTextStringLength = 0;
        if (m_PopUpRequestIBOX->ReactionTextString(
            &pStringEncoding,
            &reactionTextString,
            &reactionTextStringLength
            ))
        {
            if ((&k2l::acs::protocols::most::string::CStringEncoding::UTF8() == pStringEncoding) ||
                (&k2l::acs::protocols::most::string::CStringEncoding::ASCII() == pStringEncoding))
            {                
            }
        }
        resultData.reactionText.append(reinterpret_cast<const char*>(reactionTextString));
/*
        MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " PopUpResultFromIBOX Signal got received :: MOSTMSG_RESULT_ACK " << MOST_LOG_ENDL;
        MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " PopUpResultFromIBOX:Sender Handle = " << resultData.senderHandle << MOST_LOG_ENDL;
        MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " PopUpResultFromIBOX:Target = " << resultData.target << MOST_LOG_ENDL;
        MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " PopUpResultFromIBOX:Display Handle = " << resultData.displayHandle << MOST_LOG_ENDL;
        MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " PopUpResultFromIBOX:Reaction Button =" << resultData.reactionButton << MOST_LOG_ENDL;
        MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " PopUpResultFromIBOX:Reaction List =" << resultData.reactionList << MOST_LOG_ENDL;
        MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " PopUpResultFromIBOX:Reaction Text = " << resultData.reactionText.toAscii().data() << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung	
	MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " PopUpRequest_ResultAck <-- : SenderHandle = " << resultData.senderHandle
	<< " Target = " << resultData.target << " displayhandle = " << resultData.displayHandle << " reactionbutton = " 
	<< resultData.reactionButton << MOST_LOG_ENDL;	

		
    }
    else if(state == HMI_PROCESSING_ACK)
    {
        uint16_t senderHandle;
        m_PopUpRequestIBOX->ParseSenderHandle(payloadLength,payload,&senderHandle);
        resultData.senderHandle = senderHandle;
/*
        MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " PopUpResultFromIBOX Signal got received :: MOSTMSG_PROCESSING_ACK" << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " PopUpResultFromIBOX:Sender Handle = " << resultData.senderHandle << MOST_LOG_ENDL;
*/        
    }

    /** Wrap the Structure in QDbusVariant */
    QVariant popUpResultDataVar;
    QDBusVariant dBuspopUpResultDataVar;

    QDBusArgument popUpResultDataArg;
    popUpResultDataArg << resultData;
    popUpResultDataVar.setValue(popUpResultDataArg);

    dBuspopUpResultDataVar.setVariant(popUpResultDataVar);

    /** Send the Signal to HMI adaptor */
    emit PopUpResultFromIBOX(dBuspopUpResultDataVar);
}

/*
 *******************************************************************
 * Function:SetLanguage
 *
 * Description : Invoked by HMI Adaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      systemLanguageCode  rw System language is in ISO 639-1 coding + Number
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetLanguage(const QString systemLanguageCode)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetLanguage API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetLanguage:System Language Code = " << systemLanguageCode << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SystemLanguage_Status --> : systemlanguage = " 
    << systemLanguageCode << MOST_LOG_ENDL;

    /** Instance of Property SystemLanguage */
    acs::generated::fblock::HMI::CSystemLanguage sysytemLanguage(m_HMIFBlock);

    /** Update the System language String */
    sysytemLanguage.SystemLanguageString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                         reinterpret_cast<const uint8_t*>(systemLanguageCode.toUtf8().data()),
                                         systemLanguageCode.length()+1);

    /** Finally Call update shadow to send the data to ibox */
    return sysytemLanguage.UpdateShadows();
}

/*
 *******************************************************************
 * Function:SetVolumeStatus
 *
 * Description : Invoked by HMI Adaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      sourceType     rw source for whihc volume need to be set
 *      defaultVolume  rw Default volume level
 *      currentVolume  rw Current volume level
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetVolumeStatus(const uint sourceType, const uint defaultVolume, const uint currentVolume)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetVolumeStatus API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT <<  " SetVolumeStatus:Source Type = " << sourceType << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT <<  " SetVolumeStatus:Default Volume = " << defaultVolume << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT <<  " SetVolumeStatus:Current Volume = " << currentVolume << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SystemVolumeStatus_Status --> : sourceType = " 
    << sourceType << " defaultVolume = " << defaultVolume << " currentVolume = " << currentVolume << MOST_LOG_ENDL;

    /** Instance of Property SystemVolumeStatus */
    acs::generated::fblock::HMI::CSystemVolumeStatus volumeStatus(m_HMIFBlock);
    /** Set the received parameters to the property */
    volumeStatus.SourceTypeValue((acs::generated::fblock::HMI::CSystemVolumeStatus::TSourceType)sourceType);
    volumeStatus.DefaultVolumeValue(defaultVolume);
    volumeStatus.CurrentVolumeValue(currentVolume);

    /** Finally Call update shadow to send the data to ibox */
    return volumeStatus.UpdateShadows();
}

/*
 *******************************************************************
 * Function:SetTime
 *
 * Description :Invoked by HMI Adaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      time     rw System time
 *      time[0]     Time type
 *      time[1]     Current Hour
 *      time[2]     Current Min
 *      time[3]     Current Sec
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetTime(const QByteArray time, const int timeZone, const uint dst)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetTime API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetTime:Time(as Hex hhmmss) = " << time.toHex().data()  << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetTime:TimeZone = " << timeZone  << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetTime:DST = " << dst  << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SystemTime_Status --> : Time = " 
    << time.toHex().data() << " timeZone = " << timeZone << " DST = " << dst << MOST_LOG_ENDL;

    /** Set the received parameters to the property */
    m_SystemTime->TimeTypeValue((acs::generated::fblock::HMI::CSystemTime::TTimeType)time.at(0));
    m_SystemTime->CurrentHourValue(time.at(1));
    m_SystemTime->CurrentMinValue(time.at(2));
    m_SystemTime->CurrentSecValue(time.at(3));
    m_SystemTime->TimezoneValue((short)timeZone);
    m_SystemTime->DSTValue(dst);

    /** Finally Call update shadow to send the data to ibox */
    return m_SystemTime->UpdateShadows();
}

/*
 *******************************************************************
 * Function:SetDate
 *
 * Description :Invoked by HMI Adaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      date     rw System Date
 *      date[0]     Current Year
 *      date[1]     Current Month
 *      date[2]     Current Date
 *      datetype    Format of date
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetDate(const QDBusVariant systemDate)
{
    SDate date;
    systemDate.variant().value<QDBusArgument>() >> date;
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetDate API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetDate:Current Year = " << date.year  << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetDate:Current Month = " << date.month << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetDate:Current Day = " << date.day << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetDate:Date Type = " << date.datetype << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SystemDate_Status --> : Year = " 
    << date.year << " Month = " << date.month << " Day = " << date.day << " dateType = " << date.datetype
    << MOST_LOG_ENDL;

    /** Set the year  */
    m_SystemDate->CurrentYearValue(date.year);

    /** Set the month  */
    m_SystemDate->CurrentMonthValue(date.month);

     /** Set the Day  */
    m_SystemDate->CurrentDayValue(date.day);

    m_SystemDate->DateTypeValue((acs::generated::fblock::HMI::CSystemDate::TDateType)date.datetype);

    /** Finally Call update shadow to send the data to ibox */
    return m_SystemDate->UpdateShadows();
}

/*
 *******************************************************************
 * Function:SetModeStatusIBOX
 *
 * Description :Invoked by HMI Adaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      target  rw  Target
 *      mode    rw  Differnt modes
 * Returns : true if property is Set else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetModeStatusIBOX(const uint target, const uint mode)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetModeStatusIBOX API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetModeStatusIBOX:Target = " << target << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetModeStatusIBOX:Mode = " << mode << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " ModeStatus_Set --> : Target = " 
    << target << " Mode = " << mode << MOST_LOG_ENDL;

    switch(target)
    {
        case 0:
            m_ModeSatusIBOX->Value(target,(acs::generated::shadow::HMI::CModeStatus::TData)mode);
            m_ModeSatusIBOX->Value(target+1,(acs::generated::shadow::HMI::CModeStatus::TData)mode);
            m_ModeSatusIBOX->Value(target+2,(acs::generated::shadow::HMI::CModeStatus::TData)mode);
            break;

        default:
            m_ModeSatusIBOX->Value(target-1,(acs::generated::shadow::HMI::CModeStatus::TData)mode);
            break;

    }
    m_ModeSatusIBOX->Size(3);
    /** Call Set on property ModeStatus */    
    return m_ModeSatusIBOX->FunctionInstance().Set(target);
    
}

/*
 *******************************************************************
 * Function:SetModeStatusHU
 *
 * Description :Invoked by HMI Adaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      target  rw  Target
 *      mode    rw  Differnt modes
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetModeStatusHU(const uint target, const uint mode)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetModeStatusHU API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetModeStatusHU:Target = " << target << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetModeStatusHU:Mode = " << mode << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " ModeStatus_Status --> : Target = " 
    << target << " Mode = " << mode << MOST_LOG_ENDL;

    /** Update the paramaters for Property Mode Status */    
    switch(target)
    {
        case 0:
            m_ModeStatusHU->Value(target,(acs::generated::fblock::HMI::CModeStatus::TData)mode);
            m_ModeStatusHU->Value(target+1,(acs::generated::fblock::HMI::CModeStatus::TData)mode);
            m_ModeStatusHU->Value(target+2,(acs::generated::fblock::HMI::CModeStatus::TData)mode);
            break;

        default:
            m_ModeStatusHU->Value(target-1,(acs::generated::fblock::HMI::CModeStatus::TData)mode);
            break;
    }
    m_ModeStatusHU->Size(3);
    
    /** Finally Call update shadow to send the data to ibox */
    return m_ModeStatusHU->FunctionInstance().UpdateShadows(target);
}

/*
 *******************************************************************
 * Function:SetSoundStatus
 *
 * Description :Invoked by HMI Adaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      soundStatus  rw  background sound
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetSoundStatus(const uint soundStatus)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetSoundStatus API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetSoundStatus:Sound Status = " << soundStatus << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SoundStatus_Status --> : Status = "<< soundStatus<< MOST_LOG_ENDL;

    acs::generated::fblock::HMI::CSoundStatus soundStatusProperty(m_HMIFBlock);
    /** Update the paramaters for Property BG Sound Status */
    soundStatusProperty.BGSoundValue((acs::generated::fblock::HMI::CSoundStatus::TBGSound)soundStatus);
    /** Finally Call update shadow to send the data to ibox */
    return soundStatusProperty.UpdateShadows();
}

/*
 *******************************************************************
 * Function:SetSystemVisibilityHU
 *
 * Description :Invoked by HMIAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      vTarget    rw  Video Target
 *      vSource    rw  Video Source
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */
bool CHMIFBlockShadowControl::SetSystemVisibilityHU(const uint vTarget, const uint vSource)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetSystemVisibilityHU API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetSystemVisibilityHU:vTarget = " << vTarget << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SystemVisibility_Set --> : Pos = "<< vTarget
    << " Data = " << vSource << MOST_LOG_ENDL;

    /** Instance of Property SystemVisibility */
    acs::generated::fblock::HMI::CSystemVisibility systemVisibility(m_HMIFBlock);

    /** Update the paramaters for Property SystemVisibility */
    if(vTarget == 0)
    {
        systemVisibility.Value(vTarget, (acs::generated::fblock::HMI::CSystemVisibility::TData)vSource);
        systemVisibility.Value(vTarget+1, (acs::generated::fblock::HMI::CSystemVisibility::TData)vSource);
        systemVisibility.Value(vTarget+2, (acs::generated::fblock::HMI::CSystemVisibility::TData)vSource);        
//        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetSystemVisibilityHU:vSource = " << vSource << MOST_LOG_ENDL;
    }
    else
    {
        systemVisibility.Value(vTarget-1,(acs::generated::fblock::HMI::CSystemVisibility::TData)vSource);
//        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetSystemVisibilityHU:vSource" << vTarget << " = " << vSource << MOST_LOG_ENDL;
    }

    systemVisibility.Size(3);

    /** Finally Call update shadow to send the data to ibox */
    return systemVisibility.FunctionInstance().UpdateShadows(vTarget);
}

bool CHMIFBlockShadowControl::SetSystemVisibilityHU(const uint vTarget, const QByteArray vSource)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetSystemVisibilityHU API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetSystemVisibilityHU:vTarget = " << vTarget << MOST_LOG_ENDL;
*/
    /** Instance of Property SystemVisibility */
    acs::generated::fblock::HMI::CSystemVisibility systemVisibility(m_HMIFBlock);

    /** Update the paramaters for Property SystemVisibility */
    if(vTarget == 0)
    {
        systemVisibility.Value(vTarget, (acs::generated::fblock::HMI::CSystemVisibility::TData)vSource.at(0));
        systemVisibility.Value(vTarget+1, (acs::generated::fblock::HMI::CSystemVisibility::TData)vSource.at(1));
        systemVisibility.Value(vTarget+2, (acs::generated::fblock::HMI::CSystemVisibility::TData)vSource.at(2));

//        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetSystemVisibilityHU:vSource1, vSource2, vSource3 = "
//                 << (uint)vSource.at(0) << " " << (uint)vSource.at(1) << " " << (uint)vSource.at(2) << MOST_LOG_ENDL;
    }
    else
    {
        systemVisibility.Value(vTarget-1,(acs::generated::fblock::HMI::CSystemVisibility::TData)vSource.at(0));
//        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetSystemVisibilityHU:vSource" << vTarget << " = " << (uint)vSource.at(0) << MOST_LOG_ENDL;
    }

// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SystemVisibility_Status --> : Pos = "<< vTarget
    << " Front = " << (uint)vSource.at(0) << " Rear1 = " << (uint)vSource.at(1)
    << " Rear2 = " << (uint)vSource.at(2) <<MOST_LOG_ENDL;

    systemVisibility.Size(3);

    /** Finally Call update shadow to send the data to ibox */
    return systemVisibility.FunctionInstance().UpdateShadows(vTarget);
}

/*
 *******************************************************************
 * Function:SetSystemAudibilityHU
 *
 * Description :Invoked by HMIAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      aTarget    rw  Audio Target
 *      aSource    rw  Audio Source
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetSystemAudibilityHU(const uint aTarget, const uint aSource)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetSystemAudibilityHU API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetSystemAudibilityHU:aTarget = " << aTarget << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetSystemAudibilityHU:aSource = " << aSource << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SystemAudibility_Status --> : Pos = "<< aTarget
    << " Data = " << aSource <<MOST_LOG_ENDL;

    /** Instance of Property SystemAudibility */
    acs::generated::fblock::HMI::CSystemAudibility systemAudibility(m_HMIFBlock);
    /** Update the paramaters for Property SystemAudibility */    
    switch(aTarget)
    {
        case 0:
            systemAudibility.Value(aTarget,(acs::generated::fblock::HMI::CSystemAudibility::TData)aSource);
            systemAudibility.Value(aTarget+1,(acs::generated::fblock::HMI::CSystemAudibility::TData)aSource);
            systemAudibility.Value(aTarget+2,(acs::generated::fblock::HMI::CSystemAudibility::TData)aSource);
            break;

        default:
            systemAudibility.Value(aTarget-1,(acs::generated::fblock::HMI::CSystemAudibility::TData)aSource);
            break;
    }

    systemAudibility.Size(3);
    /** Finally Call update shadow to send the data to ibox */
    return systemAudibility.FunctionInstance().UpdateShadows(aTarget);
}

/*
 *******************************************************************
 * Function:SetSystemVisibilityHUError
 *
 * Description :Invoked by HMIAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      errorCode    rw  Error code
 *      errorInfo    rw  Error Info
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetSystemVisibilityHUError(const uint errorCode, const QString &errorInfo)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetSystemVisibilityHUError API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetSystemVisibilityHUError:Error Code = " << errorCode << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetSystemVisibilityHUError:Error Info = " << errorInfo << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SystemVisiibility_Error --> : errorcode = "<< errorCode
    << " errorinfo = " << errorInfo <<MOST_LOG_ENDL;
	
    return m_SystemVisibilityHU->Error(IBOX_DEVICE_ADDRESS,
                                    errorCode,errorInfo.length(),
                                    reinterpret_cast<const uint8_t*>(errorInfo.toUtf8().data()));
}


bool CHMIFBlockShadowControl::SetSystemAudibilityHUError(const uint errorCode, const QString &errorInfo)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetSystemAudibilityHUError API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetSystemAudibilityHUError:Error Code = " << errorCode << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetSystemAudibilityHUError:Error Info = " << errorInfo << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SystemAudibility_Error --> : errorcode = "<< errorCode
    << " errorinfo = " << errorInfo <<MOST_LOG_ENDL;

    return m_SystemAudibilityHU->Error(IBOX_DEVICE_ADDRESS,
                                    errorCode,errorInfo.length(),
                                    reinterpret_cast<const uint8_t*>(errorInfo.toUtf8().data()));
}

/*
 *******************************************************************
 * Function:SetScreen
 *
 * Description :Invoked by HMIAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters : For all below parameters
 *              The HMI allows possible settings from -10...+10 in steps by 1 (21 levels),
 *              default setting 0.
 *      brightness    rw
 *      saturation    rw
 *      contrast      rw
 *      hue           rw
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetScreen(const int brightness, const int saturation, const int contrast, const int hue)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetScreen API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetScreen:Brightness = " << brightness << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetScreen:Saturation = " << saturation << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetScreen:Contrast = " << contrast << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetScreen:HUE = " << hue << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " AdjustScreen_Status --> : Brightness = "<< brightness
    << " Saturation = " << saturation << " Contrast = " << contrast << " hue = " << hue << MOST_LOG_ENDL;

    /** Instance of Property AdjustScreen */
    acs::generated::fblock::HMI::CAdjustScreen adjustScreen(m_HMIFBlock);
    /** Update the paramaters for Property AdjustScreen */
    adjustScreen.DataBrightnessValue(brightness);
    adjustScreen.DataSaturationValue(saturation);
    adjustScreen.DataContrastValue(contrast);
    adjustScreen.DataHueValue(hue);

    /** Finally Call update shadow to send the data to ibox */
    return adjustScreen.UpdateShadows();
}

/*
 *******************************************************************
 * Function:SetLCDBrightness
 *
 * Description :Invoked by HMIAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *  lcdBrightness rw    The HMI allows possible settings from -10...+10 in steps by 1 (21 levels),
 *                      default setting 0.
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetLCDBrightness(const int bTarget,const int lcdFront, const int lcdRear1,const int lcdRear2)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetLCDBrightness API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetLCDBrightness:target = " << bTarget << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetLCDBrightness:lcdBrightness-lcdFront = "
             << lcdFront << " lcdRear1 = " << lcdRear1 << " lcdRear2 = " << lcdRear2 << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " LCDBrightness_Status --> : Pos = "<< bTarget
    << " lcdfront = " << lcdFront << " lcdRear1 = " << lcdRear1 << " lcdRear2 = " << lcdRear2 << MOST_LOG_ENDL;

    /** Update the paramaters for Property AdjustScreen */
    switch(bTarget)
    {
        case 0:
            m_LCDBrightnessHU->Value(0,lcdFront);
            m_LCDBrightnessHU->Value(1,lcdRear1);
            m_LCDBrightnessHU->Value(2,lcdRear2);
            break;
        case 1:
            m_LCDBrightnessHU->Value(0,lcdFront);
            break;
        case 2:
            m_LCDBrightnessHU->Value(1,lcdRear1);
            break;
        case 3:
            m_LCDBrightnessHU->Value(2,lcdRear2);
            break;
        default:
            break;
    }
    m_LCDBrightnessHU->Size(3);
    /** Finally Call update shadow to send the data to ibox */
    return m_LCDBrightnessHU->FunctionInstance().UpdateShadows(bTarget);
}

/*
 *******************************************************************
 * Function:SetIllumination
 *
 * Description :Invoked by HMIAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *  illumination rw  Illumination values
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetIllumination(const int illumination)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetIllumination API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetIllumination:Illumination = " << illumination << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " IlluminationSetting_Status --> : Illumination = "<< illumination << MOST_LOG_ENDL;

    /** Instance of Property IlluminationSetting */
    acs::generated::fblock::HMI::CIlluminationSetting illuminationSetting(m_HMIFBlock);
    /** Update the paramaters for Property IlluminationSetting */
    illuminationSetting.IlluminationValue((acs::generated::fblock::HMI::CIlluminationSetting::TIllumination)illumination);

    /** Finally Call update shadow to send the data to ibox */
    return illuminationSetting.UpdateShadows();
}

/*
 *******************************************************************
 * Function:SetVideoRatio
 *
 * Description :Invoked by HMIAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *  ratio rw  the ratio for which video should be adjusted
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetVideoRatio(const int ratio)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetVideoRatio API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetVideoRatio:Ratio = " << ratio << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " VideoRatio_Status --> : Ratio = "<< ratio << MOST_LOG_ENDL;

    /** Instance of Property VideoRatio */
    acs::generated::fblock::HMI::CVideoRatio videoRatio(m_HMIFBlock);
    /** Update the paramaters for Property VideoRatio */
    videoRatio.RatioValue((acs::generated::fblock::HMI::CVideoRatio::TRatio)ratio);

    /** Finally Call update shadow to send the data to ibox */
    return videoRatio.UpdateShadows();
}

/*
 *******************************************************************
 * Function:SetKeyBoard
 *
 * Description :Invoked by HMIAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *  keyboard rw  The Type of key board to be used
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetKeyBoard(const QDBusVariant keyBoardData)
{    
    /** Retrive the structure form QDbusArgument */
    QList<uint> data;
    keyBoardData.variant().value<QDBusArgument>() >> data;
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetKeyBoard API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetKeyBoard:Korean Keyboard = " << data.at(0) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetKeyBoard:English US Keyboard = " << data.at(1) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetKeyBoard:English EN Keyboard = " << data.at(2) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetKeyBoard:Chinese Keyboard = " << data.at(3) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetKeyBoard:Keyboard = " << data.at(4) << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " KeyBoard_Status --> : Korean = "<< data.at(0)
    << " EnglishUS = " << data.at(1) << " EnglishEN = " << data.at(2) << " chinese = " << data.at(3)
    << " keyboardsetting = " << data.at(4) << MOST_LOG_ENDL;

    /** Instance of Property KeyBoard */
    acs::generated::fblock::HMI::CKeyBoard keyboardProperty(m_HMIFBlock);
    /** Update the paramaters for Property KeyBoard */
    keyboardProperty.KoreanValue((acs::generated::fblock::HMI::CKeyBoard::TKorean)data.at(0));
    keyboardProperty.English_USValue((acs::generated::fblock::HMI::CKeyBoard::TEnglish_US)data.at(1));
    keyboardProperty.English_EUValue((acs::generated::fblock::HMI::CKeyBoard::TEnglish_EU)data.at(2));
    keyboardProperty.ChineseValue((acs::generated::fblock::HMI::CKeyBoard::TChinese)data.at(3));
    keyboardProperty.KeyBoardSettingValue(data.at(4));

    /** Finally Call update shadow to send the data to ibox */
    return keyboardProperty.UpdateShadows();
}

/*
 *******************************************************************
 * Function:SetProgressbar
 *
 * Description :Invoked by HMIAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *  status rw  progress bar status for PopUpRequest
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetProgressbar(const uint status)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetProgressbar API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetProgressbar:Status = " << status << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " Progressbar_Status --> : Status = " << status << MOST_LOG_ENDL;


    /** Instance of Property PopUpProgressBar */
    acs::generated::fblock::HMI::CPopUpProgressBar progressbar(m_HMIFBlock);
    /** Update the paramaters for Property PopUpProgressBar */
    progressbar.ProgressValue(status);

    /** Finally Call update shadow to send the data to ibox */
    return progressbar.UpdateShadows();
}

/*
 *******************************************************************
 * Function:SetDisplayControl
 *
 * Description :Invoked by HMIAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *  vTarget         rw  Target display
 *  displayStatus   rw  Display On/Off status
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetDisplayControl(const uint displayStatus)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetDisplayControl API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetDisplayControl:Display Status = " << displayStatus << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " DisplayControl_Status --> : Status = " << displayStatus << MOST_LOG_ENDL;	

    /** Instance of Property CDisplayControl */
    acs::generated::fblock::HMI::CDisplayControl displayControl(m_HMIFBlock);
    /** Update the paramaters for Property PopUpProgressBar */    
    displayControl.Value(displayStatus);

    /** Finally Call update shadow to send the data to ibox */
    return displayControl.UpdateShadows();
}


/*
 *******************************************************************
 * Function:SetDistanceUnit
 *
 * Description :Invoked by HMIAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *  distanceUnit         rw  distance unit
 * Returns : true if property is updated else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetDistanceUnit(const uint distanceUnit)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetDistanceUnit API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetDistanceUnit:Distance unit = " << distanceUnit << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " DistanceUnit_Status --> : distanceunit = " << distanceUnit << MOST_LOG_ENDL;	

    /** Instance of Property CDisplayControl */
    acs::generated::fblock::HMI::CDistanceUnit distUnit(m_HMIFBlock);
    /** Update the paramaters for Property PopUpProgressBar */
    distUnit.DistanceUnitValue((acs::generated::fblock::HMI::CDistanceUnit::TDistanceUnit)distanceUnit);

    /** Finally Call update shadow to send the data to ibox */
    return distUnit.UpdateShadows();
}


/*
 *******************************************************************
 * Function:SetPopUpRequestToIBOX
 *
 * Description :Invoked by HMIAdaptor to Send PopUprequest
 *                to Ibox
 * Parameters :
 *  popUpRequestData rw  Structure whihc holds PopUprequest Data
 * Returns : true if StartResultAck is called else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetPopUpRequestToIBOX(const SPopUpRequestData popUpRequestData)
{
    /** Update the paramaters for Property PopUpRequest which are received in structure */
    m_PopUpRequestIBOX->SenderHandleValue(popUpRequestData.senderHandle);
    m_PopUpRequestIBOX->TargetValue((acs::generated::shadow::HMI::CPopUpRequest::TTarget)popUpRequestData.target);
    m_PopUpRequestIBOX->DisplayHandleValue((acs::generated::shadow::HMI::CPopUpRequest::TDisplayHandle)popUpRequestData.displayHandle);
    m_PopUpRequestIBOX->PopupSizeValue((acs::generated::shadow::HMI::CPopUpRequest::TPopupSize)popUpRequestData.popupSize);
    m_PopUpRequestIBOX->PopUpTypeValue((acs::generated::shadow::HMI::CPopUpRequest::TPopUpType)popUpRequestData.popupType);
    m_PopUpRequestIBOX->TitleNoValue(popUpRequestData.titleNo);
    m_PopUpRequestIBOX->TextLineNoValue(popUpRequestData.textLineNo);
    m_PopUpRequestIBOX->ListNoValue(popUpRequestData.listNo);
    m_PopUpRequestIBOX->IconIDValue((acs::generated::shadow::HMI::CPopUpRequest::TIconID)popUpRequestData.iconID);
    m_PopUpRequestIBOX->IconPositionValue((acs::generated::shadow::HMI::CPopUpRequest::TIconPosition)popUpRequestData.iconPos);
    m_PopUpRequestIBOX->Button1Value((acs::generated::shadow::HMI::CPopUpRequest::TButton1)popUpRequestData.button1);
    m_PopUpRequestIBOX->Button2Value((acs::generated::shadow::HMI::CPopUpRequest::TButton2)popUpRequestData.button2);
    m_PopUpRequestIBOX->Button3Value((acs::generated::shadow::HMI::CPopUpRequest::TButton3)popUpRequestData.button3);
    m_PopUpRequestIBOX->Button4Value((acs::generated::shadow::HMI::CPopUpRequest::TButton4)popUpRequestData.button4);

    m_PopUpRequestIBOX->PopUpDataString(
        &k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                reinterpret_cast<const uint8_t*>(popUpRequestData.popupData.toUtf8().data()),
                popUpRequestData.popupData.length()+1);
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetPopUpRequestToIBOX API got called " << MOST_LOG_ENDL;
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SetPopUpRequestToIBOX:senderHandle " << (uint)popUpRequestData.senderHandle << MOST_LOG_ENDL;
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SetPopUpRequestToIBOX:target " << popUpRequestData.target << MOST_LOG_ENDL;
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SetPopUpRequestToIBOX:displayHandle " << popUpRequestData.displayHandle << MOST_LOG_ENDL;
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SetPopUpRequestToIBOX:popupSize = " << (uint)popUpRequestData.popupSize << MOST_LOG_ENDL;
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SetPopUpRequestToIBOX:popupType = " << (uint)popUpRequestData.popupType << MOST_LOG_ENDL;
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SetPopUpRequestToIBOX:TitleNo = " << (uint)popUpRequestData.titleNo << MOST_LOG_ENDL;
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SetPopUpRequestToIBOX:TextLineNo = " << (uint)popUpRequestData.textLineNo << MOST_LOG_ENDL;
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SetPopUpRequestToIBOX:ListNo = " << (uint)popUpRequestData.listNo << MOST_LOG_ENDL;
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SetPopUpRequestToIBOX:iconID = " << (uint)popUpRequestData.iconID << MOST_LOG_ENDL;
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SetPopUpRequestToIBOX:iconPos = " << (uint)popUpRequestData.iconPos << MOST_LOG_ENDL;
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SetPopUpRequestToIBOX:button1 = " << (uint)popUpRequestData.button1 << MOST_LOG_ENDL;
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SetPopUpRequestToIBOX:button2 = " << (uint)popUpRequestData.button2 << MOST_LOG_ENDL;
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SetPopUpRequestToIBOX:button3 = " << (uint)popUpRequestData.button3 << MOST_LOG_ENDL;
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SetPopUpRequestToIBOX:button4 = " << (uint)popUpRequestData.button4 << MOST_LOG_ENDL;
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SetPopUpRequestToIBOX:popupData = " << popUpRequestData.popupData.toAscii().data() << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " PopupRequest_StartResultAck --> : senderHandle = " << (uint)popUpRequestData.senderHandle 
    << " target = " << (uint)popUpRequestData.target << " displayHandle = " << popUpRequestData.displayHandle 
    << " popupType = " << (uint)popUpRequestData.popupType << " popupData = " << popUpRequestData.popupData.toAscii().data()
    << MOST_LOG_ENDL;	

    /** Call StartResultACK */
    return m_PopUpRequestIBOX->StartResultAck();
}

/*
 *******************************************************************
 * Function:SetPopUpResultFromHU
 *
 * Description :Invoked by HMIAdaptor to Send PopUpResult
 *                to Ibox
 * Parameters :
 *  popUpResultData rw  Structure which holds PopUpResult Data
 * Returns : true if StartResultAck is called else false
 *
 *******************************************************************
 */

bool CHMIFBlockShadowControl::SetPopUpResultFromHU(const SPopUpResultData popUpResultData)
{
    bool retVal = false;

    /** Update the paramaters for Property PopUpResult which are received in structure */
    m_PopUpRequestHU->SenderHandleValue(popUpResultData.senderHandle);
    if(popUpResultData.state == HMI_RESULT_ACK)
    {
/*
        MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetPopUpResultFromHU API got called :: MOSTMSG_RESULT_ACK " << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetPopUpResultFromHU:Sender Handle = " << popUpResultData.senderHandle << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetPopUpResultFromHU:Target = " << popUpResultData.target << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetPopUpResultFromHU:Display Handle = " << popUpResultData.displayHandle << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetPopUpResultFromHU:Reaction Button = " << (uint)popUpResultData.reactionButton << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetPopUpResultFromHU:Reaction List =" << (uint)popUpResultData.reactionList << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetPopUpResultFromHU:Reaction Text = " << popUpResultData.reactionText.toAscii().data() << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetPopUpResultFromHU:State = " << popUpResultData.state << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetPopUpResultFromHU:Device ID = " << popUpResultData.deviceId << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " PopupRequest_ResultAck --> : senderHandle = " << popUpResultData.senderHandle 
	<< " target = " << popUpResultData.target << " displayHandle = " << popUpResultData.displayHandle 
	<< " reationbutton = " << (uint)popUpResultData.reactionButton << MOST_LOG_ENDL;	

        m_PopUpRequestHU->TargetValue((acs::generated::fblock::HMI::CPopUpRequest::TTarget)popUpResultData.target);
        m_PopUpRequestHU->DisplayHandleValue((acs::generated::fblock::HMI::CPopUpRequest::TDisplayHandle)popUpResultData.displayHandle);
        m_PopUpRequestHU->ReactionButtonValue((acs::generated::fblock::HMI::CPopUpRequest::TReactionButton)popUpResultData.reactionButton);
        m_PopUpRequestHU->ReactionListValue(popUpResultData.reactionList);

        m_PopUpRequestHU->ReactionTextString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                             reinterpret_cast<const uint8_t*>(popUpResultData.reactionText.toUtf8().data()),
                                             popUpResultData.reactionText.length()+1);
        retVal = m_PopUpRequestHU->ResultAck(popUpResultData.deviceId);
    }
    else if(popUpResultData.state == HMI_PROCESSING_ACK)
    {
/*
        MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetPopUpResultFromHU API got called :: MOSTMSG_PROCESSING_ACK " << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetPopUpResultFromHU:Sender Handle = " << popUpResultData.senderHandle << MOST_LOG_ENDL;
*/
        uint16_t senderHandle = popUpResultData.senderHandle;
        retVal = m_PopUpRequestHU->ProcessingAck(popUpResultData.deviceId,senderHandle);
    }

    return retVal;
}
/*
 *******************************************************************
 * Function:SetKeyInputStatus
 *
 * Description :Invoked by HMIAdaptor to Send KeyInputStatus
 *                to Ibox
 * Parameters :
 *  keyInput     rw    holds the keyInput info
 *
 * Returns : true if Set is called else false
 *
 *******************************************************************
 */
//bool CHMIFBlockShadowControl::SetKeyInputStatus(const uint keyInput)
bool CHMIFBlockShadowControl::SetKeyInputStatus(const uint hardKey, const uint softKey) // Fcat v13.9.3
{
/*
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetKeyInputSetting API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " HardKey = " << hardKey << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SoftKey = " << softKey << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " KeyInputStatus_Status --> : HardKey = " << hardKey
	<< " SoftKey = " << softKey << MOST_LOG_ENDL;	

    m_KeyInputStatus->HardKeyValue(hardKey);
    m_KeyInputStatus->SoftKeyValue(softKey);

    return m_KeyInputStatus->Set();
}
/*
 *******************************************************************
 * Function:SetBeepSetting
 *
 * Description :Invoked by HMIAdaptor to Send BeepSetting
 *                to Ibox
 * Parameters :
 *  beepSetting     rw    holds the beepSetting value to ibox
 *
 * Returns : true if Status is called else false
 *
 *******************************************************************
 */
bool CHMIFBlockShadowControl::SetBeepSetting(const uint beepSetting)
{    
//    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetBeepSetting API got called " << MOST_LOG_ENDL;

// 2014.07.29, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " BeepSetting_Status --> : beepsetting = " << beepSetting << MOST_LOG_ENDL;	
	
    m_Beepsetting->BeepSettingValue((acs::generated::fblock::HMI::CBeepSetting::TBeepSetting)beepSetting);
    return m_Beepsetting->UpdateShadows();
}

/*
 *******************************************************************
 * Function:SetVideoDefaultResultIBOX
 *
 * Description :Invoked by HMIAdaptor to Send VideoDefaultSetting
 *                to Ibox
 * Parameters :
 *  state         rw     holds the state of result value to be sent to ibox
 *  deviceID      rw     holds the destination deviceID to which result to be sent.
 *  senderHandle  rw     holds the senderHandle value of the request message.
 *
 * Returns : true if Status is called else false
 *
 *******************************************************************
 */
bool CHMIFBlockShadowControl::SetVideoDefaultResultIBOX(const uint state, const uint deviceID, const ushort senderHandle)
{
/*
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetVideoDefaultResultIBOX API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " DeviceID : " << deviceID << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SenderHandle : " << senderHandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " state : " << state << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " VideoDefaultSet_ResultAck --> : SenderHandle = " << senderHandle << MOST_LOG_ENDL;		

    bool retVal = false;
    switch(state)
    {
    case HMI_PROCESSING_ACK:
        retVal = m_VideoDefaultSetHU->ProcessingAck(deviceID,senderHandle);
        break;
    case HMI_RESULT_ACK:
        m_VideoDefaultSetHU->SenderHandleValue(senderHandle);
        retVal = m_VideoDefaultSetHU->ResultAck(deviceID);
        break;
    default:
        break;
    }

    return retVal;
}
/*
 *******************************************************************
 * Function:SetVideoDefaulttoIBOX
 *
 * Description :Invoked by HMIAdaptor to Set VideoDefaultSetting
 *                to Ibox
 * Parameters :
 *  sTarget       rw     holds the VideoDefaultSetting value for ibox
 *  senderHandle  rw     holds the senderHandle value of the request message.
 *
 * Returns : true if Status is called else false
 *
 *******************************************************************
 */
bool CHMIFBlockShadowControl::SetVideoDefaulttoIBOX(const uint senderHandle , ushort const sTarget)
{
/*
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SetVideoDefaulttoIBOX API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SenderHandle : " << senderHandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " sTarget : " << sTarget << MOST_LOG_ENDL;
*/  
// 2014.07.29, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " VideoDefaultSet_StartResultAck --> : SenderHandle = " << senderHandle
	<< " Target = " << sTarget << MOST_LOG_ENDL;		

    m_VideoDefaultSetIBOX->SenderHandleValue(senderHandle);
    m_VideoDefaultSetIBOX->VSetTargetValue((acs::generated::shadow::HMI::CVideoDefaultSet::TVSetTarget)sTarget);
    return m_VideoDefaultSetIBOX->StartResultAck();
}
/*
 *******************************************************************
 * Function:SetEmergencySetIBOX
 *
 * Description :Invoked by HMIAdaptor to send the status of  EmergencySet property
 *                to Ibox
 * Parameters :
 *  emgcState       rw     holds the emergency state value.
 *  controllerLock  rw     holds the lock/unlock value for the controller.
 *  soundLock       rw     holds the lock/unlock value for the sound.
 *
 * Returns : true if Status is called else false
 *
 *******************************************************************
 */
bool CHMIFBlockShadowControl::SetEmergencySetIBOX(const uint emgcState, const uint controllerLock, const uint soundLock)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetEmergencySetIBOX API is invoked " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " EMGCState : " << emgcState << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " controllerLock : " << controllerLock << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " soundLock : " << soundLock << MOST_LOG_ENDL;
*/

// 2014.11.28, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " EmergencySet_Status --> : EMGCState = " << emgcState <<
	" controllerLock = " << controllerLock << "soundLock = " << soundLock << MOST_LOG_ENDL;		

    m_Emergencyset->EMGCStateValue((acs::generated::fblock::HMI::CEmergencySet::TEMGCState)emgcState);
    m_Emergencyset->ControllerLockValue((acs::generated::fblock::HMI::CEmergencySet::TControllerLock)controllerLock);
    m_Emergencyset->SoundLockValue((acs::generated::fblock::HMI::CEmergencySet::TSoundLock)soundLock);
    return m_Emergencyset->UpdateShadows();
}
/*
 *******************************************************************
 * Function:SetPhonePriority
 *
 * Description :Invoked by NaviApp/VRApp to Set PhonePriority
 *                to Ibox
 * Parameters :
 *  phonePriority       rw     holds the phonePriority value for ibox(bluetooth/bluelink)
 *
 * Returns : true if Status is called else false
 *
 *******************************************************************
 */
bool CHMIFBlockShadowControl::SetPhonePriority(const uint phonePriority)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetPhonePriority API is invoked " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " phonePriority : " << phonePriority << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " PhonePriority_Status --> : phonePriority = " << phonePriority << MOST_LOG_ENDL;		
	
    m_PhonePriority->Value((acs::generated::fblock::HMI::CPhonePriority::TPhonePriority)phonePriority);
    return m_PhonePriority->UpdateShadows();
}


// Fcat v13.06.3
bool CHMIFBlockShadowControl::SetDisplayOSDStatus(const uint osdId, const uint osdStatus)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetDisplayOSDStatus API is invoked " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " OSD ID : " << osdId << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " OSD Status : " << osdStatus << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " DisplayOSD_Status --> : OsdID = " << osdId
	<< " OsdStatus = " << osdStatus << MOST_LOG_ENDL;		

    if(osdId > 0xFF)
    {
//        MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " OSD ID value is out of range" << MOST_LOG_ENDL;
        return false;
    }

    if( (osdStatus > 0x03) && (osdStatus != 0xFF) )
    {
//        MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " OSD Status value is out of range" << MOST_LOG_ENDL;
        return false;
    }

    if( (m_DisplayOSD->OsdIdValue((uint8_t)osdId) == true)
            && (m_DisplayOSD->OsdStatusValue((acs::generated::fblock::HMI::CDisplayOSD::TOsdStatus)osdStatus) == true)
            )
    {
        return m_DisplayOSD->UpdateShadows();
    }
    else
    {
//        MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Can NOT set the values" << MOST_LOG_ENDL;
        return false;
    }
}


// Fcat v13.07.1
bool CHMIFBlockShadowControl::SetSystemDefaultSet(const uint senderHandle)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetSystemDefaultSet API is invoked " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " SenderHandle : " << senderHandle << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " SystemDefaultSet_StartResultAck --> : SenderHandle = " 
	<< senderHandle << MOST_LOG_ENDL;		

    return m_SystemDefaultSet->StartResultAck(senderHandle);
}

// Fcat v13.07.2
bool CHMIFBlockShadowControl::SetMuteStatusStatus(const bool muteStatus)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetMuteStatusStatus API is invoked " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " Mute Status : " << muteStatus << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " MuteControl_Status --> : Mutestatus = " 
	<< muteStatus << MOST_LOG_ENDL;	

    m_MuteContorl->Value(muteStatus);

    return m_MuteContorl->UpdateShadows();
}

bool CHMIFBlockShadowControl::SetMuteStatusError(const uint errorCode)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SetMuteStatusError API is invoked " << MOST_LOG_ENDL;
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " Error Code : " << errorCode << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_HMI_CNT << " MuteControl_Error --> : errorcode = " 
	<< errorCode << MOST_LOG_ENDL;	

    return m_MuteContorl->Error(IBOX_DEVICE_ADDRESS, errorCode, 0, NULL);
}


/*
 *******************************************************************
 * Function:VideoDefaultSetHandlerHU
 *
 * Description :Invoked by Handler to Set VideoDefaultSetting
 *                in HU from Ibox
 * Parameters :
 *  sourceDeviceID       rw     holds the source device ID value which has requested
 *  payloadLength        rw     holds the length of payload data.
 *  payload              rw     holds the payload data to be set
 *  state                rw     holds the value about the type of request
 *
 * Returns : none
 *
 *******************************************************************
 */
void CHMIFBlockShadowControl::VideoDefaultSetHandlerHU(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " VideoDefaultSetHandlerHU signal got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " VideoDefaultSetHandlerHU : sourceDeviceID " << sourceDeviceID << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " VideoDefaultSetHandlerHU : state " << state << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
	MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " VideoDefaultSet_Set <-- : senderHandle = " << m_VideoDefaultSetHU->SenderHandleValue()
	<< " Target = " << m_VideoDefaultSetHU->VSetTargetValue() << MOST_LOG_ENDL;	
	
    switch(state)
    {
        case STATUS_STARTRESULTACK:
            m_VideoDefaultSetHU->ParseStartResultAck(payloadLength,payload);
//            MOST_LOG_INFO << LOG_TAG_HMI_CNT << " sTarget : " << m_VideoDefaultSetHU->VSetTargetValue() << MOST_LOG_ENDL;
//            MOST_LOG_INFO << LOG_TAG_HMI_CNT << " senderHandle : " << m_VideoDefaultSetHU->SenderHandleValue() << MOST_LOG_ENDL;
            emit VideoDefaultRequestIBOX(sourceDeviceID,
                                         m_VideoDefaultSetHU->SenderHandleValue(),
                                         m_VideoDefaultSetHU->VSetTargetValue());
            break;
        case STATUS_UNDEFINED:
            break;
        default:
            break;
    }
}
/*
 *******************************************************************
 * Function:VideoDefaultSetHandlerIBOX
 *
 * Description :Invoked by Handler for VideoDefaultSetting
 *                result from Ibox
 * Parameters :
 *  sourceDeviceID       rw     holds the source device ID value which has requested
 *  payloadLength        rw     holds the length of payload data.
 *  payload              rw     holds the payload data to be set
 *  state                rw     holds the value about the type of request
 *
 * Returns : none
 *
 *******************************************************************
 */
void CHMIFBlockShadowControl::VideoDefaultSetHandlerIBOX(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " VideoDefaultSetHandlerIBOX siganl got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " VideoDefaultSetHandlerIBOX : sourceDeviceID " << sourceDeviceID << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " VideoDefaultSetHandlerIBOX : state " << state << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
	MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " VideoDefaultSet_Status <-- : senderHandle = " 
	<< m_VideoDefaultSetIBOX->SenderHandleValue() << MOST_LOG_ENDL;	

    uint16_t senderHandle;

    switch(state)
    {
        case STATUS_PROCESSINGACK:
            m_VideoDefaultSetIBOX->ParseSenderHandle(payloadLength,payload,&senderHandle);
//            MOST_LOG_INFO << LOG_TAG_HMI_CNT << " senderHandle : " << senderHandle << MOST_LOG_ENDL;
            break;
        case STATUS_RESULTACK:
            m_VideoDefaultSetIBOX->ParseResultAck(payloadLength,payload);
//            MOST_LOG_INFO << LOG_TAG_HMI_CNT << " senderHandle : " << m_VideoDefaultSetIBOX->SenderHandleValue() << MOST_LOG_ENDL;
            break;
        default:
            break;
    }

    emit VideoDefaultResultIBOX(state,senderHandle);
}


void CHMIFBlockShadowControl::PopUpRequestHandlerHU(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uint state)
{
    switch(state)
    {
        case STATUS_ABORTACK:
            AbortPopUpRequestToHU(payloadLength,payload);
            break;

        case STATUS_STARTRESULTACK:
            PopUpRequestToHU(sourceDeviceID,payloadLength,payload);
            break;

        case STATUS_UNDEFINED:
            break;

        default:
            break;
    }
}

void CHMIFBlockShadowControl::PopUpRequestHandlerIBOX(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uint state)
{
    switch(state)
    {
        case STATUS_PROCESSINGACK:
            PopUpResultFromIBOX(sourceDeviceID,payloadLength,payload,HMI_PROCESSING_ACK);
            break;

        case STATUS_RESULTACK:
            PopUpResultFromIBOX(sourceDeviceID,payloadLength,payload,HMI_RESULT_ACK);
            break;

        case STATUS_UNDEFINED:
            break;

        default:
            break;
    }
}

void CHMIFBlockShadowControl::HMIFblockStateListener(const bool state)
{    
    emit HMIFunctionBlockVisible(state);
}


// Fcat v13.07.1
void CHMIFBlockShadowControl::SystemDefaultSetHandler(
    const DeviceID_t sourceDeviceID,
    const uint32_t payloadLength,
    const uint8_t *payload,
    const uint state)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " VideoDefaultSetHandlerIBOX siganl got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " VideoDefaultSetHandlerIBOX : sourceDeviceID " << sourceDeviceID << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " VideoDefaultSetHandlerIBOX : state " << state << MOST_LOG_ENDL;
*/
    uint16_t senderHandle;
    uint8_t errorCode = 0;
    QByteArray errorInfo;

    m_SystemDefaultSet->ParseSenderHandle(payloadLength, payload, &senderHandle);

// 2014.07.29, for log, hyunyong.sung
	MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SystemDefaultSet_ResultAck <-- : senderHandle = " 
	<< senderHandle << MOST_LOG_ENDL;	

//    MOST_LOG_INFO << LOG_TAG_HMI_CNT << " senderHandle : " << senderHandle << MOST_LOG_ENDL;

    if(state == METHOD_ERRORACK)
    {
        uint32_t errorInfoLength = 0;
        const uint8_t* errorInfoPayload = NULL;

        if(k2l::acs::samples::CMOSTErrorInfo::GetMOSTError(
                        m_HMIShadow->Container(),
                        errorCode,
                        errorInfoLength,
                        errorInfoPayload
                        ))
        {
/*        
            //Retrieved error information correctly.
            MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Error code " << errorCode << MOST_LOG_ENDL;
            MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Error Info Length " << errorInfoLength << MOST_LOG_ENDL;
            MOST_LOG_CRITICAL << LOG_TAG_HMI_CNT << " Error Info Payload " << errorInfoPayload << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
		MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " SystemDefaultSet_ErrorAck <-- : errorcode = " 
		<< errorCode << " errorinfo = " << errorInfoPayload << MOST_LOG_ENDL;	

            QByteArray error((const char *)errorInfoPayload, errorInfoLength);
            errorInfo = error;
        }
    }

    emit SystemDefaultSetResult(state, senderHandle, errorCode, errorInfo);
}


// Fcat v13.07.1
void CHMIFBlockShadowControl::MuteStatusListener()
{
    bool status = m_MuteContorl->Value();
/*
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " MuteStatusListener signal got called " << MOST_LOG_ENDL;
    MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " Mute Status : " << status << MOST_LOG_ENDL;
*/
// 2014.07.29, for log, hyunyong.sung
	MOST_LOG_SIGNAL << LOG_TAG_HMI_CNT << " MuteControl_Set <-- : mutestatus = " 
	<< status << MOST_LOG_ENDL;	

    emit MuteStatusSet(status);
}
