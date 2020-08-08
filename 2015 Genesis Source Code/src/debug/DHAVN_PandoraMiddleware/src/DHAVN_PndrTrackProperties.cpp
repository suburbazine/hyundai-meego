/*
 ******************************************************************
 *        COPYRIGHT © 2011, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     : Track Properties                                  *
 * PROGRAMMER : Dayasudhan Kuruva                                    *
 * DATE       : 07/12/2011                                        *
 * VERSION    : 0.1                                               *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module request the encoder to encode frame and use
 *  frame handler to send the packet.
 *
 *
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  07/12/2011  0.1 Dayasudhan Kuruva  Cpp file created
 ******************************************************************
 */

#include "DHAVN_PndrLinkConstants.h"
#include "DHAVN_PndrTrackProperties.h"
#include "DHAVN_PndrFrameTxRx.h"
#include <QDir>


const int cAlbumArtTimeoutInterval = 10000; //10 sec
const char* __FORMAT =  "%s%s_%d_%d";
const char* ALBUM_ART_PATH = "/app/data/AppPandora/AlbumArt";
const char* ALBUM_ART_FILE = "/AlbumArt";
const char* ALBUM_ART_TYPE_JPEG  = ".jpeg";
const char* ALBUM_ART_TYPE_PNG  = ".png";
const char* ALBUM_ART_TYPE_RGB565 = ".bmp";

/**
* Class CPndrTrackProperties
*
* @param[in] inFrameDecoder FrameDecoder class instance
*/
CPndrTrackProperties::CPndrTrackProperties()
    : m_nTrackToken(0) // added by esjang 2012.11.12 for Static Test CID 13140
    , m_eAlbumArtImgType(IMG_TYPE_NONE) // added by esjang 2012.11.12 for Static Test CID 13140
    ,m_nStToken(0)

{
    LOG_INIT_V2("Pandora");
    /* No need to check the ptr validation its ensured that in the state handler only its validated*/
    m_pFrameEncoder = NULL ; //CPndrFrameEncoder::GetInstance();
    m_pFrameDecoder = NULL ; //CPndrFrameDecoder::GetInstance();
//    m_pFrameTxRx = CPndrFrameTxRx::GetInstance();
    m_pFrameTxRx = NULL;

    m_isAlbumArtLoaded = false;
    m_isAlbumArtRequested = false;
    m_nDiscardAlbumArt = 0;

//    QObject::connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent , const quint32 )),this,SLOT(OnRequestCompletion(const EPndrEvent, const quint32)));
//    QObject::connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent ,const quint32,const quint8,const quint8,const QByteArray& )),this,SLOT(OnRequestCompletion(const EPndrEvent ,const quint32,const quint8,const quint8,const QByteArray&)));

    //connect to the timeout signal of Timer
    m_AlbumArtTimer.setInterval(cAlbumArtTimeoutInterval);
    m_AlbumArtTimer.setSingleShot(true);
    connect(&m_AlbumArtTimer,SIGNAL(timeout()),this,SLOT(OnTimeout()));
}

    /**
    * Class ~CPndrTrackProperties
    *
    * @param[in] inFrameDecoder FrameDecoder class instance
    */
CPndrTrackProperties::~CPndrTrackProperties()
{
    LOG_TRACE << "In CPndrTrackProperties::~CPndrTrackProperties()) "<< LOG_ENDL;
    //m_pFrameEncoder->ReleaseInstance();
    m_pFrameEncoder = NULL;
    //m_pFrameDecoder->ReleaseInstance();
    m_pFrameDecoder = NULL;
    //m_pFrameTxRx->ReleaseInstance();
    m_pFrameTxRx = NULL;
    if(!m_ByteArr.isNull())
    {
        m_ByteArr.clear();
    }

    if( m_AlbumArtTimer.isActive())
        {
            m_AlbumArtTimer.stop();
        }
    LOG_TRACE << "Exiting CPndrTrackProperties::~CPndrTrackProperties()) "<< LOG_ENDL;
}

/**
 *******************************************************************
 * Request for clear all cache info .
 *
 * @param none
 * @return  none
 *
 *******************************************************************
 */
