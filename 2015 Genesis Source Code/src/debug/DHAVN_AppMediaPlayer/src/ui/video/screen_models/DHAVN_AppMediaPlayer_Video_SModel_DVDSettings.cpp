#include "hdr/ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_DVDSettings.h"


DVDSettingsSModel::DVDSettingsSModel(AppMediaPlayer_Video_CModel_DVD *pCModel, QObject *parent, AppMediaPlayer_EngineVideo *engineVideo) // modified by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
// { modified by eugene 2012.08.22 for static analysis defect #7474
    : QObject(parent),
    m_activeAudioLang(0),
    m_activeCaption(0),
    m_activeAngle(0),
    m_EngineVideo( engineVideo ), // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
    m_activeMenuLang(0)
// } modified by eugene 2012.08.22 for static analysis defect #7474
{
    m_activeTab = TAB_CAPTION; // modified by kihyung 2012.08.19 for 3PDMS 0136476
    m_pCModel = pCModel;
    isCaption = false; //modified by edo.lee 2013.07.15 STATIC TEST
    isLanguage = m_EngineVideo->getEngineMain().GetLanguage(); // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add


    m_defaultCaptionList = new QStringList();
    m_defaultMenuStringList = new QStringList();

    updateCaptionAndMenuList(m_defaultCaptionList, true, isLanguage); // modified by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
    updateCaptionAndMenuList(m_defaultMenuStringList, false, isLanguage); // modified by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add

    m_captionStringList = *m_defaultCaptionList;
    m_menuStringList = *m_defaultMenuStringList;
    m_audioLangStringList = *m_defaultMenuStringList;
    compareKeyByCode();
}

DVDSettingsSModel::~DVDSettingsSModel()
{
    delete m_defaultCaptionList;
    delete m_defaultMenuStringList;
}


void DVDSettingsSModel::bindWith(AppMediaPlayer_Video_CModel_DVD *target)
{
//    connect(target, SIGNAL(captionDataChanged(CaptionData*)),
//            this, SLOT(parseCaptionData(CaptionData*)));
//    connect(target, SIGNAL(activeCaptionChanged(int)),
//            this, SLOT(setActiveCaption(int)));

//    connect(target, SIGNAL(audioLangDataChanged(AudioLangData*)),
//            this, SLOT(parseAudioLangData(AudioLangData*)));
//    connect(target, SIGNAL(activeAudioLangChanged(int)),
//            this, SLOT(setActiveAudioLang(int)));

//    connect(target, SIGNAL(angleDataChanged(AngleData*)),
//            this, SLOT(parseAngleData(AngleData*)));
//    connect(target, SIGNAL(activeAngleChanged(int)),
//            this, SLOT(setActiveAngle(int)));
    connect(target, SIGNAL(activeLangCodeChanged(int,int,int)),
            this, SLOT(activeLangCodeChanged(int,int,int)));
}

void DVDSettingsSModel::activeLangCodeChanged(int captionCode, int audioLangCode, int menuLangCode)
{
    /** Caption */
    m_activeCaption = m_captionStringList.indexOf(m_compareLangCodes.key(captionCode));
    if(m_activeCaption == -1 && captionCode > 0)
    {
        m_activeCaption = m_captionStringList.count() - 1;
    }

    //emit activeCaptionChanged(m_activeCaption);  // commented by cychoi 2013.06.24 for new DVD Settings UX

    /** Audio */
    m_activeAudioLang = m_audioLangStringList.indexOf(m_compareLangCodes.key(audioLangCode));
    if(m_activeAudioLang == -1 && audioLangCode > 0)
    {
        m_activeAudioLang = m_audioLangStringList.count() - 1;
    }

    //emit activeAudioLangChanged(m_activeAudioLang);  // commented by cychoi 2013.06.24 for new DVD Settings UX

    /** Menu */
    m_activeMenuLang = m_menuStringList.indexOf(m_compareLangCodes.key(menuLangCode));
    if(m_activeMenuLang == -1 && menuLangCode > 0)
    {
        m_activeMenuLang = m_menuStringList.count() - 1;
    }

    //emit activeMenuLangChanged(m_activeMenuLang);  // commented by cychoi 2013.06.24 for new DVD Settings UX
}

void DVDSettingsSModel::setActiveCaption(int index)
{
     m_activeCaption = index;
     //emit activeCaptionChanged(index); // commented by cychoi 2013.06.24 for new DVD Settings UX
     //emit captionDataChanged(&m_captionStringList, m_activeCaption); //removed by ruindmby 2012.08.11 for CR 12455
}


void DVDSettingsSModel::setActiveAudioLang(int index)
{
     m_activeAudioLang = index;
     // emit activeAudioLangChanged(index); // commented by cychoi 2013.06.24 for new DVD Settings UX
     //emit audioLangDataChanged(&m_audioLangStringList, m_activeAudioLang); //removed by ruindmby 2012.08.11 for CR 12455
}


