#ifndef DHAVN_APPFILEMANAGER_AUDIOMODELITEM_H
#define DHAVN_APPFILEMANAGER_AUDIOMODELITEM_H

#include <QVariant>
#include <QString>
#include <QBool>

#include "DHAVN_AppFileManager_BaseModelItem.h"

class AudioModelItem : public BaseModelItem
{
public:
    AudioModelItem();

    QString getItemCover() const;
    void setItemCover(const QString& value);

    QString getItemAlbum() const;
    void setItemAlbum(const QString& value);

    QString getItemArtist() const;
    void setItemArtist(const QString& value);

private:
    QString itemCover;
    QString itemAlbum;
    QString itemArtist;
};

#endif // DHAVN_APPFILEMANAGER_AUDIOMODELITEM_H
