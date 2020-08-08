/*
 * CSpiCommThread.h
 *
 *  Created on: 2011. 9. 3.
 *      Author: yongwoo1.park
 */

#ifndef CSPICOMMTHREAD_H_
#define CSPICOMMTHREAD_H_

#include <QObject>
#include <QThread>
#include "util.h"


class CSpiCommThread : public QThread{
	Q_OBJECT
public:
	CSpiCommThread(int fd, QObject * p = 0);
	virtual ~CSpiCommThread();

protected:
	void		run();

private:
	int			fd;
	inline int	tokenize(const QByteArray &);
signals:
	void		dataArrived(const QByteArray &);
	void		printDebug(const QByteArray &);
};

#endif /* CSPICOMMTHREAD_H_ */
