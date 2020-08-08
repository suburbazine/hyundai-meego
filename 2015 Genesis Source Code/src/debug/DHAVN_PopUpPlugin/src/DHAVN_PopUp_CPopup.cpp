#include "DHAVN_PopUpCPopup.h"
#include <QApplication>

CPopUp::CPopUp(QDeclarativeItem *parent):
    QDeclarativeItem(parent)
{
    //init all popups
    DECLARE_META_EVENT();
    lprintf("[SystemPopUp] CPopUp init");
    m_pTranslator   = NULL;
    m_Popup.type    = POPUP_NONE;
    m_Popup.source  = -1;
    m_Popup.target  = -1;
    m_Popup.timeout = 0;
    m_Popup.ignoreautoclose = false;
    m_Popup.guichagneButtonNum = 0;
    m_Popup.popup_type = _CURRENT_POPUP_NONE;
    m_Popup.responsesource = 0;

    m_pSharedMemory = NULL;
    m_pRpmClient = NULL;

    if(!m_pSharedMemory){
        m_pSharedMemory = new QSharedMemory("PopUpPlugIn");
        m_pSharedMemory->create(4, QSharedMemory::ReadWrite);
        if(m_pSharedMemory)
            lprintf("[SystemPopUp] CPopUp init SharedMemory create() SUCCESS key = %s", qPrintable(m_pSharedMemory->key()));
        else
            lprintf("[SystemPopUp] CPopUp init SharedMemory create() FAIL");
    }
    if (!m_pSharedMemory->attach(QSharedMemory::ReadWrite)) {
        lprintf("[SystemPopUp] CPopUp::init() : m_pSharedMemory->attach() failed!!");
        return;
    }

#ifdef __RPMCLIENT__
    m_pRpmClient = new QRPMClient(&(qApp->applicationName()));
    connect(m_pRpmClient,SIGNAL(RPMClientAcquireResponse(TAudioMode,TAudioSink)),this,SLOT(audioCHsetResponse(TAudioMode, TAudioSink)));     //hjjung
#endif
    setenv("QML_ENABLE_TEXT_IMAGE_CACHE","1",1);
}

CPopUp::~CPopUp()
{
    lprintf("[SystemPopUp] ~CPopUp");
    delete m_pTranslator;
    if(m_pSharedMemory)
    {
        if(m_pSharedMemory->isAttached())
            m_pSharedMemory->detach();

        delete m_pSharedMemory;
    }

#ifdef __RPMCLIENT__
    if ( m_pRpmClient != NULL )
        delete m_pRpmClient;
#endif
}

void CPopUp::responseButton( int btnId )
{
   POPUP_RESPONSE_T result;
   result.type = m_Popup.type;
   result.btn = ( POPUP_BUTTON_T ) btnId;
   result.index = POPUP_INVALID_INDEX;
   result.value = 0;

   QByteArray data( ( char *) &result, sizeof( POPUP_RESPONSE_T ) );
   QVariant v(data);
    lprintf("[SystemPopUp]  CPopUp::responseButton source = %d", m_Popup.source);
   emit popup_Response( m_Popup.source, v );
}

int CPopUp::getScreenOffsetY( int nAppID )
{
    return 0;
//    APP_ID_T app_id = (APP_ID_T)nAppID;

//    if( app_id == APP_IDLE
//            || app_id == APP_MAIN_MENU
//            || app_id == APP_STANDBY
//            || app_id == APP_AVP
//            || app_id == APP_XM_DATA
//            || app_id == APP_MOBILETV
//            || app_id == APP_IDLE
//            || app_id == APP_NAVIGATION
//            || app_id == APP_CAMERA
//            || app_id == APP_E_MANUAL
//            )
//       return 0;
//    else
//       return -DH_STATUS_BAR;
}

