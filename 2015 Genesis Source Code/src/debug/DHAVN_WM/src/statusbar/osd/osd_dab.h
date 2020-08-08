#ifndef	OSD_DAB_H_
#define	OSD_DAB_H_

#include "abstract_osd_draw.h"
class StatusIcon;

class OSDDab : public AbstractOSDDraw{
Q_OBJECT
public:
	OSDDab(QPixmap *, int variant = 0);
	bool processCommand(const QStringList & args);

	int getMinimumArgCount();

	const QStringList & getCommand();

	void setUIPosition();

private:
	QLabel * bandLabel;
	QLabel * infoIcon;

	QLabel * modeIcon;

	QLabel * ensembleLabel;
	QLabel * stationLabel;

	QLabel * statusLabel;

	StatusIcon * searchIcon;

	QPalette dialPalette;
	QPalette normalPalette;

	QStringList command;
};

#endif

