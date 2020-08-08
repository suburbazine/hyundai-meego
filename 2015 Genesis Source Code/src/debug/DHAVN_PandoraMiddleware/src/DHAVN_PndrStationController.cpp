/*
 ******************************************************************
 *        COPYRIGHT ?2011, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     : Station Controller                                *
 * PROGRAMMER : Dayasudhan Kuruva                                    *
 * DATE       : 07/12/2011                                        *
 * VERSION    : 0.1                                               *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module Selects the command to be encoded for given request
 *  and
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  07/12/2011  0.1 Dayasudhan Kuruva   CPP file created
 *  20/06/2013  0.2 Radhakrushna Mohanty Modified
 *
 ******************************************************************
 */

#include "DHAVN_PndrStationController.h"
#include "DHAVN_PndrFrameTxRx.h"
#include <QDir>

/**
* Class CPndrStationController
*
*/
CPndrStationController::CPndrStationController()
    : m_ActiveStationId(0) // added by esjang 2012.11.12 for Static Test CID 13139
    , m_eImgType(IMG_TYPE_JPEG) // added by esjang 2012.11.12 for Static Test CID 13139

{
    LOG_INIT_V2("Pandora");
    /* No need to check the ptr validation its ensured that in the state handler only its validated*/
    m_pFrameEncoder = NULL; // CPndrFrameEncoder::GetInstance();
    m_pFrameDecoder = NULL ; //CPndrFrameDecoder::GetInstance();
    m_pFrameTxRx    = NULL; // modified by esjang 2013.05.22 for static test # 13139
//    m_pFrameTxRx = CPndrFrameTxRx::GetInstance();

//    QObject::connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent ,const quint8 , const QList<quint32>& )),this,SLOT(OnRequestCompletion(const EPndrEvent ,const quint8 , const QList<quint32>& )));
//    QObject::connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent ,  const quint32 , const quint8  )),this,SLOT(OnRequestCompletion(const EPndrEvent , const quint32 , const quint8 )));
//    QObject::connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent , const quint32  )),this,SLOT(OnRequestCompletion(const EPndrEvent , const quint32 )));
//    QObject::connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent , const quint8  )),this,SLOT(OnRequestCompletion(const EPndrEvent , const quint8 )));


//    QObject::connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent ,const quint32 ,
//                                                          const quint32 ,const quint8 ,
//                                                          const quint8 ,const QByteArray& )),
//                     this,SLOT(OnRequestCompletion(const EPndrEvent ,const quint32 ,
//                                                   const quint32 ,const quint8 ,
//                                                   const quint8 ,const QByteArray&)));

    m_nSearchId = 0;

    m_SortType = SORT_TYPE_NONE;

    m_nArtStationToken = 0;

    connect(&m_Timer , SIGNAL(timeout()) , this , SLOT(RequestStArt()));
    m_Timer.setSingleShot(true);
    m_Timer.setInterval(500);
}

void CPndrStationController::SetFrameTxRX(CPndrFrameTxRx*& inFrameTxRx)
{
    m_pFrameTxRx =inFrameTxRx;
}
void CPndrStationController::SetEncoder(CPndrFrameEncoder* inFrameEncoder)
{
    m_pFrameEncoder = inFrameEncoder;
}

void CPndrStationController::SetDecoder(CPndrFrameDecoder* inFrameDecoder)
{
    m_pFrameDecoder = inFrameDecoder;

    QObject::connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent ,
        const quint8 , const QList<quint32>& )),this,
        SLOT(OnRequestCompletion(const EPndrEvent ,const quint8 , const QList<quint32>& )));

    QObject::connect(m_pFrameDecoder,
        SIGNAL(ReplyReceived(const EPndrEvent ,  const quint32 , const quint8  ))
        ,this,SLOT(OnRequestCompletion(const EPndrEvent , const quint32 , const quint8 )));

    QObject::connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent , const quint32  )),
       this,SLOT(OnRequestCompletion(const EPndrEvent , const quint32 )));

    QObject::connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent , const quint8  )),
       this,SLOT(OnRequestCompletion(const EPndrEvent , const quint8 )));

    QObject::connect(m_pFrameDecoder,SIGNAL(ReplyReceived(const EPndrEvent ,const quint32 ,
       const quint32 ,const quint8 ,const quint8 ,const QByteArray& )),
       this,SLOT(OnRequestCompletion(const EPndrEvent ,const quint32 ,
       const quint32 ,const quint8 ,const quint8 ,const QByteArray&)));
    //{ modified by yongkyun.lee 2014-08-25 for : Branding Image
    QObject::connect(m_pFrameDecoder,SIGNAL(ReceivedBranding()),
       this,SLOT(OnRequestbranding()));
    //} modified by yongkyun.lee 2014-08-12 

}


