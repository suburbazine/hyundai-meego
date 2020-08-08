/* LG Electronics
// Suryanto Tan : suryanto.tan@lge.com
// This file implement the AHA Binary API
// 2012.09.01.01
*/

#include "ahadecoder.h"

//local function list
uint16_t checkErrorCode(QByteArray &byteArray);

//0x01
uint16_t decodeStartPeerSession(QByteArray &byteArray, AhaClientInfo_T &AhaClientInfo)
{
    uint16_t nErrorCode;
    uint16_t nLength;
    uint8_t nGPSAvailable;
    uint16_t nAppVersionLength, nDevModelLength, nDevSoftVersionLength;
    QString strAppVersion, strDevModel, strDevSoftVersion;
    uint16_t nextByteIndex;

    DEBUG_DECODER("\n decodeStartPeerSession \n");
    nErrorCode = checkErrorCode(byteArray);
    TAN_PRINTF("========decodeStartPeerSession=========[%d]\n",nErrorCode);

    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        DEBUG_DECODER("\nNo error\n");
        //check if the length is correct

        //AhaClientInfo.clear();

        nLength = byteToUint16_t(byteArray, 2);

        nextByteIndex = 10;

        nGPSAvailable = byteArray[nextByteIndex];
        nextByteIndex = nextByteIndex + 1;

        nAppVersionLength = byteToUint16_t(byteArray, nextByteIndex);
        nextByteIndex = nextByteIndex + 2;

        strAppVersion = byteToQString(byteArray, nextByteIndex,nAppVersionLength);
        nextByteIndex = nextByteIndex + nAppVersionLength;

        nDevModelLength = byteToUint16_t(byteArray,nextByteIndex);
        nextByteIndex = nextByteIndex + 2;

        strDevModel = byteToQString(byteArray, nextByteIndex, nDevModelLength);
        nextByteIndex = nextByteIndex + nDevModelLength;

        nDevSoftVersionLength = byteToUint16_t(byteArray, nextByteIndex);
        nextByteIndex = nextByteIndex + 2;

        strDevSoftVersion = byteToQString(byteArray, nextByteIndex, nDevSoftVersionLength);
        nextByteIndex = nextByteIndex + nDevSoftVersionLength;

        AhaClientInfo.nGPSAvailable = nGPSAvailable;
        AhaClientInfo.strAppVersion.clear();
        AhaClientInfo.strAppVersion = strAppVersion;
        AhaClientInfo.strDevModel.clear();
        AhaClientInfo.strDevModel = strDevModel;
        AhaClientInfo.strDevSoftVersion.clear();
        AhaClientInfo.strDevSoftVersion = strDevSoftVersion;

        DEBUG_DECODER("\t nGPSAvailable: %d\n", nGPSAvailable);
        DEBUG_DECODER("\t nAppVersionLength: %d\n", nAppVersionLength);
        DEBUG_DECODER("\t strAppVersion: %s\n", strAppVersion.toUtf8().data());
        DEBUG_DECODER("\t nDevModelLength: %d\n", nDevModelLength);
        DEBUG_DECODER("\t strDevModel: %s\n", strDevModel.toUtf8().data());
        DEBUG_DECODER("\t nDevSoftVersionLength: %d\n", nDevSoftVersionLength);
        DEBUG_DECODER("\t strDevSoftVersion: %s\n", strDevSoftVersion.toUtf8().data());
        break;
    case AHA_ERR_PROT_NOT_SUPPORTED:
        DEBUG_DECODER("\nProtocol not supported\n");
        break;
    case AHA_ERR_SESSION_REJECTED:
        DEBUG_DECODER("\nSession rejected\n");
        break;
    case AHA_ERR_NOT_LOGGED_IN:
        DEBUG_DECODER("\nNot logged in\n");
        break;
    case AHA_ERR_LOGIN_FAILED:
        DEBUG_DECODER("\nUser login failed\n");
        break;
    default:
        break;
    }
    return nErrorCode;
}

//0x04
uint16_t decodeQueryStations(QByteArray &byteArray, AhaClientInfo_T &AhaClientInfo)
{
    uint16_t nErrorCode;
    uint16_t nLength;
    uint32_t nTotalNumberOfStations, nCurrentNumberOfStations;
    uint64_t nStationID;
    uint16_t i;
    uint16_t nextByteIndex;

    DEBUG_DECODER("\n decodeQueryStations \n");
    nErrorCode = checkErrorCode(byteArray);
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:

        nLength = byteToUint16_t(byteArray, 2);

        if(nLength < 18)
        {
            DEBUG_DECODER("nLength Error: breaking out\n");
            return 0;
        }

        nextByteIndex = 10;

        nTotalNumberOfStations = byteToUint32_t(byteArray, nextByteIndex);
        nextByteIndex = nextByteIndex + 4;

        nCurrentNumberOfStations = byteToUint32_t(byteArray, nextByteIndex);
        nextByteIndex = nextByteIndex + 4;

        DEBUG_DECODER("nTotalNumberOfStations: %d\n", nTotalNumberOfStations);
        DEBUG_DECODER("nCurrentNumberOfStations: %d\n", nCurrentNumberOfStations);

        AhaClientInfo.nQVectPresetStationID.clear();

        for(i = 0 ; i< nCurrentNumberOfStations; i++)
        {
            nStationID = byteToUint64_t(byteArray, nextByteIndex);
            nextByteIndex = nextByteIndex + 8;
            AhaClientInfo.nQVectPresetStationID.append(nStationID);
            DEBUG_DECODER("nStationIDArray[%d]: %llu\n", i, nStationID);
        }

        break;
    default:
        DEBUG_DECODER("decodeQueryStations returns error\n");
        break;
    }
    return nErrorCode;
}

//0x07
uint16_t decodeQueryStationContent(QByteArray &byteArray, AhaClientInfo_T &AhaClientInfo)
{
    uint16_t nErrorCode;
    uint16_t nLength;
    uint16_t nTotalNumberOfContent, nCurrentNumberOfContent;
    uint64_t nContentID;
    uint16_t i;
    uint16_t nextByteIndex;

    DEBUG_DECODER("\n decodeQueryStationContent \n");
    nErrorCode = checkErrorCode(byteArray);
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:

        nLength = byteToUint16_t(byteArray, 2);

        if(nLength < 14)
        {
            DEBUG_DECODER("nLength Error: breaking out\n");
            return 0;
        }

        nextByteIndex = 10;

        nTotalNumberOfContent = byteToUint16_t(byteArray, nextByteIndex);
        nextByteIndex = nextByteIndex + 2;

        nCurrentNumberOfContent = byteToUint16_t(byteArray, nextByteIndex);
        nextByteIndex = nextByteIndex + 2;

        DEBUG_DECODER("nTotalNumberOfContent: %d\n", nTotalNumberOfContent);
        DEBUG_DECODER("nCurrentNumberOfContent: %d\n", nCurrentNumberOfContent);

        AhaClientInfo.nQVectListContentID.clear();

        for(i = 0 ; i< nCurrentNumberOfContent; i++)
        {
            nContentID = byteToUint64_t(byteArray, nextByteIndex);
            nextByteIndex = nextByteIndex + 8;
            AhaClientInfo.nQVectListContentID.append(nContentID);
            DEBUG_DECODER("nContentID[%d]: %llu\n", i, nContentID);
        }

        break;
    default:
        DEBUG_DECODER("decodeQueryStationContent returns error\n");
        break;
    }
    return nErrorCode;
}


