#include "DHAVN_QwertyKeypad_Utility.h"
#include "DHAVN_QwertyKeypad_Automata_Korean.h"
#include <QApplication>
#include <QDebug>
#include <QTime>

#include "DHAVN_QwertyKeypad_Log.h"

#ifdef DESKTOP_WINDOWS
// For Windows
#define DIR_LINGUISTIC_DATA  ("../DHAVN_QwertyKeypadWidget/localization/")
#else
// For Linux
#define DIR_LINGUISTIC_DATA  ("/usr/lib/qt4/imports/QmlQwertyKeypadWidget/localization/")
#endif

#define SETBOUNDARY(value, frValue, toValue, defValue) (((value < frValue)||(value>toValue))?defValue:value)

// Shared Memory Info for QwertyKeypad
#define SHM_SYS_LANGUAGE_TYPE_SIZE                (sizeof(char)*4)
#define SHM_SYS_CURRENT_REGION_SIZE               (sizeof(char)*4)
#define SHM_DEFAULT_KEYPAD_TYPE_SIZE              (sizeof(char)*4)
#define SHM_KR_KEYPAD_TYPE_SIZE                   (sizeof(char)*4)
#define SHM_EN_KEYPAD_TYPE_SIZE                   (sizeof(char)*4)
#define SHM_EU_KEYPAD_TYPE_SIZE                   (sizeof(char)*4)
#define SHM_RUSSIA_KEYPAD_TYPE_SIZE               (sizeof(char)*4)
#define SHM_ARABIC_KEYPAD_TYPE_SIZE               (sizeof(char)*4)


#define SHM_OFFSET_SYS_LANGUAGE_TYPE              (0)
#define SHM_OFFSET_SYS_CURRENT_REGION             (SHM_SYS_LANGUAGE_TYPE_SIZE)
#define SHM_OFFSET_DEFAULT_KEYPAD_TYPE            (SHM_OFFSET_SYS_CURRENT_REGION + SHM_SYS_CURRENT_REGION_SIZE)
#define SHM_OFFSET_KR_KEYPAD_TYPE                 (SHM_OFFSET_DEFAULT_KEYPAD_TYPE + SHM_DEFAULT_KEYPAD_TYPE_SIZE)
#define SHM_OFFSET_EN_KEYPAD_TYPE                 (SHM_OFFSET_KR_KEYPAD_TYPE + SHM_KR_KEYPAD_TYPE_SIZE)
#define SHM_OFFSET_EU_KEYPAD_TYPE                 (SHM_OFFSET_EN_KEYPAD_TYPE + SHM_EN_KEYPAD_TYPE_SIZE)
#define SHM_OFFSET_RUSSIA_KEYPAD_TYPE             (SHM_OFFSET_EU_KEYPAD_TYPE + SHM_EU_KEYPAD_TYPE_SIZE)
#define SHM_OFFSET_ARABIC_KEYPAD_TYPE             (SHM_OFFSET_RUSSIA_KEYPAD_TYPE + SHM_RUSSIA_KEYPAD_TYPE_SIZE)
#define SHM_KEYPAD_INFO_TOTAL_SIZE                (SHM_OFFSET_ARABIC_KEYPAD_TYPE + SHM_ARABIC_KEYPAD_TYPE_SIZE)

