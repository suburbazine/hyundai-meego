#ifndef DHAVN_APPMEDIAPLAYER_RADIOLISTMODEL_H
#define DHAVN_APPMEDIAPLAYER_RADIOLISTMODEL_H

#include <QAbstractListModel>

class RadioListModelItem;

class RadioListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int selectedIndex READ selectedIndex NOTIFY selectedIndexChanged)
    Q_PROPERTY(int count READ count);  // added by kihyung 2012.07.24 for CR 12054


public:
    enum RadioModelRoles
    {
        NameRole = Qt::UserRole + 1,
        CommentRole,
        EnableRole
    };

    explicit RadioListModel(QObject *parent = 0);
    ~RadioListModel();

    // Redefined QAbstractListModel methods.
    QVariant data(const QModelIndex & index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const; // modified by kihyung 2012.07.24 for CR 12054
    virtual RadioListModelItem* getListItem(int index) const;

    Q_INVOKABLE int count() { return rowCount(); }  // modified by Sergey 20.08.2013 for ITS#184640 

    void addItem(QString name);
    void clear();
    void updateModel(QStringList list, int selectedIndex);

    int selectedIndex() { return m_selectedIndex; }
    void setSelectedIndex(int value)
    {
        m_selectedIndex = value;
        emit selectedIndexChanged(m_selectedIndex);
    }

signals:
    void selectedIndexChanged(int index);


protected:
    QHash<int, QByteArray> roles;

private:
    QList<RadioListModelItem*> m_itemList;
    int m_selectedIndex;

};

#endif // DHAVN_APPMEDIAPLAYER_RADIOLISTMODEL_H
