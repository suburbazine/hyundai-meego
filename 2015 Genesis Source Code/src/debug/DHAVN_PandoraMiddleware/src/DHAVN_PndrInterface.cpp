/*
 ******************************************************************
 *        COPYRIGHT  2011, LG Electronics.
 *        All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :    Pandora Interface                              *
 * PROGRAMMER :    Snigdha Satapathy                               *
 * DATE       :    02/12/2011                                     *
 * VERSION    :    0.1                                            *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *        This module provides the interface to be used by
 *        Pandora application
 *        This is a very thin layer, which just check for the
 *        validity of input and check if requested operation is
 *        possible in given state or not.
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS
 * 1) 07/12/2011    0.1 Snigdha Satapathy   cpp file created
 * 2) 09/12/2011    0.2 Wasim Shaikh   Implementation file created
 *
 ******************************************************************
 */

#include "DHAVN_PndrInterface.h"
#include "DHAVN_PndrStateHandler.h"
#include "DHAVN_Pndr_Internal_Defs.h"
#include <QString>
#define SIZE_OF_SEARCH_REQ_STRNG 248

/**
 *******************************************************************
 * Constructor:
 *  Instanciating CPndrStateHandler Class
 *
 * @param      None
 *
 * @return     None
 *
 *******************************************************************
 */
CPndrInterface::CPndrInterface()
{
    LOG_TRACE << "In CPndrInterface::CPndrInterface()" << LOG_ENDL;
    LOG_INIT_V2("Pandora");
    LOG_TRACE << "Exiting CPndrInterface::CPndrInterface()" << LOG_ENDL;

}

/**
 *******************************************************************
 *  Init Function for memory allocation for statehandler object
 *
 *
 * @param      None
 *
 * @return     EPndrError: Its Value can be as follow
 *                         E_SUCCESS on Successful Memory Allocation
 *                         E_MEMORY on UnSuccessful Memory Alocation
 *
 *******************************************************************
 */

EPndrError CPndrInterface::Init()
{
    LOG_TRACE << "Inside CPndrInterface::Init()" << LOG_ENDL;
    return m_CPndrStateHandler.Init();
}

/**
 *******************************************************************
 *  DeInit Function for releasing memory allocated by statehandler object
 *
 *
 * @param      None
 *
 * @return     None
 *
 *******************************************************************
 */

void CPndrInterface::DeInit()
{
    LOG_TRACE << "In CPndrInterface::DeInit():Entry" << LOG_ENDL;
    m_CPndrStateHandler.DeInit();
    LOG_TRACE << "In CPndrInterface::DeInit():Exit" << LOG_ENDL;
}

/**
 *******************************************************************
 * Destructor:
 *  Setting Member variable to NULL.

 * @param      None
 *
 * @return     None
 *
 *******************************************************************
 */

CPndrInterface::~CPndrInterface()
{
    LOG_TRACE << "In CPndrInterface::~CPndrInterface()" << LOG_ENDL;
   
    LOG_TRACE << "Exiting CPndrInterface::~CPndrInterface()" << LOG_ENDL;
}
/**
 *******************************************************************
 * This function will connect to device and initiate the pandora
 * connection
 *
 *    TODO: Currently its not possible to get the device ID/name
 *            by which it can be identified that which device to
 *            connect if there are more than one device is
 *            connected through usb
 *
 * @param[in]    inDeviceType: This can be iPhone or Smart Phone
 *                    By default if iPhone is EDeviceType then USB
 *                    will be tried for connection & if smart device
 *                    is used as EDeviceType then BT will be tried
 *                    for connection.
 *                  inAlbumArtDimension: The expected size of album art for
 *                      each music
 *                  inAlbumArtImgType: The expected image type of album
 *                      art for each music, if its value is IMG_TYPE_NONE,
 *                      then default value as IMG_TYPE_JPEG will be used.
 *                  inStationArtDimension size of station art or genre station
 *                       art image
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is already connected
 *                                    the function has been called
 *                                    incorrectly
 *
 * @see:    Result
 *******************************************************************
 */