QwertyKeypadUtility::QwertyKeypadUtility(QObject *parent) : QObject(parent)
  ,m_pTranslator(NULL), m_pTranslatorLauncher(NULL), m_nCurrentAutomataID(AUTOMATA_NONE),m_nLanguageType(LanguageUnkown),m_country(CountryVariantInvalidMax)
  ,m_nKeypadTypeEn(0), m_nKeypadTypeKo(0), m_nKeypadTypeLa(0), m_nKeypadTypeAr(0), m_nKeypadTypeCh(0)
  ,m_nKeypadTypeEu(0), m_nKeypadTypeRu(0), m_nKeypadDefaultType(0), m_nFirstCapital(false), m_pSharedMemory(NULL),m_sAutomataString("")
  ,conn(QDBusConnection::sessionBus()) //added for ITS 247594 Long Touch Beep Issue
{
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("start = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());

    m_pAutomata           = getAutomataInstance();

    loadSettings();
    connectDBUSInterface(); //added for ITS 247594 Long Touch Beep Issue

    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("end = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}
//added for ITS 247594 Long Touch Beep Issue
void QwertyKeypadUtility::connectDBUSInterface()
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    if (!conn.isConnected())
    {
        //qDebug()<<"QwertyKeypadUtility :: connectDBUSInterface() connect fail!!";
        return;
    }

    soundSetupControl = QDBusMessage::createMethodCall( "com.lge.car.micom", "/","com.lge.car.micom.audioif", "ProcessAudioControlMessage" );

    if(conn.connect("com.lge.car.micom", "/", "com.lge.car.micom.audioif", "EventMessage", this, SLOT(EventMessage(QDBusMessage))))
    {
        //qDebug()<<"QwertyKeypadUtility::connectDBUSInterface() => EventMessage connect success!!";
        LOG2 ( QString("connectDBUSInterface"), Logger::PRIORITY_LOW );
    }
    else
    {
        //qDebug()<<"QwertyKeypadUtility::connectDBUSInterface() => EventMessage connect failed!!";
        LOG2 ( QString("connectDBUSInterface"), Logger::PRIORITY_LOW );
    }
}
void QwertyKeypadUtility::callAudioBeepCommand()
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );

    QList<QVariant> args;
    args.append( c_CntlAudioBeepCommand );
    args.append( (quint8) 0x0);
    soundSetupControl.setArguments(args);
    conn.call(soundSetupControl, QDBus::NoBlock);
}
//added for ITS 247594 Long Touch Beep Issue

