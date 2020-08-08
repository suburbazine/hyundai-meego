#include "DHAVN_AppPandoraStationList.h"
#include "DHAVN_AppPandora_Def.h"

#include <QStringList>
#include <DHAVN_PndrStateHandler.h>

#include <QDebug>
#include <QVariant>
#include <QMetaType>
#include <QTime> //added by esjang 2014.01.08 for check time
QTime station_time; //aded by esjang 2014.01.08 for check time 

/**
 *******************************************************************
 * Station list constructor
 *
 * @param[in] inPndrInterface   interface reference to Pandora MW
 *
 * @return  None
 *
 *******************************************************************
 */
CAppPandoraStationList::CAppPandoraStationList(/*CPndrInterface&  inPndrInterface*/):
    m_PndrInterface(NULL)
  , m_pSignals(NULL)
  , m_eCurrentRequest(REQ_TYPE_NONE)
  , m_nStationCount(0)
  , m_nActiveStationToken(0)
  , m_nRelativeActiveStationIndex(0)
  , m_nMaxCount(0)
  , m_nStartIndex(-1)
  , m_nEndIndex(-1)
  , m_nReqStartingIndex(-1)
  , m_eSortType(SORT_TYPE_NONE)
  , m_eListReqType(0)
  , m_nIs1stTimeLaunch(1)
  , m_nTuningIndex(-1)
  , m_bIsTuningInProgress(false)
  , m_RequestInprogress(false)
  , m_VrReqToPlayShuffle(false)
  , m_ShuffleStationToken(0)
  , m_PreFetchingStations(false)
{

    LOG_INIT_V2("Pandora");
}

/**
*******************************************************************
* SetInterface : Set Interface based on device on FG
* @param m_PndrInterface , Instance of current interface
* @return None
*
*******************************************************************
*/
void CAppPandoraStationList::SetInterface(CPndrInterface*  inPndrInterface)
{
    LOG_LOW <<"CAppPandoraStationList::SetInterface"<<LOG_ENDL;
    m_PndrInterface = NULL;
    m_PndrInterface = inPndrInterface;
    if(m_pSignals){
        disconnect(m_pSignals,0,this,0);
        m_pSignals=NULL;
    }
    if(inPndrInterface == NULL)
    {
        LOG_CRITICAL<<"Interface is NULL,so return "<<LOG_ENDL;
        return;        
    }
    m_pSignals = &(m_PndrInterface->GetSignalProvider());


    Qt::ConnectionType ConnType = Qt::QueuedConnection;
    qRegisterMetaType<ESortType>("ESortType");
    connect(m_pSignals,
            SIGNAL(StationInfoList(const QList<TSStationInfo>& )),
            this,
            SLOT(OnStationInfoList(const QList<TSStationInfo>& )));

    connect(m_pSignals,
            SIGNAL(StationTokens(const quint8, const QList<quint32>&)),
            this,
            SLOT(OnStationTokens(const quint8, const QList<quint32>&)));


     connect(m_pSignals,
            SIGNAL(StationDeleted(const quint32 )),
            this,
            SLOT(OnStationDeleted(const quint32 )),
            ConnType);

     connect(m_pSignals,
            SIGNAL(StationAdded(const quint8 , const TSStationInfo& )),
            this,
            SLOT(OnStationAdded(const quint8 , const TSStationInfo& )));

     connect(m_pSignals,
            SIGNAL(StationSortOrder(const ESortType )),
            this,
            SLOT(OnStationSortOrder(const ESortType )),ConnType);

     connect(m_pSignals,
             SIGNAL(StationArtReceived(quint32 )),
             this,
             SLOT(OnStationArtReceived(quint32)));

     connect(m_pSignals,
         SIGNAL(StationUpdated(const quint32)),
         this,
         SLOT(OnStationUpdated(const quint32 )),ConnType);

     connect(m_pSignals,
         SIGNAL(currentStationInfo(const TSTrackInfo& )),
         this,
         SLOT(OnCurrentStationInfo(const TSTrackInfo& )),
         ConnType);

}

/**
 *******************************************************************
 * Station list Destructor
 *
 * @param None
 *
 * @return  None
 *
 *******************************************************************
 */
CAppPandoraStationList::~CAppPandoraStationList()
{

}

