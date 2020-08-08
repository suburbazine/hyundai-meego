#include <usbcontroller.h>
#include <QTimer>
#include <QDebug>
#include <DHAVN_AppSetting_Log.h>

#include "DHAVN_AppSettings_System_JukeBoxInfo.h"

const int appSettingsMaxScaleValue = 100;
const QString jukeboxVolumeName = "/usr_data";
#define MBSIZE  (1024L * 1024L)
//const qint64 mbSize = 1000 * 1000;

AppSettings_System_JukeBoxInfo::AppSettings_System_JukeBoxInfo(SettingsAppEngine* pEngine, QObject *parent) :
    QObject(parent),
    m_pEngine(pEngine),
    jukeBoxMaxMbSize(appSettingsMaxScaleValue),
    jukeBoxMaxScaleSize(appSettingsMaxScaleValue),
    usedJukeBoxMbSize(0),
    usedJukeBoxScaleSize(0),
    fileInterface(0),
    m_pFormatTimer(0)
{
    jukeBoxMaxMbSize = 750;//GetTotalDiscSpace()

    if (m_pEngine)
    {
        connect(m_pEngine, SIGNAL(sigJukeboxCapacityChanged(bool)), this, SLOT(startUpdateInfo(bool)));
    }
}

int AppSettings_System_JukeBoxInfo::GetJukeBoxMaxMBSize()
{
    return int(jukeBoxMaxMbSize);
}

int AppSettings_System_JukeBoxInfo::GetJukeBoxMaxScaleSize()
{
    return jukeBoxMaxScaleSize;
}

int AppSettings_System_JukeBoxInfo::GetUsedJukeBoxScaleSize()
{
    LOG2(QString("Used JukeBoxScaleSize() : %1").arg(usedJukeBoxScaleSize), Logger::PRIORITY_HI);
    return usedJukeBoxScaleSize;
}

void AppSettings_System_JukeBoxInfo::startUpdateInfo(bool useTimer)
{
    if(useTimer)
        QTimer::singleShot(1000, this, SLOT(UpdateInfo()));
    else
        UpdateInfo();
}

void AppSettings_System_JukeBoxInfo::UpdateInfo()
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    double totalJukeBoxSize = 0L;

    if (fileInterface == NULL)
    {
        fileInterface = USBController::GetFirstInstance();
        connect(fileInterface, SIGNAL(OnFormatFinished(int)), SLOT(FormattingComleteHandler(int)));
    }

    LOG2 ( QString("TotalDiscSpace: %1, UsedDiscSpace: %2").arg(fileInterface->GetTotalDiscSpace(jukeboxVolumeName)).arg(fileInterface->GetTotalUsedDiscSpace(jukeboxVolumeName)), Logger::PRIORITY_HI );
    totalJukeBoxSize = fileInterface->GetTotalDiscSpace(jukeboxVolumeName) / (MBSIZE);


    if(totalJukeBoxSize != 0)
    {
        this->jukeBoxMaxMbSize = totalJukeBoxSize;
        if (JukeboxEnable == true)
            this->usedJukeBoxMbSize = (double)(fileInterface->GetTotalUsedDiscSpace(jukeboxVolumeName)) / (MBSIZE);
        else
            this->usedJukeBoxMbSize = (double)(fileInterface->GetTotalUsedDiscSpace(jukeboxVolumeName)) / (MBSIZE);

        // delete for ITS 242784
        //if (usedJukeBoxMbSize < 0.1)
        //    usedJukeBoxMbSize = 0;

        InfoRecalculation();
        LOG2 ( QString("jukeBoxMaxMbSize:%1, usedJukeBoxMbSize:%2").arg(jukeBoxMaxMbSize).arg(usedJukeBoxMbSize), Logger::PRIORITY_HI );
    }
    else
    {
        LOG2 ( QString("jukeBoxMaxMbSize:%1").arg(totalJukeBoxSize), Logger::PRIORITY_HI );
    }

    //TBD verify parametr only usedJukeBoxMbSize = size;
    //TBD after API usedJukeBoxMbSize = fileInterface->GetUsedDiscSpase()
}

void AppSettings_System_JukeBoxInfo::InfoRecalculation()
{
    //precondition: using values of usedJukeBoxMbSize and jukeBoxMaxMbSize are updated
    LOG2(QString("[ScaleSize][MbSize]: [%1][%2]")
         .arg(usedJukeBoxScaleSize).arg(usedJukeBoxMbSize), Logger::PRIORITY_HI);

    LOG2(QString("[MaxMbSize][MaxScaleSize]: [%1][%2]")
         .arg(usedJukeBoxScaleSize).arg(usedJukeBoxMbSize), Logger::PRIORITY_HI);

    usedJukeBoxScaleSize = usedJukeBoxMbSize/jukeBoxMaxMbSize*jukeBoxMaxScaleSize;

    //added for ISV 98042 Jukebox Used Capacity Image Issue(when under 1%)
    if(usedJukeBoxScaleSize == 0 && usedJukeBoxMbSize > 0)
        usedJukeBoxScaleSize = 1;
    //added for ISV 98042 Jukebox Used Capacity Image Issue(when under 1%)

    emit jukeBoxInfoUpdated(jukeBoxMaxMbSize, usedJukeBoxMbSize ,usedJukeBoxScaleSize);
}

void AppSettings_System_JukeBoxInfo::InitiateJukeBoxFormat(int screen)
{
    LOG2 ( QString("InitiateJukeBoxFormat"), Logger::PRIORITY_HI );
    emit formattingStarted(screen);
    this->m_pFormatTimer = new QTimer(this);
    this->m_pFormatTimer->setSingleShot(true);
    this->m_pFormatTimer->setInterval(300);
    this->m_pFormatTimer->start();
    connect(this->m_pFormatTimer, SIGNAL(timeout()), SLOT(StartJukeBoxFormat()));
}

//////////////// SLOTS/////////////////////////
void AppSettings_System_JukeBoxInfo::FormattingComleteHandler(int status)
{
    LOG2 ( QString("FCH status: %1").arg(status), Logger::PRIORITY_HI );

    emit formatComplete();

    UpdateInfo();
}

void AppSettings_System_JukeBoxInfo::StartJukeBoxFormat()
{
    LOG2 ( QString("SJFormat"), Logger::PRIORITY_HI );

    if (this->fileInterface != NULL)
    {
        int result = this->fileInterface->StartDiscFormat(jukeboxVolumeName);
    }
}


