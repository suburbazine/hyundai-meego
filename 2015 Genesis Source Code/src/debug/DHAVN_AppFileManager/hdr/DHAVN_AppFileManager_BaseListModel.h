#ifndef DHAVN_APPFILEMANAGER_BASELISTMODEL_H
#define DHAVN_APPFILEMANAGER_BASELISTMODEL_H

#include <QAbstractListModel>
#include <QHash>

#include "DHAVN_AppFileManager_BaseModelItem.h"
#include "DHAVN_AppFileManager_Logger.h"

class BaseListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY( bool itemsAreChecked READ getItemsAreChecked NOTIFY checkedItemsUpdated )
    Q_PROPERTY( bool fileSelected READ getFileSelected NOTIFY fileSelectedUpdated )
    
public:
    // USE_FM_LOG_UTIL
    enum BaseModelRoles {
        BaseNameRole = Qt::UserRole + 1,
        BaseIdRole,
        BaseCheckedRole,
        BasePathRole,
        BasePlayed,
        BaseSupported, // added by Dmitry 23.08.13
        BaseUserRole
    };

    explicit BaseListModel(QObject *parent = 0);
    QVariant data(const QModelIndex & index, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

    Q_INVOKABLE void checkElement( int index, bool isChecked);
    Q_INVOKABLE int getPlayedItemIndex();
    // { added by yungi 2013.03.08 for New UX FileCount
	// modified by ravikanth 06-07-13 for Sanity fix selected items logic was not proper.
    Q_INVOKABLE int getFileURLCount() const { return this->CheckBoxSelectable_count; };
    Q_INVOKABLE void setFileURLCount(int count) { this->CheckBoxSelectable_count = count; }; //modified by aettie 2013.03.28 for Static defect 20198
    // } added by yungi 2013.03.08 for New UX FileCount

    bool getItemsAreChecked() const;
    virtual const BaseModelItem* getListItem(int index) const = 0;
    virtual BaseModelItem* getListItem(int index) = 0;
    virtual int getListCount() const = 0 ;//Changed by Alexey Edelev 2012.10.18 CR14766
    virtual void removeDataItem(int);
    void clearCheckedElements();
    bool getFileSelected() const;
    bool isElementChecked(int index); // modified by ravikanth 26-06-13 for ITS 0175972
    void setCheckedRef(bool copyinit); // modified by ravikanth for ITS 0184642

signals:
    void checkedItemsUpdated( bool flag );
    void fileSelectedUpdated( bool flag );

protected:
    QHash<int, QByteArray> roles;

protected:
    int checkedRef;
    bool isFileSelected;
    int CheckBoxSelectable_count; //added by yungi 2013.03.08 for New UX FileCount

private:
    void notifyFileSelected(const BaseModelItem *item);
};

#endif // DHAVN_APPFILEMANAGER_BASELISTMODEL_H
