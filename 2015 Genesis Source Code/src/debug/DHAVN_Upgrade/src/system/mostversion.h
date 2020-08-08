#ifndef	MOST_VERSION_H_
#define	MOST_VERSION_H_

#include "abstractversion.h"
#include <QDBusAbstractInterface>
#include "proxy/swdlproxy.h"
#include "proxy/diagnosisproxy.h"

#include <QTimer>


class MOSTVersion : public AbstractVersion{
Q_OBJECT
public:
	MOSTVersion(const QString & aMountPath, QObject * obj = 0);

	void filterUpdateList();

	void setDeviceUpdateList(const QList<UpdateElement *> & updateList);

private:
	SWDLInterface *			swdlInterface;
	DiagnosisInterface *	diagnosisInterface;

	UpdateElement *			m_iboxUpdate;
	UpdateElement *			m_ampUpdate;

	QTimer *				m_versionTimer;

	QString					m_iboxHuVersion;
	QString					m_iboxHuDate;
	QString					m_iboxHuVariant;

	QString					m_amplifierVersion;

	void					startGetIBoxVersion();
	void					startGetAmplifierVersion();

	int						getFileCount(const QString & mountPoint, int type);
	bool					compareIboxVersion(const QString &, const QString &);

	bool					copyIboxFiles();
	bool					copyAmplifierFiles();

private slots:
	void amplifierHwIdentifier(uint, const QString &);
	void iBoxSWVersionStatus(const QStringList &);
};

#endif
