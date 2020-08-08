#ifndef DHAVN_APPPANDORAMENUS_H
#define DHAVN_APPPANDORAMENUS_H
#include <QtCore>
#include <DHAVN_OptionMenuModel.h>

class PandoraMenus : public QObject
{
    Q_OBJECT
    Q_PROPERTY ( OptionMenuModel* optTrackMenuModel READ GetOptTrackMenuModel WRITE SetOptTrackMenuModel NOTIFY optMenuModelChanged)
    Q_PROPERTY ( OptionMenuModel* optListMenuModel READ GetOptListMenuModel WRITE SetOptListMenuModel NOTIFY optMenuModelChanged)
    Q_PROPERTY ( OptionMenuModel* optNoStationMenuModel READ GetOptNoStationMenuModel WRITE SetOptNoStationMenuModel NOTIFY optMenuModelChanged)
public:
    PandoraMenus(QObject *parent = 0);
    ~PandoraMenus();
    OptionMenuModel* GetOptTrackMenuModel() { return m_pOptTrackMenuModel; }
    OptionMenuModel* GetOptListMenuModel() { return m_pOptListMenuModel; }
    OptionMenuModel* GetOptNoStationMenuModel() { return m_pOptNoStationMenuModel; }
    void SetOptTrackMenuModel( OptionMenuModel* optMenuModel ) { m_pOptTrackMenuModel = optMenuModel; }
    void SetOptListMenuModel( OptionMenuModel* optMenuModel ) { m_pOptListMenuModel = optMenuModel; }
    void SetOptNoStationMenuModel( OptionMenuModel* optMenuModel ) { m_pOptNoStationMenuModel = optMenuModel; }

    Q_INVOKABLE void UpdateRatingInOptionsMenu(int item, bool status);
    Q_INVOKABLE void UpdateRatingDisableInOptionsMenu();

    Q_INVOKABLE void SetSortListMenuModel( int item);

signals:
    void optMenuModelChanged();
    void menuDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
public:
    OptionMenuModel* m_pOptTrackMenuModel;
    OptionMenuModel* m_pOptListMenuModel;
    OptionMenuModel* m_pOptNoStationMenuModel;
};
#endif // DHAVN_APPPANDORAMENUS_H
