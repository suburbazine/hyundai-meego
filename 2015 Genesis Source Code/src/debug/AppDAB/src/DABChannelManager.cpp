#include "DABChannelManager.h"
#include "DABApplication_Def.h"
#include <QStandardItemModel>

DABChannelManager::DABChannelManager(DABUIListener &aDABUIListener, QObject *parent) :
    QObject(parent),
    m_DABUIListener(aDABUIListener)
{
    this->m_CurrentChannel = new DABChannelItem(parent);
    this->m_listChannel = new ListModel(new DABChannelItem(parent), parent);
    this->m_listPresetChannel = new ListModel(new DABChannelItem(parent), parent);
    this->m_tempListChannel = new ListModel(new DABChannelItem(parent), parent);
    this->m_newListChannel = new ListModel(new DABChannelItem(parent), parent);
    this->m_tempEpgListChannel = new ListModel(new DABChannelItem(parent), parent);
    this->m_iTempChannelCount = 0;
    this->m_iNewChannelCount = 0;
    this->m_iChannelCount = 0;
    initPtyTable();
}

DABChannelManager::~DABChannelManager()
{
    delete this->m_CurrentChannel;
    delete this->m_listChannel;
    delete this->m_listPresetChannel;
    delete this->m_tempListChannel;
    delete this->m_newListChannel;
    delete this->m_tempEpgListChannel;
}


/*
===========================================================================================
+++++++++++++++++++++++++++ Channel Item Info +++++++++++++++++++++++++++++++++++++++++++++
===========================================================================================
*/
QString
DABChannelManager::getProgrameTypeName(int num)
{
    QString str;
    str.resize(1);
    str[0] = QChar(0xfffe);

    if(num == 255 || num == 0)
    {
        return (m_DABUIListener.getLanguageID() == 2 || m_DABUIListener.getLanguageID() == 12) ? str : "zzzz";
    }
    else
    {
        return ptyTable[num];
    }

#if 0
#if 1
    if(num == 255 || num == 0)
    {
        QString str;
        str.resize(1);
        str[0] = QChar(0xfffe);
        return str;
    }
    else
    {
        return ptyTable[num];
    }
#else
    if(num == 255 || num == 0)
        return "zzzzz";
    return ptyTable[num];
#endif
#endif
}

void
DABChannelManager::initPtyTable(void)
{
    int cnt =0;
    ptyTable[cnt++] = "...";
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_NEWS");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_CURRENT_AFFAIRS");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_INFORMATION");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_SPORT");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_EDUCATION");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_DRAMA");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_CULTURE");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_SCIENCE");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_VARIED");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_POP_MUSIC");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_ROCK_MUSIC");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_EASY_LISTENING_MUSIC");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_LIGHT_CLASSICAL_MUSIC");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_SERIOUS_CLASSICAL_MUSIC");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_OTHER_MUSIC");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_WEATHER");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_FINANCE");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_CHILDREN_PROGRAM");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_SOCIAL_AFFAIRS");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_RELIGION");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_PHONE_IN");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_TRAVEL");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_LEISURE");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_JAZZ_MUSIC");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_COUNTRY_MUSIC");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_NATIONAL_MUSIC");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_OLDIES_MUSIC");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_FOLK_MUSIC");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_DOCUMENTARY");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_ALARM_TEST");
    ptyTable[cnt++] = QApplication::translate("StringInfo", "STR_DAB_PTY_ALARM");
}

/*
===========================================================================================
+++++++++++++++++++++++++++ Channel List Manager ++++++++++++++++++++++++++++++++++++++++++
===========================================================================================
*/

int
DABChannelManager::findNextChannelIndexForChannelScan(DABChannelItem* pItem)
{
    DABChannelItem* pNextItem = NULL;
    if(pItem == NULL) return -1;

    ListModel* pList = this->getCurrentChannelList();
    if(pList == NULL){
        qDebug() << __FUNCTION__ << ": CurrentChannel List is NULL";
        return -1;
    }

    int index = pList->findIndex(pItem->key(), pItem->subKey(), pItem->id());
    int backIndex = index;
    if( index == -1)
    {
        int count = pList->rowCount();
        for(int i = 0; i < count; i++)
        {
            pNextItem = (DABChannelItem*)pList->dataItem(i);
            if(pItem->frequency() < pNextItem->frequency())
            {
                index = i;
                qDebug() << "     find!!! ";
                break;
            }
        }

        if(index == -1 && count != 0)
        {
            index = 0;
        }
    }
    else
    {
        int findIndex = index+1;
        int count = pList->rowCount();
        if( findIndex >= count)
        {
            qDebug() << "  last item so move to fisrt item";
            index = 0;
        }
        else
        {
            for(int i=index; i<count; i++)
            {
                pNextItem = (DABChannelItem*)pList->dataItem(i);
                if((pItem->id() == pNextItem->id()) && (pItem->frequency() == pNextItem->frequency()) && (pItem->subChId() == pNextItem->subChId()))
                {
                    index = ++i;
                    break;
                }
            }
        }
    }

    if(backIndex == index)
        index = -1;

    return index;
}

int
DABChannelManager::findNextChannelIndexInChannelList(DABChannelItem* pItem)
{
    DABChannelItem* pNextItem = NULL;
    if(pItem == NULL) return -1;

    ListModel* pList = this->getCurrentChannelList();
    if(pList == NULL){
        qDebug() << __FUNCTION__ << ": CurrentChannel List is NULL";
        return -1;
    }

    int index = pList->findIndex(pItem->key(), pItem->subKey(), pItem->id());
    int backIndex = index;
    if( index == -1)
    {
        int count = pList->rowCount();
        for(int i = 0; i < count; i++)
        {
            pNextItem = (DABChannelItem*)pList->dataItem(i);
            if(pItem->frequency() < pNextItem->frequency())
            {
                index = i;
                qDebug() << "     find!!! ";
                break;
            }
        }

        if(index == -1 && count != 0)
        {
            index = 0;
        }
    }
    else
    {
        int findIndex = index+1;
        int count = pList->rowCount();
        if( findIndex >= count)
        {
            qDebug() << "  last item move to fisrt item";
            index = 0;
        }
        else
        {
            for(int i=index; i<count; i++)
            {
                pNextItem = (DABChannelItem*)pList->dataItem(i);
                if((pItem->id() == pNextItem->id()) && (pItem->frequency() == pNextItem->frequency()) && (pItem->subChId() == pNextItem->subChId()))
                {
                    index = ++i;
                    break;
                }
            }
        }
    }

    if(backIndex == index)
    {
        qDebug() << "========================================================================";
        qDebug() << "not found next channel so last item move to fisrt item";
        qDebug() << "========================================================================";
        index = 0;
    }

    return index;
}

int
DABChannelManager::findPrevChannelIndexInChannelList(DABChannelItem* pItem)
{
    DABChannelItem* pNextItem = NULL;
    if(pItem == NULL) return -1;

    ListModel* pChannelList = this->getCurrentChannelList();

    if(pChannelList == NULL){
        qDebug() << __FUNCTION__ << ": CurrentChannel List is NULL";
        return -1;
    }

    int index = pChannelList->findIndex(pItem->key(), pItem->subKey(), pItem->id());
    if( index == -1)
    {
        int count = pChannelList->rowCount();
        for(int i = count-1; i >= 0; i--)
        {
            pNextItem = (DABChannelItem*)pChannelList->dataItem(i);
            if(pItem->frequency() > pNextItem->frequency())
            {
                index = i;
                qDebug() << "     find!!! ";
                break;
            }
        }

        if(index == -1 && count != 0)
        {
            index = count-1;
        }
    }
    else
    {
        int count = pChannelList->rowCount();
        for(int i=index; i<count; i++)
        {
            pNextItem = (DABChannelItem*)pChannelList->dataItem(i);
            if((pItem->id() == pNextItem->id()) && (pItem->frequency() == pNextItem->frequency()) && (pItem->subChId() == pNextItem->subChId()))
            {
                index = --i;
                break;
            }
        }
        if( index < 0)
        {
            index = count-1;
        }
    }
    return index;
}

int
DABChannelManager::loadStationList(int sortType)
{
    qDebug() << Q_FUNC_INFO;

    ListModel* pChannelList = this->getCurrentChannelList();
    ListModel* pStationList = m_DABUIListener.getStationList();

    int listCount = 0;
    int stationCount = 0;
    DABChannelItem* pItem = NULL;
    quint32 eFreq = 0;

    if(m_DABUIListener.getViewMode())
    {
        m_DABUIListener.modeChange(false);
    }

    listCount = pChannelList->rowCount();
    pStationList->removeAllRows();

    for(int i=0; i<listCount; i++)
    {
        pItem = (DABChannelItem*)pChannelList->dataItem(i);
        pStationList->appendRow(new DABChannelItem(pItem));
    }

    stationCount = pStationList->rowCount();

    if(sortType == DAB_STATION_SORT_ENSEMBLE)
    {
        for(int i = 0; i < stationCount; i++)
        {
            pItem = (DABChannelItem*) pStationList->dataItem(i);
            if(eFreq != pItem->frequency() && pItem->frequency() != 0)
            {
                emit sendEnsembleName(pItem->ensembleLabel(), pItem->frequency(), m_DABUIListener.getSectionStatus(pItem->frequency()));
                eFreq = pItem->frequency();
            }
        }

        this->loadCurrentFreguencyChannel(this->getCurrentFrequencyID(), true);
        emit playIndexUpdate(true);
    }
    else
    {
        loadStationListBySorting(sortType);
    }

    return stationCount;
}

