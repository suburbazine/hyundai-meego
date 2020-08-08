#ifndef DECKENGINEERMODE_H
#define DECKENGINEERMODE_H

#include <QObject>

class EDeckInit1Setting
{
public:
    EDeckInit1Setting() 
    {
		m_bOSDOff = 0;
    	m_nRegionCode = 0;
    	m_nParentalLevel = 0;
    	m_nPrioSubTitleCode = 0;
   		m_nPrioAudioCode = 0;
    	m_nPrioMenuLangCode = 0;
    	m_nScreenSize = 0;
    	m_nAngleDisplay = 0;
    	m_nSubTitleDisplay = 0;
    	m_nPinNumber = 0;
    	m_nOpenningScreen = 0;
    	m_nScreenMode = 0;
    	m_nPictureMode = 0;
    	m_nPBC = 0;
    	m_nIRE = 0;
    	m_nBackColor = 0;
    	m_nCountryCode = 0;
    	m_nPlayerMode = 0;
    	m_nVideoOutput = 0;
    }

    int                         m_bOSDOff;
    int                         m_nRegionCode;
    int                         m_nParentalLevel;
    int                         m_nPrioSubTitleCode;
    int                         m_nPrioAudioCode;
    int                         m_nPrioMenuLangCode;
    int                         m_nScreenSize;
    int                         m_nAngleDisplay;
    int                         m_nSubTitleDisplay;
    int                         m_nPinNumber;
    int                         m_nOpenningScreen;
    int                         m_nScreenMode;
    int                         m_nPictureMode;
    int                         m_nPBC;
    int                         m_nIRE;
    int                         m_nBackColor;
    int                         m_nCountryCode;
    int                         m_nPlayerMode;
    int                         m_nVideoOutput;
};

class EDeckInit2aSetting
{
public:
    EDeckInit2aSetting() 
    {
        m_nAudioOutput = 0;
    	m_nDualMono = 0;
		m_nPCMSetting = 0;
		m_nDSPIFDownMix = 0;
		m_n2chIFDownMix = 0;
	    m_nFrontSp = 0;
    	m_nCenterSp = 0;
        m_nRearSp = 0;
        m_nSubwoofer = 0;
        m_nCenterSpDelay = 0;
	    m_nRearSpDelay = 0;
    	m_nCompLevel = 0;
		m_nFrontLSpLevel = 0;
		m_nFrontRSpLevel = 0;
		m_nRearLSpLevel = 0;
		m_nRearRSpLevel = 0;
    	m_nCenterSpLevel = 0;
		m_nSubwooferLevel = 0;
		m_nDTSOutput = 0;
		m_n6chStereo = 0;
		m_nBaseMngr = 0;
		m_nDownSampling = 0;
		m_nDolPro12 = 0;
		m_nCenterBlendLvl = 0;
		m_nDimension = 0;
		m_nPanorama = 0;
	    m_nCenterWidth = 0; 
    }

    int                             m_nAudioOutput;
    int                             m_nDualMono;
    int                             m_nPCMSetting;
    int                             m_nDSPIFDownMix;
    int                             m_n2chIFDownMix;
    int                             m_nFrontSp;
    int                             m_nCenterSp;
    int                             m_nRearSp;
    int                             m_nSubwoofer;
    int                             m_nCenterSpDelay;
    int                             m_nRearSpDelay;
    int                             m_nCompLevel;
    int                             m_nFrontLSpLevel;
    int                             m_nFrontRSpLevel;
    int                             m_nRearLSpLevel;
    int                             m_nRearRSpLevel;
    int                             m_nCenterSpLevel;
    int                             m_nSubwooferLevel;
    int                             m_nDTSOutput;
    int                             m_n6chStereo;
    int                             m_nBaseMngr;
    int                             m_nDownSampling;
    int                             m_nDolPro12;
    int                             m_nCenterBlendLvl;
    int                             m_nDimension;
    int                             m_nPanorama;
    int                             m_nCenterWidth;
};

class EDeckInit3Setting
{
public:
    EDeckInit3Setting() 
    {
		m_nNoFileFolder = 0;
		m_nPriorityID3Tag = 0;
		m_nMP3Play = 0;
		m_nWMAPlay = 0;
		m_nPVDSVD = 0;
		m_nRandomTrackUp = 0;
        m_nRepeatCancel = 0;
        m_nRandomCancel = 0;
        m_nFastUpPlay = 0;
        m_nFastDownPlay = 0;
        m_nTrackDownTime = 0;
        m_nSingleSession = 0;
        m_nDVDAudioPlay = 0;
        m_nHaltByHighTemp = 0;
        m_nTitleEnd = 0;
        m_nEjectRsume = 0;
        m_nDirPriority = 0;
        m_nPlayTimeDisplay = 0;
    }

    int                             m_nNoFileFolder;
    int                             m_nPriorityID3Tag;
    int                             m_nMP3Play;
    int                             m_nWMAPlay;
    int                             m_nPVDSVD;
    int                             m_nRandomTrackUp;
    int                             m_nRepeatCancel;
    int                             m_nRandomCancel;
    int                             m_nFastUpPlay;
    int                             m_nFastDownPlay;
    int                             m_nTrackDownTime;
    int                             m_nSingleSession;
    int                             m_nDVDAudioPlay;
    int                             m_nHaltByHighTemp;
    int                             m_nTitleEnd;
    int                             m_nEjectRsume;
    int                             m_nDirPriority;
    int                             m_nPlayTimeDisplay;
};

class DeckEngineerMode : public QObject
{
    Q_OBJECT
public:
    DeckEngineerMode(QObject *parent = 0);
    ~DeckEngineerMode();

    // FOR HARDWARE MENUS
    int     GetFirmwareVersion();
    int     GetLifeTime();
    int     GetTemperature();
    int     GetEjectStatus();
    int     GetInsertStatus();
    int     GetRandomStatus();
    int     GetRepeatStatus();
    int     GetScanStatus();
    int     GetMechError();
    int     GetReadError();

    // FOR VARIANT MENUS
    int     GetRegionCode();
    int     SetRegionCode(int nCode);
    int     GetMenuLanguage();
    int     SetMenuLanguage(int Code);
    
    // FOR DECK SETTING
    int     SetInitSetting1(const EDeckInit1Setting &tSetting);
    int     SetInitSetting2a(const EDeckInit2aSetting &tSetting);
    int     SetInitSetting3(const EDeckInit3Setting &tSetting);
    int     GetInitSetting1(EDeckInit1Setting &tSetting);
    int     GetInitSetting2a(EDeckInit2aSetting &tSetting);
    int     GetInitSetting3(EDeckInit3Setting &tSetting);

    // FOR DIAGNOSTIC
    void    StartDiagnostic();
    void    EndDiagnostic();

    void    DiscInOutTest();  //added by wonseok.heo for in out test

signals:
    void    OnCommErrorWithDeckChanged(int bError);

private slots:
    void    HandleCommErrorWithDeckChanged(int bError);

private:
    int     m_nResetCount;
    int     m_bStartDiagnostic;
    int     m_pTempTable[256];
};

#endif // DECKENGINEERMODE_H