/**
 *******************************************************************
 * Clear the cache on back soft key .
 *
 * @param  None
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraStationList::ClearCache()
{
    LOG_TRACE<<"CAppPandoraStationList::ClearCache() <-"<<LOG_ENDL;
    if(m_StationInfo.count() > 0)
    {
        m_StationInfo.clear();
    }
    m_eSortType = SORT_TYPE_NONE;
    ResetValue();
    m_eCurrentRequest = REQ_TYPE_NONE ;
    m_nIs1stTimeLaunch = 1;    
    m_StationTokens.clear();
    m_StationLabel.clear();
    //{ modified by cheolhwan 2014-04-17. To fix the unlimited receiving station. (ITS 238629)
    //m_PreFetchingStations =false;
    setPreFetchingStations(false);
    //} modified by cheolhwan 2014-04-17. To fix the unlimited receiving station.
    //{ modified by yongkyun.lee 2014-12-16 for : ITS 254629
    m_bIsTuningInProgress = false;
    m_RequestInprogress = false;
    //} modified by yongkyun.lee 2014-12-16
    LOG_TRACE<<"CAppPandoraStationList::ClearCache()->"<<LOG_ENDL;
}

/**
 *******************************************************************
 * Clear current request state .
 *
 * @param  None
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraStationList::ClearCurrentRequest()
{
    if(m_StationInfo.count() > 0)
    {
        m_StationInfo.clear();
    }
    m_eSortType = SORT_TYPE_NONE;
    ResetValue();
    m_eCurrentRequest = REQ_TYPE_NONE ;
    //{ modified by cheolhwan 2014-04-17. To fix the unlimited receiving station. (ITS 238629)
    //m_PreFetchingStations =false;
    setPreFetchingStations(false);
    //} modified by cheolhwan 2014-04-17. To fix the unlimited receiving station.
}


/**
 *******************************************************************
 * Reset values
 *
 * @param   None
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraStationList::ResetValue()
{
    m_nStationCount = 0 ;
    m_nStartIndex = -1 ;
    m_nEndIndex = -1 ;
    m_nReqStartingIndex = -1;
    m_VrReqToPlayShuffle = false;
    m_ShuffleStationToken = 0;    
}


/**
 *******************************************************************
 * Request from QML layer for getting the 1st n number of stations
 *
 * @param[in] inMaxCount Max number of stations that QML layer can
 *                          receive
 * @param[in] isAlphaSort Station list is requested in alphabatical
 *                        order if its value is true else date wise
 *                        sorting will be applied
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraStationList::GetFirstStationListSlot(const int inMaxCount,
                                                     quint32 inActiveStationToken)
{
    LOG_TRACE<<"CAppPandoraStationList::GetFirstStationListSlot <- inMaxCount="<<inMaxCount<<", inActiveStationToken="<<inActiveStationToken<<LOG_ENDL;

    if(REQ_TYPE_NONE != m_eCurrentRequest)
    {
        LOG_INFO<<"Request made in wrong state :"<<m_eCurrentRequest <<LOG_ENDL;
        return;
    }
    m_eCurrentRequest = REQ_TYPE_STATION_LIST ;  /** Station List request */
    ESortType sortType = m_PndrInterface->CurrentSortOrder();

     LOG_INFO<<"sortType : "<<sortType<< " , m_eSortType : "<< m_eSortType<<LOG_ENDL;

     if(SORT_TYPE_ALPHA == sortType)
     {
         m_AlphaListIndexTable.clear();
     }
     ResetValue();
     m_StationInfo.clear();
     m_StationTokens.clear();
     m_nMaxCount = inMaxCount ;
     if(inActiveStationToken> 0)
        m_nActiveStationToken = inActiveStationToken;
     m_eListReqType = PandoraListReqType::EFIRSTLIST;
     m_eSortType = sortType;

     LOG_INFO<<" Current m_nActiveStationToken : "<<m_nActiveStationToken<<LOG_ENDL;

     if( m_nIs1stTimeLaunch && sortType!=SORT_TYPE_ALPHA && sortType != SORT_TYPE_NONE)
        {
            LOG_HIGH<<" its 1st time launch, set sort type to ALPHA"<<LOG_ENDL;
            m_nIs1stTimeLaunch = 0;
            HandleIfError(m_PndrInterface->RequestStationSort(SORT_TYPE_ALPHA));
        }
     else
       {          
           if(SORT_TYPE_NONE == sortType)
           {
               m_eCurrentRequest = REQ_TYPE_SORT;
               LOG_HIGH<<"No Sort type yet. So first get Sort order "<<LOG_ENDL;
               HandleIfError(m_PndrInterface->RequestStationSortOrder());
           }
           else if(sortType == m_eSortType)
           {
               LOG_HIGH<<"Sort type is same. So request station count "<<LOG_ENDL;
               m_nIs1stTimeLaunch = 0;
               HandleIfError(m_PndrInterface->RequestStationCount());
           }
           else
           {
               LOG_HIGH<<"Sort type changed. So request station sort "<<LOG_ENDL;
               HandleIfError(m_PndrInterface->RequestStationSort(sortType));
           }
       }

    LOG_TRACE<<"CAppPandoraStationList::GetFirstStationListSlot ->"<<LOG_ENDL;
}

/**
 *******************************************************************
 * Request from QML layer for getting Sorted stations list of
 *
 * @param[in] sortType : values can be any of the PandoraSortTypes
 *
 * @param[in] inActiveStationToken : current Active Station token
 *              for finding index in latest list
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraStationList::GetSortedStationList(int inSortType, quint32 inActiveStationToken)
{
    LOG_TRACE<<"CAppPandoraStationList::GetSortedStationList <-"<<LOG_ENDL;

    if(REQ_TYPE_NONE != m_eCurrentRequest)
    {
        LOG_CRITICAL<<"Request made in wrong state :"<<m_eCurrentRequest<<LOG_ENDL;
        return;
    }
    m_eCurrentRequest = REQ_TYPE_STATION_LIST ;  /** Station List request */
    ESortType sortType = SORT_TYPE_DATE;
    if(static_cast<PandoraSortTypes::EStationListSortType>(inSortType) == PandoraSortTypes::SORT_BY_ALPHA)
    {
        m_AlphaListIndexTable.clear();
        sortType =  SORT_TYPE_ALPHA;

    }
    LOG_INFO<<"Sorting type : "<<sortType<<LOG_ENDL;
    m_eSortType = sortType ;
    ResetValue();
    m_StationInfo.clear();
    m_StationTokens.clear();
    m_nIs1stTimeLaunch = 0;
    m_nActiveStationToken = inActiveStationToken;
    m_eListReqType = PandoraListReqType::EFIRSTLIST;
    HandleIfError(m_PndrInterface->RequestStationSort(sortType));

    LOG_TRACE<<"CAppPandoraStationList::GetSortedStationList ->"<<LOG_ENDL;

}


