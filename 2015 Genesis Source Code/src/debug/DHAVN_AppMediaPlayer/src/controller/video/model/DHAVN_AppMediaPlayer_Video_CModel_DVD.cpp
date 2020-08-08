#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_DVD.h"
#include <QTranslator>


AppMediaPlayer_Video_CModel_DVD::AppMediaPlayer_Video_CModel_DVD( QObject *parent )
    : AppMediaPlayer_Video_CModel_Base( parent )
{
    m_chapter = 0;
    m_title = 0;

    m_focusChapter = 0;
    m_focusTitle = 0; //added by Michael.Kim 2013.06.07 for current focus index change

    m_chapterCount = 0;
    m_titleCount = 0;

    //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
    m_captionData.captionNo = 1; // Relevant No
    m_captionData.captionCode = 0; // No Code
    m_audioLangData.audioNo = 1; // Relevant No
    //} modified by cychoi 2013.10.06
    // { modified by cychoi 2013.07.20 for ITS 181110 Audio attribute
    m_audioLangData.audioAttribute = 7; // Not applicable
    m_audioLangData.audioChannel = 15;  // Null
    //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
    m_audioLangData.audioCode = 0;       // No Code
    //} modified by cychoi 2013.10.06
    m_audioLangData.audioLFEExist = false;
    // } modified by cychoi 2013.07.20
    m_angleData.angleNum = 0;
    m_PlayResumePosition = 0;

    m_isStartedBySettingsApp = false;

    initLangCodes();
    initAttrCodes();
    initChannelCodes();
    initLFEChannelCodes(); // added by cychoi 2013.07.20 for ITS 181110 Audio attribute

	// { modified by eugene 2012.08.22 for static analysis defect #7473
    m_playerState = 0;
    m_playerScreenState = 0;
	// } modified by eugene 2012.08.22 for static analysis defect #7473

    m_bListBtnVisible = true; // added by wspark 2013.03.29 for ISV 77716

    m_menuLangData.code = 0; // added by sungha.choi 2013.07.15 for static test
}


AppMediaPlayer_Video_CModel_DVD::~AppMediaPlayer_Video_CModel_DVD()
{
}


void AppMediaPlayer_Video_CModel_DVD::setChapter( int value )
{
    MP_MEDIUM << "value =" << value << LOG_ENDL;
    m_chapter = value;
    emit chapterChanged(value);
}

// {added by Michael.Kim 2013.06.07 for current focus index change
void AppMediaPlayer_Video_CModel_DVD::setFocusChapter( int value )
{
    MP_MEDIUM << "value =" << value << LOG_ENDL;
    m_focusChapter = value;
    emit focusChapterChanged(value);
}
// {added by Michael.Kim 2013.06.07 for current focus index change

void AppMediaPlayer_Video_CModel_DVD::setTitle( int value )
{
    MP_MEDIUM << "value =" << value << LOG_ENDL;
    m_title = value;
    emit titleChanged(value);
}

// {added by Michael.Kim 2013.06.07 for current focus index change
void AppMediaPlayer_Video_CModel_DVD::setFocusTitle( int value )
{
    MP_MEDIUM << "value =" << value << LOG_ENDL;
    m_focusTitle = value;
    emit titleChanged(value);
}
// {added by Michael.Kim 2013.06.07 for current focus index change

void AppMediaPlayer_Video_CModel_DVD::setChapterCount( int value )
{
    m_chapterCount = value;
    emit chapterCountChanged(value);
    emit trackCountChanged(value);// added by yongkyun.lee 201300702 for : disc OSD
}


void AppMediaPlayer_Video_CModel_DVD::setTitleCount( int value )
{
    // { added by junggil 2012.08.30 for CR11408 [Video DVD] Highlight is wrong when using Jog Dial on Title List screen for Video DVD.
    if(m_titleCount != value)
    {
    // } added by junggil 
        m_titleCount = value;
        emit titleCountChanged(value);
        //emit trackCountChanged(value);// removed by yongkyun.lee 201300702 for : disc OSD
    } // added by junggil 2012.08.30 for CR11408 [Video DVD] Highlight is wrong when using Jog Dial on Title List screen for Video DVD.
}


