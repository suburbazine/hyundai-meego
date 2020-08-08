/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Neelima Khedad                                   *
 * DATE       :  1 Aug 2011                                       *
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
 *                  DLNA tuner Shadow control, responsible to     *
 *                  create instance of Shadow, configure it and   *
 *                  add to most stack. if any listeners will      *
 *                  be part of this class.Communicates with ACS   *
 *                  daemon.                                       *
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * 1 Aug 2011   Neelima Khedad           Draft version
 * 11 Aug 2011  Neelima Khedad           Added some more comments in the code for understanding
 * 15 Sep 2011  Amit Koparde             Added Debug Statement   *
 * 12 Oct 2011  Neelima Khedad           Added API SetTrackposition
 * 08 Feb 2011  Neelima Khedad           Using the latest Version of LOG_INIT function Logger module
 * 21 Jun 2012  Shiva Kumar              Modified for FCAT 1.17.1, FCAT 1.17.2 and FCAT 1.17.3
 * 26 Jul 2012  Shiva Kumar              Modified for CR 12357
 ******************************************************************
 */

#include "DHAVN_MOSTManager_DLNAShadowControl.h"
#include "DHAVN_MOSTManager_MOSTStructures.h"



/*
 *******************************************************************
 * Function:CDLNAShadowControl
 *
 * Description :This is constructor of CDLNAShadowControl class.
 *              creates the instances for all the defined parameters
 * Parameters :
 *    context    rw   It holds the instance of CMarshaledACSApplication.
 *
 * Returns :
 *
 *******************************************************************
 */

CDLNAShadowControl ::CDLNAShadowControl(CMarshaledACSApplication *context)
    : CControlThreadBase(context, LOG_TAG_DLNA_CNT)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)


    /** Creating the Function Block instance for AmFmTUner with instance ID 0x01*/
    m_DLNAShadow = new CDLNA(0x01);
    m_pMOSTElement = m_DLNAShadow;

    /** Creating the instance for for DLNA properties and the Listeners */
    m_TimePostion = new CTimePosition(m_DLNAShadow);
    m_TimePostionListener = new CTimePostionListener(this);

    m_DLNAConnection = new CDLNAConnection(m_DLNAShadow);
    m_DLNAConnectionListener = new CDLNAConnectionListener(this);

    m_CurrentSongInfo = new CCurrentSongInfo(m_DLNAShadow);
    m_CurrentSongInfoListener = new CCurrentSongInfoListener(this);

    m_CurrentVideoInfo = new CCurrentVideoInfo(m_DLNAShadow);
    m_CurrentVideoInfoListener = new CCurrentVideoInfoListener(this);

    m_DeckStatus = new CDeckStatus(m_DLNAShadow);
    m_DeckStatusListener = new CDeckStatusListener(this);

    m_TrackPosition = new CTrackPosition(m_DLNAShadow);
    m_TrackPositionListener = new CTrackPositionListener(this);

    m_TrackInformation = new CTrackInformation(m_DLNAShadow);
    m_TrackInformationListener = new CTrackInformationListener(this);

}

/*
 *******************************************************************
 * Function:~CDLNAShadowControl
 *
 * Description :Is a Destructor,removes all the registered listeners.
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */

CDLNAShadowControl ::~CDLNAShadowControl()
{
    RemoveListeners();   
    delete m_TimePostionListener;
    delete m_TimePostion;
    delete m_DLNAConnection;
    delete m_DLNAConnectionListener;
    delete m_DLNAShadow;  
    delete m_CurrentSongInfoListener;
    delete m_CurrentSongInfo;
    delete m_CurrentVideoInfoListener;
    delete m_CurrentVideoInfo;
    delete m_DeckStatus;
    delete m_DeckStatusListener;
    delete m_TrackPosition;
    delete m_TrackPositionListener;
    delete m_TrackInformation;
    delete m_TrackInformationListener;
 
}