/**
* Class ~CPndrStationController
*
*/
CPndrStationController::~CPndrStationController()
{
    LOG_TRACE << "In CPndrStationController::~CPndrStationController())"<< LOG_ENDL;
    //m_pFrameEncoder->ReleaseInstance();
    m_pFrameEncoder = NULL;
    //m_pFrameDecoder->ReleaseInstance();
   // m_pFrameTxRx->ReleaseInstance();
    m_pFrameTxRx = NULL;
    m_pFrameDecoder = NULL;
    m_StationTokenList.clear();

    if(!m_ByteArr.isNull())
    {
        m_ByteArr.clear();
    }

    m_PendingStationArtList.clear();

    LOG_TRACE << "Exiting CPndrStationController::~CPndrStationController())"<< LOG_ENDL;
}

/**
 *******************************************************************
 * Request for clear all cache info .
 *
 * @param none
 * @return  none
 *
 *******************************************************************
 */
void CPndrStationController::ClearData()
{
    LOG_HIGH << "CPndrStationController::ClearData() "<< LOG_ENDL;
    m_StationTokenList.clear();
    m_PendingStationArtList.clear();
    m_ActiveStation.nStationToken = 0;
    m_ActiveStation.StationLabel.clear();
    m_ActiveStationId = 0;
    m_SortType = SORT_TYPE_NONE;
    m_ByteArr.clear();
    m_nArtStationToken = 0;
    m_QueueNextList.clear();
}

/**
 *******************************************************************
 * Request station Select
 *
 * Instructs the Mobile App to begin playing the given station. PNDR_UPDATE_STATION_ACTIVE will be sent
 *   when station changes.
 * @param[in]    inStationToken  station token
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStationController::RequestStationSelect(quint32 inStationToken)
{
    //LOG_TRACE << "In CPndrStationController::RequestStationSelect())"<< LOG_ENDL;
    ResetActiveStation();
    m_pFrameEncoder->EncodeAndSend(PNDR_EVENT_STATION_SELECT,inStationToken);
    //LOG_TRACE << "Exiting CPndrStationController::RequestStationSelect())"<< LOG_ENDL;
}


/**
 *******************************************************************
 * Request station sorting
 *
 * @param[in]    inSortType  The Sort type to be requested. It can
 *                              be alphabetical or date based
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStationController::RequestStationSort(const ESortType inSortType)
{
    //LOG_TRACE << "In CPndrStationController::RequestStationSort())"<< LOG_ENDL;

    m_pFrameEncoder->EncodeAndSend(PNDR_EVENT_STATIONS_SORT,(quint8)inSortType);

    //LOG_TRACE << "Exiting CPndrStationController::RequestStationSort())"<< LOG_ENDL;
}


/**
 *******************************************************************
 * Request station sort order
 *
 * @param[in]    None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStationController::RequestStationSortOrder()
{
    //LOG_TRACE << "In CPndrStationController::RequestStationSortOrder())"<< LOG_ENDL;
    m_pFrameEncoder->EncodeAndSend(PNDR_GET_STATIONS_ORDER);
    //LOG_TRACE << "Exiting CPndrStationController::RequestStationSortOrder())"<< LOG_ENDL;
}

/**
 *******************************************************************
 * Request station count
 *
 * @param None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStationController::RequestStationCount()
{
    //LOG_TRACE << "In CPndrStationController::RequestStationCount())"<< LOG_ENDL;
    m_pFrameEncoder->EncodeAndSend(PNDR_GET_STATION_COUNT);
    //LOG_TRACE << "Exiting CPndrStationController::RequestStationCount())"<< LOG_ENDL;
}

/**
 *******************************************************************
 * Request all station tokens
 *
 * @param None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStationController::RequestAllStationTokens()
{
    //LOG_TRACE << "In CPndrStationController::RequestAllStationTokens()"<< LOG_ENDL;
    m_pFrameEncoder->EncodeAndSend(PNDR_GET_ALL_STATION_TOKENS);
    //LOG_TRACE << "Exiting CPndrStationController::RequestAllStationTokens()"<< LOG_ENDL;
}

/**
 *******************************************************************
 * Request Station Information
 *
 * @param[in]    inStartIndex    Which is the start index for getting
 *                      the station list
 *                  inEndIndex      Which is the end index for getting
 *                      the station list
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATION_REQ_RANGE: Invalid range
 *
 *******************************************************************
 */
