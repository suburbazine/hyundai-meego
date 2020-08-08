#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenDVDSearchCaption.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_DVDSearchCaption.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_Controller.h"
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerDVD.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_DVD.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "util/video/DHAVN_AppMediaPlayer_Video_ListModel.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_ScreenManager.h" // added by lssanh 2013.04.27 ITS165097


ScreenDVDSearchCaption::
ScreenDVDSearchCaption(AppMediaPlayer_Video_Controller *pController,
                       AppMediaPlayer_EngineVideo *pEngineVideo,
                       int disp, // added by lssanh 2013.04.27 ITS165097
                       QObject *parent)
    : AppMediaPlayer_Video_Screen(parent),
      m_pController(pController),
      m_pEngineVideo(pEngineVideo),
      m_nUserSubTitleLangCode(-1), // modified by cychoi 2013.09.22 for ITS 158176 // added by sjhyun 2013.09.11 for ITS 189090
      // { added by cychoi 2013.09.22 for ITS 158176
      m_nUserAudioLangCode(-1),
      m_nUserMenuLangCode(-1),
      // } added by cychoi 2013.09.22
      m_disp(disp), // added by lssanh 2013.04.27 ITS165097
      m_launchMode(0)
{
    m_overlays << AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA; // added by Sergey 20.07.2013

    m_viewName = AppMediaPlayer_Video_Screens::VIEW_DVD_SEARCHCAPTION;
    m_pControllerDVD = static_cast<AppMediaPlayer_Video_ControllerDVD*>(m_pController->getController(MEDIA_SOURCE_DVDVIDEO));
    m_pControllerModel = qobject_cast<AppMediaPlayer_Video_CModel_DVD*>(m_pControllerDVD->getModel());
    m_pModel = new AppMediaPlayer_Video_SModel_DVDSearchCaption(this);
    setModel(m_pModel);

    m_pListModel = new VListModel(this);
}


ScreenDVDSearchCaption::~ScreenDVDSearchCaption()
{
   delete m_pModel;
   delete m_pListModel;
}


QString ScreenDVDSearchCaption::viewName()
{
   return  m_viewName;
}


void ScreenDVDSearchCaption::onHide()
{
    MP_LOG << LOG_ENDL;

    // { modified by sjhyun 2013.09.11 for ITS 189090
    QString subScreenName = m_pEngineVideo->getEngineMain().getSubCreenName();
    if(subScreenName != AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION)
    {
        m_pModel->setInputText("");
        m_pListModel->clear();
        setCurrentUserLangCode(-1); // modifed by cychoi 2013.09.22 for ITS 158176
    }
    // } modified by sjhyun 2013.09.11
}


void ScreenDVDSearchCaption::onShow(QVariant arg)
{
    Q_UNUSED(arg); // modified by sjhyun 2013.09.11 for ITS 189090

    MP_LOG << LOG_ENDL;
	//m_launchMode = arg.toInt(); // modified by sjhyun 2013.09.11 for ITS 189090

    // { added by yungi 2013.06.04 for ITS 167159
    if(!m_pModel->inputText().isEmpty()) // added by yungi 2013.10.15 for ITS 195727
    {
        //m_pModel->setInputText(getCurrentCode()); // commented by cychoi 2013.12.16 for ITS 215827 // modified by sjhyun 2013.09.11 for ITS 189090
        refreshCaptionList(); // added by cychoi 2013.12.01 for List is not displayed when entered SearchCaption first time
    }

    //if(m_pModel->inputText().count() != 0) // commented by cychoi 2013.12.01 for List is not displayed when entered SearchCaption first time
        setmIsVisible(true);
    // } added by yungi

    setTempModel(m_pListModel, "captionModel");

    setupModeArea(); // added by cychoi 2013.10.25 for SearchCaption title
}

