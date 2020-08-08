#ifndef DHAVN_APPAHA_MODELBASE_H
#define DHAVN_APPAHA_MODELBASE_H

#include <QtGlobal>
#include <QObject>
#include <QHash>
#include <QAbstractListModel>
#include "DHAVN_AppAha_ModelData.h"
#include "ahapredefinedvalues.h"

class CAppAha_ModelBase : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DataRoles
    {
        AppIDRole = Qt::UserRole + 1,
        NameRole,
        ImageRole,
        SelectRole,
        PlayRole
    };

    explicit CAppAha_ModelBase(QObject *parent = 0);

    int   getIndexID(qulonglong appID);
    int   getSelectRow();
    qulonglong getID(int new_row);

    bool existID(qulonglong appID);
    bool isSelect();
    void updateCurrentSelect(qulonglong appID);
    void updateCurrentPlay(const int play_status);
    void updateCurrentPlay(qulonglong appID, const int play_status);
    void updateImagePath(qulonglong appID, const QString &value);

    void clear();
    void reset();
    void addData(qulonglong appID, bool isSelect, const QString &name);

    int rowCount(const QModelIndex &index = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QString getStationNameIndex(int tuneIndex); //wsuk.kim TUNE
    QString getStationArtIndex(int tuneIndex);    //wsuk.kim 130902 ISV_90329 to display station art image that tune search.
    int getIndexCurrentPlayID(qulonglong appID);  //wsuk.kim TUNE
private:
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    void findSelectedRow();

private:
    QList<ModelData> m_listData;
    QHash<qulonglong, int>m_hashData;

    int i_current_row;
};

#endif // DHAVN_APPAHA_MODELBASE_H