//0x06

uint16_t decodeQuerySingleStationParameters(QByteArray &byteArray, AhaStationInfo_T &AhaStationInfo)
{
    uint16_t nErrorCode;
    uint16_t nLength, nextByteIndex, i, j;
    uint8_t nNumOfStation, nNumOfParam;
    uint64_t nStationID = 0;
    uint16_t nStationParam;
    uint16_t tempStringLength;

    uint8_t nParamDataType;

    //ok, here is the 11 parameters
    uint8_t nLikeAllowed = 0;
    uint8_t nReadOnly = 0;
    uint8_t nIsTwitter = 0;
    uint8_t nIsLBS = 0;
    uint8_t nCanTimeShift = 0;
    uint8_t nIsDynamic = 0;
    uint8_t nIsFacebook = 0;
    uint8_t nCanPostText = 0;
    uint64_t nRecordTime = 0;
    QString strStationIconURL;
    QString strStationFullName;
    QString strPartnerURL;
    QString strLikeSelectedURL;
    QString strDislikeSelectedURL;
    QString strLikeUnselectedURL;
    QString strDislikeUnselectedURL;

    DEBUG_DECODER("\n decodeQueryStationParameters \n");

    nErrorCode = checkErrorCode(byteArray);
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        //let us decode.
        DEBUG_DECODER("\nNo error\n");

        //AhaStationInfo.clear();

        nLength = byteToUint16_t(byteArray, 2);

        nextByteIndex = 10;

        nNumOfStation = byteArray[nextByteIndex];
        nextByteIndex = nextByteIndex + 1;

        for(i=0;i<nNumOfStation;i++)
        {
            nStationID = byteToUint64_t(byteArray, nextByteIndex);
            nextByteIndex = nextByteIndex + 8;

            nNumOfParam = byteArray[nextByteIndex];
            nextByteIndex = nextByteIndex + 1;

            DEBUG_DECODER(" nStationID %llu\n",nStationID);
            DEBUG_DECODER(" nNumOfParam %d\n",nNumOfParam);

            for(j=0;j<nNumOfParam;j++)
            {
                nStationParam = byteToUint16_t(byteArray, nextByteIndex);
                nextByteIndex = nextByteIndex + 2;

                DEBUG_DECODER(" nStationParam 0x%04x\n",nStationParam);

                switch(nStationParam)
                {
                case AHA_STT_PAR_LIKE_ALLOWED:
                    nLikeAllowed = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_STT_PAR_READ_ONLY:
                    nReadOnly = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_STT_PAR_TWITTER:
                    nIsTwitter = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_STT_PAR_LBS:
                    nIsLBS = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_STT_PAR_TIMESHIFT:
                    nCanTimeShift = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_STT_PAR_DYNAMIC:
                    nIsDynamic = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_STT_PAR_FACEBOOK:
                    nIsFacebook = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_STT_PAR_POST_TEXT:
                    nCanPostText = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_STT_PAR_RECORD_TIME:
                    nRecordTime = byteToUint64_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 8;
                    break;
                case AHA_STT_PAR_ST_ICON_URL:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strStationIconURL = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_STT_PAR_FULL_NAME:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strStationFullName = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_STT_PAR_PARTNER_URL:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strPartnerURL = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_STT_PAR_LIKE_S_URL:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strLikeSelectedURL = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_STT_PAR_LIKE_U_URL:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strLikeUnselectedURL = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_STT_PAR_DISLIKE_S_URL:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strDislikeSelectedURL = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_STT_PAR_DISLIKE_U_URL:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strDislikeUnselectedURL = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                default:
                    //Even if we do not care about this parameter,
                    //we still have to advance the nextbyteindex!!!
                    //extract four significant bits from nStationParam to determine the type.
                    //0: boolean 1: UINT 2: INT 3:REAL(DOUBLE) 4:STRING
                    nParamDataType = (nStationParam >> 12);
                    switch(nParamDataType)
                    {
                    case AHA_BOOLEAN_PARAM:
                        nextByteIndex = nextByteIndex + 1;
                        DEBUG_DECODER("ignore 1byte in station param\n");
                        break;
                    case AHA_UINT64_PARAM:
                    case AHA_REAL_PARAM:
                    case AHA_INT64_PARAM:
                        DEBUG_DECODER("ignore 4byte in station param\n");
                        nextByteIndex = nextByteIndex + 8;
                        break;
                    case AHA_STRING_PARAM:
                        tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                        nextByteIndex = nextByteIndex + 2;
                        nextByteIndex = nextByteIndex + tempStringLength;
                        DEBUG_DECODER("ignore %dbyte in station param\n", tempStringLength + 2);
                        break;
                    default:
                        //this will not happen!
                        break;
                    }
                    break;
                }
            }//end of number of param loop
        }//end of number of station loop

        //let us assign and print the values.
        AhaStationInfo.nStationID = nStationID;
        AhaStationInfo.nLikeAllowed = nLikeAllowed;
        AhaStationInfo.nReadOnly = nReadOnly;
        AhaStationInfo.nIsTwitter = nIsTwitter;
        AhaStationInfo.nIsLBS = nIsLBS;
        AhaStationInfo.nCanTimeShift = nCanTimeShift;
        AhaStationInfo.nIsDynamic = nIsDynamic;
        AhaStationInfo.nIsFacebook = nIsFacebook;
        AhaStationInfo.nCanPostText = nCanPostText;
        AhaStationInfo.nRecordTime = nRecordTime;
        AhaStationInfo.strStationIconURL = strStationIconURL ;
        AhaStationInfo.strStationFullName = strStationFullName;
        AhaStationInfo.strPartnerURL = strPartnerURL;

        AhaStationInfo.strLikeSelectedURL = strLikeSelectedURL;
        AhaStationInfo.strDislikeSelectedURL = strDislikeSelectedURL;
        AhaStationInfo.strLikeUnselectedURL = strLikeUnselectedURL;
        AhaStationInfo.strDislikeUnselectedURL = strDislikeUnselectedURL;

#ifdef LATE_ICON
        DEBUG_DECODER("\t nLikeAllowed: %d\n", nLikeAllowed);
        DEBUG_DECODER("\t nReadOnly: %d\n", nReadOnly);
        DEBUG_DECODER("\t nIsTwitter: %d\n", nIsTwitter);
        DEBUG_DECODER("\t nIsLBS: %d\n", nIsLBS);
        DEBUG_DECODER("\t nCanTimeShift: %d\n", nCanTimeShift);
        DEBUG_DECODER("\t nIsDynamic: %d\n", nIsDynamic);
        DEBUG_DECODER("\t nIsFacebook: %d\n", nIsFacebook);
        DEBUG_DECODER("\t nCanPostText: %d\n", nCanPostText);
        DEBUG_DECODER("\t nRecordTime: %llu\n", nRecordTime);
        DEBUG_DECODER("\t strStationIconURL: %s\n", strStationIconURL.toUtf8().data());
        DEBUG_DECODER("\t strStationFullName: %s\n", strStationFullName.toUtf8().data());
        DEBUG_DECODER("\t strPartnerURL: %s\n", strPartnerURL.toUtf8().data());
#endif
        break;
    case AHA_ERR_INVALID_STATION_ID:
        DEBUG_DECODER("decodeQueryStationParameters returns error: AHA_ERR_INVALID_STATION_ID\n");
        break;
    case AHA_ERR_INVALID_PARAMETER:
        DEBUG_DECODER("decodeQueryStationParameters returns error: AHA_ERR_INVALID_PARAMETER\n");
        break;
    default:
        break;
    }

    return nErrorCode;
}

