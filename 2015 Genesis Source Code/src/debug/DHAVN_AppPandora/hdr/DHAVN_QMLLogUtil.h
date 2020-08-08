#ifndef DHAVN_QMLLOGUTIL_H
#define DHAVN_QMLLOGUTIL_H

#include <QObject>
#include <DHAVN_LogUtil.h>

class CQMLLogUtil:public QObject
{
Q_OBJECT
    Q_ENUMS(LogSysID);

public:

	typedef enum   
	{	
        LOW_LOG,
        TRAC_LOG,
        MEDI_LOG,
        INFO_LOG,
        HIGH_LOG,
        SIGN_LOG,
        SLOT_LOG,
        TRAN_LOG,
        CRIT_LOG,
	}LogSysID;

    CQMLLogUtil();

    Q_INVOKABLE void log(QString inMessage , quint8 inLevel);

private:
    USE_LOG_UTIL

};

#endif // DHAVN_QMLLOGUTIL_H
