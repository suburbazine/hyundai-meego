#ifndef	CHECK_PARTITION_H_
#define	CHECK_PARTITION_H_

#include "abstractupdater.h"
#include <QStringList>

class CheckPartition : public AbstractUpdater{
Q_OBJECT
public:
	CheckPartition(QObject * obj = 0);	
	void checkPartitions();

	void setUpdateList(const QList<UpdateElement *> & list);
	void start();
	void stop();
	bool finish() { return true; }

private:
	QList<UpdateElement *> m_targetImages;

	void		parseProgress(const QString &);
	int			m_currentIndex;

	bool		m_stopFlag;

private slots:
	void startNext();
	void processStdin(const QString &);
};

#endif