EPndrError CPndrInterface::Connect(const EDeviceType inDeviceType,
                                   const quint16 inAlbumArtDimension,
                                   const EImageType inAlbumArtImgType,
                                   const quint16 inStationArtDimension)
{
    LOG_TRACE << "In CPndrInterface::Connect()" << LOG_ENDL;


    LOG_INFO << "Parameter Passing to Connect are  " << "inDeviceType = " <<
                inDeviceType << "inAlbumArtDimension = "  << inAlbumArtDimension <<
                "inAlbumArtImgType = " << inAlbumArtImgType <<
                "inStationArtDimension = "<< inStationArtDimension << LOG_ENDL;


    EPndrError retErrorCode = E_INVALID_ARGUMENT;

    if((inDeviceType > DEVICE_TYPE_NONE) && (inDeviceType < DEVICE_TYPE_MAX ))
    {
        retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_CONNECT);
        if(retErrorCode == E_SUCCESS)
            retErrorCode = m_CPndrStateHandler.Connect(inDeviceType,inAlbumArtDimension,
                                                       inAlbumArtImgType,inStationArtDimension);
        else
            LOG_CRITICAL << "Connect request Declined. Error : "
                         << retErrorCode << LOG_ENDL;
    }
    LOG_TRACE << "Exiting CPndrInterface::Connect() with Error code = "
              << retErrorCode << LOG_ENDL;

    return retErrorCode;

}

/**
 *******************************************************************
 * Disconnect any existing connection
 *
 * @param None
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is not connected
 *
 * @see:    Result
 *******************************************************************
 */
EPndrError CPndrInterface::Disconnect()
{
    LOG_TRACE << "In CPndrInterface::Disconnect()" << LOG_ENDL;

    EPndrError retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_DISCONNECT);

    if(retErrorCode == E_SUCCESS)
        m_CPndrStateHandler.Disconnect();

    else
        LOG_CRITICAL << "Disconnect request Declined. Error : "
                     << retErrorCode << LOG_ENDL;

    LOG_TRACE << "Exiting CPndrInterface::Disconnect() with Error code = "
              << retErrorCode << LOG_ENDL;

    return retErrorCode;

}

//{ added by esjang 2013.05.15 for certification issue, sessionterminate.

/**
 *******************************************************************
 * SessionTerminate any session
 *
 * @param None
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is not connected
 *
 * @see:    Result
 *******************************************************************
 */
void CPndrInterface::SessionTerminate()
{
    LOG_TRACE << "In CPndrInterface::SessionTerminate()" << LOG_ENDL;

    //EPndrError retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_DISCONNECT);
      m_CPndrStateHandler.SessionTerminate();


}
//} added by esjang 2013.05.15 for certification issue, sessionterminate.


//{ added by esjang 2013.05.15 for certification issue, sessionterminate.

/**
 *******************************************************************
 * Request TrackStatus any session
 *
 * @param None
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is not connected
 *
 * @see:    Result
 *******************************************************************
 */
 /*
void CPndrInterface::RequestPandoraStatus()
{
    LOG_TRACE << "esjang 0514 In CPndrInterface::RequestGetTrackStatus()" << LOG_ENDL;

    //EPndrError retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_DISCONNECT);
      m_CPndrStateHandler.RequestPandoraStatus();


}
*/
//} added by esjang 2013.05.15 for certification issue, sessionterminate.

/**
 *******************************************************************
 * This function will will provide the Information
 *                about current track being played/paused
 *
 * @param None
 *
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is Not connected OR
 *                                    No track is played/paused
 *
 * @see:    TrackInfo
 *******************************************************************
 */
EPndrError CPndrInterface::RequestTrackInfo()
{
    LOG_TRACE << "In CPndrInterface::RequestTrackInfo()" << LOG_ENDL;

    EPndrError retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_TRACK_INFO);

    if(retErrorCode == E_SUCCESS)
        m_CPndrStateHandler.RequestTrackInfo();

    else
        LOG_CRITICAL << "RequestTrackInfo request Declined. Error : "
                     << retErrorCode << LOG_ENDL;

    LOG_TRACE << "Exiting CPndrInterface::RequestTrackInfo() with Error code = "
              << retErrorCode << LOG_ENDL;

    return retErrorCode;

}



