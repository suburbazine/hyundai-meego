#ifndef	OSD_AHA_H_
#define	OSD_AHA_H_

#include "abstract_osd_draw.h"

class OSDTextWidget;
class OSDAha : public AbstractOSDDraw{
public:
	OSDAha(QPixmap *, int variant = 0);
	bool processCommand(const QStringList & args);

	int getMinimumArgCount();

	const QStringList & getCommand();

	void setUIPosition();
        void setMultiLineGeometry(QString str);

private:
	QLabel * bandLabel;

	QLabel * modeIcon;
	QLabel * infoIcon;

	QLabel * channelLabel;

	QLabel * sepPixmap;

	QPalette dialPalette;
	QPalette normalPalette;

	QStringList command;
        OSDTextWidget *textLabel2Line;
        int startX;
};

#endif
