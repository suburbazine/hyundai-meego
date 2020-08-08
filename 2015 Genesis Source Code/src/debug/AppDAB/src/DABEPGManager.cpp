/**
 * FileName: DABEPGManager.cpp
 * Author: David.Bae
 * Time: 2011-11-17 13:43
 * Description:
 *          This file is used for EPG Data.
 */

#include "DABEPGManager.h"
#include "DABProtocol.h"
#include "DABApplication_Def.h"
#include <QDebug>
#include <QFile>
#include <QDir>


DABEPGManager::DABEPGManager( QObject *parent):
    QObject(parent)
{
    pEPGDateItemList = NULL;
    pEPGDateItemList = new QList<DABEPGDateListItem*>;
    m_iDateListIndex = 0;
}

DABEPGManager::~DABEPGManager()
{
    //
//    clear();
}

void
DABEPGManager::init(void)
{
    this->remove();
    this->load();
    quint32 mjd = DABLogger::instance()->convertDateToMJD(QDate::currentDate());
    emit cmdSortingEPGData(mjd);
}

void
DABEPGManager::clear(void)
{      
    this->m_EPGHash.clear();
}

void
DABEPGManager::clear(quint32 mjd, QString ensembleName, QString serviceName)
{
    QHash<QString, QVariant*>* pEnsembleHash = this->getEnsembleHash(mjd, ensembleName);
    if(pEnsembleHash == NULL)
    {
        qDebug() << __FUNCTION__ << ": EnsembleHash Table is not exist. return !!";
        return;
    }

    // Find Service Hash
    QHash<QString, QVariant*>::const_iterator eIterator = pEnsembleHash->find(serviceName);
    if( eIterator == pEnsembleHash->end())
    {
        qDebug() << " Delete Fail (" << ensembleName <<","<<serviceName<<")";
    }
    else if( eIterator.key() == serviceName)
    {
        pEnsembleHash->remove(serviceName);
        qDebug() << " Delete OK (" << ensembleName <<","<<serviceName<<")";
    }
}

#if 1
bool
DABEPGManager::checkExistEPGHashTable(quint32 mjd, QHash<QString, QVariant*>* pEnsembleHash, QString serviceName)
{
    QList<DABEPGItem*>* pEPGListForService = NULL;
    DABEPGItem* pEpgItem = NULL;
    QList<DABEPGItem*>::iterator i;
    bool bExist = false;

    QTime cTime = QTime::currentTime();
    QDate cDate = QDate::currentDate();
    quint32 cMjd = DABLogger::instance()->convertDateToMJD(cDate);

    if(pEnsembleHash == NULL)
    {
        qDebug() << __FUNCTION__ << ": EnsembleHash Table is not exist!!";
        return bExist;
    }

    QHash<QString, QVariant*>::const_iterator eIterator = pEnsembleHash->find(serviceName);
    if( eIterator == pEnsembleHash->end())
    {
        qDebug() << __FUNCTION__ << " Find Fail. No service List.";
    }
    else if( eIterator.key() == serviceName)
    {
        qDebug() << __FUNCTION__ << " Find OK service List: key:" << eIterator.key();
        pEPGListForService = (QList<DABEPGItem*>*)eIterator.value();

        for(i = pEPGListForService->begin(); i != pEPGListForService->end(); i++)
        {
            pEpgItem = i[0];
            QTime sTime(pEpgItem->hour(), pEpgItem->minute(), pEpgItem->second());
            QTime eTime = sTime.addSecs(pEpgItem->duration());

//            qDebug() << "ServiceName:" << serviceName << " StartTime:" << sTime.toString() << ", EndTime:" << eTime.toString();

            QTime tTime(0,0,0);
            QTime endTime = pEpgItem->getEndTime();
            if(tTime == endTime)
            {
                endTime.setHMS(23,59,59);
            }

//            qDebug() << "currentTime:" << cTime.toString() << " StartTime:" << pEpgItem->getStartTime().toString() << ", EndTime:" << eTime.toString();

            if((!(endTime >= cTime || pEpgItem->getStartTime() >= cTime)) && (mjd == cMjd))
            {
                continue;
            }
            bExist = true;
        }
    }

    return bExist;
}

#else

bool
DABEPGManager::checkExistEPGHashTable(QHash<QString, QVariant*>* pEnsembleHash, QString serviceName)
{
    if(pEnsembleHash == NULL)
    {
        qDebug() << __FUNCTION__ << ": EnsembleHash Table is not exist!!";
        return false;
    }

    QHash<QString, QVariant*>::const_iterator eIterator = pEnsembleHash->find(serviceName);
    if( eIterator == pEnsembleHash->end())
    {
        qDebug() << __FUNCTION__ << " Find Fail. No service List.";
        return false;
    }
    else if( eIterator.key() == serviceName)
    {
        qDebug() << __FUNCTION__ << " Find OK service List: key:" << eIterator.key();
        return true;
    }
    else
    {
        return false;
    }
}
#endif