void CPopUp::showPopUp( QVariant aPopup )
{
   lprintf("[SystemPopUp]  showPopUp start");
   m_Popup.type    = ( (POPUP_SHOW_T *)aPopup.toByteArray().data() )->type;
   m_Popup.popup   = ( (POPUP_SHOW_T *)aPopup.toByteArray().data())->popup;
   m_Popup.source  = ( (POPUP_SHOW_T *)aPopup.toByteArray().data())->source;
   m_Popup.target  = ( (POPUP_SHOW_T *)aPopup.toByteArray().data() )->target;
   m_Popup.timeout = ( (POPUP_SHOW_T *)aPopup.toByteArray().data())->timeout;

//   qDebug() << "CPopUp::showPopUp: type(" << m_Popup.type << "), source("  << m_Popup.source <<
//               "), target("  << m_Popup.target << "), timeout(" << m_Popup.timeout <<
//               "), ignoreautoclose(" << m_Popup.ignoreautoclose <<")";
   lprintf("[SystemPopUp]  CPopUp::showPopUp: type(%d), source(%d), target(%d), timeout(%d), ignoreautoclose(%d)",
            m_Popup.type, m_Popup.source, m_Popup.target, m_Popup.timeout,  m_Popup.ignoreautoclose);

   if(m_Popup.type == POPUP_TOAST)
       m_Popup.timeout = 3000;

   if(m_Popup.timeout != 0)
       emit startPopupTimer( m_Popup.timeout );

   if(m_Popup.type != POPUP_TOAST && m_Popup.ignoreautoclose == true)   // SANGWOO ignore
       emit setignoreAutoClose(m_Popup.ignoreautoclose);
   //emit setignoreAutoClose(true);

   switch( m_Popup.type )
   {
      case POPUP_NONE:
         lprintf("[SystemPopUp]  CPopUp::POPUP_NONE");
         emit closeSystemPopup();
         break;

      case POPUP_MESSAGE_BOX:
         callMessageBox( &m_Popup.popup.msg_box );
       //callToast(&m_Popup.popup.msg_box);
         break;

      case POPUP_TEXT:
         callText( &m_Popup.popup.text );
         break;

      case POPUP_CUSTOM_TEXT:
         callCustomText( &m_Popup.popup.custom_text );
         break;

      case POPUP_WARNING:
         callText( &m_Popup.popup.text );
         break;

      case POPUP_LIST:
        callList( &m_Popup.popup.list );
        break;

      case POPUP_TOAST:
       callToast(&m_Popup.popup.toast);
       break;

   case POPUP_XM_ALERT:
       callXMAlert(&m_Popup.popup.xm_alert);
       break;
      case POPUP_NAVI:  
        callNAVIText( &m_Popup.popup.navitext );
       break;
      default:
        lprintf("[SystemPopUp]  CPopUp::showPopUp: type %d", m_Popup.type);
        break;
   }
}

void CPopUp::callMessageBox( POPUP_TYPE_MESSAGE_BOX_T *msg_box )
{
   lprintf("[SystemPopUp]  CPopUp::POPUP_MESSAGE_BOX:");
   QString message = QString::fromWCharArray( msg_box->msg ).left( POPUP_CONTENT_LENGHT );
   lprintf("[SystemPopUp]  CPopUp::POPUP_MESSAGE_BOX: ", qPrintable(message));
   emit showPopup_MessageBox( msg_box->size, message );
}

void CPopUp::callToast(POPUP_TYPE_TOAST_T * toast)
{
    lprintf("[SystemPopUp]  CPopUp::POPUP_TOAST ");
    QStringList args;
    //QString message = QString::fromWCharArray( toast->msg ).left( POPUP_CONTENT_LENGHT );
    args << QString::fromWCharArray( toast->msg ).left( POPUP_CONTENT_LENGHT );
    args << QString().setNum(toast->statusbar);
    //emit showPopup_Toast( message );
   emit showPopup_Toast(args);
}

void CPopUp::callXMAlert(POPUP_TYPE_XM_ALERT_T * xm_alert)
{
    lprintf("[SystemPopUp]  CPopUp::POPUP_XM_ALERT");
    QStringList args;
    //head
    args << QString::fromWCharArray( xm_alert->head ).left( POPUP_TITLE_LENGHT );
    //contents
    args << QString::fromWCharArray( xm_alert->contents ).left( 512 );
    //date
    args << QString::fromWCharArray( xm_alert->date ).left( POPUP_TITLE_LENGHT );
    //x
    args << QString().setNum(xm_alert->x);
    //y
    args << QString().setNum(xm_alert->y);
    //btn1
    args << QString::fromWCharArray( xm_alert->btn1_txt ).left( POPUP_TITLE_LENGHT );
    //btn2
    args << QString::fromWCharArray( xm_alert->btn2_txt ).left( POPUP_TITLE_LENGHT );
    //map_x
    args << QString().setNum(xm_alert->map_x);
    //map_y
    args << QString().setNum(xm_alert->map_y);
    args << QString().setNum(xm_alert->statusbar);

    emit showPopup_XMAlert( QVariant( args ) );
}

