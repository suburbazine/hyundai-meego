#ifndef DABEPGMANAGER_H
#define DABEPGMANAGER_H
/**
 * FileName: DABEPGManager.h
 * Author: David.Bae
 * Time: 2011-11-17 13:41
 * Description:
 *          This file is used for EPG Data.
 */
#include <QObject>
#include <QHash>
#include <QList>
#include "DABEPGItem.h"
#include "DABEPGDateListItem.h"
#include "DABLogger.h"

class DABEPGManager : public QObject
{
    Q_OBJECT
public:

    DABEPGManager(QObject *parent=0);
    ~DABEPGManager();
    bool addEPGItem(quint32, QString ensembleName, QString serviceName, DABEPGItem* pItem);
    QList<DABEPGItem*>* getEPGList(int index, QString ensembleLabel, QString serviceLabel, bool isMake);
    QList<DABEPGItem*>* getEPGList(QString ensembleLabel, QString serviceLabel);
    QList<DABEPGItem*>* getCurrentEPGData(int index, QString ensembleLabel, QString serviceLabel);
    QList<DABEPGItem*>* getCurrentEPGData(QString ensembleLabel, QString serviceLabel);
    int getEPGHashIndex(quint32 mjd, bool isMake);
    QList<DABEPGItem*>* sortEPG(quint32 mjd, QString ensembleName, QString serviceName); 
    QList<DABEPGItem*>* getEPGServiceList(quint32 mjd, QString ensembleName, QString serviceName);
    void clear(void);
    void clear(quint32 mjd, QString ensembleName, QString serviceName);    
    void sortEPGInOrderFrom0To24(QList<DABEPGItem*>* pEPGListForService);
    void sortEPGInOrderToCurrentTime(QList<DABEPGItem*>* pEPGListForService);
    bool getCurrentDateEPGExists(QString ensembleLabel, QString serviceLabel);
    void setDateListIndex(int index) { this->m_iDateListIndex = index; };
    int  getDateListIndex(void) { return m_iDateListIndex; };
    void fullSave(void);
    void saveEPGData(QList<DABEPGItem*>* pEPGListForService, quint32 mjd, QString fLabel, quint16 eID, quint32 sID);
    void load(void);
    void remove(void);
    void removeEPGFile(QString path);
    void removeEPGData(quint32 mjd);
    void init(void);
    void getEPGFullPath(QString path);
    void constructEPGData(QString path);
    QDate convertMJDToDate(quint32 mjd);
    quint32 convertDateToMJD(QDate date);
    QHash<QString, QVariant*>* getEnsembleHash(quint32 mjd, QString ensembleLabel);
#if 1
    bool checkExistEPGHashTable(quint32 mjd, QHash<QString, QVariant*>* pEnsembleHash, QString serviceName);
#else
    bool checkExistEPGHashTable(QHash<QString, QVariant*>* pEnsembleHash, QString serviceName);
#endif
    //for debugging
    void showEPG(void);

signals:
    void cmdAddEPGItem(quint32 mjd, quint32 serviceID, int Hour, int Minute, int Second, int Duration, QString ProgramLabel, QString Description);
    void cmdSortingEPGData(quint32 mjd);

private:
    //EPG Hash Table
    QHash<QString, QVariant*> m_EPGHash;
//    QHash<QString, QVariant*>* getEnsembleHash(quint32 mjd, QString ensembleLabel);
    QList<DABEPGDateListItem*> *pEPGDateItemList;
    //DABEPGDateListItem *m_xDABEPGDateItem;  //# 20130327 prevent
    int m_iDateListIndex;
};
#endif // DABEPGITEM_H