//{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
void AppMediaPlayer_Video_CModel_DVD::setActiveCaption(int no, int code)
{
    MP_MEDIUM << "code =" << code << LOG_ENDL;
    m_captionData.captionNo = no;
	// { modified by ravikanth CR 15994 12-11-22
    m_captionData.captionCode = code;
    if(no == 0)
    {
        // Relevant No == 0 means "No Caption"
        emit activeCaptionStringChanged(m_langCodes.value(0));
    }
    else
    {
        // Code == 0 means "No Code"
        emit activeCaptionStringChanged(QString::number(no) + QString(". ") + m_langCodes.value(code == 0 ? 1 : code));
    }
	// } modified by ravikanth CR 15994 12-11-22
}
//} modified by cychoi 2013.10.06

// { modified by ravikanth 13-03-13
void AppMediaPlayer_Video_CModel_DVD::setActiveMenuLang(int code)
{
    MP_MEDIUM << "code =" << code << LOG_ENDL;
    m_menuLangData.code = code;
//    emit activeMenuLangChanged(m_langCodes.value(code));
}

void AppMediaPlayer_Video_CModel_DVD::setActiveLangData(int captionLangCode, int audioLangCode, int menuLangCode)
{
	// commented by ravikanth 02-05-13
    //m_captionData.code = captionLangCode;
    //m_angleData.angleNum = audioLangCode;
    m_menuLangData.code = menuLangCode;
    emit activeLangCodeChanged(captionLangCode, audioLangCode, menuLangCode);
}
// } modified by ravikanth 13-03-13

void AppMediaPlayer_Video_CModel_DVD::setResumePosition( int value )
{
    MP_MEDIUM << " value " << value << LOG_ENDL;
    m_PlayResumePosition = value;

}

// { modified by ravikanth CR 15994 12-11-22
void AppMediaPlayer_Video_CModel_DVD::setActiveAudioLang(int no, int code, int attribute, int channel, int LFEExist) // modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display // added by cychoi 2013.07.20 for ITS 181110 Audio attribute
{
    MP_MEDIUM << "code =" << code << LOG_ENDL;

    //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
    m_audioLangData.audioNo = no;
    m_audioLangData.audioCode = code;
    //} modified by cychoi 2013.10.06
    m_audioLangData.audioAttribute = attribute;
    m_audioLangData.audioChannel = channel;
    m_audioLangData.audioLFEExist = LFEExist; // added by cychoi 2013.07.20 for ITS 181110 Audio attribute

    // remove "\n" when optionmenu width is increased and
    // revert channel and attribute position in string
    //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
    // { modified by cychoi 2013.09.26 for ISV 67967 & ITS 186197 & ITS 175110
    // { modified by ravikanth 27-11-12
    if(no == 0)
    {
        // Relevant No == 0 means "No Audio"
        m_audioLangString = m_langCodes.value(2);
        m_audioLangString2 = QString(" ");
    }
    else
    {
        // Code == 0 means "No Code"
        m_audioLangString = QString::number(no) + QString(". ") + m_langCodes.value(code == 0 ? 1 : code); // modified by cychoi 2013.07.20 for ITS 181110 Audio attribute
        // { modified by cychoi 2013.07.20 for ITS 181110 Audio attribute
        if(LFEExist)
            m_audioLangString2 = m_attrCodes.value(attribute) + m_LFEChannelCodes.value(channel);
        else
            m_audioLangString2 = m_attrCodes.value(attribute) + m_channelCodes.value(channel);
        // } modified by cychoi 2013.07.20
    }
    // } modified by ravikanth 27-11-12
    // } modified by cychoi 2013.09.26
    //} modified by cychoi 2013.10.06
    // { modified by dongjin 2013.02.27 for ISV 71500
    //emit activeAudioLangStringChanged(m_audioLangString);
    emit activeAudioLangStringChanged(m_audioLangString, m_audioLangString2);
    // } modified by dongjin
}
// } modified by ravikanth CR 15994 12-11-22

void AppMediaPlayer_Video_CModel_DVD::setActiveAngle(int index)
{
    MP_MEDIUM << "index =" << index << LOG_ENDL;
    m_angleData.angleNum = index; // modified by ravikanth CR 15994 12-11-22
    emit activeAngleChanged(index);
}

void AppMediaPlayer_Video_CModel_DVD::setIsStartedBySettingsApp(bool value)
{
    MP_MEDIUM << "value =" << value << LOG_ENDL;
    m_isStartedBySettingsApp = value;
    emit isStartedBySettingsAppChanged(value);
}


void AppMediaPlayer_Video_CModel_DVD::setPlayerState(int value)
{
    MP_MEDIUM << "value =" << value << LOG_ENDL;

    m_playerState = value;
    emit playerStateChanged(m_playerState);
}


