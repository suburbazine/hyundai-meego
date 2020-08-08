#ifndef IAPTHREAD_H
#define IAPTHREAD_H


#include <QThread>

class iAPThread : public QThread
{
    Q_OBJECT

public:

	/*
	 * Modified by jonghwan.cho@lge.com	2013.03.20.
    void run(){
        ////LOG_TRACE <<"iAPThread:run: Thread started"<<LOG_END;
        exec();
        ////LOG_TRACE <<"iAPThread:run: Thread exec lop OUT"<<LOG_END;
    }
	 */
	iAPThread();
	~iAPThread();
	void run();
	void stop();
};
#endif // IAPTHREAD_H