EPndrError CPndrStationController::RequestStationInfo(const qint8 inStartIndex,const qint8 inEndIndex)
{
    //LOG_TRACE << "In CPndrStationController::RequestStationInfo()"<< LOG_ENDL;
    LOG_INFO << "StationController inStartIndex = " << inStartIndex <<"inEndIndex = " << inEndIndex
             <<" Total count = " <<  m_StationTokenList.count() << LOG_ENDL;

    QList<quint32> stationIndexList;
    EPndrError retError = E_SUCCESS;
    if(inStartIndex > inEndIndex || m_StationTokenList.count() <= inEndIndex)
    {
        retError = E_INVALID_STATION_REQ_RANGE;
        LOG_CRITICAL << "Invalid Station range"<< LOG_ENDL;
    }
    else
    {
        stationIndexList.clear();

        for(qint8 i = inStartIndex; i <= inEndIndex ; ++i)
        {
            //LOG_INFO << "stationIndexList.count = " << stationIndexList.count()<< LOG_ENDL;
            stationIndexList.append(m_StationTokenList.at(i));
        }
        LOG_HIGH << "StationController req list count = " << stationIndexList.count()<< LOG_ENDL;
        m_pFrameEncoder->EncodeAndSend(PNDR_GET_STATION_INFO,stationIndexList);

#if 0
        LOG_INFO << "inStartIndex before clearing station art = " << inStartIndex<< LOG_ENDL;

        //Request for Station Art of given stations

        for(int i = 0; i <stationIndexList.count();i++)
        {
            if(IsStationArtExist(stationIndexList.at(i))){
               LOG_INFO << "file present : " << stationIndexList.at(i)<< LOG_ENDL;
                    continue;
            }

            LOG_INFO << "Requested for : "<<stationIndexList.at(i)<< ","<< LOG_ENDL;
            if(!m_PendingStationArtList.contains(stationIndexList.at(i)))
            {
                m_PendingStationArtList.insert(0,stationIndexList.at(i));

            }
        }

        LOG_INFO<<"After append m_PendingStationArtList.count :"<<m_PendingStationArtList.count()<<LOG_ENDL;
        if(m_PendingStationArtList.count()>0){
            for(int i =0;i<m_PendingStationArtList.count();i++)
            LOG_INFO<<"*** pending : "<<m_PendingStationArtList.at(i)<<LOG_ENDL;
            m_pFrameEncoder->EncodeAndSend(PNDR_GET_STATION_ART,m_pFrameTxRx->GetMaxPayloadLength(),
                                       m_PendingStationArtList);
        }
#endif

    }
    //LOG_TRACE << "Exiting CPndrStationController::RequestStationInfo()"<< LOG_ENDL;
    return retError;
}

 /**
     *******************************************************************
     * Fetch Station Art
     *
     * @param inStartIndex , inEndIndex , Not in use
     *
     * @return void
     *
     *******************************************************************
     */
void CPndrStationController::FetchStationArt(int inStartIndex, int inEndIndex)
{

    LOG_INFO << "inStartIndex before clearing station art = " << inStartIndex<< LOG_ENDL;

    //Request for Station Art of given stations

    if(m_PendingStationArtList.count() || m_QueueNextList.count())
    {
        LOG_INFO << "Already Station Art pending in queue "<< LOG_ENDL;
        return;
    }

//    m_PendingStationArtList.clear();
    for(int i = 0 ; i < m_StationTokenList.count() ;i++)
    {
        if(IsStationArtExist(m_StationTokenList.at(i))){
           //LOG_INFO << "file present : " << m_StationTokenList.at(i)<< LOG_ENDL;
                continue;
        }

        //LOG_INFO << "Requested for : "<<m_StationTokenList.at(i)<< ","<< LOG_ENDL;

        if(m_PendingStationArtList.count() < 20)
            m_PendingStationArtList.append(m_StationTokenList.at(i));
        else
           m_QueueNextList.append(m_StationTokenList.at(i));
    }

    LOG_INFO<<"After append m_PendingStationArtList.count :"<<m_PendingStationArtList.count()<<LOG_ENDL;
    if(m_PendingStationArtList.count()>0){
//        for(int i =0;i<m_PendingStationArtList.count();i++)
//        LOG_INFO<<"*** pending : "<<m_PendingStationArtList.at(i)<<LOG_ENDL;
        m_pFrameEncoder->EncodeAndSend(PNDR_GET_STATION_ART,m_pFrameTxRx->GetMaxPayloadLength(),
                                   m_PendingStationArtList);
    }
}