void CPndrTrackProperties::ClearData()
{
    m_ByteArr.clear();
    m_nTrackToken = 0;
    m_nStToken = 0;
}

void CPndrTrackProperties::SetFrameTxRX(CPndrFrameTxRx*& inFrameTxRx)
{
    m_pFrameTxRx =inFrameTxRx;
}

void CPndrTrackProperties::SetEncoder(CPndrFrameEncoder* inFrameEncoder)
{
    m_pFrameEncoder = inFrameEncoder;
}

void CPndrTrackProperties::SetDecoder(CPndrFrameDecoder* inFrameDecoder)
{
    m_pFrameDecoder = inFrameDecoder;

    QObject::connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent , const quint32 )),this,SLOT(OnRequestCompletion(const EPndrEvent, const quint32)));

    QObject::connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent ,const quint32,const quint8,const quint8,const QByteArray& )),this,SLOT(OnRequestCompletion(const EPndrEvent ,const quint32,const quint8,const quint8,const QByteArray&)));

}
    /**
     *******************************************************************
     * Request for Thumb up the current track
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
void CPndrTrackProperties::RequestRateUp()
{
    LOG_TRACE << "In CPndrTrackProperties::RequestRateUp())"<< LOG_ENDL;
    m_pFrameEncoder->EncodeAndSend(PNDR_EVENT_TRACK_RATE_POSITIVE);
    LOG_TRACE << "Exiting CPndrTrackProperties::RequestRateUp()) "<< LOG_ENDL;
}

    /**
     *******************************************************************
     * Request for Thumb down the current track
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
void CPndrTrackProperties::RequestRateDown()
{
    LOG_TRACE << "In CPndrTrackProperties::RequestThumbDown()) "<< LOG_ENDL;
    m_pFrameEncoder->EncodeAndSend(PNDR_EVENT_TRACK_RATE_NEGATIVE);
    LOG_TRACE << "Exiting CPndrTrackProperties::RequestThumbDown()) "<< LOG_ENDL;
}

    /**
     *******************************************************************
     * Request to bookmark the current track
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
void CPndrTrackProperties::RequestBookmarkSong()
{
    LOG_TRACE << "In CPndrTrackProperties::RequestBookmarkSong()) "<< LOG_ENDL;
    m_pFrameEncoder->EncodeAndSend(PNDR_EVENT_TRACK_BOOKMARK_SONG);
    LOG_TRACE << "Exiting CPndrTrackProperties::RequestBookmarkSong()) "<< LOG_ENDL;
}

    /**
     *******************************************************************
     * Request to bookmark artist of current track
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
void CPndrTrackProperties::RequestBookmarkArtist()
{
    LOG_TRACE << "In CPndrTrackProperties::RequestBookmarkArtist()) "<< LOG_ENDL;
    m_pFrameEncoder->EncodeAndSend(PNDR_EVENT_TRACK_BOOKMARK_ARTIST);
    LOG_TRACE << "Exiting CPndrTrackProperties::RequestBookmarkArtist()) "<< LOG_ENDL;
}

    /**
     *******************************************************************
     * Request to get the explanation for current track
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
void CPndrTrackProperties::RequestExplain()
{
    LOG_TRACE << "In CPndrTrackProperties::RequestExplains()) "<< LOG_ENDL;
    m_pFrameEncoder->EncodeAndSend(PNDR_EVENT_TRACK_EXPLAIN);
    LOG_TRACE << "Exiting CPndrTrackProperties::RequestExplain()) "<< LOG_ENDL;
}

    /**
     *******************************************************************
     * Request to get the album Art of current song
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
void CPndrTrackProperties::RequestAlbumArt(const EImageType inImgType)
{
    LOG_TRACE << "In CPndrTrackProperties::RequestAlbumArt()) "<< LOG_ENDL;

    m_eAlbumArtImgType = inImgType;
    m_isAlbumArtRequested = true;
    m_nDiscardAlbumArt = 0;

    ClearAlbumArtFolder();
    //Update Album Art event has come from server.
    // So issue request to get the AlbumArt Segments
    //if(m_isAlbumArtLoaded == true)
    {
        m_isAlbumArtLoaded = false;
        m_AlbumArtTimer.start();
        m_pFrameEncoder->EncodeAndSend( PNDR_GET_TRACK_ALBUM_ART,m_pFrameTxRx->GetMaxPayloadLength());
    }
    LOG_TRACE << "Exiting CPndrTrackProperties::RequestAlbumArt()) "<< LOG_ENDL;
}

    /**
     *******************************************************************
     * Request to get the title of current track
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
void CPndrTrackProperties::RequestTitle()
{
    LOG_TRACE << "In CPndrTrackProperties::RequestTitle()) "<< LOG_ENDL;
    m_pFrameEncoder->EncodeAndSend( PNDR_GET_TRACK_TITLE);
    LOG_TRACE << "Exiting CPndrTrackProperties::RequestTitle()) "<< LOG_ENDL;
}

    /**
     *******************************************************************
     * Request to get the Album of current track
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
void CPndrTrackProperties::RequestAlbum()
{
    LOG_TRACE << "In CPndrTrackProperties::RequestAlbum()) "<< LOG_ENDL;
    m_pFrameEncoder->EncodeAndSend( PNDR_GET_TRACK_ALBUM);
    LOG_TRACE << "Exiting CPndrTrackProperties::RequestAlbum()) "<< LOG_ENDL;
}

    /**
     *******************************************************************
     * Request for getting Artist for current track
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
void CPndrTrackProperties::RequestArtist()
{
    LOG_TRACE << "In CPndrTrackProperties::RequestArtist()) "<< LOG_ENDL;
    m_pFrameEncoder->EncodeAndSend(PNDR_GET_TRACK_ARTIST);
    LOG_TRACE << "Exiting CPndrTrackProperties::RequestArtist()) "<< LOG_ENDL;
}

    /**
     *******************************************************************
     * Request to get the track info
     * Requests the track token, album art length, flags, duration, elapsed play time in seconds, and rating for the
     * current track to be returned via PNDR_RETURN_TRACK_INFO.
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
    void CPndrTrackProperties::RequestTrackPartInfo()
    {
        //LOG_TRACE << "In CPndrTrackProperties::RequestTrackPartInfo()) "<< LOG_ENDL;
        m_pFrameEncoder->EncodeAndSend(PNDR_GET_TRACK_INFO_EXTENDED);
        //LOG_TRACE << "Exiting CPndrTrackProperties::RequestTrackPartInfo()) "<< LOG_ENDL;
    }

    /**
	 *******************************************************************
	 * Request Track basic info 
	 *
	 * @param none
	 * @return  none
	 *
	 *******************************************************************
	 */
    void CPndrTrackProperties::RequestTrackInfo()
    {
        m_pFrameEncoder->EncodeAndSend(PNDR_GET_TRACK_INFO);
    }

    //{ modified by yongkyun.lee 2014-08-25 for : Branding Image
    void CPndrTrackProperties::RequestBrandingImage()
    {
        LOG_INFO << "CPndrTrackProperties::RequestBrandingImage"  <<LOG_ENDL;
        m_pFrameEncoder->EncodeAndSend(PNDR_UPDATE_BRANDING_IMAGE);
    }
    void CPndrTrackProperties::RequestBrandingSegment( const quint8 SegIndex,const quint8 totalSeg, quint8* inData,  int fzise)
    {
        LOG_INFO << "CPndrTrackProperties::RequestBrandingSegment"  <<LOG_ENDL;
        m_pFrameEncoder->EncodeAndSend(PNDR_RETURN_BRANDING_IMAGE_SEGMENT , SegIndex, totalSeg , inData , fzise);
    }
    //} modified by yongkyun.lee 2014-08-04 

    /**
     *******************************************************************
     * Slot for receiving the signal for completion of
     *              event request for track explain and Album Art
     *              PNDR_UPDATE_TRACK_EXPLAIN
     *              PNDR_UPDATE_TRACK_ALBUM_ART
     *
     *  @param[in]  inRequest   Request type for which this is the
     *                          response
     *  @param[in]  inData      The string containing the data as part of
     *                          response
     *
     * @return  None
     *
     *******************************************************************
     */