//added by Michaele.Kim 2013.07.11 for New UX
void ScreenDVDSearchCaption::onCodeEntered(int languageCode)
{
    MP_LOG << "languageCode =" << languageCode << LOG_ENDL;

    VListModelItem* codeValue = m_pListModel->getListItem(languageCode);
    m_pModel->setInputText(codeValue->text2());
    handleItemSelected(m_pModel->inputText().toInt());
    setCurrentUserLangCode(m_pModel->inputText().toInt()); // added by cychoi 2013.09.22 for ITS 158176
    MP_LOG << "onKeyEntered Input Txt: " <<  m_pModel->inputText().toInt() << LOG_ENDL;
    emit popScreen(QVariant("DISC"));
}
//added by Michaele.Kim 2013.07.11 for New UX

void ScreenDVDSearchCaption::onKeyEntered(QString key)
{
    MP_LOG << "key =" << key << LOG_ENDL;
    MP_LOG << "onKeyEntered Input Txt: " <<  m_pModel->inputText().toInt() << LOG_ENDL;

    if ( key == "Done" )
    {
        if(m_pListModel->rowCount() == 1)
        {
            MP_LOG << "select caption code =" << m_pListModel->data(0, m_pListModel->Text2Role) << LOG_ENDL;
            // { modified by cychoi 2013.12.01
            int langCode = m_pListModel->data(0, m_pListModel->Text2Role).toInt();
            handleItemSelected(langCode);
            setCurrentUserLangCode(langCode); // added by cychoi 2013.09.22 for ITS 158176
            //m_pControllerDVD->setSubtitleLanguage(langCode);
            // } modified by cychoi 2013.12.01

            // SCREEN_DVD_SEARCHCAPTION screen will be closed when screen state is changed to PLAYER_TOPMENU_DISPLAY,
            // PLAYER_ROOTMENU_DISPLAY or PLAYER_TITLEMENU_DISPLAY

            emit popScreen(QVariant("DISC")); // added by cychoi 2013.06.24 for new DVD Settings UX
            // removed by cychoi 2013.06.16 code arrangement
        }
        else
        {
            /* If there’s no language which is for input code or have several, call
             * pop-up to alert then back to previous screen (playback screen).
             */
            // { modified by cychoi 2014.03.19 for ITS 230153
            //m_pEngineVideo->showTxtBtnPopup(POPUP_TXT_BTN_UNAVAILABLE_LANG_CODE, (VIDEO_DISPLAY_T)m_disp);
            showTxtBtnPopUp(POPUP_TXT_BTN_UNAVAILABLE_LANG_CODE);
            // } modified by cychoi 2014.03.19
        }
    }
    else if(key == "Delete")
    {
        if(m_pModel->inputText().size() >= 1)
        {
            QString choppedText = m_pModel->inputText();
            choppedText.chop(1);
            m_pModel->setInputText(choppedText);
            setCurrentUserLangCode(m_pModel->inputText().toInt()); // modifed by cychoi 2013.09.22 for ITS 158176 // added by sjhyun 2013.09.11 for ITS 189090
            refreshCaptionList();
        }
        else
            refreshCaptionList(); //added by Michael.Kim 2013.07.23 for New UX
    }
    else //digit entered
    {
        if(m_pModel->inputText().size() <= 3)
        {
            m_pModel->setInputText((m_pModel->inputText()).append(key));
            setCurrentUserLangCode(m_pModel->inputText().toInt()); // modifed by cychoi 2013.09.22 for ITS 158176 // added by sjhyun 2013.09.11 for ITS 189090
            refreshCaptionList();
        }
    }
}