int
DABEPGManager::getEPGHashIndex(quint32 mjd, bool isMake)
{
    DABEPGDateListItem *pDateListItem = NULL;

    int count = pEPGDateItemList->count();
    for(int i=0; i < count; i++)
    {
        pDateListItem = pEPGDateItemList->at(i);
//        qDebug() << __FUNCTION__ << " pEPGDateItemList[" << index <<"]" << " m_sEPGDate = " << pDateListItem->getEPGDate() << ", mjd = " << mjd;
        if(mjd == pDateListItem->getEPGDate())
        {            
            return i;
        }
    }

    if(isMake || count == 0)
    {
        qDebug() << __FUNCTION__ <<" No EPG Hash. Make it! ==================================";
        DABLogger::instance()->Log((QString("getEPGHashIndex() :: No EPG Hash. Make it! ==================================")));
        pDateListItem = new DABEPGDateListItem(this);
        pDateListItem->setEPGDate(mjd);
        pEPGDateItemList->append(pDateListItem);
        return pEPGDateItemList->indexOf(pDateListItem);
    }

    qDebug() << __FUNCTION__ << "EPG Hash not Found !!. return -1";
    return -1;
}

QList<DABEPGItem*>*
DABEPGManager::getCurrentEPGData(int index, QString ensembleLabel, QString serviceLabel)
{
    qDebug() << Q_FUNC_INFO;

    QList<DABEPGItem*>* pEPGListForService = NULL;
    DABEPGDateListItem *pDateListItem = NULL;
    QHash<QString, QVariant*>* pEnsembleHash = NULL;

    pDateListItem = pEPGDateItemList->at(index);
    if(pDateListItem == NULL)
    {
        qDebug() << __FUNCTION__ << ": ERROR pDateListItem allocation Error!!";
        return NULL;
    }

    // Find Ensemble Hash ==============================================
//    qDebug() << __FUNCTION__ << " EPGDateItemList index = " << index << ", "<< ensembleLabel << " - " << pDateListItem->getEPGHashTable() << " :: " << pDateListItem->getEPGHashTable()->size();

    QHash<QString, QVariant*>::iterator i = pDateListItem->getEPGHashTable()->find(ensembleLabel);
    if( i == pDateListItem->getEPGHashTable()->end() )
    {
        qDebug() << __FUNCTION__ <<" No ensemble Hash. return!!";
        return NULL;
    }
    else if( i.key() == ensembleLabel)
    {
//        qDebug() << " Find OK ensemble Hash: key:" << i.key();
        pEnsembleHash = (QHash<QString, QVariant*>*)i.value();
    }

    if(pEnsembleHash == NULL)
    {
        qDebug() << " pEnsembleHash is NULL";
        return NULL;
    }

    // Find Service Hash==============================================
    QHash<QString, QVariant*>::const_iterator eIterator = pEnsembleHash->find(serviceLabel);
    if( eIterator == pEnsembleHash->end())
    {
        qDebug() << __FUNCTION__<< " No service List. return !!";
        return NULL;
    }
    else if( eIterator.key() == serviceLabel)
    {
//        qDebug() << " Find OK service List: key:" << eIterator.key();
        pEPGListForService = (QList<DABEPGItem*>*)eIterator.value();
    }

    return pEPGListForService;
}

QList<DABEPGItem*>*
DABEPGManager::getEPGList(int index, QString ensembleLabel, QString serviceLabel, bool isMake)
{
//    qDebug() << Q_FUNC_INFO << " index = " << index << ", ensemble = " << ensembleLabel << ", service = " << serviceLabel;

    QList<DABEPGItem*>* pEPGListForService = NULL;
    DABEPGDateListItem *pDateListItem = NULL;
    QHash<QString, QVariant*>* pEnsembleHash = NULL;

    pDateListItem = pEPGDateItemList->at(index);
    if(pDateListItem == NULL)
    {
        qDebug() << __FUNCTION__ << ": ERROR pDateListItem allocation Error!!";
        return NULL;
    }

    // Find Ensemble Hash ==============================================
//    qDebug() << __FUNCTION__ << " EPGDateItemList index = " << index << ", "<< ensembleLabel << " - " << pDateListItem->getEPGHashTable() << " :: " << pDateListItem->getEPGHashTable()->size();

    QHash<QString, QVariant*>::iterator i = pDateListItem->getEPGHashTable()->find(ensembleLabel);
    if( i == pDateListItem->getEPGHashTable()->end() )
    {
        if(isMake)
        {
            qDebug() << __FUNCTION__ <<" No ensemble Hash. Make it!";
            pEnsembleHash = new QHash<QString, QVariant*>;

            if(pEnsembleHash == NULL)
            {
                qDebug() << __FUNCTION__ << ": ERROR allocation Error!!";
                return NULL;
            }
            pDateListItem->getEPGHashTable()->insert(ensembleLabel, (QVariant*)pEnsembleHash);
        }
        else
        {
            qDebug() << __FUNCTION__ <<" No ensemble Hash. return!!";
            return NULL;
        }
//        qDebug() << __FUNCTION__ << " : " << ensembleLabel << " - " << pDateListItem->getEPGHashTable() << " :: " << pDateListItem->getEPGHashTable()->size();
    }
    else if( i.key() == ensembleLabel)
    {
//        qDebug() << " Find OK ensemble Hash: key:" << i.key();
        pEnsembleHash = (QHash<QString, QVariant*>*)i.value();
    }

    if(pEnsembleHash == NULL)
    {
        qDebug() << " pEnsembleHash is NULL";
        return NULL;
    }

    // Find Service Hash==============================================
    QHash<QString, QVariant*>::const_iterator eIterator = pEnsembleHash->find(serviceLabel);
    if( eIterator == pEnsembleHash->end())
    {
        if(isMake)
        {
            qDebug() << __FUNCTION__ << " No service List. Make it!";
            pEPGListForService = new  QList<DABEPGItem*>;
            if(pEPGListForService == NULL)
            {
                qDebug() << __FUNCTION__ << ": ERROR allocation Error!!";
                return NULL;
            }
            pEnsembleHash->insert(serviceLabel, (QVariant*)pEPGListForService);
        }
        else
        {
            qDebug() << __FUNCTION__<< " No service List. return !!";
            return NULL;
        }
    }
    else if( eIterator.key() == serviceLabel)
    {
//        qDebug() << " Find OK service List: key:" << eIterator.key();
        pEPGListForService = (QList<DABEPGItem*>*)eIterator.value();
    }

    return pEPGListForService;
}

