#ifndef	FIRMWARE_VERSION_H_
#define	FIRMWARE_VERSION_H_

#include "abstractversion.h"
#include "cancontroller.h"
#include <QHash>

class FirmwareVersion : public AbstractVersion {
Q_OBJECT
public:
	FirmwareVersion(const QString & aMountPath, QObject * obj = 0);
	~FirmwareVersion();

	void filterUpdateList();

	void setFirmwareUpdateList(const QList<UpdateElement *> & list, bool isUser);

private:
	void variantFilter();
        void lcdtypeFilter();
	void duplicateFilter();
	void fileFilter();
	void fileCopy();
	void changePath();

	void versionFilter();

	bool m_isUser;
	bool m_getLCDVersion;

	bool removeDir(const QString & name);
	void copyDir(const QString & from, const QString & to);

	void checkInic();

	QHash<int, QByteArray> m_canVersion;

	void checkCanDevice(int type, const QString & device);

	int existFirmware(const QString & device);

	void gpsVariantCheck();
	void biosVariantCheck();
	void carTypeCheckForMicom();

	CanController * canController;

private slots:
	void parseCanVersion(int, const QByteArray &);
};


#endif
