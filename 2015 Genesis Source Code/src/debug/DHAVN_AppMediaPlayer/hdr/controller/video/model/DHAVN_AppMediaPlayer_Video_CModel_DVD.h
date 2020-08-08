#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_CMODEL_DVD_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_CMODEL_DVD_H

#include "DHAVN_AppMediaPlayer_Video_CModel_Base.h"

struct AudioLangData
{
    int audioNo; // modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
	// { modified by ravikanth CR 15994 12-11-22
    int audioCode; // modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
    int audioAttribute;
    int audioChannel;
	// } modified by ravikanth CR 15994 12-11-22
    int audioLFEExist; // added by cychoi 2013.07.20 for ITS 181110 Audio attribute
};

struct CaptionData
{
    //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
    int captionNo;
    int captionCode; // modified by ravikanth CR 15994 12-11-22
    //} modified by cychoi 2013.10.06
};

struct AngleData
{
    int angleNum; // modified by ravikanth CR 15994 12-11-22
};

// { modified by ravikanth CR 15994 12-11-22
struct MenuLangData
{
    int code; // modified by ravikanth 13-03-13
};
// } modified by ravikanth CR 15994 12-11-22

class AppMediaPlayer_Video_CModel_DVD : public AppMediaPlayer_Video_CModel_Base
{
    Q_OBJECT

public:
    AppMediaPlayer_Video_CModel_DVD( QObject *parent );
    virtual ~AppMediaPlayer_Video_CModel_DVD();

    int chapter() { return m_chapter; }
    void setChapter(int value);

    int title() { return m_title; }
    void setTitle(int value);

    //{ added by Michael.Kim 2013.06.07 for current focus index change
    int focusChapter() { return m_focusChapter; }
    void setFocusChapter(int value);

    int focusTitle() { return m_focusTitle; }
    void setFocusTitle(int value);
    //{ added by Michael.Kim 2013.06.07 for current focus index change

    int chapterCount() {return m_chapterCount;}
    void setChapterCount(int value);

    int titleCount() {return m_titleCount;}
    void setTitleCount(int value);

    // { modified by ravikanth CR 15994 12-11-22
    //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
    void setActiveAudioLang(int no, int code, int attribute, int channel, int LFEExist); // added by cychoi 2013.07.20 for ITS 181110 Audio attribute // modified by ravikanth CR 15994 12-11-22
    void setActiveCaption(int no, int code);
    //} modified by cychoi 2013.10.06
    void setActiveAngle(int index);
	// { modified by ravikanth 13-03-13
    void setActiveMenuLang(int code);
    void setActiveLangData(int captionLangCode, int audioLangCode, int menuLangCode);
	// } modified by ravikanth 13-03-13
    // removed by cychoi 2014.03.18 for ITS 229882 No option menu on Title/Disc Menu Screen // added by yongkyun.lee 2013-11-05 for ITS 206356
    QString activeCaption();
    QString activeAudioLang();
    QString activeAudioLang2();  // added by dongjin 2013.02.27 for ISV 71500
    int activeAngle();
    //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
    int activeCaptionCode() { return m_captionData.captionCode; }
    int activeAudioLangCode() { return m_audioLangData.audioCode; }
    //} modified by cychoi 2013.10.06
    // } modified by ravikanth CR 15994 12-11-22

    int activeAudioLangNo() { return m_audioLangData.audioNo; } // added by wspark 2014.01.21 for ITS 221677

    int resumePosition() {return m_PlayResumePosition;}
    void setResumePosition(int value);

    bool isStartedBySettingsApp() { return m_isStartedBySettingsApp; }
    void setIsStartedBySettingsApp( bool value );

    int playerState() { return m_playerState; }
    void setPlayerState(int value);

    int playerScreenState() { return m_playerScreenState; }
    void setPlayerScreenState(int value);

