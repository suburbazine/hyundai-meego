#ifndef	PARTITION_UPDATER_H_
#define	PARTITION_UPDATER_H_

#include "abstractupdater.h"

class StdinProcess;

class PartitionUpdater : public AbstractUpdater{
Q_OBJECT
public:
	PartitionUpdater(QObject * obj = 0);
	~PartitionUpdater();

	void setUpdateList(const QList<UpdateElement *> & list);
	void start();
	void stop();
	bool finish();

private:
	QList<UpdateElement *>	m_targets;
	int						m_updateEndCount;
	int						m_countRunningProcess;
	int						m_currentRunningIndex;

	void					startPartClone();
	int			 			findPartitionInfo(const QString & name);
	bool					setFlag(const QString & name, UpdateElement::ElementStatus);
	UpdateElement *			getUpdateElementByName(const QString & name);

	bool					mountUpdatePartition();
	void					cloneBootPartition();
	void					recordNewVersion();
	void					recordFailFlag();
	void					writeReport(QFile & file);
	bool					needSwapOperation();
	void					copyMicomEmergency();
        int                                     ssd_trim_cmd(int cmd_option);
	QList<UpdateElement *>	m_updateAllTargets;

private:
	void			parseProgress(QObject * obj, const QString & str);

	void			processFailPartition(const QString & name);

private slots:
	void			checkEndProcess(const QString &);
	void			processStdin(const QString &);
	void			countProcess();
	void			logStdin(const QString &);
};


#endif
