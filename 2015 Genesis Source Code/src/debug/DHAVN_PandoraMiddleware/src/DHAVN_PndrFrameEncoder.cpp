/*
 ******************************************************************
 *        COPYRIGHT  2011, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     : Frame Encoder                                     *
 * PROGRAMMER : Snigdha Satapathy                                 *
 * DATE       : 07/12/2011                                        *
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
 *  07/12/2011  0.1 Snigdha Satapathy       cpp file created
 *
 ******************************************************************
 */

#include <QDataStream>
#include "DHAVN_PndrFrameEncoder.h"
#include "DHAVN_PndrLinkConstants.h"
#include "crc.h"

//Static variables initialization
//int CPndrFrameEncoder::m_nInstanceCounter = 0;

//CPndrFrameEncoder* CPndrFrameEncoder::m_pInstance = 0;

const int c_OneByte = 1;
const int c_TwoBytes = 2;

const int c_charArraySize = 248;
const int c_accId_size = 8;

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
/*
CPndrFrameEncoder* CPndrFrameEncoder::GetInstance()
{
    USE_LOG_UTIL;
    LOG_INIT_V2("Pandora");
    LOG_TRACE<< "CPndrFrameEncoder::GetInstance:Entry" <<LOG_ENDL;

    if(m_pInstance == NULL)
    {
        m_pInstance = new CPndrFrameEncoder();
    }

    //The counter is incremented only if m_pInstance allocation is succesfull.
    if(m_pInstance)
    {
        m_nInstanceCounter++;
        LOG_TRACE<< "Encoder::m_nInstanceCounter = " << m_nInstanceCounter<<LOG_ENDL;

    }
    else
    {
        LOG_INFO << "Encoder Init Failed." << LOG_ENDL;
    }

    LOG_TRACE<< "CPndrFrameEncoder::GetInstance:Exit" <<LOG_ENDL;

    return m_pInstance;
}
*/
void CPndrFrameEncoder::SetValidator(CPndrFrameValidator* inFrameValidator)
{
    m_pFrameValidator = inFrameValidator ;
}

/**
 *******************************************************************
 * Releases instance of CPndrFrameEncoder
 *
 * @param    None
 *
 * @return     None
 *
 *******************************************************************
 */
/*
void CPndrFrameEncoder::ReleaseInstance()
{
//    USE_LOG_UTIL;
//    LOG_INIT_V2("Pandora");

    LOG_TRACE<< "CPndrFrameEncoder::ReleaseInstance:Entry" <<LOG_ENDL;

    LOG_TRACE<< "Encoder Instance to be deleted is m_nInstanceCounter = " << m_nInstanceCounter <<LOG_ENDL;

    m_nInstanceCounter--;
    if (!m_nInstanceCounter)
    {
        LOG_TRACE<< "CPndrFrameEncoder::Instance deleted" <<LOG_ENDL;
        delete m_pInstance;
        m_pInstance = NULL;
    }

   // LOG_TRACE<< "CPndrFrameEncoder::ReleaseInstance:Exit" <<LOG_ENDL;

}
*/
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
 * @return     None
 *
 *******************************************************************
 */
void CPndrFrameEncoder::EncodeAndSend(const quint8 inCommandCode)
{
    //LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Entry" << "inCommandCode =" << inCommandCode<<LOG_ENDL;
    QByteArray encodedFrame;

    QByteArray payload;
    QDataStream payloadDs(&payload,QIODevice::WriteOnly);
    payloadDs.setByteOrder(QDataStream::BigEndian);

    CreateDataFrameHeader(encodedFrame);

    quint32 payloadLen = sizeof(quint8);
    payloadDs << payloadLen << inCommandCode;
    encodedFrame.append(payload);

    AppendFrameTrailerAndSend(encodedFrame);

    //LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Exit" <<LOG_ENDL;

}