/**
 *******************************************************************
 * Request Station Information
 *
 * @param[in]    inStartIndex    Which is the start index for getting
 *                      the station list
 *                  inEndIndex      Which is the end index for getting
 *                      the station list
 *
 * @return     EPndrError: Its value can be
 *                E_SUCCESS: If successfully connected
 *                E_INVALID_STATION_REQ_RANGE: Invalid range
 *
 *******************************************************************
 */
void CPndrStationController::RequestCurrentStationInfo(const EImageType inImgType)
{
    LOG_TRACE << "In CPndrStationController::RequestCurrentStationInfo()"<< LOG_ENDL;
    m_eImgType = inImgType;
    QList<quint32> stationIndexList;
    stationIndexList.append(m_ActiveStationId);
    LOG_INFO << "RequestCurrentStationInfo: m_ActiveStationId = "<< m_ActiveStationId << LOG_ENDL;

    TSTrackInfo info; // dummy
    if(!IsStInfoAvail(info) )
    {
        m_pFrameEncoder->EncodeAndSend(PNDR_GET_STATION_INFO,stationIndexList);
    }

//    if(!IsStationArtExist(m_ActiveStationId)){
//    LOG_INFO<<"Request for Station Art of current station"<<LOG_ENDL;
//    m_pFrameEncoder->EncodeAndSend(PNDR_GET_STATION_ART,m_pFrameTxRx->GetMaxPayloadLength(),
//                                   stationIndexList);
//    }
    LOG_TRACE << "Exiting CPndrStationController::RequestCurrentStationInfo()"<< LOG_ENDL;
}

/**
 *******************************************************************
 * Request for current station art .
 *
 * @param none
 * @return  none
 *
 *******************************************************************
 */
void CPndrStationController::RequestCurrentStationArt()
{

    QList<quint32> stationIndexList;
    stationIndexList.append(m_ActiveStationId);
    if(!IsStationArtExist(m_ActiveStationId)){
    LOG_INFO<<"Request for Station Art of current station"<<LOG_ENDL;
    m_pFrameEncoder->EncodeAndSend(PNDR_GET_STATION_ART,m_pFrameTxRx->GetMaxPayloadLength(),
                                   stationIndexList);
    }
}

/**
 *******************************************************************
 * If StationArts presents in folder for specific token id
 *
 * @param inTokenId , station token  id
 *
 * @return  true , if avilable
 *
 *******************************************************************
 */
bool CPndrStationController::IsStationArtExist(qint32 inTokenId)
{
    bool exist = false;

    /* create file path from token id */
    QString filePath;
    filePath.append(STATION_ART_PATH);//append file path
    filePath.append(STATION_ART_FILE);//append file name
    QString stationNo;
    stationNo.setNum(inTokenId);
    filePath.append(stationNo);
    switch(m_eImgType)
    {
        case IMG_TYPE_JPEG:
        {
            filePath.append(STATION_ART_TYPE_JPEG);//append file extension
            break;
        }
        case IMG_TYPE_PNG:
        {
            filePath.append(STATION_ART_TYPE_PNG);//append file extension
            break;
        }
        case IMG_TYPE_RGB565:
        {
            filePath.append(STATION_ART_TYPE_RGB565);//append file extension
            break;
        }
        default:
        {
            break;
        }
    }

    //If the same filename exists previously
    if(QFile::exists(filePath))
    {
         exist = true;
    }

    return exist;
}