void AppMediaPlayer_Video_CModel_DVD::setPlayerScreenState(int value)
{
    MP_MEDIUM << "value =" << value << LOG_ENDL;

    m_playerScreenState = value;
    emit playerScreenStateChanged(m_playerScreenState);
}


void AppMediaPlayer_Video_CModel_DVD::initLangCodes()
{
	// { modified by ravikanth 13-03-13
    // { modified by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add // { modified by yungi 2013.10.19 for ITS 196375
    m_defaultLangCodes.append(0);
    m_defaultLangCodes.append(514);
    m_defaultLangCodes.append(405);
    m_defaultLangCodes.append(519);
    m_defaultLangCodes.append(618);
    m_defaultLangCodes.append(1821);
    m_defaultLangCodes.append(1115);
    m_defaultLangCodes.append(2608);
    m_defaultLangCodes.append(1001);
    m_defaultLangCodes.append(118);

    m_chineseLangCodes.append(0);
    m_chineseLangCodes.append(2608);
    m_chineseLangCodes.append(514);
    m_chineseLangCodes.append(405);
    m_chineseLangCodes.append(519);
    m_chineseLangCodes.append(618);
    m_chineseLangCodes.append(1821);
    m_chineseLangCodes.append(1115);
    m_chineseLangCodes.append(1001);
    m_chineseLangCodes.append(118);

    m_arabLangCodes.append(0);
    m_arabLangCodes.append(118);
    m_arabLangCodes.append(514);
    m_arabLangCodes.append(405);
    m_arabLangCodes.append(519);
    m_arabLangCodes.append(618);
    m_arabLangCodes.append(1821);
    m_arabLangCodes.append(1115);
    m_arabLangCodes.append(2608);
    m_arabLangCodes.append(1001);

    m_domesticLangCodes.append(0);
    m_domesticLangCodes.append(1115);
    m_domesticLangCodes.append(514);
    m_domesticLangCodes.append(2608);
    m_domesticLangCodes.append(1001);
    m_domesticLangCodes.append(405);
    m_domesticLangCodes.append(618);
    m_domesticLangCodes.append(519);
    m_domesticLangCodes.append(1821); // modified by cychoi 2013.06.30 for updating language codes (Reference: KH AVN User Manual)
    m_domesticLangCodes.append(118);
    // } modified by yungi
	// } modified by ravikanth 13-03-13

    // { modified by cychoi 2013.06.30 for updating language codes (Reference: KH AVN User Manual)
    // Language codes in compliance with ISO 639:1988
    m_langCodes.insert(0, QApplication::translate("main", "STR_SETTING_DVD_NO_CAPTION"));
    m_langCodes.insert(1115, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_KOR"));
    m_langCodes.insert(514, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_ENG"));
    m_langCodes.insert(2608, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_CHN"));
    m_langCodes.insert(1001, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_JPN"));
    m_langCodes.insert(618, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_FRA"));
    m_langCodes.insert(519, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_ESP"));
    m_langCodes.insert(405, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_GER"));
    m_langCodes.insert(118, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_ARB")); // modified by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
    m_langCodes.insert(1821, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_RUS")); // modified by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add

    // 17 * 8 = 136 Language codes
    m_langCodes.insert(101, "Afar");
    m_langCodes.insert(102, "Abkhazian");
    m_langCodes.insert(106, "Afrikaans");
    m_langCodes.insert(113, "Amharic");
    //m_langCodes.insert(118, "Arabic"); // deleted by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
    m_langCodes.insert(119, "Assamese");
    m_langCodes.insert(125, "Aymara");
    m_langCodes.insert(126, "Azerbaijani");
    m_langCodes.insert(201, "Bashkir");
    m_langCodes.insert(205, "Belarusian");
    m_langCodes.insert(207, "Bulgarian");
    m_langCodes.insert(208, "Bihari");
    m_langCodes.insert(209, "Bislama");
    m_langCodes.insert(214, "Bengali");
    m_langCodes.insert(215, "Tibetan");
    m_langCodes.insert(218, "Breton");
    m_langCodes.insert(301, "Catalan");

    m_langCodes.insert(315, "Corsican");
    m_langCodes.insert(319, "Czech");
    m_langCodes.insert(325, "Welsh");
    m_langCodes.insert(401, "Danish");
//    m_langCodes.insert(405, "German");
    m_langCodes.insert(426, "Bhutani");
    m_langCodes.insert(512, "Greek");
//    m_langCodes.insert(514, "English");
    m_langCodes.insert(515, "Esperanto");
//    m_langCodes.insert(519, "Spanish");
    m_langCodes.insert(520, "Estonian");
    m_langCodes.insert(521, "Basque");
    m_langCodes.insert(601, "Persian");
    m_langCodes.insert(609, "Finnish");
    m_langCodes.insert(610, "Fijian");
    m_langCodes.insert(615, "Faroese");
//    m_langCodes.insert(618, "French");

    m_langCodes.insert(625, "Frisian");
    m_langCodes.insert(701, "Irish");
    m_langCodes.insert(704, "Scots");
    m_langCodes.insert(712, "Gallegan");
    m_langCodes.insert(714, "Guarani");
    m_langCodes.insert(721, "Gujarati");
    m_langCodes.insert(801, "Hausa");
    m_langCodes.insert(809, "Hindi");
    m_langCodes.insert(818, "Croatian");
    m_langCodes.insert(821, "Hungarian");
    m_langCodes.insert(825, "Armenian");
    m_langCodes.insert(901, "Interlingua");
    m_langCodes.insert(905, "Interlingue");
    m_langCodes.insert(911, "Inupiak");
    m_langCodes.insert(914, "Indonesian"); // added by mozart 2013.06.23, for ITS 158175
    m_langCodes.insert(919, "Icelandic");
    m_langCodes.insert(920, "Italian");

    m_langCodes.insert(923, "Hebrew");
    m_langCodes.insert(1009, "Yiddish");
//    m_langCodes.insert(1001, "Japanese");
    m_langCodes.insert(1022, "Javanese");
    m_langCodes.insert(1101, "Georgian");
    m_langCodes.insert(1111, "Kazakh");
    m_langCodes.insert(1112, "Greenlandic");
    m_langCodes.insert(1113, "Cambodian");
    m_langCodes.insert(1114, "Kannada");
//    m_langCodes.insert(1115, "Korean");
    m_langCodes.insert(1119, "Kashmiri");
    m_langCodes.insert(1121, "Kurdish");
    m_langCodes.insert(1125, "Kirghiz");
    m_langCodes.insert(1201, "Latin");
    m_langCodes.insert(1214, "Lingala");
    m_langCodes.insert(1215, "Laotian");
    m_langCodes.insert(1220, "Lithuanian");

    m_langCodes.insert(1222, "Latvian");
    m_langCodes.insert(1307, "Malagasy");
    m_langCodes.insert(1309, "Maori");
    m_langCodes.insert(1311, "Macedonian");
    m_langCodes.insert(1312, "Malayalam");
    m_langCodes.insert(1314, "Mongolian");
    m_langCodes.insert(1315, "Moldavian");
    m_langCodes.insert(1318, "Marathi");
    m_langCodes.insert(1319, "Malay");
    m_langCodes.insert(1320, "Maltese");
    m_langCodes.insert(1325, "Burmese");
    m_langCodes.insert(1401, "Nauru");
    m_langCodes.insert(1405, "Nepali");
    m_langCodes.insert(1412, "Dutch");
    m_langCodes.insert(1415, "Norwegian");
    m_langCodes.insert(1503, "Occitan");
    m_langCodes.insert(1513, "Oromo");

    m_langCodes.insert(1518, "Oriya");
    m_langCodes.insert(1601, "Punjabi");
    m_langCodes.insert(1612, "Polish");
    m_langCodes.insert(1619, "Pushto");
    m_langCodes.insert(1620, "Portuguese");
    m_langCodes.insert(1721, "Quechua");
    m_langCodes.insert(1813, "Romance");
    m_langCodes.insert(1814, "Kirundi");
    m_langCodes.insert(1815, "Romanian");
    //m_langCodes.insert(1821, "Russian"); // deleted by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
    m_langCodes.insert(1823, "Kinyarwanda");
    m_langCodes.insert(1901, "Sanskrit");
    m_langCodes.insert(1904, "Sindhi");
    m_langCodes.insert(1907, "Sangho");
    m_langCodes.insert(1908, "Croatian");
    m_langCodes.insert(1909, "Sinhalese");
    m_langCodes.insert(1911, "Slovak");

    m_langCodes.insert(1912, "Slovenian");
    m_langCodes.insert(1913, "Samoan");
    m_langCodes.insert(1914, "Shona");
    m_langCodes.insert(1915, "Somal");
    m_langCodes.insert(1917, "Albanian");
    m_langCodes.insert(1918, "Serbian");
    m_langCodes.insert(1919, "Swati");
    m_langCodes.insert(1920, "Sesotho");
    m_langCodes.insert(1921, "Sundanese");
    m_langCodes.insert(1922, "Swedish");
    m_langCodes.insert(1923, "Swahili");
    m_langCodes.insert(2001, "Tamil");
    m_langCodes.insert(2005, "Telugu");
    m_langCodes.insert(2007, "Tajik");
    m_langCodes.insert(2008, "Thai");
    m_langCodes.insert(2009, "Tigrinya");
    m_langCodes.insert(2011, "Turkmen");

    m_langCodes.insert(2012, "Tagalog");
    m_langCodes.insert(2014, "Tswana");
    m_langCodes.insert(2015, "Tongo");
    m_langCodes.insert(2018, "Turkish");
    m_langCodes.insert(2019, "Tsonga");
    m_langCodes.insert(2020, "Tatar");
    m_langCodes.insert(2023, "Twi");
    m_langCodes.insert(2111, "Ukrainian");
    m_langCodes.insert(2118, "Urdu");
    m_langCodes.insert(2126, "Uzbek");
    m_langCodes.insert(2209, "Vietnamese");
    m_langCodes.insert(2215, "Volapuk");
    m_langCodes.insert(2315, "Wolof");
    m_langCodes.insert(2408, "Xhosa");
    m_langCodes.insert(2515, "Yoruba");
//    m_langCodes.insert(2608, "Chinese");
    m_langCodes.insert(2621, "Zulu");
    // } modified by cychoi 2013.06.30

    //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display // { modified by cychoi 2013.09.26 for ISV 67967 & ITS 186197 & ITS 175110
    m_langCodes.insert(1, QApplication::translate("main", "STR_SETTING_DVD_NO_CODE")); //modified by yungi 2013.10.04 for DVD Active Audio Menu Scenario changed
    m_langCodes.insert(2, QApplication::translate("main", "STR_SETTING_DVD_NO_AUDIO")); // modified by ravikanth 27-11-12
    //} modified by cychoi 2013.10.06 // } modified by cychoi 2013.09.26
}


void AppMediaPlayer_Video_CModel_DVD::initAttrCodes()
{
   m_attrCodes.insert(0," LPCM");
   m_attrCodes.insert(1," DOLBY Digital");
   m_attrCodes.insert(2," DTS");
   m_attrCodes.insert(3," MPEG");
   m_attrCodes.insert(4," SDDS");
   m_attrCodes.insert(5," PCM(MLP)");
   m_attrCodes.insert(7," Not applicable");
}


void AppMediaPlayer_Video_CModel_DVD::initChannelCodes()
{
    m_channelCodes.insert(0," 1.0ch");
    m_channelCodes.insert(1," 1.0ch");
    m_channelCodes.insert(2," 2.0ch");
    m_channelCodes.insert(3," 3.0ch");
    m_channelCodes.insert(4," 3.0ch");
    m_channelCodes.insert(5," 4.0ch");
    m_channelCodes.insert(6," 4.0ch");
    m_channelCodes.insert(7," 5.0ch");
    m_channelCodes.insert(15,"");
}

void AppMediaPlayer_Video_CModel_DVD::initLFEChannelCodes() // added by cychoi 2013.07.20 for ITS 181110 Audio attribute
{
    m_LFEChannelCodes.insert(0," 1.1ch");
    m_LFEChannelCodes.insert(1," 1.1ch");
    m_LFEChannelCodes.insert(2," 2.1ch");
    m_LFEChannelCodes.insert(3," 3.1ch");
    m_LFEChannelCodes.insert(4," 3.1ch");
    m_LFEChannelCodes.insert(5," 4.1ch");
    m_LFEChannelCodes.insert(6," 4.1ch");
    m_LFEChannelCodes.insert(7," 5.1ch");
    m_LFEChannelCodes.insert(15,"");
}

// { modified by ravikanth CR 15994 12-11-22
QString AppMediaPlayer_Video_CModel_DVD::activeCaption()
{
    //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
    if(m_captionData.captionNo == 0)
    {
        // Relevant No == 0 means "No Caption"
        return m_langCodes.value(0);
    }
    else
    {
        // Code == 0 means "No Code"
        return QString::number(m_captionData.captionNo) + QString(". ") + m_langCodes.value(m_captionData.captionCode == 0 ? 1 : m_captionData.captionCode);
    }
    //} modified by cychoi 2013.10.06
}

// { modified by dongjin 2013.02.27 for ISV 71500
//QString AppMediaPlayer_Video_CModel_DVD::activeAudioLang()
//{
//    return m_audioLangString;
//}
QString AppMediaPlayer_Video_CModel_DVD::activeAudioLang()
{
    // { modified by cychoi 2013.06.30 for ITS 175110
    //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
    if(m_audioLangData.audioNo == 0)
    {
        // Relevant No == 0 means "No Audio"
        m_audioLangString = m_langCodes.value(2);
    }
    else
    {
        // Code == 0 means "No Code"
        m_audioLangString = QString::number(m_audioLangData.audioNo) + QString(". ") + m_langCodes.value(m_audioLangData.audioCode == 0 ? 1 : m_audioLangData.audioCode);
    }
    //} modified by cychoi 2013.10.06
    return m_audioLangString;
    //return m_langCodes.value(m_audioLangData.audioNum);
    // } modified by cychoi 2013.06.30
}
QString AppMediaPlayer_Video_CModel_DVD::activeAudioLang2()
{
    //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display
    if(m_audioLangData.audioNo == 0)
    {
        // Relevant No == 0 means "No Audio"
        m_audioLangString2 = QString(" ");
    }
    else
    {
        // { modified by cychoi 2013.09.26 for ISV 67967 & ITS 186197 & ITS 175110 // { modified by cychoi 2013.06.30 for ITS 175110
        // { modified by cychoi 2013.07.20 for ITS 181110 Audio attribute
        if(m_audioLangData.audioLFEExist)
            m_audioLangString2 = m_attrCodes.value(m_audioLangData.audioAttribute) + m_LFEChannelCodes.value(m_audioLangData.audioChannel);
        else
            m_audioLangString2 = m_attrCodes.value(m_audioLangData.audioAttribute) + m_channelCodes.value(m_audioLangData.audioChannel);
    }
    // } modified by cychoi 2013.07.20 for ITS 181110 Audio attribute
    // } modified by cychoi 2013.09.26 // } modified by cychoi 2013.06.30
    //} modified by cychoi 2013.10.06
    return m_audioLangString2;
    //return m_attrCodes.value(m_audioLangData.audioAttribute) + m_channelCodes.value(m_audioLangData.audioChannel);
}
// } modified by dongjin

int AppMediaPlayer_Video_CModel_DVD::activeAngle()
{
    return m_angleData.angleNum;
}
// } modified by ravikanth CR 15994 12-11-22
// { added by wspark 2013.03.29 for ISV 77716
void AppMediaPlayer_Video_CModel_DVD::setListBtnVisible(bool value)
{
    Q_UNUSED(value);
    // { commented by cychoi 2013.07.13 for List View spec out
    //m_bListBtnVisible = value;
    //emit listBtnVisibleChanged(value);
    // } commented by cychoi 2013.07.13
}
// } added by wspark

// { added by cychoi 2013.10.22 for ITS 197384
void AppMediaPlayer_Video_CModel_DVD::retranslateUI()
{
    m_langCodes.insert(0, QApplication::translate("main", "STR_SETTING_DVD_NO_CAPTION"));
    m_langCodes.insert(1115, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_KOR"));
    m_langCodes.insert(514, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_ENG"));
    m_langCodes.insert(2608, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_CHN"));
    m_langCodes.insert(1001, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_JPN"));
    m_langCodes.insert(618, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_FRA"));
    m_langCodes.insert(519, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_ESP"));
    m_langCodes.insert(405, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_GER"));
    m_langCodes.insert(1821,QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_RUS")); // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
    m_langCodes.insert(118, QApplication::translate("main","STR_SETTING_DVD_CAPTION_LANG_ARB")); // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add

    //{ modified by cychoi 2013.10.06 for ITS 186197 Caption & Audio Display // { modified by cychoi 2013.09.26 for ISV 67967 & ITS 186197 & ITS 175110
    m_langCodes.insert(1, QApplication::translate("main", "STR_SETTING_DVD_NO_CODE")); //modified by yungi 2013.10.04 for DVD Active Audio Menu Scenario changed
    m_langCodes.insert(2, QApplication::translate("main", "STR_SETTING_DVD_NO_AUDIO")); // modified by ravikanth 27-11-12
    //} modified by cychoi 2013.10.06 // } modified by cychoi 2013.09.26
}
// } added by cychoi 2013.10.22

// removed by cychoi 2014.03.18 for ITS 229882 No option menu on Title/Disc Menu Screen //{ added by yongkyun.lee 2013-11-05 for ITS 206356