uint16_t decodeQueryMultipleStationParameters(QByteArray &byteArray,
                                              QVector <AhaStationBriefInfo_T> &qVectListOfStationsInfo)
{
    uint8_t nIsLBS = 0;
    uint16_t nErrorCode;
    uint16_t nLength, nextByteIndex, i, j;
    uint8_t nNumOfStation, nNumOfParam;
    uint64_t nStationID = 0;
    uint16_t nStationParam;
    uint16_t tempStringLength;

    uint8_t nParamDataType;

    //ok, here is the 11 parameters
    QString strStationIconURL;
    QString strStationFullName;

    AhaStationBriefInfo_T AhaStationBriefInfo;

    DEBUG_DECODER("\n decodeQueryStationParameters \n");

    //qVectListOfStationsInfo.clear();

    nErrorCode = checkErrorCode(byteArray);
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        //let us decode.
        DEBUG_DECODER("\nNo error\n");

//        qVectListOfStationsInfo.clear();

        nLength = byteToUint16_t(byteArray, 2);

        nextByteIndex = 10;


        // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com
        // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com
        // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com
        nNumOfStation = byteArray[nextByteIndex];
        // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com
        // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com
        // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com


        nextByteIndex = nextByteIndex + 1;

        for(i=0;i<nNumOfStation;i++)
        {
            nStationID = byteToUint64_t(byteArray, nextByteIndex);
            nextByteIndex = nextByteIndex + 8;


            // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com
            // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com
            // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com
            nNumOfParam = byteArray[nextByteIndex];
            // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com
            // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com
            // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com


            nextByteIndex = nextByteIndex + 1;

            DEBUG_DECODER(" nStationID %llu\n",nStationID);
            DEBUG_DECODER(" nNumOfParam %d\n",nNumOfParam);

            for(j=0;j<nNumOfParam;j++)
            {
                nStationParam = byteToUint16_t(byteArray, nextByteIndex);
                nextByteIndex = nextByteIndex + 2;

                DEBUG_DECODER(" nStationParam 0x%04x\n",nStationParam);

                switch(nStationParam)
                {
                case AHA_STT_PAR_ST_ICON_URL:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strStationIconURL = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_STT_PAR_FULL_NAME:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strStationFullName = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_STT_PAR_LBS:


                    // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com
                    // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com
                    // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com
                    nIsLBS = byteArray[nextByteIndex];
                    // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com
                    // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com
                    // How to validate byteArray variable??????!!!!!!!!!!!!!!!!!!!!!!! 25.Feb.2014 heemin.kang@lge.com


                    nextByteIndex = nextByteIndex + 1;
                    break;
                default:
                    //Even if we do not care about this parameter,
                    //we still have to advance the nextbyteindex!!!
                    //extract four significant bits from nStationParam to determine the type.
                    //0: boolean 1: UINT 2: INT 3:REAL(DOUBLE) 4:STRING
                    nParamDataType = (nStationParam >> 12);
                    switch(nParamDataType)
                    {
                    case AHA_BOOLEAN_PARAM:
                        nextByteIndex = nextByteIndex + 1;
                        DEBUG_DECODER("ignore 1byte in station param\n");
                        break;
                    case AHA_UINT64_PARAM:
                    case AHA_REAL_PARAM:
                    case AHA_INT64_PARAM:
                        DEBUG_DECODER("ignore 4byte in station param\n");
                        nextByteIndex = nextByteIndex + 8;
                        break;
                    case AHA_STRING_PARAM:
                        tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                        nextByteIndex = nextByteIndex + 2;
                        nextByteIndex = nextByteIndex + tempStringLength;
                        DEBUG_DECODER("ignore %dbyte in station param\n", tempStringLength + 2);
                        break;
                    default:
                        //this will not happen!
                        break;
                    }
                    break;
                }
            }//end of number of param loop
            //let us assign and print the values.
            AhaStationBriefInfo.nStationID = nStationID;
            AhaStationBriefInfo.strStationIconURL.clear();
            AhaStationBriefInfo.strStationIconURL = strStationIconURL ;
            AhaStationBriefInfo.strStationFullName.clear();
            AhaStationBriefInfo.strStationFullName = strStationFullName;
            AhaStationBriefInfo.nIsLBS = nIsLBS;

            qVectListOfStationsInfo.append(AhaStationBriefInfo);

            DEBUG_DECODER("\t nStationID: %llu\n", nStationID);
            DEBUG_DECODER("\t strStationIconURL: %s\n", strStationIconURL.toUtf8().data());
            DEBUG_DECODER("\t strStationFullName: %s\n", strStationFullName.toUtf8().data());
            DEBUG_DECODER("\t nIsLBS: %d\n", nIsLBS);
        }//end of number of station loop


        break;
    case AHA_ERR_INVALID_STATION_ID:
        DEBUG_DECODER("decodeQueryStationParameters returns error: AHA_ERR_INVALID_STATION_ID\n");
        break;
    case AHA_ERR_INVALID_PARAMETER:
        DEBUG_DECODER("decodeQueryStationParameters returns error: AHA_ERR_INVALID_PARAMETER\n");
        break;
    default:
        break;
    }
    return nErrorCode;
}

