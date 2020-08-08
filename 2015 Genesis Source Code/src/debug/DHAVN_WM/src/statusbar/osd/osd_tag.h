#ifndef	OSD_TAG_H_
#define	OSD_TAG_H_

#include "abstract_osd_draw.h"
class StatusIcon;

class OSDTagging : public AbstractOSDDraw{
Q_OBJECT
public:
	OSDTagging(QPixmap *, int variant = 0);
	bool processCommand(const QStringList & args);

	int getMinimumArgCount();

	const QStringList & getCommand();

	void setUIPosition();

private:
	QLabel * bandLabel;

	QLabel * modeIcon;

	QLabel * titleLabel;
	QLabel * descLabel;

	QLabel * sepPixmap;

	int	titleStartX;

	QStringList command;
};

#endif