QList<DABEPGItem*>*
DABEPGManager::getEPGList(QString ensembleLabel, QString serviceLabel)
{
    if(ensembleLabel == "" || serviceLabel == "")
    {
        qDebug() << __FUNCTION__ << ": ERROR ensemble:" << ensembleLabel <<", service:" << serviceLabel;
        return NULL;
    }
    QHash<QString, QVariant*>* pEnsembleHash = NULL;
    QList<DABEPGItem*>* pEPGListForService = NULL;

    // Find Ensemble Hash
//    qDebug() << " Find ensemble Hash: key:" << ensembleLabel;
    QHash<QString, QVariant*>::iterator i = this->m_EPGHash.find(ensembleLabel);
    if( i == m_EPGHash.end() )
    {
//        qDebug() << " No ensemble Hash. Make it!";
        pEnsembleHash = new QHash<QString, QVariant*>;
        if(pEnsembleHash == NULL)
        {
            qDebug() << __FUNCTION__ << ": ERROR allocation Error!!";
            return NULL;
        }
        this->m_EPGHash.insert(ensembleLabel, (QVariant*)pEnsembleHash);
    }else if( i.key() == ensembleLabel)
    {
//        qDebug() << " Find OK ensemble Hash: key:" << i.key();
        pEnsembleHash = (QHash<QString, QVariant*>*)i.value();
    }

    if(pEnsembleHash == NULL)
    {
        qDebug() << " pEnsembleHash is NULL";
        return NULL;
    }

    // Find Service Hash
//    qDebug() << " Find service List: key:" << serviceLabel;
    QHash<QString, QVariant*>::const_iterator eIterator = pEnsembleHash->find(serviceLabel);
    if( eIterator == pEnsembleHash->end())
    {
//        qDebug() << " No service List. Make it!";
        pEPGListForService = new  QList<DABEPGItem*>;
        if(pEPGListForService == NULL)
        {
            qDebug() << __FUNCTION__ << ": ERROR allocation Error!!";
            return NULL;
        }
        pEnsembleHash->insert(serviceLabel, (QVariant*)pEPGListForService);
    }else if( eIterator.key() == serviceLabel){
//        qDebug() << " Find OK service List: key:" << eIterator.key();
        pEPGListForService = (QList<DABEPGItem*>*)eIterator.value();
    }

    return pEPGListForService;
}

QHash<QString, QVariant*>*
DABEPGManager::getEnsembleHash(quint32 mjd, QString ensembleLabel)
{
    if(ensembleLabel == "")
    {
        qDebug() << __FUNCTION__ << ": ERROR ensemble:" << ensembleLabel;
        return NULL;
    }

    QHash<QString, QVariant*>* pEnsembleHash = NULL;
    QHash<QString, QVariant*>* pEPGHansh = NULL;

    int index = getEPGHashIndex(mjd, false);
    if(index == -1)
    {
        qDebug() << " No ensemble Hash. return!! hash index = " << index;
        return NULL;
    }

    DABEPGDateListItem* pDateListItem = pEPGDateItemList->at(index);
    pEPGHansh = pDateListItem->getEPGHashTable();

    // Find Ensemble Hash
    QHash<QString, QVariant*>::iterator i = pEPGHansh->find(ensembleLabel);

    if( i == pEPGHansh->end() )
    {
        qDebug() << " No ensemble Hash. return! ensemble label = " << ensembleLabel;
        return NULL;
    }
    else if( i.key() == ensembleLabel)
    {
        qDebug() << " Find OK ensemble Hash: key:" << i.key();
        pEnsembleHash = (QHash<QString, QVariant*>*)i.value();
    }

    return pEnsembleHash;
}

