/*
 ******************************************************************
 *        COPYRIGHT © 2011, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  Track Controller                                 *
 * PROGRAMMER :  Wasim Shaikh                                     *
 * DATE       :  08/12/2011                                       *
 * VERSION    :  0.1                                              *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module provide all API for controlling the track
 *  This module provide Frame Encoder the required data and
 *  use Frame Validator for sending the data
 *
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
  *  03/12/2011  0.1   Wasim Shaikh  Implementation file created
  *
 ******************************************************************
 */

#include"DHAVN_PndrTrackController.h"
#include "DHAVN_PndrLinkConstants.h"
#include"DHAVN_PndrFrameEncoder.h"

/**
 *******************************************************************
 * Constructor:
 *  Creating instance of singleton class.
 *
 *
 *
 * @param      None
 *
 * @return     None
 *
 *******************************************************************
 */
CPndrTrackController::CPndrTrackController()
{
    LOG_INIT_V2("Pandora");
    //LOG_TRACE << "In CPndrTrackController::CPndrTrackController() " << LOG_ENDL;
    m_pFrameEncoder = NULL ; //CPndrFrameEncoder::GetInstance();
    //LOG_TRACE << "Exiting CPndrTrackController::CPndrTrackController() " << LOG_ENDL;
}
/**
 *******************************************************************
 * Destructor:
 *  Calling release to Free instance of Class.
 *
 *
 *
 *
 * @param      None
 *
 * @return     None
 *
 *******************************************************************
 */

CPndrTrackController::~CPndrTrackController()
{
    LOG_TRACE << "In CPndrTrackController::~CPndrTrackController() " << LOG_ENDL;
   // m_pFrameEncoder->ReleaseInstance();
    m_pFrameEncoder = NULL;
    LOG_TRACE << "Exiting CPndrTrackController::~CPndrTrackController() " << LOG_ENDL;
}

void CPndrTrackController::SetEncoder(CPndrFrameEncoder* inFrameEncoder)
{
    m_pFrameEncoder = inFrameEncoder;
}

/**
 *******************************************************************
 * Request for playing the current track when it was
 *              paused before
 *
 * @param None
 *
 * @return  None
 *
 *******************************************************************
 */
void CPndrTrackController::Play()
{
    //LOG_TRACE << "In CPndrTrackController::Play() " << LOG_ENDL;

    m_pFrameEncoder->EncodeAndSend(PNDR_EVENT_TRACK_PLAY);/** Send play request to Encoder*/

    //LOG_TRACE << "Exiting CPndrTrackController::Play() " << LOG_ENDL;
}

/**

 *******************************************************************
 * Request for pausing the current track when it was
 *              playing before
 *
 * @param None
 *
 * @return  None
 *
 *******************************************************************
 */
void CPndrTrackController::Pause()
{
   //LOG_TRACE << "In CPndrTrackController::Pause() " << LOG_ENDL;

    m_pFrameEncoder->EncodeAndSend(PNDR_EVENT_TRACK_PAUSE);/** Send Pause request to Encoder*/

    //LOG_TRACE << "Exiting CPndrTrackController::Pause() " << LOG_ENDL;

}

/**
 *******************************************************************
 * Skip the current song
 *
 * @param None
 *
 * @return  None
 *
 *******************************************************************
 */
void CPndrTrackController::Skip()
{
   // LOG_TRACE << "In CPndrTrackController::Skip() " << LOG_ENDL;

    m_pFrameEncoder->EncodeAndSend(PNDR_EVENT_TRACK_SKIP);  /** Send Skip request to Encoder*/

   // LOG_TRACE << "Exiting CPndrTrackController::Skip() " << LOG_ENDL;
}

//End Of File
