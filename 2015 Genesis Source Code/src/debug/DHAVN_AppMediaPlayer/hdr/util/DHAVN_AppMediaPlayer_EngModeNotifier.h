#ifndef DHAVN_APPMEDIAPLAYER_ENGMODENOTIFIER_H
#define DHAVN_APPMEDIAPLAYER_ENGMODENOTIFIER_H

//#include <QImage>
//#include <QTimer>
#include <QDBusInterface>
#include <QDBusConnectionInterface>
#include <QDBusServiceWatcher>
#include "deckengineermode.h"
#include "usbengineermode.h"
//#include <QSharedMemory>

/**
* List of possible country variants.
*/
enum EMPCountryVariant
{
    eMPCVInvalid      = -1,
    eMPCVKorea        =  0,
    eMPCVNorthAmerica =  1,
    eMPCVChina        =  2,
    eMPCVGeneral      =  3,
    eMPCVMiddleEast   =  4,
    eMPCVEuropa       =  5,
    eMPCVCanada       =  6,
    eMPCVRussia       =  7,
    eMPCVInvalidMax   =  8
};

class EngModeNotifier: public QObject
{
    Q_OBJECT
public:
    explicit EngModeNotifier(QObject *parent = 0);

    ~EngModeNotifier();

    DeckEngineerMode* GetDeckEngineerMode();
    USBEngineerMode* GetUSBEngineerMode();

    void SendUSBDiagResult(bool bFront, bool bFail);

public slots:
    void OnAsk_DeckInfo();
    void OnAsk_AudioPathInfo();
    void OnSet_AudioPathInfo(uchar in0);
    void OnServiceRegistered(QString service);
    void HandleCommErrorWithDeckChanged(int aErrorCode);
    // { added by cychoi 2013.07.26 for ITS 181382 & 158365 Initialize DVD Settings
    void ResetData(EMPCountryVariant eCountryVariant);
    void FactoryReset(EMPCountryVariant eCountryVariant);
    // } added by cychoi 2013.07.26

private:
   QDBusInterface *m_pInterface;
   QDBusServiceWatcher *m_pWatcher;
   DeckEngineerMode *m_pDeckEngMode;
   USBEngineerMode *m_pUsbEngMode; 
   QDBusInterface *m_pMicomIface;
   EDeckInit2aSetting m_tInit2a;
};

#endif // DHAVN_APPMEDIAPLAYER_ENGMODENOTIFIER_H
