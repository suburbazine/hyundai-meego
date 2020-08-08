#ifndef DHAVN_APPSETTINGS_CNAVIVERSIONINFORMER_H
#define DHAVN_APPSETTINGS_CNAVIVERSIONINFORMER_H

#include <QObject>
#include <QString>
#include <QSharedMemory>

class CNaviVersionInformer : public QObject
{
    Q_OBJECT
public:
    explicit CNaviVersionInformer(QObject *parent = 0);

    QString getMapVersion();
    QString getAssociationNumber();
    QString getAppVersion();

signals:

public slots:

private:
    QString mapVersion;
    QString associationNumber;
    QString appVersion;

    QSharedMemory *m_pNaviVerInfoSharedMem;

private:
    void loadNaviInfo();
};

#endif // DHAVN_APPSETTINGS_CNAVIVERSIONINFORMER_H
