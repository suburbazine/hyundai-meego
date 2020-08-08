#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_LISTMODEL_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_LISTMODEL_H

#include <QAbstractListModel>
#include "DHAVN_AppMediaPlayer_Video_ListModelItem.h" //added by Michael.Kim 2013.07.23 for New UX

//class VListModelItem; //deleted by Michael.Kim 2013.07.23 for New UX

class VListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int selectedIndex READ selectedIndex NOTIFY selectedIndexChanged);
    Q_PROPERTY(int count READ count); // added by Sergey 20.07.2013

public:
    enum RadioModelRoles
    {
        Text1Role = Qt::UserRole + 1,
        Text2Role
    };

    explicit VListModel(QObject *parent = 0);
    ~VListModel();

    // Redefined QAbstractListModel methods.
    QVariant data(const QModelIndex & index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual int rowCount (const QModelIndex &parent = QModelIndex()) const;
    virtual VListModelItem* getListItem(int index) const;

    QVariant data(const int index, int role);
    void addItem(QString text1 = "", QString tex2 = "");
    void clear();
    void updateModel(QStringList *list);

    int selectedIndex() { return m_selectedIndex; }
    void setSelectedIndex(int value)
    {
        m_selectedIndex = value;
        emit selectedIndexChanged(m_selectedIndex);
    }

    int count() { return rowCount(); } // added by Sergey 20.07.2013

signals:
    void selectedIndexChanged(int index);


protected:
    QHash<int, QByteArray> roles;


private:
    QVariant getItemData(VListModelItem *item, int role);

    QList<VListModelItem*> m_itemList;
    int m_selectedIndex;

};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_LISTMODEL_H