/*
 *******************************************************************
 * Function:Initialize
 *
 * Description :Function initializes the MOST elements to DLNA
 *             Shadow and adds to MOST stack.
 *
 * Parameters :
 *
 *
 * Returns :true or false
 *
 *******************************************************************
 */

bool CDLNAShadowControl::Initialize()
{
    /** Initialize the properties of DLNA and set the notification for properties required */
    if(m_DLNAShadow->InitializeMOSTElement())
    {
        SetListeners();
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
void CDLNAShadowControl::SetListeners()
{
    m_TimePostion->SetListener(m_TimePostionListener);   
    m_DLNAConnection->SetListener(m_DLNAConnectionListener);  
    m_CurrentSongInfo->SetListener(m_CurrentSongInfoListener);
    m_CurrentVideoInfo->SetListener(m_CurrentVideoInfoListener);
    m_DeckStatus->SetListener(m_DeckStatusListener);
    m_TrackPosition->SetListener(m_TrackPositionListener);
    m_TrackInformation->SetListener(m_TrackInformationListener);
 
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

void CDLNAShadowControl::RemoveListeners()
{
    m_TimePostion->RemoveListener(m_TimePostionListener);   
    m_DLNAConnection->RemoveListener(m_DLNAConnectionListener); 
    m_CurrentSongInfo->RemoveListener(m_CurrentSongInfoListener);
    m_CurrentVideoInfo->RemoveListener(m_CurrentVideoInfoListener);
    m_DeckStatus->RemoveListener(m_DeckStatusListener);
    m_TrackPosition->RemoveListener(m_TrackPositionListener);
    m_TrackInformation->RemoveListener(m_TrackInformationListener);
}


bool CDLNAShadowControl::FBlockVisible()
{
    return m_DLNAShadow->FunctionBlockVisible();
}
/*
 *******************************************************************
 * Function:TimePositionListener
 *
 * Description : Sends the signal for time position values to Adaptor class
 *
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CDLNAShadowControl::TimePositionListener()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_DLNA_CNT << " TimePositionListener Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " TimePosition:Disk Time = " << m_TimePostion->DataDiskTimeValue() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " TimePosition:Track Time = " << m_TimePostion->DataTrackTimeValue() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " TimePosition:Title Time = " <<  m_TimePostion->DataTitleTimeValue() << MOST_LOG_ENDL;
*/
// 2014.07.22, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_DLNA_CNT << " TimePosition_Status <-- :  DiskTime = " << m_TimePostion->DataDiskTimeValue()
    << " TrackTime = " << m_TimePostion->DataTrackTimeValue() << " TitleTime = " << m_TimePostion->DataTitleTimeValue() << MOST_LOG_ENDL;

    emit TimePosition(m_TimePostion->DataDiskTimeValue(),
                      m_TimePostion->DataTrackTimeValue(),
                      m_TimePostion->DataTitleTimeValue());
}

/*
 *******************************************************************
 * Function:DLNAConnStatusListener
 *
 * Description : Sends the signal for DLNA connection to Adaptor class
 *
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CDLNAShadowControl::DLNAConnStatusListener()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_DLNA_CNT << " DLNAConnStatusListener Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " DLNAConnStatus:Status = " << m_DLNAConnection->Value() << MOST_LOG_ENDL;
*/
// 2014.07.22, for log, hyunyong.sung
	MOST_LOG_SIGNAL << LOG_TAG_DLNA_CNT << " DLNAConnection_Status <-- : DLNAStauts = " 
	<< m_DLNAConnection->Value() << MOST_LOG_ENDL;

    emit DLNAConnStatus(m_DLNAConnection->Value());
}

/*
 *******************************************************************
 * Function:CurrentSongInfoListener
 *
 * Description : Sends the signal for Current song information to Adaptor class
 *
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CDLNAShadowControl::CurrentSongInfoListener()
{
    const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding = NULL;
    uint8_t* pSongInfoString = NULL;
    uint32_t songInfoStringLength = 0;
    QStringList songinfo;
    /**
     songinfo[0] = title;
     songinfo[1] = filename;
     songinfo[2] = artist;
     songinfo[3] = album;
     songinfo[4] = genre;
    */
    /** Title String */
    m_CurrentSongInfo->TitleString(
        &pStringEncoding,
        &pSongInfoString,
        &songInfoStringLength
        );
    songinfo.append(QString::fromUtf8((const char *)pSongInfoString,songInfoStringLength));


    /** FileName String */
    m_CurrentSongInfo->FileNameString(
        &pStringEncoding,
        &pSongInfoString,
        &songInfoStringLength
        );
    songinfo.append(QString::fromUtf8((const char *)pSongInfoString,songInfoStringLength));

    /** Artist String */
    m_CurrentSongInfo->ArtistString(
        &pStringEncoding,
        &pSongInfoString,
        &songInfoStringLength
        );
    songinfo.append(QString::fromUtf8((const char *)pSongInfoString,songInfoStringLength));

    /** Album String */
    m_CurrentSongInfo->AlbumString(
        &pStringEncoding,
        &pSongInfoString,
        &songInfoStringLength
        );
    songinfo.append(QString::fromUtf8((const char *)pSongInfoString,songInfoStringLength));

    /** Genre String */
    m_CurrentSongInfo->GenreString(
        &pStringEncoding,
        &pSongInfoString,
        &songInfoStringLength
        );
    songinfo.append(QString::fromUtf8((const char *)pSongInfoString,songInfoStringLength));

/*
    MOST_LOG_SIGNAL << LOG_TAG_DLNA_CNT << " CurrentSongInfoListener Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " CurrentSongInfo:Play Time = " << m_CurrentSongInfo->PlayTimeValue() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " CurrentSongInfo:Track Number = " << m_CurrentSongInfo->TrackNoValue() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " CurrentSongInfo:Title Name = " << songinfo.at(0) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " CurrentSongInfo:File Name = " << songinfo.at(1) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " CurrentSongInfo:Artist = " << songinfo.at(2) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " CurrentSongInfo:Album = " << songinfo.at(3) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " CurrentSongInfo:Genre = " << songinfo.at(4) << MOST_LOG_ENDL;
*/
// 2014.07.22, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_DLNA_CNT << " CurrentSongInfo_Status <-- : PlayTime = " << m_CurrentSongInfo->PlayTimeValue()
	<< " TrackNumber = " << m_CurrentSongInfo->TrackNoValue() << " Title = " << songinfo.at(0) << " FileName = " << songinfo.at(1)
	<< " Artist = " << songinfo.at(2) << " Album = " << songinfo.at(3) << " Genre = " << songinfo.at(4) << MOST_LOG_ENDL;

     /** Emit the signal to adptor for the info with required format in Qt */
    emit CurrentSongInfo(m_CurrentSongInfo->PlayTimeValue(),
                         m_CurrentSongInfo->TrackNoValue(),songinfo);
}