/**
 *******************************************************************
 * Request for auto search complete
 *
 * @param[in]    inSearchWord    String to be searched
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStationController::RequestAutoCompleteSearch(const QString& inSearchWord)
{
    LOG_TRACE << "In CPndrStationController::RequestAutoCompleteSearch()"<< LOG_ENDL;
    m_nSearchId++;
    LOG_INFO << "Search Id requested is "<< m_nSearchId << LOG_ENDL;
    m_pFrameEncoder->EncodeAndSend(PNDR_EVENT_SEARCH_AUTO_COMPLETE,m_nSearchId,inSearchWord);
    LOG_TRACE << "Exiting CPndrStationController::RequestAutoCompleteSearch()"<< LOG_ENDL;
}

/**
 *******************************************************************
 * Request to select a station for start playing
 *
 * @param[in]    inStationToken    Token station which is selected
 *                                    to be played
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStationController::RequestExtendedSearch(const QString& inSearchWord)
{
    LOG_TRACE << "In CPndrStationController::RequestExtendedSearch()"<< LOG_ENDL;
    m_nSearchId++ ;
    m_pFrameEncoder->EncodeAndSend( PNDR_EVENT_SEARCH_EXTENDED,m_nSearchId,inSearchWord);
    LOG_TRACE << "Exiting CPndrStationController::RequestExtendedSearch()"<< LOG_ENDL;
}
/**
 *******************************************************************
 * Request to get info for given search ID
 *
 * @param[in]    inSearchID    searchID for which the info
 *                                  is requested
 *
 * @return     None
 *to ask
 *******************************************************************
 */
void CPndrStationController::RequestSearchResultInfo(const quint32 inSearchID,const QList<quint32>& inStationTokens)
{
    LOG_TRACE << "In CPndrStationController::RequestSearchResultInfo()"<< LOG_ENDL;

    LOG_INFO << "RequestSearchResultInfo m_nSearchId = "<< m_nSearchId  << " inSearchID = " << inSearchID<< LOG_ENDL;

    if(m_nSearchId == inSearchID)
    {
        m_pFrameEncoder->EncodeAndSend(PNDR_GET_SEARCH_RESULT_INFO,inSearchID,inStationTokens);
    }
    else
    {
        LOG_CRITICAL<<"Incorrect Search Id. So simply Discard this request" << LOG_ENDL;
    }
    LOG_TRACE << "Exiting CPndrStationController::RequestSearchResultInfo()"<< LOG_ENDL;
}
/**
 *******************************************************************
 * Request to select an Item from search result
 *
 * @param[in]    inSearchID    The search ID which is in use
 * @param[in]    inToken       The token id for item within search
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrStationController::RequestSearchSelect(const quint32 inToken)
{
    LOG_TRACE << "In CPndrStationController::RequestSearchSelect()"<< LOG_ENDL;
    ResetActiveStation();
    m_pFrameEncoder->EncodeAndSend(PNDR_EVENT_SEARCH_SELECT,m_nSearchId,inToken);
    LOG_TRACE << "Exiting CPndrStationController::RequestSearchSelect()"<< LOG_ENDL;
}

//{ modified by yongkyun.lee 2014-08-25 for : Branding Image
void CPndrStationController::RequestBrandingImage(quint32 indata)
{
    LOG_TRACE << "CPndrStationController::RequestBrandingImage()"<< LOG_ENDL;
    m_pFrameEncoder->EncodeAndSend(PNDR_UPDATE_BRANDING_IMAGE , indata );
    return;
}
void CPndrStationController::RequestBrandingSegment( const quint8 SegIndex,const quint8 totalSeg, quint8* inData,  int fzise)
{
    LOG_TRACE << "CPndrStationController::RequestBrandingSegment"<< LOG_ENDL;
    m_pFrameEncoder->EncodeAndSend(PNDR_RETURN_BRANDING_IMAGE_SEGMENT , SegIndex, totalSeg , inData , fzise );
    return;
}
//} modified by yongkyun.lee 2014-08-04 

/**
 *******************************************************************
 * Save station tokens , If not received .
 *
 * @param inTokenList , Token list
 * @return  none
 *
 *******************************************************************
 */
void CPndrStationController::SaveStationTokens(const QList<quint32>& inTokenList)
{
    LOG_SLOT<< "In CPndrStationController::SaveStationTokens"<< LOG_ENDL;
    m_StationTokenList.clear();
    m_StationTokenList =  inTokenList;
}