void CPopUp::callWarning ( POPUP_TYPE_TEXT_T *pop )
{
   lprintf("[SystemPopUp]  CPopUp::POPUP_TEXT");

   QStringList args;
   /** Set title & message */
   args << QString::fromWCharArray( pop->title ).left( POPUP_TITLE_LENGHT );
   args << QString::fromWCharArray( pop->msg ).left( POPUP_CONTENT_LENGHT );

   /** Set button count */
   pop->btn_count = pop->btn_count > POPUP_BUTTON_MAX ? POPUP_BUTTON_MAX : pop->btn_count;
   args << QString().setNum( pop->btn_count );

   /** Set button text */
   for ( int i = 0; i < pop->btn_count; i++ )
   {
      switch ( pop->btn[i] )
      {
         case POPUP_BUTTON_TYPE_OK:
            args << QString("STR_POPUP_OK");
            break;

         case POPUP_BUTTON_TYPE_CANCEL:
            args << QString("STR_POPUP_CANCEL");
            break;

         case POPUP_BUTTON_TYPE_ICON:

         case POPUP_BUTTON_TYPE_TEXT:
         default:
            args << QString::fromWCharArray( pop->btn_txt[i] ).left( POPUP_TITLE_LENGHT );
            break;
      }
   }
   args << QString().setNum( pop->statusbar);
   //args << QString().setNum( POPUP_STATUSBAR_EXIST);

   emit showPopup_Warning( QVariant( args ) );
}

void CPopUp::callList ( POPUP_TYPE_LIST_T *pop )
{
   lprintf("[SystemPopUp]  CPopUp::POPUP_LIST");

   QStringList args;
   /** Set title & message */
   args << QString::fromWCharArray( pop->title ).left( POPUP_TITLE_LENGHT );

   QString sText;
   QStringList sTextList;

   if( wcslen( pop->list ) != 0 )
   {
      sText = QString::fromWCharArray( pop->list );
      sTextList = sText.split( "|" );

      args << QString::number( sTextList.count() );

      for( int i = 0; i < sTextList.count(); i++ )
      {
          args << sTextList.at(i);
      }
   }

   /** Set button count */
   pop->btn_count = pop->btn_count > POPUP_BUTTON_MAX ? POPUP_BUTTON_MAX : pop->btn_count;
   args << QString().setNum( pop->btn_count );

   /** Set button text */
   for ( int i = 0; i < pop->btn_count; i++ )
   {
      switch ( pop->btn[i] )
      {
         case POPUP_BUTTON_TYPE_OK:
            args << QString("STR_POPUP_OK");
            break;

         case POPUP_BUTTON_TYPE_CANCEL:
            args << QString("STR_POPUP_CANCEL");
            break;

         case POPUP_BUTTON_TYPE_ICON:
         case POPUP_BUTTON_TYPE_TEXT:
         default:
            args << QString::fromWCharArray( pop->btn_txt[i] ).left( POPUP_TITLE_LENGHT );
            break;
      }
   }

   emit showPopup_List( QVariant( args ) );
}

void CPopUp::callText ( POPUP_TYPE_TEXT_T * pop )
{
    lprintf("[SystemPopUp]  CPopUp::POPUP_TEXT");

    QStringList args;
    /** Set title & message */
    args << QString::fromWCharArray( pop->title ).left( POPUP_TITLE_LENGHT );
    args << QString::fromWCharArray( pop->msg ).left( POPUP_CONTENT_LENGHT );

    /** Set button count */
    pop->btn_count = pop->btn_count > POPUP_BUTTON_MAX ? POPUP_BUTTON_MAX : pop->btn_count;
    args << QString().setNum( pop->btn_count );

    /** Set button text */
    for ( int i = 0; i < pop->btn_count; i++ )
    {
        switch ( pop->btn[i] )
        {
        case POPUP_BUTTON_TYPE_OK:
            args << QString("STR_POPUP_OK");
            break;

        case POPUP_BUTTON_TYPE_CANCEL:
            args << QString("STR_POPUP_CANCEL");
            break;

        case POPUP_BUTTON_TYPE_ICON:
        case POPUP_BUTTON_TYPE_TEXT:
        default:
            args << QString::fromWCharArray( pop->btn_txt[i] ).left( POPUP_TITLE_LENGHT );
            break;
        }
    }
    args << QString().setNum( pop->useAnimation );
    args << QString().setNum( pop->statusbar );

        emit showPopup_Text( QVariant( args ) );
}
// SANGWOO_TEMP NAVI START