void CPndrTrackProperties::OnRequestCompletion(const EPndrEvent inRequest, const quint32 inData)
{
    LOG_SLOT<< "CPndrTrackProperties::Event Track Update for AlbumArt/Explain/Acive st. Req = " << inRequest
            << " Data = " << inData<< LOG_ENDL;

    if(EVENT_TRACK_UPDATED == inRequest && inData > 0)
    {
        //LOG_INFO << "trackProp got Track Updated received for token " <<  inData <<LOG_ENDL;

        //Save the new track token as current track token and clear the ByteArray for AlbumArt
        m_nTrackToken = inData;

        m_ByteArr.clear();
    }
    else if(EVENT_EXPLAIN == inRequest)
    {
        //LOG_INFO << "trackProp got track token as " <<  inData <<LOG_ENDL;

        m_pFrameEncoder->EncodeAndSend(PNDR_GET_TRACK_EXPLAIN,
                                       m_pFrameTxRx->GetMaxPayloadLength());
    }
    else if(EVENT_ALBUM_ART == inRequest)
    {
       // LOG_INFO << "Received album art event for track token  " <<  inData <<LOG_ENDL;

        //qDebug() << "event album art " << inData;
        if(m_nTrackToken == inData)
        {
            //If request for album art had already come,
            // send GetAlbumArtSegment req as soon as server sends event AlbumArt
            if(m_isAlbumArtRequested)
            {
                m_pFrameEncoder->EncodeAndSend( PNDR_GET_TRACK_ALBUM_ART,
                                                m_pFrameTxRx->GetMaxPayloadLength());
                m_isAlbumArtRequested = false;
            }
            else
            {
                //Set AlbumArt loaded flag as true;
                //AlbumArtSegment request will be sent when ReqAlbumArt comes
                m_isAlbumArtLoaded = true;
            }
        }
    }
    else if(EVENT_STATION_ACTIVE == inRequest && inData > 0 )
    {
        m_nStToken = inData;
    }
    else{
        //make compiler happy
    }

   // LOG_INFO<< "Exiting CPndrTrackProperties::OnRequestCompletion1()) "<< LOG_ENDL;
}

   /**
     *******************************************************************
     * Slot for receiving the signal for completion of
     *              event request for track explain and Album Art
     *              PNDR_RETURN_TRACK_EXPLAIN_SEGMENT
     *              PNDR_RETURN_TRACK_ALBUM_ART_SEGMENT
     *
     *  @param[in]  inRequest   Request type for which this is the
     *                          response
     *  @param[in]  inData1     The segment index
     *  @param[in]  inData2     Total Segments
     *  @param[in]  inData3     Data
     *
     * @return  None
     *
     *******************************************************************
     */

