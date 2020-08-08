#ifndef	OSD_DRIVING_H_
#define OSD_DRIVING_H_

#include "abstract_osd_draw.h"

class OSDDriving : public AbstractOSDDraw{
public:
	OSDDriving(QPixmap *, int variant = 0);

	bool processCommand(const QStringList & args);
	int getMinimumArgCount();
	const QStringList & getCommand();

	void setUIPosition();

private:
	QLabel * modeIcon;
	QLabel * driveLabel;
	QLabel * driveIcon;
	QLabel * textLabel;
//	QLabel * sepPixmap;
	QStringList command;
};

#endif
