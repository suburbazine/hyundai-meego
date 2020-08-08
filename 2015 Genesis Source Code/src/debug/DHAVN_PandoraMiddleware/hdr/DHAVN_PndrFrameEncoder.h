/*
 ******************************************************************
 *        COPYRIGHT ?2011, LG Electronics.  
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     : Frame Encoder                                     *
 * PROGRAMMER : Snigdha Satapathy                                 *
 * DATE       : 05/12/2011                                        *
 * VERSION    : 0.1                                               *
 *--------------------------------------------------------------- *
 * MODULE SUMMARY :                                               *
 *  This module is required to be singleton. It will be used ONLY
 *  to Encode given data. 
 *
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  30/11/2011  0.1 Snigdha Satapathy       header file created
 *  05/12/2011  0.2 Vandana Ahuja           File modified, considering 
 *                                          change in overall design.
 ******************************************************************
 */


#ifndef DHAVN_PNDR_FRAME_ENCODER_H
#define DHAVN_PNDR_FRAME_ENCODER_H

#include <QObject>
#include "DHAVN_PndrFrameValidator.h"
#include "DHAVN_LogUtil.h"

class CPndrFrameEncoder : public QObject
{
    Q_OBJECT

public:

    /**
    * Constructor
    */
    CPndrFrameEncoder();

    /**
     *******************************************************************
     * Destructor
     *******************************************************************
     */
    ~CPndrFrameEncoder();

    /**
     *******************************************************************
     * Get the current instance
     *
     * @param[in]   inDeviceType: The device to be used for sending and
     *                              receiving information. E_DEVICE_NONE
     *                              if device is disconnected.
     *
     * @return     None
     * 
     *******************************************************************
     */
    //static CPndrFrameEncoder* GetInstance();

    /**
     *******************************************************************
     * Releases instance of FrameEncoder
     *
     * @param    None
     *
     * @return     None
     *
     *******************************************************************
     */
    // void ReleaseInstance();

     void SetValidator(CPndrFrameValidator* inFrameValidator);


     /**
      *******************************************************************
      * Calculates the CRC for the encoded frame
      *
      * @param[in]   inCRCData: The encoded frame whose checksum is
      *              to be calculated

      *
      * @return     The CRC for given data
      *
      *******************************************************************
      */
      static quint16 CalculateCRC(QByteArray& inCRCData,int len);


    /**
     *******************************************************************
     * Encode the command which doesn't need any parameter as input
     *
     * @param[in]   inCommandCode: The command to be encoded
     *              TrackInfo, TrackTitle, TrackArtist,
     *              TrackAlbum, Play, Pause, Skip,ratePositive,rateNegative,
     *              Explain, Bookmark Song, Bookmark Artist, StationCount,
     *              AllStationTokens,
     *
     * @return     QByteArray
     * 
     *******************************************************************
     */
     void EncodeAndSend(const quint8 inCommandCode);


    /**
     *******************************************************************
     * Encode the connect command
     *
     * @param[in]   inCommandCode: The command to be encoded
     * @param[in]   inApiVersion: API version
     * @param[in]   inAccessoryId: Accessory ID
     * @param[in]   inAlbumArtDimension: Expected AlbumArt size
     * @param[in]   inImageType: Expected AlbumArt Image type
     * @param[in]   inFlag: Various flag for debugging purpose
     * @param[in]   inStationArtDimension size of station art or genre station art image
     *
     * @return     QByteArray
     * 
     *******************************************************************
     */
     void EncodeAndSend(const quint8 inCommandCode,const quint16 inApiVersion,
                      const QString& inAccessoryId,const quint16 inAlbumArtDimension,
                      const quint8 inImageType,const quint8 inFlag,
                      const quint16 inStationArtDimension,bool inAppendToQueue = true);

    /**
     *******************************************************************
     * Encode the command which needs quit32 
     *                              data as input
     *
     * @param[in]   inCommandCode: The command to be encoded
     *              TrackAlbumArt, TrackExplain, StationTokens,
     *              stationSelect, 
     * @param[in]   inData: The to be encoded along with command.   
     *
     * @return     QByteArray
     * 
     *******************************************************************
     */
     void EncodeAndSend(const quint8 inCommandCode,const quint32 inData);

