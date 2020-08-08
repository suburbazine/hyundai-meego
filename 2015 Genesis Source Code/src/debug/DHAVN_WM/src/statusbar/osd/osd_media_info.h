#ifndef	OSD_MEDIA_INFO_H_
#define OSD_MEDIA_INFO_H_

#include "abstract_osd_draw.h"
#include <QLabel>

class InfoIcon;
class OSDTextWidget;
class OSDMediaInfo : public AbstractOSDDraw{
public:
    OSDMediaInfo(QPixmap *, int variant = 0);

    bool processCommand(const QStringList & args);

    int getMinimumArgCount();

    const QStringList & getCommand();

    void setUIPosition();

        void setMultiLineGeometry(QString str);

    void setUIGeometry(int startY, int height);
private:
    QStringList command;

    QLabel *	modeIcon;
    QLabel *	deviceTypeLabel;
    OSDTextWidget *deviceTypeLabel2Line;
    //QLabel *	sepPixmap;
    InfoIcon *	infoIcon;
    QLabel *	textLabel;
    OSDTextWidget *textLabel2Line;
    int startX;
};


#endif