/**
 *******************************************************************
 * Request for explanation for currently played/paused
 *              song
 *
 * @param None
 *
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is Not connected OR
 *                                    No track is played/paused
 *
 * @see:    TrackExplain
 *******************************************************************
 */
EPndrError CPndrInterface::RequestExplanation()
{
    LOG_TRACE << "In CPndrInterface::RequestExplanation()" << LOG_ENDL;

    EPndrError retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_EXPLAIN);

    if(retErrorCode == E_SUCCESS)
        m_CPndrStateHandler.RequestExplanation();

    else
        LOG_CRITICAL << "RequestExplanation request Declined. Error : "
                     << retErrorCode << LOG_ENDL;

    LOG_TRACE << "Exiting CPndrInterface::RequestExplanation() with Error code = "
              << retErrorCode << LOG_ENDL;

    return retErrorCode;
}

/**
 *******************************************************************
 * Request to Play the track. This will be possible ONLY
 *                if track was paused before
 *
 * @param None
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is Not connected OR
 *                                    Track was not paused before
 *
 * @see:    Result
 *******************************************************************
 */
EPndrError CPndrInterface::Play()
{
    LOG_TRACE << "In CPndrInterface::Play()" << LOG_ENDL;

    EPndrError retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_PLAY);

    if(retErrorCode == E_SUCCESS)
        m_CPndrStateHandler.Play();

    else
        LOG_CRITICAL << "Play request Declined. Error : "
                     << retErrorCode << LOG_ENDL;

    LOG_TRACE << "Exiting CPndrInterface::Play() with Error code = "
              << retErrorCode << LOG_ENDL;
    return retErrorCode;
}

/**
 *******************************************************************
 * Request to Pause the current track being played.
 *
 * @param None
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is Not connected OR
 *                                    No track playing to be paused
 *
 * @see:    Result
 *******************************************************************
 */
EPndrError CPndrInterface:: Pause()
{
    LOG_TRACE << "In CPndrInterface::Pause()" << LOG_ENDL;

    EPndrError retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_PAUSE);

    if(retErrorCode == E_SUCCESS)
        m_CPndrStateHandler.Pause();

    else
        LOG_CRITICAL << "Pause request Declined. Error : "
                     << retErrorCode << LOG_ENDL;

    LOG_TRACE << "Exiting CPndrInterface::Pause() with Error code = "
              << retErrorCode << LOG_ENDL;
    return retErrorCode;
}

/**
 *******************************************************************
 * Request to Skip the song
 *
 * @param None
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is Not connected OR
 *                                    No track is in Play/Pause status
 *
 * @see:    TrackUpdated
 *******************************************************************
 */
EPndrError CPndrInterface::Skip()
{
    LOG_TRACE << "In CPndrInterface::Skip()" << LOG_ENDL;

    EPndrError retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_SKIP);

    if(retErrorCode == E_SUCCESS)
        m_CPndrStateHandler.Skip();

    else
        LOG_CRITICAL << "Skip request Declined. Error : "
                     << retErrorCode << LOG_ENDL;

    LOG_TRACE << "Exiting CPndrInterface::Skip() with Error code = "
              << retErrorCode << LOG_ENDL;
    return retErrorCode;
}

//{ modified by yongkyun.lee 2014-08-25 for : Branding Image
EPndrError CPndrInterface::RequestBrandingImage(QString BIPath ,QString BIName )
{
    LOG_HIGH<<"CPndrInterface::RequestBrandingImage" <<LOG_ENDL;
    m_CPndrStateHandler.RequestBrandingImage( BIPath , BIName);
    return E_SUCCESS;    
}

EPndrError CPndrInterface::RequestBrandingSegment( const quint8 SegIndex,const quint8 totalSeg, quint8* inData, int fzise )
{
    LOG_TRACE<<"CPndrInterface::RequestBrandingSegment" <<LOG_ENDL;
    EPndrError retErrorCode = E_INVALID_TOKEN;
    
    retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_BRANDING_SEGMENT);

    if(retErrorCode == E_SUCCESS)
    {//RequestBrandingSegment(QByteArray* inData)
       m_CPndrStateHandler.RequestBrandingSegment(  SegIndex, totalSeg,  inData, fzise);
    }
    else
    {
        LOG_CRITICAL << "RequestBrandingSegment request Declined. Error : "
                     << retErrorCode << LOG_ENDL;
    }
    
    LOG_TRACE << "Exiting RequestBrandingSegment::RequestBrandingImage() with Error code = "
              << retErrorCode << LOG_ENDL;
    
    return retErrorCode;
}
//} modified by yongkyun.lee 2014-08-04 