/*
 *******************************************************************
 * Function:CurrentVideoInfoListener
 *
 * Description : Sends the signal for Current video information to Adaptor class
 *
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CDLNAShadowControl::CurrentVideoInfoListener()
{
    const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding = NULL;
    uint8_t* pVideoTitleString = NULL;
    uint32_t videoTitleStringLength = 0;
    uint8_t* pVideoFilenameString = NULL;
    uint32_t videoFileanameStringLength = 0;

    /** Video title info */
    m_CurrentVideoInfo->VideoTitleString(
        &pStringEncoding,
        &pVideoTitleString,
        &videoTitleStringLength
        );

    /** Video Fielname Info */
    m_CurrentVideoInfo->VideoFilenameString(
        &pStringEncoding,
        &pVideoFilenameString,
        &videoFileanameStringLength
        );
/*
    MOST_LOG_SIGNAL << LOG_TAG_DLNA_CNT << " CurrentVideoInfoListener Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " CurrentVideoInfo:vTitle = " << reinterpret_cast<const char*>(pVideoTitleString) << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " CurrentVideoInfo:Time = " << m_CurrentVideoInfo->VideoTimeValue() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " CurrentVideoInfo:Track = " << m_CurrentVideoInfo->VideoTrackValue() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " CurrentVideoInfo:vFilename = " << reinterpret_cast<const char*>(pVideoFilenameString) << MOST_LOG_ENDL;
*/
// 2014.07.22, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_DLNA_CNT << " CurrentVideoInfo_Status <-- : Title = " << reinterpret_cast<const char*>(pVideoTitleString)
	<< "VedeoTime = " << m_CurrentVideoInfo->VideoTimeValue() << " Track = " << m_CurrentVideoInfo->VideoTrackValue()
	<< " FileName = " << reinterpret_cast<const char*>(pVideoFilenameString) << MOST_LOG_ENDL;

    /** Emit the signal to adptor for the info with required format in Qt */
    emit CurrentVideoInfo(reinterpret_cast<const char*>(pVideoTitleString),
                          m_CurrentVideoInfo->VideoTimeValue(),
                          m_CurrentVideoInfo->VideoTrackValue(),
                          reinterpret_cast<const char*>(pVideoFilenameString));
}


