#ifndef	OSD_SXM_INFO_H_
#define	OSD_SXM_INFO_H_

#include "abstract_osd_draw.h"

class OSDSxmInfo : public AbstractOSDDraw{
Q_OBJECT
public:
	OSDSxmInfo(QPixmap *, int variant = 0);
	int getMinimumArgCount();

	bool processCommand(const QStringList & args);
	const QStringList & getCommand();

	void setUIPosition();

private:
	QStringList command;
	QLabel *	titleLabel;
	QLabel *	bandLabel;
	QLabel *	modeIcon;
	QLabel *	infoIcon;

	QLabel *	stationLabel;
	QLabel *	statusLabel;

	int			titleStartX;
};

#endif