/**
* Used for capturing signal for following
*    PNDR_RETURN_STATION_TOKENS
*/
void CPndrStationController::OnRequestCompletion(const EPndrEvent inRequest,const quint8 inStart_index, const QList<quint32>& inTokenList)
{
    Q_UNUSED(inRequest)
    Q_UNUSED(inStart_index)
    LOG_SLOT<< "In CPndrStationController::OnRequestCompletion(PNDR_RETURN_STATION_TOKENS)1)"<< LOG_ENDL;
    m_StationTokenList =  inTokenList;
    LOG_SLOT<< "Exiting CPndrStationController::OnRequestCompletion(PNDR_RETURN_STATION_TOKENS)1)"<< LOG_ENDL;
}
/**
* Used for capturing signal for following
*   PNDR_UPDATE_STATION_ADDED
*/
void CPndrStationController::OnRequestCompletion(const EPndrEvent inRequest, const quint32 inData, const quint8 inIndex)
{
    Q_UNUSED(inRequest)
    Q_UNUSED(inIndex)
    LOG_SLOT<< "In CPndrStationController::OnRequestCompletion(PNDR_UPDATE_STATION_ADDED)2)"<< LOG_ENDL;
    //{ added contains by wonseok.heo for ITS 243645
    if(m_StationTokenList.contains(inData)){
        LOG_CRITICAL<<"CPndrStationController::OnRequestCompletion <- inData ="<< inData<<LOG_ENDL;
        return;
    }//} added contains by wonseok.heo for ITS 243645

    m_StationTokenList.insert(inIndex,inData);
    LOG_SLOT<< "Exiting CPndrStationController::OnRequestCompletion(PNDR_UPDATE_STATION_ADDED)2)"<< LOG_ENDL;
}

/**
* Used for capturing signal for following
* PNDR_UPDATE_STATION_DELETED
* EVENT_STATION_ACTIVE
*/
void CPndrStationController::OnRequestCompletion(const EPndrEvent inRequest, const quint32 inData)
{
   // LOG_INFO<< "In CPndrStationController::OnRequestCompletion3(ACTIVE/DELETE)"<< LOG_ENDL;

    LOG_SLOT<< "CPndrStationController active/delete st . Req = "<< inRequest << "  Data = "<<inData<< LOG_ENDL;

    if(EVENT_STATION_DELETE == inRequest)
    {
        if(!m_StationTokenList.isEmpty())
        {
            m_StationTokenList.removeOne(inData);
        }
        else
        {
            LOG_CRITICAL << "Token List Empty"<< LOG_ENDL;
        }

        //Once the station is deleted remove that token from the list
        if(m_PendingStationArtList.count()  &&
                m_PendingStationArtList.indexOf(inData) != -1)
        {
            m_PendingStationArtList.removeAt(m_PendingStationArtList.indexOf(inData));
        }

        //If same station get deleted ,  clear data.
        if(m_ActiveStationId == inData)
        {
            ResetActiveStation();
        }
    }
    //Dnt update m_ActiveStationId to wrong station ID
    if(EVENT_STATION_ACTIVE == inRequest && inData > 0)
    {
        //If station changed clear old data.
        if(m_ActiveStationId != inData)
        {
            ResetActiveStation();
        }

        m_ActiveStationId = inData;
        m_ActiveStation.nStationToken = m_ActiveStationId;
//        if(SORT_TYPE_NONE == m_SortType ){
//            LOG_TRACE<<"Request station sort order in advance"<<LOG_ENDL;
//            RequestStationSortOrder();
//        }
    }

   // LOG_INFO<< "Exiting CPndrStationController::OnRequestCompletion3(ACTIVE/DELETE)"<< LOG_ENDL;
}


/**
* Used for capturing signal for following
* PNDR_RETURN_STATIONS_ORDER
*/
void CPndrStationController::OnRequestCompletion(const EPndrEvent inRequest, const quint8 inData)
{
    LOG_SLOT<< "CPndrStationController::OnRequestCompletion:Entry inRequest["<<inRequest<<"] inData["<<inData<<"]" <<LOG_ENDL;  //BCH_CHECK. setting sort type.

    if(EVENT_SORT_ORDER == inRequest || EVENT_SORT == inRequest)
    {
        if(inData == 1 )
        {
            m_SortType = SORT_TYPE_ALPHA;
        }
        else if(inData == 0)
        {
            m_SortType = SORT_TYPE_DATE;
        }

    }

    //LOG_INFO<<" CPndrStationController Sort order =  " << m_SortType<<LOG_ENDL;

    //LOG_INFO<< "CPndrStationController::OnRequestCompletion:Exit" <<LOG_ENDL;

}


