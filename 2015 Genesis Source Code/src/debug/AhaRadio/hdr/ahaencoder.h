#ifndef AHAENCODER_H
#define AHAENCODER_H

#include <QObject>
#include <QVector>
#include <QDataStream>
#include <stdint.h>
#include <stdio.h>
#include "ahaCommonUtility.h"
#include "ahapredefinedvalues.h"


QByteArray createStartPeerSessionRequest();                                 //0x01
QByteArray createQueryStationsRequest();                                    //0x04
QByteArray createQuerySingleStationParametersRequest(uint64_t nStationIDtoQuery);   //0x06
QByteArray createQueryMultipleStationParametersRequest(QVector <uint64_t> &nQVectStationIDtoQuery, int nStartPresetStationIndex); //0x06
QByteArray createQueryStationContentRequest(uint64_t nStationID); //0x07
QByteArray createQuerySingleContentParametersRequest(uint64_t nContentIDtoQuery); //0x08
QByteArray createQueryMultipleContentParametersRequest(QVector <uint64_t> &nQVectContentIDtoQuery, int nStartContentIndex); //0x08
QByteArray createQueryBinaryDataRequest(QString strURLString, uint32_t nOffset,
                                        uint16_t nWidth, uint16_t nHeight); //0x09
QByteArray createQueryCurrentStationRequest();                              //0x0B
QByteArray createSelectStationForPlaybackRequest(uint64_t nStationID);      //0x0C
QByteArray createSelectContentForPlaybackRequest(uint64_t nContentID);      //0x0D
QByteArray createChangeClientPlaybackRequest(uint8_t nPlaybackRequest);     //0x0E
QByteArray createTimeShiftContentRequest(uint16_t nSeconds);                //0x0F
QByteArray createPerformActionRequest(uint32_t nActionEnumValue,uint64_t nContentID); //0x11
QByteArray createRequestCurrentContentInfo();                               //0x12

#endif// AHAENCODER_H