int
DABChannelManager::loadStationListBySorting(int sortingBy)
{
    qDebug() << Q_FUNC_INFO << ", sortingBy:" << sortingBy;

    bool bPlayIcon = false;
    int count = 0;
    ListModel* pListModel = m_DABUIListener.getStationList();
    ListModel* pStationListPTY = m_DABUIListener.getStationListPTY();
    ListModel* pStationListAlphabet = m_DABUIListener.getStationListAlphabet();
    DABChannelItem* pCurrentItem = this->getCurrentChannelItem();

    count = pListModel->rowCount();
    pStationListPTY->removeAllRows();
    pStationListAlphabet->removeAllRows();

    QStandardItemModel alphabetModel;
    for(int nCnt = 0; nCnt < count; nCnt++)
    {
        DABChannelItem* cItem = (DABChannelItem*)pListModel->dataItem(nCnt);
        QStandardItem* sItem = new QStandardItem(nCnt);
        sItem->setText(cItem->label().toLower());
        alphabetModel.appendRow(sItem);
    }

    QSortFilterProxyModel alphabetFilterModel;
    alphabetFilterModel.setSourceModel(&alphabetModel);
    alphabetFilterModel.setSortLocaleAware(true);
    alphabetFilterModel.setSortRole(Qt::DisplayRole);
    alphabetFilterModel.setDynamicSortFilter(true);
    alphabetFilterModel.sort(0);

    if(sortingBy == DAB_STATION_SORT_ALPHABET)
    {
        for(int nCnt = 0; nCnt < alphabetFilterModel.rowCount() ; nCnt++)
        {
            QStandardItem* alphabetItem = alphabetModel.itemFromIndex(alphabetFilterModel.mapToSource(alphabetFilterModel.index(nCnt,0)));
            DABChannelItem* pItem = (DABChannelItem*)pListModel->dataItem(alphabetItem->row());
            qWarning() << __FUNCTION__ << " ====current item isDABtoDAB = " << pCurrentItem->isDABtoDAB() << ", isDABtoFM = " << pCurrentItem->isDABtoFM() << ", isNoSignal = " << pCurrentItem->isNoSignal();
            qWarning() << __FUNCTION__ << " ====next    item isDABtoDAB = " << pItem->isDABtoDAB() << ", isDABtoFM = " << pItem->isDABtoFM() << ", isNoSignal = " << pItem->isNoSignal();
            if((pCurrentItem->subChId() == pItem->subChId()) && (pCurrentItem->serviceID() == pItem->serviceID()) && (pCurrentItem->frequency() == pItem->frequency()))
            {
                bPlayIcon = true;
                pItem->setDABtoFM(pCurrentItem->isDABtoFM());
                pItem->setNoSignal(pCurrentItem->isNoSignal());
                pItem->setDABtoDAB(pCurrentItem->isDABtoDAB());
            }
            else
            {
                pItem->setDABtoFM(false);
                pItem->setNoSignal(false);
                pItem->setDABtoDAB(false);
                bPlayIcon = false;
            }
            emit sendServiceNameForSortingBy(nCnt, pItem->frequency(), pItem->ensembleLabel() ,pItem->label(), pItem->programeType(), pItem->ps(), pItem->isDABtoDAB(), pItem->isDABtoFM(), pItem->isNoSignal(), bPlayIcon, nCnt);
            pStationListAlphabet->appendRow(new DABChannelItem(pItem));
        }
    }
    else if(sortingBy == DAB_STATION_SORT_PTY)
    {
        QStandardItemModel ptyModel;
        ListModel* m_tempListChannel = new ListModel(new DABChannelItem(this), this);
        for(int nCnt = 0; nCnt < alphabetFilterModel.rowCount() ; nCnt++)
        {
            QStandardItem* alphabetItem = alphabetModel.itemFromIndex(alphabetFilterModel.mapToSource(alphabetFilterModel.index(nCnt,0)));
            DABChannelItem* pItem = (DABChannelItem*)pListModel->dataItem(alphabetItem->row());
            pItem->setProgrameTypeName(getProgrameTypeName(pItem->programeType()));
            m_tempListChannel->appendRow(pItem);
            QStandardItem* ptyItem = new QStandardItem(nCnt);
            ptyItem->setText(pItem->programeTypeName());
            ptyModel.appendRow(ptyItem);
        }

        QSortFilterProxyModel sortfilterModel;
        sortfilterModel.setSourceModel(&ptyModel);
        if(m_DABUIListener.getLanguageID() != 2 && m_DABUIListener.getLanguageID() != 12) // Korea, Russian Language
            sortfilterModel.setSortLocaleAware(true);
        sortfilterModel.setSortRole(Qt::DisplayRole);
        sortfilterModel.setDynamicSortFilter(true);
        sortfilterModel.sort(0);

        for(int nCnt = 0; nCnt < sortfilterModel.rowCount() ; nCnt++)
        {
            QStandardItem* ptyItem = ptyModel.itemFromIndex(sortfilterModel.mapToSource(sortfilterModel.index(nCnt,0)));
            DABChannelItem* nextItem = (DABChannelItem*)m_tempListChannel->dataItem(ptyItem->row());
            qWarning() << __FUNCTION__ << " ====current item isDABtoDAB = " << pCurrentItem->isDABtoDAB() << ", isDABtoFM = " << pCurrentItem->isDABtoFM() << ", isNoSignal = " << pCurrentItem->isNoSignal();
            qWarning() << __FUNCTION__ << " ====next    item isDABtoDAB = " << nextItem->isDABtoDAB() << ", isDABtoFM = " << nextItem->isDABtoFM() << ", isNoSignal = " << nextItem->isNoSignal();
            if((pCurrentItem->subChId() == nextItem->subChId()) && (pCurrentItem->serviceID() == nextItem->serviceID()) && (pCurrentItem->frequency() == nextItem->frequency()))
            {
                nextItem->setDABtoFM(pCurrentItem->isDABtoFM());
                nextItem->setNoSignal(pCurrentItem->isNoSignal());
                nextItem->setDABtoDAB(pCurrentItem->isDABtoDAB());
                bPlayIcon = true;
            }
            else
            {
                nextItem->setDABtoFM(false);
                nextItem->setNoSignal(false);
                nextItem->setDABtoDAB(false);
                bPlayIcon = false;
            }
            emit sendServiceNameForSortingBy(nCnt, nextItem->frequency(), nextItem->ensembleLabel() ,nextItem->label(), nextItem->programeType(), nextItem->ps(), nextItem->isDABtoDAB(), nextItem->isDABtoFM(), nextItem->isNoSignal(), bPlayIcon, nCnt);
            pStationListPTY->appendRow(new DABChannelItem(nextItem));
        }
    }
    emit playIndexUpdate(true);
    return 0;
}

DABChannelItem*
DABChannelManager::findTAChannelInChannelList(void)
{
    DABChannelItem* pNextItem = NULL;
    DABChannelItem* pItem = this->getCurrentChannelItem();
    if(pItem == NULL) return NULL;

    ListModel* pList = this->getCurrentChannelList();
    if(pList == NULL)
    {
        qDebug() << __FUNCTION__ << ": CurrentChannel List is NULL";
        return NULL;
    }

    int currentIndex = pList->findIndex(pItem->key(), pItem->subKey(), pItem->id());
    bool result = false;
    if( currentIndex == -1)
    {
        return NULL;
    }
    else
    {
        int findIndex = currentIndex + 1;
        int count = pList->rowCount();
        if( findIndex >= count)
        {
            findIndex = 0;
        }

        qDebug() << __FUNCTION__ << " index = " << currentIndex << "  findIndex = " << findIndex;

        while(currentIndex != findIndex)
        {
            pNextItem = (DABChannelItem*)pList->dataItem(findIndex);

            if((pNextItem->asuInfo().ASUFlags & 0x0002) > 0)
            {
                qDebug() << __FUNCTION__ << " ==================== Find !!";
                result = true;
                break;
            }
            findIndex++;
            if( findIndex >= count)
            {
                findIndex = 0;
            }
        }
    }

    if(result)
        return pNextItem;
    else
        return NULL;
}

