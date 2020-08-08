#include "DHAVN_AppPandoraSearch.h"
#include <QStringList>
#include <QListIterator>
#include <DHAVN_PndrStateHandler.h>
#include <QDebug>

/**
 *******************************************************************
 * Constructor for CAppPandoraSearch class.
 * It will just initialize all the parameters required by the class
 *
 * @param[in]   inPndrInterface Pandora MW interface reference
 *
 * @return  None
 *
 *******************************************************************
 */
CAppPandoraSearch::CAppPandoraSearch(/*CPndrInterface&  inPndrInterface*/):
    m_PndrInterface(NULL)
  , m_eCurrentRequest(REQ_TYPE_NONE)
  ,m_pSignals(NULL)
  ,m_SearchRequestCnt(0) // added by cheolhwan 2014-03-08. To modified the screen update error in SearchView during the continuous inputting.
{
    LOG_INIT_V2("Pandora");
}


void CAppPandoraSearch::SetInterface(CPndrInterface*  inPndrInterface)
{
    LOG_CRITICAL<<"CAppPandoraSearch::SetInterface"<<LOG_ENDL;
    m_PndrInterface = NULL;
    m_PndrInterface = inPndrInterface;
    if(m_pSignals){
        disconnect(m_pSignals,0,this,0);
        m_pSignals=NULL;
    }
    if(inPndrInterface == NULL)
    {        
        LOG_CRITICAL<<"Interface is NULL,so return"<<LOG_ENDL;
        return;
    }
    m_pSignals = &(m_PndrInterface->GetSignalProvider());

    Qt::ConnectionType ConnType = Qt::QueuedConnection;

    connect(m_pSignals,SIGNAL(SearchComplete(const QList<TSSearchInfo>& )),
        this,SLOT(OnSearchComplete(const QList<TSSearchInfo>& )));
}

/**
 *******************************************************************
 * Destructor for CAppPandoraSearch class.
 *
 * @param None
 *
 * @return  None
 *
 *******************************************************************
 */
CAppPandoraSearch::~CAppPandoraSearch()
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
void CAppPandoraSearch::ClearCache()
{
    LOG_TRACE<<"CAppPandoraSearch::ClearCache() <-"<<LOG_ENDL;
    if(m_SearchInfoList.count() >0)
    {
        m_SearchInfoList.clear();
    }
    m_eCurrentRequest = REQ_TYPE_NONE;
    if(m_sSearchString.count() > 0)
    {
        m_sSearchString.clear();
    }
    LOG_TRACE<<"CAppPandoraSearch::ClearCache() ->"<<LOG_ENDL;
}

/**
 *******************************************************************
 * Request for Search auto complete given the word to be searched
 *
 * @param[in] inSearchWord: The word to be searched
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraSearch::SearchAutoComplete(const QString& inSearhWord)
{
    LOG_TRACE<<"CAppPandoraSearch::SearchAutoComplete <-"<<LOG_ENDL;

    m_eCurrentRequest =  REQ_TYPE_SEARCH_AUTO_COMPLETE;
    m_SearchRequestCnt++;  // added by cheolhwan 2014-03-08. To modified the screen update error in SearchView during the continuous inputting.
    HandleIfError(m_PndrInterface->RequestSearchAutoComplete(inSearhWord));

    LOG_TRACE<<"CAppPandoraSearch::SearchAutoComplete ->"<<LOG_ENDL;

}


/**
 *******************************************************************
 * Request for extended Search given the word to be searched
 *
 * @param[in] inSearchWord: The word to be searched
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraSearch::SearchExtended(const QString& inSearhWord)
{
    LOG_TRACE<<"CAppPandoraSearch::SearchExtended <-"<<LOG_ENDL;

    m_eCurrentRequest =  REQ_TYPE_SEARCH;
    HandleIfError(m_PndrInterface->RequestSearch(inSearhWord));

    LOG_TRACE<<"CAppPandoraSearch::SearchExtended ->"<<LOG_ENDL;

}

/**
 *******************************************************************
 * Request to select a perticular serch
 *
 * @param[in]    inIndex   Index id from the list
 *
 * @return    None
 *******************************************************************
 */
