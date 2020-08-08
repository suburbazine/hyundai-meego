/*
 ******************************************************************
 *        COPYRIGHT  2011, LG Electronics.  
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :   Pandor Frame Decoder                            *
 * PROGRAMMER :   Vandana Ahuja                                   *
 * DATE       :   05/12/2011                                      *
 * VERSION    :   0.1                                             *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :    This module will receive the data          *
 *  from transport layer. Check if its Ack, if so then
 *  update packet validator about received ack else will decode the 
 *  packet and emit the signal to be listened by interested modules.
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  05/12/2011  0.1 Vandana Ahuja   header file created
 ******************************************************************
 */


#ifndef DHAVN_PNDR_FRAME_DECODER_H
#define DHAVN_PNDR_FRAME_DECODER_H

#include <QObject>
#include <QMap>
#include "DHAVN_Pndr_Internal_Defs.h"
#include "DHAVN_Pndr_Defs.h"
#include "DHAVN_PndrLinkConstants.h"
#include "DHAVN_LogUtil.h"

/**
* This class will decode the frames received from transport layer
*/
class CPndrFrameDecoder : public QObject
{
    Q_OBJECT

public:


    /**
    * Constructor
    */
    CPndrFrameDecoder();

    /**
     *******************************************************************
     * Destructor
     *******************************************************************
     */
    ~CPndrFrameDecoder();


    /**
     *******************************************************************
     * Get the single instance of decoder class
     *
     * @param   None
     *  
     * @return  None
     *
     *******************************************************************
    */
   // static CPndrFrameDecoder* GetInstance();


    /**
     *******************************************************************
     * Releases instance of FrameDecoder
     *
     * @param    None
     *
     * @return     None
     *
     *******************************************************************
     */
    //void ReleaseInstance();

    /**
     *******************************************************************
     * Check if received frame is Ack frame
     *
     * @param   None
     *  
     * @return  None
     *
     *******************************************************************
    */
    bool IsAckFrame(const QByteArray& inFrame);

    /**
     *******************************************************************
     * Check if received frame is a complete frame
     *
     * @param   None
     *
     * @return  None
     *
     *******************************************************************
    */
    bool IsFrameComplete(const QByteArray& inFrame);

    /**
     *******************************************************************
     * Gets the sequence no.
     *
     * @param   None
     *
     * @return  The seq. no.
     *
     *******************************************************************
    */
    quint8 GetSequenceNumber(const QByteArray& inFrame);

//{ added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack
    /**
     *******************************************************************
     * Gets the sequence no.
     *
     * @param   None
     *
     * @return  The seq. no.
     *
     *******************************************************************
    */
    quint8 GetRequestIDNumber(const QByteArray& inFrame);
//} added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack


    /**
     *******************************************************************
     * Gets the payload length
     *
     * @param   None
     *
     * @return  The payload length
     *
     *******************************************************************
    */
    quint32 GetPayloadLength(const QByteArray& inFrame);

    /**
     *******************************************************************
     * Gets the CRC
     *
     * @param   None
     *
     * @return  The CRC
     *
     *******************************************************************
    */
    quint16 GetCRC(const QByteArray& inFrame);

    /**
     *******************************************************************
     * Decode the data frame and emit the required signal
     *
     * @param   None
     *  
     * @return  None
     *
     *******************************************************************
    */
    void DecodeAndEmit(const QByteArray& inFrame);

    /**
     *******************************************************************
     * Emit the network error signal since multiple trials have failed
     *
     * @param   None
     *
     * @return  None
     *
     *******************************************************************
    */
    void EmitNetworkError();

    /**
     *******************************************************************
     * Map the ResponseId to event Id
     *
     * @param   None
     *
     * @return  None
     *
     *******************************************************************
    */
    void initResCmdEventIdMap();

    /**
     *******************************************************************
     * Map the Response to Error Id
     *
     * @param   None
     *
     * @return  None
     *
     *******************************************************************
    */
    void initResponseErrIdMap();

    /**
     *******************************************************************
     * Remove the escape from given array
     *
     * @param   None
     *
     * @return  None
     *
     *******************************************************************
    */
    void RemoveEscape(QByteArray& inFrame);

signals:
    /**
    * Following signals will be emitted after decoding the data on reception
    */
    
