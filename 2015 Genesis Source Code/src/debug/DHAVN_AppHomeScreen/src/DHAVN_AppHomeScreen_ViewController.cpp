#include <DHAVN_AppHomeScreen_ViewController.h>
#include <DHAVN_AppHomeScreen_AppEngine.h>

#define TIME_SHOW_PREPARING_POPUP      2000

extern int Command_Field;

struct VIEW_MAP_T
{
    HSDefP::VIEW_ID_T nViewId;
    QString sViewName;
};

ViewController::ViewController( int nDisplay, QDeclarativeContext *pCtxt, InitData *pInitData, LocalizeTrigger *pLocTrigger )
    : m_pCtxt( pCtxt ),
      m_pInitData( pInitData ),
      m_pLocTrigger( pLocTrigger ),
      m_nDisplay( nDisplay ),
      m_sTitleText( QString("") ),
      m_nTitleAlign( Qt::AlignLeft ),
      m_bFocusEnabled( true ),
      m_bJogPressed( false ),
      m_nPopUpType( HSDefP::POPUP_INVALID ),
      //m_bHBVisible( false ),
      m_bMeOnTop( false ),
      m_bMediaInfoUpdated(false),
      m_pOSD_manager(NULL)
{
    AddView( HSDefP::VIEW_ID_MAIN, "STR_HOME" );

    m_pOSD_manager = new OSD_manager;

    //connect( m_pLocTrigger, SIGNAL( retrigger() ), this, SLOT( UpdateTitle() ) );
}

ViewController::~ViewController()
{
    disconnect();
}

void ViewController::UpdateViewContext()
{
    UpdateTitle();

    m_pCtxt->setContextProperty( "View",
                                m_viewStack[ HSDefP::VIEW_ID_T( m_viewStack.size() - 1 ) ] );
    m_pCtxt->setContextProperty( "IconsModel",
                                m_viewStack[ HSDefP::VIEW_ID_T( m_viewStack.size() - 1 ) ]->Model() );

    UpdateFocusIconIdx();
}

void ViewController::setMediaInfoUpdated(bool value)
{
    m_bMediaInfoUpdated = value;
}

void ViewController::UpdateTitle(QString title)
{
    m_sTitleText = title;

    if( LANGUAGE_AR == m_pLocTrigger->GetLanguage() )
    {
        SetTitleAlign( Qt::AlignRight );
    }
    else
    {
        SetTitleAlign( Qt::AlignLeft );
    }

    emit UpdateTitleText();
}

void ViewController::UpdateTitle()
{
    m_sTitleText = m_pLocTrigger->Translate( m_viewStack[ m_viewStack.size() - 1 ]->GetViewName() );

    if( LANGUAGE_AR == m_pLocTrigger->GetLanguage() )
    {
        SetTitleAlign( Qt::AlignRight );
    }
    else
    {
        SetTitleAlign( Qt::AlignLeft );
    }

    emit UpdateTitleText();
}

bool ViewController::AddView( HSDefP::VIEW_ID_T nView, QString sViewName )
{
    if( nView < HSDefP::VIEW_ID_MAX )
    {

        View *pNewView = new View( m_nDisplay, nView, sViewName, m_pInitData,
                                  HSDefP::VIEW_ID_MAIN != nView );

        m_viewStack.append( pNewView );

        UpdateViewContext();
    }

    return ( m_viewStack.size() > 1 );
}

bool ViewController::UpdateSubMenuContext( HSDefP::VIEW_ID_T nView )
{
    m_viewStack[0]->UpdateSubMenuContext(nView, m_pCtxt);
    //UpdateFocusIconIdx();
    return 0;
}

bool ViewController::RemoveView( HSDefP::VIEW_ID_T nView )
{
    if( HSDefP::VIEW_ID_INVALID == nView )
    {
        if( m_viewStack.size() > 1 )
        {
            delete m_viewStack.last();
            m_viewStack.removeLast();
        }
    }
    else
    {
        for( int i = 0; i < m_viewStack.size(); i++ )
        {
            if( nView == m_viewStack[ i ]->ViewId() )
            {
                delete m_viewStack.takeAt( i );
                m_viewStack[ i ] = NULL;
            }
        }
    }

    UpdateViewContext();

    return ( m_viewStack.size() > 1 );
}

