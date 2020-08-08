/*
 ******************************************************************
 *        COPYRIGHT © 2011, LG Electronics.
 *        All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :    Station Cache                              *
 * PROGRAMMER :    Radhakrushna Mohanty                                  *
 * DATE       :    16/07/2013                                     *
 * VERSION    :    0.1                                            *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *        This module kepps station informaion to be used pandora
 *        StateHandler ,
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS
 * 1) 16/07/2013    0.1 Radhakrushna Mohanty   header file created
 *
 ******************************************************************
 */
#ifndef DHAVN_PNDRCACHE_H
#define DHAVN_PNDRCACHE_H


#include <QObject>
#include <QList>
#include <QMap>

#include "DHAVN_Pndr_Defs.h"


class CPndrCache:public QObject
{
    Q_OBJECT

public:
    CPndrCache(QObject* parent = 0);
    ~CPndrCache();

    bool AllStationAvailable(ESortType sortType , QList<TSStationInfo>& stList);

    bool IsStationsAvailable(ESortType sortType , const qint8 startIndex ,
                             const qint8 endIndex , QList<TSStationInfo>& stList);

    void SaveStationToken(ESortType sortType , const QList<quint32>& tokenList);

    void InsertToken(ESortType sortType ,qint8 index, quint32 token);

    void RemoveToken(ESortType sortType , quint32 token);

    void SaveStationInfo(const QList<TSStationInfo>& stInfos);

    int StationCount(ESortType sortType , QList<quint32>& tokenList);

    void ClearInfo();

private:

    bool PopulateList(QList<quint32>& inTokenList , QList<TSStationInfo>& stList);


private:

    QList<quint32> m_AlphabetTokenList;
    QList<quint32> m_DateTokenList;

    QMap<quint32 , TSStationInfo> m_StList;

};

#endif // DHAVN_PNDRCACHE_H
