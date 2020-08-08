#include "hdr/DHAVN_AppFileManager_VideoModelItem.h"

VideoModelItem::VideoModelItem():
    itemsCount (0) //fix coverity issue
{
}

QString VideoModelItem::getThumbPicture1() const
{
    return this->thumbPicture1;
}

void VideoModelItem::setThumbPicture1(const QString& value)
{
    this->thumbPicture1 = value;
}

QString VideoModelItem::getThumbPicture2() const
{
    return this->thumbPicture2;
}

void VideoModelItem::setThumbPicture2(const QString& value)
{
    this->thumbPicture2 = value;
}

qint32 VideoModelItem::getItemsCount() const
{
    return this->itemsCount;
}

void VideoModelItem::setItemsCount(qint32 value)
{
    this->itemsCount = value;
}