//0x08 //only valid for decoding result of querying single content
uint16_t decodeQuerySingleContentParameters(QByteArray &byteArray, AhaContentInfo_T &AhaContentInfo)
{
    uint16_t nErrorCode;
    uint16_t nLength, nextByteIndex, i, j;
    uint8_t nNumOfContent, nNumOfParam;

    uint16_t nContentParam;
    uint16_t tempStringLength;

    uint8_t nParamDataType;

    //we come here to get these values!
    uint64_t nContentID = 0;
    uint8_t nLiked = 0;
    uint8_t nCanPause = 0;
    uint8_t nCanNext = 0;
    uint8_t nCanBack = 0;
    uint8_t nCanChange = 0;
    uint8_t nCanTimeShift = 0;
    uint8_t nCanLike = 0;
    uint8_t nCanDislike = 0;
    uint8_t nCanUnvote = 0;
    uint8_t nDisliked = 0;

    double dbLon = 0;
    double dbLat = 0;
    double dbRating = 0;

    QString strTitle;
    QString strImageURL;
    QString strZip;
    QString strAddress1;
    QString strState;
    QString strCity;
    QString strPhone;
    QString strFullAddress;
    QString strAddress2;
    QString strCountryCode;
    QString strLangCode;
    QString strRatingImageURL;

    nErrorCode = checkErrorCode(byteArray);

    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        DEBUG_DECODER("\nNo error\n");

        //AhaContentInfo.clear();

        nLength = byteToUint16_t(byteArray, 2);

        nextByteIndex = 10;

        nNumOfContent = byteArray[nextByteIndex];
        nextByteIndex = nextByteIndex + 1;

        for(i=0;i<nNumOfContent;i++)
        {
            nContentID = byteToUint64_t(byteArray, nextByteIndex);
            nextByteIndex = nextByteIndex + 8;

            nNumOfParam = byteArray[nextByteIndex];
            nextByteIndex = nextByteIndex + 1;

            DEBUG_DECODER(" nContentID %llu\n",nContentID);
            DEBUG_DECODER(" nNumOfParam %d\n",nNumOfParam);

            for(j=0;j<nNumOfParam;j++)
            {
                nContentParam = byteToUint16_t(byteArray, nextByteIndex);
                nextByteIndex = nextByteIndex + 2;
                DEBUG_DECODER(" nContentParam 0x%04x\n",nContentParam);

                switch(nContentParam)
                {
                case AHA_CONT_PAR_LIKED:
                    nLiked = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_CONT_PAR_CAN_PAUSE:
                    nCanPause = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_CONT_PAR_CAN_NEXT:
                    nCanNext = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_CONT_PAR_CAN_BACK:
                    nCanBack = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_CONT_PAR_CAN_CHANGE:
                    nCanChange = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_CONT_PAR_CAN_TIMESHIFT:
                    nCanTimeShift = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_CONT_PAR_CAN_LIKE:
                    nCanLike = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_CONT_PAR_CAN_DISLIKE:
                    nCanDislike = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_CONT_PAR_CAN_UNVOTE:
                    nCanUnvote = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_CONT_PAR_DISLIKED:
                    nDisliked = byteArray[nextByteIndex];
                    nextByteIndex = nextByteIndex + 1;
                    break;
                case AHA_CONT_PAR_LON:
                    dbLon = byteToDouble(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 8;
                    break;
                case AHA_CONT_PAR_LAT:
                    dbLat = byteToDouble(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 8;
                    break;
                case AHA_CONT_PAR_RATING:
                    dbRating = byteToDouble(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 8;
                    break;
                case AHA_CONT_PAR_TITLE:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strTitle = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_CONT_PAR_IMAGE_URL:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strImageURL = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_CONT_PAR_ZIP:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strZip = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_CONT_PAR_ADDRESS1:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strAddress1 = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_CONT_PAR_STATE:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strState = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_CONT_PAR_CITY:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strCity = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_CONT_PAR_PHONE:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strPhone = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_CONT_PAR_FULL_ADDRESS:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strFullAddress = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_CONT_PAR_ADDRESS2:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strAddress2 = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_CONT_PAR_COUNTRY_CODE:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strCountryCode = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_CONT_PAR_LANG_CODE:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strLangCode = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                case AHA_CONT_PAR_RATING_IMG_URL:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strRatingImageURL = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;
                default:
                    //Even if we do not care about this parameter,
                    //we still have to advance the nextbyteindex!!!
                    //extract four significant bits from nStationParam to determine the type.
                    //0: boolean 1: UINT 2: INT 3:REAL(DOUBLE) 4:STRING
                    nParamDataType = (nContentParam >> 12);
                    switch(nParamDataType)
                    {
                    case AHA_BOOLEAN_PARAM:
                        nextByteIndex = nextByteIndex + 1;
                        DEBUG_DECODER("ignore 1 byte in station param\n");
                        break;
                    case AHA_UINT64_PARAM:
                    case AHA_INT64_PARAM:
                    case AHA_REAL_PARAM:
                        DEBUG_DECODER("ignore 4 byte in station param\n");
                        nextByteIndex = nextByteIndex + 8;
                        break;
                    case AHA_STRING_PARAM:
                        tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                        nextByteIndex = nextByteIndex + 2;
                        nextByteIndex = nextByteIndex + tempStringLength;
                        DEBUG_DECODER("ignore %d byte in station param\n", tempStringLength + 2);
                        break;
                    default:
                        //this will never happen! unless in the future protocol ? OMG.
                        break;
                    }
                    break;
                }
            }
        }

        AhaContentInfo.nContentID = nContentID;

        AhaContentInfo.nLiked	= nLiked;
        AhaContentInfo.nCanPause = nCanPause;
        AhaContentInfo.nCanNext = nCanNext;
        AhaContentInfo.nCanBack = nCanBack;
        AhaContentInfo.nCanChange	= nCanChange;
        AhaContentInfo.nCanTimeShift = nCanTimeShift;
        AhaContentInfo.nCanLike = nCanLike;
        AhaContentInfo.nCanDislike = nCanDislike;
        AhaContentInfo.nCanUnvote = nCanUnvote;
        AhaContentInfo.nDisliked = nDisliked;
        AhaContentInfo.dbLon = dbLon;
        AhaContentInfo.dbLat = dbLat;
        AhaContentInfo.dbRating = dbRating;
        AhaContentInfo.strTitle = strTitle;
        AhaContentInfo.strImageURL = strImageURL;
        AhaContentInfo.strZip = strZip;
        AhaContentInfo.strAddress1 = strAddress1;
        AhaContentInfo.strState = strState;
        AhaContentInfo.strCity = strCity;
        AhaContentInfo.strPhone = strPhone;
        AhaContentInfo.strFullAddress = strFullAddress;
        AhaContentInfo.strAddress2 = strAddress2;
        AhaContentInfo.strCountryCode = strCountryCode;
        AhaContentInfo.strLangCode = strLangCode;
        AhaContentInfo.strRatingImageURL = strRatingImageURL;

        DEBUG_DECODER("\t	nContentID : %llu\n",	nContentID	);
        DEBUG_DECODER("\t	nLiked : %d\n",	 nLiked	);
        DEBUG_DECODER("\t	nCanPause : %d\n",	 nCanPause	);
        DEBUG_DECODER("\t	nCanNext :	%d\n",	 nCanNext	);
        DEBUG_DECODER("\t	nCanBack :	%d\n",	 nCanBack	);
        DEBUG_DECODER("\t	nCanChange : %d\n",	 nCanChange	);
        DEBUG_DECODER("\t	nCanTimeShift : %d\n",	 nCanTimeShift	);
        DEBUG_DECODER("\t	nCanLike : %d\n",	 nCanLike	);
        DEBUG_DECODER("\t	nCanDislike : %d\n",	 nCanDislike	);
        DEBUG_DECODER("\t	nCanUnvote : %d\n",	 nCanUnvote	);
        DEBUG_DECODER("\t	nDisliked : %d\n",	nDisliked	);

        DEBUG_DECODER("\t	dbLon : %f\n",	dbLon	);
        DEBUG_DECODER("\t	dbLat : %f\n",	dbLat	);
        DEBUG_DECODER("\t	dbRating : %f\n",	dbRating	);

        DEBUG_DECODER("\t	strTitle : %s\n",	strTitle.toUtf8().data()	);
        DEBUG_DECODER("\t	strImageURL : %s\n",	strImageURL.toUtf8().data()	);
        DEBUG_DECODER("\t	strZip : %s\n",	strZip.toUtf8().data()	);
        DEBUG_DECODER("\t	strAddress1 : %s\n",	strAddress1.toUtf8().data()	);
        DEBUG_DECODER("\t	strState :	%s\n",	strState.toUtf8().data()	);
        DEBUG_DECODER("\t	strCity : %s\n",	strCity.toUtf8().data()	);
        DEBUG_DECODER("\t	strPhone :	%s\n",	strPhone.toUtf8().data()	);
        DEBUG_DECODER("\t	strFullAddress	:		%s\n",	strFullAddress.toUtf8().data()	);
        DEBUG_DECODER("\t	strAddress2 : %s\n",	strAddress2.toUtf8().data()	);
        DEBUG_DECODER("\t	strCountryCode : %s\n",	strCountryCode.toUtf8().data()	);
        DEBUG_DECODER("\t	strLangCode : %s\n",	strLangCode.toUtf8().data()	);
        DEBUG_DECODER("\t	strRatingImageURL : %s\n",	strRatingImageURL.toUtf8().data()	);

        break;
    case AHA_ERR_INVALID_CONTENT_ID:
        DEBUG_DECODER("\nAHA_ERR_INVALID_CONTENT_ID\n");
        break;
    case AHA_ERR_INVALID_PARAMETER:
        DEBUG_DECODER("\nAHA_ERR_INVALID_PARAMETER\n");
        break;
    default:
        break;
    }
    return nErrorCode;
}

uint16_t decodeQueryMultipleContentParameters(QByteArray &byteArray,
                                              QVector<AhaContentBriefInfo_T> &qVectListOfContentsInfo)
{
    AhaContentBriefInfo_T AhaContentBriefInfo;
    uint16_t nErrorCode;
    uint16_t nLength, nextByteIndex, i, j;
    uint8_t nNumOfContent, nNumOfParam;

    uint16_t nContentParam;
    uint16_t tempStringLength;

    uint8_t nParamDataType;

    //we come here to get these values!
    uint64_t nContentID = 0;
    QString strTitle;


    //qVectListOfContentsInfo.clear();
    nErrorCode = checkErrorCode(byteArray);
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        DEBUG_DECODER("\nNo error\n");

//        qVectListOfContentsInfo.clear();

        nLength = byteToUint16_t(byteArray, 2);

        nextByteIndex = 10;

        nNumOfContent = byteArray[nextByteIndex];
        nextByteIndex = nextByteIndex + 1;

        for(i=0;i<nNumOfContent;i++)
        {
            nContentID = byteToUint64_t(byteArray, nextByteIndex);
            nextByteIndex = nextByteIndex + 8;

            nNumOfParam = byteArray[nextByteIndex];
            nextByteIndex = nextByteIndex + 1;

            DEBUG_DECODER(" nContentID %llu\n",nContentID);
            DEBUG_DECODER(" nNumOfParam %d\n",nNumOfParam);

            for(j=0;j<nNumOfParam;j++)
            {
                nContentParam = byteToUint16_t(byteArray, nextByteIndex);
                nextByteIndex = nextByteIndex + 2;
                DEBUG_DECODER(" nContentParam 0x%04x\n",nContentParam);

                switch(nContentParam)
                {
                case AHA_CONT_PAR_TITLE:
                    tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                    nextByteIndex = nextByteIndex + 2;
                    strTitle = byteToQString(byteArray, nextByteIndex, tempStringLength);
                    nextByteIndex = nextByteIndex + tempStringLength;
                    break;

                default:
                    //Even if we do not care about this parameter,
                    //we still have to advance the nextbyteindex!!!
                    //extract four significant bits from nStationParam to determine the type.
                    //0: boolean 1: UINT 2: INT 3:REAL(DOUBLE) 4:STRING
                    nParamDataType = (nContentParam >> 12);
                    switch(nParamDataType)
                    {
                    case AHA_BOOLEAN_PARAM:
                        nextByteIndex = nextByteIndex + 1;
                        DEBUG_DECODER("ignore 1 byte in station param\n");
                        break;
                    case AHA_UINT64_PARAM:
                    case AHA_INT64_PARAM:
                    case AHA_REAL_PARAM:
                        DEBUG_DECODER("ignore 4 byte in station param\n");
                        nextByteIndex = nextByteIndex + 8;
                        break;
                    case AHA_STRING_PARAM:
                        tempStringLength = byteToUint16_t(byteArray, nextByteIndex);
                        nextByteIndex = nextByteIndex + 2;
                        nextByteIndex = nextByteIndex + tempStringLength;
                        DEBUG_DECODER("ignore %d byte in station param\n", tempStringLength + 2);
                        break;
                    default:
                        //this will never happen! unless in the future protocol ? OMG.
                        break;
                    }
                    break;
                }
            }//number of param loop

            AhaContentBriefInfo.nContentID = nContentID;
            AhaContentBriefInfo.strTitle.clear();
            AhaContentBriefInfo.strTitle = strTitle;
            qVectListOfContentsInfo.append(AhaContentBriefInfo);

            DEBUG_DECODER("\t	nContentID : %llu\n",	nContentID);
            DEBUG_DECODER("\t	strTitle : %s\n",	strTitle.toUtf8().data());

        }//number of content loop

        break;
    case AHA_ERR_INVALID_CONTENT_ID:
        DEBUG_DECODER("\nAHA_ERR_INVALID_CONTENT_ID\n");
        break;
    case AHA_ERR_INVALID_PARAMETER:
        DEBUG_DECODER("\nAHA_ERR_INVALID_PARAMETER\n");
        break;
    default:
        break;
    }
    return nErrorCode;
}


//0x09
uint16_t decodeQueryBinaryData(QByteArray &byteArray, AhaImageInfo_T &AhaImageInfo)
{
    uint16_t nErrorCode;
    uint16_t nLength;
    uint16_t nextByteIndex;
    uint32_t nContentTotalSize, nOffsetRetInResp, nDataReturnedAmount;
    QByteArray binaryData;

    DEBUG_DECODER("\n decodeQueryBinaryData \n");
    nErrorCode = checkErrorCode(byteArray);

    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        nLength = byteToUint16_t(byteArray, 2);
        nextByteIndex = 10;

        nContentTotalSize = byteToUint32_t(byteArray, nextByteIndex);
        nextByteIndex = nextByteIndex + 4;

        nOffsetRetInResp = byteToUint32_t(byteArray, nextByteIndex);
        nextByteIndex = nextByteIndex + 4;

        nDataReturnedAmount = byteToUint16_t(byteArray, nextByteIndex);
        nextByteIndex = nextByteIndex + 2;

        //let us collect the data now.
        binaryData = byteArray.mid(nextByteIndex, nDataReturnedAmount);

        //check the offset. If zero, then it is the first package of data.
        //clear the binary data.
        if( 0 == nOffsetRetInResp )
        {
            DEBUG_DECODER("Start of binary image\n");
            AhaImageInfo.imageDataByte.clear();
            AhaImageInfo.nFragmentCount = 0;
            AhaImageInfo.imageDataByte.resize(nContentTotalSize);
        }
        AhaImageInfo.imageDataByte.replace(nOffsetRetInResp, nDataReturnedAmount, binaryData);
        AhaImageInfo.nOffset = nOffsetRetInResp + nDataReturnedAmount;
        AhaImageInfo.nFragmentCount = AhaImageInfo.nFragmentCount + 1;

        if(nContentTotalSize <= nOffsetRetInResp + nDataReturnedAmount)
        {
            DEBUG_DECODER("We have received all the binary data\n");
            AhaImageInfo.nIsDownloadComplete = 1;
        }
        DEBUG_DECODER("nContentTotalSize: %d\n",nContentTotalSize);
        DEBUG_DECODER("nOffsetRetInResp: %d\n",nOffsetRetInResp);
        DEBUG_DECODER("nDataReturnedAmount: %d\n",nDataReturnedAmount);
        DEBUG_DECODER("AhaImageInfo.nOffset: %d\n", AhaImageInfo.nOffset);
        break;
    case AHA_ERR_UNAVAILABLE_RESOURCE:
        //we have to make sure that the same request is not repeated by the continueDownload data.
        AhaImageInfo.nIsDownloadComplete = -1;
        DEBUG_DECODER("decodeQueryBinaryData returns error: AHA_ERR_UNAVAILABLE_RESOURCE\n");
        break;
    case AHA_ERR_INVALID_RESOURCE:
        //we have to make sure that the same request is not repeated by the continueDownload data.
        AhaImageInfo.nIsDownloadComplete = -1;
        DEBUG_DECODER("decodeQueryBinaryData returns error: AHA_ERR_INVALID_RESOURCE\n");
        break;
    default:
        break;
    }
    return nErrorCode;
}