/**
 *******************************************************************
 * Request from QML layer for getting the next n number of stations
 *
 * @param[in] inMaxCount Max number of stations that QML layer can
 *                          receive
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraStationList::GetStationListSlot(const int inStartIndex, const int inEndIndex)
{
    LOG_TRACE<<"CAppPandoraStationList::GetStationListSlot <- ,start="<< inStartIndex << ", end=" <<inEndIndex <<LOG_ENDL;

    if( inStartIndex <= inEndIndex )
    {
       int tempS = m_nStartIndex;
       int tempE = m_nEndIndex;
       m_nStartIndex = (inStartIndex <= m_nStartIndex) ? inStartIndex : m_nStartIndex ;
       m_nEndIndex = (inEndIndex >=m_nEndIndex) ? inEndIndex : m_nEndIndex;
       m_eCurrentRequest = REQ_TYPE_STATION_LIST;
       m_nReqStartingIndex = inStartIndex ;
       EPndrError error = m_PndrInterface->RequestStationList(inStartIndex,inEndIndex);

       if(error != E_SUCCESS)
       {
           if(m_PreFetchingStations)
           {
               //{ modified by cheolhwan 2014-04-17. To fix the unlimited receiving station. (ITS 238629)
               //m_PreFetchingStations =false;
               setPreFetchingStations(false);
               //} modified by cheolhwan 2014-04-17. To fix the unlimited receiving station.
           }
           m_nReqStartingIndex = -1;
           m_nStartIndex = tempS;
           m_nEndIndex = tempE;
           emit requestFail();
           HandleIfError(error);
       }
    }
    else
    {
       if(m_PreFetchingStations)
       {
           //{ modified by cheolhwan 2014-04-17. To fix the unlimited receiving station. (ITS 238629)
           //m_PreFetchingStations =false;
           setPreFetchingStations(false);
           //} modified by cheolhwan 2014-04-17. To fix the unlimited receiving station.
       }
       ResetValue();
    }
    LOG_TRACE<<"CAppPandoraStationList::GetStationListSlot ->"<<LOG_ENDL;

}


/**
 *******************m_nEndIndex************************************************
 * Request from QML layer for selecting the station given the index
 * from station list
 *
 * @param[in] index index of selected station
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraStationList::SelectStation(const int index)
{
    LOG_TRACE<<"CAppPandoraStationList::SelectStation <-" <<LOG_ENDL;
    LOG_INFO<<"Index requested :"<<index<<"Station list count :"<<m_StationInfo.count()<<LOG_ENDL;

    if(m_StationInfo.count()>index)
    {
        emit toastLoadingPopup();// modified by yongkyun.lee 2014-09-01 for : Pandora 4.25 TEST
        HandleIfError( m_PndrInterface->RequestStationSelect(m_StationInfo.at(index).nStationToken));
    }
    LOG_TRACE<<"CAppPandoraStationList::SelectStation ->" <<LOG_ENDL;
}

/**
 *******************************************************************
 * Request from QML layer for selecting the station given the name
 * from station list
 *
 * @param[in] name , name  of selected station
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraStationList::SelectStation(const QString stationName)
{
 LOG_TRACE<<"CAppPandoraStationList::SelectStation <-" <<LOG_ENDL;
 LOG_INFO<<"station Count : "<<m_StationInfo.count()<<"Station name :"<<stationName<<LOG_ENDL;

    for (int index = 0 ; index < m_StationInfo.count() ; index++)
    {
        if( m_StationInfo.at(index).StationLabel.compare(stationName) == 0 )
        {
            emit toastLoadingPopup();  // modified by yongkyun.lee 2014-09-01 for : Pandora 4.25 TEST   
            HandleIfError(m_PndrInterface->RequestStationSelect(m_StationInfo.at(index).nStationToken));
            break;
        }
    }

 LOG_TRACE<<"CAppPandoraStationList::SelectStation ->" <<LOG_ENDL;
}


/**
 *******************************************************************
 * Slot to listen to signal from Pandora MW for receiving the
 * station info list
 *
 * @param[in] inStationInfoList Station list info.
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraStationList::OnStationInfoList(const QList<TSStationInfo>& inStationInfoList)
{
    LOG_SLOT<<"CAppPandoraStationList::OnStationInfoList <- inStationInfoList.count():"<< inStationInfoList.count() <<LOG_ENDL;

    m_eCurrentRequest = REQ_TYPE_NONE ;

    m_RequestInprogress = false;

    if(inStationInfoList.count() == m_StationTokens.count())
    {
        m_nStartIndex = 0;
        m_nEndIndex = m_StationTokens.count() - 1;
    }

    int startIndex = m_nReqStartingIndex;
    if(inStationInfoList.count() > 0)
    {
        startIndex = GetStationIndex(inStationInfoList.at(0).nStationToken);
    }
    //LOG_LOW<<"OnStationInfoList->startIndex: "<< startIndex<<LOG_ENDL;

    if(startIndex < 0)
    {
       LOG_HIGH<< "******RETURNED OnStationInfoList startIndex is NEGATIVE , count : "<<inStationInfoList.count() <<LOG_ENDL;
       if(inStationInfoList.count() > 0)
       {
           LOG_HIGH<<"OnStationInfoList Received 1st token:" << inStationInfoList.at(0).nStationToken <<LOG_ENDL;
           LOG_HIGH<<"OnStationInfoList m_StationTokens.count() "<< m_StationTokens.count()<<LOG_ENDL;
       }
       return;
    }

    for(int c  = 0 ; c < inStationInfoList.count(); c++)
    {

        TSStationInfo stationInfoList = inStationInfoList.at(c);
        /** // removed by jyjeon 2013.12.09 for UX Update
        if(stationInfoList.bShared && !stationInfoList.bQuickMix)
        {
            stationInfoList.StationLabel.append(" [Shared]");
        }
        // removed by jyjeon 2013.12.09 for UX Update **/
        //{ modified by esjang 2014.01.08 for ITS # 218509
        if(m_StationInfo.count()>= (/*startIndex +*/ c)) //modified by jyjeon 2014-04-04 for NAQC
        {
            m_StationInfo.replace((/*startIndex+*/c) , stationInfoList); //modified by jyjeon 2014-04-04 for NAQC
            /*
            m_StationInfo.at((startIndex+c)).nStationToken = stationInfoList.nStationToken;
            m_StationInfo.at((startIndex+c)).bQuickMix = stationInfoList.bQuickMix;
            m_StationInfo.at((startIndex+c)).StationLabel = stationInfoList.StationLabel;
            m_StationInfo.at((startIndex+c)).bShared = stationInfoList.bShared;
            */
        }
        else
            LOG_CRITICAL << "OnStationInfoList m_StationInfo.count(): "<< m_StationInfo.count()<< " startIndex: "<< startIndex << " c: "<< c <<LOG_ENDL;
        //} modified by esjang 2014.01.08 for ITS # 218509

      //  stToken.append(QString::number(stationInfoList.nStationToken));

        if( !m_StationLabel.contains(stationInfoList.nStationToken) )
            m_StationLabel.insert(stationInfoList.nStationToken , stationInfoList.StationLabel);
    }

    if(m_eListReqType == PandoraListReqType::ENONELIST)
    {
        if(startIndex < GetStationIndex(m_nActiveStationToken))
        {
            m_eListReqType|=PandoraListReqType::EPREVLIST;
        }
        else{
            m_eListReqType|=PandoraListReqType::ENEXTLIST;
        }
    }

    ESortType inType = SORT_TYPE_DATE;
    if(m_eSortType == SORT_TYPE_ALPHA)
    {
        EvaluateAlphaSortedListIndexes();
        inType = SORT_TYPE_ALPHA ; // alphabetically sorted list received
    }
    m_nRelativeActiveStationIndex = GetActiveStationIndex();

    LOG_CRITICAL<<"PreFetching status :"<<m_PreFetchingStations<<", startIndex="<<  startIndex <<" list count="<< inStationInfoList.count()<<LOG_ENDL;
    if(!m_PreFetchingStations){
        emit stationListReceived( startIndex, inStationInfoList.count(),static_cast<quint16>(inType),
                             m_nRelativeActiveStationIndex, m_eListReqType );
    }
    else{
        LOG_CRITICAL<<"PreFetching in progress ,m_nEndIndex : "<<m_nEndIndex <<
                    ", token count :"<<m_StationTokens.count()<<LOG_ENDL;
        if(m_nEndIndex < (m_StationTokens.count() - 1 ))
            GetNextStationList(PandoraListReqType::ENEXTLIST);
        else
        {
           // added by esjang 2014.01.08 for check time
           LOG_CRITICAL<<"PreFetching Finished  , Inform to QML"<< " Receiving station [Time : " << station_time.elapsed()<< "msec] " <<LOG_ENDL;
           //{ modified by cheolhwan 2014-04-17. To fix the unlimited receiving station. (ITS 238629)
           //m_PreFetchingStations =false;
           setPreFetchingStations(false);
           //} modified by cheolhwan 2014-04-17. To fix the unlimited receiving station.
           emit preFetchingCompleted();
        }
    }

    LOG_SLOT<<"CAppPandoraStationList::OnStationInfoList ->"<<LOG_ENDL;
}