bool ViewController::ShowRootView()
{
    while( m_viewStack.size() > 1 ) RemoveView();

    return ( m_viewStack.size() > 1 );
}

bool ViewController::IsRootView()
{
    return ( HSDefP::VIEW_ID_MAIN == m_viewStack.last()->ViewId() );
}

void ViewController::SetFocusEnabled( bool bEnabled )
{
    m_bFocusEnabled = bEnabled;

    emit UpdateFocusEnabled();
}

void ViewController::SetJogPressed( bool bPressed )
{
    m_bJogPressed = bPressed;

    emit UpdateJogPressed();
}

void ViewController::SetTitleAlign( Qt::AlignmentFlag nFlag )
{
    m_nTitleAlign = nFlag;
    emit UpdateTitleAlign();
}

void ViewController::ShowPopUp( int nAppId, int nViewId )
{

    /*
    if ( !m_pInitData->GetAppData()->GetValue( HSDefP::APP_DATA_ENABLED_PARKED ).toBool() )
    {
        if ( ( nAppId == HSDefP::APP_ID_VEHICLE_DIAGNOSTICS ) ||
             ( nAppId == HSDefP::APP_ID_APP_STORE ) ||
             ( nAppId == HSDefP::APP_ID_WIFI_SETTINGS ) ||

             //( nViewId == HSDefP::VIEW_ID_APPS ) ||
             //( nAppId == HSDefP::APP_ID_DOWNLOAD_APP ) ||
             //( nAppId == HSDefP::APP_ID_MAINTENANCE ) ||
             //( nAppId == HSDefP::APP_ID_INTERNET ) ||

             ( nAppId == HSDefP::APP_ID_E_MANUAL ) ||

             ( nAppId == HSDefP::APP_ID_PHOTO_JBOX ) ||
             ( nAppId == HSDefP::APP_ID_PHOTO_USB1 ) ||
             ( nAppId == HSDefP::APP_ID_PHOTO_USB2 ) )
        {
            SetPopUpType( (int)HSDefP::POPUP_RESTRICTION );
            return;
        }
    }
    */

    if ( (nAppId == HSDefP::APP_ID_NAVI) ||
            (nAppId == HSDefP::APP_ID_SETTINGS_NAVI) )
    {
        if(Command_Field == 1)
        {
            HS_LOG( "Command_Field = " << Command_Field );
            SetPopUpType( (int)HSDefP::POPUP_NO_SDCARD );
            return;
        }
        if(Command_Field == 2)
        {
            HS_LOG( "Command_Field = " << Command_Field );
            SetPopUpType( (int)HSDefP::POPUP_SDCARD_NOT_SUPPORT_PREMIUM20 );
            return;
        }
    }

    if ( nAppId == HSDefP::APP_ID_SETTINGS_VR)
    {
        SetPopUpType( (int)HSDefP::POPUP_VR_NOT_SUPPORT );
        return;
    }

    if ( nAppId == HSDefP::APP_ID_E_MANUAL)
    {
        SetPopUpType( (int)HSDefP::POPUP_HELP_NOT_SUPPORT );
        return;
    }

    /*
    if ( (nAppId == HSDefP::APP_ID_PANDORA1) ||
         (nAppId == HSDefP::APP_ID_PANDORA2) ||
         (nAppId == HSDefP::APP_ID_AHA1) ||
         (nAppId == HSDefP::APP_ID_AHA2) )
    {
        SetPopUpType( (int)HSDefP::POPUP_NO_PHONE_CONNECTED );
        return;
    }
    */

    if( //( nAppId == HSDefP::APP_ID_AUX ) ||
            //( nAppId == HSDefP::APP_ID_AUDIO_BLUETOOTH ) ||
            //( nAppId == HSDefP::APP_ID_AUDIO_DISC ) ||
            //( nAppId == HSDefP::APP_ID_AUDIO_IPOD1 ) ||
            //( nAppId == HSDefP::APP_ID_AUDIO_IPOD2 ) ||
            //( nAppId == HSDefP::APP_ID_VIDEO_DISC ) ||
            ( nViewId == HSDefP::VIEW_ID_MEDIA ) )
    {
        if (m_bMediaInfoUpdated)
            SetPopUpType( (int)HSDefP::POPUP_NO_MEDIA );
        else
        {
            if (m_pOSD_manager)
                m_pOSD_manager->OSD_AppsBooting(m_nDisplay);
        }


        return;
    }

    /*
    if ( nAppId == HSDefP::APP_ID_PHOTO_JBOX )
    {
        SetPopUpType( (int)HSDefP::POPUP_NO_IMAGE_IN_JBOX );
        return;
    }
    */

    /*
    if ( ( nAppId == HSDefP::APP_ID_PHOTO_USB1 ) ||
            ( nAppId == HSDefP::APP_ID_PHOTO_USB2 ) )
    {
        SetPopUpType( (int)HSDefP::POPUP_NO_IMAGE_IN_USB );
        return;
    }
    */

    if (nAppId == HSDefP::APP_ID_AUDIO_BLUETOOTH)
    {
        SetPopUpType( (int)HSDefP::POPUP_DISABLE_BT_MUSIC );
        return;
    }

    /*
    if ( nAppId == HSDefP::APP_ID_AUDIO_JBOX )
    {
        SetPopUpType( (int)HSDefP::POPUP_NO_MUSIC_IN_JBOX );
        return;
    }
    */

    /*
    if ( ( nAppId == HSDefP::APP_ID_AUDIO_USB1 ) ||
            ( nAppId == HSDefP::APP_ID_AUDIO_USB2 ) )
    {
        SetPopUpType( (int)HSDefP::POPUP_NO_MUSIC_IN_USB );
        return;
    }
    */

    /*
    if ( nAppId == HSDefP::APP_ID_VIDEO_JBOX )
    {
        SetPopUpType( (int)HSDefP::POPUP_NO_VIDEO_IN_JBOX );
        return;
    }
    */

    /*
    if ( ( nAppId == HSDefP::APP_ID_VIDEO_USB1 ) ||
            ( nAppId == HSDefP::APP_ID_VIDEO_USB2 ) )
    {
        SetPopUpType( (int)HSDefP::POPUP_NO_VIDEO_IN_USB );
        return;
    }
    */

    /*
    if ( nAppId == HSDefP::APP_ID_MODEM_DIAL  ||
            nAppId == HSDefP::APP_ID_BLUELINK_CENTER )
    {
        SetPopUpType( (int)HSDefP::POPUP_NOT_USE_BLUELINK_PHONE );
        return;
    }
    */
}

