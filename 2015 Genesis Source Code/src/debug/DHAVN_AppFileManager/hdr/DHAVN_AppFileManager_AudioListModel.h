#ifndef DHAVN_APPFILEMANAGER_AUDIOLISTMODEL_H
#define DHAVN_APPFILEMANAGER_AUDIOLISTMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "DHAVN_AppFileManager_AudioModelItem.h"
#include "DHAVN_AppFileManager_BaseListModel.h"

class AudioListModel : public BaseListModel
{
    Q_OBJECT
public:
    enum AudioModelRoles {
        AudioIconRole = BaseListModel::BaseUserRole,
        AudioAlbumRole,
        AudioSignerRole,
        AudioViewName
    };

    explicit AudioListModel(QObject *parent = 0);

    void addDataItem(const AudioModelItem &data);
    virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    void clear();
    bool updateElement(int index, const AudioModelItem &data);
    virtual int getListCount() const;//Changed by Alexey Edelev 2012.10.18 CR14766

protected:
    virtual const BaseModelItem* getListItem(int index) const;
    virtual BaseModelItem* getListItem(int index);
    virtual void removeDataItem(int index);
    QString encodeString(QString inputString) const;
    bool isUtf8(const char * str ) const;

private:
    QList<AudioModelItem> listItems;
};

#endif // DHAVN_APPFILEMANAGER_AUDIOLISTMODEL_H
