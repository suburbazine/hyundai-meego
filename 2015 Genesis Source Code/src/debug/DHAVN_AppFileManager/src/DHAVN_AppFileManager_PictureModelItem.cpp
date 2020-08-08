#include "hdr/DHAVN_AppFileManager_PictureModelItem.h"

PictureModelItem::PictureModelItem()
    : itemsCount(0)
{
}

QString PictureModelItem::getThumbPicture1() const
{
    return this->thumbPicture1;
}

void PictureModelItem::setThumbPicture1(const QString& value)
{
    this->thumbPicture1 = value;
}

QString PictureModelItem::getThumbPicture2() const
{
    return this->thumbPicture2;
}

void PictureModelItem::setThumbPicture2(const QString& value)
{
    this->thumbPicture2 = value;
}

qint32 PictureModelItem::getItemsCount() const
{
    return this->itemsCount;
}

void PictureModelItem::setItemsCount(qint32 value)
{
    this->itemsCount = value;
}

//{Added by Alexey Edelev 2012.10.25 Fix thumbnails issue
QString PictureModelItem::getThumbFilePath() const
{
    return thumbFilePath;
}

void PictureModelItem::setThumbFilePath(const QString& value)
{
    thumbFilePath = value;
}
//}Added by Alexey Edelev 2012.10.25 Fix thumbnails issue