void CAppPandoraSearch::SearchSelect(const quint32 inIndex)
{
    LOG_TRACE<<"CAppPandoraSearch::StationSelect <-"<<LOG_ENDL;

    m_eCurrentRequest =  REQ_TYPE_SEARCH_SELECT;

    quint32 inSearchInfolistCount =  m_SearchInfoList.count();
    LOG_INFO<< "inIndex : "<<inIndex <<LOG_ENDL;
    if(inSearchInfolistCount >= inIndex)
    {
         quint32 tokenId = m_SearchInfoList.at(inIndex).nMusicToken;
         HandleIfError(m_PndrInterface->RequestSearchSelect(tokenId));
    }
    else
    {
        LOG_CRITICAL<<"array index out of bound "<<LOG_ENDL;
        emit handleError(E_SEARCH_SELECT_FAILED);
    }

    LOG_TRACE<<"CAppPandoraSearch::StationSelect ->"<<LOG_ENDL;
}

/**
 *******************************************************************
 * Listening slot for signal from Pandora MW for search
 * complete result
 *
 * @param[in]   inSearchInfoList    List of TSSearchInfo
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraSearch::OnSearchComplete(const QList<TSSearchInfo>& inSearchInfoList)
{
    //LOG_SLOT<<"CAppPandoraSearch::OnSearchComplete <-"<<LOG_ENDL;

    //cacheing search list data.
    m_SearchInfoList = inSearchInfoList;

    QStringList searchInfo;

    for(int counter = 0; counter < inSearchInfoList.count(); counter++)
    {
        searchInfo << inSearchInfoList.at(counter).Label;
    }

    LOG_SLOT<<"CAppPandoraSearch::OnSearchComplete Req ID(m_eCurrentRequest) = "<< \
              m_eCurrentRequest<<" Search count = "<<searchInfo.count()<<LOG_ENDL;
    switch(m_eCurrentRequest)
    {
        case REQ_TYPE_SEARCH: /** Extended Search request */
        {
            emit extendedSearchCompleted(searchInfo);
        }
        break;
        case REQ_TYPE_SEARCH_AUTO_COMPLETE:/** Auto complete search request */
        {
            if(m_SearchRequestCnt > 0) m_SearchRequestCnt--;  // added by cheolhwan 2014-03-08. To modified the screen update error in SearchView during the continuous inputting.
            emit autoSearchCompleted(searchInfo);
        }
        break;
        default:
        {
            LOG_CRITICAL<<"Default request id(m_eCurrentRequest) :m_SearchRequestCnt:"<<m_SearchRequestCnt<<LOG_ENDL;            
            //{ added by cheolhwan 2014-03-08. To modified the screen update error in SearchView during the continuous inputting.
            if(m_SearchRequestCnt > 0)
            {
                LOG_CRITICAL<<"request id is error : m_eCurrentRequest:"<<m_eCurrentRequest<<LOG_ENDL;
                m_SearchRequestCnt--;
                if(searchInfo.count() > 0)
                    emit autoSearchCompleted(searchInfo);
            }
            //} added by cheolhwan 2014-03-08. To modified the screen update error in SearchView during the continuous inputting.
            break;
        }
    }

    m_eCurrentRequest = REQ_TYPE_NONE;

    //LOG_SLOT<<"CAppPandoraSearch::OnSearchComplete ->"<<LOG_ENDL;
}


/**
 *******************************************************************
 * The result of request from Pandora MW
 * No network related error should be handled here
 *
 * @param[in] inRequestType The request for which the reply is
 *                          received
 * @param[in] inPndrError   The result of the request
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraSearch::OnRequestCompleted(const ERequestType inRequestType, const EPndrError inPndrError)
{
//    LOG_SLOT<<"CAppPandoraSearch::OnRequestCompleted <-"<<LOG_ENDL;

    LOG_INFO<<"CAppPandoraSearch::OnRequestCompleted Req = "<<inRequestType<<" status = "<<inPndrError << LOG_ENDL;

//    LOG_SLOT<<"CAppPandoraSearch::OnRequestCompleted ->"<<LOG_ENDL;
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
void CAppPandoraSearch::HandleIfError(const EPndrError inPndrError)
{
    LOG_INFO<<"CAppPandoraSearch::HandleIfError <-"<<LOG_ENDL;
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
    LOG_INFO<<"CAppPandoraSearch::HandleIfError ->"<<LOG_ENDL;
}

/*****************************************************************************
 *************************** Q_PROPERTY setters ******************************
 *****************************************************************************/

void CAppPandoraSearch::setSearchString( QString sSearchString )
{
   m_sSearchString = sSearchString;
   emit updateSearchString();
}