bool
DABEPGManager::addEPGItem(quint32 mjd, QString ensembleName, QString serviceName, DABEPGItem* pItem)
{
    //qDebug() << __FUNCTION__ << " mjd = " << mjd << ", ensembleName = " << ensembleName << ", serviceName = " << serviceName;

    int index = getEPGHashIndex(mjd, true);

    if(index == -1)
    {
        qDebug() << __FUNCTION__ << "ERROR : can't find EPGList for " << ensembleName <<","<<serviceName;
        DABLogger::instance()->Log((QString("addEPGItem() :: ERROR : can't find EPGList for = ====")));
        return false;
    }

    QList<DABEPGItem*>* pEPGListForService = this->getEPGList(index, ensembleName, serviceName, true);

    if(pEPGListForService == NULL)
    {
        qDebug() << __FUNCTION__ << "ERROR : can't find EPGList for " << ensembleName <<","<<serviceName;
        DABLogger::instance()->Log((QString("addEPGItem() :: ================ can't find EPGList for = ====")));
        return false;
    }

    DABEPGItem* pNewItem = new DABEPGItem(pItem, this);
    if(pNewItem == NULL)
    {
        qDebug() << __FUNCTION__ << ": ERROR allocation Error!!";
        DABLogger::instance()->Log((QString("addEPGItem() :: ERROR allocation Error ====")));
        return NULL;
    }

    pEPGListForService->append(pNewItem);

//    //Debugging ==============================
//    int count = pEPGDateItemList->count();
//    int i = 0;
//    DABEPGDateListItem *pDateListItem = NULL;
//    for(i=0; i < count; i++)
//    {
//        pDateListItem = pEPGDateItemList->at(i);
//        qDebug() << __FUNCTION__ << " pEPGDateItemList[" << i <<"] m_sEPGDate = " << pDateListItem->getEPGDate();
//    }
    return true;
}

void
DABEPGManager::fullSave(void)
{
    qDebug() << Q_FUNC_INFO;

    DABEPGItem* pItem;
    DABEPGDateListItem *pDateListItem = NULL;
    QList<DABEPGItem*>* pEPGListForService = NULL;
    QList<DABEPGItem*>::iterator k;
    QHash<QString, QVariant*>::iterator i;
    QHash<QString, QVariant*>::iterator j;
    QHash<QString, QVariant*>* pEPGHansh;
    QHash<QString, QVariant*>* pEnsembleHash = NULL;
    stEVT_EPG_SAVE_ITEM saveEPGItem;
    QString eName = "";
    QString sName = "";
    int sizeSaveData = 0;
    int index = 0;
    int listCount = 0;

    sizeSaveData = sizeof(saveEPGItem);
    memset(&saveEPGItem, 0x00, sizeSaveData);

    listCount = pEPGDateItemList->count();

    qDebug() << __FUNCTION__ << "====== Save All EPG Data ===================================================";
    qDebug() << __FUNCTION__ << " :: EPG DateItem List count = " << listCount;
    for(index=0; index < listCount; index++)
    {
        qDebug() << __FUNCTION__ << " :: index = " << index;
        pDateListItem = pEPGDateItemList->at(index);
        quint32 mjd = pDateListItem->getEPGDate();
        QDate date = QDate::fromJulianDay(mjd+2400001);
        QString strMjd = QString::number(mjd,10);
        QString filename = DAB_DATA_ROOT + strMjd + "_" + date.toString("dd.MM.yyyy") + ".dat";
        qDebug() << " File Name = " << filename;
        QFile file(filename);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            qDebug() << Q_FUNC_INFO << "File Not found: " + filename;
            return; //temporaty...
        }

        QDataStream out;
        out.setDevice(&file);

        int epgCount = 0;

        pEPGHansh = pDateListItem->getEPGHashTable();
        for(i = pEPGHansh->begin(); i != pEPGHansh->end(); i++)
        {
            pEnsembleHash = (QHash<QString, QVariant*>*)i.value();
            eName = i.key();
            qDebug() << "= pEnsembleHash List count : " << pEnsembleHash->count();
            for(j = pEnsembleHash->begin(); j != pEnsembleHash->end(); j++)
            {
                pEPGListForService = (QList<DABEPGItem*>*)j.value();
                qDebug() << "= Service List count : " << pEPGListForService->count();
                sName = j.key();
                for(k = pEPGListForService->begin(); k != pEPGListForService->end(); k++)
                {
                    pItem = k[0];
                    qDebug() << "[ "<< date.toString() << "] [" << mjd <<"] [" << eName<< "," << sName << "] [" << epgCount++ << "]" << pItem->getStartTime() << "-" << pItem->getEndTime() << " Program Title :" << pItem->programtitle() << " Descrition : " << pItem->description();
                    saveEPGItem.mjd = mjd;
                    pItem->saveData(&saveEPGItem);
                    out.writeRawData((const char*)&saveEPGItem, sizeSaveData);
                    memset(&saveEPGItem, 0x00, sizeSaveData);
                }
            }
        }
        file.close();
    }
    qDebug() << __FUNCTION__ << "=====Save EPG END ===================================================================";
}


void
DABEPGManager::removeEPGData(quint32 mjd)
{
    qDebug() << Q_FUNC_INFO;

    QDate date = QDate::fromJulianDay(mjd+2400001);
    QString strMJD = QString::number(mjd,10);
    QString strDate = date.toString("dd.MM.yyyy");
    QString filename = DAB_DATA_ROOT + strMJD + "_" + strDate + ".dat";

    if(QFile::remove(filename))
    {
        qDebug() << __FUNCTION__ << " :: " << filename<< " :: File Remove Successful";
    }
    else
    {
        qDebug() << __FUNCTION__ << " :: " << filename<< " :: File Remove Fail";
    }
}


