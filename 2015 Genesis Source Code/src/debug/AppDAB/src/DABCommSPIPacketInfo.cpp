#include "DABCommSPIPacketInfo.h"
#include <QFile>
#include <QDebug>

DABCommSPIPacketInfo::DABCommSPIPacketInfo(QObject *parent) :
    QObject(parent)
{
    device = 0;
    frameposition = 0;
    totalLength = 0;
    commandValue = 0;
    slsFilename = "";
    pDABCommManager = NULL;
    packetBuffer = NULL;
    packetLastIndex = 0;
    m_bExistLogo = false;
    m_sLogoFilename = "";
    m_sTempLogoFilename = "";
}

void
DABCommSPIPacketInfo::washOffData(void)
{
    device = 0;
    frameposition = 0;
    totalLength = 0;
    commandValue = -1;
    packetBuffer = NULL;
    packetLastIndex = 0;
}
