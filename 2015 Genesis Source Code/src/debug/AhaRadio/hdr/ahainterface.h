#ifndef AHAINTERFACE_H
#define AHAINTERFACE_H

#include <QVector>
#include <DHAVN_IPodAppController_CommonVars.h>
#include <DHAVN_IPodAppController_Def.h>
#include "ahastatehandler.h"
#include "ahapredefinedvalues.h"

class CAhaInterface : public QObject
{
    Q_OBJECT
public:
    CAhaInterface();
    ~CAhaInterface();

    int Init();
    int DeInit();       //deinitialize variables created during init
    EAhaError Connect(EDeviceType inDeviceType);      //Ask state handler to connect
    int Disconnect();   //Ask state handler to disconnect

    void pause();
    void play();
    void playPause();
    void next();
    void back();
    void rewind15();
    void forward30();
    void likeButtonClicked();
    void dislikeButtonClicked();
    void logCall();
    void logNavigate();

    void selectPresetsToPlay(int nIndex);
    void selectPresetsToPlayID(uint64_t appID);//han

    void selectContentToPlay(int nIndex);
    void selectContentToPlayID(uint64_t appID);//han

    void showCurrentLikeDislikeIcon();

    void queryingPresetStations();
    void queryingContentList();

    bool  queryPhoneNumber(QString &strPhoneNumber);

    bool queryAhaLBSValue(QString &strName,
                          QString &strStateName,
                          QString &strStreet,
                          QString &strCity,
                          QString &strZipCode,
                          QString &strPhoneNumber,
                          double &dbLon, double &dbLat);


    bool reqSendDataToBTPhone(QString strPhoneNumber);
    bool reqSendContentInfoToNavigation(QString strName,
                                        QString strStateName,
                                        QString strStreet,
                                        QString strCity,
                                        QString strZipCode,
                                        QString strPhoneNumber,
                                        double dbLon, double dbLat);

    CAhaStateHandler& GetSignalProvider();

    //member variable
    CAhaStateHandler m_AhaStateHandler;

signals:

    void contentListLoaded(QVector <AhaContentBriefInfo_T> &qVectContentListInfo);
    void stationListLoaded(QVector <AhaStationBriefInfo_T> &qVectStationListInfo);
public:
    USE_LOG_UTIL
};


#endif // AHAINTERFACE_H
