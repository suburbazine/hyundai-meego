#ifndef DABEPGDATELISTITEM_H
#define DABEPGDATELISTITEM_H

#include <QObject>
#include <QHash>

class DABEPGDateListItem : public QObject
{
    Q_OBJECT
public:
    explicit DABEPGDateListItem(QObject *parent = 0);

    quint32 getEPGDate(void) { return m_sEPGDate; };
    QString getEPGEnsembleName(void) { return m_sEnsembleName; };
    QString getEPGServiceName(void) { return m_sServiceName; };
    QHash<QString, QVariant*>* getEPGHashTable(void) { return &m_pEPGHashTable; };
//    QHash<QString, QVariant*>* getEnsembleHashTable(void) { return m_pEnsembleHash; };

    void setEPGDate(quint32 date) { this->m_sEPGDate = date; };
    void setEPGEnsembleName(QString ensemble) { this->m_sEnsembleName = ensemble; };
    void setEPGServiceName(QString service) { this->m_sServiceName = service; };
//    void setEPGHashTable(QHash<QString, QVariant*> hashTable) { this->m_pEPGHashTable = hashTable; };
//    void setEnsembleHashTable(QHash<QString, QVariant*> *emsembleHash) { this->m_pEnsembleHash = emsembleHash; };

private:
    quint32 m_sEPGDate;
    QString m_sEnsembleName;
    QString m_sServiceName;
    QHash<QString, QVariant*> m_pEPGHashTable;
//    QHash<QString, QVariant*>* m_pEnsembleHash;

signals:

public slots:



};

#endif // DABEPGDATELISTITEM_H
