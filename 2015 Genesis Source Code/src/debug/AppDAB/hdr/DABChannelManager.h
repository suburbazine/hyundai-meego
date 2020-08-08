#ifndef DABCHANNELMANAGER_H
#define DABCHANNELMANAGER_H

#include <QObject>
#include <QApplication>
#include <QDebug>
#include <QSortFilterProxyModel>

#include "ListModel.h"
#include "DABChannelItem.h"
#include "DABLogger.h"
#include "DABUIListener.h"

#define MAX_PRESET_COUNT    12


class DABUIListener;
class DABChannelManager : public QObject
{
    Q_OBJECT
public:
    explicit DABChannelManager(DABUIListener &aDABUIListener, QObject *parent = 0);
    ~DABChannelManager();

    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ Current Channel +++++++++++++++++++++++++++++++++++++++++++++++
    ===========================================================================================
    */
    DABChannelItem* getCurrentChannelItem(void) { return m_CurrentChannel; }
    QString getCurrentChannelNumber(void) {return (m_CurrentChannel != NULL)?m_CurrentChannel->frequencyLabel():""; }
    QString getCurrentServiceName(void) { return (m_CurrentChannel != NULL )?m_CurrentChannel->label():""; }
    QString getCurrentEnsembleName(void) { return(m_CurrentChannel != NULL)?m_CurrentChannel->ensembleLabel():""; }
    quint8 getCurrentPtyName(void) { return (m_CurrentChannel != NULL)?m_CurrentChannel->programeType():0; }
    quint8 getSerivceType(void) { return (m_CurrentChannel != NULL)?m_CurrentChannel->ps():0x01; }
    int getCurrentFrequencyID(void) { return(m_CurrentChannel != NULL)?m_CurrentChannel->frequency():0; }    
    QString getProgrameTypeName(int num);
    void initPtyTable(void);

    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ Channel List Manager ++++++++++++++++++++++++++++++++++++++++++
    ===========================================================================================
    */
    Q_INVOKABLE int loadStationList(int sortType);
    Q_INVOKABLE int loadCurrentFreguencyChannel(quint32 frequency, bool isSelect);
    Q_INVOKABLE int listSelectBySeekDown(quint32 frequency);
    Q_INVOKABLE int loadListByLongSeekUp(quint32 frequency);
    Q_INVOKABLE int loadListByLongSeekDown(quint32 frequency);
    Q_INVOKABLE void deleteChannelList(void);
    Q_INVOKABLE int findPlayIndexFromStationList(QString sortingBy);
    ListModel* getCurrentChannelList(void) { return m_listChannel; }
    DABChannelItem* getChannelItemFromChannelList(int index);
    int findNextChannelIndexForChannelScan(DABChannelItem* pItem);
    int findNextChannelIndexInChannelList(DABChannelItem* pItem);    
    int findPrevChannelIndexInChannelList(DABChannelItem* pItem);
    int getCurrentChannelCount(void) { return this->m_listChannel->rowCount(); }
    int getCurrentChannelIndexFromChannelList(void);
    int searchPresetChannelFromChannelList(void);
    void setChannelCount(int count) { this->m_iChannelCount = count; }
    int getChannelCount(void) const { return this->m_iChannelCount; }
    bool isAlreadyExistCurrentChannel(DABChannelItem* pItem);
    int loadStationListBySorting(int sortingBy);
    DABChannelItem* findTAChannelInChannelList(void);
    bool updateChannelItemStatus(quint8 type, bool onOff);
    void addCurrentItemInChannelList(DABChannelItem* pItem);
    void redrawStationList(void);    

    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ Preset List Manager +++++++++++++++++++++++++++++++++++++++++++
    ===========================================================================================
    */
    Q_INVOKABLE int getPresetListItem(void);
    ListModel* getCurrentPresetList(void) { return m_listPresetChannel; }
    DABChannelItem* getChannelItemFromPresetList(int index);
    bool isAlreadyExistCurrentChannelInPresetList(void);
    void makePresetListFromChannelList(void);
    int isExistPresetList(DABChannelItem* pItem);
    int findNextChannelIndexInPresetList(DABChannelItem* pItem);
    int findPrevChannelIndexInPresetList(DABChannelItem* pItem);
    int getCurrentChannelIndexFromPresetList(void);
    int getIndexFromPresetList(int index);
    void updatePresetListFromAutoScan(void);


    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ Temp Channel List Manager +++++++++++++++++++++++++++++++++++++
    ===========================================================================================
    */
    ListModel* getTempChannelList(void) const { return this->m_tempListChannel;}
    ListModel* getNewChannelList(void) const { return this->m_newListChannel;}
    void setTempChannelCount(int count) {this->m_iTempChannelCount = count;}
    void setNewChannelCount(int count) {this->m_iNewChannelCount = count;}
    int getTempChannelCount(void) const {return this->m_iTempChannelCount;}
    int getNewChannelCount(void) const {return this->m_iNewChannelCount;}
    ListModel* getTempEpgChannelList(void) const { return this->m_tempEpgListChannel;}
    void saveTempEpgChannelList(void);


    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ ETC +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ===========================================================================================
    */
    void loadChannelData(QString fileName);
    void saveChannelData(QString fileName);

signals:
    void cmdReqSetSelectService(void);
    void cmdWritePresetList(void);
    void scanStop(void);
    void cmdCheckTAService(void);
    void cmdInitPICodeValue(void);
    void playIndexUpdate(bool bViewPostionChanged);

    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ QML Connection  +++++++++++++++++++++++++++++++++++++++++++++++
    ===========================================================================================
    */
    void updateStationList(void);
    void playIconUpdateInPresetList(int currPresetindex);
    void serviceNameForPresetList(QString serviceName, QString ensembleName, int index, int realIndex);
    void sendEnsembleName(QString ensembleName, quint32 frequency, QString sectionStatus);
    void sendServiceName(int listIndex, quint32 frequency, QString ensembleName, QString serviceName, int pty, int ps, bool isDABtoDAB, bool isDABtoFM, bool isNoSignal, bool bPlayIcon);
    void sendServiceNameForSortingBy(int listIndex, quint32 frequency, QString ensembleName, QString serviceName,int pty, int ps, bool isDABtoDAB, bool isDABtoFM, bool isNoSignal, bool bPlayIcon, int index);

public slots:
    void onAddTempChannelItem(quint32 freq, quint16 eId, quint8 iId, QString eLabel, quint8 sCount, quint32 sID, quint8 sType, quint8 subChId, quint16 bitrate, quint8 ps, quint8 charset, quint8 pty, quint8 language, QString sLabel, stASU_INFO asuInfo, stPICODE_INFO picodeInfo, quint8 subCHSize, quint16 address);
    void onUpdateChannelItem(quint32 freq, quint16 eId, quint8 iId, QString eLabel, quint8 sCount, quint32 sID, quint8 sType, quint8 subChId, quint16 bitrate, quint8 ps, quint8 charset, quint8 pty, quint8 language, QString sLabel, stASU_INFO asuInfo, stPICODE_INFO picodeInfo, quint8 subCHSize, quint16 address);
    void onAddNewChannelItem(quint32 freq, quint16 eId, quint8 iId, QString eLabel, quint8 sCount, quint32 sID, quint8 sType, quint8 subChId, quint16 bitrate, quint8 ps, quint8 charset, quint8 pty, quint8 language, QString sLabel, stASU_INFO asuInfo, stPICODE_INFO picodeInfo, quint8 subCHSize, quint16 address);
    void onRemoveUnlockFrequency(quint32 freq);
    void onUpdateChannelListFromAutoScan(void);
    void onUpdateChannelListFromReconfiguration(void);
    void onUpdateChannelListFromReconfigurationNotChanged(void);

private:
    static DABChannelManager *m_DABChannelManager;
    DABUIListener &m_DABUIListener;
    DABChannelItem* m_CurrentChannel;
    ListModel* m_listChannel;
    ListModel* m_listPresetChannel;
    ListModel* m_tempListChannel;
    ListModel* m_newListChannel;
    ListModel* m_tempEpgListChannel;
    QHash<int, QString> ptyTable;
    int m_iChannelCount;
    int m_iTempChannelCount;
    int m_iNewChannelCount;
};

#endif // DABCHANNELMANAGER_H