//0x0B
uint16_t decodeQueryCurrentStation(QByteArray &byteArray, AhaClientInfo_T &AhaClientInfo, bool ignoreStationID)
{
    uint16_t nErrorCode;
    uint16_t nLength;
    uint64_t nStationID, nContentID;
    uint8_t nPlayBackState;
    uint32_t nElapsedSecs;
    uint16_t nextByteIndex;
    DEBUG_DECODER("\n decodeQueryCurrentStation \n");
    nErrorCode = checkErrorCode(byteArray);

    if (nErrorCode != AHA_ERR_NO_ERROR)
    {
        DEBUG_DECODER("decodeQueryCurrentStation returns error\n");
        return 0;
    }

    nLength = byteToUint16_t(byteArray, 2);

    //the new protocol says we do not have to worry about the length
    //so as long it is not shorter than what we wish to access, we will OK it.
    if(nLength < 31)
    {
        DEBUG_DECODER("nLength Error: breaking out\n");
        return 0;
    }
    //let us decode the message now
    nextByteIndex = 10;

    nStationID = byteToUint64_t(byteArray, nextByteIndex);
    nextByteIndex = nextByteIndex + 8;

    nContentID = byteToUint64_t(byteArray, nextByteIndex);
    nextByteIndex = nextByteIndex + 8;

    nPlayBackState = byteArray[nextByteIndex];
    nextByteIndex = nextByteIndex + 1;

    nElapsedSecs = byteToUint32_t(byteArray, nextByteIndex);

    DEBUG_DECODER("\t nStationID: %llu\n", nStationID );
    DEBUG_DECODER("\t nContentID: %llu\n", nContentID );
    DEBUG_DECODER("\t playBackState: %d\n", nPlayBackState );
    DEBUG_DECODER("\t nNumberOfSecs: %d\n", nElapsedSecs );

    if(!ignoreStationID)
    {
        AhaClientInfo.nActiveStationID = nStationID;
    }
    AhaClientInfo.nActiveContentID = nContentID;
    AhaClientInfo.nPlaybackState = nPlayBackState;
    AhaClientInfo.nElapsedSecs = nElapsedSecs;

    return nErrorCode;
}

