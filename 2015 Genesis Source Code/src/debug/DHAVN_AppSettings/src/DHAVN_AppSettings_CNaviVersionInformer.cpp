#include "hdr/DHAVN_AppSettings_CNaviVersionInformer.h"
#include <QSharedMemory>
#include <QDebug>
#include <cstring>

#define NAVI_VERINFO_SHARED_MEMORY_ID  "NaviVerInfoSharedData"

struct NAVI_VERINFO
{
     char MapVerStr[64]; //Map version info string
     char AppVerStr[64]; //App version info string
     char AssoNumStr[256]; //Association Number string
};

CNaviVersionInformer::CNaviVersionInformer(QObject *parent):
    QObject(parent), m_pNaviVerInfoSharedMem(NULL)
{
    loadNaviInfo();
}

void CNaviVersionInformer::loadNaviInfo()
{
    NAVI_VERINFO naviVerInfo;
    memset(&naviVerInfo, 0, sizeof(naviVerInfo));

    if(m_pNaviVerInfoSharedMem == NULL)
        m_pNaviVerInfoSharedMem = new QSharedMemory(NAVI_VERINFO_SHARED_MEMORY_ID);

    m_pNaviVerInfoSharedMem->setKey(NAVI_VERINFO_SHARED_MEMORY_ID);
    m_pNaviVerInfoSharedMem->attach();
    m_pNaviVerInfoSharedMem->create(sizeof(naviVerInfo));

    bool bValid = m_pNaviVerInfoSharedMem->lock();
    if(bValid)
    {
        char *from = (char*) m_pNaviVerInfoSharedMem->data();
        memcpy(&naviVerInfo, from, sizeof(naviVerInfo));

        /*
        qDebug("CNaviVersionInformer::loadNaviInfo(): Map ver= %s",
               naviVerInfo.MapVerStr);
        qDebug("CNaviVersionInformer::loadNaviInfo(): Association Num= %s",
               naviVerInfo.AssoNumStr);
        qDebug("CNaviVersionInformer::loadNaviInfo(): AppVerStr Num= %s",
               naviVerInfo.AppVerStr);
        */
        //Logger::Instance()->Log(QString("MapVer:[%1], Ass:[%2], App:[%3]").arg(naviVerInfo.MapVerStr)
        //                        .arg(naviVerInfo.AssoNumStr).arg(naviVerInfo.AppVerStr), Logger::PRIORITY_HI);

        this->associationNumber = QString::fromUtf8(naviVerInfo.AssoNumStr);
        this->mapVersion = QString::fromUtf8(naviVerInfo.MapVerStr);
        this->appVersion = QString::fromUtf8(naviVerInfo.AppVerStr);
    }
    else
    {
        //qDebug("CNaviVersionInformer::loadNaviInfo(): no access to NAVI shared memory");
        this->associationNumber = this->mapVersion = this->appVersion = "Unknown";
    }

    m_pNaviVerInfoSharedMem->unlock();
}

QString CNaviVersionInformer::getMapVersion()
{
    loadNaviInfo();

    return this->mapVersion;
}

QString CNaviVersionInformer::getAppVersion()
{
    loadNaviInfo();

    return this->appVersion;
}

QString CNaviVersionInformer::getAssociationNumber()
{
    loadNaviInfo();

    return this->associationNumber;
}
