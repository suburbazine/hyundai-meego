#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "util/DHAVN_AppMediaPlayer_EngModeNotifier.h"

EngModeNotifier::EngModeNotifier(QObject *parent) :
    QObject(parent),
    m_pInterface(NULL),
    m_pWatcher(NULL),
    m_pDeckEngMode(new DeckEngineerMode()),
    m_pUsbEngMode(new USBEngineerMode()),
    m_pMicomIface(NULL)
{
   if (QDBusConnection::sessionBus().interface()->isServiceRegistered("com.lge.car.eng.service"))
   {
      m_pInterface = new QDBusInterface( "com.lge.car.eng.service", "/deck", "com.lge.car.eng.deck");
      m_pInterface->connection().connect("com.lge.car.eng.service", "/deck", "com.lge.car.eng.deck",
                                         "Ask_DeckInfo", this, SLOT(OnAsk_DeckInfo()));
      m_pInterface->connection().connect("com.lge.car.eng.service", "/deck", "com.lge.car.eng.deck",
                                         "Ask_AudioPathInfo", this, SLOT(OnAsk_AudioPathInfo()));
      m_pInterface->connection().connect("com.lge.car.eng.service", "/deck", "com.lge.car.eng.deck",
                                         "Set_AudioPathInfo", this, SLOT(OnSet_AudioPathInfo(uchar)));
   }
   else
   {
      m_pWatcher = new QDBusServiceWatcher("com.lge.car.eng.service",
                                           QDBusConnection::sessionBus(),
                                           QDBusServiceWatcher::WatchForRegistration);
      m_pWatcher->addWatchedService("com.lge.car.eng.service");
      QObject::connect(m_pWatcher,
                       SIGNAL(serviceRegistered(QString)),
                       this,
                       SLOT(OnServiceRegistered(QString)));
   }

   connect(m_pDeckEngMode , SIGNAL(OnCommErrorWithDeckChanged(int)), this, SLOT(HandleCommErrorWithDeckChanged(int)));
   m_pDeckEngMode->StartDiagnostic();
   if (QDBusConnection::sessionBus().interface()->isServiceRegistered("com.lge.car.micom"))
   {
      m_pMicomIface = new QDBusInterface("com.lge.car.micom","/Dtc", "com.lge.car.micom.dtc");
   }
   else
   {
       MP_LOG << "Service com.lge.car.micom is not registered" << LOG_ENDL;
   }
}

EngModeNotifier::~EngModeNotifier()
{
    if(m_pInterface)
    {
        delete m_pInterface;
    }
    if(m_pWatcher)
    {
        delete m_pWatcher;
    }
    if(m_pDeckEngMode)
    {
        m_pDeckEngMode->EndDiagnostic();
        delete m_pDeckEngMode;
    }
    if(m_pMicomIface)
    {
        delete m_pMicomIface;
    }
}

// { added by kihyung 2012.07.12 for CR 9874
DeckEngineerMode* EngModeNotifier::GetDeckEngineerMode()
{
    return m_pDeckEngMode;
}
// } added by kihyung

USBEngineerMode* EngModeNotifier::GetUSBEngineerMode()
{
    return m_pUsbEngMode;
}

void EngModeNotifier::SendUSBDiagResult(bool bFront, bool bFail)
{
    if(bFront) {
        char pData[] = {0xA8, 0x41, 0x00, 0x00};
        if(bFail) {
            pData[3] = 0x01; // if there is an error, change value into 1.
        }
        if(m_pMicomIface) {
            m_pMicomIface->call("LgeDBus_DTCSendMsg", QByteArray(pData, 4));
        }
    }
    else
    {
        char pData[] = {0xA9, 0x54, 0x00, 0x00};
        if(bFail) {
            pData[3] = 0x01; // if there is an error, change value into 1.
        }
        if(m_pMicomIface) {
            m_pMicomIface->call("LgeDBus_DTCSendMsg", QByteArray(pData, 4));
        }
    }
}