//0x0C
uint16_t decodeSelectStationForPlayback(QByteArray &byteArray)
{
    uint16_t nErrorCode;
    DEBUG_DECODER("\n decodeSelectStationForPlayback \n");
    nErrorCode = checkErrorCode(byteArray);
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        //no error
        DEBUG_DECODER("NO ERROR\n");
        break;
    case AHA_ERR_INVALID_STATION_ID:
        DEBUG_DECODER("decodeSelectStationForPlayback returns AHA_ERR_INVALID_CONTENT_ID\n");
        break;
    default:
        break;
    }
    return nErrorCode;
}


//0x0D
uint16_t decodeSelectContentForPlayback(QByteArray &byteArray)
{
    uint16_t nErrorCode;
    DEBUG_DECODER("\n decodeSelectContentForPlayback \n");
    nErrorCode = checkErrorCode(byteArray);
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        //no error
        DEBUG_DECODER("NO ERROR\n");
        break;
    case AHA_ERR_INVALID_CONTENT_ID:
        DEBUG_DECODER("decodeSelectContentForPlayback returns AHA_ERR_INVALID_CONTENT_ID\n");
        break;
    default:
        break;
    }

    return nErrorCode;
}

//0x0E
uint16_t decodeChangeClientPlayback(QByteArray &byteArray, AhaClientInfo_T &AhaClientInfo)
{
    uint16_t nErrorCode;
    uint16_t nLength;
    uint64_t nContentID;
    uint8_t nPlayBackState;
    uint16_t nextByteIndex;
    DEBUG_DECODER("\n decodeChangeClientPlayback \n");
    nErrorCode = checkErrorCode(byteArray);

    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        nLength = byteToUint16_t(byteArray, 2);

        nextByteIndex = 10;

        nContentID = byteToUint64_t(byteArray, nextByteIndex);
        nextByteIndex = nextByteIndex + 8;

        nPlayBackState = byteArray[nextByteIndex];
        nextByteIndex = nextByteIndex + 1;

        AhaClientInfo.nActiveContentID = nContentID;
        AhaClientInfo.nPlaybackState = nPlayBackState;
        //TAN_PRINTF("decodeChangeClientPlayback   nPlaybackState = %d \n", nPlayBackState);

        DEBUG_DECODER("nContentID: %llu\n", nContentID);
        DEBUG_DECODER("nPlaybackState: %d\n", nPlayBackState);

        break;
    case AHA_ERR_INVALID_STATION_ID:
        DEBUG_DECODER("decodeChangeClientPlayback returns AHA_ERR_INVALID_STATION_ID\n");
        break;
    default:
        break;
    }

    return nErrorCode;
}