/**
 *******************************************************************
 * Request to Rate the song
 *
 * @param[in]    inRate    The Rating that user would like to give to
 *                        current track
 *                        1:    ThumbUp
 *                        2:    ThumbDown
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is Not connected OR
 *                                    No track is in Play/Pause status
 *
 * @see:    Result
 *******************************************************************
 */
EPndrError CPndrInterface::Rate(const quint8 inRate)
{
    LOG_TRACE << "In CPndrInterface::Rate()" << LOG_ENDL;
    LOG_INFO << "Parameter Passing to Rate is " << "inRate = "<< inRate << LOG_ENDL;

    EPndrError retErrorCode = E_INVALID_ARGUMENT;

    if(1 == inRate || 2 == inRate)
    {
        retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_RATE);

        if(retErrorCode == E_SUCCESS)
            m_CPndrStateHandler.Rate(inRate);
        else
            LOG_CRITICAL << "Rate request Declined. Error : "
                         << retErrorCode << LOG_ENDL;
    }

    LOG_TRACE << "Exiting CPndrInterface::Rate() with Error code = "
              << retErrorCode << LOG_ENDL;

    return retErrorCode;

}

/**
 *******************************************************************
 * Request to Bookmark current Track
 *
 * @param None
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is Not connected OR
 *                                    No track is in Play/Pause status
 *
 * @see:    Result
 *******************************************************************
 */
EPndrError CPndrInterface::BookmarkSong()
{
    LOG_TRACE << "In CPndrInterface::BookmarkSong()" << LOG_ENDL;

    EPndrError retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_BOOKMARK_SONG);

    if(retErrorCode == E_SUCCESS)
        m_CPndrStateHandler.BookmarkSong();

    else
        LOG_CRITICAL << "BookmarkSong request Declined. Error : "
                     << retErrorCode << LOG_ENDL;

    LOG_TRACE << "Exiting CPndrInterface::BookmarkSong() with Error code = "
              << retErrorCode << LOG_ENDL;

    return retErrorCode;
}

/**
 *******************************************************************
 * Bookmark Artist of current Track
 *
 * @param None
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is Not connected OR
 *                                    No track is in Play/Pause status
 *
 * @see:    Result
 *******************************************************************
 */
EPndrError CPndrInterface::BookmarkArtist()
{
    LOG_TRACE << "In CPndrInterface::BookmarkArtist()" << LOG_ENDL;

    EPndrError retErrorCode =  m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_BOOKMARK_ARTIST);

    if(retErrorCode == E_SUCCESS)
        m_CPndrStateHandler.BookmarkArtist();

    else
        LOG_CRITICAL << "BookmarkArtist request Declined. Error : "
                     << retErrorCode << LOG_ENDL;

    LOG_TRACE << "Exiting CPndrInterface::BookmarkArtist() with Error code = "
              << retErrorCode << LOG_ENDL;

    return retErrorCode;
}


/**
 *******************************************************************
 * Request to Get the station list.
 *

 * @param[in]    inSortType    What is expected order for station List
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is Not connected
 *
 * @see:    StationTokens
 *******************************************************************
 */
EPndrError CPndrInterface::RequestStationSort(const ESortType inSortType)
{
    LOG_TRACE << "In CPndrInterface::RequestStationSort()" << LOG_ENDL;

    LOG_INFO << "Parameter passing to RequestStationSort are" << "inSortType = "
             << inSortType << LOG_ENDL;

    EPndrError retErrorCode = E_INVALID_ARGUMENT;

    if(inSortType ==SORT_TYPE_ALPHA || inSortType == SORT_TYPE_DATE)
    {
        retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_SORT);
        if(retErrorCode == E_SUCCESS)
            m_CPndrStateHandler.RequestStationSort(inSortType);
        else
            LOG_CRITICAL << "RequestStationSort request Declined. Error : "
                         << retErrorCode << LOG_ENDL;
    }

    LOG_TRACE << "Exiting CPndrInterface::RequestStationSort() with Error code = "
              << retErrorCode << LOG_ENDL;

    return retErrorCode;
}


