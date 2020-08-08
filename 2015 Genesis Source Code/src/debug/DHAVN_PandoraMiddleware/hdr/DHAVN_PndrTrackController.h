/*
 ******************************************************************
 *        COPYRIGHT © 2011, LG Electronics.  
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  Track Controller                                 *
 * PROGRAMMER :  Vandana Ahuja                                    *
 * DATE       :  03/12/2011                                       *
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
  *  03/12/2011  0.1 Vandana Ahuja   header file created
  *  16/12/2011  0.2 Wasim Shaikh    header file modified
  ******************************************************************
 */
 
#ifndef DHAVN_PNDR_TRACK_CONTROLLER_H
#define DHAVN_PNDR_TRACK_CONTROLLER_H

#include "DHAVN_LogUtil.h"

//Forward Declaration
class CPndrFrameEncoder;

class CPndrTrackController
{
    USE_LOG_UTIL;

public:

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
    CPndrTrackController();


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

    ~CPndrTrackController();

    void SetEncoder(CPndrFrameEncoder* inFrameEncoder);

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
     void Play();


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
     void Pause();


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
     void Skip();


private:    
    /** Instance of Encoder class */
    CPndrFrameEncoder* m_pFrameEncoder;

};

#endif //DHAVN_PNDR_TRACK_CONTROLLER_H