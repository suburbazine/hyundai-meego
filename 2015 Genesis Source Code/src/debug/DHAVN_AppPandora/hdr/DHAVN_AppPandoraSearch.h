/*
 ******************************************************************
 *        COPYRIGHT © 2011, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     : Station Search                                    *
 * PROGRAMMER : Vandana Ahuja                                     *
 * DATE       : 21/12/2011                                        *
 * VERSION    : 0.1                                               *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module is responsible to provide search related data
 *  This is not complete as discussion on search behavior is
 *  still going on
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  21/12/2011  0.1 Vandana Ahuja   header file created
 *
 ******************************************************************
 */

#ifndef DHAVN_APPPANDORASEARCH_H
#define DHAVN_APPPANDORASEARCH_H

#include <DHAVN_Pndr_Defs.h>
#include <DHAVN_PndrInterface.h>
#include <DHAVN_LogUtil.h>
#include <QList>

class CPndrStateHandler ;
class CAppPandoraSearch : public QObject
{
    Q_OBJECT
public:
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
    CAppPandoraSearch(/*CPndrInterface&  inPndrInterface*/);

    void SetInterface(CPndrInterface*  m_PndrInterface);


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
    ~CAppPandoraSearch();


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
    Q_INVOKABLE void SearchAutoComplete(const QString& inSearhWord);


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
    Q_INVOKABLE void SearchExtended(const QString& inSearhWord);

    /**
     *******************************************************************
     * Request to select a perticular search
     *
     * @param[in]    inIndex    Index id
     *
     * @return    None
     *******************************************************************
     */
    Q_INVOKABLE void SearchSelect(const quint32 inIndex);

    /**
     *******************************************************************
     * Q_PROPERTIES for search string from keboard input
     *******************************************************************
     */
    Q_PROPERTY( QString sSearchString READ getSearchString WRITE setSearchString NOTIFY updateSearchString )

    /**
     *******************************************************************
     * getSearchString
     *
     * @param[in] None
     *
     * @return QString
     *
     *******************************************************************
     */
   QString getSearchString(){ return m_sSearchString; }

   /**
    *******************************************************************
    * setSearchString
    *
    * @param[in] sSearchString search string
    *
    * @return None
    *
    *******************************************************************
    */
   void setSearchString( QString sSearchString );

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
    void ClearCache();

Q_SIGNALS:

  /** Q_RPOPERTIES signals */
   void updateSearchString();

private:
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
    void HandleIfError(const EPndrError inPndrError);

signals:
    /**
     *******************************************************************
     * Signal to QML updating the result of auto search
     *
     * @param[in] inSearchResultList: The list of found results
     *
     * @return  None
     *
     *******************************************************************
     */
    void autoSearchCompleted(const QStringList& inSearchResultList);


    /**
     *******************************************************************
     * Signal to QML updating the result of extended search
     *
     * @param[in] inSearchResultList: The list of found results
     *
     * @return  None
     *
     *******************************************************************
     */
    void extendedSearchCompleted(const QStringList& inSearchResultList);

    /**
     *******************************************************************
     * Signal to QML updating the unexpected behaviour
     *
     * @param[in] inError: The error code
     *
     * @return  None
     *
     *******************************************************************
     */
    void handleError(const EPndrError inError);


public slots:
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
    void OnSearchComplete(const QList<TSSearchInfo>& inSearchInfoList);


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
    void OnRequestCompleted(const ERequestType inRequestType, const EPndrError inPndrError);

private:
    /** Pandora MW interface reference */
    CPndrInterface* m_PndrInterface;

    /** Current outstanding request*/
    ERequestType m_eCurrentRequest;

    /** Search info list caching */
    QList<TSSearchInfo> m_SearchInfoList;

    /** Signal handler */
    CPndrStateHandler* m_pSignals ;

    /** Search Request Count **/
    quint32 m_SearchRequestCnt; // added by cheolhwan 2014-03-08. To modified the screen update error in SearchView during the continuous inputting.
	
    /** keboard entered string for search */
    QString m_sSearchString;
    /** Logger instance */
    USE_LOG_UTIL
};

#endif // DHAVN_APPPANDORASEARCH_H
