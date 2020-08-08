#ifndef	OSD_DMB_H_
#define	OSD_DMB_H_

#include "abstract_osd_draw.h"
class StatusIcon;

class OSDDmb : public AbstractOSDDraw{
public:
	OSDDmb(QPixmap *, int variant = 0);
	bool processCommand(const QStringList & args);

	int getMinimumArgCount();

	const QStringList & getCommand();

	void setUIPosition();

private:
	QLabel * bandLabel;

	QLabel * modeIcon;
	QLabel * infoIcon;

	QLabel * channelLabel;


	QLabel * statusLabel;
	StatusIcon * statusIcon;

	QPalette dialPalette;
	QPalette normalPalette;

	QStringList command;
};

#endif