void
DABEPGManager::removeEPGFile(QString path)
{
    QDir directory;
    directory.setPath(path);

    QStringList list = directory.entryList();
    foreach (const QString &str, list)
    {
        if(str.contains(".dat"))
        {
            QString fullPath = path + str;
            QFile file(fullPath);
//            DABLogger::instance()->Log((QString("  =====DABEPGManager::removeEPGFile() :: fullPath = %1").arg(fullPath)));
            if(file.remove())
            {
                DABLogger::instance()->Log((QString("  =====DABEPGManager::removeEPGFile() :: File Remove Sucess : %1").arg(fullPath)));
            }
            else
            {
                DABLogger::instance()->Log((QString("  =====DABEPGManager::removeEPGFile() :: File Remove Fail : %1").arg(fullPath)));
            }
            file.close();
        }
        else if(!(str.contains('.') || str.contains("..")))
        {
            QString subPath = path + str +"/";
            removeEPGFile(subPath);
            QDir dir;
//            DABLogger::instance()->Log((QString("  =====DABEPGManager::removeEPGFile() :: subPath = %1").arg(subPath)));
            if(dir.rmpath(subPath))
            {
                DABLogger::instance()->Log((QString("  =====DABEPGManager::removeEPGFile() :: Dir Remove Fail : %1").arg(subPath)));
            }
            else
            {
                DABLogger::instance()->Log((QString("  =====DABEPGManager::removeEPGFile() :: Dir Remove Fail : %1").arg(subPath)));
            }
        }
    }
}

void
DABEPGManager::remove(void)
{
    QDir directory;
    directory.setPath(DAB_STATION_EPG_ROOT);

    QStringList list = directory.entryList();
    foreach (const QString &str, list)
    {
        if((str.contains('.') || str.contains("..")))
        {
            continue;
        }

        QString fullPath = DAB_STATION_EPG_ROOT + str + "/";
        QString strDate = str;
        bool ok;
        int year = strDate.left(4).toInt(&ok);
        strDate.remove(0, 5);
        int month = strDate.left(2).toInt(&ok);
        int day = strDate.right(2).toInt(&ok);
        QDate epgDate(year, month, day);
        QDate currentDate = QDate::currentDate();
        QDate addDate = currentDate.addDays(6);

        DABLogger::instance()->Log((QString("  =====DABEPGManager::removeEPGFile() :: currentDate = %1 EPG Date =%2 Add Date =%3").arg(epgDate.toString()).arg(currentDate.toString()).arg(addDate.toString())));
        if(epgDate < currentDate || epgDate > addDate)
        {
            removeEPGFile(fullPath);
        }
    }
}

void
DABEPGManager::getEPGFullPath(QString path)
{
    QDir dir;
    QString fullPath = "";
    dir.setPath(path);

    QStringList list = dir.entryList();

//    DABLogger::instance()->Log((QString("  =====getEPGFullPath() : path = %1").arg(path)));

    foreach (const QString &str, list)
    {
//        DABLogger::instance()->Log((QString("  =====getEPGFullPath() : str = %1").arg(str)));

        if(str.contains(".dat"))
        {
            fullPath = path + str;
//            DABLogger::instance()->Log((QString("  =====getEPGFullPath() : File Found !!. break. FullPath = %1").arg(fullPath)));
            constructEPGData(fullPath);
            break;
        }

        if(!(str.contains('.') || str.contains("..")))
        {
            QString subPath = path + str +"/";
            qDebug() << " path = " << subPath;
//            DABLogger::instance()->Log((QString("  =====getEPGFullPath() : File Not Found !!. getEPGFullPath() Called. subPath = %1").arg(subPath)));
            getEPGFullPath(subPath);
        }
    }
}