int
DABChannelManager::loadCurrentFreguencyChannel(quint32 frequency, bool isSelect)
{
    DABChannelItem* pItem = NULL;
    ListModel* pListModel = m_DABUIListener.getStationList();
    DABChannelItem* pCurrentItem = this->getCurrentChannelItem();
    int count = pListModel->rowCount();
    bool bPlayIcon = false;
    int addedListCount = 0;

    for(int i = 0; i < count; i++)
    {
        pItem = (DABChannelItem*) pListModel->dataItem(i);
        qWarning() << __FUNCTION__ << " frequency:" << frequency << " pItem->frequency : " << pItem->frequency() << " current item->frequency  = " << pCurrentItem->frequency();
        if( frequency == pItem->frequency() && pItem->frequency() != 0)
        {
            qWarning() << __FUNCTION__ << "====current item  freq = " << pCurrentItem->frequency() << ", subChid =  " << pCurrentItem->subChId() << ", serviceID = " << pCurrentItem->serviceID();
            qWarning() << __FUNCTION__ << "====next    item  freq = " << pItem->frequency() << ", subChid =  " << pItem->subChId() << ", serviceID = " << pItem->serviceID();
            qWarning() << __FUNCTION__ << " ====current item isDABtoDAB = " << pCurrentItem->isDABtoDAB() << ", isDABtoFM = " << pCurrentItem->isDABtoFM() << ", isNoSignal = " << pCurrentItem->isNoSignal();
            qWarning() << __FUNCTION__ << " ====next    item isDABtoDAB = " << pItem->isDABtoDAB() << ", isDABtoFM = " << pItem->isDABtoFM() << ", isNoSignal = " << pItem->isNoSignal();
            if((pCurrentItem->subChId() == pItem->subChId()) && (pCurrentItem->serviceID() == pItem->serviceID()) && (pCurrentItem->frequency() == pItem->frequency()))
            {
                bPlayIcon = true;
                pItem->setDABtoFM(pCurrentItem->isDABtoFM());
                pItem->setNoSignal(pCurrentItem->isNoSignal());
                pItem->setDABtoDAB(pCurrentItem->isDABtoDAB());
            }
            else
            {
                pItem->setDABtoFM(false);
                pItem->setNoSignal(false);
                pItem->setDABtoDAB(false);
                bPlayIcon = false;
            }
            emit sendServiceName( i, pItem->frequency(), pItem->ensembleLabel() ,pItem->label(), pItem->programeType(), pItem->ps(), pItem->isDABtoDAB(), pItem->isDABtoFM(), pItem->isNoSignal(), bPlayIcon);
            addedListCount++;
        }
    }

    qDebug() << __FUNCTION__ << " :: isSelect = " << isSelect << ", EnsembleSeek = " << m_DABUIListener.getEnsembleSeek();
    if(!isSelect && !m_DABUIListener.getEnsembleSeek())
        emit playIndexUpdate(isSelect);

    return addedListCount;
}

int
DABChannelManager::listSelectBySeekDown(quint32 frequency)
{
    qDebug() << __FUNCTION__ << " frequency:" << frequency;
    DABChannelItem* pChannelItem = NULL;
    ListModel* pListModel = m_DABUIListener.getStationList();
    int count = pListModel->rowCount();
    bool bPlayIcon = false;
    for(int i = 0; i < count; i++)
    {
        pChannelItem = (DABChannelItem*) pListModel->dataItem(i);
        if( frequency == pChannelItem->frequency())
        {
            if(i == 0)
            {
                m_DABUIListener.setCurrentChannelInfoFromItem(pChannelItem);
                emit cmdReqSetSelectService();
                bPlayIcon = true;
            }
            else
            {
                bPlayIcon = false;
            }
            emit sendServiceName(i, pChannelItem->frequency(), pChannelItem->ensembleLabel(), pChannelItem->label(), pChannelItem->programeType(), pChannelItem->ps(), pChannelItem->isDABtoDAB(), pChannelItem->isDABtoFM(), pChannelItem->isNoSignal(), bPlayIcon);
        }
    }

    emit playIndexUpdate(true);
    return 0;
}

int
DABChannelManager::loadListByLongSeekUp(quint32 frequency)
{
    qDebug() << __FUNCTION__ << " frequency:" << frequency;
    DABChannelItem* pChannelItem = NULL;
    ListModel* pListModel = m_DABUIListener.getStationList();
    int count = pListModel->rowCount();

    for(int i = 0; i < count; i++)
    {
        pChannelItem = (DABChannelItem*) pListModel->dataItem(i);
        if( frequency == pChannelItem->frequency())
        {
            emit sendServiceName(i, pChannelItem->frequency(), pChannelItem->ensembleLabel() ,pChannelItem->label(), pChannelItem->programeType(), pChannelItem->ps(), pChannelItem->isDABtoDAB(), pChannelItem->isDABtoFM(), pChannelItem->isNoSignal(), false);
        }
    }

    return 0;
}

int
DABChannelManager::loadListByLongSeekDown(quint32 frequency)
{
    qDebug() << __FUNCTION__ << " frequency:" << frequency;
    DABChannelItem* pChannelItem = NULL;
    ListModel* pListModel = m_DABUIListener.getStationList();
    int count = pListModel->rowCount();

    for(int i = 0; i < count; i++)
    {
        pChannelItem = (DABChannelItem*) pListModel->dataItem(i);
        qDebug() << __FUNCTION__ << "index : " << i << ", frequency : " << frequency << "  pChannelItem->frequency : " << pChannelItem->frequency() << "  serviceCount = " << pChannelItem->serviceCount();
        if( frequency == pChannelItem->frequency())
        {
            emit sendServiceName(i, pChannelItem->frequency(), pChannelItem->ensembleLabel() ,pChannelItem->label(), pChannelItem->programeType(), pChannelItem->ps(), pChannelItem->isDABtoDAB(), pChannelItem->isDABtoFM(), pChannelItem->isNoSignal(), false);
        }
    }
    return 0;
}

void
DABChannelManager::deleteChannelList(void)
{
    qDebug() << Q_FUNC_INFO;

    QFile file(DAB_MODE1_DATA_FILE);
    if(file.remove())
    {
        DABLogger::instance()->Log((QString(" deleteChannelList() : channel list file remove success !!")));
    }
    else
    {
        DABLogger::instance()->Log((QString(" deleteChannelList() : channel list file remove Fail !!")));
    }
    file.close();
}

int
DABChannelManager::getCurrentChannelIndexFromChannelList(void)
{
    qDebug() << Q_FUNC_INFO;
    ListModel* currentList = this->getCurrentChannelList();
    DABChannelItem* pItem = this->getCurrentChannelItem();

    int index = currentList->findIndex(pItem->key(), pItem->subKey(), pItem->id());
    if(index == -1)
    {
        qDebug() <<"current item is not exist in channel list";
        return index;
    }
    else
    {
        return index;
    }
    return -1;
}

int
DABChannelManager::searchPresetChannelFromChannelList(void)
{
    qDebug() << Q_FUNC_INFO;
    ListModel* currentList = this->getCurrentChannelList();
    DABChannelItem* pItem = this->getCurrentChannelItem();

    int index = currentList->findIndex(pItem->key(), pItem->subKey(), pItem->id());
    if(index == -1)
    {
        qDebug() << __FUNCTION__ << "current item is not exist in channel list";
        if(currentList->findIndexOnlyPreset(pItem->key(), pItem->ensembleLabel()) == 0)
        {
            qDebug() << __FUNCTION__ << "Ensemble Label is different. ";
            return 0;
        }
        else
        {
            return index;
        }
    }
    else
    {
        return index;
    }
}

void
DABChannelManager::onRemoveUnlockFrequency(quint32 freq)
{
    DABChannelItem* pNextItem = NULL;
    int serviceCount = 0;
    int removeStart = 0;
    int removeIndex = 0;
    int channelCount = 0;
    bool existPlayService = false;

    ListModel* currentList = this->getCurrentChannelList();
    DABChannelItem* pCurrentItem = this->getCurrentChannelItem();

    channelCount = currentList->rowCount();

    // 1. compared current channel list to update channel list. so checked same frequency count
//    qDebug() << __FUNCTION__ << "=======================START===================================================================";
    for(int i = 0; i<channelCount; i++)
    {
        pNextItem = (DABChannelItem*)currentList->dataItem(i);
        if(pNextItem->frequency() == freq)
        {
            serviceCount++;
            removeIndex = i;
//            qDebug() << __FUNCTION__ << "Current Channel Item :: freq = " << pCurrentItem->frequency() << ", subChid =  " << pCurrentItem->subChId() << ", serviceID = " << pCurrentItem->serviceID();
//            qDebug() << __FUNCTION__ << "   Next Channel Item :: freq = " << pNextItem->frequency() << ", subChid =  " << pNextItem->subChId() << ", serviceID = " << pNextItem->serviceID();
            if((pNextItem->serviceID() == pCurrentItem->serviceID()) && (pNextItem->subChId() == pCurrentItem->subChId()) && (pNextItem->frequency() == pCurrentItem->frequency()))
            {
                qDebug() << __FUNCTION__ << " current channel is exist !!";
                existPlayService = true;
            }
        }
    }

    if(serviceCount == 0)
    {
//        qDebug() << __FUNCTION__ << "same frequency is not exist!! so return !! frequency = " << freq;
//        qDebug() << __FUNCTION__ << "=======================END===================================================================";
        return;
    }

    if(channelCount == 1 && existPlayService == true)
    {
//        qDebug() << __FUNCTION__ << " ================================if channel count is 1, Current Channel is not remove !!!!";
        existPlayService = false;
//        qDebug() << __FUNCTION__ << "=======================END===================================================================";
        return;
    }

    // 2. cacluate delete index
    removeStart = removeIndex - serviceCount + 1;

    // 3. same frequency remove from current channel list
    for(int i = 0; i<serviceCount; i++)
    {
        if( !currentList->removeRow(removeStart) )
        {
//            qDebug() << __FUNCTION__ << " removeRow returns false.";
//            qDebug() << __FUNCTION__ << "=======================END===================================================================";
            return;
        }
    }   

    if(existPlayService)
    {
//        qDebug() << __FUNCTION__ << " ================================Current Channel Service is not remove . removeStart = " << removeStart;
        currentList->insertRow(removeStart, new DABChannelItem(pCurrentItem));
        int count = channelCount - serviceCount + 1;
        setChannelCount(count);
    }
    else
    {
        // 4. update channel count
        int count = channelCount - serviceCount;
        setChannelCount(count);
    }
//    qDebug() << __FUNCTION__ << "=======================END===================================================================";
}