void CPndrTrackProperties::OnRequestCompletion(const EPndrEvent inRequest,
                                               const quint32 inTrackToken,
                                               const quint8 inSegmentindex,
                                               const quint8 inTotalsegments,
                                               const QByteArray& inData)
{
    //LOG_INFO<< "In CPndrTrackProperties::OnRequestCompletion2()) "<< LOG_ENDL;

    //If the album art is received for previous track, ignore it
    if(m_nTrackToken != inTrackToken)
    {
        LOG_HIGH<<"CPndrTrackProperties::OnRequestCompletion Invalid Track data: m_nTrackToken["<<m_nTrackToken<<"] inTrackToken["<<inTrackToken<<"]"<<LOG_ENDL;
    }
    //Process Album Art data for current track
    else
    {
        m_AlbumArtTimer.stop();
        //{ added by esjang 2014.02.17 for ITS # 222062
        if(inSegmentindex == 0)
        {
            LOG_SLOT << "clear byte" << LOG_ENDL;
            m_ByteArr.clear();
        }
        //} added by esjang 2014.02.17 for ITS # 222062
        
        if(!inData.isNull())
        {
            m_ByteArr.append(inData);
        }
        /* If  all the segments are download completed, then emit the signal to state handler */
        if((inSegmentindex + 1) == inTotalsegments)
        {
            if(EVENT_GET_EXPLAIN == inRequest)
            {
                const QString str(m_ByteArr);
                emit RequestCompleted(inRequest,str);
                m_ByteArr.clear();
            }
            else
            {                

                QString filePath;
                filePath= filePath.sprintf(__FORMAT,ALBUM_ART_PATH,ALBUM_ART_FILE,
                                           m_nStToken , inTrackToken);
                /* File to store albumart picture*/
                QFile albumartFile;

                switch(m_eAlbumArtImgType)
                {
                    case IMG_TYPE_JPEG:
                    {
                        filePath.append(ALBUM_ART_TYPE_JPEG);//append file extension
                        break;
                    }
                    case IMG_TYPE_PNG:
                    {
                        filePath.append(ALBUM_ART_TYPE_PNG);//append file extension
                        break;
                    }
                    case IMG_TYPE_RGB565:
                    {
                        filePath.append(ALBUM_ART_TYPE_RGB565);//append file extension
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                //append the file name
                albumartFile.setFileName(filePath);

                if(albumartFile.open(QIODevice::WriteOnly))
                {
                    albumartFile.write(m_ByteArr);
                    m_ByteArr.clear();
                    albumartFile.close();


                    if(!m_nDiscardAlbumArt)
                    {
                        m_nDiscardAlbumArt = 1;
                        //LOG_INFO<<"Album Art emitted "<<LOG_ENDL;
                        emit RequestCompleted(inRequest,albumartFile.fileName());

                    }
                }
                else
                {
                    m_ByteArr.clear();
                    LOG_SIGNAL<<" Error , Opening file . signaled no album art "<<LOG_ENDL;

                    if(!m_nDiscardAlbumArt)
                    {
                        m_nDiscardAlbumArt = 1;
                        emit RequestCompleted(inRequest,"");
                    }
                }

                m_isAlbumArtRequested = false;
            }
           // LOG_INFO<< "Exiting CPndrTrackProperties::OnRequestCompletion2()) "<< LOG_ENDL;
        }
    }
}

/**
 *******************************************************************
 * Clears the previous AlbumArts if any from the folder before making
 * a new request
 * @param None
 *
 * @return  None
 *
 *******************************************************************
 */
void CPndrTrackProperties::ClearAlbumArtFolder()
{
    LOG_TRACE<< "Entry CPndrTrackProperties::ClearAlbumArtFolder()) "<< LOG_ENDL;

    QString filePath;
    filePath.append(ALBUM_ART_PATH);//append file name
    QDir albumArtDir(filePath);

    //If the AlbumArt folder doesnt exist, create one
    if(!albumArtDir.exists())
    {
        albumArtDir.mkpath(ALBUM_ART_PATH);
    }
    else
    {
        //Check if any Album Art file is present and delete them
        for(uint i= 0; i < albumArtDir.count();i++)
        {
            albumArtDir.remove(albumArtDir.entryList().at(i));
        }
    }

    LOG_TRACE<< "Exiting CPndrTrackProperties::ClearAlbumArtFolder()) "<< LOG_ENDL;

}


/**
*******************************************************************
* Timer slot to be listened to for album art response
*
* @param[in]   None
*
* @return     None
*
*******************************************************************
*/

void CPndrTrackProperties::OnTimeout()
{
    LOG_SLOT<< "CPndrTrackProperties::OnTimeout:Entry" <<LOG_ENDL;

    m_nDiscardAlbumArt = 1;
    emit RequestCompleted(EVENT_GET_ALBUM_ART,QString(""));

    LOG_SLOT<< "CPndrTrackProperties::OnTimeout:Exit" <<LOG_ENDL;

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
 void CPndrTrackProperties::SetTrackElapsedPolling(bool inEnabled)
 {
     //LOG_INFO<< "CPndrTrackProperties::SetTrackElapsed Polling:Entry" <<LOG_ENDL;

     quint8 enabled = PNDR_FALSE;
     if(inEnabled)
     {
         enabled = PNDR_TRUE;
     }
     m_pFrameEncoder->EncodeAndSend(PNDR_SET_TRACK_ELAPSED_POLLING, enabled);

     //LOG_INFO<< "CPndrTrackProperties::SetTrackElapsed Polling:Exit" <<LOG_ENDL;

 }

/**
 *******************************************************************
 * Update track token from state handler 
 *
 * @param none
 * @return  none
 *
 *******************************************************************
 */
void CPndrTrackProperties::UpodateTrackToken(quint32 inToken)
{
    m_nTrackToken = inToken;
}

//End Of File
