#ifndef DHAVN_APPFILEMANAGER_PICTUREMODELITEM_H
#define DHAVN_APPFILEMANAGER_PICTUREMODELITEM_H

#include <QList>
#include "DHAVN_AppFileManager_BaseModelItem.h"

class PictureModelItem : public BaseModelItem
{
public:
    PictureModelItem();

    QString getThumbPicture1() const;
    void setThumbPicture1(const QString& value);

    QString getThumbPicture2() const;
    void setThumbPicture2(const QString& value);

    qint32 getItemsCount() const;
    void setItemsCount(qint32 value);

//{Added by Alexey Edelev 2012.10.25 Fix thumbnails issue
    QString getThumbFilePath() const;
    void setThumbFilePath(const QString& value);
//}Added by Alexey Edelev 2012.10.25 Fix thumbnails issue

private:
    QString thumbPicture1;
    QString thumbPicture2;
    QString thumbFilePath;//Added by Alexey Edelev 2012.10.25 Fix thumbnails issue
    qint32 itemsCount;
};

#endif // DHAVN_APPFILEMANAGER_PICTUREMODELITEM_H
