#ifndef	CHECK_ESSENTIALS_H_
#define	CHECK_ESSENTIALS_H_

#include <QObject>

class CheckEssentials{
public:
	CheckEssentials(const QString & mountPoint, const QString & checksum);

	bool isEssentialValidate();

private:
	bool m_essentialValidate;

	QString getEssentialChecksum(const QString & path);
};

#endif
