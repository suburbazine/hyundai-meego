/* LG Electronics
// Suryanto Tan : suryanto.tan@lge.com
// This file implement the AHA Binary API
// 2012.09.01
*/

//frame encoder and decoder for aha radio.

#include "ahaencoder.h"

//local functions
QByteArray createStationParamByte(uint8_t nStationParamMode, uint8_t &nNumberOfParam);
QByteArray createContentParamByte(uint8_t nStationParamMode, uint8_t &nNumberOfParam);
void prependAhaHeader(QByteArray &pInData, uint16_t nUniqueID, uint16_t nOpCode);

//0x01 AHA_OPCODE_START_PEER_SESSION
QByteArray createStartPeerSessionRequest()
{
    QString str;
    QByteArray byteCommand;
    QDataStream byteCommandDS(&byteCommand, QIODevice::WriteOnly);

    byteCommandDS.setByteOrder(QDataStream::LittleEndian);
    byteCommandDS << (uint16_t) AHA_PROTOCOL_VERSION;
    byteCommandDS << (uint16_t) AHA_START_SESSION_FLAGS;
    byteCommandDS << (uint16_t) AHA_MAX_LENGTH_RETURN;

    str.clear();
    str.append(AHA_MAKE_STRING);
    byteCommandDS <<(uint8_t) str.size();
    byteCommandDS.writeRawData(str.toUtf8().data(),str.size());

    str.clear();
    str.append(AHA_MODEL_STRING);
    byteCommandDS <<(uint8_t) str.size();
    byteCommandDS.writeRawData(str.toUtf8().data(),str.size());

    str.clear();
    str.append(AHA_HU_SERIAL_STRING);
    byteCommandDS <<(uint8_t) str.size();
    byteCommandDS.writeRawData(str.toUtf8().data(),str.size());

    byteCommandDS << (uint8_t) AHA_DELAY_TRACK;

    str.clear();
    str.append(AHA_HU_MODEL_STRING);
    byteCommandDS <<(uint8_t) str.size();
    byteCommandDS.writeRawData(str.toUtf8().data(),str.size());

    str.clear();
    str.append(AHA_CAR_YEAR_STRING);
    byteCommandDS <<(uint8_t) str.size();
    byteCommandDS.writeRawData(str.toUtf8().data(),str.size());

    str.clear();
    str.append(AHA_VIN_STRING);
    byteCommandDS <<(uint8_t) str.size();
    byteCommandDS.writeRawData(str.toUtf8().data(),str.size());

    byteCommandDS << (uint16_t) AHA_MILEAGE;
    byteCommandDS << (uint8_t) SHOUT_FORMAT;

    prependAhaHeader(byteCommand, generateUniqueQueryID(true), AHA_OPCODE_START_PEER_SESSION);
    return byteCommand;
}


//0x04 AHA_OPCODE_QUERY_STATIONS
QByteArray createQueryStationsRequest()
{
    QByteArray byteCommand;
    QDataStream byteCommandDS(&byteCommand, QIODevice::WriteOnly);

    byteCommandDS.setByteOrder(QDataStream::LittleEndian);
    byteCommandDS << (uint8_t) 1;
    byteCommandDS << (uint32_t)0;
    byteCommandDS << (uint32_t)AHA_MAX_NUM_OF_PRESET_STATION;
    prependAhaHeader(byteCommand, generateUniqueQueryID(true), AHA_OPCODE_QUERY_STATIONS);
    return byteCommand;
}