void ScreenDVDSearchCaption::refreshCaptionList()
{
    MP_LOG << "input code =" << m_pModel->inputText() << LOG_ENDL;

//    m_pListModel->clear();

//    if(m_pModel->inputText().count() != 0)
//    {
//        for(int i = 0; i < m_pControllerModel->captionData().quantity; i++)
//        {
//            QString searchCode = QString::number(m_pControllerModel->captionData().captionList[i]);

//            MP_LOG << "searchCode =" << searchCode << LOG_ENDL;

//            if(searchCode.startsWith(m_pModel->inputText(), Qt::CaseInsensitive))
//            {
//                QString foundCaptionName = m_pControllerModel->langCodes().value(searchCode.toInt());

//                MP_LOG << "found caption name =" << foundCaptionName << LOG_ENDL;

//                m_pListModel->addItem(foundCaptionName, searchCode);
//            }
//        }
//    }

//    setTempModel(m_pListModel, "captionModel");

    m_pListModel->clear();
    if(m_pModel->inputText().count() != 0)
    {
        // { modified by cychoi 2013.12.05 for ITS 211510
        QHashIterator<int, QString> iter(m_pControllerModel->langCodes());
        while (iter.hasNext())
        {
            iter.next();
            MP_LOG << iter.key() << ": " << iter.value() << LOG_ENDL;
            QString searchcode = langCodeToString(iter.key());
            int langCode = searchcode.toInt();
            if(!isDefaultLangCode(langCode) && !isNotLangCode(langCode) && // modified by cychoi 2013.11.27 for ITS 211510
               searchcode.startsWith(m_pModel->inputText(), Qt::CaseInsensitive))
            // { modified by cychoi 2013.06.30 for Nex UX
            //if(!isDefaultLangCode(langCode) && searchcode.startsWith(m_pModel->inputText(), Qt::CaseInsensitive))
            //if(searchcode.startsWith(m_pModel->inputText(), Qt::CaseInsensitive))
            // } modified by cychoi 2013.06.30
            {
                QString foundCaptionName = m_pControllerModel->langCodes().value(langCode);
                MP_LOG << "found caption name =" << foundCaptionName << LOG_ENDL;
                //m_pListModel->addItem(searchcode,foundCaptionName);
                m_pListModel->addItem(foundCaptionName,searchcode); //modified by Michael.Kim 2013.07.20 for New UX
            }
        }
        // } modified by cychoi 2013.12.05
    }
    setTempModel(m_pListModel, "captionModel");
}


void ScreenDVDSearchCaption::onSoftkeyBack()
{
   MP_LOG << LOG_ENDL;

   // { added by cychoi 2013.06.16 for ISV 85091 DVD Settings title disappear
   emit popScreen(QVariant("DISC"));
   //emit popScreen();
   // } added by cychoi 2013.06.16
}


void ScreenDVDSearchCaption::onBackKeyPressed()
{
    MP_LOG << LOG_ENDL;

    // { added by cychoi 2013.06.16 for ISV 85091 DVD Settings title disappear
    emit popScreen(QVariant("DISC"));
    //emit popScreen();
    // } added by cychoi 2013.06.16
}

// { added by sjhyun 2013.09.11 for ITS 189090
void ScreenDVDSearchCaption::onEventReceived(const QString &srcScreenName, const QString &resScreenName,
                                     const int eventId, const QVariant arg)
{
    MP_LOG << " srcScreenName = " << srcScreenName << " resScreenName = " << resScreenName << \
              " eventId = " << eventId << " arg = " << arg << LOG_ENDL;
    
    Q_UNUSED(srcScreenName);
    
    if ( resScreenName == AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION )
    {
       switch( eventId )
       {
       case EVENTS_SET_DVD_ACTIVE_TAP:
           m_launchMode = arg.toInt();
           break;
       default:
           break; // make compiler happy
       }
    }
}
// } added by sjhyun 2013.09.11

void ScreenDVDSearchCaption::handleItemSelected(int code)
{
    MP_LOG << "item type =" << m_launchMode << LOG_ENDL;
    MP_LOG << "item code =" << code << LOG_ENDL; // modified by ravikanth CR 15994 12-11-22

    switch((VC_DATA)m_launchMode)
    {
        case VC_DATA_CAPTIONS:
        {
                // { modified by ravikanth CR 15994 12-11-22
            m_pControllerDVD->setSubtitleLanguage(code);
                // } modified by ravikanth CR 15994 12-11-22
            break;
        }
        case VC_DATA_VOICELANG:
        {
            m_pControllerDVD->setAudioLanguage(code); // modified by ravikanth CR 15994 12-11-22
            break;
        }
                // { modified by ravikanth CR 15994 12-11-22
        case VC_DATA_MENULANG:
        {
            m_pControllerDVD->setMenuLanguage(code);
            break;
        }
                // } modified by ravikanth CR 15994 12-11-22
        default:
        {
           break;
        }
    }
}