void
DABEPGManager::constructEPGData(QString path)
{
    stEVT_EPG_SAVE_ITEM data;

    int readSize = sizeof(data);
    memset(&data, 0x00, readSize);

//    DABLogger::instance()->Log((QString("  =====DABEPGManager::constructEPGData(QString path) : path = %1").arg(path)));

    QFile filedata(path);
    QDataStream out;
    out.setDevice(&filedata);

    DABLogger::instance()->Log((QString("  =====EPG Load START ===================================================================")));
    if(filedata.open(QIODevice::ReadOnly))
    {
        int epgCount = 0;
        while(out.readRawData((char*)&data,readSize) == readSize)
        {
            QString programTitle = "";
            QString description = "";
            programTitle = QString::fromUtf8(data.programTitle, strlen(data.programTitle));
            description = QString::fromUtf8(data.description, strlen(data.description));            
#ifdef __UNIX_DESKTOP__
            QDate currentDate = QDate::currentDate();
            quint32 mjd = DABLogger::instance()->convertDateToMJD(currentDate);
            QDate epgDate = QDate::fromJulianDay(mjd+2400001);

            QTime cTime = QTime::currentTime();
            QTime rTime(data.sHour, data.sMinute, data.sSecond);
            QTime eTime = rTime.addSecs(data.duration);

            qDebug() << " cTime = " << cTime;
            qDebug() << " rTime = " << rTime;
            qDebug() << " eTime = " << eTime;

            if(epgDate == currentDate)
            {
                if(eTime >= cTime || rTime >= cTime)
                {
                    qDebug() << "[" << data.mjd <<"] [" << data.serviceID << "] [" << data.sHour << "] [" << data.sMinute <<"] [" << data.sSecond << "] [" << data.duration << "] ["  << programTitle << "] [" << description << "]";
                    emit cmdAddEPGItem(mjd, data.serviceID, data.sHour, data.sMinute, data.sSecond, data.duration, programTitle, description);
                }
            }
            else if(epgDate > currentDate)
            {
                qDebug() << "[" << data.mjd <<"] [" << data.serviceID << "] [" << data.sHour << "] [" << data.sMinute <<"] [" << data.sSecond << "] [" << data.duration << "] ["  << programTitle << "] [" << description << "]";
                emit cmdAddEPGItem(mjd, data.serviceID, data.sHour, data.sMinute, data.sSecond, data.duration, programTitle, description);
            }
#else
            emit cmdAddEPGItem(data.mjd, data.serviceID, data.sHour, data.sMinute, data.sSecond, data.duration, programTitle, description);
#endif
            epgCount++;
            memset(&data, 0x00, readSize);
        }
        DABLogger::instance()->Log((QString(" initEPGData() : EPG Count = %1").arg(epgCount)));
    }
    else
    {
        DABLogger::instance()->Log((QString(" initEPGData() : File Not found: path = %1").arg(path)));
        return;
    }
    filedata.close();
    DABLogger::instance()->Log((QString("  =====EPG Load END ===================================================================")));
}

void
DABEPGManager::load(void)
{
    DABLogger::instance()->Log((QString("  =====FULL EPG PATH LOAD START ===================================================================")));
    getEPGFullPath(DAB_STATION_EPG_ROOT);
    DABLogger::instance()->Log((QString("  =====FULL EPG PATH LOAD END ===================================================================")));
}

void
DABEPGManager::showEPG(void) //For Debugging..
{
    qDebug() << Q_FUNC_INFO;

    QHash<QString, QVariant*>* pEnsembleHash = NULL;
    QHash<QString, QVariant*>* pEPGHansh;
    QList<DABEPGItem*>* pEPGListForService = NULL;
    DABEPGDateListItem *pDateListItem = NULL;

    QHash<QString, QVariant*>::iterator i;
    QHash<QString, QVariant*>::iterator j;
    QList<DABEPGItem*>::iterator k;
    DABEPGItem* pItem;
    QString eName;
    QString sName;
    int count = 0;
    int listCount = pEPGDateItemList->count();
    int index = 0;
    qDebug() << "====== Show All EPG Data ===================================================";

    qDebug() << "= EPGDate List Count = " << listCount;

    for(index=0; index < listCount; index++)
    {
        pDateListItem = pEPGDateItemList->at(index);
        quint32 mjd = pDateListItem->getEPGDate();
        QDate date = QDate::fromJulianDay(mjd+2400001);
        pEPGHansh = pDateListItem->getEPGHashTable();
        qDebug() << __FUNCTION__ << " pEPGDateItemList[" << index <<"]" << " m_sEPGDate = " << pDateListItem->getEPGDate();

        for(i = pEPGHansh->begin(); i != pEPGHansh->end(); i++)
        {
            pEnsembleHash = (QHash<QString, QVariant*>*)i.value();
            eName = i.key();
            for(j = pEnsembleHash->begin(); j != pEnsembleHash->end(); j++)
            {
                pEPGListForService = (QList<DABEPGItem*>*)j.value();
                qDebug() << "= Service List count : " << pEPGListForService->count();
                sName = j.key();
                for(k = pEPGListForService->begin(); k != pEPGListForService->end(); k++)
                {
                    pItem = k[0];
                    qDebug() << "[ "<< date.toString() << "] [" << mjd <<"] [" << eName<< "," << sName << "] [" << count++ << "]" << pItem->getStartTime() << "-" << pItem->getEndTime() << " Program Title :" << pItem->programtitle();
//                    qDebug() << "[ "<< mjd << "]  [" << eName<< "," << sName << "] [" << count++ << "]" << pItem->getStartTime() << "-" << pItem->getEndTime() << " Program Title :" << pItem->programtitle();
                }
            }
        }
    }
    qDebug() << "===== EPG END ===================================================================";

//    int count = 0;
////    int freq = frequency;
//    QHash<QString, QVariant*>* pEnsembleHash = NULL;
//    QList<DABEPGItem*>* pEPGListForService = NULL;

//    QHash<QString, QVariant*>::iterator i;
//    QHash<QString, QVariant*>::iterator j;
//    QList<DABEPGItem*>::iterator k;
//    DABEPGItem* pItem;
//    QString eName;
//    QString sName;
//    qDebug() << "====== Show All EPG Data =================";
//    qDebug() << "= Ensemble hash count : " << m_EPGHash.count();
//    for(i = this->m_EPGHash.begin(); i != this->m_EPGHash.end(); i++)
//    {
//        pEnsembleHash = (QHash<QString, QVariant*>*)i.value();
//        eName = i.key();
//        for(j = pEnsembleHash->begin(); j != pEnsembleHash->end(); j++)
//        {
//            pEPGListForService = (QList<DABEPGItem*>*)j.value();
//            qDebug() << "= Service List count : " << pEPGListForService->count();
//            sName = j.key();
//            for(k = pEPGListForService->begin(); k != pEPGListForService->end(); k++)
//            {
//                pItem = k[0];
//                qDebug() << "   [" << eName<< "," << sName << "] [" << count++ << "]" << pItem->getStartTime() << "-" << pItem->getEndTime() << " Program Title :" << pItem->programtitle();
//            }
//        }
//    }
//    qDebug() << " ===== EPG END ================";
}