//0x06: AHA_OPCODE_QUERY_STATION_PARAMETERS
//this is only query the parameters of a single station
QByteArray createQuerySingleStationParametersRequest(uint64_t nStationIDtoQuery)
{
    uint8_t nNumberOfParam;
    QByteArray byteCommand;
    QByteArray byteParam;

    QDataStream byteCommandDS(&byteCommand, QIODevice::WriteOnly);
    byteCommandDS.setByteOrder(QDataStream::LittleEndian);

    byteCommandDS << (uint8_t) 1;//anyvalue will do.
    byteCommandDS << (uint8_t) 1;//only query one.
    byteCommandDS << (uint64_t) nStationIDtoQuery; //station id being query

    byteParam = createStationParamByte(QUERY_MODE_COMPLETE,nNumberOfParam);

    byteCommandDS << (uint8_t) nNumberOfParam;
    byteCommand.append(byteParam);

    if(nNumberOfParam != byteParam.size()/2)
    {
        DEBUG_ENCODER("CRITICAL_ERROR: number of param do not match!!!!!!");
        TAN_PRINTF("CRITICAL_ERROR: number of param do not match!!!!!!");
    }
    else
    {
        DEBUG_ENCODER("nNumber of Param and byteParam size checked OK.\n");
        //TAN_PRINTF("nNumber of Param and byteParam size checked OK.\n");
    }
    prependAhaHeader(byteCommand, generateUniqueQueryID(true), AHA_OPCODE_QUERY_STATION_PARAMETERS);
    return byteCommand;
}


//0x06 AHA_OPCODE_QUERY_STATION_PARAMETERS
//query multiple stations at once
QByteArray createQueryMultipleStationParametersRequest(QVector <uint64_t> &nQVectStationIDtoQuery, int nStartIndex)
{
    int i, nEndIndex;
    uint8_t nStationCount;
    uint8_t nNumberOfParam;
    QByteArray byteCommand;
    QByteArray byteParam;

    QDataStream byteCommandDS(&byteCommand, QIODevice::WriteOnly);
    byteCommandDS.setByteOrder(QDataStream::LittleEndian);

    //nStationCount = nQVectStationIDtoQuery.count();
    if((nStartIndex + MAX_PRESET_STATION_PER_QUERY) <= nQVectStationIDtoQuery.count() )
    {
        nStationCount = MAX_PRESET_STATION_PER_QUERY;
    }
    else
    {
        nStationCount = nQVectStationIDtoQuery.count() - nStartIndex;
    }

    byteCommandDS << (uint8_t) 1;//anyvalue will do.
    byteCommandDS << (uint8_t) nStationCount;//only query one.

    //for(i=0; i<nStationCount; i++)
    for(i=nStartIndex; i<nStartIndex+nStationCount; i++)
    {
        byteCommandDS << (uint64_t) nQVectStationIDtoQuery[i]; //station id being query
    }
    byteParam = createStationParamByte(QUERY_MODE_SIMPLE,nNumberOfParam);

    byteCommandDS << (uint8_t) nNumberOfParam;
    byteCommand.append(byteParam);

    if(nNumberOfParam != byteParam.size()/2)
    {
        DEBUG_ENCODER("CRITICAL_ERROR: number of param do not match!!!!!!");
        TAN_PRINTF("CRITICAL_ERROR: number of param do not match!!!!!!");
    }
    else
    {
        DEBUG_ENCODER("nNumber of Param and byteParam size checked OK.\n");
        //TAN_PRINTF("nNumber of Param and byteParam size checked OK.\n");
    }
    prependAhaHeader(byteCommand, generateUniqueQueryID(true), AHA_OPCODE_QUERY_STATION_PARAMETERS);
    return byteCommand;
}

QByteArray createQueryStationContentRequest(uint64_t nStationID)
{
    QByteArray byteCommand;
    QDataStream byteCommandDS(&byteCommand, QIODevice::WriteOnly);

    byteCommandDS.setByteOrder(QDataStream::LittleEndian);
    byteCommandDS << (uint64_t) nStationID;
    byteCommandDS << (uint16_t)0;
    prependAhaHeader(byteCommand, generateUniqueQueryID(true), AHA_OPCODE_QUERY_STATION_CONTENT);
    return byteCommand;
}

