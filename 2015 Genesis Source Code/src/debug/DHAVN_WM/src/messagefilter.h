#ifndef	MESSAGE_FILTER_H_
#define	MESSAGE_FILTER_H_

#include <QStringList>

class MessageFilter{
public:
	MessageFilter();
	void setCurrentMode(int);
	bool canPass(const QStringList & list);

	enum Mode {
		MODE_UNKNOWN,
		MODE_NONE,
                MODE_AVOFF,
		MODE_RADIO,
		MODE_USB,
		MODE_IPOD,
		MODE_AHA,
		MODE_PANDORA,
		MODE_DISC,
		MODE_JUKEBOX,
		MODE_BT_AUDIO,
		MODE_AUX,
		MODE_DMB,
		MODE_SXM,
		MODE_DAB,
                MODE_EMERGENCY, //dmchoi
	};
	Mode m_currentMode;	

        QString getModeString(Mode mode);

private:
	Mode getMode(const QStringList & list);

	QStringList m_radioGroup;
	QStringList m_mediaCommand;

	bool isRadioGroup(const QString & command);
	bool isMediaCommand(const QString & command);


	Mode getMediaMode(const QString & mode);
};


#endif