void EngModeNotifier::OnServiceRegistered(QString service)
{
//    MP_LOG << "service =" << service << LOG_ENDL;
    if (service == "com.lge.car.eng.service" && !m_pInterface)
    {
        m_pInterface = new QDBusInterface( "com.lge.car.eng.service", "/deck", "com.lge.car.eng.deck");

        MP_LOG << "m_pInterface->isValid()" << m_pInterface->isValid() << LOG_ENDL;
        bool connected = m_pInterface->connection().connect("com.lge.car.eng.service",
                                                            "/deck",
                                                            "com.lge.car.eng.deck",
                                                            "Ask_DeckInfo",
                                                            this,
                                                            SLOT(OnAsk_DeckInfo()));
        MP_LOG << "Ask_DeckInfo connected " << connected << LOG_ENDL;
        connected = m_pInterface->connection().connect("com.lge.car.eng.service",
                                                       "/deck",
                                                       "com.lge.car.eng.deck",
                                                       "Ask_AudioPathInfo",
                                                       this,
                                                       SLOT(OnAsk_AudioPathInfo()));
        MP_LOG << "Ask_AudioPathInfo connected " << connected << LOG_ENDL;
        connected = m_pInterface->connection().connect("com.lge.car.eng.service",
                                                       "/deck",
                                                       "com.lge.car.eng.deck",
                                                       "Set_AudioPathInfo",
                                                       this,
                                                       SLOT(OnSet_AudioPathInfo(uchar)));
        MP_LOG << "Set_AudioPathInfo connected " << connected << LOG_ENDL;
    }
}

void EngModeNotifier::OnAsk_DeckInfo()
{
    MP_LOG << LOG_ENDL;

    QStringList  _DeckInfo;
    _DeckInfo.clear();
    _DeckInfo << QString::number( m_pDeckEngMode->GetFirmwareVersion() );
    _DeckInfo << QString::number( m_pDeckEngMode->GetLifeTime() );
    _DeckInfo << QString::number( m_pDeckEngMode->GetTemperature() );

    MP_LOG << "_DeckInfo " << _DeckInfo << LOG_ENDL;
    m_pInterface->call("resDeckInfoString", _DeckInfo);

    QByteArray  _DeckStatus;
    _DeckStatus.clear();
    _DeckStatus.resize(7);
    _DeckStatus[0] = m_pDeckEngMode->GetEjectStatus();
    _DeckStatus[1] = m_pDeckEngMode->GetInsertStatus();
    _DeckStatus[2] = m_pDeckEngMode->GetRandomStatus();
    _DeckStatus[3] = m_pDeckEngMode->GetRepeatStatus();
    _DeckStatus[4] = m_pDeckEngMode->GetScanStatus();
    _DeckStatus[5] = m_pDeckEngMode->GetMechError();
    _DeckStatus[6] = m_pDeckEngMode->GetReadError();

    MP_LOG << "_DeckStatus " << QString::number(_DeckStatus[0]) \
                             << QString::number(_DeckStatus[1]) << QString::number(_DeckStatus[2]) \
                             << QString::number(_DeckStatus[3]) << QString::number(_DeckStatus[4]) \
                             << QString::number(_DeckStatus[5]) << QString::number(_DeckStatus[6]) << LOG_ENDL;

    m_pInterface->call("resDeckInfoStatus", _DeckStatus);
}

void EngModeNotifier::OnAsk_AudioPathInfo()
{
    MP_LOG << LOG_ENDL;

    m_pDeckEngMode->GetInitSetting2a(m_tInit2a);

    int audioPath = m_tInit2a.m_nAudioOutput;

    MP_LOG << "audioPath =" << audioPath << LOG_ENDL;

    m_pInterface->call("resAudioPathInfo",(uchar)audioPath );
}

void EngModeNotifier::OnSet_AudioPathInfo( uchar in0 )
{
   MP_LOG << LOG_ENDL;
   m_pDeckEngMode->GetInitSetting2a(m_tInit2a);
   m_tInit2a.m_nAudioOutput = in0;
   m_pDeckEngMode->SetInitSetting2a(m_tInit2a);
}

