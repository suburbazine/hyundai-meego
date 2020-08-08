#ifndef	MSC_IDENTIFICATIION_H_
#define	MSC_IDENTIFICATIION_H_

#include "msc_abstract.h"

class MscIdentification : public MscAbstract{
Q_OBJECT
public:
	MscIdentification(QObject * obj, QDBusAbstractInterface * p, SWDLID::SWDLID swdlid);
	~MscIdentification();

	void readHWIdentifier();
	void readHWVersion();
	void readDevName();
	void readNumberOfModule();
	void readModuleVersion();

	void start();

private slots:
	void ReadHWIdentifier(uint, const QString &);
	void ReadDevName(uint, const QString &);
	void ReadHWVersion(uint, const QString &);
	void ReadNumberofModule(uint, const QByteArray &);
	void ReadModuleVersion(uint, const QDBusVariant &);
};

#endif
