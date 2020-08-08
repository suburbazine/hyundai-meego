#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_DVDSETTINGS_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_DVDSETTINGS_H

#include <QStringList>
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_DVD.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h" // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add

enum TAB_T
{
    TAB_CAPTION = 0,
    TAB_AUDIO,
//    TAB_ANGLE,
    TAB_MENUL, // modified by ravikanth 13-03-13
    TAB_DIVX
};

class AppMediaPlayer_Video_CModel_DVD;

class DVDSettingsSModel : public QObject
{
    Q_OBJECT

	// { modified by ravikanth 13-03-13
    Q_PROPERTY(int  activeTab       READ activeTab      NOTIFY activeTabChanged);
    Q_PROPERTY(int  activeCaption   READ activeCaption  NOTIFY activeCaptionChanged)
    Q_PROPERTY(int  activeAudioLang   READ activeAudioLang  NOTIFY activeAudioLangChanged)
    Q_PROPERTY(int  activeMenuLang   READ activeMenuLang  NOTIFY activeMenuLangChanged)
	// } modified by ravikanth 13-03-13

public:
     DVDSettingsSModel(AppMediaPlayer_Video_CModel_DVD *pCModel, QObject *parent = 0, AppMediaPlayer_EngineVideo *engineVideo = NULL); // modified by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
    ~DVDSettingsSModel();

    void bindWith(AppMediaPlayer_Video_CModel_DVD *target);
    QStringList* getCaptionList() { return &m_captionStringList; }
    QStringList* getAudioLangList() { return &m_audioLangStringList; }
    QStringList* getAngleList() { return &m_angleStringList; }
    QStringList* getMenuLangList() { return &m_menuStringList; } // modified by ravikanth 13-03-13

    int activeCaption() { return m_activeCaption; }
    int activeAudioLang() { return m_activeAudioLang; }
    int activeAngle() { return m_activeAngle; }
    int activeMenuLang() { return m_activeMenuLang; } // modified by ravikanth 13-03-13

    int activeTab() { return m_activeTab; }

    AppMediaPlayer_EngineVideo * getEngineVideo() const {return m_EngineVideo;} // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add

    void retranslateUI(); // added by cychoi 2013.12.04 for ITS 212522

	// { modified by ravikanth 13-03-13
private:
    void updateCaptionAndMenuList(QStringList *list, bool isCaption, int isLanguage); // modified by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
    void compareKeyByCode();
	// } modified by ravikanth 13-03-13

public slots:
    void setActiveCaption(int index);
    void setActiveAudioLang(int index);
    void setActiveAngle(int index);
    void setActiveMenuLang(int index); // modified by ravikanth 13-03-13
    void setIsDVDDimmed(bool value);
    void setActiveTab(int value);
    void activeLangCodeChanged(int captionCode, int audioLangCode, int menuLangCode); // modified by ravikanth 13-03-13

signals:
    void captionDataChanged(QStringList *list, int index);
    void audioLangDataChanged(QStringList *list, int index);
    void angleDataChanged(QStringList *list, int index);
    void isDVDDimmedChanged(bool value);
    void activeTabChanged(bool value);
	
	// { modified by ravikanth 13-03-13
    void activeCaptionChanged(int index);
    void activeAudioLangChanged(int index);
    void activeMenuLangChanged(int index);
	// } modified by ravikanth 13-03-13

private:
    AppMediaPlayer_Video_CModel_DVD *m_pCModel;
    AppMediaPlayer_EngineVideo *m_EngineVideo; // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
    QStringList m_audioLangStringList;
    QStringList m_captionStringList;
    QStringList m_angleStringList;
    QStringList m_menuStringList; // modified by ravikanth 13-03-13

    int m_activeAudioLang;
    int m_activeCaption;
    int m_activeAngle;
    int m_activeMenuLang; // modified by ravikanth 13-03-13

    int m_activeTab;
	
	// { modified by ravikanth 13-03-13
    QStringList *m_defaultCaptionList;
    QStringList *m_defaultMenuStringList;

    QList<int> m_defaultLangCodes;
    QHash<QString,int> m_compareLangCodes;
    bool isCaption;
    int isLanguage; // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
	// } modified by ravikanth 13-03-13
};


#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_DVDSETTINGS_H

