/*
 ******************************************************************
 *      COPYRIGHT  2012, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :                                                   *
 * PROGRAMMER : Radhakrushna Mohanty                              *
 * DATE       : 21/12/2011                                        *
 * VERSION    : 0.1                                               *
 *----------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module is responsible for displaying OSD message
 *      * Check pandora connected status
 *      * Check the app running status
 *      * Check av mode change status
 *      * If all conditions satisfies , then display the OSD .
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  26/05/2012 0.1 Radhakrushna Mohanty   header file created
 *
 ******************************************************************
 */

#ifndef DHAVN_APPPANDORA_NOTIFIER_H
#define DHAVN_APPPANDORA_NOTIFIER_H

#define PNDR_ENCODE_OSD_FORMAT_RULE2(strMessage) \
            (strMessage).replace("^", "\\^") \

#define PNDR_ENCODE_OSD_FORMAT_RULE1(strMessage) \
            (strMessage).replace("@", "\\@") \


#include <QObject>
#include <DHAVN_LogUtil.h>
#include <DHAVN_QCANController_ApplnTxRx.h>
#include <QDBusInterface>
#include <DHAVN_AppFramework_AppEngineQML_Def.h>
#include <QDeclarativeView>


class CAppPandoraBase;
class StatusAdapter;
class CExternalNotifier: public QObject
{
    Q_OBJECT
    USE_LOG_UTIL

public:

    enum HandleOSDEvent
    {
        ETuneKeyPress,
        ETrackInfo,
        ESeekUp,
        EThumbsUp, //Reserved for  thumbs up ,  not decided
        EThumbsDown, //Reserved for  thumbs down , not decided
        ESkipLimitReached,
        EDeviceDisconnected,
        ESeekDown, //added by esjang 2013.04.02 for ISV # 77996
        ETuneKeyDial,
        ETuneCurrStation,
        ECallingState, //added by esjang 2013.08.21 for BT phone call
        Econnecting,      //{ modified by yongkyun.lee 2014-09-18 for : ITS 248606
        EDisconnect,     // added by esjang 2013.10.26 for ITS # 195120
        ENetworkErrorOSD, //added by esjang 2013.10.26 for ITS # 195120
        ENoTrackInfo, // added by jyjeon 2013.11.20 for OSD Update
        ECannotBeManipulated, //added by jyjeon 2013.12.18 for ITS 216143
        EStationDoesNotExist, //modified by yongkyun.lee 2014-07-17 for : ITS 243240
        EFailedToConnect,
        ENoNetwork //added by jyjeon 2014-03-27 for NAQC Request
    };

    /**
     *******************************************************************
     * Constructor
     *
     * @param[in] inEngine , Base class pointer
     *
     *******************************************************************
     */
    CExternalNotifier(CAppPandoraBase* inEngineMain );

    /**
     *******************************************************************
     * Destructor
     *
     *******************************************************************
     */
    ~CExternalNotifier();

    /**
     *******************************************************************
     * Device disconnected , update OSD.
     *
     * @param[in] inOSDEvent , type of osd event
     *
     * @return  None
     *
     *******************************************************************
     */
    void DisplayOSD(CExternalNotifier::HandleOSDEvent inOSDEvent ,  QString info = "");

    /**
     *******************************************************************
     * Display AV Off on OSD
     *
     * @param[in] None
     *
     * @return  None
     *
     *******************************************************************
     */
    void DisplayOSDAVOff();

    /**
     *******************************************************************
     * Clear OSD
     *
     * @param[in] None
     *
     * @return  None
     *
     *******************************************************************
     */
    void ClearOSD();

    /**
     *******************************************************************
     * Clear AV OSD
     *
     * @param[in] None
     *
     * @return  None
     *
     *******************************************************************
     */
    void ClearAVOSD();


    /**
     *******************************************************************
     * Clear Cluster
     *
     * @param[in] None
     *
     * @return  None
     *
     *******************************************************************
     */
    Q_INVOKABLE void ClearCluster();

    /**
     *******************************************************************
     * SendNullToCluster Cluster
     *
     * @param[in] None
     *
     * @return  None
     *
     *******************************************************************
     */
    Q_INVOKABLE void SendNullToCluster();
    
    
    /**
     *******************************************************************
     * Device disconnected update OSD.
     *
     * @param[in] None
     *
     * @return  None
     *
     *******************************************************************
     */
    void DeviceDisconnected();

    void SetSkiped(bool skiped){ isSkiped = skiped; }


    void SendClusterData_Progress(int position);
    Q_INVOKABLE void UpdateTrackTitle(QString title); // modified by wonseok.heo for TP Cluster 2015.01.29

    /**
     *******************************************************************
     * Forcee update , irrespective of FG or any mode.
     *
     * @param[in] None
     *
     * @return  None
     *
     *******************************************************************
     */
    Q_INVOKABLE void UpdateOSDOnCallingState();

signals:
    void Can_data_send(const SQCanFrame &canframe);
    public slots:
    void OnTunedStation(quint32 /*token*/, QString inStation , bool isCurr);

private:
    void SendClusterData(quint64 data);
    void SendClusterData_OpStateOnly();
    int GetClusterState(); 
    bool sendCanTpMessage(quint16 id, QString data);

    // EngineMain links:
    CAppPandoraBase *m_pEngineMain;
    // Cluster interface:
    CQCANController_ApplnTxRx* m_pQCanController;
    QDBusInterface* m_pCanTPInterface;
    QDeclarativeView* m_pOSDDisplay;
    bool isSkiped;


};

#endif // DHAVN_APPPANDORA_NOTIFIER_H
