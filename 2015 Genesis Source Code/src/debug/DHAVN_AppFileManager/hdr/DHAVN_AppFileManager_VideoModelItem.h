#ifndef DHAVN_APPFILEMANAGER_VIDEOMODELITEM_H
#define DHAVN_APPFILEMANAGER_VIDEOMODELITEM_H

#include <QBool>
#include <QList>

#include "DHAVN_AppFileManager_BaseModelItem.h"

class VideoModelItem : public BaseModelItem
{
public:
    VideoModelItem();

    QString getThumbPicture1() const;
    void setThumbPicture1(const QString& value);

    QString getThumbPicture2() const;
    void setThumbPicture2(const QString& value);

    qint32 getItemsCount() const;
    void setItemsCount(qint32 value);

private:
    QString thumbPicture1;
    QString thumbPicture2;
    qint32 itemsCount;
};

#endif // DHAVN_APPFILEMANAGER_VIDEOMODELITEM_H
