#include "DHAVN_AppSettings_SystemInfo.h"

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QtDBus>
#include <DHAVN_AppSettings_CMOSTManager.h>
#include "DHAVN_AppSetting_Log.h"

#define SYSTEM_MODEL_INFO "SYSTEM_MODEL_INFO"
#define SYSTEM_BT_MAC_ADDRESS_INFO "SYSTEM_BT_MAC_ADDRESS_INFO"

#define TEST_IBOX_VERSION "PR_DH_NA E293 20130503.200954"
SystemInfo::SystemInfo(SettingsAppEngine* engine, ECountryVariant variant, QObject *parent)
    : QObject(parent)
    , m_Engine(engine)
    , m_vehicleModel("")
    , m_swVersion("")
    , m_kernelVersion("")
    , m_micomVersion("")
    , m_mapVersion("")
    , m_appVersion("")
    , m_associationNumber("")
    , m_iboxVersion("")
    , m_pNaviInformer( NULL )
    , m_Variant(variant)
    , m_gracenoteSDKVer("")
    //    , m_gracenoteDBVer(0)
    , m_pSettingsInfo( NULL )
    , m_gracenoteDBVer("")
    , m_sBTMacAddress("")
    , carHwType("DH") //added for DH15MY version
{

    m_pSettingsInfo = new QSettings( SETTINGS_SYSTEM_SETTINGS_INFO_FILE_PATH, QSettings::NativeFormat );

    connect(engine, SIGNAL(sigNaviSdcardRemoved()), this, SLOT(clearNaviInfo()));
    connect(engine, SIGNAL(sigNaviSdcardMounted()), this, SLOT(updateNaviInfo()));

    getModelInfo();
    GetIBoxVersion();

    RequestGNSDKVer();
    RequestGNDBVer();

    getBTMacAddressInfo();
}

SystemInfo::~SystemInfo()
{
    if(m_pNaviInformer)
        delete m_pNaviInformer;

    if (m_pSettingsInfo)
        delete m_pSettingsInfo;
}


void SystemInfo::clearNaviInfo ( )
{
    setMapVersion(" ");
    setAppVersion(" ");
    setAssociationNumber(" ");
}

void SystemInfo::getModelInfo()
{
    if ( m_vehicleModel.isEmpty() )
    {
        QDBusInterface interface( "com.lge.car.micom",
                                  "/SysInfo",
                                  "com.lge.car.micom.sysinfo");

        interface.setTimeout(2000);

        QDBusReply<QByteArray> reply = interface.call("LgeDBus_AskSysOption");
        if (reply.isValid())
        {
            QByteArray tmp(reply.value());
            if (tmp.size() > 0)
            {
                char vehicleVariant = tmp.at(0);
                switch (vehicleVariant)
                {
                case 0:
                    m_vehicleModel = "DH";
                    break;
                case 1:
                    m_vehicleModel = "KH";
                    break;
                case 2:
                    m_vehicleModel = "VI";
                    break;
                default:
                    m_vehicleModel = "";
                }

                if(!(m_vehicleModel.isEmpty()))
                {
                    if( m_pSettingsInfo )
                    {
                        //LOG2 ( QString("veh Model(%1)").arg(m_vehicleModel), Logger::PRIORITY_HI );
                        m_pSettingsInfo->setValue( SYSTEM_MODEL_INFO, QVariant( m_vehicleModel ) );
                        m_pSettingsInfo->sync();
                    }
                }
            }
            else
            {
                LOG2 ( QString("Model Info Bad return"), Logger::PRIORITY_HI );
            }
        }
        else
        {
            LOG2 ( QString("Model Info Reply inValid: %1)").arg(reply.error().message()), Logger::PRIORITY_HI );
        }
    }

    if(m_vehicleModel.isEmpty())
    {
        LOG2 ( QString("info failed from MMUS ."), Logger::PRIORITY_HI );

        if(m_pSettingsInfo)
        {
            if ( m_pSettingsInfo->contains(SYSTEM_MODEL_INFO) )
            {
                m_vehicleModel = m_pSettingsInfo->value(SYSTEM_MODEL_INFO).toString();
            }
        }
    }

    LOG2 ( QString("## Model Version(%1)").arg(m_vehicleModel), Logger::PRIORITY_HI );
}