//0x08: AHA_OPCODE_QUERY_CONTENT_PARAMETERS
//this is only query the parameters of a single content
QByteArray createQuerySingleContentParametersRequest(uint64_t nContentIDtoQuery)
{
    uint8_t nNumberOfParam;
    QByteArray byteCommand;
    QByteArray byteParam;

    QDataStream byteCommandDS(&byteCommand, QIODevice::WriteOnly);
    byteCommandDS.setByteOrder(QDataStream::LittleEndian);

    byteCommandDS << (uint8_t) 1;   //only query one content
    byteCommandDS << (uint64_t) nContentIDtoQuery;

    byteParam = createContentParamByte(QUERY_MODE_COMPLETE, nNumberOfParam);

    byteCommandDS << (uint8_t) nNumberOfParam; //We will replace this byte later.
    byteCommand.append(byteParam);

    if(nNumberOfParam != byteParam.size()/2)
        DEBUG_ENCODER("CRITICAL_ERROR: number of param do not match!!!!!!");
    else
        DEBUG_ENCODER("nNumber of Param and byteParam size checked OK.\n");

    prependAhaHeader(byteCommand, generateUniqueQueryID(true), AHA_OPCODE_QUERY_CONTENT_PARAMETERS);
    return byteCommand;
}

//0x08: AHA_OPCODE_QUERY_CONTENT_PARAMETERS
//query multiple contents at once.
QByteArray createQueryMultipleContentParametersRequest(QVector <uint64_t> &nQVectContentIDtoQuery, int nStartIndex)
{
    int i, nEndIndex;
    uint8_t nContentCount;
    uint8_t nNumberOfParam;
    QByteArray byteCommand;
    QByteArray byteParam;

    QDataStream byteCommandDS(&byteCommand, QIODevice::WriteOnly);
    byteCommandDS.setByteOrder(QDataStream::LittleEndian);

    if((nStartIndex + MAX_CONTENT_PER_QUERY) <= nQVectContentIDtoQuery.count() )
    {
        nContentCount = MAX_CONTENT_PER_QUERY;
    }
    else
    {
        nContentCount = nQVectContentIDtoQuery.count() - nStartIndex;
    }

    byteCommandDS << (uint8_t) nContentCount;   //only query one content
    for(i = nStartIndex; i < nStartIndex + nContentCount ; i++)
    {
        byteCommandDS << (uint64_t) nQVectContentIDtoQuery[i];
    }

    byteParam = createContentParamByte(QUERY_MODE_SIMPLE, nNumberOfParam);

    byteCommandDS << (uint8_t) nNumberOfParam; //We will replace this byte later.
    byteCommand.append(byteParam);

    if(nNumberOfParam != byteParam.size()/2)
        DEBUG_ENCODER("CRITICAL_ERROR: number of param do not match!!!!!!");
    else
        DEBUG_ENCODER("nNumber of Param and byteParam size checked OK.\n");

    prependAhaHeader(byteCommand, generateUniqueQueryID(true), AHA_OPCODE_QUERY_CONTENT_PARAMETERS);
    return byteCommand;
}

//0x09 AHA_OPCODE_QUERY_BINARY_DATA
QByteArray createQueryBinaryDataRequest(QString strURLString, uint32_t nOffSet,
                                        uint16_t nWidth, uint16_t nHeight)
{
    QByteArray byteCommand;

    QDataStream byteCommandDS(&byteCommand, QIODevice::WriteOnly);
    byteCommandDS.setByteOrder(QDataStream::LittleEndian);

    byteCommandDS << (uint32_t) nOffSet; //offset always set to zero when start querying for the first time
    byteCommandDS << (uint16_t) MAX_LEN_OF_DATA_RET; //max length of data. should be small enough that the data is not fragmented
    byteCommandDS << (uint16_t) strURLString.size();//length of the url
    byteCommandDS.writeRawData(strURLString.toUtf8().data(), strURLString.size());//URL
    byteCommandDS << (uint16_t) nWidth;
    byteCommandDS << (uint16_t) nHeight;
    byteCommandDS << (uint8_t) 1; //keep aspect ratio

    prependAhaHeader(byteCommand, generateUniqueQueryID(true),  AHA_OPCODE_QUERY_BINARY_DATA);
    return byteCommand;
}