/**
 *******************************************************************
 * GetStationTokenIndex - for finding the current station token index
  * @param quint32 : Station token
 *
 * @return qint8 : return current active station index
 *
 *******************************************************************
 */
qint8 CAppPandoraStationList::GetStationIndex(const quint32 inToken)
{
    qint8 index = -1;
    for(int i = 0; i < m_StationTokens.count(); i ++)
    {
        if(m_StationTokens.at(i) == inToken)
        {
            index = i;
            break;
        }
    }
    return index;
}

/**
 *******************************************************************
 * GetActiveStationIndex - for finding the current active station index
 * stations list
 *
 * @return qint8 : return current active station index
 *
 *******************************************************************
 */
qint8 CAppPandoraStationList::GetActiveStationIndex()
{
    qint8 activeStationIndex = -1;
    for(qint8 i = 0; i < m_StationInfo.count(); i ++)
    {
        if(m_StationInfo.at(i).nStationToken == m_nActiveStationToken)
        {
            activeStationIndex = i;
            break;
        }
    }
    LOG_HIGH<<"CAppPandoraStationList:: Active st. index in st. info list = " << activeStationIndex<<LOG_ENDL;
    return activeStationIndex;
}


/**
 *******************************************************************
 * EvaluateAlphaSortedListIndexes - for finding the start indexes of alpha sorted
 * stations list
 *
 * @return none
 *
 *******************************************************************
 */
void CAppPandoraStationList::EvaluateAlphaSortedListIndexes()
{

    m_AlphaListIndexTable.clear();
    for(int i = 0; i < m_StationInfo.count(); i++)
    {
        if(m_StationInfo.at(i).bQuickMix)
            continue;
        if(m_StationInfo.at(i).StationLabel.length() < 0)
            continue;

        char key ;
        QString str = m_StationInfo.at(i).StationLabel.simplified();
        //{ added by cheolhwan 2014-02-11. To prevent crash.
        if(str.length() < 1)
        {
            LOG_CRITICAL<<"CAppPandoraStationList:: StationLabel is null "<<LOG_ENDL;
            continue;
        }
        //} added by cheolhwan 2014-02-11. To prevent crash.
        if(str.startsWith("a ",Qt::CaseInsensitive) || str.startsWith("an ",Qt::CaseInsensitive)
                || str.startsWith("the ",Qt::CaseInsensitive))
        {
            key = str.split(" ").at(1).at(0).toLower().toAscii();
        }
        else
        {
            key = str.at(0).toLower().toAscii();
        }

        if(QChar(key).isDigit())
            key = '#';

        if(!m_AlphaListIndexTable.contains(key))
        {
            LOG_LOW << "index for " << key << " is : " << i << LOG_ENDL;
            m_AlphaListIndexTable[key] = i;
        }
    }
}


/**
 *******************************************************************
 * Request from QML layer for getting the key value of station name
 *
 * @param[in] inStr Station Name
 *
 * @return Key
 *
 *******************************************************************
 */
    //{ modified by yongkyun.lee 2014-06-03 for : ITS 238641
QString CAppPandoraStationList::Key(QString inStr , bool isBT)
{
     QString key = "";
     //QString str = m_StationInfo.at(i).StationLabel.simplified();

    if(isBT)
    {
         if(inStr.startsWith("a ",Qt::CaseInsensitive) || inStr.startsWith("an ",Qt::CaseInsensitive))
         {
             key = inStr.split(" ").at(1).at(0).toUpper()/*.toAscii()*/;
         }
         else
         {
            if(inStr.length()>0)
                key = inStr.at(0).toUpper();/*.toAscii();*/
         }
     }
     else
     {
         if(inStr.startsWith("a ",Qt::CaseInsensitive) || inStr.startsWith("an ",Qt::CaseInsensitive)
                 || inStr.startsWith("the ",Qt::CaseInsensitive))
         {
             key = inStr.split(" ").at(1).at(0).toUpper()/*.toAscii()*/;
         }
         else
         {
            if(inStr.length()>0)
                key = inStr.at(0).toUpper();/*.toAscii();*/
         }
     }

    return key;
}
//} modified by yongkyun.lee 2014-06-03 

/**
 *******************************************************************
 * returns the index for the given alphabet
 *
 * @param  int : index of the alphabet
 *
 * @return None
 *
 *******************************************************************
 */