void SystemInfo::getBTMacAddressInfo()
{
    if ( m_sBTMacAddress.isEmpty() )
    {
        QDBusInterface interface( "com.lge.car.micom",
                                  "/SysInfo",
                                  "com.lge.car.micom.sysinfo");

        interface.setTimeout(2000);

        QDBusReply<QByteArray> reply = interface.call("LgeDBus_AskBTMACAddr");
        if (reply.isValid())
        {
            QByteArray tmp(reply.value());
            if (tmp.size() > 0)
            {
                LOG2 ( QString("BT's Mac Add size(%1)").arg(tmp.size()), Logger::PRIORITY_HI );

                int temp[4] = {0,};
                int head_unit_id = 0;

                for (int i = 0; i < 4; i++)
                {
                    temp[i] = tmp.at(i);
                }

                head_unit_id = ( (temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | temp[3] );

                m_sBTMacAddress.clear();
                m_sBTMacAddress = QString("%1").arg(head_unit_id);

                while(m_sBTMacAddress.length() < 10)
                {
                    m_sBTMacAddress = m_sBTMacAddress.insert(0, "0");
                }

                if(!(m_sBTMacAddress.isEmpty()))
                {
                    if( m_pSettingsInfo )
                    {
                        //LOG2 ( QString("BT's Mac Address (%1).").arg(m_sBTMacAddress), Logger::PRIORITY_HI );
                        m_pSettingsInfo->setValue( SYSTEM_BT_MAC_ADDRESS_INFO, QVariant( m_sBTMacAddress ) );
                        m_pSettingsInfo->sync();
                    }
                }
            }
            else
                LOG2 ( QString("BT's Mac Add Bad return"), Logger::PRIORITY_HI );
        }
        else
            LOG2 ( QString("BT's Mac Add Reply inValid: %1").arg(reply.error().message()), Logger::PRIORITY_HI );
    }

    if(m_sBTMacAddress.isEmpty())
    {
        LOG2 ( QString("BT's Mac Add fail from MMUS."), Logger::PRIORITY_HI );

        if(m_pSettingsInfo)
        {
            if ( m_pSettingsInfo->contains(SYSTEM_BT_MAC_ADDRESS_INFO) )
            {
                m_sBTMacAddress = m_pSettingsInfo->value(SYSTEM_BT_MAC_ADDRESS_INFO).toString();
            }
        }
    }

    LOG2 ( QString("BT's Mac Address(%1)").arg(m_sBTMacAddress), Logger::PRIORITY_HI );
}

QString SystemInfo::GetModel()
{
    //return m_vehicleModel;
    return carHwType; //added for DH15MY version

}

QString SystemInfo::GetSWVersion()
{
    if ( m_swVersion.isEmpty() )
    {
        int line_index = -1;

        switch ( m_Variant )
        {
        case eCVKorea:        line_index = 0; break;
        case eCVNorthAmerica: line_index = 1; break;
        case eCVChina:        line_index = 2; break;
        case eCVGeneral:      line_index = 3; break;
        case eCVMiddleEast:   line_index = 4; break;
        case eCVEuropa:       line_index = 5; break;
        case eCVCanada:       line_index = 6; break;
        case eCVRussia:       line_index = 7; break;
        default:              line_index = 0; break;
        }

        QFile swVerFile( QString("/etc/version.txt") );

        if ( !swVerFile.open(QIODevice::ReadOnly | QIODevice::Text) )
        {
            LOG2 ( QString("can't o file /etc/version.txt"), Logger::PRIORITY_HI );
        }
        else
        {
            QTextStream swVerFileStream(&swVerFile);
            do
            {
                QString string = swVerFileStream.readLine();

                if ( !string.isNull() )
                {
                    m_swVersion = string;
                }
            }
            while( line_index-- );

            LOG2(QString("txt file Sw version : %1").arg(m_swVersion), Logger::PRIORITY_HI);

            QString sBoardVer = getHWMainBoardVersion();
            if ( m_swVersion.isEmpty() == false && sBoardVer.isEmpty() == false )
            {
                makeSWVersionWithBoardVersion( sBoardVer );
            }
        }


        swVerFile.close();
    }

    LOG2 ( QString("SW Version(%1)").arg(m_swVersion), Logger::PRIORITY_HI );
    return m_swVersion;
}

QString SystemInfo::GetKernelVersion()
{
    if ( m_kernelVersion.isEmpty() )
    {
        QFile kernelVerFile( QString("/proc/version") );

        if ( !kernelVerFile.open(QIODevice::ReadOnly | QIODevice::Text) )
        {
            LOG2 ( QString("can't' open file /proc/version"), Logger::PRIORITY_HI );
        }
        else
        {
            QTextStream kernelVerFileStream(&kernelVerFile);
            while (true)
            {
                QString line = kernelVerFileStream.readLine();
                QString pattern("Linux version ");

                if ( line.startsWith(pattern) )
                {
                    m_kernelVersion = getFirstSubString(pattern, line);
                    LOG2 ( QString("kernel version(%1)").arg(m_kernelVersion), Logger::PRIORITY_HI );
                    break;
                }

                if ( line.size() == 0 ) break;
            }
        }

        kernelVerFile.close();
    }

    return m_kernelVersion;
}

/*
QString SystemInfo::GetMicomVersion()
{
    if ( m_micomVersion.isEmpty() )
    {
        qDebug() << "SystemInfo::GetMicomVersion()";

        QDBusInterface interface( "com.lge.car.micom",
                                  "/SysInfo",
                                  "com.lge.car.micom.sysinfo");

        //interface.setTimeout(5000);

        QDBusReply<QByteArray> reply = interface.call("LgeDBus_AskSysVersion");
        if (reply.isValid())
        {
            QByteArray tmp(reply.value());
            //micomVersion.append(tmp.at(1) + '0');
            m_micomVersion = QString("%1.%2.%3").arg((int)tmp.at(0)).arg((int)tmp.at(1)).arg((int)tmp.at(2));
            qDebug() << "MICOM VERSION:" << m_micomVersion;
        }
        else
        {
            LOG2 ( QString("## Get Micom Version error(%1)").arg(reply.error().message()), Logger::PRIORITY_HI );
        }
    }

    return m_micomVersion;
}
*/

QString SystemInfo::GetMapVersion()
{
    if ( m_mapVersion.isEmpty() )
    {
        if(m_pNaviInformer == NULL)
        {
            m_pNaviInformer = new CNaviVersionInformer();
        }
        if( m_pNaviInformer != NULL )
        {
            m_mapVersion = m_pNaviInformer->getMapVersion();
        }
    }
    LOG2 ( QString("Map Version(%1)").arg(m_mapVersion), Logger::PRIORITY_HI );
    return m_mapVersion;
}

QString SystemInfo::GetAppVersion()
{
    if ( m_appVersion.isEmpty() )
    {
        if(m_pNaviInformer == NULL)
        {
            m_pNaviInformer = new CNaviVersionInformer();
        }
        if( m_pNaviInformer != NULL )
        {
            m_appVersion = m_pNaviInformer->getAppVersion();
        }
    }
    LOG2 ( QString("App Version(%1)").arg(m_appVersion), Logger::PRIORITY_HI );
    return m_appVersion;
}

QString SystemInfo::GetAssociationNumber()
{
    if ( m_associationNumber.isEmpty() )
    {
        if(m_pNaviInformer == NULL)
        {
            m_pNaviInformer = new CNaviVersionInformer();
        }
        if( m_pNaviInformer != NULL )
        {
            m_associationNumber = m_pNaviInformer->getAssociationNumber();
        }
    }

    LOG2 ( QString("Ass Number(%1)<---").arg(m_associationNumber), Logger::PRIORITY_HI );
    return m_associationNumber;
}

QString SystemInfo::GetIBoxVersion()
{
    if ( m_iboxVersion.isEmpty() )
    {
        //qDebug() << "SystemInfo::GetIBoxVersion()  isEmpty";
        connect( CMOSTManager::getInstance(),
                 SIGNAL(iboxVersionUpdated(const QStringList)),
                 this,
                 SLOT(iboxVersionUpdated(const QStringList)) );

    }

    CMOSTManager::getInstance()->requestIBoxVersion();

    LOG2 ( QString("i-Box Version(%1)").arg(m_iboxVersion), Logger::PRIORITY_HI );
    return m_iboxVersion;
}

void SystemInfo::iboxVersionUpdated( const QStringList version )
{
    int i;
    /*
    for ( i = 0; i < version.count(); i++ )
    {
        qDebug() << "Idx= " << i << " Version= " << version.at(i);
    }
    */

    if ( version.isEmpty() == false )
    {
        //! If string is empty, initialize with space character ( Error Exception )
        if ( version.at(0).isEmpty() == true )
        {
            m_iboxVersion = QString( "" );
        }
        else
        {
            m_iboxVersion = version.at(0);
        }
        emit notifyIBoxVersion();
    }
}


QString SystemInfo::getFirstSubString( const QString skipped, const QString src )
{
    QString tmp(src);

    tmp.remove(skipped);
    QStringList list = tmp.split(" ");

    return list.at(0);
}

QString SystemInfo::getHWMainBoardVersion()
{
    qDebug() << "SystemInfo::getHWMainBoardVersion()";
    QString retValue;

    QDBusInterface interface( "com.lge.car.micom",
                              "/SysInfo",
                              "com.lge.car.micom.sysinfo");

    //interface.setTimeout(5000);

    QDBusReply<QByteArray> reply = interface.call("LgeDBus_AskSysVersion");
    if (reply.isValid())
    {
        QByteArray tmp(reply.value());

        if ( tmp.size() >= 17 )
        {
            int iBDVer = (int) tmp.at(16);

            //added for DH15MY version
            if((int) tmp.at(13) == 0x00)
            {
                LOG2(QString("HW type value : 0x00"), Logger::PRIORITY_HI );
                carHwType.clear();
                carHwType = "DH";
            }
            else if((int) tmp.at(13) == 0x01)
            {
                LOG2(QString("HW type value : 0x01"), Logger::PRIORITY_HI );
                carHwType.clear();
                if(m_Variant == 1 || m_Variant == 6)
                {
                    carHwType = "DH16MY";
                }
                else
                {
                    carHwType = "DH15MY";
                }

            }
            else if((int) tmp.at(13) == 0x02)
            {
                LOG2(QString("HW type value : 0x02"), Logger::PRIORITY_HI );
                carHwType.clear();
                carHwType = "DHPE";
            }
            else if( (int)tmp.at(13) == 17 ){
                LOG2(QString("HW type value : 17"), Logger::PRIORITY_HI );
                carHwType.clear();
                carHwType = "DH17MY";
            }
            else
            {
                LOG2(QString("HW type value :[%1]").arg((int) tmp.at(13)), Logger::PRIORITY_HI );
            }
            //added for DH15MY version
            emit notifyModel();

            //qDebug() << "BD Ver: " << iBDVer;
            if ( iBDVer >= 0 )
            {
                if ( iBDVer >= 10 )
                {
                    retValue = QString("%1").arg(iBDVer);
                }
                else
                {
                    retValue = QString("0%1").arg(iBDVer);
                }
            }
        }
    }
    else
    {
        LOG2 ( QString("HW MainBoard Ver Err code(%1)<---").arg(reply.error().message()), Logger::PRIORITY_HI );
    }

    return retValue;
}

void SystemInfo::makeSWVersionWithBoardVersion( const QString BoardVer )
{
    LOG2 ( QString("SW ver(%1), Board Ver(%2)").arg(m_swVersion).arg(BoardVer), Logger::PRIORITY_HI );
    //int iReplaceIdx = -1; //added for DH15MY version
    QStringList tParseList = m_swVersion.split( "." );

    //added for DH15MY version
    int strCount = tParseList.count();

    QString swVersionInfo = "";

    if(strCount == 6){
        swVersionInfo = QString("%1.%2.%3.%4.%5.%6")
                .arg(carHwType).arg(tParseList.at(1)).arg(tParseList.at(2)).arg(BoardVer)
                .arg(tParseList.at(4)).arg(tParseList.at(5));
    }
    else if(strCount == 5){
        swVersionInfo = QString("%1.%2.%3.%4.%5")
                .arg(carHwType).arg(tParseList.at(1)).arg(tParseList.at(2)).arg(BoardVer)
                .arg(tParseList.at(4));
    }
    else if(strCount == 0){
        swVersionInfo = QString("Not.Received");
    }
    else{
        swVersionInfo = QString("%1.%2.%3.%4.%5")
                .arg(carHwType).arg(tParseList.at(1)).arg(tParseList.at(2)).arg(BoardVer)
                .arg(tParseList.at(4));
    }
    LOG2(QString("Parsing Software Version: [%1]").arg(swVersionInfo), Logger::PRIORITY_HI);

    m_swVersion = swVersionInfo;

     //added for DH15MY version
    /* 
    for (int i = 0; i < tParseList.size(); i++)
    {
        QString sStr = tParseList.at(i);

        if ( isContainNumber( sStr ) == true )
        {
            iReplaceIdx = i;
            break;
        }
    }

    if ( iReplaceIdx >= 0 && iReplaceIdx < tParseList.size() )
    {
        m_swVersion = "";
        for ( int i=0; i < tParseList.size(); i++ )
        {
            if ( i == iReplaceIdx )
            {
                m_swVersion.append( BoardVer );
            }
            else if(i == 0) //added for DH15MY version
            {
                m_swVersion.append(carHwType);
            }
            else
            {
                m_swVersion.append( tParseList.at(i) );
            }

            if ( i != (tParseList.size()-1) )
            {
                m_swVersion.append( "." );
            }
        }

        //qDebug() << "SW Version: " << m_swVersion;
    }
    */
    //added for DH15MY version

}

bool SystemInfo::isContainNumber( const QString string )
{
    bool ret = false;
    QByteArray::const_iterator constIterator;

    QByteArray pArray = string.toAscii();
    for (constIterator = pArray.constBegin(); constIterator != pArray.constEnd();
         ++constIterator)
    {
        char cChar = *constIterator;
        //! Check it contains number
        if ( cChar >= 0x30 && cChar <= 0x39 )
        {
            ret = true;
            break;
        }
    }

    return ret;
}

void SystemInfo::RequestGNSDKVer()
{
    //LOG2 ( QString("RequestGNSDKVer"), Logger::PRIORITY_HI );
    // Connect signal with Gracenote DBus interface
    QDBusConnection m_Connection = QDBusConnection::sessionBus();
    bool result = m_Connection.connect("org.lge.GracenoteCDLookupDBus", "/",
                                       "org.lge.GracenoteCDLookupDBus", "GNSDKVer", this,
                                       SLOT(onGNSDKVer(QString)));

    QDBusInterface interface ("org.lge.GracenoteCDLookupDBus", "/");
    QDBusMessage msg = interface.call("GetSDKVer");
    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

void SystemInfo::onGNSDKVer(QString ver)
{
    // Disconnect signal with Gracenote DBus interface
    QDBusConnection m_Connection = QDBusConnection::sessionBus();
    m_Connection.disconnect("org.lge.GracenoteCDLookupDBus", "/",
                            "org.lge.GracenoteCDLookupDBus", "GNSDKVer", this,
                            SLOT(onGNSDKVer(QString)));
    setGraceNoteSDKVersion(ver);

    LOG2 ( QString("Gnote SDK Ver(%1)").arg(ver), Logger::PRIORITY_HI );
}

void SystemInfo::RequestGNDBVer()
{
    //LOG2 ( QString("--->"), Logger::PRIORITY_HI );
    // Connect signal with Gracenote DBus interface
    QDBusConnection m_Connection = QDBusConnection::sessionBus();
    bool result = m_Connection.connect("org.lge.GracenoteCDLookupDBus", "/",
                                       "org.lge.GracenoteCDLookupDBus", "GNDBRegionVer", this,
                                       SLOT(onGNDBVer(QString)));

    QDBusInterface interface ("org.lge.GracenoteCDLookupDBus", "/");
    QDBusMessage msg = interface.call("GetDBRegionVer");
    //LOG2 ( QString("<---"), Logger::PRIORITY_HI );
}

void SystemInfo::onGNDBVer(QString ver)
{
    // Disconnect signal with Gracenote DBus interface
    QDBusConnection m_Connection = QDBusConnection::sessionBus();
    m_Connection.disconnect("org.lge.GracenoteCDLookupDBus", "/",
                            "org.lge.GracenoteCDLookupDBus", "GNDBRegionVer", this,
                            SLOT(onGNDBVer(QString)));

    setGraceNoteDBVersion(ver);

    LOG2 ( QString("Gnote DB_R Ver(%1)").arg(ver), Logger::PRIORITY_HI );
}

void SystemInfo::updateNaviInfo()
{
    LOG2 ( QString("mapVer(%1),appVer(%2),assNumber(%3)").arg(m_mapVersion).arg(m_appVersion).arg(m_associationNumber), Logger::PRIORITY_HI );
    if ( m_mapVersion.isEmpty() || m_appVersion.isEmpty() || m_associationNumber.isEmpty() ||
         m_mapVersion == " " || m_appVersion == " " || m_associationNumber == " ")
    {
        if(m_pNaviInformer == NULL)
            m_pNaviInformer = new CNaviVersionInformer();

        if( m_pNaviInformer != NULL )
        {
            setMapVersion(m_pNaviInformer->getMapVersion());
            setAppVersion(m_pNaviInformer->getAppVersion());
            setAssociationNumber(m_pNaviInformer->getAssociationNumber());
        }
    }
}