//0x0F
uint16_t decodeTimeShiftContent(QByteArray &byteArray)
{
    uint16_t nErrorCode;
    DEBUG_DECODER("\n decodeTimeShiftContent \n");
    nErrorCode = checkErrorCode(byteArray);
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        //no error
        DEBUG_DECODER("NO ERROR\n");
        break;
    case AHA_ERR_CANNOT_TIMESHIFT:
        DEBUG_DECODER("decodeTimeShiftContent returns AHA_ERR_CANNOT_TIMESHIFT \n");
        break;
    default:
        break;
    }

    return nErrorCode;
}

//0x11
uint16_t decodePerformAction(QByteArray &byteArray)
{
    uint16_t nErrorCode;
    DEBUG_DECODER("\n decodePerformAction \n");
    nErrorCode = checkErrorCode(byteArray);
    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:
        //no error
        DEBUG_DECODER("NO ERROR\n");
        break;
    case AHA_ERR_INVALID_CONTENT_ID:
        DEBUG_DECODER("decodePerformAction returns AHA_ERR_INVALID_CONTENT_ID \n");
        break;
    case AHA_ERR_INVALID_ACTION:
        DEBUG_DECODER("decodePerformAction returns AHA_ERR_INVALID_ACTION \n");
        break;
    case AHA_ERR_NEED_MYFEEDS_STATION:
        DEBUG_DECODER("decodePerformAction returns AHA_ERR_NEED_MYFEEDS_STATION \n");
        break;
    default:
        break;
    }

    return nErrorCode;
}


//0x12
uint16_t decodeRequestCurrentContentInfo(QByteArray &byteArray, AhaContentInfo_T &AhaContentInfo)
{
    uint16_t nErrorCode;
    uint16_t nLength, i;

    uint64_t nContentID;
    uint8_t nColor;
    uint8_t nTotalStrings;
    uint8_t nNumOfStringsRet;

    uint16_t nextByteIndex;

    QString strContentInfo;
    uint16_t nTempLength;

    DEBUG_DECODER("\n decodeRequestCurrentContentInfo \n");
    nErrorCode = checkErrorCode(byteArray);

    switch(nErrorCode)
    {
    case AHA_ERR_NO_ERROR:

        //AhaContentInfo.clear();

        nLength = byteToUint16_t(byteArray, 2);

        nextByteIndex = 10;

        nContentID = byteToUint64_t(byteArray, nextByteIndex);
        nextByteIndex = nextByteIndex + 8;

        nColor = byteArray[nextByteIndex];
        nextByteIndex = nextByteIndex + 1;

        nTotalStrings = byteArray[nextByteIndex];
        nextByteIndex = nextByteIndex + 1;

        nNumOfStringsRet = byteArray[nextByteIndex];
        nextByteIndex = nextByteIndex + 1;

        DEBUG_DECODER("nLength: %d\n", nLength);
        DEBUG_DECODER("nContentID: %llu\n", nContentID);
        DEBUG_DECODER("nColor: %d\n", nColor);
        DEBUG_DECODER("nTotalStrings: %d\n", nTotalStrings);
        DEBUG_DECODER("nNumOfStringsRet: %d\n", nNumOfStringsRet);

        AhaContentInfo.strQVectContentInfoString.clear();
        for(i=0;i<nNumOfStringsRet;i++)
        {
            nTempLength = byteToUint16_t(byteArray, nextByteIndex);
            nextByteIndex = nextByteIndex + 2;
            strContentInfo.clear();
            strContentInfo = byteToQString(byteArray,nextByteIndex,nTempLength);
            nextByteIndex = nextByteIndex + nTempLength;
            AhaContentInfo.strQVectContentInfoString.append(strContentInfo);
            DEBUG_DECODER("strContentInfo %s", strContentInfo.toUtf8().data());
        }

        break;
    case AHA_ERR_INVALID_CONTENT_ID:
        DEBUG_DECODER("decodeRequestCurrentContentInfo returns AHA_ERR_INVALID_CONTENT_ID\n");
        break;
    default:
        break;
    }
    return nErrorCode;
}