/*
 *******************************************************************
 * Function:CTimePostionListener
 *
 * Description :  Constructor
 *
 * Parameters :
 *  context   rw   Context of CDLNAShadowControl
 * Returns :        void
 *
 *******************************************************************
 */

CTimePostionListener::CTimePostionListener(CDLNAShadowControl *context)
{
    m_CDLNAShadowControl = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description :  Called when any property of TIme Position is changed.
 *
 * Parameters :     None
 *
 * Returns :        void
 *
 *******************************************************************
 */

void CTimePostionListener::OnChange()
{

    m_CDLNAShadowControl->TimePositionListener();
}

/*
 *******************************************************************
 * Function:CDLNAConnectionListener
 *
 * Description :  Constructor
 *
 * Parameters :
 *  context   rw   Context of CDLNAShadowControl
 * Returns :        void
 *
 *******************************************************************
 */

CDLNAConnectionListener::CDLNAConnectionListener(CDLNAShadowControl *context)
{
    m_CDLNAShadowControl = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description :  Called when  property of DLNA Connection status is changed.
 *
 * Parameters :     None
 *
 * Returns :        void
 *
 *******************************************************************
 */

void CDLNAConnectionListener::OnChange()
{
    m_CDLNAShadowControl->DLNAConnStatusListener();
}

/*
 *******************************************************************
 * Function:CCurrentSongInfoListener
 *
 * Description :  Constructor
 *
 * Parameters :
 *  context   rw   Context of CDLNAShadowControl
 * Returns :        void
 *
 *******************************************************************
 */

CCurrentSongInfoListener::CCurrentSongInfoListener(CDLNAShadowControl *context)
{
    m_CDLNAShadowControl = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description :  Called when any property of Current song information is changed.
 *
 * Parameters :     None
 *
 * Returns :        void
 *
 *******************************************************************
 */
void CCurrentSongInfoListener::OnChange()
{
    m_CDLNAShadowControl->CurrentSongInfoListener();
}

/*
 *******************************************************************
 * Function:CCurrentVideoInfoListener
 *
 * Description :  Constructor
 *
 * Parameters :
 *  context   rw   Context of CDLNAShadowControl
 * Returns :        void
 *
 *******************************************************************
 */

CCurrentVideoInfoListener::CCurrentVideoInfoListener(CDLNAShadowControl *context)
{
    m_CDLNAShadowControl = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description :  Called when any property of Current Video information is changed.
 *
 * Parameters :     None
 *
 * Returns :        void
 *
 *******************************************************************
 */

void CCurrentVideoInfoListener::OnChange()
{
    m_CDLNAShadowControl->CurrentVideoInfoListener();
}
/*
 *******************************************************************
 * Function:CDeckStatusListener
 *
 * Description :  Constructor
 *
 * Parameters :
 *  context   rw   Context of CDLNAShadowControl
 * Returns :        void
 *
 *******************************************************************
 */
CDeckStatusListener::CDeckStatusListener(CDLNAShadowControl* context)
{
    m_CDLNAShadowControl = context;
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description :  Called when any property of DeckStatus information is changed.
 *
 * Parameters :     None
 *
 * Returns :        void
 *
 *******************************************************************
 */
void CDeckStatusListener::OnChange()
{
    m_CDLNAShadowControl->DeckStatusListener();
}
/*
 *******************************************************************
 * Function:CTrackPositionListener
 *
 * Description :  Constructor
 *
 * Parameters :
 *  context   rw   Context of CDLNAShadowControl
 * Returns :        void
 *
 *******************************************************************
 */
CTrackPositionListener::CTrackPositionListener(CDLNAShadowControl *context)
{
    m_CDLNAShadowControl = context;
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description :  Called when any property of TrackPosition information is changed.
 *
 * Parameters :     None
 *
 * Returns :        void
 *
 *******************************************************************
 */
void CTrackPositionListener::OnChange()
{
    m_CDLNAShadowControl->TrackPositionListener();
}
/*
 *******************************************************************
 * Function:CTrackInformationListener
 *
 * Description :  Constructor
 *
 * Parameters :
 *  context   rw   Context of CDLNAShadowControl
 * Returns :        void
 *
 *******************************************************************
 */
CTrackInformationListener::CTrackInformationListener(CDLNAShadowControl* context)
{
    m_CDLNAShadowControl = context;
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description :  Called when any property of TrackInformation is changed.
 *
 * Parameters :     None
 *
 * Returns :        void
 *
 *******************************************************************
 */
void CTrackInformationListener::OnChange()
{
    m_CDLNAShadowControl->TrackInformationListener();
}

/*
 *******************************************************************
 * Function:DeckStatusListener
 *
 * Description : Sends the signal for DeckStatus to Adaptor class
 *
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CDLNAShadowControl::DeckStatusListener()
{
/*
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " DeckStatus Slot invoked " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " DeckStatus:: " << m_DeckStatus->DeckStatusValue() << MOST_LOG_ENDL;
*/
// 2014.07.22, for log, hyunyong.sung
	MOST_LOG_SIGNAL << LOG_TAG_DLNA_CNT << " DeckStatus_Status <-- : DeckStatus = " << m_DeckStatus->DeckStatusValue()
	<< MOST_LOG_ENDL;

    emit Deckstatus(m_DeckStatus->DeckStatusValue());
}
/*
 *******************************************************************
 * Function:TrackPositionListener
 *
 * Description : Sends the signal for TrackPosition information to Adaptor class
 *
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CDLNAShadowControl::TrackPositionListener()
{
/*
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " TrackPosition Slot invoked " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " TrackPosition:: " << m_TrackPosition->Value() << MOST_LOG_ENDL;
*/
// 2014.07.22, for log, hyunyong.sung
	MOST_LOG_SIGNAL << LOG_TAG_DLNA_CNT << " TrackPosition_Status <--  : DeckStatus = " << m_TrackPosition->Value()
	<< MOST_LOG_ENDL;

    emit Deckstatus(m_TrackPosition->Value());
}
/*
 *******************************************************************
 * Function:TrackInformationListener
 *
 * Description : Sends the signal for TrackInformation to Adaptor class
 *
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CDLNAShadowControl::TrackInformationListener()
{
/*
2    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " TrackInformation Slot invoked " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " TrackInformation::CurrentNumberTracks " << m_TrackInformation->CurrentNumberTracksValue() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " TrackInformation::CurrentRelativeTrack " << m_TrackInformation->CurrentRelativeTrackPositionValue() << MOST_LOG_ENDL;
*/
// 2014.07.22, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_DLNA_CNT << " TrackInformation_Status <--  : CurrentNumberTracks = "
    << m_TrackInformation->CurrentNumberTracksValue() << "CurrentRelativeTrack = " 
    << m_TrackInformation->CurrentRelativeTrackPositionValue() << MOST_LOG_ENDL;

    emit TrackInformation(m_TrackInformation->CurrentNumberTracksValue(),
                    m_TrackInformation->CurrentRelativeTrackPositionValue());
}

/*
 *******************************************************************
 * Function:SetDeckStatus
 *
 * Description : Sets deck status value to DLNA FBlock in IBOX
 *
 * Parameters :
 *    deckStatus  rw contains deckstatus information set by user
 *
 * Returns :true or false
 *
 *******************************************************************
 */
bool CDLNAShadowControl::SetDeckStatus(const uint deckStatus)
{
/*
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " Set DeckStatus API called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " DeckStatus:: " << deckStatus << MOST_LOG_ENDL;
*/
// 2014.07.22, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_DLNA_CNT << " DeckStatus_Set --> : DeckStatus = " << deckStatus << MOST_LOG_ENDL;

    m_DeckStatus->DeckStatusValue((acs::generated::shadow::DLNA::CDeckStatus::TDeckStatus)deckStatus);
    return m_DeckStatus->Set();
}
/*
 *******************************************************************
 * Function:SetTrackPosition
 *
 * Description : Sets track position information to DLNA Fblock in IBOX
 *
 * Parameters :
 *   trackPosition rw   contains trackposition information for DLNA
 *
 * Returns :true or false
 *
 *******************************************************************
 */
bool CDLNAShadowControl::SetTrackPosition(const uint trackPosition)
{
/*
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " Set TrackPosition API called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " TrackPosition:: " << trackPosition << MOST_LOG_ENDL;
*/
// 2014.07.22, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_DLNA_CNT << " TrackPosition_Set --> : TrackPosition = " << trackPosition << MOST_LOG_ENDL;

    return m_TrackPosition->Set(trackPosition);
}
/*
 *******************************************************************
 * Function:SetIncrementTrackPosition
 *
 * Description : Increments track position information to DLNA Fblock in IBOX
 *
 * Parameters :
 *   nSteps rw   contains trackposition information to be incremented for DLNA
 *
 * Returns :true or false
 *
 *******************************************************************
 */
bool CDLNAShadowControl::SetIncrementTrackPosition(const uint nSteps)
{
/*
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " SetIncrementTrackPosition API called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " TrackPosition:: nSteps " << nSteps << MOST_LOG_ENDL;
*/
// 2014.07.22, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_DLNA_CNT << " IncrementTrackPosition_Set --> : nSteps = " << nSteps << MOST_LOG_ENDL;

    return m_TrackPosition->Increment(nSteps);
}
/*
 *******************************************************************
 * Function:SetDecrementTrackPosition
 *
 * Description : Decrements track position information to DLNA Fblock in IBOX
 *
 * Parameters :
 *   nSteps rw   contains trackposition information to be decremented for DLNA
 *
 * Returns :true or false
 *
 *******************************************************************
 */
bool CDLNAShadowControl::SetDecrementTrackPosition(const uint nSteps)
{
/*
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " SetDecrementTrackPosition API called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DLNA_CNT << " TrackPosition:: nSteps " << nSteps << MOST_LOG_ENDL;
*/
// 2014.07.22, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_DLNA_CNT << " DecrementTrackPosition_Set --> : nSteps = " << nSteps << MOST_LOG_ENDL;

    return m_TrackPosition->Decrement(nSteps);
}