int CAppPandoraStationList::GetIndexForAlphabet(QString alphabet, bool isBT ) // modified by yongkyun.lee 2014-06-03 for :  ITS 238641
{
    LOG_LOW << "index for char: " << alphabet << LOG_ENDL;
    char key;
    char nextChar;

    //for all stations starting with digits are stored for key = '#', and the next char for this is 'a'
    if(alphabet.at(0).toLower().isDigit())
    {
        key = '#';
        nextChar = 'a';
    }
    else
    {
        key = alphabet.at(0).toLower().toAscii();
        // if key is 'z' then the there is no next char so 'z' is the nextChar
        nextChar = key < 'z' ? key + 1 : 'z';
    }
    LOG_LOW << "so char is: " << key << LOG_ENDL;
    LOG_LOW << "so next char is: " << nextChar << LOG_ENDL;
    int index = m_AlphaListIndexTable.value(key,-1);
    m_eListReqType |= PandoraListReqType::EQUICKLIST;

    LOG_LOW << "index : " << index << LOG_ENDL;

    if(index == -1)
    {
        LOG_LOW << "IsPrevItemExist  : "<<IsPrevItemExist() << LOG_ENDL;
        int i =0;
        for(i =0; i < m_StationInfo.count() ; i++){
            TSStationInfo stationInfo = m_StationInfo.at(i);
            LOG_LOW << "station name  : "<<stationInfo.StationLabel << LOG_ENDL;
            if(stationInfo.StationLabel.length() > 0){
                break;
            }
        }
         LOG_LOW << "index   : "<<i<< LOG_ENDL;
        if(i >= m_StationInfo.count()){
            i =0;
        }

        char firstChar;
        QString str = m_StationInfo.at(i).StationLabel.simplified();
        //{ modified by yongkyun.lee 2014-06-03 for : ITS 238641
        if(isBT)
        {
            if(str.startsWith("a ",Qt::CaseInsensitive) || str.startsWith("an ",Qt::CaseInsensitive) )
            {
                firstChar = str.split(" ").at(1).at(0).toLower().toAscii();
            }
            else
            {
                firstChar = str.at(0).toLower().toAscii();
            }
        }
        else
        {
            if(str.startsWith("a ",Qt::CaseInsensitive) || str.startsWith("an ",Qt::CaseInsensitive)
                || str.startsWith("the ",Qt::CaseInsensitive))
            {
                firstChar = str.split(" ").at(1).at(0).toLower().toAscii();
            }
            else
            {
                firstChar = str.at(0).toLower().toAscii();
            }
        }
        //} modified by yongkyun.lee 2014-06-03 


        LOG_LOW <<  "Org 1st char : "<<m_StationInfo.at(i).StationLabel.at(0).toLower() << LOG_ENDL;
        LOG_LOW << " simplefied firstChar: "<<firstChar << LOG_ENDL;

        if(firstChar >= key && IsPrevItemExist())
        {
            LOG_LOW << "NO Stations for this so checking prev stations" << LOG_ENDL;
            GetNextStationList(PandoraListReqType::EPREVLIST);
        }
        else
        {
            // index is -1 so we have to find out the next available index and return last index of the
            //station under the prev alpha char station.
            //i.e if No index for 'S' we have to find the index for 'T' and show the item just above it,
            //it may be start with 'R','N',etc..
            while(nextChar)
            {
                if(nextChar > 'z')
                {
                    // till 'z' no valid index found. so just retrun the last item index of the list
                    if(IsNextItemExist())
                    {
                        GetNextStationList(PandoraListReqType::ENEXTLIST);
                    }
                    else
                    {
                        index = m_nStationCount - 1;
                    }
                    break;
                }
                index = m_AlphaListIndexTable.value(nextChar,-1);
                if(index != -1)
                {
                    // so there is valid index for nextChar. then return the index of the above station.
                    index = index - 1 ;
                    break;
                }
                nextChar++;
            }
        }
    }
    return index;
}

/**
 *******************************************************************
 * Request from QML layer for getting whether next stations are exist or not
 *
 * @return bool : returns true or false
 *
 *******************************************************************
 */
bool CAppPandoraStationList::IsNextItemExist()
{
    return (m_nEndIndex < m_nStationCount - 1);
}


/**
 *******************************************************************
 * Request from QML layer for getting whether prev stations are exist or not
 *
 * @return bool : returns true or false
 *
 *******************************************************************
 */
bool CAppPandoraStationList::IsPrevItemExist()
{
    return (m_nStartIndex > 0);
}

/**
 *******************************************************************
 * Request from QML layer for getting station art image if avialable
 *
 * @return filePath if exist ortherwise NULL
 *
 *******************************************************************
 */
QString CAppPandoraStationList::IsStationArtPresent(int index)
{
    QString path("");
    if( m_StationTokens.count() > index)
    {
        path = StationArtPresent(m_StationTokens.at(index));
    }
    return path;
}
/**
 *******************************************************************
 * Request from QML layer for getting station art image if avialable
 *
 * @return filePath if exist ortherwise NULL
 *
 *******************************************************************
 */
QString CAppPandoraStationList::StationArtPresent(quint32 inTokenId)
{  
    QString filePath(STATION_ART_PATH);
    QString stationNo;
    stationNo.setNum(inTokenId);
    filePath.append(stationNo);
    filePath.append(".jpeg");
    //If the same file exists
    if(!QFile::exists(filePath))
    {
         filePath = "";
    }
//    LOG_LOW<<"IsStationArtPresent for Token : "
//                <<inTokenId  <<" ? return "<<(filePath.length()>0)<<LOG_ENDL;
    return filePath;
}

/**
 *******************************************************************
 * Slot to listen to signal from Pandora MW for receiving the
 * station sort update along with the station count found
 *
 * @param[in] inStationCount The total count of station found
 *                            after sorting
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraStationList::OnStationTokens(const quint8 inStationCount, const QList<quint32>& inTokenList)
{

    LOG_HIGH<<"CAppPandoraStationList::OnStationTokens count = "<< inStationCount<<", m_PreFetchingStations = "<<m_PreFetchingStations<<LOG_ENDL;

    if(m_VrReqToPlayShuffle)
    {
        m_VrReqToPlayShuffle = false;
        m_ShuffleStationToken = inTokenList.count() > 0 ? inTokenList.at(0) : 0 ;
        LOG_HIGH << "Play shuffle station from VR , hence return token = " << m_ShuffleStationToken <<LOG_ENDL;
        emit toastLoadingPopup();   // modified by yongkyun.lee 2014-09-01 for : Pandora 4.25 TEST     
        m_PndrInterface->RequestStationSelect(m_ShuffleStationToken);
        return;
    }

    ResetValue();
    emit stationToken(inStationCount );//{ modified by yongkyun.lee 2014-03-28 for : List update
    m_nStationCount = inStationCount ;
    m_StationTokens.clear();
    m_StationInfo.clear();
    m_StationTokens = inTokenList ;

    for(int i = 0; i < inTokenList.count() ; i++)
    {
        TSStationInfo info;
        info.nStationToken = inTokenList.at(i);
        m_StationInfo.append(info);
    }

    qint8 absoluteActiveStationIndex = m_StationTokens.indexOf(m_nActiveStationToken);

    int start = 0 , end = 0 ;

    /* //removed by jyjeon 2014-04-03 for NAQC
    if(!m_PreFetchingStations)
    {
        start = absoluteActiveStationIndex - 5;
    }

    if(start < 0)
    {
        start = 0;
    }
    end = start + 9;

    if(end >= m_nStationCount)
   */ //removed by jyjeon 2014-04-03 for NAQC
    {
        end = m_nStationCount - 1;
    }

    LOG_HIGH<<"OnStationTokens start = "<<start<<" end = "<<end
            <<" Actve Index =  "<< absoluteActiveStationIndex <<LOG_ENDL;

    //After receiving the total station count , Request for station list .
    if(inStationCount > 0)
    {
        //emit stationToken(inStationCount );//{ modified by yongkyun.lee 2014-03-28 for : List update
        m_eListReqType = PandoraListReqType::EFIRSTLIST;
        m_nStartIndex = start ;
        m_nEndIndex = end;
        GetStationListSlot(start,end);
    }   
   // LOG_SLOT<<"CAppPandoraStationList::OnStationTokens ->"<<LOG_ENDL;
}

/**
 *******************************************************************
 * Request from QML layer for getting the next n number of stations
 *
 * @param[in] inReqType : -1 for first list, 0 for next list and 1 for Prev List
 *
 * @return None
 *
 *******************************************************************
 */