// { added by lssanh 2013.04.19 ITS158176
QString ScreenDVDSearchCaption::getCurrentCode()
{
    MP_LOG << "item type =" << m_launchMode << LOG_ENDL;
    QString ret = "";

    // removed by cychoi 2013.09.22 for ITS 158176
		
    switch((VC_DATA)m_launchMode)
    {
        case VC_DATA_CAPTIONS:
        {
            // { added by cychoi 2013.09.22 for ITS 158176
            if(m_nUserSubTitleLangCode != -1)
            {
                // { modified by cychoi 2013.12.05 for ITS 211510
                ret = langCodeToString(m_nUserSubTitleLangCode);
                //ret = QString::number(m_nUserSubTitleLangCode);
                // } modified by cychoi 2013.12.05
            }
            // } added by cychoi 2013.09.22
            // { added by cychoi 2013.06.30 for Nex UX
            else if(isDefaultLangCode(m_pControllerDVD->getSubtitleLanguage())) // modified by cychoi 2013.09.22 for ITS 158176
            {
                ret = QString();
            }
            else
            {
                // { modified by cychoi 2013.12.05 for ITS 211510
                ret = langCodeToString(m_pControllerDVD->getSubtitleLanguage());
                //ret = QString::number(m_pControllerDVD->getSubtitleLanguage());
                // } modified by cychoi 2013.12.05
            }
            //ret = QString::number(m_pControllerDVD->getSubtitleLanguage());
            // } added by cychoi 2013.06.30
            break;
        }
        case VC_DATA_VOICELANG:
        {
            // { added by cychoi 2013.09.22 for ITS 158176
            if(m_nUserAudioLangCode != -1)
            {
                // { modified by cychoi 2013.12.05 for ITS 211510
                ret = langCodeToString(m_nUserAudioLangCode);
                //ret = QString::number(m_nUserAudioLangCode);
                // } modified by cychoi 2013.12.05
            }
            // } added by cychoi 2013.09.22
            // { added by cychoi 2013.06.30 for Nex UX
            else if(isDefaultLangCode(m_pControllerDVD->getAudioLanguage())) // modified by cychoi 2013.09.22 for ITS 158176
            {
                ret = QString();
            }
            else
            {
                // { modified by cychoi 2013.12.05 for ITS 211510
                ret = langCodeToString(m_pControllerDVD->getAudioLanguage());
                //ret = QString::number(m_pControllerDVD->getAudioLanguage());
                // } modified by cychoi 2013.12.05
            }
            //ret = QString::number(m_pControllerDVD->getAudioLanguage());
            // } added by cychoi 2013.06.30
            break;
        }
        case VC_DATA_MENULANG:
        {
            // { added by cychoi 2013.09.22 for ITS 158176
            if(m_nUserMenuLangCode != -1)
            {
                // { modified by cychoi 2013.12.05 for ITS 211510
                ret = langCodeToString(m_nUserMenuLangCode);
                //ret = QString::number(m_nUserMenuLangCode);
                // } modified by cychoi 2013.12.05
            }
            // } added by cychoi 2013.09.22
            // { added by cychoi 2013.06.30 for Nex UX
            else if(isDefaultLangCode(m_pControllerDVD->getMenuLanguage())) // modified by cychoi 2013.09.22 for ITS 158176
            {
                ret = QString();
            }
            else
            {
                // { modified by cychoi 2013.12.05 for ITS 211510
                ret = langCodeToString(m_pControllerDVD->getMenuLanguage());
                //ret = QString::number(m_pControllerDVD->getMenuLanguage());
                // } modified by cychoi 2013.12.05
            }
            //ret = QString::number(m_pControllerDVD->getMenuLanguage());
            // } added by cychoi 2013.06.30
            break;
        }
        default:
        {
           break;
        }
    }
    MP_LOG << "return code = " << ret << LOG_ENDL;
    return ret;
}

