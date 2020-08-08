#ifndef	OSD_SEATCONTROL_H_
#define	OSD_SEATCONTROL_H_

#include "abstract_osd_draw.h"

class OSDSeatControl : public AbstractOSDDraw{
public:
	OSDSeatControl(QPixmap *, int variant = 0);

	bool processCommand(const QStringList & args);
	int getMinimumArgCount();
	const QStringList & getCommand();

	void setUIPosition();

private:
	QLabel * modeIcon;
	QLabel * seatLabel;
	QLabel * textLabel;
	QStringList command;
};


#endif