void CAppPandoraStationList::GetNextStationList(int inReqType)
{
    LOG_TRACE<<"CAppPandoraStationList::GetNextStationList <-"<<LOG_ENDL;

    int startIndex = m_nStartIndex;
    int endIndex = m_nEndIndex;
    if(inReqType == PandoraListReqType::ENEXTLIST)
    {
        LOG_TRACE<<"Requested For NEXTLIST "<<LOG_ENDL;

        if(endIndex < m_nStationCount - 1)
        {
            startIndex = endIndex + 1;
            endIndex = startIndex + 9;
            if(endIndex >= m_nStationCount)
            {
                endIndex = m_nStationCount - 1;
            }           
            m_eListReqType |= inReqType;
        }
    }
    else if(inReqType == PandoraListReqType::EPREVLIST)
    {
        LOG_TRACE<<"Requested For PREVLIST "<<LOG_ENDL;
        if(startIndex > 0)
        {
            endIndex = startIndex -1;

            startIndex -= m_nMaxCount;

            if(startIndex < 0)
            {
                startIndex = 0;
            }
            m_eListReqType |= inReqType;
        }
    }

    LOG_TRACE<<"StartIndex: "<< startIndex << " EndIndex: " << endIndex <<LOG_ENDL;

    GetStationListSlot(startIndex,endIndex);

    LOG_TRACE<<"CAppPandoraStationList::GetNextStationList ->"<<LOG_ENDL;

}

/**
 *******************************************************************
 * Slot to listen to signal from Pandora MW for getting info on
 * station deletion
 *
 * @param[in] inToken   station identifier for deleted station
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraStationList::OnStationDeleted(const quint32 inToken)
{
    LOG_SLOT<<"CAppPandoraStationList::OnStationDeleted <-"<<LOG_ENDL;
    quint8 index = 0;

    if(m_nActiveStationToken == inToken)
    {
        m_nActiveStationToken = 0;
    }

    m_StationTokens.removeOne(inToken);

    while( index < m_StationInfo.count() )
    {
        if(m_StationInfo.at(index).nStationToken == inToken)
        {
            //match case found so delete from
            m_StationInfo.removeAt(index);            
            emit stationDeleted(index);
            break;
        }
     index++;
    }
    m_StationLabel.remove(inToken);
}

/**
 *******************************************************************
 * Slot to listen to signal from Pandora MW for getting info on
 * added station
 *
 * @param[in] inIndexLocation The index at which station is added
 * @param[in] inStationInfo   The station info for station which is
 *                              added
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraStationList::OnStationAdded(const quint8 inIndexLocation, const TSStationInfo& inStationInfo)
{

    LOG_SLOT<<"CAppPandoraStationList::OnStationAdded <-"<<LOG_ENDL;
    //{ added contains by wonseok.heo for ITS 243645
    if(m_StationTokens.contains(inStationInfo.nStationToken)){
        LOG_CRITICAL<<"CAppPandoraStationList::OnStationAdded <- inStationInfo ="<< inStationInfo.nStationToken <<LOG_ENDL;
        return;
    } //} added contains by wonseok.heo for ITS 243645

    TSStationInfo stationInfoList = inStationInfo;
    /** // removed by jyjeon 2013.12.09 for UX Update
    if(stationInfoList.bShared  && inStationInfo.StationLabel.length()>0)
    {
        stationInfoList.StationLabel.append("[Shared]");
    }
    // removed by jyjeon 2013.12.09 for UX Update **/
    int pos = m_StationInfo.count();
    if( m_StationInfo.count() > inIndexLocation )
    {
        //Station count is more than requested position .
        m_StationInfo.insert(inIndexLocation , stationInfoList /*inStationInfo*/);
        m_StationTokens.insert(inIndexLocation,stationInfoList.nStationToken);
        pos = inIndexLocation;
    }
    else
    {
           //station count is less or equal as requested position .
           m_StationInfo.append(stationInfoList/*inStationInfo*/);
           m_StationTokens.append(stationInfoList.nStationToken);
    }
    emit stationInserted(pos , stationInfoList.StationLabel/*inStationInfo.StationLabel*/);

    LOG_SLOT<<"CAppPandoraStationList::OnStationAdded ->"<<LOG_ENDL;

}

/**
 *******************************************************************
 * Slot to listen to signal from Pandora MW for getting info on
 * station sort order
 *
 * @param[in] inType The station sort order
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraStationList::OnStationSortOrder(const ESortType inType)
{
    LOG_INFO<<"CAppPandoraStationList::OnStationSortOrder type-> "<<inType<<LOG_ENDL;

    m_eSortType = inType;

    m_eCurrentRequest =  REQ_TYPE_SORT;

    if(m_eSortType != SORT_TYPE_NONE)
    {
        if(m_nIs1stTimeLaunch)
        {
            m_nIs1stTimeLaunch = 0;
            if(m_eSortType != SORT_TYPE_ALPHA)
            {
                LOG_INFO<<"Sort type changed. So request station sort "<<LOG_ENDL;
                HandleIfError(m_PndrInterface->RequestStationSort(SORT_TYPE_ALPHA));
            }
            else
            {
                LOG_INFO<<"Sort type is same. So request station count "<<LOG_ENDL;
                HandleIfError(m_PndrInterface->RequestStationCount());
            }

        }
        else{
            LOG_INFO<<"Sort type is same. So request station count "<<LOG_ENDL;
            HandleIfError(m_PndrInterface->RequestStationCount());
        }

    }
}

/**
*******************************************************************
* Slot to listen to signal from Pandora MW for getting info on
* station update
*
* @param[in] inStationToken   station identifier for new station
*
* @return  None
*
*******************************************************************
*/
void CAppPandoraStationList::OnStationUpdated(const quint32 inStationToken)
{
   LOG_INFO<<"CAppPandoraStationList::OnStationUpdated station token :"<< inStationToken<<LOG_ENDL;
   if(inStationToken > 0){
       m_nActiveStationToken = inStationToken;
       m_nRelativeActiveStationIndex = GetActiveStationIndex();
       emit activeStationToken(m_nActiveStationToken);
   }
}