void
DABChannelManager::onUpdateChannelListFromAutoScan(void)
{
    int channelCount = this->getChannelCount();
    int tempChannelCount = this->getTempChannelCount();

    if(tempChannelCount == 0)
    {
        qDebug() << __FUNCTION__ << " :: tempChannel count is zero. so return!!";
        return;
    }

    if(channelCount == 0)
    {
        qDebug() << " Channel list Count is Zero!!";
        DABChannelItem* pItem = NULL;
        ListModel* currentList = getCurrentChannelList();
        ListModel* tempChannelList = getTempChannelList();
        int count = 0;
        for(int i=0; i<tempChannelCount; i++)
        {
            pItem = (DABChannelItem*) tempChannelList->dataItem(i);
            currentList->appendRow(new DABChannelItem(pItem));
            count++;
        }

        setChannelCount(count);
        tempChannelList->removeAllRows();
        setTempChannelCount(0);

        if(count == 0)
        {
//            qDebug() << __FUNCTION__ << " :: channel and tempChannel count is zero. so not make preset list";
//            qDebug() << __FUNCTION__ << "=======================END===================================================================";
            return;
        }

        int presetCount = this->getCurrentPresetList()->rowCount();
        if(presetCount == 0)
        {
            makePresetListFromChannelList();
            m_DABUIListener.setCurrentChannelInfoFromPresetList(0);
            emit cmdReqSetSelectService();
        }
        m_DABUIListener.saveChannelInfo();
    }
    else
    {
        DABChannelItem* pTempItem = NULL;
        DABChannelItem* pNextItem = NULL;
        ListModel* tempChannelList = this->getTempChannelList();
        ListModel* currentList = this->getCurrentChannelList();
        int serviceCount = 0;
        int insertIndex = 0;
        int removeIndex = 0;

        pTempItem = (DABChannelItem*)tempChannelList->dataItem(0);

        // 1. compared current channel list to update channel list. so checked same frequency count
        for(int i = 0; i<channelCount; i++)
        {
            pNextItem = (DABChannelItem*)currentList->dataItem(i);
            if(pNextItem->frequency() == pTempItem->frequency())
            {
//                qDebug() << __FUNCTION__ << "remove : name = " << pNextItem->label() << " ensemble = " << pNextItem->ensembleLabel();
                serviceCount++;
                removeIndex = i;
            }
        }

        // 2-1. case same frequency is not exists !!
        if(serviceCount == 0)
        {
            for(int i = 0; i<channelCount; i++)
            {
                pNextItem = (DABChannelItem*)currentList->dataItem(i);
                if(pTempItem->frequency() > pNextItem->frequency())
                {
                    insertIndex = i+1;
                }
                else if(pTempItem->frequency() < pNextItem->frequency())
                {
                    if(i == 0)
                    {
                        insertIndex = i;
                        break;
                    }
                    insertIndex = i;
                    break;
                }
            }
        }
        // 2-2. same frequency is exists !!
        else
        {
            // 1). cacluate insert index
            insertIndex = removeIndex - serviceCount + 1;

            // 2). same frequency remove from current channel list
            for(int i = 0; i<serviceCount; i++)
            {
                if( !currentList->removeRow(insertIndex) )
                {
                    tempChannelList->removeAllRows();
                    setTempChannelCount(0);
                    return;
                }
            }
        }

        // 3). new frequency insert from current channel list
        int insertCount = 0;
        DABChannelItem* pCurrentItem = this->getCurrentChannelItem();
        for(int i = 0; i<tempChannelCount; i++, insertIndex++)
        {
            pTempItem = (DABChannelItem*)tempChannelList->dataItem(i);
            if((pTempItem->serviceID() == pCurrentItem->serviceID()) && (pTempItem->subChId() == pCurrentItem->subChId()) && (pTempItem->frequency() == pCurrentItem->frequency()))
            {
                qDebug() << __FUNCTION__ << " ================ current channel insert !!!" << pCurrentItem->isDABtoDAB() << ", isDABtoFM = " << pCurrentItem->isDABtoFM() << ", isNoSignal = " << pCurrentItem->isNoSignal();
                pTempItem->setDABtoDAB(pCurrentItem->isDABtoDAB());
                pTempItem->setDABtoFM(pCurrentItem->isDABtoFM());
                pTempItem->setNoSignal(pCurrentItem->isNoSignal());
                pTempItem->setPresetIndex(pCurrentItem->presetIndex());
                currentList->insertRow(insertIndex, new DABChannelItem(pTempItem));
                m_DABUIListener.setCurrentChannelInfoFromItem(pTempItem);
                emit cmdInitPICodeValue();
            }
            else
            {
                currentList->insertRow(insertIndex, new DABChannelItem(pTempItem));
            }
            insertCount++;
        }

        // 5. update channel count
        int count = channelCount - serviceCount + insertCount;
        setChannelCount(count);

        // 6. update Preset List
        updatePresetListFromAutoScan();

        // 7. remove temp channel list
        tempChannelList->removeAllRows();
        setTempChannelCount(0);
    }

#ifdef __DAB_DABUG_INFO__
    int temp1, temp2;
    temp1 = m_DABUIListener.getFirstEnsembleCount();
    m_DABUIListener.setFirstEnsembleCount(tempChannelCount);
    temp2 = m_DABUIListener.getSecondEnsembleCount();
    m_DABUIListener.setSecondEnsembleCount(temp1);
    m_DABUIListener.setThirdEnsembleCount(temp2);
#endif
}

void
DABChannelManager::onUpdateChannelListFromReconfiguration(void)
{
    qDebug() << Q_FUNC_INFO;

    DABChannelItem* pTempItem = NULL;
    DABChannelItem* pNextItem = NULL;
    ListModel* newChannelList = this->getNewChannelList();
    ListModel* currentList = this->getCurrentChannelList();
    int channelCount = this->getChannelCount();
    int tempChannelCount = this->getNewChannelCount();
    int serviceCount = 0;
    int insertIndex = 0;
    int removeIndex = 0;

    pTempItem = (DABChannelItem*)newChannelList->dataItem(0);
    qDebug() << __FUNCTION__ << " : tempchannel count = " << tempChannelCount << " listchannel count = " << channelCount;

    // 1. compared current channel list to update channel list. so checked same frequency count
    for(int i = 0; i<channelCount; i++)
    {
        pNextItem = (DABChannelItem*)currentList->dataItem(i);
        if(pNextItem->frequency() == pTempItem->frequency())
        {
            qDebug() << __FUNCTION__ << "remove : name = " << pNextItem->label() << " ensemble = " << pNextItem->ensembleLabel();
            serviceCount++;
            removeIndex = i;
        }
    }

    // 2. cacluate insert index
    insertIndex = removeIndex - serviceCount + 1;
    qDebug() << __FUNCTION__ << ", service count = " << serviceCount << ": remove index = " << removeIndex << ", insertIndex = " << insertIndex;

    // 3. same frequency remove from current channel list
    for(int i = 0; i<serviceCount; i++)
    {
        if( !currentList->removeRow(insertIndex) )
        {
            qDebug() << __FUNCTION__ << " removeRow returns false.";
            return;
        }
    }

    // 4. new frequency insert from current channel list
    int insertCount = 0;
    for(int i = 0; i<tempChannelCount; i++, insertIndex++)
    {
        pTempItem = (DABChannelItem*)newChannelList->dataItem(i);
        qDebug() << __FUNCTION__ << "insert : name = " << pTempItem->label() << " ensemble = " << pTempItem->ensembleLabel();
        currentList->insertRow(insertIndex, new DABChannelItem(pTempItem));
        insertCount++;
    }
    \
    // 5. update channel count
    int count = channelCount - serviceCount + insertCount;
    setChannelCount(count);

    // 6. remove temp channel list
    newChannelList->removeAllRows();
    setNewChannelCount(0);

    // 7. update station list
    redrawStationList();

    // 8. update current channel
    int index = this->getCurrentChannelIndexFromChannelList();
    if(index < 0)
    {
        qDebug() << __FUNCTION__ << "channel index is 0 !!!!. so Return !!!";
        return;
    }
    DABChannelItem* item = (DABChannelItem*)this->getChannelItemFromChannelList(index);
    m_DABUIListener.setCurrentChannelInfoFromItem(item);
    emit cmdInitPICodeValue();
}

