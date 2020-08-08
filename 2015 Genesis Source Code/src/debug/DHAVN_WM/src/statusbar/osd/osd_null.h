#ifndef	OSD_NULL_H_
#define	OSD_NULL_H_

#include "abstract_osd_draw.h"
#include <QLabel>

class OSDNull : public AbstractOSDDraw{
public:
	OSDNull(QPixmap *, int variant = 0);

	bool processCommand(const QStringList & args);

	int getMinimumArgCount();

	const QStringList & getCommand();
	void setUIPosition();

private:
	QStringList command;
};

#endif