/**
  *******************************************************************
  * Slot for receiving the signal for completion of
  *              station Art
  *              PNDR_RETURN_STATION_ART_SEGMENT
  *
  *  @param[in]  inRequest   Request type for which this is the
  *                          response
  *  @param[in]  inStationToken     The station token
  *  @param[in]  inArtLength        The art length
  *  @param[in]  inSegmentindex     The segment index
  *  @param[in]  inTotalsegments     Total Segments
  *  @param[in]  inData     Data
  *
  * @return  None
  *
  *******************************************************************
  */

void CPndrStationController::OnRequestCompletion(const EPndrEvent inRequest,
                                            const quint32 inStationToken,
                                            const quint32 inArtLength,
                                            const quint8 inSegmentindex,
                                            const quint8 inTotalsegments,
                                            const QByteArray& inData)
{
    //LOG_INFO<< "Station Art segment recvd for "<< inStationToken << LOG_ENDL;

    if((EVENT_GET_STATION_ART == inRequest) && inArtLength)
    {
        LOG_INFO << "St. Art recved Exc St.Token = "<<m_nArtStationToken << " Rec St. Token = " << inStationToken << LOG_ENDL ;

        if(m_nArtStationToken != inStationToken || inSegmentindex == 0 )
        {
            LOG_INFO << "Token does not match or start , prev data cleared" <<LOG_ENDL;
            m_ByteArr.clear();
            m_nArtStationToken = inStationToken;
        }


//        if(!inData.isNull())
//        {
            m_ByteArr.append(inData);
//            LOG_INFO << " station art data appended: "<< LOG_ENDL ;

//        }
        /* If  all the segments are download completed, then write it to a file */
        if((inSegmentindex + 1) == inTotalsegments)
        {

           // LOG_INFO << "All Station Art data received m_ByteArr = :" << m_ByteArr.toHex() <<LOG_ENDL;

            QString stationNo;
            stationNo.setNum(inStationToken);
            /* File to store albumart picture*/
            QFile stationArtFile;
            /* Write to cache file */
            QString filePath;
            filePath.append(STATION_ART_PATH);//append file path

            QDir stationArtDir(filePath);
            //If the Station Art folder doesnt exist, create one
            if(!stationArtDir.exists())
            {
                stationArtDir.mkpath(STATION_ART_PATH);
            }

            filePath.append(STATION_ART_FILE);//append file name
            filePath.append(stationNo);

            switch(m_eImgType)
            {
                case IMG_TYPE_JPEG:
                {
                    filePath.append(STATION_ART_TYPE_JPEG);//append file extension
                    break;
                }
                case IMG_TYPE_PNG:
                {
                    filePath.append(STATION_ART_TYPE_PNG);//append file extension
                    break;
                }
                case IMG_TYPE_RGB565:
                {
                    filePath.append(STATION_ART_TYPE_RGB565);//append file extension
                    break;
                }
                default:
                {
                    break;
                }
            }

            //append the file name
            stationArtFile.setFileName(filePath);

            //If the same filename exists previously, delete it and create a new one
            if(QFile::exists(filePath))
            {
                bool del = QFile::remove(filePath);
                //LOG_TRACE<< "Station Art File removed "<< del << LOG_ENDL;
            }

            if(stationArtFile.open(QIODevice::WriteOnly))
            {
                stationArtFile.write(m_ByteArr);               
                stationArtFile.close();
                m_ByteArr.clear();

//                TSStationArt stArt;
//                stArt.nStationToken = inStationToken;
//                stArt.StationArtPath = filePath ;//stationArtFile.fileName();

                if(m_PendingStationArtList.contains(inStationToken))
                {
                    m_PendingStationArtList.removeOne(inStationToken);
                }

                emit StationArtReceived(inStationToken);


                if(!m_Timer.isActive() && m_PendingStationArtList.count() < 10)
                    m_Timer.start();
            }
            else
            {

                m_ByteArr.clear();
                m_nArtStationToken = 0;
                LOG_TRACE<< "Error , St. art file opening"<< LOG_ENDL;
            }

           // LOG_INFO<< "Exiting CPndrStationController::OnRequestCompletion "<< LOG_ENDL;
        }
    }
}

//{ modified by yongkyun.lee 2014-08-25 for : Branding Image
void CPndrStationController::OnRequestbranding()
{
    LOG_HIGH<< "CPndrStationController::OnRequestbranding()"<<LOG_ENDL;
    emit getBrandingImage();
}
//} modified by yongkyun.lee 2014-08-12 

