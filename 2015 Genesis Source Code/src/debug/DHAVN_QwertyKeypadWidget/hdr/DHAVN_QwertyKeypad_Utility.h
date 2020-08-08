#ifndef QWERTYKEYPADUTILITY_H
#define QWERTYKEYPADUTILITY_H

#include <QtDeclarative/QDeclarativeItem>
#include <QObject>
#include <QMutex>
#include <QTranslator>
#include <QSharedMemory>
#include "DHAVN_QwertyKeypad_Automata.h"

#ifndef DESKTOP_WINDOWS
#include <persistman.h>
#endif

#include <DHAVN_AppSettings_Def.h>

//added for ITS 247594 Long Touch Beep Issue
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusInterface>

const quint16 c_CntlAudioBeepCommand = 0x0115;
//added for ITS 247594 Long Touch Beep Issu

class QwertyKeypadUtility : public QObject
{
    Q_OBJECT
    Q_ENUMS(CountryVariant)
    Q_ENUMS(Language)
public:
    enum AUTOMATA_ID
    {
        AUTOMATA_NONE = 0,
        AUTOMATA_KO   = 1
    };

    enum CountryVariant{
        CountryVariantKorea         = 0,
        CountryVariantNorthAmerica  =  1,
        CountryVariantChina         =  2,
        CountryVariantGeneral       =  3,
        CountryVariantMiddleEast    =  4,
        CountryVariantEuropa        =  5,
        CountryVariantCanada        =  6,
        CountryVariantRussian       =  7,
        CountryVariantAustralia     = 160, //added for AUS Variant BT Keypad Black Screen Issue
        CountryVariantInvalidMax    =  8
    };

    enum Language{
        LanguageUnkown = 0,
        LanguageEN,
        LanguageKO,
        LanguageZH_CN,  //< Peoples Republic Of China (Mandarin)
        LanguageZH_YUE, //< Peoples Republic Of China (Cantonese)
        LanguagePT,     //< Europeen Portuguese
        LanguageEN_GB,  //< English (UK)
        LanguageEN_US,  //< English (US)
        LanguageFR,     //< European French
        LanguageIT,     //< Italian
        LanguageDE,     //< German            10
        LanguageES,     //< European Spanish
        LanguageRU,     //< Russian
        LanguageNL,     //< Dutch
        LanguageSV,     //< Swedish
        LanguagePL,     //< Polish
        LanguageTR,     //< Turkish
        LanguageCS,     //< Czech
        LanguageDA,     //< Danish
        LanguageSK,     //< Slovakia
        LanguageAR,     //< Arabic            20
        LANGUAGE_FR_NA, //=  LANGUAGE_FR_NA    //< French (US)
        LANGUAGE_ES_NA //=  LANGUAGE_ES_NA    //< Spanish (US)
    };


    explicit QwertyKeypadUtility(QObject *parent = 0);
    ~QwertyKeypadUtility();

    Q_INVOKABLE void    changeLang(QString screen, int nMode, int nShift);
    Q_INVOKABLE QString makeWord(int nKeyCode, QString strKeyString);
    Q_INVOKABLE bool    isComposing();
    Q_INVOKABLE void    clearAutomata();

    Q_INVOKABLE QString    currentTime();
    Q_INVOKABLE void HWRCurrentTime(bool isPressed); //added for HWR Input issue press and release Time

    Q_INVOKABLE void printLogMessage(QString logString); //added for QML Log Message

    Q_INVOKABLE QString getResourcePath();

    Q_PROPERTY( Language languageType READ getLanguageType WRITE setLanguageType NOTIFY languageTypeChanged)
    Q_PROPERTY( CountryVariant country READ getCountryVariant WRITE setCountryVariant)
    Q_PROPERTY( int keypadTypeEn READ getKeypadTypeEn WRITE setKeypadTypeEn NOTIFY keypadTypeEnChanged)
    Q_PROPERTY( int keypadTypeKo READ getKeypadTypeKo WRITE setKeypadTypeKo NOTIFY keypadTypeKoChanged)
    Q_PROPERTY( int keypadTypeLa READ getKeypadTypeLa WRITE setKeypadTypeLa NOTIFY keypadTypeLaChanged)
    Q_PROPERTY( int keypadTypeAr READ getKeypadTypeAr WRITE setKeypadTypeAr NOTIFY keypadTypeArChanged)
    Q_PROPERTY( int keypadTypeCh READ getKeypadTypeCh WRITE setKeypadTypeCh NOTIFY changedChineseKeypadType)
    Q_PROPERTY( int keypadTypeEu READ getKeypadTypeEu WRITE setKeypadTypeEu NOTIFY keypadTypeEuChanged)
    Q_PROPERTY( int keypadTypeRu READ getKeypadTypeRu WRITE setKeypadTypeRu NOTIFY keypadTypeRuChanged)
    Q_PROPERTY( int keypadDefaultType READ getKeypadDefaultType WRITE setKeypadDefaultType NOTIFY keypadDefaultTypeChanged)
    Q_PROPERTY( bool isFirstCapital READ getFirstCapital WRITE setIsFirstCapital NOTIFY isFirstCapitalChanged)
    Q_PROPERTY( QString automataString READ automataString WRITE setAutomataString NOTIFY automataStringChanged)

