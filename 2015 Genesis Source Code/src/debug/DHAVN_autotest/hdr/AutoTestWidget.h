#ifndef AUTOTESTWIDGET_H
#define AUTOTESTWIDGET_H

#include <QtGui/QWidget>
#include <QProcess>

class AutoTestWidget : public QWidget
{
    Q_OBJECT

public:
    AutoTestWidget(QWidget *parent = 0);
    ~AutoTestWidget();

    void ExecuteAgentVersion();
};

#endif // AUTOTESTWIDGET_H

