void CPndrStationController::RequestStArt()
{

    if(m_PendingStationArtList.count() < 10 && m_QueueNextList.count() > 0)
    {
        for(int i = 0; i < 20 && i < m_QueueNextList.count() ; i++)
        {
            m_PendingStationArtList.append(m_QueueNextList.first());
            m_QueueNextList.removeFirst();
        }

        m_pFrameEncoder->EncodeAndSend(PNDR_GET_STATION_ART,m_pFrameTxRx->GetMaxPayloadLength(),
                                   m_PendingStationArtList);
    }
    else{
        LOG_HIGH<< "Station art request list is cleared"<<LOG_ENDL;
    }
}



/**
 *******************************************************************
 * Clears the previous StationArts if any from the folder before making
 * a new request
 * @param None
 *
 * @return  None
 *
 *******************************************************************
 */
void CPndrStationController::ClearStationArtFolder()
{
    LOG_TRACE<< "Entry CPndrStationController::ClearStationArtFolder()) "<< LOG_ENDL;

    QString filePath;
    filePath.append(STATION_ART_PATH);//append file name
    QDir stationArtDir(filePath);

    //If the Station Art folder doesnt exist, create one
    if(!stationArtDir.exists())
    {
        stationArtDir.mkpath(STATION_ART_PATH);
    }
    else
    {
        //Check if any Station Art file is present and delete them
        for(uint i= 0; i < stationArtDir.count();i++)
        {
            stationArtDir.remove(stationArtDir.entryList().at(i));
        }
    }

    LOG_TRACE<< "Exiting CPndrStationController::ClearStationArtFolder()) "<< LOG_ENDL;

}

/**
 *******************************************************************
 * Cancels any outstanding request of station Art when user goes back
 * from List view
 *
 * @return  void
 *
 *******************************************************************
 */
void CPndrStationController::CancelStationArtRequest()
{
    LOG_TRACE<<"CPndrStationController::CancelStationArtRequest Entry"<<LOG_ENDL;

    if(m_Timer.isActive())
        m_Timer.stop();

    if(m_PendingStationArtList.count() > 0){
        m_pFrameEncoder->EncodeAndSend(PNDR_EVENT_CANCEL_STATION_ART);
        m_PendingStationArtList.clear();
    }

    if(m_QueueNextList.count() >0)
    {
        m_QueueNextList.clear();
    }

    LOG_TRACE<<"CPndrStationController::CancelStationArtRequest Exit"<<LOG_ENDL;
}

/**
 *******************************************************************
 * Returns the current sort order
 *
 * @return  ESortType the current sort type
 *
 *******************************************************************
 */
ESortType CPndrStationController::CurrentSortOrder()
{
   LOG_HIGH<<"In CPndrStationController::CurrentSortOrder : m_SortType = "<<m_SortType<<LOG_ENDL;

   return m_SortType;
}

/**
 *******************************************************************
 * Returns the status of station info availability and fill the same
 *
 * @param inTrack, update station info , if available
 * @return  true , If available
 *
 *******************************************************************
 */
bool CPndrStationController::IsStInfoAvail(TSTrackInfo& inTrack)
{
    bool ret = false;
    QString label(m_ActiveStation.StationLabel);

    if(label.length() > 0)
    {
        inTrack.bShared = m_ActiveStation.bShared ;
        inTrack.StationToken = m_ActiveStation.nStationToken ;
        inTrack.CurrentStation = label ;
        ret = true;
    }
    return ret;
}

/**
 *******************************************************************
 * Save active station info
 *
 * @param inStation, Active station info to be be saved
 * @return  none
 *
 *******************************************************************
 */
void CPndrStationController::SetActiveStation(TSTrackInfo& inStation)
{
    m_ActiveStation.nStationToken = inStation.StationToken;
    m_ActiveStation.StationLabel = inStation.CurrentStation;
    m_ActiveStation.bShared = inStation.bShared;
}

/**
 *******************************************************************
 * Clear station info on station change
 *
 * @param none
 * @return  none
 *
 *******************************************************************
 */
void CPndrStationController::ResetActiveStation()
{
    m_ActiveStation.nStationToken = 0;
    m_ActiveStation.StationLabel.clear();
}

/**
 *******************************************************************
 * Request to get current active station
 *
 * @param none
 * @return  none
 *
 *******************************************************************
 */
void CPndrStationController::GetActiveStationToken()
{
    m_pFrameEncoder->EncodeAndSend(PNDR_GET_STATION_ACTIVE);
}

//End Of File