    Language getLanguageType() const;

    CountryVariant getCountryVariant() const;
    void setCountryVariant(CountryVariant cv);
    void setLanguageType(Language lang);
    int getKeypadTypeEn();
    int getKeypadTypeKo();
    int getKeypadTypeLa();
    int getKeypadTypeAr();
    int getKeypadTypeCh();
    int getKeypadTypeEu();
    int getKeypadTypeRu();
    int getKeypadDefaultType();
    bool getFirstCapital();
    QString automataString();

    void setKeypadTypeEn(int value){ m_nKeypadTypeEn = value; emit keypadTypeEnChanged();}
    void setKeypadTypeKo(int value){ m_nKeypadTypeKo = value; emit keypadTypeKoChanged();}
    void setKeypadTypeLa(int value){ m_nKeypadTypeLa = value; emit keypadTypeLaChanged();}
    void setKeypadTypeAr(int value){ m_nKeypadTypeAr = value; emit keypadTypeArChanged();}
    void setKeypadTypeCh(int value){ m_nKeypadTypeCh = value; emit changedChineseKeypadType();}
    void setKeypadTypeEu(int value){ m_nKeypadTypeEu = value; emit keypadTypeEuChanged();}
    void setKeypadTypeRu(int value){ m_nKeypadTypeRu = value; emit keypadTypeRuChanged();}
    void setKeypadDefaultType(int value){ m_nKeypadDefaultType = value; emit keypadDefaultTypeChanged();}

    void setIsFirstCapital(bool value){ m_nFirstCapital = value; emit isFirstCapitalChanged();}
    void setAutomataString(QString value){ m_sAutomataString = value; emit automataStringChanged();}

    //added for ITS 247594 Long Touch Beep Issue
    void connectDBUSInterface();
    Q_INVOKABLE void callAudioBeepCommand();
    QDBusConnection conn;
    QDBusMessage soundSetupControl;
    //added for ITS 247594 Long Touch Beep Issue

private:
    QwertyKeypadAutomata * getAutomataInstance();
    bool isNonCharacterKey(int nKeyCode);

private:
    QTranslator* m_pTranslator;
    QTranslator* m_pTranslatorLauncher;
    AUTOMATA_ID  m_nCurrentAutomataID;
    Language     m_nLanguageType;
    QwertyKeypadAutomata* m_pAutomata;
    CountryVariant        m_country;
    QMutex                m_mutex;

    int m_nKeypadTypeEn;
    int m_nKeypadTypeKo;
    int m_nKeypadTypeLa;
    int m_nKeypadTypeAr;
    int m_nKeypadTypeCh;
    int m_nKeypadTypeEu;
    int m_nKeypadTypeRu;
    int m_nKeypadDefaultType;

    bool m_nFirstCapital;

    QString m_sAutomataString;
    QSharedMemory *m_pSharedMemory;

private:
    QString getCountryInfo( int nLanguageType );
    bool GetSharedMemory(int offset, unsigned char * pBuf, int size);

signals:
    void keypadTypeEnChanged();
    void keypadTypeKoChanged();
    void keypadTypeLaChanged();
    void keypadTypeArChanged();
    void changedChineseKeypadType();
    void keypadTypeEuChanged();
    void keypadTypeRuChanged();
    void keypadDefaultTypeChanged();

    void savedNewChineseKeypadType();
    void languageTypeChanged(bool isChanged);
    void isFirstCapitalChanged();
    void automataStringChanged();

public slots:
    void    loadSettings(void);
};


#endif // QWERTYKEYPADUTILITY_H