void ViewController::SetFocusIconIndex( int nViewId, int iconIdx )
{

    HSDefP::VIEW_ID_T eViewId = (HSDefP::VIEW_ID_T) nViewId;
    if ( m_FocusIconIdxList.contains( eViewId ) )
    {
        m_FocusIconIdxList.remove( eViewId );
    }

    m_FocusIconIdxList.insert( eViewId, iconIdx );
}

void ViewController::SetPopUpType( int nType )
{
    m_nPopUpType = HSDefP::POPUP_TYPE_T( nType );
    //HS_LOG( "kjw :: nType = " << nType );
    emit UpdatePopUpType();
}

/*
void ViewController::SetHomeButtonState( HSDefP::HB_STATE nState )
{
    switch( nState )
    {
    case HSDefP::HB_STATE_BUTTON: SetHBVisible( true ); break;
    case HSDefP::HB_STATE_TEXT: SetHBVisible( false ); break;
    default: SetHBVisible( !IsRootView() );
    }
}
*/

/*
void ViewController::SetHBVisible( bool bVisible )
{
    m_bHBVisible = bVisible;
}
*/

void ViewController::SetForeground( bool bOnTop )
{
    m_bMeOnTop = bOnTop;

//    if( m_bMeOnTop )
//    {
//        /** It means that we received EVT_REQUEST_FG,
//        * so need to update HB state by timeout
//        */
//        CreateTimerForSetHBState();
//    }
//    else
//    {
//        /** It means that we received EVT_REQUEST_BG,
//        * so need to show Home Button immediately
//        */
//        SetHomeButtonState( HSDefP::HB_STATE_BUTTON );
//    }
}

void ViewController::UpdateFocusIconIdx()
{
    HSDefP::VIEW_ID_T eCurViewId;
    int iFocusIconIdx = 0;
    View *pCurView = m_viewStack[ m_viewStack.size() - 1 ];

    if ( pCurView )
    {
        eCurViewId = pCurView->ViewId();

        if ( m_FocusIconIdxList.contains( eCurViewId ) )
        {
            iFocusIconIdx = m_FocusIconIdxList.value( eCurViewId );
            pCurView->SetFocusIconIndex( iFocusIconIdx );
            SetFocusEnabled( true );
        }

    }
}