/**
 *******************************************************************
 * Encode the connect command
 *
 * @param[in]   inCommandCode: The command to be encoded
 * @param[in]   inApiVersion: API version
 * @param[in]   inAccessoryId: Accessory ID
 * @param[in]   inAlbumArtDimension: Expected AlbumArt Size
 * @param[in]   inImageType: Expected AlbumArt Image type
 * @param[in]   inFlag: Various flag for debugging purpose
 * @param[in]   inStationArtDimension size of station art or genre station art image
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrFrameEncoder::EncodeAndSend(const quint8 inCommandCode,const quint16 inApiVersion,
                                     const QString& inAccessoryId,const quint16 inAlbumArtDimension,
                                     const quint8 inImageType,const quint8 inFlag,
                                     const quint16 inStationArtDimension,bool inAppendToQueue)
{
    //LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Entry" << "inCommandCode =" << inCommandCode <<LOG_ENDL;

    QByteArray encodedFrame;

    QByteArray payload;
    QDataStream payloadDs(&payload,QIODevice::WriteOnly);
    payloadDs.setByteOrder(QDataStream::BigEndian);

    //resize is not setting all bytes to 0. Hence used this method.
    QString accId(c_accId_size, 0);
    accId.insert(0,inAccessoryId);

    CreateDataFrameHeader(encodedFrame);

    quint32 payloadlen = sizeof(quint8) + sizeof(quint16) + c_accId_size +sizeof(quint16)
            + sizeof(quint8)+ sizeof(quint8) + sizeof(quint16);

    payloadDs << payloadlen <<inCommandCode <<inApiVersion;
    payloadDs.writeRawData(accId.toUtf8().data(),c_accId_size);
    payloadDs << inAlbumArtDimension <<inImageType <<inFlag<< inStationArtDimension ;

   
    encodedFrame.append(payload);

    AppendFrameTrailerAndSend(encodedFrame,inAppendToQueue);

    //LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Exit" <<LOG_ENDL;

}

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
 * @return     None
 *
 *******************************************************************
 */