/**
 *******************************************************************
 * Handle the error
 *
 * @param[in] inPndrError error code
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraStationList::HandleIfError(const EPndrError inPndrError)
{
    LOG_INFO<<"CAppPandoraStationList::HandleIfError <-"<<LOG_ENDL;
    if(E_SUCCESS != inPndrError)
    {
        LOG_CRITICAL<<"Error code :"<<inPndrError<<LOG_ENDL;
        m_eCurrentRequest = REQ_TYPE_NONE ;

        switch(inPndrError)
        {
            case E_INVALID_STATE:
            case E_INVALID_ARGUMENT:
            {
                // unexpected error
                emit handleError(inPndrError);
            }
            break;
            default:
            {
                LOG_CRITICAL<<"Default :"<<inPndrError<<LOG_ENDL;
                break;
            }
        }
    }
    LOG_INFO<<"CAppPandoraStationList::HandleIfError ->"<<LOG_ENDL;
}

/**
 *******************************************************************
 * Request from QML Layer to play shuffle .
 *
 * @param None
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraStationList::PlayShuffle()
{
    LOG_INFO<<"CAppPandoraStationList::PlayShuffle <-"<<LOG_ENDL;    

    if(m_PreFetchingStations){
        LOG_INFO<<"CAppPandoraStationList::PlayShuffle m_PreFetching Stations In progress , Wait "<<LOG_ENDL;
        return;
    }
    if(m_StationTokens.count()>0 )
    {
        // Assuming 1st station is shuffle , Will save the station token of shuffle.
        if(m_nActiveStationToken != m_StationTokens.at(0))
        {
            emit toastLoadingPopup();   // modified by yongkyun.lee 2014-09-01 for : Pandora 4.25 TEST     
            m_PndrInterface->RequestStationSelect(m_StationTokens.at(0));
        }
    }

    LOG_INFO<<"CAppPandoraStationList::PlayShuffle ->"<<LOG_ENDL;
}


/**
 *******************************************************************
 * This slot is invoked when any Station Art is received
 * @param   inStationArt The station Art Object
 *
 * @return  void
 *
 *******************************************************************
 */