void EngModeNotifier::HandleCommErrorWithDeckChanged(int aErrorCode)
{
    char pData[] = {0xA8, 0x35, 0x00, 0x00}; // First 3bytes(A83500): DTC code. Last 1bytes(0x00): value.

    if(aErrorCode)
    {
        pData[3] = 0x01; // if there is an error, change value into 1.
    }

    if(m_pMicomIface)
    {
        m_pMicomIface->call("LgeDBus_DTCSendMsg", QByteArray(pData, 4));
    }
}

// { added by cychoi 2013.07.26 for ITS 181382 & 158365 Initialize DVD Settings
void EngModeNotifier::ResetData(EMPCountryVariant eCountryVariant)
{
    MP_LOG << LOG_ENDL;

    EDeckInit1Setting tInit1;
    m_pDeckEngMode->GetInitSetting1(tInit1);
    switch(eCountryVariant)
    {
        case eMPCVKorea:
            tInit1.m_nPrioSubTitleCode = 1115;
            tInit1.m_nPrioAudioCode = 1115;
            tInit1.m_nPrioMenuLangCode = 1115;
            break;
        case eMPCVChina:
            tInit1.m_nPrioSubTitleCode = 2608;
            tInit1.m_nPrioAudioCode = 2608;
            tInit1.m_nPrioMenuLangCode = 2608;
            break;
        case eMPCVMiddleEast:
            tInit1.m_nPrioSubTitleCode = 118;
            tInit1.m_nPrioAudioCode = 118;
            tInit1.m_nPrioMenuLangCode = 118;
            break;
        case eMPCVRussia:
            tInit1.m_nPrioSubTitleCode = 1821;
            tInit1.m_nPrioAudioCode = 1821;
            tInit1.m_nPrioMenuLangCode = 1821;
            break;
        case eMPCVNorthAmerica:
        case eMPCVGeneral:
        case eMPCVEuropa:
        case eMPCVCanada:
        case eMPCVInvalid:
        case eMPCVInvalidMax:
        default:
            tInit1.m_nPrioSubTitleCode = 514;
            tInit1.m_nPrioAudioCode = 514;
            tInit1.m_nPrioMenuLangCode = 514;
            break;
    }
    m_pDeckEngMode->SetInitSetting1(tInit1);

}

void EngModeNotifier::FactoryReset(EMPCountryVariant eCountryVariant)
{
    MP_LOG << LOG_ENDL;

    EDeckInit1Setting tInit1;
    m_pDeckEngMode->GetInitSetting1(tInit1);
    switch(eCountryVariant)
    {
        case eMPCVKorea:
            tInit1.m_nPrioSubTitleCode = 1115;
            tInit1.m_nPrioAudioCode = 1115;
            tInit1.m_nPrioMenuLangCode = 1115;
            break;
        case eMPCVChina:
            tInit1.m_nPrioSubTitleCode = 2608;
            tInit1.m_nPrioAudioCode = 2608;
            tInit1.m_nPrioMenuLangCode = 2608;
            break;
        case eMPCVMiddleEast:
            tInit1.m_nPrioSubTitleCode = 118;
            tInit1.m_nPrioAudioCode = 118;
            tInit1.m_nPrioMenuLangCode = 118;
            break;
        case eMPCVRussia:
            tInit1.m_nPrioSubTitleCode = 1821;
            tInit1.m_nPrioAudioCode = 1821;
            tInit1.m_nPrioMenuLangCode = 1821;
            break;
        case eMPCVNorthAmerica:
        case eMPCVGeneral:
        case eMPCVEuropa:
        case eMPCVCanada:
        case eMPCVInvalid:
        case eMPCVInvalidMax:
        default:
            tInit1.m_nPrioSubTitleCode = 514;
            tInit1.m_nPrioAudioCode = 514;
            tInit1.m_nPrioMenuLangCode = 514;
            break;
    }
    m_pDeckEngMode->SetInitSetting1(tInit1);
}
// } added by cychoi 2013.07.26

