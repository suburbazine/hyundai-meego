#ifndef	OSD_VOLUME_H_
#define	OSD_VOLUME_H_

#include "abstract_osd_draw.h"
#include <QLabel>

class OSDTextWidget;

class OSDVolume : public AbstractOSDDraw{
public:
	enum { MaxVolumeStep = 46 };

	OSDVolume(QPixmap *, int variant = 0);

	bool processCommand(const QStringList & args);

	int getMinimumArgCount();

	const QStringList & getCommand();

	void setUIPosition();

private:
	QString text;

	QLabel * iconLabel;
	OSDTextWidget * textLabel;
	OSDTextWidget * volLabel;
	QLabel * volPixLabel;

	QStringList command;
	QPixmap * volPixmaps[MaxVolumeStep];
};



#endif

