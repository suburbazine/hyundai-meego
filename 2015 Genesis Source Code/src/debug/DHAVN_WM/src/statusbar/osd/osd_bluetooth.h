#ifndef	OSD_BLUETOOTH_H_
#define	OSD_BLUETOOTH_H_

#include "abstract_osd_draw.h"

class OSDTextWidget;
class InfoIcon;
class OSDBluetooth : public AbstractOSDDraw{
public:
	OSDBluetooth(QPixmap *, int variant = 0);

	bool processCommand(const QStringList & args);
	int getMinimumArgCount();
	const QStringList & getCommand();

	void setUIPosition();
        void setMultiLineGeometry(QString str);

private:
	QLabel * modeIcon;
	QLabel * btLabel;
	QLabel * subLabel;
	QLabel * timeLabel;

	InfoIcon * infoIcon;
	QLabel * textLabel;

	QFont	 audioFont;
	QFont	 normalFont;

	int		 posX;

	QStringList command;
        OSDTextWidget *textLabel2Line;
};



#endif