    QHash<int, QString> langCodes() { return m_langCodes; }
    QHash<int, QString> attrCodes() { return m_attrCodes; }
    QHash<int, QString> channelCodes() { return m_channelCodes; }
    QHash<int, QString> LFEChannelCodes() { return m_LFEChannelCodes; } // added by cychoi 2013.07.20 for ITS 181110 Audio attribute
    QList<int> defaulLanguageCodes() { return m_defaultLangCodes; } // modified by ravikanth 13-03-13
    QList<int> domesticLanguageCodes() { return m_domesticLangCodes; } // added by yungi 2013.10.19 for ITS 196375
    QList<int> arabLanguageCodes() { return m_arabLangCodes; } // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
    QList<int> chineseLanguageCodes() { return m_chineseLangCodes; } // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add

    // { added by wspark 2013.03.29 for ISV 77716
    void setListBtnVisible(bool value);
    bool listBtnVisible() { return m_bListBtnVisible; }
    // } added by wspark
    void retranslateUI(); // added by cychoi 2013.10.22 for ITS 197384

signals:
    void chapterChanged(int value);
    void titleChanged(int value);
    void focusChapterChanged(int value); //added by Michael.Kim 2013.06.07 for current focus index change
    void focusTitleChanged(int value); //added by Michael.Kim 2013.06.07 for current focus index change
    void chapterCountChanged(int value);
    void titleCountChanged(int value);
    void trackCountChanged(int value);// added by yongkyun.lee 20130626 for : disc OSD
    void audioLangDataChanged(AudioLangData*);
    void activeAudioLangChanged(int index);
    // { modified by dongjin 2013.02.27 for ISV 71500
    //void activeAudioLangStringChanged(QString audioLang); // modified by ravikanth CR 15994 12-11-22
    void activeAudioLangStringChanged(QString audioLang, QString audioLang2);
    // } modified by dongjin
    void captionDataChanged(CaptionData*);
    void activeCaptionChanged(int index);
    void activeCaptionStringChanged(QString captionLang); // modified by ravikanth CR 15994 12-11-22
    void angleDataChanged(AngleData*);
    void activeAngleChanged(int index);
    // removed by cychoi 2014.03.18 for ITS 229882 No option menu on Title/Disc Menu Screen // added by yongkyun.lee 2013-11-05 for ITS 206356
    void activeLangCodeChanged(int captionLangCode, int audioLangCode, int menuLangCode); // modified by ravikanth 13-03-13
    void isStartedBySettingsAppChanged (bool value);
    void playerStateChanged(int value);
    void playerScreenStateChanged(int value);
    void listBtnVisibleChanged(bool value); // added by wspark 2013.03.29 for ISV 77716


private:
    void initLangCodes();
    void initAttrCodes();
    void initChannelCodes();
    void initLFEChannelCodes(); // added by cychoi 2013.07.20 for ITS 181110 Audio attribute

    int             m_chapter;
    int             m_title;
    int             m_focusTitle;     // added by Michael.Kim 2013.06.07 for current focus index change
    int             m_focusChapter;    // added by Michael.Kim 2013.06.07 for current focus index change
    int             m_chapterCount;
    int             m_titleCount;
    AudioLangData   m_audioLangData;
    CaptionData     m_captionData;
    AngleData       m_angleData;
    MenuLangData    m_menuLangData; // modified by ravikanth CR 15994 12-11-22
    int m_PlayResumePosition;
    bool            m_isStartedBySettingsApp;
    int             m_playerState;
    int             m_playerScreenState;

    QHash<int, QString> m_langCodes;
    QHash<int, QString> m_attrCodes;
    QHash<int, QString> m_channelCodes;
    QHash<int, QString> m_LFEChannelCodes; // added by cychoi 2013.07.20 for ITS 181110 Audio attribute
    QString m_audioLangString; // modified by ravikanth CR 15994 12-11-22
    QString m_audioLangString2; // added by dongjin 2013.02.27 for ISV 71500
    QList<int> m_defaultLangCodes; // modified by ravikanth 13-03-13
    QList<int> m_domesticLangCodes; // added by yungi 2013.10.19 for ITS 196375
    QList<int> m_arabLangCodes; // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
    QList<int> m_chineseLangCodes; // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add

    bool            m_bListBtnVisible; // added by wspark 2013.03.29 for ISV 77716
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_CMODEL_DVD_H