/**
 *******************************************************************
 * Request station sort order
 *
 * @param[in]    None
 *
 * @return     None
 *
 *******************************************************************
 */
EPndrError CPndrInterface::RequestStationSortOrder()
{
    LOG_TRACE << "In CPndrInterface::RequestStationSortOrder()" << LOG_ENDL;

    EPndrError retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_SORT);
    if(retErrorCode == E_SUCCESS)
        m_CPndrStateHandler.RequestStationSortOrder();
    else
        LOG_CRITICAL << "RequestStationSortOrder request Declined. Error : "
                     << retErrorCode << LOG_ENDL;


    LOG_TRACE << "Exiting CPndrInterface::RequestStationSortOrder() with Error code = "
              << retErrorCode << LOG_ENDL;

    return retErrorCode;

}

/**
 *******************************************************************
 * Request station count
 *
 * @param[in]    None
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Invalid State of mw
 *
 *******************************************************************
 */
EPndrError CPndrInterface::RequestStationCount()
{
    LOG_TRACE << "In CPndrInterface::RequestStationCount()" << LOG_ENDL;

    m_CPndrStateHandler.RequestStationCount();

   return E_SUCCESS;

}

/**
 *******************************************************************
 * Request to Get the station list.
 *
 * @param[in]    inStartIndex    Which is the start index for getting
 *                      the station list
 *                  inEndIndex      Which is the end index for getting
 *                      the station list
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is Not connected
 *                E_INVALID_STATION_REQ_RANGE: Invalid range
 *
 * @see:    StationInfoList
 *******************************************************************
 */
EPndrError CPndrInterface::RequestStationList(const qint8 inStartIndex,
                                              const qint8 inEndIndex)
{
    LOG_TRACE << "In CPndrInterface::RequestStationList()" << LOG_ENDL;

    LOG_INFO << "Parameter passing to RequestStationList are" << "inStartIndex = "
             << inStartIndex << "inEndIndex = " << inEndIndex << LOG_ENDL;

    EPndrError retErrorCode = E_INVALID_STATION_REQ_RANGE;

    if((inEndIndex >= inStartIndex)) /** Cheaking for Invalid value of range */
    {
        retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_STATION_LIST);

        if(retErrorCode == E_SUCCESS)
            m_CPndrStateHandler.RequestStationList(inStartIndex,inEndIndex);
        else
            LOG_CRITICAL << "RequestStationList request Declined. Error : "
                         << retErrorCode << LOG_ENDL;

    }

    LOG_TRACE << "Exiting CPndrInterface::RequestStationList() with Error code = "
              << retErrorCode << LOG_ENDL;

    return retErrorCode;
}

/**
 *******************************************************************
 * Request to select a station for start playing
 *
 * @param[in]    inStationToken    Token station which is selected
 *                                    to be played
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is NOT connected OR
 *                                    No station list was requested
 *                                    before
 *                E_INVALID_TOKEN: Station Token is not valid
 *
 * @see:    Result
 *******************************************************************
 */
EPndrError CPndrInterface::RequestStationSelect(const quint32 inStationToken)
{
    LOG_TRACE << "In CPndrInterface::RequestStationSelect()" << LOG_ENDL;

    LOG_TRACE << "Parameter passing to RequestStationSelect are" << "inStationToken = "
              << inStationToken << LOG_ENDL;

    EPndrError retErrorCode = E_INVALID_TOKEN;

        retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_STATION_SELECT);

        if(retErrorCode == E_SUCCESS)
            m_CPndrStateHandler.RequestStationSelect(inStationToken);
        else
            LOG_CRITICAL << "RequestStationSelect request Declined. Error : "
                         << retErrorCode << LOG_ENDL;


    LOG_TRACE << "Exiting CPndrInterface::RequestStationSelect() with Error code = "
              << retErrorCode << LOG_ENDL;

    return retErrorCode;
}