//0x0B AHA_OPCODE_QUERY_CURRENT_STATION
QByteArray createQueryCurrentStationRequest()
{
    QByteArray byteCommand;
    prependAhaHeader(byteCommand, generateUniqueQueryID(true),  AHA_OPCODE_QUERY_CURRENT_STATION);
    return byteCommand;
}

//0x0C AHA_OPCODE_SELECT_STATION_FOR_PLAYBACK
QByteArray createSelectStationForPlaybackRequest(uint64_t nStationID)
{
    QByteArray byteCommand;
    QDataStream byteCommandDS(&byteCommand, QIODevice::WriteOnly);
    byteCommandDS.setByteOrder(QDataStream::LittleEndian);

    byteCommandDS << (uint64_t) nStationID; //offset always set to zero
    byteCommandDS << (uint8_t) 0; //always zero, no force to play from start!

    prependAhaHeader(byteCommand, generateUniqueQueryID(true),  AHA_OPCODE_SELECT_STATION_FOR_PLAYBACK);
    return byteCommand;}


//0x0D AHA_OPCODE_SELECT_CONTENT_FOR_PLAYBACK
QByteArray createSelectContentForPlaybackRequest(uint64_t nContentID)
{
    QByteArray byteCommand;
    QDataStream byteCommandDS(&byteCommand, QIODevice::WriteOnly);
    byteCommandDS.setByteOrder(QDataStream::LittleEndian);

    byteCommandDS << (uint64_t) nContentID; //offset always set to zero

    prependAhaHeader(byteCommand, generateUniqueQueryID(true),  AHA_OPCODE_SELECT_CONTENT_FOR_PLAYBACK);
    return byteCommand;
}

//0x0E AHA_OPCODE_CHANGE_CLIENT_PLAYBACK_STATE
QByteArray createChangeClientPlaybackRequest(uint8_t nPlaybackRequest)
{
    QByteArray byteCommand;
    QDataStream byteCommandDS(&byteCommand, QIODevice::WriteOnly);
    byteCommandDS.setByteOrder(QDataStream::LittleEndian);

    byteCommandDS << (uint8_t) nPlaybackRequest; //offset always set to zero

    prependAhaHeader(byteCommand, generateUniqueQueryID(true),  AHA_OPCODE_CHANGE_CLIENT_PLAYBACK_STATE);
    return byteCommand;
}

//0x0F AHA_OPCODE_TIME_SHIFT_CONTENT
QByteArray createTimeShiftContentRequest(uint16_t nSeconds)
{
    QByteArray byteCommand;
    QDataStream byteCommandDS(&byteCommand, QIODevice::WriteOnly);
    byteCommandDS.setByteOrder(QDataStream::LittleEndian);

    byteCommandDS << (uint16_t) nSeconds;

    prependAhaHeader(byteCommand, generateUniqueQueryID(true),  AHA_OPCODE_TIME_SHIFT_CONTENT);
    return byteCommand;
}


//0x11 Perform Action
QByteArray createPerformActionRequest(uint32_t nActionEnumValue, uint64_t nContentID)
{
    QByteArray byteCommand;
    QDataStream byteCommandDS(&byteCommand, QIODevice::WriteOnly);
    byteCommandDS.setByteOrder(QDataStream::LittleEndian);

    byteCommandDS << (uint32_t) nActionEnumValue;
    byteCommandDS << (uint64_t) nContentID;

    prependAhaHeader(byteCommand, generateUniqueQueryID(true),  AHA_OPCODE_PERFORM_ACTION);
    return byteCommand;
}

//0x12
QByteArray createRequestCurrentContentInfo()
{
    QByteArray byteCommand;
    QDataStream byteCommandDS(&byteCommand, QIODevice::WriteOnly);
    byteCommandDS.setByteOrder(QDataStream::LittleEndian);

    byteCommandDS << (uint8_t) 0; //start index
    byteCommandDS << (uint8_t) 3;//number of string to return
    byteCommandDS << (uint16_t) 60;     //wsuk.kim_130628_ITS_0177003 40->60... words

    prependAhaHeader(byteCommand, generateUniqueQueryID(true),  AHA_OPCODE_REQUEST_CURRENT_CONTENT_INFORMATION);
    return byteCommand;
}