void
DABChannelManager::onUpdateChannelListFromReconfigurationNotChanged(void)
{
    qDebug() << Q_FUNC_INFO;

    DABChannelItem* pTempItem = NULL;
    DABChannelItem* pNextItem = NULL;
    ListModel* newChannelList = this->getNewChannelList();
    ListModel* currentList = this->getCurrentChannelList();
    int channelCount = this->getChannelCount();
    int tempChannelCount = this->getNewChannelCount();
    int serviceCount = 0;
    int insertIndex = 0;
    int removeIndex = 0;
    bool isSameSerivceInfo = false;

    pTempItem = (DABChannelItem*)newChannelList->dataItem(0);
    qDebug() << __FUNCTION__ << " : tempchannel count = " << tempChannelCount << " listchannel count = " << channelCount;

    // 1. compared current channel list to update channel list. so checked same frequency count
    for(int i = 0; i<channelCount; i++)
    {
        pNextItem = (DABChannelItem*)currentList->dataItem(i);
        if(pNextItem->frequency() == pTempItem->frequency())
        {
            qDebug() << __FUNCTION__ << "remove : name = " << pNextItem->label() << " ensemble = " << pNextItem->ensembleLabel();
            serviceCount++;
            removeIndex = i;
        }
    }
#if 1
    for(int k=0; k<tempChannelCount; k++)
    {
        pTempItem = (DABChannelItem*)newChannelList->dataItem(k);
        for(int j=0; j<channelCount; j++)
        {
            pNextItem = (DABChannelItem*)currentList->dataItem(j);
            qDebug() << __FUNCTION__ << "Temp Channel[" << k << "] EnsembleLabel = " << pTempItem->ensembleLabel() << ", ServiceLabel = " << pTempItem->label();
            qDebug() << __FUNCTION__ << "List Channel[" << j << "] EnsembleLabel = " << pNextItem->ensembleLabel() << ", ServiceLabel = " << pNextItem->label();
            if(pTempItem->frequency() == pNextItem->frequency())
            {
                if((pTempItem->ensembleLabel() != pNextItem->ensembleLabel()) || (pTempItem->label() != pNextItem->label()))
                {
                    isSameSerivceInfo = true;
                    break;
                }
            }
        }
    }
#endif
    // 2. cacluate insert index
    insertIndex = removeIndex - serviceCount + 1;
    qDebug() << __FUNCTION__ << ", service count = " << serviceCount << ": remove index = " << removeIndex << ", insertIndex = " << insertIndex;

    // 3. same frequency remove from current channel list
    for(int i = 0; i<serviceCount; i++)
    {
        if( !currentList->removeRow(insertIndex) )
        {
            qDebug() << __FUNCTION__ << " removeRow returns false.";
            return;
        }
    }

    // 4. new frequency insert from current channel list
    int insertCount = 0;
    for(int i = 0; i<tempChannelCount; i++, insertIndex++)
    {
        pTempItem = (DABChannelItem*)newChannelList->dataItem(i);
        qDebug() << __FUNCTION__ << "insert : name = " << pTempItem->label() << " ensemble = " << pTempItem->ensembleLabel();
        currentList->insertRow(insertIndex, new DABChannelItem(pTempItem));
        insertCount++;
    }
    \
    // 5. update channel count
    int count = channelCount - serviceCount + insertCount;
    setChannelCount(count);

    // 6. remove temp channel list
    newChannelList->removeAllRows();
    setNewChannelCount(0);

    // 7. update station list
#if 1
    if(tempChannelCount == serviceCount && isSameSerivceInfo)
    {
        qDebug() << __FUNCTION__ << " ===================== currnet channel list is same temp channel list. so not upate station";
    }
    else
    {
        redrawStationList();
    }
#else
    redrawStationList();
#endif
    // 8. update current channel
    int index = this->getCurrentChannelIndexFromChannelList();
    if(index < 0)
    {
        qDebug() << __FUNCTION__ << "channel index is 0 !!!!. so Return !!!";
        return;
    }
    DABChannelItem* item = (DABChannelItem*)this->getChannelItemFromChannelList(index);
    m_DABUIListener.setCurrentChannelInfoFromItem(item);
    emit cmdInitPICodeValue();
}

void
DABChannelManager::onUpdateChannelItem(quint32 freq, quint16 eId, quint8 iId, QString eLabel, quint8 sCount, quint32 sID, quint8 sType, quint8 subChId, quint16 bitrate, quint8 ps, quint8 charset, quint8 pty, quint8 language, QString sLabel, stASU_INFO asuInfo, stPICODE_INFO picodeInfo, quint8 subCHSize, quint16 address)
{
    qDebug() <<Q_FUNC_INFO;
    DABChannelItem* pNextItem = NULL;
    ListModel* currentList = this->getCurrentChannelList();

    if(currentList == NULL){
        qDebug() << __FUNCTION__ << ": CurrentChannel List is NULL";
        return;
    }

    DABChannelItem *pItem = new DABChannelItem(currentList);

    if(pItem == NULL){
        qDebug() << __FUNCTION__ << ": Channel Item is NULL";
        return;
    }

    pItem->setFrequency(freq);
    pItem->setEnsembleID(eId);
    pItem->setInternationalTableID(iId);
    pItem->setEnsembleLabel(eLabel);
    pItem->setServiceCount(sCount);
    pItem->setServiceID(sID);
    pItem->setServiceType(sType);
    pItem->setSubChId(subChId);
    pItem->setBitrate(bitrate);
    pItem->setPS(ps);
    pItem->setCharset(charset);
    pItem->setProgrameType(pty);
    pItem->setLanguage(language);
    pItem->setLabel(sLabel);
    pItem->setPresetIndex(this->isExistPresetList(pItem));
    pItem->setASUInfo(&asuInfo);
    pItem->setPICodeInfo(&picodeInfo);
    pItem->setSubCHSize(subCHSize);
    pItem->setAddress(address);
    if(isAlreadyExistCurrentChannel(pItem))
    {        
        int index = currentList->findIndex(pItem->key(), pItem->subKey(),pItem->id());
        qDebug() << " this Item is exist already in channel list.find index = " << index;
        if( !currentList->removeRow(index) )
        {
            qDebug() << " removeRow returns false.";
            return;
        }

        qDebug() << "New Item Inserted !!";
        currentList->insertRow(index, new DABChannelItem(pItem));
    }
    else
    {
        qDebug() << " this Item is not exist in channel list.";
        int count = currentList->rowCount();
        int insertIndex = 0 ;
        bool isSameFrequency = false;

//        for(int i = 0; i<count; i++)
//        {
//            pNextItem = (DABChannelItem*)pChannelList->dataItem(i);
////            qDebug() << "[ Current Channel List START ]";
////            qDebug() <<"Index : " << i <<" pNextItem : serviceID : " << pNextItem->id() << "ensembleID :" << pNextItem->ensembleLabel()<<"Label  : " <<pNextItem->label();
//        }
//        qDebug() << "[ Current Channel List END ]";

//        qDebug() << "pItem->label : " << pItem->label() << "  pItem->frequency : " << pItem->frequency();
        for(int i = 0; i<count; i++)
        {
            pNextItem = (DABChannelItem*)currentList->dataItem(i);
            if(pItem->frequency() == pNextItem->frequency())
            {
//                qDebug() << "Frequency Same !!";
//                qDebug() <<"index : " << i<< " pItem->label : " << pItem->label() << " vs  pNextItem->label : " << pNextItem->label();
                if(QString::compare(pItem->label(), pNextItem->label(), Qt::CaseInsensitive)> 0)
                {
                    insertIndex = i+1;
//                    qDebug() << "pItem=>Label is big than pNextItem: insertIndex = " << insertIndex;
                    isSameFrequency = true;
                }

                if(QString::compare(pItem->label(), pNextItem->label(), Qt::CaseInsensitive)< 0)
                {
//                    qDebug() << " index = " << i << " insertindex = " << insertIndex;
                    insertIndex = i;
//                    qDebug() << "pNextItem=>Label is big than pItem : insertIndex = " << insertIndex;
                    break;
                }
            }
            else if(pItem->frequency() > pNextItem->frequency() && !isSameFrequency)
            {
//                qDebug() << "case : pItem->frequncty > pNextItem->frequency";
//                qDebug() <<"index : " << i << "pItem->label : " << pItem->label() << "  pNextItem->label : " << pNextItem->label();
                insertIndex = i+1;
//                qDebug() << "Current InsertIndex : " << insertIndex;
            }
            else if(pItem->frequency() < pNextItem->frequency() && !isSameFrequency)
            {
                if(i == 0)
                {
//                    qDebug() << "case : pItem->frequncty < pNextItem->frequency";
//                    qDebug() <<"index : " << i<< "pItem->label : " << pItem->label() << "  pNextItem->label : " << pNextItem->label();
//                    qDebug() <<"insertIndex is 0. break;";
                    insertIndex = 0;
                    break;
                }
//                qDebug() << "case : pItem->frequncty < pNextItem->frequency";
//                qDebug() <<"index : " << i<< "pItem->label : " << pItem->label() << "  pNextItem->label : " << pNextItem->label();
//                qDebug() << "Current InsertIndex : " << insertIndex;
                insertIndex = i;
                break;
            }
        }

        qDebug() << "Current Insert Row : inserIndex = " << insertIndex;
//        if(insertIndex < 0) insertIndex = 0;
        currentList->insertRow(insertIndex, new DABChannelItem(pItem));

        this->m_iChannelCount++;
        this->setChannelCount(this->m_iChannelCount);

        // update current channel
        int index = this->getCurrentChannelIndexFromChannelList();
        if(index < 0)
        {
            qDebug() << __FUNCTION__ << "channel index is 0 !!!!. so Return !!!";
            return;
        }
        DABChannelItem* item = (DABChannelItem*)this->getChannelItemFromChannelList(index);
        m_DABUIListener.setCurrentChannelInfoFromItem(item);
    }
}