//0x0101
uint16_t decodeNotifyPlaybackOfNewStation(QByteArray &byteArray, AhaClientInfo_T &AhaClientInfo)
{
    uint16_t nLength;
    uint64_t nStationID;
    uint16_t nextByteIndex;

    DEBUG_DECODER("\n decodeNotifyPlaybackOfNewStation \n");

    nLength = byteToUint16_t(byteArray, 2);

    nextByteIndex = 10;

    nStationID = byteToUint64_t(byteArray, nextByteIndex);
    nextByteIndex = nextByteIndex + 8;

    DEBUG_DECODER("nStationID: %llu\n", nStationID);
    AhaClientInfo.nActiveStationID = nStationID;
    return AHA_ERR_NO_ERROR;
}

//0x0102
uint16_t decodeNotifyNewContentPlaybackState(QByteArray &byteArray, AhaClientInfo_T &AhaClientInfo)
{
    uint16_t nLength;
    uint64_t nContentID;
    uint8_t nPlayBackState;
    uint16_t nextByteIndex;

    DEBUG_DECODER("\n decodeNotifyNewContentPlaybackState \n");

    nLength = byteToUint16_t(byteArray, 2);

    nextByteIndex = 10;

    nContentID = byteToUint64_t(byteArray, nextByteIndex);
    nextByteIndex = nextByteIndex + 8;

    nPlayBackState = byteArray[nextByteIndex];
    nextByteIndex = nextByteIndex + 1;

    AhaClientInfo.nActiveContentID = nContentID;
    AhaClientInfo.nPlaybackState = nPlayBackState;
//    TAN_PRINTF("decodeNotifyNewContentPlaybackState   nPlaybackState = %d \n", nPlayBackState);

    DEBUG_DECODER("nContentID: %llu\n", nContentID);
    DEBUG_DECODER("nPlaybackState: %d\n", nPlayBackState);

    return AHA_ERR_NO_ERROR;
}

//0x0103
uint16_t decodeNotifyNewContentAvailable(QByteArray &byteArray, uint64_t &nStationID)
{
    uint16_t nLength;
    uint8_t nIsNewerContentAvailable, nIsOlderContentAvailable;
    uint16_t nextByteIndex;

    DEBUG_DECODER("\n decodeNotifyNewContentAvailable \n");

    nLength = byteToUint16_t(byteArray, 2);

    nextByteIndex = 10;

    nStationID = byteToUint64_t(byteArray, nextByteIndex);
    nextByteIndex = nextByteIndex + 8;

    nIsNewerContentAvailable = byteArray[nextByteIndex];
    nextByteIndex = nextByteIndex + 1;

    nIsOlderContentAvailable = byteArray[nextByteIndex];
    nextByteIndex = nextByteIndex + 1;

    DEBUG_DECODER("nStationID: %llu\n", nStationID);
    DEBUG_DECODER("nIsNewerContentAvailable: %d\n", nIsNewerContentAvailable);
    DEBUG_DECODER("nIsOlderContentAvailable: %d\n", nIsOlderContentAvailable);

    return AHA_ERR_NO_ERROR;
}

//0x0104
uint16_t decodeNotifyContentDeleted(QByteArray &byteArray,
                                    QVector <AhaContentBriefInfo_T> &qVectListOfContentsInfo)
{
    uint16_t nLength, i, j;
    uint64_t nStationID;
    uint8_t nNumOfContRemoved;
    uint64_t *nContentIDArray;
    uint16_t nextByteIndex;

    DEBUG_DECODER("\n decodeNotifyContentDeleted \n");

    nLength = byteToUint16_t(byteArray, 2);

    nextByteIndex = 10;

    nStationID = byteToUint64_t(byteArray, nextByteIndex);
    nextByteIndex = nextByteIndex + 8;

    nNumOfContRemoved = byteArray[nextByteIndex];
    nextByteIndex = nextByteIndex + 1;

    DEBUG_DECODER("nStationID %llu\n", nStationID);
    DEBUG_DECODER("nNumOfContRemoved %d\n", nNumOfContRemoved);

    nContentIDArray = new uint64_t[nNumOfContRemoved];
    for (i=0; i < nNumOfContRemoved; i++)
    {
        nContentIDArray[i] = byteToUint64_t(byteArray, nextByteIndex);
        nextByteIndex = nextByteIndex + 8;
        for(j = 0; j< qVectListOfContentsInfo.count(); j++)
        {
            if(qVectListOfContentsInfo[j].nContentID == nContentIDArray[i])
            {
                DEBUG_STATEHANDLER("Removing: %s\n", qVectListOfContentsInfo[j].strTitle.toUtf8().data());
                qVectListOfContentsInfo.remove(j);
                break;
            }
        }
        DEBUG_DECODER("nContentIDArray[%d] %llu\n", i,nContentIDArray[i]);
    }

    delete [] nContentIDArray;
    return AHA_ERR_NO_ERROR;
}

//0x0105
uint16_t decodeNotifyClientStatusUpdate(QByteArray &byteArray, uint32_t &nConnStatusCode)
{
    uint16_t nLength;
    uint32_t nStatusCode;
    uint16_t nextByteIndex;

    DEBUG_DECODER("\n decodeNotifyClientStatusUpdate \n");
    nLength = byteToUint16_t(byteArray, 2);

    nextByteIndex = 10;

    nStatusCode  = byteToUint32_t(byteArray, nextByteIndex);
    nextByteIndex = nextByteIndex + 4;

    DEBUG_DECODER("nStatusCode %d\n", nStatusCode);
    nConnStatusCode = nStatusCode;
    return AHA_ERR_NO_ERROR;
}

//0x0108
uint16_t decodeNotifySubscribedStationListChange()
{
    DEBUG_DECODER("\n decodeNotifySubscribedStationListChange \n");
    DEBUG_DECODER("Subscribed list changed\n");
    return AHA_ERR_NO_ERROR;
}


bool verifyChecksum(QByteArray &byteArray)
{
    uint16_t calculatedChecksum, receivedChecksum;
    calculatedChecksum = calculateCheckSum(byteArray);
    receivedChecksum = byteToUint16_t(byteArray,8);
    DEBUG_DECODER("calculatedChecksum: %d, receivedChecksum: %d\n",
           calculatedChecksum, receivedChecksum);
    return (calculatedChecksum == receivedChecksum);
}

uint16_t checkErrorCode(QByteArray &byteArray)
{
    uint16_t nErrorCode;
    nErrorCode = byteToUint16_t(byteArray, 6);
    return nErrorCode;
}