    /**
     *******************************************************************
     * Encode the command which needs quint8 data as input
     *
     * @param[in]   inCommandCode: The command to be encoded
     *              setTrackElapsedPolling, stationsSort
     * @param[in]   inData: The to be encoded along with command.   
     *
     * @return     QByteArray
     * 
     *******************************************************************
     */
     void EncodeAndSend(const quint8 aCommandCode,quint8 inEnabled);

    /**
     *******************************************************************
     * Encode the command which needs qlist data as input
     *
     * @param[in]   inCommandCode: The command to be encoded
     *              getStationInfo
     * @param[in]   inDataArray: The to be encoded along with command.   

     *
     * @return     QByteArray
     * 
     *******************************************************************
     */
     void EncodeAndSend(const quint8 inCommandCode,const QList<quint32>& inDataArray);

     /**
      *******************************************************************
      * Encode the command which needs quint32 & QString data as input
      *
      * @param[in]   inCommandCode: The command to be encoded
      *              searchAutoComplete, searchExtended
      * @param[in]   inData: The Search Id to be encoded along with command.
      * @param[in]   inDataArray: The Search Text to be encoded along with command.
      *
      * @return     None
      *
      *******************************************************************
      */
     void EncodeAndSend(const quint8 inCommandCode,const quint32 inData,
                       const QString& inDataArray);

    /**
     *******************************************************************
     * Encode the command which needs quint32 & QList data as input
     *
     * @param[in]   inCommandCode: The command to be encoded
     *              getSearchResultInfo
     * @param[in]   inData: The to be encoded along with command.  
     * @param[in]   inDataArray: The to be encoded along with command.   

     *
     * @return     QByteArray
     * 
     *******************************************************************
     */
     void EncodeAndSend(const quint8 inCommandCode,const quint32 inData,
                       const QList<quint32>& inDataArray);

    /**
     *******************************************************************
     *
     * Encode the command which needs quint32 & QByteArray 
     *                              data as input
     *
     * @param[in]   inCommandCode: The command to be encoded
     *              searchSelect
     * @param[in]   inData1: The to be encoded along with command.  
     * @param[in]   inData2: The to be encoded along with command.   
     *
     * @return     QByteArray
     * 
     *******************************************************************
     */
    void EncodeAndSend(const quint8 inCommandCode,const quint32 inData1,
                       const quint32 inData2);


//{ modified by yongkyun.lee 2014-08-25 for : Branding Image
    void EncodeAndSend(const quint8 inCommandCode, const quint8 SegIndex,const quint8 totalSeg, quint8* inData, int fzise);
//} modified by yongkyun.lee 2014-08-04 
private:

    /**
     *******************************************************************
     * Encodes the escape characters in the encoded frame
     *
     * @param[in]   inEncodedFrame: The encoded frame whose checksum is
     *              to be calculated

     *
     * @return     None
     *
     *******************************************************************
     */
     void EncodeEscapeCharacters(QByteArray& inEncodedFrame);


     /**
      *******************************************************************
      * Created the header for the encoded frame
      *
      * @param[in]   encodedFrame: The encoded frame

      *
      * @return     None
      *
      *******************************************************************
      */
     void CreateDataFrameHeader(QByteArray& encodedFrame);

     /**
      *******************************************************************
      * Created the trailer for the encoded frame and send it to transport layer
      *
      * @param[in]   encodedFrame: The encoded frame

      *
      * @return     None
      *
      *******************************************************************
      */
     void AppendFrameTrailerAndSend(QByteArray& encodedFrame,bool inAppendToQueue = true);

     /**
      *******************************************************************
      * Append the CRC to the encoded frame
      *
      * @param[in]   encodedFrame: The encoded frame

      *
      * @return     None
      *
      *******************************************************************
      */
     void AppendCRCtoDataFrame(QByteArray& encodedFrame);

private:


    /** Instance of Frame validator class */
    CPndrFrameValidator* m_pFrameValidator;

    /** Instance counter */
    static int m_nInstanceCounter;

    /** Reference to self */
    static CPndrFrameEncoder* m_pInstance;

    /** The seq. no */
    quint8 m_nSeqNo;

    USE_LOG_UTIL;

};

#endif //DHAVN_PNDR_FRAME_ENCODER_H
