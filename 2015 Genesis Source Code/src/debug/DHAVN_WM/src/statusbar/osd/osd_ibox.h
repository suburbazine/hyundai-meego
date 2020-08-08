#ifndef	OSD_IBOX_H_
#define	OSD_IBOX_H_

#include "abstract_osd_draw.h"

class OSDTextWidget;
class InfoIcon;
class StatusIcon;

class IBoxMessage;

class OSDIBox : public AbstractOSDDraw{
public:
	enum {
		ScanAll = 1,
		ScanFolder
	};
	enum {
		Shuffle = 1
	};

	enum {
		RepeatAll = 1,
		RepeatOne,
		RepeatFolder
	};

	OSDIBox(QPixmap *, int variant = 0);

	bool processCommand(const QStringList & args);

	int getMinimumArgCount();

	const QStringList & getCommand();

	void setUIPosition();

private:
	QStringList command;

	QLabel * m_modeIcon;
	OSDTextWidget * m_modeText;

	InfoIcon * m_infoIcon;
	OSDTextWidget * m_infoText;

	StatusIcon * m_shuffleIcon;
	StatusIcon * m_repeatIcon;
	StatusIcon * m_scanIcon;
	QLabel * m_timeLabel;	

	QPalette m_normalPalette;
	QPalette m_dialPalette;

	int infoStringToIndex(const QString &);

	void applyTextColor(const QString & str);

	QString fetchIconFromText(const QString &, int &);

	void hideWidgets();
	void processCommonOSD(const QString &);
	void processModeOSD(const QString & modeString, const QString & infoString );
	void parsingStatusOSD(const QString &);

	void processModeIcon();
	void setStatus(const QString &);
	void setMediaStatus(int scanType, bool isShuffle, int repeatType);

	void setTimeText(const QString & str);

	void findOccurence(QList<int> &, const QString &, const QString &);



	IBoxMessage * m_message;
};

#endif
