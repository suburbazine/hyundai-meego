#ifndef AHADECODER_H
#define AHADECODER_H

#include <stdio.h>
#include "ahaCommonUtility.h"
#include "ahapredefinedvalues.h"

//Head Unit initiated.
uint16_t decodeStartPeerSession(QByteArray &byteArray, AhaClientInfo_T &AhaClientInfo);                     //0x01

uint16_t decodeQueryStations(QByteArray &byteArray, AhaClientInfo_T &AhaClientInfo);                        //0x04

uint16_t decodeQuerySingleStationParameters(QByteArray &byteArray, AhaStationInfo_T &AhaStationInfo);       //0x06

uint16_t decodeQueryMultipleStationParameters(QByteArray &byteArray,
                                              QVector <AhaStationBriefInfo_T> &qVectListOfStationsInfo);    //0x06
uint16_t decodeQueryStationContent(QByteArray &byteArray, AhaClientInfo_T &AhaClientInfo);                  //0x07

uint16_t decodeQuerySingleContentParameters(QByteArray &byteArray, AhaContentInfo_T &AhaContentInfo);       //0x08

uint16_t decodeQueryMultipleContentParameters(QByteArray &byteArray,
                                              QVector <AhaContentBriefInfo_T> &qVectListOfContentsInfo);    //0x08
uint16_t decodeQueryBinaryData(QByteArray &byteArray, AhaImageInfo_T &AhaImageInfo);                        //0x09

uint16_t decodeQueryCurrentStation(QByteArray &byteArray, AhaClientInfo_T &AhaClientInfo, bool ignoreStationID);                  //0x0B

uint16_t decodeSelectStationForPlayback(QByteArray &byteArray);                                             //0x0C

uint16_t decodeSelectContentForPlayback(QByteArray &byteArray);                                             //0x0D

uint16_t decodeChangeClientPlayback(QByteArray &byteArray, AhaClientInfo_T &AhaClientInfo);                 //0x0E

uint16_t decodeTimeShiftContent(QByteArray &byteArray);                                                     //0x0F
uint16_t decodePerformAction(QByteArray &byteArray);                                                        //0x11
uint16_t decodeRequestCurrentContentInfo(QByteArray &byteArray, AhaContentInfo_T &AhaContentInfo);          //0x12

//client initiated
uint16_t decodeNotifyPlaybackOfNewStation(QByteArray &byteArray, AhaClientInfo_T &AhaClientInfo);           //0x0101
uint16_t decodeNotifyNewContentPlaybackState(QByteArray &byteArray, AhaClientInfo_T &AhaClientInfo);        //0x0102
uint16_t decodeNotifyNewContentAvailable(QByteArray &byteArray, uint64_t &nStationID);                                            //0x0103
uint16_t decodeNotifyContentDeleted(QByteArray &byteArray, QVector <AhaContentBriefInfo_T> &qVectListOfContentsInfo);                                                 //0x0104
uint16_t decodeNotifyClientStatusUpdate(QByteArray &byteArray, uint32_t &nConnStatusCode);                                             //0x0105
uint16_t decodeNotifySubscribedStationListChange();                                                         //0x0108

bool verifyChecksum(QByteArray &byteArray);

#endif // AHADECODER_H