    /**
     *******************************************************************
     *    Used for signaling for following commands
     *                  PNDR_UPDATE_STATUS
     *                  PNDR_UPDATE_NOTICE
     *                  PNDR_UPDATE_TRACK_BOOKMARK_SONG
     *                  PNDR_UPDATE_TRACK_BOOKMARK_ARTIST
     *                  PNDR_RETURN_STATION_COUNT
     *                  PNDR_UPDATE_STATIONS_ORDER
     *                  PNDR_UPDATE_TRACK_RATING
     *
     * @param[in]  inRequest   Request type for which this is the 
     *                          response
     * @param[in]  inData1     Respose data based on request type
     *
     * @return  None
     *
     *******************************************************************
    */
    void ReplyReceived(const EPndrEvent inRequest, const quint8 inData);
    /**
    TODO : Add comment for track rating
    */

     void ReplyReceived(const EPndrEvent inRequest, const qint8 inData);

    /*
     *******************************************************************
     * Used for signaling for following commands
     *                  PNDR_UPDATE_TRACK_ELAPSED
     *
     * @param[in]  inRequest   Request type for which this is the 
     *                          response
     * @param[in]  inData      Respose data based on request type
     *
     * @return  None
     *
     *******************************************************************
    */
    void ReplyReceived(const EPndrEvent inRequest, const quint16 inData);

    /**
     *******************************************************************
     * Used for signaling for following commands
     *                  PNDR_RETURN_TRACK_TITLE
     *                  PNDR_RETURN_TRACK_ARTIST
     *                  PNDR_RETURN_TRACK_ALBUM
     *
     * @param[in]  inRequest   Request type for which this is the 
     *                          response
     * @param[in]  inData      Response data based on request type
     *
     * @return  None
     *
     *******************************************************************
    */
    void ReplyReceived(const EPndrEvent inRequest, const QString& inData);

    /**
     *******************************************************************
     * Used for signaling for following commands
     *                  PNDR_RETURN_SEARCH_RESULT_INFO
     *
     *
     * @param[in]  inRequest   Request type for which this is the 
     *                          response
     * @param[in]  inSearchList Respose data based on request type
     *
     * @return  None
     *
     *******************************************************************
    */
    void ReplyReceived(const EPndrEvent inRequest,const quint32 inSearchId, const QList<TSSearchInfo>& inSearchList);
    
    /**
     *******************************************************************
     * Used for signaling for following commands
     *                  PNDR_UPDATE_STATION_ADDED
     *
     * @param[in]  inRequest   Request type for which this is the 
     *                          response
     * @param[in]  inData1 Respose data based on request type
     * @param[in]  inData2 Respose data based on request type
     *
     * @return  None
     *
     *******************************************************************
    */
    void ReplyReceived(const EPndrEvent inRequest, const quint32 inData1, 
                       const quint8 inData2);
    
    /**
     *******************************************************************
     * Used for signaling for following commands
     *                  PNDR_UPDATE_TRACK
     *                  PNDR_UPDATE_TRACK_ALBUM_ART
     *                  PNDR_UPDATE_TRACK_EXPLAIN
     *                  PNDR_UPDATE_STATION_DELETED
     *                  PNDR_UPDATE_STATION_ACTIVE
     *
     * @param[in]  inRequest   Request type for which this is the 
     *                          response
     * @param[in]  inData1 Respose data based on request type
     *
     * @return  None
     *
     *******************************************************************
    */
    void ReplyReceived(const EPndrEvent inRequest, const quint32 inData1);
    
    /**
     *******************************************************************
     * Used for signaling for following commands
     *                  PNDR_RETURN_TRACK_INFO_EXTENDED
     *
     * @param[in]  inRequest   Request type for which this is the 
     *                          response
     * @param[in]  inData  Respose data based on request type
     *
     * @return  None
     *
     *******************************************************************
    */
    void ReplyReceived(const EPndrEvent inRequest, const TSTrackInfo& inData);
    

    /**
     *******************************************************************
     * Used for signaling for following commands
     *                  PNDR_UPDATE_SEARCH
     *
     * @param[in]  inRequest   Request type for which this is the 
     *                          response
     * @param[in]  inData1 Respose data based on request type
     * @param[in]  inData2 Respose data based on request type
     *
     * @return  None
     *
     *******************************************************************
    */
    void ReplyReceived(const EPndrEvent inRequest, const quint32 inData1,
                       const QList<quint32>& inData2);

