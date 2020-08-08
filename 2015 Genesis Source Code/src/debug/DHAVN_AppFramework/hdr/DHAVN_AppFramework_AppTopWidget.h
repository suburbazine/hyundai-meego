#ifndef DHAVN_APPFRAMEWORK_APPTOPWIDGET_H
#define DHAVN_APPFRAMEWORK_APPTOPWIDGET_H

#include <QWidget>
#include "DHAVN_AppFramework_Def.h"

class AppTopWidget : public QWidget
{
    Q_OBJECT
public:
    AppTopWidget(QWidget *parent = 0);

public slots:
    void startEventHandling(DISPLAY_T nDisplay = DISPLAY_AUTO );

signals:
    void SignalDrawFinish(uint);
private:
    void paintEvent(QPaintEvent *event);
    bool m_bHandleEvents;
    DISPLAY_T m_nDisplay;

private slots:
    void SlotEmitFinish();
};

#endif // DHAVN_APPFRAMEWORK_APPTOPWIDGET_H
