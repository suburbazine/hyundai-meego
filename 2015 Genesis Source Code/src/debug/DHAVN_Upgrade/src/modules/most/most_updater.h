#ifndef	MOST_UPDATER_H_
#define	MOST_UPDATER_H_

#include "abstractupdater.h"
#include <QDBusAbstractInterface>
#include "proxy/swdlproxy.h"
#include "most.h"

class MscAbstract;

class MOSTUpdater : public AbstractUpdater{
Q_OBJECT
public:
	MOSTUpdater(QObject * obj = 0);
	~MOSTUpdater();

	void setUpdateList(const QList<UpdateElement *> & list);
	void start();
	void stop();
	bool finish();

private:
	enum { NumberOfSteps = 6 };
	SWDLInterface * m_swdlInterface;
	UpdateElement * m_iboxUpdate;
	UpdateElement * m_ampUpdate;
	MscAbstract ** m_currentSteps;

	void startIBoxUpdate();
	void startAmplifierUpdate();

	MscAbstract ** createSteps(SWDLID::SWDLID);

	SWDLID::SWDLID m_currentUpdate;

	QString sequenceToString(int);
	int sequenceToPercentage(int);

	QString getCurrentUpdateName();

private slots:
	void statusChanged(int, int, int, const QString &);
	void cannotReceiveResponse();

	void blockTransfered(quint64, quint64);

	void processIBoxWaitCount(int);

	void finishCurrentUpdate(bool isError = false, const QString & = QString());
};

#endif
