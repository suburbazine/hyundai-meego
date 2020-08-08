#ifndef	MSC_REQUEST_DOWNLOAD_H_
#define	MSC_REQUEST_DOWNLOAD_H_

#include "msc_abstract.h"

class MscRequestDownload : public MscAbstract {
Q_OBJECT
public:
	MscRequestDownload(QObject * obj, QDBusAbstractInterface * p, SWDLID::SWDLID swdlid);
	~MscRequestDownload();

	void start();

private:
	void startRequestDownload();

private slots:
	void RequestDownload(const QDBusVariant &);
};

#endif
