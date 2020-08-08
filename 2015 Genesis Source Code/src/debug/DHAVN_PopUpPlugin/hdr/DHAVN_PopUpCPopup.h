#ifndef POPUP_H
#define POPUP_H

#include <QTranslator>
#include <QtDeclarative/qdeclarative.h>
#include <QtDeclarative/QDeclarativeItem>
#include "DHAVN_PopUp_Import.h"
#include <DHAVN_AppFramework_AppEngineQML_Def.h>
#include <DHAVN_AppFramework_AppScreenView.h>
#include <QSharedMemory>
#include <DHAVN_AppFramework_Logger_Def.h>
#include "DHAVN_QRPMClient.h"


class CPopUp : public QDeclarativeItem
{
   Q_OBJECT

public:
    CPopUp(QDeclarativeItem *parent = 0);
    ~CPopUp();
    Q_INVOKABLE void showPopUp( QVariant );
    Q_INVOKABLE void responseButton( int );
    Q_INVOKABLE int  getScreenOffsetY( int nAppID );
    Q_INVOKABLE void setSharedMemData(int data);
    Q_INVOKABLE int getSharedMemData();

Q_SIGNALS:
   void showPopup_MessageBox( int size, QString message );
   void showPopup_Text( QVariant args );
   void showNaviPopup_Text( QVariant args );  
   void showPopup_CustomText( QVariant args );
   void showPopup_Warning( QVariant args );
   void showPopup_List( QVariant args );
   void closeSystemPopup();
   void popup_Response( int target, QVariant res );
   void startPopupTimer( int timeout );
   void showSystemPopup();

   void setignoreAutoClose(bool ignore);  // SANGWOO ignoreautoclose
   //void showPopup_Toast( QString message );
   void showPopup_Toast( QVariant args );
   void showPopup_XMAlert( QVariant args);

private:
   void callMessageBox ( POPUP_TYPE_MESSAGE_BOX_T * );
   void callCustomText ( POPUP_TYPE_CUSTOM_TEXT_T * );
   void callText ( POPUP_TYPE_TEXT_T * );
   void callNAVIText ( POPUP_TYPE_NAVI_TEXT_T * ); 
   void callWarning ( POPUP_TYPE_TEXT_T * );
   void callList( POPUP_TYPE_LIST_T * );
   void callToast( POPUP_TYPE_TOAST_T *);
   void callXMAlert(POPUP_TYPE_XM_ALERT_T *);
   void lprintf(const char* fmt, ...);
   QTranslator* m_pTranslator;
   POPUP_SHOW_T m_Popup;
   QSharedMemory *m_pSharedMemory;
   QRPMClient* m_pRpmClient;   //hjjung

};

QML_DECLARE_TYPE(CPopUp)

#endif // POPUP_H