QList<DABEPGItem*>*
DABEPGManager::sortEPG(quint32 mjd, QString ensembleName, QString serviceName)
{
    int index = getEPGHashIndex(mjd, false);

    if(index == -1)
    {
        qDebug() << __FUNCTION__ << "ERROR : can't find EPGList for " << ensembleName <<","<<serviceName;
        return NULL;
    }

    QList<DABEPGItem*>* pEPGListForService = this->getEPGList(index, ensembleName, serviceName, false);

    if((pEPGListForService == NULL) || (pEPGListForService->count() == 0))
    {
        qDebug() << __FUNCTION__ << "pEPGListForService is NULL !! " << ensembleName <<","<<serviceName;
        return NULL;
    }

    sortEPGInOrderFrom0To24(pEPGListForService);
    return pEPGListForService;
}

QList<DABEPGItem*>*
DABEPGManager::getEPGServiceList(quint32 mjd, QString ensembleName, QString serviceName)
{
    int index = getEPGHashIndex(mjd, false);

    if(index == -1)
    {
        qDebug() << __FUNCTION__ << "ERROR : can't find EPGList for " << ensembleName <<","<<serviceName;
        return NULL;
    }

    QList<DABEPGItem*>* pEPGListForService = this->getEPGList(index, ensembleName, serviceName, false);

    if((pEPGListForService == NULL) || (pEPGListForService->count() == 0))
    {
        qDebug() << __FUNCTION__ << "pEPGListForService is NULL !! " << ensembleName <<","<<serviceName;
        return NULL;
    }
    return pEPGListForService;
}

bool
DABEPGManager::getCurrentDateEPGExists(QString ensembleLabel, QString serviceLabel)
{
    qDebug() << __FUNCTION__ << " : EnsembleName : " << ensembleLabel << ", ServiceName : " << serviceLabel;
    if(ensembleLabel == "" || serviceLabel == "")
    {
        qDebug() << __FUNCTION__ << ": ERROR ensemble:" << ensembleLabel <<", service:" << serviceLabel;
        return false;
    }

    DABEPGDateListItem *pDateListItem = NULL;
    QHash<QString, QVariant*>* pEnsembleHash = NULL;
    QHash<QString, QVariant*>* pEPGHansh = NULL;
    QList<DABEPGItem*>* pEPGListForService = NULL;

    QDate currentDate = QDate::currentDate();
    int mjd = DABLogger::instance()->convertDateToMJD(currentDate);

    int index = getEPGHashIndex(mjd, false);
    if(index == -1)
    {
        qDebug() << __FUNCTION__ << " Index is -1 !! so EPG item is not exists. return";
        return false;
    }

    qDebug() << __FUNCTION__ << " DateListIndex = " << index;
    pDateListItem = pEPGDateItemList->at(index);
    if(pDateListItem == NULL)
    {
        qDebug() << __FUNCTION__ << " EPG Item is not exists. return";
        return false;
    }

    pEPGHansh = pDateListItem->getEPGHashTable();

    QHash<QString, QVariant*>::iterator i = pEPGHansh->find(ensembleLabel);
    if( i == pEPGHansh->end() )
    {
        qDebug() << " No ensemble Hash.";
        return false;
    }
    else if( i.key() == ensembleLabel)
    {
        qDebug() << " Find OK ensemble Hash: key:" << i.key();
        pEnsembleHash = (QHash<QString, QVariant*>*)i.value();
    }

    if(pEnsembleHash == NULL)
    {
        qDebug() << " pEnsembleHash is NULL";
        return false;
    }

    QHash<QString, QVariant*>::const_iterator eIterator = pEnsembleHash->find(serviceLabel);
    if( eIterator == pEnsembleHash->end())
    {
        qDebug() << " No service List.";
        return false;
    }
    else if( eIterator.key() == serviceLabel)
    {
        qDebug() << " Find OK service List: key:" << eIterator.key();
        pEPGListForService = (QList<DABEPGItem*>*)eIterator.value();
    }

    if(pEPGListForService == NULL)
    {
        qDebug() << " pEPGListForService is NULL";
        return false;
    }

    qDebug() << __FUNCTION__ << " pEPGListForService->count = " << pEPGListForService->count();
#if 1
    if(pEPGListForService->count())
    {
        bool bExist = false;
        QList<DABEPGItem*>::iterator iterator;
        DABEPGItem* pEpgItem = NULL;
        QTime cTime = QTime::currentTime();
        QDate currentDate = QDate::currentDate();
        quint32 cMjd = DABLogger::instance()->convertDateToMJD(currentDate);
        for(iterator = pEPGListForService->begin(); iterator != pEPGListForService->end(); iterator++)
        {
            pEpgItem = iterator[0];
            QTime tTime(0,0,0);
            QTime endTime = pEpgItem->getEndTime();
            if(tTime == endTime)
            {
                endTime.setHMS(23,59,59);
            }
            if((!(endTime >= cTime || pEpgItem->getStartTime() >= cTime)) && (mjd == cMjd))
            {
                continue;
            }
            bExist = true;
        }
        return bExist;
    }
    else
    {
        return false;
    }
#else
    if(pEPGListForService->count())
    {
        return true;
    }
    else
    {
        return false;
    }
#endif
}

