/*
 ******************************************************************
 *        COPYRIGHT © 2011, LG Electronics.  
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     : Track Properties                                  *
 * PROGRAMMER : Vandana Ahuja                                     *
 * DATE       : 04/12/2011                                        *
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
 *  04/12/2011  0.1 Vandana Ahuja   header file created
 ******************************************************************
 */

#ifndef DHAVN_PNDR_TRACK_PROPERTIES_H
#define DHAVN_PNDR_TRACK_PROPERTIES_H

#include <QObject>
#include "DHAVN_Pndr_Internal_Defs.h"
#include "DHAVN_PndrFrameEncoder.h"
#include "DHAVN_PndrFrameDecoder.h"
#include <QFile>
#include "DHAVN_LogUtil.h"
#include "DHAVN_PndrLinkConstants.h"




class PndrFrameEncoder;
class CPndrFrameTxRx;

class CPndrTrackProperties : public QObject
{
    Q_OBJECT
public:
    /** 
    * Class CPndrTrackProperties
    *   
    * @param[in] inFrameDecoder FrameDecoder class instance
    */

    CPndrTrackProperties();

    void SetFrameTxRX(CPndrFrameTxRx*& inFrameTxRx);

    void SetEncoder(CPndrFrameEncoder* inFrameEncoder);

    void SetDecoder(CPndrFrameDecoder* inFrameDecoder);

    /**
    * Class CPndrTrackProperties Destructor
    *
    * @param[in] inFrameDecoder FrameDecoder class instance
    */

    ~CPndrTrackProperties();

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
     void RequestRateUp();

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
     void RequestRateDown();

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
     void RequestBookmarkSong();

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
     void RequestBookmarkArtist();
    
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
     void RequestExplain();

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
     void RequestAlbumArt(const EImageType inImgType);

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
     void RequestTitle();
    
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
     void RequestAlbum();

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
     void RequestArtist();

    /**
     *******************************************************************
     * Request to get the track info 
     *
     * @param None
     *
     * @return  None
     *
     *******************************************************************
     */
     void RequestTrackPartInfo();

	 /**
	 *******************************************************************
	 * Request Track basic info 
	 *
	 * @param none
	 * @return  none
	 *
	 *******************************************************************
	 */
     void RequestTrackInfo();

	 //{ modified by yongkyun.lee 2014-08-25 for : Branding Image
	 void RequestBrandingImage();
	 void RequestBrandingSegment( const quint8 SegIndex,const quint8 totalSeg, quint8* inData,  int fzise);
	 //} modified by yongkyun.lee 2014-08-04 
     /**
      *******************************************************************
      * Update track token from state handler 
      *
      * @param none
      * @return  none
      *
      *******************************************************************
      */
      void UpodateTrackToken(quint32 inToken);

     /**
      *******************************************************************
      * Request to set the elapsed time polling
      * @param  true elapsed time polling is enabled
      *         false elapsed time polling is disabled
      * @return  void
      *
      *******************************************************************
      */
      void SetTrackElapsedPolling(bool inEnabled);

      /**
       *******************************************************************
       * Request for clear all cache info .
       *
       * @param none
       * @return  none
       *
       *******************************************************************
       */
      void ClearData();

private:
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
     void ClearAlbumArtFolder();

public slots:

     void OnTimeout();
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
    void OnRequestCompletion(const EPndrEvent inRequest, const quint32 inData);

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
    void OnRequestCompletion(const EPndrEvent inRequest,
                           const quint32 inTrackToken,
                           const quint8 inSegmentindex,
                           const quint8 inTotalsegments,
                           const QByteArray& inData);

signals:
    /**
     *******************************************************************
     * Signal from this class for providing information
     * about completion of explain request and Album Art request
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
    void RequestCompleted(const EPndrEvent inRequest, const QString& inData);

private:     
    USE_LOG_UTIL;
    /** Instance of Encoder class */
    CPndrFrameEncoder   *   m_pFrameEncoder;
    /** Instance of Decoder class */
    CPndrFrameDecoder   *   m_pFrameDecoder;
    /** Instance of FrameTxRx class */
    CPndrFrameTxRx* m_pFrameTxRx;

    /*bytearray to store the data */
    QByteArray  m_ByteArr;
    /* to check weather albumart loaded*/
    bool m_isAlbumArtLoaded;
    /* to check if AlbumArt has been requested */
    bool m_isAlbumArtRequested;
    /* track token*/
    quint32  m_nTrackToken;
    /* image type */
    EImageType m_eAlbumArtImgType;

    /** Timer for AlbumArt */
    QTimer m_AlbumArtTimer;

    /** Flag to disable AlbumArt if it is received after timeout */
    quint8 m_nDiscardAlbumArt;
    /** station token*/
    quint32 m_nStToken;
};

#endif //DHAVN_PNDR_TRACK_PROPERTIES_H
