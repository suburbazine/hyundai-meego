#ifndef	OSD_MEDIA_H_
#define	OSD_MEDIA_H_

#include "abstract_osd_draw.h"
class StatusIcon;
class InfoIcon;

class OSDTextWidget;

class OSDMedia : public AbstractOSDDraw{
Q_OBJECT
public:
	enum {
		SearchAll = 1,
                SearchFolder,
                SearchFF,
                SearchREW
	};

	enum {
		RepeatAll = 1,
		RepeatOne,
		RepeatFolder
	};
	OSDMedia(QPixmap *, int variant = 0);

	bool processCommand(const QStringList & args);

	void setTimeText(const QString &);
	void clearTimeText();

	void setMediaStatus(int searchType, bool isShuffle, int repeatType);
	int getMinimumArgCount();
	const QStringList & getCommand();

	void setUIPosition();

private:
	QLabel * modeIcon;
	QLabel * deviceTypeLabel;
        OSDTextWidget *deviceTypeLabel2Line;
	QLabel * numberingLabel;
	QLabel * titleLabel;
	QLabel * titleSubLabel;
	QLabel * subLabel;
	StatusIcon * shuffleIcon;
	StatusIcon * searchIcon;
	StatusIcon * repeatIcon;

	QPalette	activePalette;
	QPalette	normalPalette;

	InfoIcon *	infoIcon;

	QLabel * timeLabel;

	bool	isMusic;
	int		timeDisplayType;
	int		titleMaxWidth;
	int		titleStartX;
        int             deviceTypeLabelX;
	QStringList command;

	void processTitleLabel(const QString &, const QString &, const QString &, bool modeExist);
};


#endif
