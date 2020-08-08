#ifndef DHAVN_MOSTMANAGER_LOGFBLOCKCONTROL_H
#define DHAVN_MOSTMANAGER_LOGFBLOCKCONTROL_H

#include <QtCore>
#include <QTimer>

#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_ACSApplication.h"
#include "DHAVN_MOSTManager_LogFBlock.h"

#define LOG_TAG_LOG_CNT "LogControl"
#define LOG_TAG_MICOM   "MICOM_LOG"

#define ACS_AMF_BRIDGE_MESSAGE_CHANNEL 0xFE
#define ACS_AMF_BRIDGE_TX_MESSAGE_ID   0x01
#define ACS_AMF_BRIDGE_RX_MESSAGE_ID   0x02
#define ACS_AMF_BRIDGE_NET_STATE       0x03
#define ACS_AMF_BRIDGE_INIC_MACADDRESS 0x04

#define EVENT_CHANNEL_MOST_HIGH                 17
#define EVENT_ID_MOST_HIGH_RECEIVE_REFUSED      4
#define EVENT_ID_MOST_HIGH_RECEIVE_FAILED	5
#define EVENT_ID_MOST_HIGH_TRANSMIT_COMPLETE	6
#define EVENT_ID_MOST_HIGH_TRANSMIT_FAILED	7
#define EVENT_ID_MOST_HIGH_TX_CONNECTION_END    8

////2015.01.08, hyunyong.sung, For Debugging on MHP error
#define EVENT_ID_MHP_TX_ERROR_NAC				9
#define EVENT_ID_MHP_TX_ERROR_KILLED				10
#define EVENT_ID_MHP_TX_ERROR_BUILDCON			11
#define EVENT_ID_MHP_TX_ERROR_PRIO				12
#define EVENT_ID_MHP_TX_ERROR_SCALE				13
#define EVENT_ID_MHP_TX_ERROR_NOBUF				14
#define EVENT_ID_MHP_TX_KILLED_TX					15
#define EVENT_ID_MHP_TX_ERROR_NDF				16
#define EVENT_ID_MHP_TX_LEN_ZERO					17
#define EVENT_ID_MHP_TX_CONFIG_NOTOK				18
#define EVENT_ID_MHP_TX_LEN_TOO_BIG				19
#define EVENT_ID_MHP_UNKNOWN_FAILED				20
//===========================================
#define EVENT_CHANNEL_MLB_PINGPONG      24
#define EVENT_ID_MLB_PINGPONG_CYCLIC    1

#define EVENT_CHANEL_LOG_MESSAGE  	23
#define EVENT_ID_LOG_START 		0
#define EVENT_ID_LOG_FROM_AMF_TO_ACS	1
#define EVENT_ID_LOG_FROM_ACS_TO_AMF	2

using namespace acs::generated::fblock::Log;
using namespace k2l::acs::protocols::most;
using namespace k2l::acs::protocols::events;

extern bool g_ACSDisconnByMHP;

typedef struct MbmUsage
{
    ushort tx;                                /*!< The number of used TX buffers */
    ushort rx;                                /*!< The number of used RX buffers */
    ushort num;                               /*!< The sum of tx and rx at a moment */
    ushort memory;                            /*!< Used memory in units (memory * MBM_MEM_UNIT_SIZE = n bytes) */
    ushort fragments;                         /*!< Number of fragments in MNS internal memory */
} MbmUsage;

class CLogFblockControl;

class CMemoryCheckListener : public IACSMOSTEventListener
{
    CLogFblockControl *m_LogFblockControlContext;
public:
    CMemoryCheckListener(CLogFblockControl *context);
    virtual void OnChange();
};

class CLogFblockControl : public QObject, public IACSEventListener
{
    Q_OBJECT
    USE_LOG_UTIL

    bool m_Initialized;

    CMarshaledACSApplication *m_AppContext;

    CLog *m_LogFblock;

    CMemoryCheck *m_CMemoryCheck;
    CMemoryCheckListener *m_CMemoryCheckListener;

public:
    CLogFblockControl(CMarshaledACSApplication *context);
    ~CLogFblockControl();    

    bool Initialize();
    void LogEventListener(uint32_t payloadLength, const uint8_t *pPayload);
    void MemoryCheckListener();

    virtual void OnEvent(uint8_t channel, uint8_t eventID, uint32_t payloadLength, const uint8_t *pPayload);
};

#endif // DHAVN_MOSTMANAGER_LOGFBLOCKCONTROL_H
