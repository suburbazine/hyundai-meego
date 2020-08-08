#ifndef	OSD_RADIO_H_
#define	OSD_RADIO_H_

#include "abstract_osd_draw.h"
class StatusIcon;

class OSDRadio : public AbstractOSDDraw{
Q_OBJECT
public:
	OSDRadio(QPixmap *, int variant = 0);
	bool processCommand(const QStringList & args);

	int getMinimumArgCount();

	const QStringList & getCommand();
	void setUIPosition();

private:
	QLabel * infoIcon;
	QLabel * bandLabel;

	QLabel * modeIcon;

	QLabel * frequencyLabel;
	QLabel * stationLabel;

	QLabel * hdChannelLabel;

	StatusIcon * searchIcon;
	StatusIcon * autoStoreIcon; //dmchoi
	StatusIcon * hdIcon;

	QLabel * statusLabel;

	QPalette dialPalette;
	QPalette normalPalette;

	QStringList command;

    QPalette m_palette;
};

#endif