void CAppPandoraStationList::OnStationArtReceived(quint32 inToken)
{

    QString stationNo("");
    stationNo.setNum(inToken);
    QString filePath(STATION_ART_PATH);
    filePath.append(stationNo);
    filePath.append(".jpeg");

    if(m_nActiveStationToken != inToken)
    {
        int index = m_StationTokens.indexOf(inToken ,0);
        if(index >= 0){
            emit stationArtReceived(index,filePath);
        }
        else
        {
            LOG_CRITICAL<<"Error CAppPandoraStationList st art token not found : "<<inToken<<LOG_ENDL;
        }
    }
    else
    {
        emit currStArt(filePath);
    }
 
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
void CAppPandoraStationList::CancelStationArtRequest()
{
    LOG_TRACE<<"CAppPandoraStationList::CancelStationArtRequest Entry"<<LOG_ENDL;
    if(m_PndrInterface)
        m_PndrInterface->CancelStationArtRequest();

    LOG_TRACE<<"CAppPandoraStationList::CancelStationArtRequest Exit"<<LOG_ENDL;
}


/**
*******************************************************************
* SaveState : Save state on device changed
* @param inDevice , Current device
* @return None
*
*******************************************************************
*/
void CAppPandoraStationList::SaveState(DeviceInfo* inDevice)
{
    inDevice->SaveStation(m_nActiveStationToken , m_eSortType );
}

/**
*******************************************************************
* RetriveState : Retrive state of device ,on device changed from cache
* @param inDevice , Current device
* @return None
*
*******************************************************************
*/
void CAppPandoraStationList::RetriveState(DeviceInfo* inDevice)
{
    LOG_HIGH<<"RetriveState() m_nActiveStationToken : "<<m_nActiveStationToken<<LOG_ENDL;
    inDevice->StationInfo(m_nActiveStationToken , m_eSortType );
}


//{ modified by yongkyun.lee 2014-06-18 for : ITS 240645
void CAppPandoraStationList::GetStationCount()
{
    m_PndrInterface->RequestStationCount();
}
//} modified by yongkyun.lee 2014-06-18 

/**
 *******************************************************************
 * Get Station name from the tune index
 * @param isForward , True : Forward , False : Reverse
 *
 * @return  void
 *
 *******************************************************************
 */
void CAppPandoraStationList::GetStationName(bool isForward)
{
    LOG_HIGH <<"GetStationName() -> m_PreFetchingStations: "<< m_PreFetchingStations << " m_StationLabel.count(): " << m_StationLabel.count() << " m_StationTokens.count(): " << m_StationTokens.count() << LOG_ENDL;
    if(m_PreFetchingStations || (m_StationLabel.count() == 0 && m_StationTokens.count() == 0))
    {
        if(m_PreFetchingStations){
            LOG_CRITICAL<<"CAppPandoraStationList::GetStationName Pre fetching in progress , Return"<<LOG_ENDL;
            return;
        }
        else{
            LOG_HIGH<<"CAppPandoraStationList::GetStationName m_nActiveStationToken : " << m_nActiveStationToken
                        << " m_eSortType : " << m_eSortType<<LOG_ENDL;
            if(m_nActiveStationToken > 0 /*&& m_eSortType != SORT_TYPE_NONE*/ && !m_bIsTuningInProgress){ //modified by jyjeon 2014-03-28
                m_bIsTuningInProgress = true;
                m_nIs1stTimeLaunch = 0;
                HandleIfError(m_PndrInterface->RequestStationCount());
            }
        }

        return;
    }

    if( (!m_bIsTuningInProgress || m_nTuningIndex == -1 )&& m_StationTokens.count() > 0)
    {
        m_nTuningIndex = m_StationTokens.indexOf(m_nActiveStationToken);
    }

    m_bIsTuningInProgress = true;


    int counter = m_nTuningIndex ;
    if(isForward)
        counter++;
    else
        counter--;

    if(counter < 0 )
        counter = m_StationTokens.count() - 1 ;
    else if(counter >= m_StationTokens.count() )
        counter = 0;

    QString value;
    quint32 token;

    LOG_INFO<<"StationTokens : " << m_StationTokens.count() << " StationLabels: " << m_StationLabel.count()
            <<" Req TuningIndex : "<<counter <<LOG_ENDL;

    if( m_StationLabel.count() == 0 && m_StationTokens.count() == 0 )
    {
        LOG_HIGH<<"No Station presents " <<LOG_ENDL;
        //m_nTuningIndex = GetActiveStationIndex();
        //m_nTuningIndex = -1;
        m_RequestInprogress = false;
        m_bIsTuningInProgress = false;
        m_nTuningIndex = -1;
        //GetFirstStationListSlot(10 , m_nActiveStationToken);
        emit noStationPresent();
    }
    else if( m_StationLabel.count() > 0 && m_StationTokens.count() > m_StationLabel.count() )
    {
        LOG_HIGH<<"Some stations are present"<<LOG_ENDL;

        if(m_StationTokens.count() > counter)
        {
            token = m_StationTokens.at(counter);
            if(m_StationLabel.contains(token))
            {
                value = m_StationLabel.value(token);
                m_nTuningIndex = counter ;
                emit tunedStation(token , value , (m_nTuningIndex == m_StationTokens.indexOf(m_nActiveStationToken)));
            }
            else if(!m_RequestInprogress){

                m_RequestInprogress = true;

                int start = 0;
                int end = 0;

                if(isForward)
                {
                    start = counter ;
                    end = start + 9;

                    if(end >= m_StationTokens.count())
                        end = m_StationTokens.count() -1;

//                     token = m_StationTokens.at(end);

//                    if(!m_StationLabel.contains(token) )
//                    {
                        GetStationListSlot(start ,end);
//                    }
                    if(start <= end)
                        m_PndrInterface->RequestStationList(start,end);
                }
                else
                {
                    end = counter ;
                    start = end - 9;

                    if(start < 0)
                        start = 0;

//                     token = m_StationTokens.at(end);

//                    if(!m_StationLabel.contains(token) )
//                    {
//                        GetStationListSlot(start ,end);
//                    }
                    if(start <= end)
                        m_PndrInterface->RequestStationList(start,end);
                }

            }
        }
    }
    else if(m_StationLabel.count() > 0 && m_StationLabel.count() == m_StationTokens.count())
    {
         LOG_INFO<<"All Stations are present"<<LOG_ENDL;

        if(m_StationTokens.count() > counter)
        {
            token = m_StationTokens.at(counter);
            if(m_StationLabel.contains(token))
                value = m_StationLabel.value(token);

            m_nTuningIndex = counter;
            emit tunedStation(token , value , (m_nTuningIndex == m_StationTokens.indexOf(m_nActiveStationToken)));
        }
    }

}
/**
 *******************************************************************
 * Update OSD on Station tunned , If app is in BG
 * @param void
 *
 * @return  void
 *
 *******************************************************************
 */
void CAppPandoraStationList::UpdateTunnedStation_OSD()
{
    if(m_StationTokens.count() > m_nTuningIndex && (m_nTuningIndex >= 0) )//{ modified by yongkyun.lee 2014-03-24 for : ITS 230769
    {
        quint32 token = m_StationTokens.at(m_nTuningIndex);
        QString value = "";
        if(m_StationLabel.contains(token))
        {
            value = m_StationLabel.value(token);
        }
        emit tunedStation(token , value , true);
    }
}

/**
 *******************************************************************
 * Tune off , Timer off
 * @param void
 *
 * @return  void
 *
 *******************************************************************
 */
void CAppPandoraStationList::TuneOff()
{
    m_bIsTuningInProgress = false;
    m_RequestInprogress = false;

    if(m_StationLabel.count() > 0)
    {
        QString value;
        if(m_StationLabel.contains(m_nActiveStationToken))
        {
            value = m_StationLabel.value(m_nActiveStationToken);
        }
        emit tunedStation(m_nActiveStationToken , value , true );
    }
}

/**
 *******************************************************************
 * Tune select on perticular station , Will change to that station
 * @param void
 *
 * @return  void
 *
 *******************************************************************
 */
void CAppPandoraStationList::TuneSelect()
{
    m_bIsTuningInProgress = false;
    m_RequestInprogress = false;
    UpdateTunnedStation_OSD();

    if( m_StationTokens.count() > m_nTuningIndex && m_nTuningIndex >= 0)
    {
        if(m_nActiveStationToken != m_StationTokens.at(m_nTuningIndex))
        {
            emit toastLoadingPopup();    // modified by yongkyun.lee 2014-09-01 for : Pandora 4.25 TEST    
            HandleIfError( m_PndrInterface->RequestStationSelect(m_StationTokens.at(m_nTuningIndex))); //modified by jyjeon 2014-03-21 for ITS 228830
        }
    }
}


void CAppPandoraStationList::OnCurrentStationInfo(const TSTrackInfo& /*inTrackInfo*/ )
{
    if( m_StationTokens.count() <= 0 && !m_PreFetchingStations )
    {
        LOG_CRITICAL<<"CAppPandoraStationList::OnTrackInfoReceived Retrieve station Names only"<<LOG_ENDL;

        // show loading pop up on UI and retrieve station names
        //{ modified by cheolhwan 2014-04-17. To fix the unlimited receiving station. (ITS 238629)
        //m_PreFetchingStations =true;
        setPreFetchingStations(true);
        //} modified by cheolhwan 2014-04-17. To fix the unlimited receiving station.
        station_time.start(); // added by esjang 2014.01.08 for check time
        emit preFetchingStart();
        GetFirstStationListSlot(10 , 0);
    }
}

void CAppPandoraStationList::FetchStationArt()
{
    m_PndrInterface->FetchStationArt();
}

//{ modified by yongkyun.lee 2014-03-28 for : List update
QString CAppPandoraStationList::StationName(int index)
{
    QString str("");
    if(m_StationInfo.count() > index)
        str = m_StationInfo.at(index).StationLabel;
    //LOG_INFO<<"CAppPandoraStationList::StationName : :index =" << index<< ", str=[" << str<< "]"<<LOG_ENDL;
    return str;
}     

bool CAppPandoraStationList::IsSharedStation(int index)
{
    bool bShared = false;
    if(m_StationInfo.count() > index)
        bShared = m_StationInfo.at(index).bShared;
    //LOG_INFO<<"CAppPandoraStationList::IsSharedStation :index =" << index<< ", bShared=[" << bShared<< "]"<<LOG_ENDL;
    return bShared;
}
//} modified by yongkyun.lee 2014-03-28 for : List update

QString CAppPandoraStationList::CurrentStationName()
{

    QString value;
    if(m_StationLabel.count() > 0)
    {
        if(m_StationLabel.contains(m_nActiveStationToken))
        {
            value = m_StationLabel.value(m_nActiveStationToken);
        }
    }
    LOG_HIGH<<"Error recovery from null station name initially : "<<value<<LOG_ENDL;
    return value;
}

//{ added by cheolhwan 2014-04-17. To fix the unlimited receiving station. (ITS 238629)
void CAppPandoraStationList::setPreFetchingStations(bool inPreFetchingStations)
{
    LOG_HIGH<<"setPreFetchingStations() <- m_PreFetchingStations : "<<inPreFetchingStations<<LOG_ENDL;
    m_PreFetchingStations = inPreFetchingStations;
	if(m_PndrInterface)
    	m_PndrInterface->setPreFetchingStations(m_PreFetchingStations);
}
//} added by cheolhwan 2014-04-17.  To fix the unlimited receiving station.

//End Of File