DABChannelItem*
DABChannelManager::getChannelItemFromChannelList(int index)
{
    int count = this->getCurrentChannelList()->rowCount();
    //qDebug() << __FUNCTION__ << " channel count = " << count << ", search index = " << index;
    if(count == 0)
    {
        qDebug() << __FUNCTION__ << " Channel List is 0. Empty..";
        return NULL;
    }
    if(index < 0 || index >= count)
    {
        qWarning() << __FUNCTION__ << " index is out of Bound. index:"<<index<<", list Count:"<< count;
        return NULL;
    }    
    return (DABChannelItem*)this->getCurrentChannelList()->dataItem(index);
}

bool
DABChannelManager::updateChannelItemStatus(quint8 type, bool onOff)
{
    int index = this->getCurrentChannelIndexFromChannelList();
    if(index < 0)
    {
        qDebug() << __FUNCTION__ << "channel index is 0 !!!!. so Return !!!";
        return false;
    }

    DABChannelItem* pItem = (DABChannelItem*)this->getChannelItemFromChannelList(index);
    DABChannelItem* pCurrentItem = this->getCurrentChannelItem();

    qDebug() << __FUNCTION__ << " :: isNoSignal = " << pCurrentItem->isNoSignal() << ", isDABtoDAB = " << pCurrentItem->isDABtoDAB() << ", isDABtoFM = " << pCurrentItem->isDABtoFM() << ", type = " << type << ", onOff = " << onOff;
    pItem->setDABtoFM(false);
    pItem->setNoSignal(false);
    pItem->setDABtoDAB(false);

    switch(type)
    {
        case DAB_CHANNEL_DAB_TO_DAB:
            pItem->setDABtoDAB(onOff);
            break;

        case DAB_CHANNEL_DAB_TO_FM:
            pItem->setDABtoFM(onOff);
            break;

        case DAB_CHANNEL_NO_SIGNAL:
            pItem->setNoSignal(onOff);
            break;

        case DAB_CHANNEL_ICON_CLEAR:
            break;

        default:
            break;
    }

    pCurrentItem->setDABtoDAB(pItem->isDABtoDAB());
    pCurrentItem->setDABtoFM(pItem->isDABtoFM());
    pCurrentItem->setNoSignal(pItem->isNoSignal());
    return true;
}

void
DABChannelManager::addCurrentItemInChannelList(DABChannelItem* pItem)
{
    qDebug() << Q_FUNC_INFO;

    DABChannelItem* pNextItem = NULL;
    ListModel* currentList = this->getCurrentChannelList();

    if(currentList == NULL){
        qDebug() << __FUNCTION__ << ": CurrentChannel List is NULL";
        return;
    }

    int count = currentList->rowCount();
    int insertIndex = 0 ;
    bool isSameFrequency = false;

    for(int i = 0; i<count; i++)
    {
        pNextItem = (DABChannelItem*)currentList->dataItem(i);
        if(pItem->frequency() == pNextItem->frequency())
        {
            if(QString::compare(pItem->label(), pNextItem->label(), Qt::CaseInsensitive)> 0)
            {
                insertIndex = i+1;
                isSameFrequency = true;
            }

            if(QString::compare(pItem->label(), pNextItem->label(), Qt::CaseInsensitive)< 0)
            {
                insertIndex = i;
                break;
            }
        }
        else if(pItem->frequency() > pNextItem->frequency() && !isSameFrequency)
        {
            insertIndex = i+1;
        }
        else if(pItem->frequency() < pNextItem->frequency() && !isSameFrequency)
        {
            if(i == 0)
            {
                insertIndex = 0;
                break;
            }
            insertIndex = i;
            break;
        }
    }

    currentList->insertRow(insertIndex, new DABChannelItem(pItem));

    this->m_iChannelCount++;
    this->setChannelCount(this->m_iChannelCount);
}

void
DABChannelManager::redrawStationList(void)
{
    emit updateStationList();
}

int
DABChannelManager::findPlayIndexFromStationList(QString sortingBy)
{
    qDebug() << Q_FUNC_INFO << " :: sortingBy = " << sortingBy;
    ListModel* pStationList = NULL;
    int playIndex = 0;
    if(sortingBy == "alphabet")
    {
        pStationList = m_DABUIListener.getStationListAlphabet();
    }
    else if(sortingBy == "pty")
    {
        pStationList = m_DABUIListener.getStationListPTY();
    }
    else
    {
        pStationList = m_DABUIListener.getStationList();
    }
    DABChannelItem* pItem = this->getCurrentChannelItem();
    DABChannelItem* pNextItem = NULL;
    for(int i = 0; i < pStationList->rowCount(); i++)
    {
        pNextItem = (DABChannelItem*) pStationList->dataItem(i);

        if((pItem->frequency() == pNextItem->frequency()) && (pItem->serviceID() == pNextItem->serviceID()) && (pItem->subChId() == pNextItem->subChId()))
        {
            playIndex = i;
            break;
        }
    }
    return playIndex;
}

/*
===========================================================================================
+++++++++++++++++++++++++++ Preset List Manager +++++++++++++++++++++++++++++++++++++++++++
===========================================================================================
*/
void
DABChannelManager::makePresetListFromChannelList(void)
{
    qDebug() << Q_FUNC_INFO;
    DABChannelItem* pItem = NULL;
    int listCount = this->getCurrentChannelList()->rowCount();
    qDebug() << __FUNCTION__ <<" Channel List Count: " << listCount;

    for(int k = 0; k < listCount && k < MAX_PRESET_COUNT; k++)
    {
        pItem = (DABChannelItem*)this->getCurrentChannelList()->dataItem(k);
        pItem->setPresetIndex(k);
        this->getCurrentPresetList()->appendRow(new DABChannelItem(pItem));
    }

    if(listCount == 0)
    {
        qDebug() << " preset List count is 0. so return";
        return;
    }

    emit cmdWritePresetList();
}

bool
DABChannelManager::isAlreadyExistCurrentChannel(DABChannelItem* pItem)
{
    ListModel* pChannelList = this->getCurrentChannelList();
    if(pChannelList == NULL) return false;

    if( pChannelList->find(pItem->id())){
        qDebug() << " this Item is already in channel list.";
        return true;
    }
    return false;
}

bool
DABChannelManager::isAlreadyExistCurrentChannelInPresetList(void)
{
    DABChannelItem* pItem = this->getCurrentChannelItem();
    if(pItem == NULL) return false;
    ListModel* pPresetList = this->getCurrentPresetList();
    if(pPresetList == NULL) return false;

    if( pPresetList->find(pItem->id()) ){
        return true;
    }
    return false;
}

int
DABChannelManager::isExistPresetList(DABChannelItem *pItem)
{
    int returnValue = -1;

    if(pItem == NULL)
    {
        qDebug() << __FUNCTION__ << " DABChannelItem is Null !!";
        return returnValue;
    }

    ListModel* pPresetList = this->getCurrentPresetList();
    if(pPresetList == NULL)
    {
        qDebug() << __FUNCTION__ << " : PresetList is Null !!";
        return returnValue;
    }
    int index = pPresetList->findIndex(pItem->key(), pItem->subKey(), pItem->id());
    if(index == -1)
    {
        qDebug() << __FUNCTION__ << " : This Channel is not exist in preset list";
        return index;
    }
    else
    {
        DABChannelItem* pPresetItem = (DABChannelItem*) pPresetList->dataItem(index);
        index = pPresetItem->presetIndex();
//        qDebug() << __FUNCTION__ << " :  This Channel is exist in preset list. so preset index = " << index;
        return index;
    }
}

int
DABChannelManager::findNextChannelIndexInPresetList(DABChannelItem* pItem)
{
    qDebug() << Q_FUNC_INFO;
    DABChannelItem* pNextItem = NULL;
    ListModel* pList = NULL;
    int index, tempCount, currIndex = 0;
    int findIndex = -1;

    currIndex = pItem->presetIndex();
    pList = this->getCurrentPresetList();

    if(pList == NULL){
        qDebug() << __FUNCTION__ << ": CurrentChannel List is NULL";
        return -1;
    }

    QStandardItemModel *model = new QStandardItemModel(0, 2, this);
    model->setHeaderData(0, Qt::Horizontal, "index");
    model->setHeaderData(1, Qt::Horizontal, "presetIndex");

    tempCount = pList->rowCount();

    for(int i = 0; i < tempCount; i++)
    {
        pNextItem = (DABChannelItem*)pList->dataItem(i);
        model->insertRow(0);
        model->setData(model->index(0, 0), i);
        model->setData(model->index(0, 1), pNextItem->presetIndex());
    }

    model->sort(1, Qt::AscendingOrder);

    for(int i = 0; i < tempCount; i++)
    {
        index = model->data(model->index(i, 0)).toInt();
        pNextItem = (DABChannelItem*)pList->dataItem(index);

        if(currIndex < pNextItem->presetIndex())
        {
            qDebug() << __FUNCTION__ << "     find!!! ";
            findIndex = getIndexFromPresetList(pNextItem->presetIndex());
            break;
        }
    }

    if(findIndex == -1)
    {
        index = model->data(model->index(0, 0)).toInt();
        pNextItem = (DABChannelItem*)pList->dataItem(index);
        findIndex = getIndexFromPresetList(pNextItem->presetIndex());
    }

    return findIndex;
}

