#ifndef DHAVN_APPHOMESCREEN_ViewController_H
#define DHAVN_APPHOMESCREEN_ViewController_H

#include <QObject>
#include <QDeclarativeContext>
#include <QApplication>
#include <QtGui/QDesktopWidget>
#include <QGLWidget>
#include <QtDeclarative>
#include <QTimer>
#include <DHAVN_AppHomeScreen_InitData.h>
#include <DHAVN_AppHomeScreen_View.h>
#include <DHAVN_AppHomeScreen_LocalizeTrigger.h>
#include "osd_manager.h"
#include <DHAVN_AppHomeScreen_Log_Def.h>

class ViewControllerHelper;

class ViewController : public QObject
{
   Q_OBJECT

public:
    friend class ViewControllerHelper;

   ViewController( int nDisplay, QDeclarativeContext *pCtxt, InitData *pInitData, LocalizeTrigger *pLocTrigger );
   ~ViewController();

   Q_INVOKABLE void ShowPopUp( int nAppId, int nViewId );
   Q_INVOKABLE void SetFocusIconIndex( int nViewId, int iconIdx );
   Q_INVOKABLE void SetFocusEnabled( bool bEnabled );

   bool AddView( HSDefP::VIEW_ID_T nView, QString sViewName );
   bool UpdateSubMenuContext( HSDefP::VIEW_ID_T nView );
   bool RemoveView( HSDefP::VIEW_ID_T nView = HSDefP::VIEW_ID_INVALID );
   bool ShowRootView();
   bool IsRootView();
   //bool IsDrivingRestrictView();
   Q_INVOKABLE int GetDisplay(){ return m_nDisplay; }
   void SetDisplay(int nDisplay) { if (m_nDisplay != nDisplay) m_nDisplay = nDisplay; }
   void SetForeground( bool bOnTop );
   bool GetMeOnTop(){ return m_bMeOnTop; }
   bool GetFocusEnabled(){ return m_bFocusEnabled; }
   void UpdateFocusIconIdx();
   int GetPopUpType(){ return (int)m_nPopUpType; }
   void SetPopUpType( int nType );
   void SetCurrentMediaName (QString str) { m_sCurrentMediaName = str; }
   void UpdateViewContext();
   QString GetTitleText(){ return m_sTitleText; }
   int GetTitleAlign(){ return (int)m_nTitleAlign; }
   bool GetJogPressed(){ return m_bJogPressed; }

   View* getCurrentView() { return m_viewStack[ m_viewStack.size() - 1 ]; }

   void setMediaInfoUpdated (bool value);

   Q_PROPERTY( QString sTitleText READ GetTitleText NOTIFY UpdateTitleText );
   Q_PROPERTY( int nTitleAlign READ GetTitleAlign NOTIFY UpdateTitleAlign );
   Q_PROPERTY( bool bFocusEnabled READ GetFocusEnabled WRITE SetFocusEnabled NOTIFY UpdateFocusEnabled );
   Q_PROPERTY( bool bJogPressed READ GetJogPressed WRITE SetJogPressed NOTIFY UpdateJogPressed );
   Q_PROPERTY( int nPopUpType READ GetPopUpType WRITE SetPopUpType NOTIFY UpdatePopUpType );

public slots:
   //void UpdateHomeButtonState( HSDefP::HB_STATE nState = HSDefP::HB_STATE_AUTO );
   void onTimeoutShowPopup();
   void UpdateTitle(QString title);

private:
   void SetTitleAlign( Qt::AlignmentFlag nFlag );
   //void CreateTimerForSetHBState();
   //void SetHomeButtonState( HSDefP::HB_STATE nState );

   /** Q_PROPERTY getters */

   //bool GetHBVisible(){ return m_bHBVisible; }

   /** Q_PROPERTY setters */
   void SetJogPressed( bool bPressed );
   void SetHBVisible( bool bVisible );

signals:
   /** Q_PROPERTY signals */
   void UpdateTitleText();
   void UpdateTitleAlign();
   void UpdateFocusEnabled();
   void UpdateJogPressed();
   void UpdatePopUpType();
   void closepopup(int screen);

private slots:
   void UpdateTitle();

private:
   QString m_sCurrentMediaName;
   QDeclarativeContext *m_pCtxt;
   InitData *m_pInitData;
   LocalizeTrigger *m_pLocTrigger;
   QList<View*> m_viewStack;
   int m_nDisplay;
   QHash<HSDefP::VIEW_ID_T, int> m_FocusIconIdxList;

   /** Q_PROPERTY members */
   QString m_sTitleText;
   Qt::AlignmentFlag m_nTitleAlign;
   bool m_bFocusEnabled;
   bool m_bJogPressed;
   HSDefP::POPUP_TYPE_T m_nPopUpType;
   //bool m_bHBVisible;
   bool m_bMeOnTop;

   OSD_manager *m_pOSD_manager;
   bool m_bMediaInfoUpdated;
};

class ViewControllerHelper
{
public:
    static void SwapViewControllerContentForDCSwap(ViewController* pView1, ViewController* pView2);
};

#endif // DHAVN_APPHOMESCREEN_ViewController_H