void QwertyKeypadUtility::loadSettings(void)
{
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("start = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());

    //m_mutex.lock();


    if(m_pSharedMemory == NULL)
        m_pSharedMemory = new QSharedMemory("QwertyKeypadInformation");

    if (m_pSharedMemory)
    {
        if ( !m_pSharedMemory->isAttached() )
        {
            if(!m_pSharedMemory->attach(QSharedMemory::ReadOnly))
                LOG2(QString("Shared Memory Attach Failed!!"), Logger::PRIORITY_HI); //added for Log System Lock Issue
                //qCritical()<<"Shared Memory Attach Failed!!";
        }

        if(GetSharedMemory(SHM_OFFSET_SYS_LANGUAGE_TYPE, (unsigned char *)&m_nLanguageType, SHM_SYS_LANGUAGE_TYPE_SIZE ))
            LOG2(QString("Read Language Type(%1) from Shared memory").arg(m_nLanguageType), Logger::PRIORITY_LOW); //added for Log System Lock Issue
            //qCritical()<<"Read Language Type("<<m_nLanguageType<<") from Shared memory";
        else
            LOG2(QString("Failed to read Language Type from Shared memory"), Logger::PRIORITY_HI); //added for Log System Lock Issue
            //qCritical()<<"Failed to read Language Type from Shared memory";

        if(GetSharedMemory(SHM_OFFSET_SYS_CURRENT_REGION, (unsigned char *)&m_country, SHM_SYS_CURRENT_REGION_SIZE ))      
            LOG2(QString("Read Current Region(%1) from Shared memory").arg(m_country), Logger::PRIORITY_LOW); //added for Log System Lock Issue
            //qCritical()<<"Read Current Region("<<m_country<<") from Shared memory";
        else
            LOG2(QString("Failed to read Current Region from Shared memory"), Logger::PRIORITY_HI); //added for Log System Lock Issue
            //qCritical()<<"Failed to read Current Region from Shared memory";

		//added for AUS Variant BT Keypad Black Screen Issue
        if(m_country == CountryVariantAustralia)
        {
            m_country = CountryVariantNorthAmerica;
            m_nLanguageType = LanguageEN_US;
        }

        if( m_nLanguageType == LanguageUnkown )
        {
            switch(m_country)
            {
            case CountryVariantNorthAmerica:
            case CountryVariantGeneral:
            case CountryVariantEuropa:
            case CountryVariantAustralia:
                m_nLanguageType = LanguageEN;
                break;
            case CountryVariantRussian:
                m_nLanguageType = LanguageRU;
                break;
            case CountryVariantChina:
                m_nLanguageType = LanguageZH_CN;
                break;
            case CountryVariantMiddleEast:
                m_nLanguageType = LanguageAR;
                break;
            case CountryVariantKorea:
            default:
                m_nLanguageType = LanguageKO;
            }
        }
        //qCritical()<<"m_nLanguageType:"<<m_nLanguageType<<", m_country:"<<m_country;
        //LOG2(QString("m_nLanguageType: [%1], m_country:[%2]").arg(m_nLanguageType).arg(m_country), Logger::PRIORITY_HI); //added for Log System Lock Issue

        // English Keypad
        if(GetSharedMemory(SHM_OFFSET_EN_KEYPAD_TYPE, (unsigned char *)&m_nKeypadTypeEn, SHM_EN_KEYPAD_TYPE_SIZE ))
        {
            //qCritical()<<"Read English Keypad("<<m_nKeypadTypeEn<<") from Shared memory";
            //LOG2(QString("Read English Keypad: [%1] from Shared memory").arg(m_nKeypadTypeEn), Logger::PRIORITY_HI); //added for Log System Lock Issue
            m_nKeypadTypeEn = SETBOUNDARY(m_nKeypadTypeEn, 0,1,0);
        }
        else
            LOG2(QString("Failed to read English Keypad from Shared memory"), Logger::PRIORITY_HI); //added for Log System Lock Issue
            //qCritical()<<"Failed to read English Keypad from Shared memory";

        // Korean Keypad
        if(GetSharedMemory(SHM_OFFSET_KR_KEYPAD_TYPE, (unsigned char *)&m_nKeypadTypeKo, SHM_KR_KEYPAD_TYPE_SIZE ))
        {
            //qCritical()<<"Read Korean Keypad("<<m_nKeypadTypeKo<<") from Shared memory";
            //LOG2(QString("Read Korean Keypad: [%1] from Shared memory").arg(m_nKeypadTypeKo), Logger::PRIORITY_HI); //added for Log System Lock Issue
            m_nKeypadTypeKo = SETBOUNDARY(m_nKeypadTypeKo, 0,1,0);
        }
        else
            LOG2(QString("Failed to read Korean Keypad from Shared memory"), Logger::PRIORITY_HI); //added for Log System Lock Issue
            //qCritical()<<"Failed to read Korean Keypad from Shared memory";

        // Arabic Keypad
        if(GetSharedMemory(SHM_OFFSET_ARABIC_KEYPAD_TYPE, (unsigned char *)&m_nKeypadTypeAr, SHM_ARABIC_KEYPAD_TYPE_SIZE ))
        {
            //qCritical()<<"Read Arabic Keypad("<<m_nKeypadTypeAr<<") from Shared memory";
            //LOG2(QString("Read Arabic Keypad: [%1] from Shared memory").arg(m_nKeypadTypeAr), Logger::PRIORITY_HI); //added for Log System Lock Issue
            m_nKeypadTypeAr = SETBOUNDARY(m_nKeypadTypeAr, 0,1,0);
        }
        else
            LOG2(QString("Failed to read Arabic Keypad from Shared memory"), Logger::PRIORITY_HI); //added for Log System Lock Issue
            //qCritical()<<"Failed to read Arabic Keypad from Shared memory";

        // European Keypad
        if(GetSharedMemory(SHM_OFFSET_EU_KEYPAD_TYPE, (unsigned char *)&m_nKeypadTypeEu, SHM_EU_KEYPAD_TYPE_SIZE ))
        {
            //qCritical()<<"Read European Keypad("<<m_nKeypadTypeEu<<") from Shared memory";
            //LOG2(QString("Read European Keypad: [%1] from Shared memory").arg(m_nKeypadTypeEu), Logger::PRIORITY_HI); //added for Log System Lock Issue
            m_nKeypadTypeEu = SETBOUNDARY(m_nKeypadTypeEu, 0,3,0);
        }
        else
            LOG2(QString("Failed to read European Keypad from Shared memory"), Logger::PRIORITY_HI); //added for Log System Lock Issue
            //qCritical()<<"Failed to read European Keypad from Shared memory";

        // Russia Keypad
        if(GetSharedMemory(SHM_OFFSET_RUSSIA_KEYPAD_TYPE, (unsigned char *)&m_nKeypadTypeRu, SHM_RUSSIA_KEYPAD_TYPE_SIZE ))
        {
            //qCritical()<<"Read Russian Keypad("<<m_nKeypadTypeRu<<") from Shared memory";
            //LOG2(QString("Read Russian Keypad: [%1] from Shared memory").arg(m_nKeypadTypeRu), Logger::PRIORITY_HI); //added for Log System Lock Issue
            m_nKeypadTypeRu = SETBOUNDARY(m_nKeypadTypeRu, 0,1,0);
        }
        else
            LOG2(QString("Failed to read Russian Keypad from Shared memory"), Logger::PRIORITY_HI); //added for Log System Lock Issue
            //qCritical()<<"Failed to read Russian Keypad from Shared memory";

        if(GetSharedMemory(SHM_OFFSET_DEFAULT_KEYPAD_TYPE, (unsigned char *)&m_nKeypadDefaultType, SHM_DEFAULT_KEYPAD_TYPE_SIZE ))
            LOG2(QString("Read Default Keypad: [%1] from Shared memory").arg(m_nKeypadDefaultType), Logger::PRIORITY_LOW); //added for Log System Lock Issue
            //qCritical()<<"Read Default Keypad("<<m_nKeypadDefaultType<<") from Shared memory";
        else
            LOG2(QString("Failed to read Default Keypad from Shared memory"), Logger::PRIORITY_HI); //added for Log System Lock Issue
            //qCritical()<<"Failed to read Default Keypad from Shared memory";
    }

    if( m_pTranslatorLauncher == NULL )
        m_pTranslatorLauncher = new QTranslator( this );

    if( m_pTranslatorLauncher )
    {
        QString countryName = getCountryInfo(m_nLanguageType);
        QString file = QString("DHAVN_QwertyKeypad_Launch_%1").arg(countryName);
        if(m_pTranslatorLauncher->load(file, DIR_LINGUISTIC_DATA )){
            qApp->installTranslator( m_pTranslatorLauncher );

            LOG2(QString("Type[%1], country[%2], En[%3], Kor[%4]").arg(m_nLanguageType).arg(m_country).arg(m_nKeypadTypeEn).arg(m_nKeypadTypeKo), Logger::PRIORITY_HI);
            LOG2(QString("Ar[%1], Eu[%2], Rus[%3], Def[%4]").arg(m_nKeypadTypeAr).arg(m_nKeypadTypeEu).arg(m_nKeypadTypeRu).arg(m_nKeypadDefaultType), Logger::PRIORITY_HI);


            LOG2(QString("(O)TS File [%1]").arg(file), Logger::PRIORITY_HI); //added for Log System Lock Issue
            //qCritical()<<"(O) TS File Loading.."<<file;
            //m_mutex.unlock();
            return;
        }
        LOG2(QString("Type[%1], country[%2], En[%3], Kor[%4]").arg(m_nLanguageType).arg(m_country).arg(m_nKeypadTypeEn).arg(m_nKeypadTypeKo), Logger::PRIORITY_HI);
        LOG2(QString("Ar[%1], Eu[%2], Rus[%3], Def[%4]").arg(m_nKeypadTypeAr).arg(m_nKeypadTypeEu).arg(m_nKeypadTypeRu).arg(m_nKeypadDefaultType), Logger::PRIORITY_HI);

        LOG2(QString("(X)TS File [%1]").arg(file), Logger::PRIORITY_HI); //added for Log System Lock Issue
        //qCritical()<<"(X) TS File Loading.."<<file;
    }
    //m_mutex.unlock();

    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("end = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}

QString QwertyKeypadUtility::getCountryInfo( int nLanguageType )
{
    //qDebug() << "[LBG] "  << __FUNCTION__ << QString("start = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());

    switch(nLanguageType)
    {
    case LanguageKO:            return "KO";

    case LanguageZH_CN:
    case LanguageZH_YUE:        return "ZH-CN";     //< China

    case LanguagePT:            return "PT";        //< Europeen Portuguese
    case LanguageEN_GB:         return "EN-GB";     //< English (UK)

    case LanguageEN:
    case LanguageEN_US:         return "EN-US";        //< English (US)

    case LANGUAGE_FR_NA:
    case LanguageFR:            return "FR-EU";     //< European French

    case LanguageIT:            return "IT";        //< Italian
    case LanguageDE:            return "GE";        //< German

    case LANGUAGE_ES_NA:
    case LanguageES:            return "ES-EU";     //< European Spanish

    case LanguageRU:            return "RU";        //< Russian
    case LanguageNL:            return "NL";        //< Dutch
    case LanguageSV:            return "SV";        //< Swedish
    case LanguagePL:            return "PL";        //< Polish
    case LanguageTR:            return "TR";        //< Turkish
    case LanguageCS:            return "CS";        //< Czech
    case LanguageDA:            return "DA";        //< Danish
    case LanguageSK:            return "SK";        //< Slovakia
    case LanguageAR:            return "AR";        //< Arabic

    case LanguageUnkown:        return "KO";
    default:                    return "KO";        //< default
    }

    return "";
    //qDebug() << "[LBG] "  << __FUNCTION__ << QString("end = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}

QString QwertyKeypadUtility::currentTime()
{
    return QString("%1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}
void QwertyKeypadUtility::printLogMessage(QString logString)
{
    LOG2(QString("%1").arg(logString), Logger::PRIORITY_HI);
}

void QwertyKeypadUtility::HWRCurrentTime(bool isPressed)
{
    if(isPressed)
    {
        //qCritical() <<"[QT] HWR onPressed: " << QTime::currentTime().second() <<"." << QTime::currentTime().msec();
        LOG2(QString("[QT] HWR onPressed: %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec()), Logger::PRIORITY_HI); //added for Log System Lock Issue
    }
    else
    {
        //qCritical() <<"[QT] HWR onReleased: " << QTime::currentTime().second() <<"." << QTime::currentTime().msec();
        LOG2(QString("[QT] HWR onReleased: %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec()), Logger::PRIORITY_HI); //added for Log System Lock Issue
    }
}

QwertyKeypadAutomata* QwertyKeypadUtility::getAutomataInstance()
{
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("start = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());

    if( m_nCurrentAutomataID == AUTOMATA_NONE )
        return NULL;

    if(m_pAutomata != NULL)
    {
        if( m_pAutomata->getAutomataID() == m_nCurrentAutomataID )
            return m_pAutomata;
        else
        {
            delete m_pAutomata;
            m_pAutomata = NULL;
        }
    }

    // If you want to add other automata, then you shoul add other "case" statement
    switch( m_nCurrentAutomataID )
    {
    case AUTOMATA_KO:
    {
        m_pAutomata = new QwertyKeypadAutomataKorean();
        m_pAutomata->setAutomataID(AUTOMATA_KO);
        break;
    }

    default:
        break;
    }

    return m_pAutomata;

    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("end = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}

bool QwertyKeypadUtility::isNonCharacterKey(int nKeyCode)
{
    switch(nKeyCode)
    {
    case Qt::Key_CapsLock:
    case Qt::Key_Shift:
    case Qt::Key_Control:
    case Qt::Key_Launch0:
    case Qt::Key_Launch1:
    case Qt::Key_Launch2:
    case Qt::Key_Launch3:
    case Qt::Key_Launch4:
    case Qt::Key_Launch5:
    case Qt::Key_Launch6:
    case Qt::Key_Launch7:   // For change mode (1~2) in Number keypad
    case Qt::Key_Launch8:   // For change mode (1~3) in EU Latin Extended keypad
    case Qt::Key_Launch9:   // For Chinese Setting popup
    case Qt::Key_LaunchA:   // For change mode (1~2) in EU Cyrillic Extended keypad
    case Qt::Key_Home:
        return true;
    }

    return false;
}

bool QwertyKeypadUtility::isComposing()
{

    if( m_nCurrentAutomataID == AUTOMATA_NONE )
        return false;
    else
        return m_pAutomata->isComposing();
}

void QwertyKeypadUtility::clearAutomata()
{
    if( m_nCurrentAutomataID != AUTOMATA_NONE )
    {
        m_pAutomata->reset();
    }

    return;
}

QString QwertyKeypadUtility::makeWord(int nKeyCode, QString strKeyString)
{
    //qCritical() << "Utility::makeWord:: m_nCurrentAutomataID - " << m_nCurrentAutomataID;
    //qCritical() << "Utility::makeWord:: isNonCharacterKey(nKeyCode) - " << isNonCharacterKey(nKeyCode);
    if( m_nCurrentAutomataID == AUTOMATA_NONE )
        return strKeyString;
    else
    {
        if(isNonCharacterKey(nKeyCode))
            return strKeyString;
        else
        {
            m_sAutomataString = m_pAutomata->makeWord( nKeyCode, strKeyString );

            return m_sAutomataString;
        }
    }
}

void QwertyKeypadUtility::changeLang(QString screen, int nMode, int nShift)
{
    if( m_pTranslator == NULL )
        m_pTranslator = new QTranslator( this );

    if( m_pTranslator )
    {
        qApp->removeTranslator(m_pTranslator);
        QString file = QString("DHAVN_QwertyKeypad_%1_T%2_%3").arg(screen).arg(nMode)
                .arg(nShift?"UP":"DN");

        if(m_pTranslator->load( file, DIR_LINGUISTIC_DATA )){
            qApp->installTranslator( m_pTranslator );
            //qCritical()<<"(O) TS File Loading.."<<file;
            LOG2(QString("ChLang:(O)TS File [%1]").arg(file), Logger::PRIORITY_HI); //added for Log System Lock Issue
        }
        else{
            //qCritical()<<"(X) TS File Loading.."<<file;
            LOG2(QString("ChLang:(X)TS File [%1]").arg(file), Logger::PRIORITY_HI); //added for Log System Lock Issue
        }

        if(screen=="Korean")
            m_nCurrentAutomataID = AUTOMATA_KO;
        else
            m_nCurrentAutomataID = AUTOMATA_NONE;

        m_pAutomata = getAutomataInstance();
    }
}

QwertyKeypadUtility::Language QwertyKeypadUtility::getLanguageType() const{
    return m_nLanguageType;
}

QwertyKeypadUtility::CountryVariant QwertyKeypadUtility::getCountryVariant() const
{
    return m_country;
}

void QwertyKeypadUtility::setCountryVariant(CountryVariant cv)
{
    if(m_country == cv)
        return;

    m_country = cv;

    loadSettings();
}

void QwertyKeypadUtility::setLanguageType(Language lang)
{
    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("start = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());

    if(m_nLanguageType == lang)
        return;

    m_nLanguageType = lang;

    if( m_pTranslatorLauncher == NULL )
        m_pTranslatorLauncher = new QTranslator( this );

    if( m_pTranslatorLauncher )
    {
        QString countryName = getCountryInfo(m_nLanguageType);
        QString file = QString("DHAVN_QwertyKeypad_Launch_%1").arg(countryName);
        if(m_pTranslatorLauncher->load(file, DIR_LINGUISTIC_DATA )){
            qApp->installTranslator( m_pTranslatorLauncher );

            emit languageTypeChanged(true);
        }
    }

    emit languageTypeChanged(false);

    //qDebug()  << "[LBG] " << __FUNCTION__ << QString("end = %1.%2").arg(QTime::currentTime().second()).arg(QTime::currentTime().msec());
}

int QwertyKeypadUtility::getKeypadTypeEn(){
    return m_nKeypadTypeEn;
}

int QwertyKeypadUtility::getKeypadTypeKo(){
    return m_nKeypadTypeKo;
}

int QwertyKeypadUtility::getKeypadTypeLa(){
    return m_nKeypadTypeLa;
}

int QwertyKeypadUtility::getKeypadTypeAr(){
    return m_nKeypadTypeAr;
}

int QwertyKeypadUtility::getKeypadTypeCh(){
    return m_nKeypadTypeCh;
}

int QwertyKeypadUtility::getKeypadTypeEu(){
    return m_nKeypadTypeEu;
}

int QwertyKeypadUtility::getKeypadTypeRu(){
    return m_nKeypadTypeRu;
}

int QwertyKeypadUtility::getKeypadDefaultType(){
    return m_nKeypadDefaultType;
}

bool QwertyKeypadUtility::getFirstCapital(){
    return m_nFirstCapital;
}

QString QwertyKeypadUtility::automataString(){
    return m_sAutomataString;
}

QString QwertyKeypadUtility::getResourcePath()
{
#ifdef DESKTOP_WINDOWS
    return QString("../../DHAVN_ImageInstaller/res/");
#else
    return QString("/app/share/images/");
#endif
}

bool QwertyKeypadUtility::GetSharedMemory(int offset, unsigned char * pBuf, int size)
{
    if ( !m_pSharedMemory->isAttached() )
        return false;

    m_pSharedMemory->lock();
    memcpy( pBuf, ((unsigned char *)m_pSharedMemory->data())+offset, size );
    m_pSharedMemory->unlock();

    return true;
}

QwertyKeypadUtility::~QwertyKeypadUtility()
{
    if( m_pTranslator != NULL)
        delete m_pTranslator;

    if( m_pTranslatorLauncher != NULL)
        delete m_pTranslatorLauncher;

    if (m_pAutomata != NULL)
        delete m_pAutomata;

    if(m_pSharedMemory != NULL)
    {
        if(m_pSharedMemory->isAttached())
            m_pSharedMemory->detach();

        delete m_pSharedMemory;
    }
}