void DVDSettingsSModel::setActiveAngle(int index)
{
     m_activeAngle = index;
     //emit angleDataChanged(&m_angleStringList, m_activeAngle); //removed by ruindmby 2012.08.11 for CR 12455
}

void DVDSettingsSModel::setActiveMenuLang(int index)
{
     m_activeMenuLang = index;
     //emit activeMenuLangChanged(index); // commented by cychoi 2013.06.24 for new DVD Settings UX
     //emit angleDataChanged(&m_angleStringList, m_activeMenuLang); //removed by ruindmby 2012.08.11 for CR 12455
}

void DVDSettingsSModel::setIsDVDDimmed(bool value)
{
    MP_LOG << "DVD_Settings value =" << value << LOG_ENDL;
}


void DVDSettingsSModel::setActiveTab(int value)
{
    MP_LOG << "DVD_Settings value =" << value << LOG_ENDL;

    m_activeTab = value;
    emit activeTabChanged(m_activeTab);
}

// { modified by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
void DVDSettingsSModel::updateCaptionAndMenuList(QStringList *list, bool isCaption, int isLanguage)
{
    switch(isLanguage)
    {
        case LANGUAGE_KO:
        {
            if(isCaption)
                list->append(tr("STR_SETTING_DVD_NO_CAPTION"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_KOR"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_ENG"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_CHN"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_JPN"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_GER"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_FRA"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_ESP"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_RUS"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_ARB"));
            list->append(tr("STR_MEDIA_OTHERS"));
            break;
        }
        case LANGUAGE_ZH_CN:
        case LANGUAGE_ZH_YUE:
        {
            if(isCaption)
                list->append(tr("STR_SETTING_DVD_NO_CAPTION"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_CHN"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_ENG"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_GER"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_ESP"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_FRA"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_RUS"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_KOR"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_JPN"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_ARB"));
            list->append(tr("STR_MEDIA_OTHERS"));
            break;
        }
        case LANGUAGE_AR:
        {
            if(isCaption)
                list->append(tr("STR_SETTING_DVD_NO_CAPTION"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_ARB"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_ENG"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_GER"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_ESP"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_FRA"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_RUS"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_KOR"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_CHN"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_JPN"));
            list->append(tr("STR_MEDIA_OTHERS"));
            break;
        }
        default:
        {
            if(isCaption)
                list->append(tr("STR_SETTING_DVD_NO_CAPTION"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_ENG"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_GER"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_ESP"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_FRA"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_RUS"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_KOR"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_CHN"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_JPN"));
            list->append(tr("STR_SETTING_DVD_CAPTION_LANG_ARB"));
            list->append(tr("STR_MEDIA_OTHERS"));
            break;
        }
    }
}
// } modified by yungi

void DVDSettingsSModel::compareKeyByCode()
{
    // All Langauages
    m_compareLangCodes.insert(tr("STR_SETTING_DVD_NO_CAPTION"),0);
    m_compareLangCodes.insert(tr("STR_SETTING_DVD_CAPTION_LANG_KOR"),1115);
    m_compareLangCodes.insert(tr("STR_SETTING_DVD_CAPTION_LANG_ENG"),514);
    m_compareLangCodes.insert(tr("STR_SETTING_DVD_CAPTION_LANG_CHN"),2608); //////////// 2008
    m_compareLangCodes.insert(tr("STR_SETTING_DVD_CAPTION_LANG_JPN"),1001);
    m_compareLangCodes.insert(tr("STR_SETTING_DVD_CAPTION_LANG_FRA"),618);  /////////////612
    m_compareLangCodes.insert(tr("STR_SETTING_DVD_CAPTION_LANG_ESP"),519);  // modified by cychoi 2013.07.02 for ITS 177614 SubTitle language /////////////514
    m_compareLangCodes.insert(tr("STR_SETTING_DVD_CAPTION_LANG_GER"),405);
    m_compareLangCodes.insert(tr("STR_SETTING_DVD_CAPTION_LANG_RUS"),1821); // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
    m_compareLangCodes.insert(tr("STR_SETTING_DVD_CAPTION_LANG_ARB"),118);  // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
}

// { added by cychoi 2013.12.04 for ITS 212522
void DVDSettingsSModel::retranslateUI()
{
    isLanguage = m_EngineVideo->getEngineMain().GetLanguage(); // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add

    m_defaultCaptionList->clear();
    m_defaultMenuStringList->clear();

    updateCaptionAndMenuList(m_defaultCaptionList, true, isLanguage); // modified by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
    updateCaptionAndMenuList(m_defaultMenuStringList, false, isLanguage); // modified by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add

    m_captionStringList = *m_defaultCaptionList;
    m_menuStringList = *m_defaultMenuStringList;
    m_audioLangStringList = *m_defaultMenuStringList;
}
// } added by cychoi 2013.12.04