#if 1
int
DABChannelManager::findPrevChannelIndexInPresetList(DABChannelItem* pItem)
{
    DABChannelItem* pNextItem = NULL;
    ListModel* pList = NULL;
    int index, listCount, currIndex = 0;
    int findIndex = MAX_PRESET_COUNT;
    if(pItem == NULL) return -1;

    pList = this->getCurrentPresetList();
    if(pList == NULL){
    qDebug() << __FUNCTION__ << ": CurrentChannel List is NULL";
    return -1;
    }

    currIndex = pItem->presetIndex();

    QStandardItemModel *model = new QStandardItemModel(0, 2, this);
    model->setHeaderData(0, Qt::Horizontal, "index");
    model->setHeaderData(1, Qt::Horizontal, "presetIndex");

    listCount = pList->rowCount();

    for(int i = 0; i < listCount; i++)
    {
        pNextItem = (DABChannelItem*)pList->dataItem(i);
        model->insertRow(0);
        model->setData(model->index(0, 0), i);
        model->setData(model->index(0, 1), pNextItem->presetIndex());
    }

    model->sort(1, Qt::AscendingOrder);

    for(int i = listCount-1; i >= 0; i--)
    {
        index = model->data(model->index(i, 0)).toInt();
        pNextItem = (DABChannelItem*)pList->dataItem(index);

        if(currIndex > pNextItem->presetIndex())
        {
            qDebug() << __FUNCTION__ << " find index  = " << i;
            findIndex = getIndexFromPresetList(pNextItem->presetIndex());
            break;
        }
    }

    if(findIndex == MAX_PRESET_COUNT)
    {
        index = model->data(model->index(listCount-1, 0)).toInt();
        pNextItem = (DABChannelItem*)pList->dataItem(index);
        findIndex = getIndexFromPresetList(pNextItem->presetIndex());
    }

    return findIndex;
}


#else
int
DABChannelManager::findPrevChannelIndexInPresetList(DABChannelItem* pItem)
{
    DABChannelItem* pNextItem = NULL;
    if(pItem == NULL) return -1;

    ListModel* pList = this->getCurrentPresetList();
    if(pList == NULL){
        qDebug() << __FUNCTION__ << ": CurrentChannel List is NULL";
        return -1;
    }

    int index = pList->findIndex(pItem->key(), pItem->subKey(),pItem->id());
    qDebug() << __FUNCTION__ << " Current Index : " << index;
    if( index == -1)
    {
        int count = pList->rowCount();
        for(int i = count-1; i >= 0; i--)
        {
            pNextItem = (DABChannelItem*)pList->dataItem(i);
            qDebug() << "    pItem : " << pItem->id() << ", pNextItem : " << pNextItem->id();
            if(pItem->id() > pNextItem->id())
            {
                qDebug() << "     find!!! ";
                index = i;
                break;
            }
        }
    }else{
        // check previous index item
        index--;
        qDebug() << "  Prev Item is: " << index;
        int count = pList->rowCount();
        if( index < 0)
        {
            qDebug() << "  fisrt item so move to Last item";
            index = count-1;
        }
    }
    return index;
}
#endif

DABChannelItem*
DABChannelManager::getChannelItemFromPresetList(int index)
{
    qDebug() << Q_FUNC_INFO << " :: index = " << index;
    int listCount = this->getCurrentPresetList()->rowCount();

    if(index < 0 || index >= listCount)
    {
        qDebug() << __FUNCTION__ << " index is out of Bound. index:" << index;
        return NULL;
    }

    return (DABChannelItem*)this->getCurrentPresetList()->dataItem(index);
}

int
DABChannelManager::getPresetListItem(void)
{
    DABChannelItem* pItem = NULL;
    ListModel* pListModel = this->getCurrentPresetList();
    int listCount = pListModel->rowCount();

    if(listCount > MAX_PRESET_COUNT)
        listCount = MAX_PRESET_COUNT;

    if(m_DABUIListener.getViewMode())
    {
        m_DABUIListener.modeChange(false);
    }

    for(int i = 0; i < listCount; i++)
    {
        pItem = (DABChannelItem*) pListModel->dataItem(i);
        emit serviceNameForPresetList(pItem->label(), pItem->ensembleLabel(), pItem->presetIndex(), i);
    }

    qDebug() << __FUNCTION__ << " m_DABUIListener.getPresetIndex() = " << m_DABUIListener.getPresetIndex();
    emit playIconUpdateInPresetList( (m_DABUIListener.getPresetIndex() -1) );
    return listCount;
}

int
DABChannelManager::getCurrentChannelIndexFromPresetList(void)
{
    DABChannelItem* pItem = this->getCurrentChannelItem();
    int returnValue = 0;
    int currIndex = pItem->presetIndex();
    qDebug() << __FUNCTION__ << " current channel item preset index = " << currIndex;

    DABChannelItem* pNextItem = NULL;
    ListModel* pListModel = this->getCurrentPresetList();
    int listCount = pListModel->rowCount();

    for(int i = 0; i < listCount && i < MAX_PRESET_COUNT; i++)
    {
        pNextItem = (DABChannelItem*) pListModel->dataItem(i);
        if(currIndex == pNextItem->presetIndex())
        {
            returnValue = i;
        }
    }

    return returnValue;
}

int
DABChannelManager::getIndexFromPresetList(int index)
{
    int findIndex = 0;
    qDebug() << __FUNCTION__ << " current channel item preset index = " << index;

    DABChannelItem* pNextItem = NULL;
    ListModel* pListModel = this->getCurrentPresetList();
    int listCount = pListModel->rowCount();

    for(int i = 0; i < listCount && i < MAX_PRESET_COUNT; i++)
    {
        pNextItem = (DABChannelItem*) pListModel->dataItem(i);
        if(index == pNextItem->presetIndex())
        {
            findIndex = i;
        }
    }

    return findIndex;
}


void
DABChannelManager::updatePresetListFromAutoScan(void)
{
    ListModel* tempChannelList = this->getTempChannelList();
    ListModel* presetList = this->getCurrentPresetList();
    DABChannelItem* pTempItem = NULL;
    DABChannelItem* pPresetItem = NULL;
    int tempChannelCount = tempChannelList->rowCount();
    for(int i=0; i<tempChannelCount; i++)
    {
        pTempItem = (DABChannelItem*)tempChannelList->dataItem(i);

        pPresetItem = (DABChannelItem*)presetList->findItem(pTempItem->key(), pTempItem->subKey(), pTempItem->id());

        if(pPresetItem == NULL)
        {
            continue;
        }
        else
        {
            pPresetItem->setSupplementary(pTempItem->asuInfo(), pTempItem->picodeInfo());
        }
    }
}

/*
===========================================================================================
+++++++++++++++++++++++++++ Temp Channel List Manager +++++++++++++++++++++++++++++++++++++++++++
===========================================================================================
*/

void
DABChannelManager::onAddTempChannelItem(quint32 freq, quint16 eId, quint8 iId, QString eLabel, quint8 sCount, quint32 sID, quint8 sType, quint8 subChId, quint16 bitrate, quint8 ps, quint8 charset, quint8 pty, quint8 language, QString sLabel, stASU_INFO asuInfo, stPICODE_INFO picodeInfo, quint8 subCHSize, quint16 address)
{
    ListModel* tempChannelList = this->getTempChannelList();

    DABChannelItem *pItem = new DABChannelItem(tempChannelList);
    pItem->setFrequency(freq);
    pItem->setEnsembleID(eId);
    pItem->setInternationalTableID(iId);
    pItem->setEnsembleLabel(eLabel);
    pItem->setServiceCount(sCount);
    pItem->setServiceID(sID);
    pItem->setServiceType(sType);
    pItem->setSubChId(subChId);
    pItem->setBitrate(bitrate);
    pItem->setPS(ps);
    pItem->setCharset(charset);
    pItem->setProgrameType(pty);
    pItem->setLanguage(language);
    pItem->setLabel(sLabel);
    pItem->setPresetIndex(this->isExistPresetList(pItem));
    pItem->setASUInfo(&asuInfo);
    pItem->setPICodeInfo(&picodeInfo);
    pItem->setSubCHSize(subCHSize);
    pItem->setAddress(address);
    tempChannelList->appendRow(pItem);

//    qDebug("      ASU Flags         : 0x%04X (%d)", asuInfo.ASUFlags, asuInfo.ASUFlags);
//    qDebug("      ASU ClusterCount  : 0x%02X (%d)", asuInfo.ASUClusterCount, asuInfo.ASUClusterCount);
//    for(int k=0; k < asuInfo.ASUClusterCount; k++)
//    {
//    qDebug("      ASU ClusterIDs[%d]: 0x%02X (%d)", k, asuInfo.ASUClusterIDs[k], asuInfo.ASUClusterIDs[k]);
//    }
//    qDebug("      PICode Count      : 0x%02X (%d)", picodeInfo.PICodeCount, picodeInfo.PICodeCount);
//    for(int k=0; k < picodeInfo.PICodeCount; k++)
//    {
//    qDebug("      PICode Type[%d]   : 0x%02X (%d)", k, picodeInfo.PICodeType[k], picodeInfo.PICodeType[k]);
//    qDebug("      PICode Value[%d]  : 0x%04X (%d)", k, picodeInfo.PICodeValue[k], picodeInfo.PICodeValue[k]);
//    }
//    qDebug("      PICode Validation : 0x%04X (%d)", picodeInfo.PICodeValidation, picodeInfo.PICodeValidation);


//    qDebug() << "=====================================================================================================";
//    qDebug() << "============================= pItem->picodeInfo().PICodeCount = " << pItem->picodeInfo().PICodeCount;
//    for(int i=0; i<pItem->picodeInfo().PICodeCount; i++)
//    {
//        qDebug() << "========================= picodeType = " << pItem->picodeInfo().PICodeType[i];
//        qDebug() << "========================= PICodeValue = " << pItem->picodeInfo().PICodeValue[i];
//    }
//    qDebug() << "=====================================================================================================";

    this->m_iTempChannelCount++;
}