void
DABEPGManager::saveEPGData(QList<DABEPGItem*>* pEPGListForService, quint32 mjd, QString fLabel, quint16 eID, quint32 sID)
{
    DABEPGItem* pItem = NULL;
    QList<DABEPGItem*>::iterator i = pEPGListForService->begin();

    QDate date = QDate::fromJulianDay(mjd+2400001);
    QString strDate = date.toString("yyyy_MM_dd");
    QString strSID = QString::number(sID,10);
    QString strEID = QString::number(eID,10);
    QString filePath = DAB_STATION_EPG_ROOT + strDate + "/" + fLabel + "/" + strEID + "/";
    QString filename = DAB_STATION_EPG_ROOT + strDate + "/" + fLabel + "/" + strEID + "/" + strSID + ".dat";

    qDebug() << __FUNCTION__ << " EPG File Name = " << filename;

    QDir dir(filePath);
    if(!dir.exists())
    {
        if(!dir.mkpath(filePath))
        {
            qDebug() << __FUNCTION__ <<"make directory Fail !!";
            return;
        }
        else
        {
            qDebug() << __FUNCTION__ << " :: " << filePath << " make success !!";
        }
    }

    QFile saveEPGFile(filename);
    if(saveEPGFile.exists())
    {
        qDebug() << __FUNCTION__ << " :: EPG File is exists. so remove!!";
        saveEPGFile.remove();
    }
    else
    {
        qDebug() << __FUNCTION__ << " :: EPG File is not exists!!";
    }

    if (!saveEPGFile.open(QIODevice::WriteOnly))
    {
        qDebug() << Q_FUNC_INFO << "File Not found: " + filename;
        return;
    }

    QDataStream out;
    out.setDevice(&saveEPGFile);

    stEVT_EPG_SAVE_ITEM saveEPGItem;
    int sizeSaveData = 0;

    sizeSaveData = sizeof(saveEPGItem);
    memset(&saveEPGItem, 0x00, sizeSaveData);

    for(int index =0 ; i != pEPGListForService->end(); i++, index++)
    {
        pItem = i[0];
        saveEPGItem.mjd = mjd;
        pItem->saveData(&saveEPGItem);
        out.writeRawData((const char*)&saveEPGItem, sizeSaveData);
        memset(&saveEPGItem, 0x00, sizeSaveData);
    }
    saveEPGFile.close();
}

void
DABEPGManager::sortEPGInOrderFrom0To24(QList<DABEPGItem*>* pEPGListForService)
{
    DABEPGItem* pItem = NULL;
    QList<DABEPGItem*>::iterator i = pEPGListForService->begin();
    QTime rTime(0,0,0);
    int count = pEPGListForService->count();
    for(int index =0 ; i != pEPGListForService->end(); i++, index++)
    {
        pItem = i[0];
        //qDebug() << "["<<index<<"] StartTime:" << (pItem->getStartTime().toString()) << ", EndTime:" << pItem->getEndTime();
        if(pItem->getStartTime() > rTime){
            //qDebug() << " ==> Move 0 to " << count-1;
            pEPGListForService->move(0, count-1);
        }else{
            //qDebug() << " ==> Break";
            break;
        }
    }
}

void
DABEPGManager::sortEPGInOrderToCurrentTime(QList<DABEPGItem*>* pEPGListForService)
{ 
    DABEPGItem* pItem = NULL;
    QList<DABEPGItem*>::iterator i = pEPGListForService->begin();
    QTime rTime= QTime::currentTime();
    int count = pEPGListForService->count();

    for(int index =0; i != pEPGListForService->end() && index < count; i++, index++)
    {
        pItem = i[0];
//        qDebug() << "["<<index<<"] StartTime:" << (pItem->getStartTime().toString()) << ", EndTime:" << pItem->getEndTime() << "Current Time: " << rTime.toString();
#if 1
        if(!(pItem->getEndTime() >= rTime || pItem->getStartTime() >= rTime))
        {
            qDebug() << __FUNCTION__ << " This EPG ITEM is removed !! ["<<index<<"] StartTime:" << (pItem->getStartTime().toString()) << ", EndTime:" << pItem->getEndTime();
            pEPGListForService->removeFirst();
        }
#else
        if(pItem->getEndTime() >= rTime || pItem->getStartTime() >= rTime)
        {

        }
        else
        {
            qDebug() << __FUNCTION__ << " This EPG ITEM is removed !! ["<<index<<"] StartTime:" << (pItem->getStartTime().toString()) << ", EndTime:" << pItem->getEndTime();
            pEPGListForService->removeAt(index);
        }
#endif
    }
}
