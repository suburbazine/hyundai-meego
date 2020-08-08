#include "DHAVN_AppFramework_AppTopWidget.h"
#include "DHAVN_AppFramework_Logger_Def.h"
#include <QTimer>
#define ONE_FRAME_TIME 250

AppTopWidget::AppTopWidget(QWidget *parent) :
    QWidget(parent),
    m_bHandleEvents(false),
    m_nDisplay(DISPLAY_AUTO)
{
   setParent(parent);
   setWindowOpacity(0);
   Logger::Instance()->Log("AppTopWidget::AppTopWidget");
   setGeometry(0, 0, 1, 1);
   QPalette p(palette());
   // Set background colour to black
   p.setColor(QPalette::Background, Qt::black);
   setPalette(p);
   show();
}

void AppTopWidget::startEventHandling( DISPLAY_T nDisplay )
{
   m_bHandleEvents = true;
   m_nDisplay = nDisplay;

   setGeometry( ((QWidget*)parent())->x(), ((QWidget*)parent())->y(), 1, 1);
   Logger::Instance()->Log(QString("AppTopWidget::startEventHandling x=") + QString::number(x())
                                   + " y=" + QString::number(y()) );
   raise();
   repaint();
   QTimer::singleShot(ONE_FRAME_TIME,this, SLOT(SlotEmitFinish()));
}

void AppTopWidget::paintEvent(QPaintEvent *)
{
   Logger::Instance()->Log("AppTopWidget::paintEvent");
   if( m_bHandleEvents == true )
   {
      Logger::Instance()->Log("AppTopWidget::paintEvent emit");
   //   QTimer::singleShot(ONE_FRAME_TIME,this, SLOT(SlotEmitFinish()));
      m_bHandleEvents = false;
   }

}

void AppTopWidget::SlotEmitFinish()
{
   emit SignalDrawFinish((uint)m_nDisplay);
}