void CPopUp::callNAVIText ( POPUP_TYPE_NAVI_TEXT_T  * pop )
{
    lprintf("[SystemPopUp]  CPopUp::POPUP_NAVI");
    lprintf("[SystemPopUp]  CPopUp callNAVIText");
    lprintf("[SystemPopUp]  CPopUp callNAVIText: %s", pop->traffic_icon);
    lprintf("[SystemPopUp]  CPopUp callNAVIText: %s", pop->road_icon);
    lprintf("[SystemPopUp]  CPopUp callNAVIText: %s", pop->road_number);
    lprintf("[SystemPopUp]  CPopUp callNAVIText: %s", pop->title);
    QStringList args;
    /** Set title & message */
    args << QString::fromWCharArray( pop->title ).left( POPUP_TITLE_LENGHT );
    args << QString::fromWCharArray( pop->msg ).left( POPUP_CONTENT_LENGHT );



    /** Set button count */
    pop->btn_count = pop->btn_count > POPUP_BUTTON_MAX ? POPUP_BUTTON_MAX : pop->btn_count;
    args << QString().setNum( pop->btn_count );

    /** Set button text */
    for ( int i = 0; i < pop->btn_count; i++ )
    {
        switch ( pop->btn[i] )
        {
        case POPUP_BUTTON_TYPE_OK:
            args << QString("STR_POPUP_OK");
            break;

        case POPUP_BUTTON_TYPE_CANCEL:
            args << QString("STR_POPUP_CANCEL");
            break;

        case POPUP_BUTTON_TYPE_ICON:
        case POPUP_BUTTON_TYPE_TEXT:
        default:
            args << QString::fromWCharArray( pop->btn_txt[i] ).left( POPUP_TITLE_LENGHT );
            break;
        }
    }
    args << QString().setNum( pop->useAnimation );
    args << QString().setNum( pop->statusbar );

    args << QString::fromWCharArray( pop->traffic_icon ).left( POPUP_CONTENT_LENGHT );
    args << QString::fromWCharArray( pop->road_icon ).left( POPUP_CONTENT_LENGHT );
    args << QString::fromWCharArray( pop->road_number ).left( POPUP_CONTENT_LENGHT );
        emit showNaviPopup_Text( QVariant( args ) );
}
// SANGWOO_TEMP NAVI STOP
void CPopUp::callCustomText( POPUP_TYPE_CUSTOM_TEXT_T * pop )
{
   lprintf("[SystemPopUp]  CPopUp::POPUP_CUSTOM_TEXT");

   QStringList args;
   /** Set title */
   args << QString::fromWCharArray( pop->title ).left( POPUP_TITLE_LENGHT );

   /** Set button count */
   pop->btn_count = pop->btn_count > POPUP_BUTTON_MAX ? POPUP_BUTTON_MAX : pop->btn_count;
   args << QString().setNum( pop->btn_count );

   /** Set button text */
   for ( int i = 0; i < POPUP_BUTTON_MAX; i++ )
   {
      switch ( pop->btn[i] )
      {
         case POPUP_BUTTON_TYPE_OK:
            args << QString("STR_POPUP_OK");
            break;

         case POPUP_BUTTON_TYPE_CANCEL:
            args << QString("STR_POPUP_CANCEL");
            break;

         case POPUP_BUTTON_TYPE_TEXT:
         case POPUP_BUTTON_TYPE_ICON:
         default:
            args << QString::fromWCharArray( pop->btn_txt[i] ).left( POPUP_TITLE_LENGHT );
            break;
      }
   }

   /** Set message count */
   pop->msg_count = pop->msg_count > POPUP_TEXT_LINE_MAX ? POPUP_TEXT_LINE_MAX : pop->msg_count;
   args << QString().setNum( pop->msg_count );

   /** Set message text */
   for ( int i = 0; i < POPUP_TEXT_LINE_MAX; i++ )
   {
      /** Set text */
      args << QString::fromWCharArray( pop->msg_style[i].msg ).left( POPUP_CONTENT_LENGHT );
      /** Set font size */
      if ( pop->msg_style[i].size ) args << QString().setNum( pop->msg_style[i].size );
      else args << QString();
      /** Set color */
      if ( pop->msg_style[i].argb[0] ||
           pop->msg_style[i].argb[1] ||
           pop->msg_style[i].argb[2] ||
           pop->msg_style[i].argb[3] )
      {
         args << QString("#%1%2%3%4")
                             .arg( ( ( int ) ( pop->msg_style[i].argb[0] ) ) & 0xFF, 2, 16, QChar('0') )
                             .arg( ( ( int ) ( pop->msg_style[i].argb[1] ) ) & 0xFF, 2, 16, QChar('0') )
                             .arg( ( ( int ) ( pop->msg_style[i].argb[2] ) ) & 0xFF, 2, 16, QChar('0') )
                             .arg( ( ( int ) ( pop->msg_style[i].argb[3] ) ) & 0xFF, 2, 16, QChar('0') );
      }
      else args << QString();
      /** Set spacing */
      if ( pop->msg_style[i].spacing ) args << QString().setNum( pop->msg_style[i].spacing );
      else args << QString();
   }

   emit showPopup_CustomText( QVariant( args ) );
}


