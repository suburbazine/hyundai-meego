#ifndef	OSD_COMMON_H_
#define	OSD_COMMON_H_

#include "abstract_osd_draw.h"
#include <QLabel>

class OSDTextWidget;

class OSDCommon : public AbstractOSDDraw{
public:
	OSDCommon(QPixmap *, int variant = 0);

	bool processCommand(const QStringList & args);

	int getMinimumArgCount();

	const QStringList & getCommand();

	void setUIPosition();

private:
	QString text;

	QLabel * iconLabel;
	OSDTextWidget * textLabel;

	QStringList command;
};

#endif
