// { modified by Sergey for CR#11607
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_ScreenPopToast.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_PopToast.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include <QApplication>

AppMediaPlayer_Video_ScreenPopToast::
AppMediaPlayer_Video_ScreenPopToast( AppMediaPlayer_Video_Controller *controller,
                                     AppMediaPlayer_EngineVideo *pEngineVideo,
                                     QObject *parent )
   : AppMediaPlayer_Video_Screen ( parent ),
     m_pModel ( NULL ),
     m_pTimer ( NULL )
{
   Q_UNUSED(controller);
   Q_UNUSED(pEngineVideo);

   m_pModel = new AppMediaPlayer_Video_SModel_PopToast(this);
   m_pTimer = new QTimer(this);

   setModel(m_pModel);
   initMessage();

   connect(m_pTimer, SIGNAL(timeout()),this, SLOT(closeScreen()));
}

AppMediaPlayer_Video_ScreenPopToast::~AppMediaPlayer_Video_ScreenPopToast()
{
}

void AppMediaPlayer_Video_ScreenPopToast::closeScreen()
{
    if(m_pTimer->isActive())
        m_pTimer->stop();

    emit popScreen();
}

void AppMediaPlayer_Video_ScreenPopToast::onHide()
{
    if(m_pTimer->isActive())
        m_pTimer->stop();
}

void AppMediaPlayer_Video_ScreenPopToast::onShow( QVariant arg )
{
   MP_LOG << LOG_ENDL;

   if(arg.canConvert<POPUP_TOAST_TYPE_T>())
       setupPopUp(arg.value<POPUP_TOAST_TYPE_T>());
   else
       MP_LOG << "Can not convert input arg to popUp type!" << LOG_ENDL;
}

void AppMediaPlayer_Video_ScreenPopToast::setupPopUp(POPUP_TOAST_TYPE_T typePopUp)
{
    MP_LOG << "typePopUp = " << typePopUp << LOG_ENDL;

//    QString text = QApplication::translate("main", m_messagePopUp.value(typePopUp,"").toAscii()); //deleted by Michael.Kim 2013.06.25 for ISV Issue 85785
    QString text = m_messagePopUp.value(typePopUp,""); //added by Michael.Kim 2013.06.25 for ISV Issue 85785

    switch(typePopUp)
    {
        case POPUP_TOAST_COPY_COMPLETED:
        case POPUP_TOAST_COPY_CANCELED:
        {
            m_pTimer->start(TIMER_PERIOD);
            break;
        }
        default: // make compiler happy
            break;
    }

    m_pModel->setSText(text);
}

void AppMediaPlayer_Video_ScreenPopToast::initMessage()
{
    if(false == m_messagePopUp.isEmpty())
        m_messagePopUp.clear();

    m_messagePopUp.insert(POPUP_TOAST_READING_FILE, QT_TR_NOOP("STR_MEDIA_READING_FILE")); //modified by Michael.Kim 2013.06.25 for ISV Issue 85785
    m_messagePopUp.insert(POPUP_TOAST_COPY_COMPLETED,QT_TR_NOOP("STR_MEDIA_MNG_COPY_COMPLETED")); //modified by Michael.Kim 2013.06.25 for ISV Issue 85785
    m_messagePopUp.insert(POPUP_TOAST_COPY_CANCELED, QT_TR_NOOP("STR_MEDIA_MNG_CANCEL_FILE_COPY_CANCELLED")); //modified by Michael.Kim 2013.06.25 for ISV Issue 85785
    m_messagePopUp.insert(POPUP_TOAST_MAX, "");
}
// } modified by Sergey for CR#11607
