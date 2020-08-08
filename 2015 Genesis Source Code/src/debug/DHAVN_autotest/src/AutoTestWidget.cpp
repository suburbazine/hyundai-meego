#include "AutoTestWidget.h"
#include <stdio.h>
#include <QElapsedTimer>
#include <QDebug>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>

#define CMD_AGENT_EXECUTE "/usr/bin/sudo -u meego /app/bin/matagent"
#define CMD_AGENT_VERSION "/app/bin/matagent"

AutoTestWidget::AutoTestWidget(QWidget *parent)
    : QWidget(parent)
{
    ExecuteAgentVersion();
}

void AutoTestWidget::ExecuteAgentVersion()
{
    QString prog = CMD_AGENT_VERSION;
    QStringList arguments;
    arguments << "-v";

    QProcess myProcess;
    myProcess.start(prog, arguments);
    myProcess.waitForFinished(500);

    QByteArray result = myProcess.readAll();
    QList<QByteArray> pList = result.split('\n');

    qDebug("Mat Agent Version : [%s]", pList.at(0).data());
}

AutoTestWidget::~AutoTestWidget()
{
}


