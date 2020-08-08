#include "DHAVN_AppFileManager_AudioModelItem.h"

AudioModelItem::AudioModelItem()
{
}

QString AudioModelItem::getItemCover() const
{
    return this->itemCover;
}

void AudioModelItem::setItemCover(const QString& value)
{
    this->itemCover = value;
}

QString AudioModelItem::getItemAlbum() const
{
    return this->itemAlbum;
}

void AudioModelItem::setItemAlbum(const QString& value)
{
    this->itemAlbum = value;
}

QString AudioModelItem::getItemArtist() const
{
    return this->itemArtist;
}

void AudioModelItem::setItemArtist(const QString& value)
{
    this->itemArtist = value;
}