void CPopUp::setSharedMemData(int data)
{

    if(!m_pSharedMemory->isAttached()){
        lprintf("[SystemPopUp] CPopUp::setSharedMemData() : m_pSharedMemory->isAttached() FALSE!!");
        if(!m_pSharedMemory->attach(QSharedMemory::ReadWrite)){
           lprintf("[SystemPopUp] CPopUp::setSharedMemData() : m_pSharedMemory->attach() FAIL!!");
        }
        else{
            lprintf("[SystemPopUp] CPopUp::setSharedMemData() : m_pSharedMemory->attach() SUCCESS!!  %s", qPrintable(m_pSharedMemory->key()));
        }
    }
    else{
        lprintf("[SystemPopUp] CPopUp::setSharedMemData() : m_pSharedMemory->isAttached() TRUE!!");
    }

    bool bValid = m_pSharedMemory->lock();
    if(!bValid)
    {
        // debug message
        lprintf("[SystemPopUp] CPopUp: m_pSharedMemory->lock failed");
        // set with default value
        return;
    }
    memcpy(m_pSharedMemory->data() ,&data , sizeof(data));
    lprintf("[SystemPopUp] CPopUp:setSharedMemData m_pSharedMemory->data()  = %d", (int *)m_pSharedMemory->data());
    m_pSharedMemory->unlock();

}

int CPopUp::getSharedMemData()
{
    int data=-1;

    if(!m_pSharedMemory->isAttached()){
        lprintf("[SystemPopUp] CPopUp::getSharedMemData() : m_pSharedMemory->isAttached() FALSE!!");
        //m_pSharedMemory->setKey("PopUpPlugIn");
        if (!m_pSharedMemory->attach(QSharedMemory::ReadWrite)) {
            lprintf("[SystemPopUp] CPopUp::getSharedMemData() : m_pSharedMemory->attach() failed!!");
            return -1;
        }
    }
    else{
        lprintf("[SystemPopUp] CPopUp::getSharedMemData() : m_pSharedMemory->isAttached() TRUE!!");
    }
    memcpy(&data, m_pSharedMemory->data(), 4);
    lprintf("[SystemPopUp] CPopUp:getSharedMemData m_pSharedMemory->data() = %d", (int *)m_pSharedMemory->data());

    return data;
}

void CPopUp::lprintf(const char* fmt, ...) {


    va_list args;
    char msg[8192];


    va_start(args, fmt);
    vsnprintf(msg, 8192, fmt, args);
    va_end(args);

    Logger:: Instance()->Log(msg, Logger::PRIORITY_LOW);
//    Logger:: Instance()->Log(msg, Logger::PRIORITY_HI);
}