// } added by lssanh 2013.04.19 ITS158176

// { added by cychoi 2013.06.30 for Nex UX
bool ScreenDVDSearchCaption::isDefaultLangCode(int langCode)
{
    bool ret = false;

    switch(langCode)
    {
    case 1115: // KOR
    case 514: // ENG
    case 2608: // CHN
    case 1001: // JPN
    case 618: // FRA
    case 519: // ESP
    case 405: // GER
    case 1821: // RUS  // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
    case 118: // ARB  // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
    case 0: // added by yungi 2013.10.30 for Nex UX - DVD-Setting AR,RUS List Add
        ret = true;
        break;
    default:
        ret = false;
        break;
    }

    return ret;
}
// } added by cychoi 2013.06.30

// { added by cychoi 2013.11.27 for ITS 211510
bool ScreenDVDSearchCaption::isNotLangCode(int langCode)
{
    bool ret = false;

    switch(langCode)
    {
    case 1:
    case 2:
        ret = true;
        break;
    default:
        ret = false;
        break;
    }

    return ret;
}
// } added by cychoi 2013.11.27

// { added by cychoi 2013.09.22 for ITS 158176
void ScreenDVDSearchCaption::setCurrentUserLangCode(int langCode)
{
    switch((VC_DATA)m_launchMode)
    {
        case VC_DATA_CAPTIONS:
        {
            m_nUserSubTitleLangCode = langCode;
            break;
        }
        case VC_DATA_VOICELANG:
        {
            m_nUserAudioLangCode = langCode;
            break;
        }
        case VC_DATA_MENULANG:
        {
            m_nUserMenuLangCode = langCode;
            break;
        }
        default:
        {
            // make compiler happy
            break;
        }
    }
}
// } added by cychoi 2013.09.22

// { added by cychoi 2013.12.05 for ITS 211510
QString ScreenDVDSearchCaption::langCodeToString(int langCode)
{
    QString codeString;
    
    codeString = langCode > 999 ? QString::number(langCode) : QString::number(0) + QString::number(langCode);

    return  codeString;
}
// } added by cychoi 2013.12.05

// { added by cychoi 2013.10.25 for SearchCaption title
void ScreenDVDSearchCaption::setupModeArea()
{
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_TEXT, QVariant(QT_TR_NOOP("STR_MEDIA_SEARCH")));

    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_SETTING, QVariant());
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_RTEXT_VISIBLE, QVariant(false));
    notifyScreenManager(AppMediaPlayer_Video_Screens::SCREEN_DVD_SEARCHCAPTION, AppMediaPlayer_Video_Screens::OVERLAY_SOURCE_MODEAREA,
                        EVENTS_SET_ICON_VISIBLE, QVariant(false));
}
// } added by cychoi 2013.10.25

// { added by sjhyun 2013.12.28 for ITS 198332
void ScreenDVDSearchCaption::setDefaultInputText()
{
    m_pModel->setInputText("");
    refreshCaptionList();
}
// } modified by sjhyun 2013.12.28

// { added by cychoi 2014.01.23 for ITS 222055
//int ScreenDVDSearchCaption::getCurrentInputTextCount()
//{
//    return m_pModel->inputText().count();
//}
// } added by cychoi 2014.01.23

// { added by wspark 2014.01.24 for ITS 222575
void ScreenDVDSearchCaption::deleteAllInputText()
{
    if(m_pModel->inputText().size() >= 1)
    {
        QString choppedText = m_pModel->inputText();
        choppedText.chop(m_pModel->inputText().size());
        m_pModel->setInputText(choppedText);
        setCurrentUserLangCode(m_pModel->inputText().toInt());
        refreshCaptionList();
    }
}
// } added by wspark


