
#include <QDeclarativeItem>
#include <QDeclarativeContext>

#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"

AppMediaPlayer_Video_Screen::AppMediaPlayer_Video_Screen( QObject *parent )
    : QObject( parent ),
      m_pView(NULL),
      m_pContext(NULL),
      m_pModel(NULL),
      m_isVisible(false),
      m_myName(QString()), // added by Sergey 31.08.2013 for ITS#186799
      m_isModeAreaFocused(false) // added by Sergey 10.01.2014 for ITS#218955
{
}

AppMediaPlayer_Video_Screen::~AppMediaPlayer_Video_Screen()
{
}


/** @copydoc AppMediaPlayer_Video_Screen::hide() */
void AppMediaPlayer_Video_Screen::hide()
{
    MP_LOG << LOG_ENDL;

    if ( m_pView != NULL )
    {
        MP_LOG << " setProperty visible = false" << LOG_ENDL;
        m_pView->setProperty("visible", false);
    }
    onHide();

    m_isVisible = false;
    m_pView = NULL;
    m_pContext = NULL;
}

/** @copydoc AppMediaPlayer_Video_Screen::show() */
void AppMediaPlayer_Video_Screen::show( QDeclarativeItem *item,
                                            QDeclarativeContext *context,
                                            QVariant arg )
{
    MP_LOG << LOG_ENDL;

    m_pView = item;
    m_pContext = context;

    // Should never happen: view loader will not allow NULL contexts or views
    if ( m_pView == NULL || m_pContext == NULL )
    {
        MP_LOG << "Somewhat goes wrong: either item or context are NULLs." << LOG_ENDL;
        return;
    }

    // If we have model, we want to expose it to the QML code. Note, that
    // setContextProperty() should be run once per view creation. However,
    // resetting the same model would cause QML engine to reattach all
    // bindings. Need additional investigation if it is good or not.
    if ( m_pModel != NULL )
    {
        // Avoid setting model if did not changed. This will save us a few
        // bindings that are required to be rebuilt in this case.
        QObject *model = qvariant_cast<QObject *>(m_pContext->contextProperty("model"));
        MP_LOG << "Previous model: " << (void*)model << LOG_ENDL;
        if ( model != m_pModel )
        {
            m_pContext->setContextProperty("video_model", m_pModel);
        }
        else
        {
            MP_LOG << "Skipping model update. It has not been changed." << LOG_ENDL;
        }
    }

    QObject *oldController = qvariant_cast<QObject *>(m_pContext->contextProperty("controller"));
    if ( oldController != this )
    {
        m_pContext->setContextProperty("controller", this);
    }
    loadingCompleted();
    onShow( arg );

    m_pView->setProperty("visible", true);

    m_isVisible = true;
	MP_LOG<<"view visible true" << LOG_ENDL;
}
//added by edo.lee 2013.02.05

void AppMediaPlayer_Video_Screen::lockoutMode(bool on)
{
	onLockoutMode(on);
}
//added by edo.lee 2013.02.05


/** @copydoc AppMediaPlayer_Video_Screen::deactivate() */
void AppMediaPlayer_Video_Screen::deactivate()
{
   MP_LOG << LOG_ENDL;
   onDeactivate();
}


/** @copydoc AppMediaPlayer_Video_Screen::showItem() */
void AppMediaPlayer_Video_Screen::showItem( const QString &itemName )
{
    setItemProperty( itemName, "visible", true );
}


/** @copydoc AppMediaPlayer_Video_Screen::hideItem() */
void AppMediaPlayer_Video_Screen::hideItem( const QString &itemName )
{
    // Note about QVariant here, looks like compiler treats 'false' as 0,
    // 0 as void*, void* as QObject* and tries to invoke wrong function :)
    setItemProperty( itemName, "visible", QVariant(false) );
}


/** @copydoc AppMediaPlayer_Video_Screen::getChildItem() */
QDeclarativeItem *AppMediaPlayer_Video_Screen::getChildItem( const QString &itemName )
{
    QDeclarativeItem *item;

    item = m_pView->findChild<QDeclarativeItem *>(itemName);
    return item;
}


/** @copydoc AppMediaPlayer_Video_Screen::setModel() */
void AppMediaPlayer_Video_Screen::setModel( QObject *model )
{
    m_pModel = model;

    // If we're visible, notify the UI that model was changed.
    // If screen is not visible, this will be done once screen is shown.
    if ( m_isVisible )
    {
        m_pContext->setContextProperty("video_model", m_pModel);
    }
}


/** @copydoc AppMediaPlayer_Video_Screen::setTempModel() */
void AppMediaPlayer_Video_Screen::setTempModel( QObject *model, QString name )
{
    // If screen is not visible it's useless to call setTempModel().
    if ( m_isVisible )
    {
        m_pContext->setContextProperty(name, model);
    }
}


void AppMediaPlayer_Video_Screen::showTxtBtnPopUp( POPUP_TXT_BTN_TYPE_T typePopUp )
{
    // { modified by kihyung 2012.09.03
    MP_LOG << LOG_ENDL;
    emit hideAllFocus(); // added by wspark 2012.09.06 for CR13001, CR13046, CR13049
    QVariant msgPopUp;
    msgPopUp.setValue( typePopUp );
    emit pushScreen( AppMediaPlayer_Video_Screens::SCREEN_POP_TEXTBUTTON, msgPopUp );
    // } modified by kihyung
}

// { added by Sergey for CR#11607
void AppMediaPlayer_Video_Screen::pushToastPopUp( POPUP_TOAST_TYPE_T typePopUp )
{
    QVariant msgPopUp;
    msgPopUp.setValue( typePopUp );
    emit pushScreen( AppMediaPlayer_Video_Screens::SCREEN_POP_TOAST, msgPopUp );
}
// } added by Sergey for CR#11607

void AppMediaPlayer_Video_Screen::onBackKeyPressed()
{
    emit popScreen();
}

void AppMediaPlayer_Video_Screen::onMousePressed()
{
    //emit hideAllFocus(); //deleted by aettie 2013.03.27 for Touch focus rule
}

// { added by yungi 2013.06.04 for ITS 167159
void AppMediaPlayer_Video_Screen::setmIsVisible(const bool visible)
{
    m_isVisible = visible;
}
// } added by yungi


// { added by Sergey 27.10.2013 for ITS#198669
void AppMediaPlayer_Video_Screen::invokeQmlMethod(const QString method, QVariant arg)
{
    if(m_pView)
        QMetaObject::invokeMethod(m_pView, method.toAscii(), Q_ARG(QVariant, arg));
}
// } added by Sergey 27.10.2013 for ITS#198669