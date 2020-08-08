/*
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * History
 * 01-Aug-2011      GADDAM SATEESH
 ******************************************************************
 */
#ifndef DHAVN_GRACENOTEPLUGINPROCESSTHREAD_H
#define DHAVN_GRACENOTEPLUGINPROCESSTHREAD_H

#include <QThread>
#include<QDebug>
#include <DHAVN_LogUtil.h>

class GNThread : public QThread
{
    Q_OBJECT
    USE_LOG_UTIL

public:

    void run();

};

#endif // DHAVN_GRACENOTEPLUGINPROCESSTHREAD_H
