#ifndef	OSD_TA_H_
#define	OSD_TA_H_

#include "abstract_osd_draw.h"
class StatusIcon;

class OSDTa : public AbstractOSDDraw{
Q_OBJECT
public:
	OSDTa(QPixmap *, int variant = 0);
	bool processCommand(const QStringList & args);

	int getMinimumArgCount();

	const QStringList & getCommand();

	void setUIPosition();

private:
	QLabel * bandLabel;

	QLabel * modeIcon;

	QLabel * trafficLabel;
	QLabel * stationLabel;

	QLabel * sepPixmap;

	QStringList command;
};

#endif
