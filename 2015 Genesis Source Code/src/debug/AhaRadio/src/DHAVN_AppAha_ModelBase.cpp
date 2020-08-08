#include "DHAVN_AppAha_ModelBase.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAppAha_ModelBase::CAppAha_ModelBase(QObject *parent)
    : QAbstractListModel(parent), i_current_row(-1)
{
    QHash<int, QByteArray> roles;
    roles[AppIDRole] = "appID";
    roles[NameRole] = "name";
    roles[ImageRole] = "imagePath";
    roles[SelectRole] = "isSelectStatus";
    roles[PlayRole] = "isPlayStatus";
    setRoleNames(roles);

    clear();
}

//qSort(list.begin(), list.end(), PtrLess<MyClass>());

void CAppAha_ModelBase::addData(qulonglong appID, bool isSelect, const QString &name)
{
    if(m_hashData.contains(appID)) return;

    int rowIndex = rowCount();
    m_hashData[appID] = rowIndex;

    if(isSelect) i_current_row = rowIndex;

    const ModelData value(appID, isSelect, name);
    beginInsertRows(QModelIndex(), rowIndex, rowIndex);
    m_listData << value;
    endInsertRows();
}

void CAppAha_ModelBase::updateImagePath(qulonglong appID, const QString &value)
{
    int rowIndex = m_hashData[appID];

     if(hasIndex(rowIndex, 0))
    {
        this->setData(index(rowIndex, 0), value, ImageRole);
    }
}

void CAppAha_ModelBase::updateCurrentPlay(qulonglong appID, const int play_status)
{
    int rowIndex = m_hashData[appID];

    if(hasIndex(rowIndex, 0))
    {
        switch(play_status)
        {
        case AHA_PLAYBACK_STOP:
        case AHA_PLAYBACK_PAUSE:
            this->setData(index(rowIndex, 0), false, PlayRole);
            break;

        case AHA_PLAYBACK_PLAY:
        case AHA_PLAYBACK_BUFFERING:
            this->setData(index(rowIndex, 0), true, PlayRole);
            break;
        }
    }
}

void CAppAha_ModelBase::updateCurrentPlay(const int play_status)
{
    if(hasIndex(i_current_row, 0))
    {
        switch(play_status)
        {
        case AHA_PLAYBACK_STOP:
        case AHA_PLAYBACK_PAUSE:
            this->setData(index(i_current_row, 0), false, PlayRole);
            break;

        case AHA_PLAYBACK_PLAY:
        case AHA_PLAYBACK_BUFFERING:
            this->setData(index(i_current_row, 0), true, PlayRole);
            break;
        }
    }
}

void CAppAha_ModelBase::updateCurrentSelect(qulonglong appID)
{
    int rowIndex = m_hashData[appID];

     if(hasIndex(rowIndex, 0))
     {
        if(hasIndex(i_current_row, 0))
        {
            this->setData(index(i_current_row, 0), false, PlayRole);
            this->setData(index(i_current_row, 0), false, SelectRole);
        }

        this->setData(index(rowIndex, 0), true, SelectRole);
        this->setData(index(rowIndex, 0), true, PlayRole); //hsryu_0528_track_content_list

        i_current_row = rowIndex;
    }
}

qulonglong CAppAha_ModelBase::getID(int new_row)
{
    if(hasIndex(i_current_row, 0))
    {
        this->setData(index(i_current_row, 0), false, PlayRole);
        this->setData(index(i_current_row, 0), false, SelectRole);
    }

    if(hasIndex(new_row, 0))
    {
        i_current_row = new_row;
        QModelIndex mi_index = index(new_row, 0);

        this->setData(mi_index, true, SelectRole);

        QVariant varFind = this->data(mi_index, AppIDRole);
        return varFind.toULongLong();
    }

    return 0;
}

int CAppAha_ModelBase::getIndexID(qulonglong appID)
{
    if(m_hashData.contains(appID))
    {
        return m_hashData[appID];
    }

    return -1;
}

//wsuk.kim TUNE
QString CAppAha_ModelBase::getStationNameIndex(int tuneIndex)
{
    QVariant strStationName = this->data(index(tuneIndex, 0), NameRole);

    return strStationName.toString();
}

QString CAppAha_ModelBase::getStationArtIndex(int tuneIndex)    //wsuk.kim 130902 ISV_90329 to display station art image that tune search.
{
    QVariant strStationArt = this->data(index(tuneIndex, 0), ImageRole);

    return strStationArt.toString();
}
//wsuk.kim TUNE

bool CAppAha_ModelBase::existID(qulonglong appID)
{
    if(m_hashData.contains(appID)) return true;

    return false;
}

bool CAppAha_ModelBase::isSelect()
{
    if(i_current_row == -1) return false;
    return true;
}

int CAppAha_ModelBase::getSelectRow()
{
    if(i_current_row == -1)
    {
        findSelectedRow();
    }
    return i_current_row;
}

void CAppAha_ModelBase::findSelectedRow()
{
    for(int k = 0; k < rowCount(); k++)
    {
        QVariant isSelect = this->data(index(k, 0), SelectRole);
        if(isSelect.toBool())
        {
            i_current_row = k;
            break;
        }
    }
}

void CAppAha_ModelBase::reset()
{
    i_current_row = -1;

    for(int k = 0; k < rowCount(); k++)
    {
        this->setData(index(k, 0), false, PlayRole);
        this->setData(index(k, 0), false, SelectRole);
    }
}

int CAppAha_ModelBase::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return m_listData.count();
}

void CAppAha_ModelBase::clear()
{
    i_current_row = -1;

    m_hashData.clear();

    beginRemoveRows(QModelIndex(), 0,  rowCount() - 1);
    m_listData.clear();
    endRemoveRows();
}

bool CAppAha_ModelBase::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() > m_listData.count())
    {
        return false;
    }

    ModelData md = m_listData[index.row()];

    switch(role)
    {
    case AppIDRole: md.setAppID(value.toULongLong()); break;
    case NameRole: md.setName(value.toString()); break;
    case ImageRole: md.setImagePath(value.toString()); break;
    case SelectRole:    md.setIsSelect(value.toBool()); break;
    case PlayRole:    md.setIsPlay(value.toBool()); break;
    default: return false;
    }

    m_listData.replace(index.row(), md);
    emit(dataChanged(index, index));
    return true;
}

QVariant CAppAha_ModelBase::data(const QModelIndex & index, int role) const
{
    if (index.row() < 0 || index.row() > m_listData.count())
    {
        return QVariant();
    }

    const ModelData &md = m_listData[index.row()];

    switch(role)
    {
    case AppIDRole: return md.getAppID();
    case NameRole: return md.getName();
    case ImageRole: return md.getImagePath();
    case SelectRole: return md.getIsSelect();
    case PlayRole:  return md.getIsPlay();
    }

    return QVariant();
}