/*
void ViewController::UpdateHomeButtonState( HSDefP::HB_STATE nState )
{
    if( m_bMeOnTop ) SetHomeButtonState( nState );
}
*/

void ViewController::onTimeoutShowPopup()
{
    SetPopUpType( HSDefP::POPUP_INVALID );
    emit closepopup(m_nDisplay);
}

/*
void ViewController::CreateTimerForSetHBState()
{
    QTimer::singleShot( 400, this, SLOT( UpdateHomeButtonState() ) );
}
*/

/*
bool ViewController::IsDrivingRestrictView()
{
    if( HSDefP::VIEW_ID_APPS == m_viewStack.last()->ViewId() )
        return true;
    return false;
}
*/

void ViewControllerHelper::SwapViewControllerContentForDCSwap(ViewController* pV1, ViewController* pV2)
{
    if (pV1 == NULL || pV2 == NULL || pV1 == pV2)
    {
        HS_LOG( "invalid param : pV1=" << pV1 << " pV2=" << pV2 );
        return;
    }

    // pV1 to Temp ---------------
    QString sCurrentMediaName = pV1->m_sCurrentMediaName;
    //QDeclarativeContext *pCtxt;
    InitData *pInitData = pV1->m_pInitData;
    //LocalizeTrigger *pLocTrigger;
    QList<View*> viewStack = pV1->m_viewStack;
    int nDisplay = pV1->m_nDisplay;
    QHash<HSDefP::VIEW_ID_T, int> FocusIconIdxList = pV1->m_FocusIconIdxList;

    QString sTitleText = pV1->m_sTitleText;
    Qt::AlignmentFlag nTitleAlign = pV1->m_nTitleAlign;
    bool bFocusEnabled = pV1->m_bFocusEnabled;
    bool bJogPressed = pV1->m_bJogPressed;
    HSDefP::POPUP_TYPE_T nPopUpType = pV1->m_nPopUpType;
    //bool bHBVisible = pV1->m_bHBVisible;
    bool bMeOnTop = pV1->m_bMeOnTop;
    // ~pV1 to Temp ---------------

    // pV2 to pV1 ---------------
    pV1->m_sCurrentMediaName = pV2->m_sCurrentMediaName;
    //QDeclarativeContext *pCtxt;
    pV1->m_pInitData = pV2->m_pInitData;
    //LocalizeTrigger *pLocTrigger;
    pV1->m_viewStack = pV2->m_viewStack;
    pV1->m_nDisplay = pV2->m_nDisplay;
    pV1->m_FocusIconIdxList = pV2->m_FocusIconIdxList;

    pV1->m_sTitleText = pV2->m_sTitleText;
    pV1->m_nTitleAlign = pV2->m_nTitleAlign;
    pV1->m_bFocusEnabled = pV2->m_bFocusEnabled;
    pV1->m_bJogPressed = pV2->m_bJogPressed;
    pV1->m_nPopUpType = pV2->m_nPopUpType;
    //pV1->m_bHBVisible = pV2->m_bHBVisible;
    pV1->m_bMeOnTop = pV2->m_bMeOnTop;
    // ~pV2 to pV1 ---------------

    // Temp to pV2 ---------------
    pV2->m_sCurrentMediaName = sCurrentMediaName;
    //QDeclarativeContext *pCtxt;
    pV2->m_pInitData = pInitData;
    //LocalizeTrigger *pLocTrigger;
    pV2->m_viewStack = viewStack;
    pV2->m_nDisplay = nDisplay;
    pV2->m_FocusIconIdxList = FocusIconIdxList;

    pV2->m_sTitleText = sTitleText;
    pV2->m_nTitleAlign = nTitleAlign;
    pV2->m_bFocusEnabled = bFocusEnabled;
    pV2->m_bJogPressed = bJogPressed;
    pV2->m_nPopUpType = nPopUpType;
    //pV2->m_bHBVisible = bHBVisible;
    pV2->m_bMeOnTop = bMeOnTop;
    // ~Temp to pV2 ---------------
}