void CPndrFrameEncoder::EncodeAndSend(const quint8 inCommandCode,const quint32 inData)
{
    LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Entry 1" << "inCommandCode =" << inCommandCode <<LOG_ENDL;

    QByteArray encodedFrame;
    QByteArray payload;
    QDataStream payloadDs(&payload,QIODevice::WriteOnly);
    payloadDs.setByteOrder(QDataStream::BigEndian);

    CreateDataFrameHeader(encodedFrame);

    quint32 payloadlen = sizeof(quint8) + sizeof(quint32);

    payloadDs << payloadlen <<inCommandCode << inData ;
    encodedFrame.append(payload);

    AppendFrameTrailerAndSend(encodedFrame);

    //LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Encode the command which needs quint8 data as input
 *
 * @param[in]   inCommandCode: The command to be encoded
 *              setTrackElapsedPolling, stationsSort
 * @param[in]   inData: The to be encoded along with command.
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrFrameEncoder::EncodeAndSend(const quint8 inCommandCode,quint8 inEnabled)
{
    LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Entry 2" << "inCommandCode =" << inCommandCode <<LOG_ENDL;

    QByteArray encodedFrame;
    QByteArray payload;
    QDataStream payloadDs(&payload,QIODevice::WriteOnly);
    payloadDs.setByteOrder(QDataStream::BigEndian);

    CreateDataFrameHeader(encodedFrame);

    quint32 payloadlen = sizeof(quint8) + sizeof(quint8);

    payloadDs << payloadlen <<inCommandCode << inEnabled ;
    encodedFrame.append(payload);

    AppendFrameTrailerAndSend(encodedFrame);

    //LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Encode the command which needs qlist data as input
 *
 * @param[in]   inCommandCode: The command to be encoded
 *              getStationInfo
 * @param[in]   inDataArray: The to be encoded along with command.

 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrFrameEncoder::EncodeAndSend(const quint8 inCommandCode,const QList<quint32>& inDataArray)
{
    LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Entry 3" << "inCommandCode =" << inCommandCode <<LOG_ENDL;

    QByteArray payload;
    QDataStream payloadDs(&payload,QIODevice::WriteOnly);
    payloadDs.setByteOrder(QDataStream::BigEndian);


    quint32 payloadLen = sizeof(inCommandCode) + sizeof(quint32) * inDataArray.count();

    QByteArray encodedFrame;

    CreateDataFrameHeader(encodedFrame);

    payloadDs << payloadLen << inCommandCode;

    for(int i=0;i<inDataArray.count();i++)
    {
        payloadDs<< inDataArray[i];
    }

    encodedFrame.append(payload);

    AppendFrameTrailerAndSend(encodedFrame);

    //LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Exit" <<LOG_ENDL;


}

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
void CPndrFrameEncoder::EncodeAndSend(const quint8 inCommandCode,const quint32 inData,
            const QString& inDataArray)
{
   // LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Entry" << "inCommandCode =" << inCommandCode <<LOG_ENDL;
    QByteArray encodedFrame;
    QByteArray payload;
    QDataStream payloadDs(&payload,QIODevice::WriteOnly);
    payloadDs.setByteOrder(QDataStream::BigEndian);

    //If input Search text is > 247 chars, truncate it to 247 chars and append NULL at end.
    int searchTextLen = inDataArray.length();
    if(inDataArray.length() >= c_charArraySize)
    {
        searchTextLen = c_charArraySize - c_OneByte;
    }

    quint32 payloadLen = sizeof(inCommandCode) +
                        sizeof(inData) +
                        searchTextLen + //Search String variable length
                        c_OneByte ;    // For Null terminator

    CreateDataFrameHeader(encodedFrame);

    //resize is not setting all bytes to 0. Hence used this method.
    QString dataArr(searchTextLen + c_OneByte, 0);
    dataArr.insert(0,inDataArray.left(searchTextLen)); //Last Byte remains NULL

    payloadDs<<payloadLen <<inCommandCode<<inData;
    payloadDs.writeRawData(dataArr.toUtf8().data(),searchTextLen+c_OneByte);
    encodedFrame.append(payload);

    AppendFrameTrailerAndSend(encodedFrame);

    //LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Encode the command which needs quint32 & QList data as input
 *
 * @param[in]   inCommandCode: The command to be encoded
 *              getSearchResultInfo
 * @param[in]   inData: The to be encoded along with command.
 * @param[in]   inDataArray: The to be encoded along with command.

 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrFrameEncoder::EncodeAndSend(const quint8 inCommandCode,const quint32 inData,
                    const QList<quint32>& inDataArray)
{
    //LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Entry" << "inCommandCode =" << inCommandCode <<LOG_ENDL;
    QByteArray encodedFrame;

    QByteArray payload;
    QDataStream payloadDs(&payload,QIODevice::WriteOnly);
    payloadDs.setByteOrder(QDataStream::BigEndian);


    quint32 payloadLen = sizeof(inCommandCode) +
                        sizeof(inData) +
                        sizeof(quint32)* inDataArray.count();


    CreateDataFrameHeader(encodedFrame);

    payloadDs << payloadLen << inCommandCode << inData;

    for(int i=0;i<inDataArray.count();i++)
    {
        payloadDs<<inDataArray[i];
    }

    encodedFrame.append(payload);

    AppendFrameTrailerAndSend(encodedFrame);

    //LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Exit" <<LOG_ENDL;


}

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
void CPndrFrameEncoder::EncodeAndSend(const quint8 inCommandCode,const quint32 inData1,
                    const quint32 inData2)
{
    //LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Entry" << "inCommandCode =" << inCommandCode <<LOG_ENDL;
    QByteArray encodedFrame;

    QByteArray payload;
    QDataStream payloadDs(&payload,QIODevice::WriteOnly);
    payloadDs.setByteOrder(QDataStream::BigEndian);

    CreateDataFrameHeader(encodedFrame);

    quint32 payloadLen = sizeof(quint8) + sizeof(quint32) + sizeof(quint32);

    payloadDs << payloadLen << inCommandCode <<inData1 << inData2;

    encodedFrame.append(payload);

    AppendFrameTrailerAndSend(encodedFrame);

   // LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Exit" <<LOG_ENDL;

}

//{ modified by yongkyun.lee 2014-08-25 for : Branding Image
void CPndrFrameEncoder::EncodeAndSend( quint8 inCommandCode, const quint8 SegIndex , const quint8 totalSeg, quint8* inData, int fzise )
{
    LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Entry inCommandCode =" << inCommandCode <<" , sizeof(inData) =" << fzise <<LOG_ENDL;
    LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:SegIndex=" << SegIndex <<" , totalSeg =" << totalSeg  <<LOG_ENDL;
 
    QByteArray encodedFrame;
    QByteArray payload;
    QDataStream payloadDs(&payload,QIODevice::WriteOnly);
    payloadDs.setByteOrder(QDataStream::BigEndian);

    CreateDataFrameHeader(encodedFrame);

    quint32 payloadlen = sizeof(inCommandCode) +sizeof(SegIndex) +sizeof(totalSeg) + fzise;

    payloadDs << payloadlen <<inCommandCode << SegIndex << totalSeg  ;
    payloadDs.writeRawData((char *)inData,fzise);

    encodedFrame.append(payload);

    AppendFrameTrailerAndSend(encodedFrame);

    LOG_TRACE<< "CPndrFrameEncoder::EncodeAndSend:Exit" <<LOG_ENDL;
    return;
}
//} modified by yongkyun.lee 2014-08-04 


/**
* Constructor
*/
CPndrFrameEncoder::CPndrFrameEncoder()
{
    LOG_INIT_V2("Pandora");

    //m_pFrameValidator = CPndrFrameValidator::GetInstance();

    m_nSeqNo = PNDR_FRAME_SEQUENCE_0;
    m_pFrameValidator = NULL;   // added by esjang 2013.05.23 for static test # 20899
    
     LOG_TRACE<< "CPndrFrameEncoder::CPndrFrameEncoder() created" <<LOG_ENDL;

}



/**
 *******************************************************************
 * Destructor
 *******************************************************************
 */
CPndrFrameEncoder::~CPndrFrameEncoder()
{
    LOG_TRACE<< "CPndrFrameEncoder::~CPndrFrameEncoder:Entry" <<LOG_ENDL;
//    if(m_pFrameValidator)
//    {
//        m_pFrameValidator->ReleaseInstance();
//        m_pFrameValidator = NULL;
//    }

    m_pFrameValidator = NULL;

    LOG_TRACE<< "CPndrFrameEncoder::~CPndrFrameEncoder:Exit" <<LOG_ENDL;
}

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
void CPndrFrameEncoder::EncodeEscapeCharacters(QByteArray& inEncodedFrame)
{
    //LOG_TRACE<< "CPndrFrameEncoder::EncodeEscapeCharacters:Entry" <<LOG_ENDL;

    //start with i=1 and go till last but one byte
    //since start and end byte are not to be escaped
    for(int i=1;i<inEncodedFrame.count()-1;i++)
    {
        if( (uint)inEncodedFrame[i] == PNDR_FRAME_START)
        {
            inEncodedFrame[i] = PNDR_FRAME_ESCAPE;
            inEncodedFrame.insert(i+1,PNDR_FRAME_ESCAPE_START);
        }
        else if( (uint)inEncodedFrame[i] == PNDR_FRAME_END)
        {
            inEncodedFrame[i] = PNDR_FRAME_ESCAPE;
            inEncodedFrame.insert(i+1,PNDR_FRAME_ESCAPE_END);
        }
        else if((uint)inEncodedFrame[i] == PNDR_FRAME_ESCAPE)
        {
            inEncodedFrame[i] = PNDR_FRAME_ESCAPE;
            inEncodedFrame.insert(i+1,PNDR_FRAME_ESCAPE_ESCAPE);
        }
    }

   //LOG_TRACE<< "CPndrFrameEncoder::EncodeEscapeCharacters:Exit" <<LOG_ENDL;

}


/**
 *******************************************************************
 * Calculates the CRC for the encoded frame
 *
 * @param[in]   inCRCData: The encoded frame whose CRC is
 *              to be calculated

 *
 * @return     The CRC for given data
 *
 *******************************************************************
 */
quint16 CPndrFrameEncoder:: CalculateCRC(QByteArray& inCRCData,int len)
{
//    USE_LOG_UTIL;
//    LOG_INIT_V2("Pandora");
//    LOG_TRACE<< "CPndrFrameEncoder::CalculateCRC:Entry" <<LOG_ENDL;

    unsigned char* crcdata = (unsigned char*)inCRCData.data();
    crcInit();
    quint16 fast = crcFast(crcdata,len);

    //LOG_INFO << "CRC = " << fast;
    //LOG_TRACE<< "CPndrFrameEncoder::CalculateCRC:Exit" <<LOG_ENDL;

    return fast;
}

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
void CPndrFrameEncoder:: CreateDataFrameHeader(QByteArray& encodedFrame)
{
    //LOG_TRACE<< "CPndrFrameEncoder::CreateDataFrameHeader:Entry" <<LOG_ENDL;
    encodedFrame.resize(ePayloadLenBytePos1);
    encodedFrame[eStartBytePos]=  PNDR_FRAME_START;
    encodedFrame[eTypeBytePos]=  PNDR_FRAME_TYPE_DATA;
    encodedFrame[eSeqNoBytePos]=  m_nSeqNo;
    m_nSeqNo = m_nSeqNo ^1 ;
    //LOG_TRACE<< "CPndrFrameEncoder::CreateDataFrameHeader:Exit" <<LOG_ENDL;
}

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
void CPndrFrameEncoder:: AppendFrameTrailerAndSend(QByteArray& encodedFrame,bool inAppendToQueue)
{
    //LOG_TRACE<< "CPndrFrameEncoder::AppendFrameTrailerAndSend:Entry" <<LOG_ENDL;
    AppendCRCtoDataFrame(encodedFrame);
    encodedFrame.append(PNDR_FRAME_END);
    EncodeEscapeCharacters(encodedFrame);
    //LOG_TRACE << "Encoded Frame to be sent is " << encodedFrame.toHex()<<LOG_ENDL;
    m_pFrameValidator->Send(encodedFrame,inAppendToQueue);
    //LOG_TRACE<< "CPndrFrameEncoder::AppendFrameTrailerAndSend:Exit" <<LOG_ENDL;
}

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
void CPndrFrameEncoder:: AppendCRCtoDataFrame(QByteArray& encodedFrame)
{
   // LOG_TRACE<< "CPndrFrameEncoder::AppendCRCtoDataFrame:Entry" <<LOG_ENDL;

    QByteArray crcData;
    QDataStream crcDs(&crcData,QIODevice::WriteOnly);
    crcDs.setByteOrder(QDataStream::BigEndian);
    //Calculate the CRC for the encodedFrame excluding the first byte(start byte)
    QByteArray crcByteArray = encodedFrame.right(encodedFrame.size()-c_OneByte);
    quint16 crc = CalculateCRC(crcByteArray,
                               encodedFrame.size()-c_OneByte);
    crcDs << crc;
    //Append the CRC to the encoded frame
    encodedFrame.append(crcData);
    //LOG_TRACE<< "CPndrFrameEncoder::AppendCRCtoDataFrame:Exit" <<LOG_ENDL;

}

//End Of File