void
DABChannelManager::onAddNewChannelItem(quint32 freq, quint16 eId, quint8 iId, QString eLabel, quint8 sCount, quint32 sID, quint8 sType, quint8 subChId, quint16 bitrate, quint8 ps, quint8 charset, quint8 pty, quint8 language, QString sLabel, stASU_INFO asuInfo, stPICODE_INFO picodeInfo, quint8 subCHSize, quint16 address)
{
    ListModel* newChannelList = this->getNewChannelList();
    DABChannelItem *pItem = new DABChannelItem(newChannelList);
    pItem->setFrequency(freq);
    pItem->setEnsembleID(eId);
    pItem->setInternationalTableID(iId);
    pItem->setEnsembleLabel(eLabel);
    pItem->setServiceCount(sCount);
    pItem->setServiceID(sID);
    pItem->setServiceType(sType);
    pItem->setSubChId(subChId);
    pItem->setBitrate(bitrate);
    pItem->setPS(ps);
    pItem->setCharset(charset);
    pItem->setProgrameType(pty);
    pItem->setLanguage(language);
    pItem->setLabel(sLabel);
    pItem->setPresetIndex(this->isExistPresetList(pItem));
    pItem->setASUInfo(&asuInfo);
    pItem->setPICodeInfo(&picodeInfo);
    pItem->setSubCHSize(subCHSize);
    pItem->setAddress(address);
    newChannelList->appendRow(pItem);

    this->m_iNewChannelCount++;
}

/*
===========================================================================================
+++++++++++++++++++++++++++ ETC +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
===========================================================================================
*/
void
DABChannelManager::loadChannelData(QString fileName)
{
    qDebug() << Q_FUNC_INFO;

    DABChannelItem* cItem = this->getCurrentChannelItem();
    ListModel* listModel = this->getCurrentChannelList();
    ListModel* presetListModel = this->getCurrentPresetList();

    DABLogger::instance()->Log((QString(" =====================Channel Info FILE LOAD START=========================")));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << Q_FUNC_INFO;
        qDebug() << " : Data File Not found:" << fileName;
        DABLogger::instance()->Log((QString("loadDataFromFile() :: = Data File Not found: = %1").arg(fileName)));
        return;
    }
    QTextStream in(&file);
    qDebug() << " Load(" << fileName << ") -------- ";
    DABLogger::instance()->Log((QString("loadDataFromFile() :: = load file  = %1").arg(fileName)));
    QString readData = in.readLine();
    int chCount = 0;
    if(readData.startsWith("Channel Num:")){
        readData.remove("Channel Num:");
        chCount = readData.toUInt();
    }else{
        qDebug() << " Warning! DataFile is broken. (No Channel Num)" << fileName;
        file.close();
        return;
    }

    DABLogger::instance()->Log((QString("loadDataFromFile() :: = channel count  = %1").arg(chCount)));
    //int chCount = in.readLine().toUInt();
    qDebug() << "  channel count: " << chCount;
    if(chCount > 0){
        qDebug() << "  Saved Channel : " << chCount;
        cItem->loadData(in);
        in.readLine(); //
        int listCount = 0; //in.readLine().toUInt();
        readData = in.readLine();
        if(readData.startsWith("All list Count:")){
            readData.remove("All list Count:");
            listCount = readData.toUInt();
        }else{
            qDebug() << " Warning! DataFile is broken. (No All list Count) " << fileName;
            file.close();
            return;
        }
        DABLogger::instance()->Log((QString("loadDataFromFile() :: = listCount = %1").arg(listCount)));
        for(int i = 0; i < listCount; i++){
            DABChannelItem* chItem = new DABChannelItem(listModel);
            chItem->loadData(in);
            if(chItem->frequency() != 0)
            {
                listModel->appendRow(chItem);
            }
            else
            {
                qDebug() << " Warning! DataFile is broken. so ignored !!! frequency : " << chItem->frequency() << ", Ensemble Label : " << chItem->ensembleLabel();
            }
        }
        in.readLine(); //
        int presetCount = 0;//in.readLine().toUInt();
        readData = in.readLine();
        if(readData.startsWith("Preset list Count:")){
            readData.remove("Preset list Count:");
            presetCount = readData.toUInt();
        }else{
            qDebug() << " Warning! DataFile is broken. (No Preset list Count) " << fileName;
            file.close();
            return;
        }

        for(int i = 0; i < presetCount; i++)
        {
            DABChannelItem* chItem = new DABChannelItem(presetListModel);
            if(chItem->loadData(in))
                presetListModel->appendRow(chItem);
            else
                break;
        }
        }else{
            qDebug() << " Saved Channel Count is 0. -------";
            DABLogger::instance()->Log((QString("loadDataFromFile() :: =Saved Channel Count is 0. -------")));
        }
    file.close();
    DABLogger::instance()->Log((QString(" =====================Channel Info FILE LOAD END=========================")));
}

void
DABChannelManager::saveChannelData(QString fileName)
{
    qDebug() << Q_FUNC_INFO;
    DABChannelItem *cItem = this->getCurrentChannelItem();
    ListModel* listModel = this->getCurrentChannelList();
    ListModel* presetListModel = this->getCurrentPresetList();

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << Q_FUNC_INFO << "File Not found: " + fileName;
        //make file
        return; //temporaty...
    }

    QTextStream out(&file);
    qDebug() << "===================================";
    qDebug() << "= SaveDataToFile : " << fileName;
    qDebug() << "===================================";

    qDebug() << "   write count: " << listModel->rowCount();
    out << "Channel Num:" << listModel->rowCount() << endl;
    qDebug() << "   write currentItem ";
    cItem->saveData(out);

    int count = listModel->rowCount();
    qDebug() << "   listModel Count: " << count;
    out << endl << "All list Count:" << count << endl;
    for(int i = 0; i < count; i++)
    {
        //DABChannelItem* item = (DABChannelItem*)listModel->takeRow(0);
        DABChannelItem* item = (DABChannelItem*)listModel->dataItem(i);
        //qDebug() << "   -save list model item : " << i;
        item->saveData(out);
    }

    count = presetListModel->rowCount();
    qDebug() << "   preset listModel Count: " << count;
    out << endl << "Preset list Count:" << count << endl;
    for(int i = 0; i < count; i++)
    {
        DABChannelItem* item = (DABChannelItem*)presetListModel->dataItem(i);
//        qDebug() << __FUNCTION__ << " presetIndex = " << item->presetIndex();
        //qDebug() << "   -save preset list model item : " << i;
        item->saveData(out);
    }
    file.close();
    DABLogger::instance()->sendProcessCommand("sync");
}

void
DABChannelManager::saveTempEpgChannelList(void)
{
    qDebug() << Q_FUNC_INFO;
    ListModel* pList = this->getCurrentChannelList();
    ListModel* pEpgTempList = this->getTempEpgChannelList();
    DABChannelItem* pItem = NULL;

    if(pEpgTempList == NULL){
        qDebug() << Q_FUNC_INFO << " Error pEpgTempList is NULL!!!!!";
        return;
    }
    if(pEpgTempList->rowCount() > 0){
        pEpgTempList->removeAllRows();
    }

    if(pList == NULL){
        qDebug() << Q_FUNC_INFO << " Error getCurrentChannelList is NULL!!!!!";
        return;
    }

    for(int i=0; i<pList->rowCount(); i++)
    {
        pItem = (DABChannelItem*)pList->dataItem(i);
        if(pItem != NULL)
            pEpgTempList->appendRow(new DABChannelItem(pItem));
    }
    qDebug() << Q_FUNC_INFO << " getTempEpgChannelList COUNT = " << pEpgTempList->rowCount();
}