/**
 *******************************************************************
 * Request for auto search complete given a word
 *
 * @param[in]    inWordToSearch    The word to be searched
 *                                    max length: 248
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is NOT connected OR
 *                                    No search was requested before
 *                E_INVALID_STRING: Either string is empty or more than
 *                                    248 characters
 *
 * @see:    SearchComplete
 *******************************************************************
 */
EPndrError CPndrInterface::RequestSearchAutoComplete(const QString& inWordToSearch)
{
    LOG_TRACE << "In CPndrInterface::RequestSearchAutoComplete()" << LOG_ENDL;

    LOG_TRACE << "Parameter passing to RequestSearchAutoComplete are" << "inWordToSearch = "
              << inWordToSearch << LOG_ENDL;

    EPndrError retErrorCode = E_INVALID_ARGUMENT;

    int sizeOfReqString = inWordToSearch.size();

    if (sizeOfReqString <= SIZE_OF_SEARCH_REQ_STRNG || sizeOfReqString!=0)
    {
        retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_SEARCH_AUTO_COMPLETE);

        if(retErrorCode == E_SUCCESS)
            m_CPndrStateHandler.RequestSearchAutoComplete(inWordToSearch);
        else
            LOG_CRITICAL << "RequestSearchAutoComplete request Declined. Error : "
                         << retErrorCode << LOG_ENDL;
    }

    LOG_TRACE << "Exiting CPndrInterface::RequestSearchAutoComplete() with Error code = "
              << retErrorCode << LOG_ENDL;
    return retErrorCode;
}

/**
 *******************************************************************
 * Request for extended search for a given word
 *
 * @param[in]    inWordToSearch    The word to be searched
 *                                    max length: 248
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is NOT connected OR
 *                                    No search was requested before
 *                E_INVALID_STRING: Either string is empty or more than
 *                                    248 characters
 *
 * @see:    SearchComplete
 *******************************************************************
 */
EPndrError CPndrInterface::RequestSearch(const QString& inWordToSearch)
{
    LOG_TRACE << "In CPndrInterface::RequestSearch()" << LOG_ENDL;

    LOG_TRACE << "Parameter passing to RequestSearch are" << "inWordToSearch = "
              << inWordToSearch << LOG_ENDL;

    int sizeOfReqString = inWordToSearch.size();
    EPndrError retErrorCode = E_INVALID_ARGUMENT;

    if (sizeOfReqString > 0 && sizeOfReqString <= SIZE_OF_SEARCH_REQ_STRNG)
    {

        retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_SEARCH);

        if(retErrorCode == E_SUCCESS)
            m_CPndrStateHandler.RequestSearch(inWordToSearch);
        else
            LOG_CRITICAL << "RequestSearch request Declined. Error : "
                         << retErrorCode << LOG_ENDL;
    }
    else
    {
        LOG_CRITICAL << "Search request Declined. Search String length is invalid"<< LOG_ENDL;
    }


    LOG_TRACE << "Exiting CPndrInterface::RequestSearch() with Error code = "
              << retErrorCode << LOG_ENDL;

    return retErrorCode;
}

/**
 *******************************************************************
 * Request to select the item from search
 *
 * @param[in]    inToken    The token to identify the track
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATE: Device is NOT connected OR
 *                                    No search was completed
 *                                    before
 *                E_INVALID_TOKEN: Token is not valid
 *
 * @see:    TrackUpdated
 *******************************************************************
 */
EPndrError CPndrInterface::RequestSearchSelect(const quint32 inToken)
{
    LOG_TRACE << "In CPndrInterface::RequestSearchSelect()" << LOG_ENDL;

    LOG_INFO << "Parameter passing to RequestSearchSelect are" << "inToken = "
             << inToken << LOG_ENDL;

    EPndrError retErrorCode = E_INVALID_TOKEN;

        retErrorCode = m_CPndrStateHandler.CanProceedWithRequest(REQ_TYPE_SEARCH_SELECT);

        if(retErrorCode == E_SUCCESS)
        {
            m_CPndrStateHandler.RequestSearchSelect(inToken);
        }
        else
        {
            LOG_CRITICAL << "RequestSearchSelect request Declined. Error : "
                         << retErrorCode << LOG_ENDL;
        }

    LOG_TRACE << "Exiting CPndrInterface::RequestSearchSelect() with Error code = "
              << retErrorCode << LOG_ENDL;

    return retErrorCode;
}

