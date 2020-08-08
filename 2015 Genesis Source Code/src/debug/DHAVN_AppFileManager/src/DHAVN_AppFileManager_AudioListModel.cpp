#include "hdr/DHAVN_AppFileManager_AudioListModel.h"
#include <QHash>
#include <QDebug>
#include <QTextCodec>

AudioListModel::AudioListModel(QObject *parent) :
    BaseListModel(parent)
{
    roles[AudioIconRole] = "itemIcon";
    roles[AudioViewName] = "itemViewName";

    setRoleNames(roles);
}

void AudioListModel::addDataItem(const AudioModelItem &data)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    this->listItems << data;
    endInsertRows();
}

int AudioListModel::rowCount(const QModelIndex & /* parent */) const
{
    return this->listItems.count();
}

QVariant AudioListModel::data(const QModelIndex & index, int role) const
{
    if (index.row() < 0 || index.row() > this->listItems.count())
        return QVariant();

    const AudioModelItem &data = this->listItems[index.row()];
    if (role == AudioIconRole) {
        return data.getItemCover();
    } else if (role == AudioViewName) {
        return encodeString(data.getItemName());
    }

    return BaseListModel::data(index, role);
}

bool AudioListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() > this->listItems.count())
    {
        return false;
    }

    return BaseListModel::setData(index, value, role);
}

void AudioListModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    this->listItems.clear();
    endRemoveRows();
    clearCheckedElements();
}
\
bool AudioListModel::updateElement(int index, const AudioModelItem &data)
{
    if (index < 0 || index > this->listItems.count())
    {
        return false;
    }

    this->listItems[index] = data;
    QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex, modelIndex);

    return true;
}

const BaseModelItem* AudioListModel::getListItem(int index) const
{
    if (index < 0 || index > this->listItems.count())
    {
        return NULL;
    }

    return &(this->listItems[index]);
}

BaseModelItem* AudioListModel::getListItem(int index)
{
    if (index < 0 || index > this->listItems.count())
    {
        return NULL;
    }

    return &(this->listItems[index]);
}

int AudioListModel::getListCount() const//Changed by Alexey Edelev 2012.10.18 CR14766
{
    return this->listItems.count();
}

void AudioListModel::removeDataItem(int index)
{
    if (index < 0 || index >= getListCount())
    {
        return;
    }

    // MP_LOG << "AudioListModel::removeDataItem(): remove item index=" << index << LOG_ENDL;

    beginRemoveRows(QModelIndex(), index, index);
    this->listItems.removeAt(index);
    endRemoveRows();
}

QString
AudioListModel::encodeString(QString inputString) const
{
    if ( isUtf8( inputString.toAscii() ) )
    {
        return inputString;
    }

    QTextCodec *codec = QTextCodec::codecForName("EUC-KR");
    QString result;

    // MP_LOG << "AudioListModel::encodeString inputString = " << inputString << LOG_ENDL;

    QTextStream in(inputString.toAscii());

    in.setAutoDetectUnicode(false);
    in.setCodec(codec);

    result = in.readAll ();

    // MP_LOG << "AudioListModel::encodeString output String = " << result << LOG_ENDL;
    return result;
}

bool
AudioListModel::isUtf8(const char * str ) const
{
    int str_bytes = 0;
    if( str == NULL )
        return false;

    str_bytes = strlen(str);
    bool isUtf8(true);
    int index = 0;

    while( index < str_bytes && isUtf8 )
    {
        char achar = str[index];
        int supplemental_bytes = 0;
        if( (achar & 0x80) == 0 )
        {
            ++index;
        }
        else if( (achar &0xF8) == 0xF0 )
        {
            ++index;
            supplemental_bytes = 3;
        }
        else if( (achar &0xE0) == 0xE0 )
        {
            ++index;
            supplemental_bytes = 2;
        }
        else if( (achar &0xE0) == 0xC0 )
        {
            ++index;
            supplemental_bytes = 1;
        }
        else
        {
            isUtf8 = false;
        }

        while( isUtf8 && supplemental_bytes-- )
        {
            if( index >= str_bytes )
            {
                isUtf8 = false;
            }
            else if( (str[index++] & 0xC0) != 0x80 )
            {
                isUtf8 = false;
            }
        }
    }

    return isUtf8;
}
