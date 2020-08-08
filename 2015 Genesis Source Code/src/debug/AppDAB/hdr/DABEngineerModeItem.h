#ifndef DABENGINEERMODEITEM_H
#define DABENGINEERMODEITEM_H

#include "ListModel.h"

class DABEngineerModeItem : public ListItem
{
    Q_OBJECT

public:
    enum Roles {
        DataName = Qt::UserRole+1,
        DataValue
    };
    explicit DABEngineerModeItem(QObject *parent = 0);
    explicit DABEngineerModeItem(QString dataName, QString dataValue, QObject *parent);
    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;
    inline QString id() const { return m_dataName; }
    inline quint32 key() const { return m_dataFreq; }
    inline quint8 subKey() const { return m_dataSubChId; }
    inline QString dataName() const { return m_dataName;}
    inline QString dataValue() const { return m_dataValue;}
    inline quint8 ausFlag() const { return m_dataSubChId; }
    inline QString ensemble() const { return m_dataName; }

private:
    QString m_dataName;
    QString m_dataValue;
    quint32 m_dataFreq;
    quint8 m_dataSubChId;

signals:

public slots:

};

#endif // DABENGINEERMODEITEM_H
