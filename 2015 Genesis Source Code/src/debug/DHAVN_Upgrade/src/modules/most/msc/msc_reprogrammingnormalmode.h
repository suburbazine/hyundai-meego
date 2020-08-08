#ifndef	REPROGRAMMING_NORMAL_MODE_H_
#define	REPROGRAMMING_NORMAL_MODE_H_

#include "msc_abstract.h"

class MscReprogrammingNormalMode : public MscAbstract {
Q_OBJECT
public:
	MscReprogrammingNormalMode(QObject * obj, QDBusAbstractInterface * p, SWDLID::SWDLID swdlid);
	~MscReprogrammingNormalMode();
	void start();

private:
	void startReprogrammingNormal();

private slots:
	void ReprogrammingMode(const QDBusVariant &);
};

#endif