/**
 *******************************************************************
 * Provide the CPndrStateHandler refference which needs to
 *              be connected to for listening to signal
 *
 * @param None
 *
 * @return     CPndrStateHandler&: object reference to through
 *                  which signals can be listened to by app
 *
 *******************************************************************
 */
CPndrStateHandler& CPndrInterface::GetSignalProvider()
{
    LOG_TRACE << "Inside CPndrInterface::GetSignalProvider()" << LOG_ENDL;
    return m_CPndrStateHandler;
}


/**
 *******************************************************************
 * Cancels any outstanding request of station Art when user goes back
 * from List view
 *
 * @return  void
 *
 *******************************************************************
 */
void CPndrInterface::CancelStationArtRequest()
{
    LOG_TRACE<<"CPndrInterface::CancelStationArtRequest Entry"<<LOG_ENDL;

    m_CPndrStateHandler.CancelStationArtRequest();

    LOG_TRACE<<"CPndrInterface::CancelStationArtRequest Exit"<<LOG_ENDL;
}


/**
 *******************************************************************
 * Request to set the elapsed time polling
 * @param  true elapsed time polling is enabled
 *         false elapsed time polling is disabled
 * @return  void
 *
 *******************************************************************
 */
void CPndrInterface::SetTrackElapsedPolling(bool inEnabled)
{
    //LOG_SLOT<< "CPndrInterface::SetTrackElapsedPolling:Entry" <<LOG_ENDL;

    m_CPndrStateHandler.SetTrackElapsedPolling(inEnabled);

    //LOG_SLOT<< "CPndrInterface::SetTrackElapsedPolling:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Returns the current sort order
 *
 * @return  ESortType the current sort type
 *
 *******************************************************************
 */
ESortType CPndrInterface::CurrentSortOrder()
{
   LOG_TRACE<<"In CPndrInterface::CurrentSortOrder "<<LOG_ENDL;

   return m_CPndrStateHandler.CurrentSortOrder();

}

/**
 *******************************************************************
 * Fetch station Art when user comes to Station List       *
 *
 * @return  void
 *
 *******************************************************************
 */
 void CPndrInterface::FetchStationArt()
 {
    m_CPndrStateHandler.FetchStationArt();
 }

//{ added by cheolhwan 2014-04-09.
/**
 *******************************************************************
 * Returns the Primary state of Pandora       *
 *
 * @return  EPriState
 *
 *******************************************************************
 */
 EPriState CPndrInterface::GetPriState()
 {
    return m_CPndrStateHandler.GetPriState();
 }

/**
 *******************************************************************
 * Returns the Secondary state of Pandora       *
 *
 * @return  ESecState
 *
 *******************************************************************
 */
 ESecState CPndrInterface::GetSecState()
 {
    return m_CPndrStateHandler.GetSecState();
 }

 /**
 *******************************************************************
 * Returns the Previous Secondary state of Pandora       *
 *
 * @return  ESecState
 *
 *******************************************************************
 */
 ESecState CPndrInterface::GetSecPreviousState()
 {
    return m_CPndrStateHandler.GetSecPreviousState();
 }
 //} added by cheolhwan 2014-04-09.

//{ added by cheolhwan 2014-04-17. To fix the unlimited receiving station. (ITS 238629)
/**
 *******************************************************************
 * Request to set the prefetching stations flag.
 * @param  true prefetching stations is enabled
 *         false prefetching stations is disabled
 * @return  void
 *
 *******************************************************************
 */
void CPndrInterface::setPreFetchingStations(bool inPreFetchingStations)
{
    m_CPndrStateHandler.setPreFetchingStations(inPreFetchingStations);
}
//} added by cheolhwan 2014-04-17. To fix the unlimited receiving station.

//End Of File
