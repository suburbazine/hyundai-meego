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
 * 1) 16/07/2013    0.1 Radhakrushna Mohanty   source file created
 *
 ******************************************************************
 */

#include "DHAVN_PndrCache.h"
#include <QDebug>


CPndrCache::CPndrCache(QObject* parent):QObject(parent)
{

}

CPndrCache::~CPndrCache()
{
    m_AlphabetTokenList.clear();
    m_DateTokenList.clear();
    m_StList.clear();
}

bool CPndrCache::AllStationAvailable(ESortType sortType , QList<TSStationInfo>& stList)
{
    int start = 0 , end = 0;
    if (sortType == SORT_TYPE_ALPHA)
    {
        end = m_AlphabetTokenList.count() ;
    }
    else if(sortType == SORT_TYPE_DATE)
    {
        end = m_DateTokenList.count() ;
    }
    bool ret = false;
    if(start < end)
        ret = IsStationsAvailable(sortType , start , end - 1 , stList);

    return ret ;

}

bool CPndrCache::IsStationsAvailable(ESortType sortType , const qint8 startIndex ,
                         const qint8 endIndex , QList<TSStationInfo>& stList)
{
    qDebug() << "CPndrCache::IsStationsAvailable : "<< sortType ;
    bool retStatus = false ;
    QList<quint32> tokenList;

    if (sortType == SORT_TYPE_ALPHA)
    {
        qDebug() << "CPndrCache:: startIndex : "<< startIndex<<"endIndex :" << endIndex ;

        if( m_AlphabetTokenList.count() > startIndex && m_AlphabetTokenList.count() > endIndex )
        {
            for (int index = startIndex ; index <= endIndex; index++)
            {
                tokenList.append(m_AlphabetTokenList.at(index));
            }
            retStatus = PopulateList(tokenList , stList);
        }

    }
    else if(sortType == SORT_TYPE_DATE)
    {
        if( m_DateTokenList.count() > startIndex && m_DateTokenList.count() > endIndex )
        {
            for (int index = startIndex ; index <= endIndex; index++)
            {
                tokenList.append(m_DateTokenList.at(index));
            }
            retStatus = PopulateList(tokenList , stList);
        }

    }
    else
    {

    }

    qDebug()<<"IsStationsAvailable retStatus -> "<<retStatus;
    return retStatus;
}


bool CPndrCache::PopulateList(QList<quint32>& inTokenList , QList<TSStationInfo>& stList)
{

    qDebug() << "CPndrCache::PopulateList for : "<< inTokenList.count() ;
    bool retStatus = true;
    for( int c = 0 ; c < inTokenList.count() ; c++)
    {
        quint32 token = inTokenList.at(c);
        if(m_StList.contains(token))
        {
            stList.append(m_StList.value(token));
        }
        else
        {
            retStatus = false;
            stList.clear();
            break;
        }
    }
    return retStatus;
}

void CPndrCache::SaveStationToken(ESortType sortType , const QList<quint32>& tokenList)
{
    if(SORT_TYPE_ALPHA == sortType)
    {
        m_AlphabetTokenList.clear();
        m_AlphabetTokenList = tokenList;
    }
    else if(SORT_TYPE_DATE == sortType)
    {
        m_DateTokenList.clear();
        m_DateTokenList = tokenList;
    }
    else
    {

    }
}

void CPndrCache::InsertToken(ESortType sortType ,qint8 index, quint32 token)
{
    //qDebug() << "CPndrCache::InsertToken sort : " << sortType <<" , index : "<< index <<", token :"<< token;
    if(SORT_TYPE_ALPHA == sortType )
    {
        //{ added contains by wonseok.heo for ITS 243645
        if(m_AlphabetTokenList.contains(token)){
            return;
        }//} added contains by wonseok.heo for ITS 243645
        if(m_AlphabetTokenList.count() > index)
            m_AlphabetTokenList.insert(index , token);
        else
            m_AlphabetTokenList.append(token);
    }
    else if(SORT_TYPE_DATE == sortType)
    {
        //{ added contains by wonseok.heo for ITS 243645
        if(m_DateTokenList.contains(token)){
            return;
        }//} added contains by wonseok.heo for ITS 243645
        if(m_DateTokenList.count() > index)
            m_DateTokenList.insert(index , token);
        else
            m_DateTokenList.append(token);
    }
    else
    {

    }

    if(SORT_TYPE_ALPHA == sortType)
        m_DateTokenList.clear();
    else if(SORT_TYPE_DATE == sortType)
        m_AlphabetTokenList.clear();
    else{}
}

void CPndrCache::RemoveToken(ESortType sortType , quint32 token)
{
    if(SORT_TYPE_ALPHA == sortType && m_AlphabetTokenList.count() >0)
    {
       int i =0;
       while (i < m_AlphabetTokenList.count())
        {   if (token == m_AlphabetTokenList.at(i))
            {
                //{ modified by yongkyun.lee 2014-08-22 for :  ITS 246133
                m_AlphabetTokenList.removeAt(i);
                m_StList.remove(i);
                //} modified by yongkyun.lee 2014-08-22 
                break;
            }
            i++;
        }
    }
    else if(SORT_TYPE_DATE == sortType && m_DateTokenList.count() >0)
    {
        int i =0;
        while (i < m_DateTokenList.count())
         {   if (token == m_DateTokenList.at(i))
             {
                 //{ modified by yongkyun.lee 2014-08-22 for :  ITS 246133
                 m_DateTokenList.removeAt(i);
                 m_StList.remove(i);
                 //} modified by yongkyun.lee 2014-08-22 
                 break;
             }
             i++;
         }
    }

    if(SORT_TYPE_ALPHA == sortType)
        m_DateTokenList.clear();
    else
        m_AlphabetTokenList.clear();
}

void CPndrCache::SaveStationInfo(const QList<TSStationInfo>& stInfos)
{
    qint8 c = 0;
    while(c < stInfos.count())
    {
        quint32 token = stInfos.at(c).nStationToken;
        if(!m_StList.contains(token))
        {
            m_StList.insert(token , stInfos.at(c));
        }
        ++c;
    }
}

int CPndrCache::StationCount(ESortType sortType , QList<quint32>& tokenList)
{
    if(SORT_TYPE_ALPHA == sortType && m_AlphabetTokenList.count() >= 0)
    {
        tokenList = m_AlphabetTokenList;
    }
    else if(SORT_TYPE_DATE == sortType && m_DateTokenList.count() >= 0)
    {
        tokenList = m_DateTokenList;
    }
    return tokenList.count();
}

void CPndrCache::ClearInfo()
{
    m_AlphabetTokenList.clear();
    m_DateTokenList.clear();
    m_StList.clear();
}