void prependAhaHeader(QByteArray &pInData, uint16_t nUniqueID, uint16_t nOpCode)
{
	//|2: 0xB6B6|2: Length|2: ID|2: OpCode|2: Checksum|
    uint16_t nCheckSum = 0;
    QByteArray checksumQByte;
    QDataStream checksumQDS(&checksumQByte, QIODevice::WriteOnly);
    checksumQDS.setByteOrder(QDataStream::LittleEndian);

    QByteArray header;
	QDataStream headerDS(&header, QIODevice::WriteOnly);
	headerDS.setByteOrder(QDataStream::LittleEndian);	
    headerDS<< (uint16_t) START_AHA_PACKAGE;
    headerDS<< (uint16_t) (pInData.size() + AHA_HEADER_LENGTH);
    headerDS<< (uint16_t) nUniqueID;
    headerDS<< (uint16_t) nOpCode;
    headerDS<<(nCheckSum);
	pInData.prepend(header);
    nCheckSum = calculateCheckSum(pInData);
    checksumQDS << nCheckSum;
    pInData.replace(8,2,checksumQByte);
}

QByteArray createContentParamByte(uint8_t nContentParamMode, uint8_t &nNumberOfParam)
{
    QByteArray byteParamToQuery;

    QDataStream byteParamToQueryDS(&byteParamToQuery, QIODevice::WriteOnly);
    byteParamToQueryDS.setByteOrder(QDataStream::LittleEndian);

    nNumberOfParam = 0;
    switch(nContentParamMode)
    {
    case QUERY_MODE_COMPLETE:
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_LIKED;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_CAN_PAUSE;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_CAN_NEXT;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_CAN_BACK;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_CAN_CHANGE;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_CAN_TIMESHIFT;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_CAN_LIKE;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_CAN_DISLIKE;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_CAN_UNVOTE;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_DISLIKED;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_LON;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_LAT;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_RATING;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_TITLE;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_IMAGE_URL;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_ZIP;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_ADDRESS1;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_STATE;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_CITY;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_PHONE;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_FULL_ADDRESS;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_ADDRESS2;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_COUNTRY_CODE;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_LANG_CODE;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_RATING_IMG_URL;
        nNumberOfParam++;
        break;
    case QUERY_MODE_SIMPLE:
        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_TITLE;
        nNumberOfParam++;
//hsryu_0530_delete_content_url
//        byteParamToQueryDS << (uint16_t) AHA_CONT_PAR_IMAGE_URL;
//        nNumberOfParam++;
        break;
    default:
        break;
    }
    return byteParamToQuery;
}

QByteArray createStationParamByte(uint8_t nStationParamMode, uint8_t &nNumberOfParam)
{
    QByteArray byteParamToQuery;

    QDataStream byteParamToQueryDS(&byteParamToQuery, QIODevice::WriteOnly);
    byteParamToQueryDS.setByteOrder(QDataStream::LittleEndian);

    nNumberOfParam = 0;
    switch(nStationParamMode)
    {
    case QUERY_MODE_COMPLETE:
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_LIKE_ALLOWED;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_READ_ONLY;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_TWITTER;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_LBS;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_TIMESHIFT;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_DYNAMIC;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_FACEBOOK;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_POST_TEXT;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_RECORD_TIME;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_ST_ICON_URL;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_FULL_NAME;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_PARTNER_URL;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_LIKE_U_URL;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_DISLIKE_U_URL;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_LIKE_S_URL;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_DISLIKE_S_URL;
        nNumberOfParam++;
        break;
    case QUERY_MODE_SIMPLE:
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_ST_ICON_URL;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_FULL_NAME;
        nNumberOfParam++;
        byteParamToQueryDS << (uint16_t) AHA_STT_PAR_LBS;
        nNumberOfParam++;
        break;
    default:
        break;
    }
    return byteParamToQuery;
}