    /**
     *******************************************************************
     * Used for signaling for following commands
     *                  PNDR_RETURN_STATION_TOKENS
     *
     * @param[in]  inRequest   Request type for which this is the
     *                          response
     * @param[in]  inData1 Respose data based on request type
     * @param[in]  inData2 Respose data based on request type
     *
     * @return  None
     *
     *******************************************************************
    */
    void ReplyReceived(const EPndrEvent inRequest, const quint8 inData1,
                       const QList<quint32>& inData2);


    /**
     *******************************************************************
     * Used for signaling for following commands
     *                  PNDR_RETURN_STATION_INFO
     *
     * @param[in]  inRequest   Request type for which this is the
     *                          response
     * @param[in]  inStationList Respose data based on request type
     *
     * @return  None
     *
     *******************************************************************
    */
    void ReplyReceived(const EPndrEvent inRequest,const QList<TSStationInfo>& inStationList);

    /**
     *******************************************************************
     * Used for signaling for following commands
     *                  PNDR_RETURN_TRACK_ALBUM_ART_SEGMENT
     *                  PNDR_RETURN_TRACK_EXPLAIN_SEGMENT
     *
     * @param[in]  inRequest   Request type for which this is the
     *                          response
     * @param[in]  inTrackToken The track token
     * @param[in]  segIndex The seg index
     * @param[in]  totalSeg The total no. of segments
     * @param[in]  data the image data
     *
     * @return  None
     *
     *******************************************************************
    */
    void ReplyReceived(const EPndrEvent inRequest,const quint32 inTrackToken,const quint8 segIndex,
                       const quint8 totalSeg,const QByteArray& data);

    /**
     *******************************************************************
     * Used for signaling for following commands
     *                  PNDR_RETURN_STATION_ART_SEGMENT
     *
     * @param[in]  inRequest   Request type for which this is the
     *                          response
     * @param[in]  inStationToken The station token
     * @param[in]  inArtLength    The art Length
     * @param[in]  segIndex The seg index
     * @param[in]  totalSeg The total no. of segments
     * @param[in]  data the image data
     *
     * @return  None
     *
     *******************************************************************
    */
    void ReplyReceived(const EPndrEvent inRequest,const quint32 inStationToken,
                       const quint32 inArtLength,const quint8 segIndex,
                       const quint8 totalSeg,const QByteArray& data);

//{ added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack

    /**
     *******************************************************************
     * Used for Received update Status.
     *
     *
     * @param[in]  Error code
     *
     * @return  None
     *
     *******************************************************************
    */

    void ReceivedStatus();
//} added by esjang 2013.05.10 for certification # 11frame drops. session start and returns valid ack

//{ modified by yongkyun.lee 2014-08-25 for : Branding Image
    void  ReceivedBranding();
//} modified by yongkyun.lee 2014-08-04 

    /**
     *******************************************************************
     * Used for signaling network errors.
     *
     *
     * @param[in]  Error code
     *
     * @return  None
     *
     *******************************************************************
    */
    void ErrorReceived(const EPndrError inErrCode);

    private:

    /**
     *******************************************************************
     * Returns the event Id for given Response id
     *
     * @param[in]   inResponseId The given response id
     *
     * @return  EPndrEvent The event Id for given response id
     *
     *******************************************************************
    */
    EPndrEvent GetEventIdForResponse(quint8 inResponseId);

    /**
     *******************************************************************
     * Returns the error Id for given Response
     *
     * @param[in]   inResponse The given response
     *
     * @return  EPndrError The error Id for given response
     *
     *******************************************************************
    */
    EPndrError GetErrorIdForResponse(qint8 inResponse, bool isStatus);


    /**
     *******************************************************************
     * Reads a Null terminated String from the QDatastream
     *
     * @param   inDataStream QDataStream already pointing to received data
     *
     * @return  QString the string read
     *
     *******************************************************************
    */
    QString ReadNullTerminatedString(QDataStream& inDataStream);

private:
    /** Instance counter */
    //static int m_nInstanceCounter;

    /** Reference to self */
    //static CPndrFrameDecoder* m_pInstance;

    /** Map of responseId and event */
     QMap <quint8,EPndrEvent> m_RespIdEventMap;

    /** Map of response and error */
     QMap <qint8,EPndrError> m_RespNoticeErrorMap;

     /** Map of response and error */
     QMap <qint8,EPndrError> m_RespStatusErrorMap;


    USE_LOG_UTIL;
};

#endif //DHAVN_PNDR_FRAME_DECODER_H
