#ifndef DHAVN_APPAHAMENUS_H
#define DHAVN_APPAHAMENUS_H
#include <QtCore>
#include <DHAVN_OptionMenuModel.h>

class AhaMenus : public QObject
{
    Q_OBJECT
    Q_PROPERTY ( OptionMenuModel* optTrackMenuModel READ GetOptTrackMenuModel WRITE SetOptTrackMenuModel NOTIFY optMenuModelChanged)
    Q_PROPERTY ( OptionMenuModel* optListMenuModel READ GetOptListMenuModel WRITE SetOptListMenuModel NOTIFY optMenuModelChanged)

public:
    explicit AhaMenus(QObject *pParent = NULL);
    ~AhaMenus();
    OptionMenuModel* GetOptTrackMenuModel() { return m_pOptTrackMenuModel; }
    OptionMenuModel* GetOptListMenuModel() { return m_pOptListMenuModel; }
    void SetOptTrackMenuModel( OptionMenuModel* optMenuModel ) { m_pOptTrackMenuModel = optMenuModel; }
    void SetOptListMenuModel( OptionMenuModel* optMenuModel ) { m_pOptListMenuModel = optMenuModel; }
    void InitEnabled(int menuCount); //wsuk.kim dynamic_menu

    Q_INVOKABLE void UpdateLikeDislikeIconInOptionsMenu(QString strCurrLikeIcon, QString strCurrDislikeIcon); //wsuk.kim menu_option
    Q_INVOKABLE void UpdateAllowInOptionsMenu(bool bAllowLike, bool bAllowDislike, bool bAllowCall, bool bAllowNavigate, bool bNoNetwork=false); //wsuk.kim dynamic_menu // ITS 225504

signals:
    void optMenuModelChanged();
    void menuDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

public:
    OptionMenuModel* m_pOptTrackMenuModel;
    OptionMenuModel* m_pOptListMenuModel;

};
#endif // DHAVN_APPAHAMENUS_H
