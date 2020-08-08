#include "DHAVN_MOSTManager_LogFBlockControl.h"

CLogFblockControl::CLogFblockControl(CMarshaledACSApplication *context)
    : m_Initialized(false)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)

    m_AppContext = context;

    m_LogFblock = new CLog(0x01);

    m_CMemoryCheck = new CMemoryCheck(m_LogFblock);
    m_CMemoryCheckListener = new CMemoryCheckListener(this);
}

CLogFblockControl::~CLogFblockControl()
{
    delete m_LogFblock;
    delete m_CMemoryCheck;
    delete m_CMemoryCheckListener;
}

bool CLogFblockControl::Initialize()
{
    bool result = false;

    if(m_LogFblock->InitializeMOSTElement())
    {
        /** Setting the handlers for required parameters */
        m_CMemoryCheck->SetListener(m_CMemoryCheckListener);

        result = m_AppContext->MOST()->AddFunctionBlock(m_LogFblock);

        m_Initialized = result;
    }

    return result;
}

void CLogFblockControl::OnEvent(uint8_t channel, uint8_t eventID, uint32_t payloadLength, const uint8_t *pPayload)
{
    if(channel == EVENT_CHANEL_LOG_MESSAGE)
    {
        if(eventID == EVENT_ID_LOG_FROM_AMF_TO_ACS)
        {
            LogEventListener(payloadLength, pPayload);
        }
    }
    else if(channel == EVENT_CHANNEL_MOST_HIGH)
    {
        switch(eventID)
        {
            case EVENT_ID_MOST_HIGH_RECEIVE_REFUSED:
                LOG_CRITICAL << LOG_TAG_LOG_CNT << " MOST_HIGH_RECEIVE_REFUSED" << LOG_ENDL;
                //LOG_CRITICAL << "MOST_HIGH_RECEIVE_REFUSED" << " PID : " << getpid() << ", TID : " << syscall(__NR_gettid) << LOG_ENDL;
                break;
            case EVENT_ID_MOST_HIGH_RECEIVE_FAILED:
                LOG_CRITICAL << LOG_TAG_LOG_CNT << " MOST_HIGH_RECEIVE_FAILED" << LOG_ENDL;
                break;
            case EVENT_ID_MOST_HIGH_TRANSMIT_COMPLETE:
                LOG_TRACE << LOG_TAG_LOG_CNT << " MOST_HIGH_TRANSMIT_COMPLETE" << LOG_ENDL;
                break;
/*				
            case EVENT_ID_MOST_HIGH_TRANSMIT_FAILED:
			g_ACSDisconnByMHP = true;	
                LOG_CRITICAL << LOG_TAG_LOG_CNT << " MOST_HIGH_TRANSMIT_FAILED" << LOG_ENDL;
                break;
*/                
            case EVENT_ID_MOST_HIGH_TX_CONNECTION_END:
                LOG_INFO << LOG_TAG_LOG_CNT << " MOST_HIGH_TX_CONNECTION_END" << LOG_ENDL;
                break;
//2015.01.08, hyunyong.sung, For Debugging on MHP error===================================
		case EVENT_ID_MHP_TX_ERROR_NAC:
			g_ACSDisconnByMHP = true;					
			LOG_CRITICAL << LOG_TAG_LOG_CNT << " EVENT_ID_MHP_TX_ERROR_NAC" << LOG_ENDL;
			break;
            case EVENT_ID_MHP_TX_ERROR_KILLED:
			g_ACSDisconnByMHP = true;									
			LOG_CRITICAL << LOG_TAG_LOG_CNT << " EVENT_ID_MHP_TX_ERROR_KILLED" << LOG_ENDL;
			break;
            case EVENT_ID_MHP_TX_ERROR_BUILDCON:
			g_ACSDisconnByMHP = true;									
			LOG_CRITICAL << LOG_TAG_LOG_CNT << " EVENT_ID_MHP_TX_ERROR_BUILDCON" << LOG_ENDL;
			break;
            case EVENT_ID_MHP_TX_ERROR_PRIO:
			g_ACSDisconnByMHP = true;									
			LOG_CRITICAL << LOG_TAG_LOG_CNT << " EVENT_ID_MHP_TX_ERROR_PRIO" << LOG_ENDL;
			break;
            case EVENT_ID_MHP_TX_ERROR_SCALE:
			g_ACSDisconnByMHP = true;									
			LOG_CRITICAL << LOG_TAG_LOG_CNT << " EVENT_ID_MHP_TX_ERROR_SCALE" << LOG_ENDL;
			break;
            case EVENT_ID_MHP_TX_ERROR_NOBUF:
			g_ACSDisconnByMHP = true;									
			LOG_CRITICAL << LOG_TAG_LOG_CNT << " EVENT_ID_MHP_TX_ERROR_NOBUF" << LOG_ENDL;
			break;
            case EVENT_ID_MHP_TX_KILLED_TX:
			g_ACSDisconnByMHP = true;									
			LOG_CRITICAL << LOG_TAG_LOG_CNT << " EVENT_ID_MHP_TX_KILLED_TX" << LOG_ENDL;
			break;
            case EVENT_ID_MHP_TX_ERROR_NDF:
			g_ACSDisconnByMHP = true;									
			LOG_CRITICAL << LOG_TAG_LOG_CNT << " EVENT_ID_MHP_TX_ERROR_NDF" << LOG_ENDL;
			break;
            case EVENT_ID_MHP_TX_LEN_ZERO:
			g_ACSDisconnByMHP = true;									
			LOG_CRITICAL << LOG_TAG_LOG_CNT << " EVENT_ID_MHP_TX_LEN_ZERO" << LOG_ENDL;
			break;
            case EVENT_ID_MHP_TX_CONFIG_NOTOK:
			g_ACSDisconnByMHP = true;									
			LOG_CRITICAL << LOG_TAG_LOG_CNT << " EVENT_ID_MHP_TX_CONFIG_NOTOK" << LOG_ENDL;
			break;
            case EVENT_ID_MHP_TX_LEN_TOO_BIG:
			g_ACSDisconnByMHP = true;									
			LOG_CRITICAL << LOG_TAG_LOG_CNT << " EVENT_ID_MHP_TX_LEN_TOO_BIG" << LOG_ENDL;
			break;
//================================================================================				
            default:
			LOG_CRITICAL << LOG_TAG_LOG_CNT << " EVENT_ID_MHP_UNKNOWN_FAILED" << LOG_ENDL;
			break;
        }
    }
    else if(channel == EVENT_CHANNEL_MLB_PINGPONG)
    {
        if(eventID == EVENT_ID_MLB_PINGPONG_CYCLIC)
        {
            LOG_INFO << LOG_TAG_LOG_CNT << " MLB_PINGPONG_SIGNAL" << LOG_ENDL;
            m_AppContext->Events()->Send(EVENT_CHANNEL_MLB_PINGPONG, EVENT_ID_MLB_PINGPONG_CYCLIC, 0, NULL);
        }
    }
    else
    {
        /* No Statement */
    }
}

/********************************************
(0xFFFF & 0x0?00 = 0: SPIDF, 1: MONO1, 2: MONO2, 3: IBOX, 4: Telephone)
0x0??? = msm_CM_BuildConnection 의 결과값
0x1??? = msm_CM_cb_BuildConnectionResult 의 결과값

0x20F0 = K2LQueue Push Error in MSMMAdaptor_build_connection_handler
0x20F1 = K2LQueue Push Error in MSMMAdaptor_cb_rebuild_timer
0x20F2 = K2LQueue Initialization Error
0x20F3 = Processing Timer Initialization Failed

0x2100 = Call MostStartUp()
0x2101 = msm_PM_NetworkStartUp() returns false
0x22?? = Warning, MostStartUp() returns ??
0x23?? = Power master state
0x24?? = MOST network state

0x301? = AMP State (0: Disconnected, 1: Connected)
0x302? = IBOX State (0: Disconnected, 1: Connected)
0x303? = TELEPHONE State (0: Disconnected, 1: Connected)
0x304? = LOG State (0: Disconnected, 1: Connected)

0x311? = Request SPDIF Channel (0: Allocate, 1: DeAllocate)
0x312? = Request MONO1 Channel (0: Allocate, 1: DeAllocate)
0x313? = Request MONO2 Channel (0: Allocate, 1: DeAllocate)

0x3200 = Low voltage detected
0x3201 = Low voltage detected and shutdown
0x3210 = Critical voltage detected
0x3211 = Critical voltage detected and shutdown
0x3220 = Wake voltage detected
0x3221 = Low -> Wake voltage detected and wakeup
0x3222 = Critical -> Wake voltage detected and wakeup
0x3230 = MPM Low voltage detected and shutdown
0x3231 = MPM Low -> Normal voltage detected and wakeup
0x3240 = MPM Critical voltage detected
0x3241 = MPM Critical -> Normal voltage detected and wakeup

0x40?? = AMP Mute State
0x41?? = Current System Power State
0x42?? = Fail to Send System Power State
0x4301 ~ 0x4315 = MLB_IPC_TRACE_MESSAGE

0x7??? = Amount of used bandwidth in number of bytes per MOST frame

0x8000 ~ 0xCFFF : NetSerivices Error

0XD0?? = msg_tx_status
0XD1?? = msval_error
0XD2?? = msval_event
0xD300 = INIC reset by NetServices, MostHal_ResetInicBegin is called
0xD301 = INIC reset by alarm timeout
0xD302 = INIC reset by NSR_E_COMM event

0xFD?? = No Ack response from ACS, the length of messages
0xFE?? = Queue overflow count
0xFFFF = Logging Start
********************************************/
void CLogFblockControl::LogEventListener(uint32_t payloadLength, const uint8_t *pPayload)
{
    QByteArray codeArray;
    uint16_t code;
    bool isQueueing = false;
    uchar ack;

    codeArray.append((const char *)pPayload, payloadLength);

    if(codeArray.size() == 0)
    {
        LOG_CRITICAL << LOG_TAG_LOG_CNT << " Invalid log" << LOG_ENDL;
    }

    if(codeArray.size() > 2)
    {
        LOG_INFO << LOG_TAG_MICOM << " Log Queued ---->" << LOG_ENDL;
        isQueueing = true;
    }

    LOG_INFO << LOG_TAG_MICOM << " Log : " << codeArray.toHex().data() << LOG_ENDL;
    LOG_INFO << LOG_TAG_MICOM << " Log Size : " << codeArray.size()/2 << LOG_ENDL;

    for(int i=0; i<codeArray.size();)
    {
       // Convert byte ordering from big endian
       code = (uint16_t)codeArray.at(i+1) << 8 | (uint8_t)codeArray.at(i);
       i = i + 2;

       if( code < 0x2000 )
       {
           uint8_t type = (code & 0xF000) >> 12;
           uint8_t handle = (code & 0x0F00) >> 8;
           uint8_t info = code & 0x00FF;

           if(type == 0x00)
           {
               LOG_HIGH << LOG_TAG_MICOM << " msm_CM_BuildConnection [" << handle << "] : " << info << LOG_ENDL;
           }
           else
           {
               LOG_HIGH << LOG_TAG_MICOM << " msm_CM_cb_BuildConnectionResult [" << handle << "] : " << info << LOG_ENDL;
           }
       }
       else if( (0x2000 <= code) && (code < 0x3000) )
       {
           uint8_t type = (code & 0xFF00) >> 8;
           uint8_t info = code & 0x00FF;

           switch(type)
           {
               case 0x20:
                   switch(info)
                   {
                       case 0xF0:
                           LOG_HIGH << LOG_TAG_MICOM << " K2LQueue Push Error in MSMMAdaptor_build_connection_handler" << LOG_ENDL;
                           break;
                       case 0xF1:
                           LOG_HIGH << LOG_TAG_MICOM << " K2LQueue Push Error in MSMMAdaptor_cb_rebuild_timer" << LOG_ENDL;
                           break;
                       case 0xF2:
                           LOG_HIGH << LOG_TAG_MICOM << " K2LQueue Initialization Error" << LOG_ENDL;
                           break;
                       case 0xF3:
                           LOG_HIGH << LOG_TAG_MICOM << " Processing Timer Initialization Failed" << LOG_ENDL;
                           break;
                       default:
                           LOG_HIGH << LOG_TAG_MICOM << " Unknown : " << code << LOG_ENDL;
                           break;
                   }
                   break;
               case 0x21:
                   if(info == 0x00)
                   {
                       LOG_HIGH << LOG_TAG_MICOM << " MostStartUp()" << LOG_ENDL;
                   }
                   else if(info == 0x01)
                   {
                       LOG_HIGH << LOG_TAG_MICOM << " msm_PM_NetworkStartUp() failed" << LOG_ENDL;
                   }
                   else
                   {
                       LOG_HIGH << LOG_TAG_MICOM << " Unknown : " << code << LOG_ENDL;
                   }
                   break;
               case 0x22:
                   LOG_HIGH << LOG_TAG_MICOM << " Warning, MostStartUp() returns : " << info << LOG_ENDL;
                   break;
               case 0x23:
                   LOG_HIGH << LOG_TAG_MICOM << " Power Master State : " << info << LOG_ENDL;
                   break;
               case 0x24:
                   LOG_HIGH << LOG_TAG_MICOM << " MOST Network State : " << info << LOG_ENDL;
                   break;
               default:
                   LOG_HIGH << LOG_TAG_MICOM << " Unknown : " << code << LOG_ENDL;
                   break;
           }
       }
       else if( (0x3000 <= code) && (code < 0x7000) )
       {
           uint8_t type = (code & 0xFF00) >> 8;
           uint8_t info = code & 0x00FF;

           switch(type)
           {
               case 0x30:
                   switch( (info&0xF0) >> 4 )
                   {
                       case 0x01:
                           if( (info&0x0F) == 0 )
                           {
                               LOG_HIGH << LOG_TAG_MICOM << " AMP Disconnected" << LOG_ENDL;
                           }
                           else
                           {
                               LOG_HIGH << LOG_TAG_MICOM << " AMP Connected" << LOG_ENDL;
                           }
                           break;
                       case 0x02:
                           if( (info&0x0F) == 0 )
                           {
                               LOG_HIGH << LOG_TAG_MICOM << " IBOX Disconnected" << LOG_ENDL;
                           }
                           else
                           {
                               LOG_HIGH << LOG_TAG_MICOM << " IBOX Connected" << LOG_ENDL;
                           }
                           break;
                       case 0x03:
                           if( (info&0x0F) == 0 )
                           {
                               LOG_HIGH << LOG_TAG_MICOM << " Telephone Disconnected" << LOG_ENDL;
                           }
                           else
                           {
                               LOG_HIGH << LOG_TAG_MICOM << " Telephone Connected" << LOG_ENDL;
                           }
                           break;
                       case 0x04:
                           if( (info&0x0F) == 0 )
                           {
                               LOG_HIGH << LOG_TAG_MICOM << " LOGFB Disconnected" << LOG_ENDL;
                           }
                           else
                           {
                               LOG_HIGH << LOG_TAG_MICOM << " LOGFB Connected" << LOG_ENDL;
                           }
                           break;
                       default:
                           LOG_HIGH << LOG_TAG_MICOM << " Unknown : " << code << LOG_ENDL;
                           break;
                   }
                   break;
               case 0x31:
                   switch( (info & 0xF0) >> 4 )
                   {
                       case 0x01:
                           if( (info&0x0F) == 0 )
                           {
                               LOG_HIGH << LOG_TAG_MICOM << " Request SPDIF Channel Allocation" << LOG_ENDL;
                           }
                           else
                           {
                               LOG_HIGH << LOG_TAG_MICOM << " Request SPDIF Channel DeAllocation" << LOG_ENDL;
                           }
                           break;
                       case 0x02:
                           if( (info&0x0F) == 0 )
                           {
                               LOG_HIGH << LOG_TAG_MICOM << " Request MONO1 Channel Allocation" << LOG_ENDL;
                           }
                           else
                           {
                               LOG_HIGH << LOG_TAG_MICOM << " Request MOMO1 Channel DeAllocation" << LOG_ENDL;
                           }
                           break;
                       case 0x03:
                           if( (info&0x0F) == 0 )
                           {
                               LOG_HIGH << LOG_TAG_MICOM << " Request MOMO2 Channel Allocation" << LOG_ENDL;
                           }
                           else
                           {
                               LOG_HIGH << LOG_TAG_MICOM << " Request MOMO2 Channel DeAllocation" << LOG_ENDL;
                           }
                           break;
                       default:
                           LOG_HIGH << LOG_TAG_MICOM << " Unknown : " << code << LOG_ENDL;
                           break;
                   }
                   break;
               case 0x32:
                   switch(info)
                   {
                       case 0x00:
                           LOG_HIGH << LOG_TAG_MICOM << " Low voltage detected" << LOG_ENDL;
                           break;
                       case 0x01:
                           LOG_HIGH << LOG_TAG_MICOM << " Low voltage detected and shutdown" << LOG_ENDL;
                           break;
                       case 0x10:
                           LOG_HIGH << LOG_TAG_MICOM << " Critical voltage detected" << LOG_ENDL;
                           break;
                       case 0x11:
                           LOG_HIGH << LOG_TAG_MICOM << " Critical voltage detected and shutdown" << LOG_ENDL;
                           break;
                       case 0x20:
                           LOG_HIGH << LOG_TAG_MICOM << " Wake voltage detected" << LOG_ENDL;
                           break;
                       case 0x21:
                           LOG_HIGH << LOG_TAG_MICOM << " Low -> Wake voltage detected and wakeup" << LOG_ENDL;
                           break;
                       case 0x22:
                           LOG_HIGH << LOG_TAG_MICOM << " Critical -> Wake voltage detected and wakeup" << LOG_ENDL;
                           break;
                       case 0x30:
                           LOG_HIGH << LOG_TAG_MICOM << " MPM Low voltage detected and shutdown" << LOG_ENDL;
                           break;
                       case 0x31:
                           LOG_HIGH << LOG_TAG_MICOM << " MPM Low -> Normal voltage detected and wakeup" << LOG_ENDL;
                           break;
                       case 0x40:
                           LOG_HIGH << LOG_TAG_MICOM << " MPM Critical voltage detected" << LOG_ENDL;
                           break;
                       case 0x41:
                           LOG_HIGH << LOG_TAG_MICOM << " MPM Critical -> Normal voltage detected and wakeup" << LOG_ENDL;
                           break;
                       default:
                           LOG_HIGH << LOG_TAG_MICOM << " Unknown : " << code << LOG_ENDL;
                           break;
                   }
                   break;
               case 0x40:
                   switch(info)
                   {
                       case 0x00:
                           LOG_HIGH << LOG_TAG_MICOM << " AMP Mute State : Demute" << LOG_ENDL;
                           break;
                       case 0x01:
                           LOG_HIGH << LOG_TAG_MICOM << " AMP Mute State : Mute" << LOG_ENDL;
                           break;
                       case 0xFF:
                           LOG_HIGH << LOG_TAG_MICOM << " AMP Mute State : Invalid" << LOG_ENDL;
                           break;
                       default:
                           LOG_HIGH << LOG_TAG_MICOM << " Unknown : " << code << LOG_ENDL;
                           break;
                   }
                   break;
               case 0x41:
                   switch(info)
                   {
                       case 0xFF:
                           LOG_HIGH << LOG_TAG_MICOM << " System Power State : Invalid" << LOG_ENDL;
                           break;
                       default:
                           LOG_HIGH << LOG_TAG_MICOM << " System Power State : " << info << LOG_ENDL;
                           break;
                   }
                   break;
               case 0x42:
                   LOG_HIGH << LOG_TAG_MICOM << " Fail to Send System Power State : " << info << LOG_ENDL;
                   break;
               case 0x43:
                   LOG_HIGH << LOG_TAG_MICOM << " MLB_IPC_TRACE_MESSAGE : " << info << LOG_ENDL;
                   break;
               default:
                   LOG_HIGH << LOG_TAG_MICOM << " Unknown : " << code << LOG_ENDL;
                   break;
           }
       }
       else if( (0x7000 <= code) && (code < 0x8000) )
       {
           ushort info = code & 0xFFF;
           LOG_HIGH << LOG_TAG_MICOM << " Total bandwidth : " << info << " bytes" << LOG_ENDL;
       }
       else if( (0x8000 <= code) && (code < 0xD000) )
       {
           LOG_HIGH << LOG_TAG_MICOM << " NetSerivce Error Code : " << code << LOG_ENDL;
       }
       else if( (0xD000 <= code) && (code < 0xE000) )
       {
           uint8_t type = (code & 0xFF00) >> 8;
           uint8_t info = code & 0x00FF;

           switch(type)
           {
               case 0xD0:
                   LOG_HIGH << LOG_TAG_MICOM << " msg_tx_status : " << info << LOG_ENDL;
                   break;
               case 0xD1:
                   if(info == 0x00)
                   {
                       LOG_HIGH << LOG_TAG_MICOM << " msval_error : MSVAL_ERR_UNLOCK_SHORT" << LOG_ENDL;
                   }
                   else if(info == 0x01)
                   {
                       LOG_HIGH << LOG_TAG_MICOM << " msval_error : MSVAL_ERR_UNLOCK_CRITICAL" << LOG_ENDL;
                   }
                   else if(info == 0x04)
                   {
                       LOG_HIGH << LOG_TAG_MICOM << " msval_error : MSVAL_ERR_INIT_ERROR" << LOG_ENDL;
                   }
                   else if(info == 0x10)
                   {
                       LOG_HIGH << LOG_TAG_MICOM << " msval_error : MSVAL_ERR_STARTUP_FAILED" << LOG_ENDL;
                   }
                   else
                   {
                       LOG_HIGH << LOG_TAG_MICOM << " msval_error : " << info << LOG_ENDL;
                   }
                   break;
               case 0xD2:
                   if(info == 0x20)
                   {
                       LOG_HIGH << LOG_TAG_MICOM << " msval_event : MSVAL_E_NETON" << LOG_ENDL;
                   }
                   else if(info == 0x21)
                   {
                       LOG_HIGH << LOG_TAG_MICOM << " msval_event : MSVAL_E_SHUTDOWN" << LOG_ENDL;
                   }
                   else
                   {
                       LOG_HIGH << LOG_TAG_MICOM << " msval_event : " << info << LOG_ENDL;
                   }
                   break;
               case 0xD3:
                   if(info == 0x00)
                   {
                       LOG_HIGH << LOG_TAG_MICOM << " INIC reset by NetServices" << LOG_ENDL;
                   }
                   else if(info == 0x01)
                   {
                       LOG_HIGH << LOG_TAG_MICOM << " INIC reset by alarm timeout" << LOG_ENDL;
                   }
                   else if(info == 0x02)
                   {
                       LOG_HIGH << LOG_TAG_MICOM << " INIC reset by NSR_E_COMM" << LOG_ENDL;
                   }
                   else
                   {
                       LOG_HIGH << LOG_TAG_MICOM << " Unknown : " << code << LOG_ENDL;
                   }
                   break;
               default:
                   LOG_HIGH << LOG_TAG_MICOM << " Unknown : " << code << LOG_ENDL;
                   break;
           }
       }
       else if( code >= 0xE000 )
       {
           uint8_t type = (code & 0xFF00) >> 8;
           uint8_t info = code & 0x00FF;

           switch(type)
           {
               case 0xFD:
                   LOG_HIGH << LOG_TAG_MICOM << " No Ack response, length : " << info << LOG_ENDL;
                   return;  // Do NOT respond message to AMF, This is only information.
               case 0xFE:
                   LOG_HIGH << LOG_TAG_MICOM << " Queue overflow count : " << info << LOG_ENDL;
                   break;
               case 0xFF:
                   LOG_HIGH << LOG_TAG_MICOM << " Logging start" << LOG_ENDL;
                   break;
               default:
                   LOG_HIGH << LOG_TAG_MICOM << " Unknown : " << code << LOG_ENDL;
                   break;
           }
       }
       else
       {
           LOG_HIGH << LOG_TAG_MICOM << " Unknown : " << code << LOG_ENDL;
       }
    }

    if( isQueueing )
    {
        LOG_INFO << LOG_TAG_MICOM << " <---- End of Queue" << LOG_ENDL;
        isQueueing = false;
    }

    ack = (uchar)codeArray.size()/2;
    m_AppContext->Events()->Send(EVENT_CHANEL_LOG_MESSAGE, EVENT_ID_LOG_FROM_ACS_TO_AMF, 1, &ack);
}

CMemoryCheckListener::CMemoryCheckListener(CLogFblockControl *context)
{
    m_LogFblockControlContext = context;
}

void CMemoryCheckListener::OnChange()
{
    m_LogFblockControlContext->MemoryCheckListener();
}

void CLogFblockControl::MemoryCheckListener()
{
    static MbmUsage top;
    MbmUsage current;

    bool isUpdated = false;

    m_CMemoryCheck->MemoryDataType().LoadNext();

    current.tx = m_CMemoryCheck->MbmTxValue();
    current.rx = m_CMemoryCheck->MbmRxValue();
    current.num = m_CMemoryCheck->MbmNumValue();
    current.memory = m_CMemoryCheck->MbmMemoryValue();
    current.fragments = m_CMemoryCheck->MbmFragValue();

    if(top.tx < current.tx)
    {
        top.tx = current.tx;
        isUpdated = true;
    }

    if(top.rx < current.rx)
    {
        top.rx = current.rx;
        isUpdated = true;
    }

    if(top.num < current.num)
    {
        top.num = current.num;
        isUpdated = true;
    }

    if(top.memory < current.memory)
    {
        top.memory = current.memory;
        isUpdated = true;
    }

    if(top.fragments < current.fragments)
    {
        top.fragments = current.fragments;
        isUpdated = true;
    }

    if(isUpdated)
    {
        LOG_HIGH << LOG_TAG_LOG_CNT << " The number of used TX buffers : " << top.tx << LOG_ENDL;
        LOG_HIGH << LOG_TAG_LOG_CNT << " The number of used RX buffers : " << top.rx << LOG_ENDL;
        LOG_HIGH << LOG_TAG_LOG_CNT << " The sum of tx and rx at a moment : " << top.num << LOG_ENDL;
        LOG_HIGH << LOG_TAG_LOG_CNT << " Used memory in units (memory * MBM_MEM_UNIT_SIZE = n bytes) : " << top.memory << LOG_ENDL;
        LOG_HIGH << LOG_TAG_LOG_CNT << " Number of fragments in MNS internal memory : " << top.fragments << LOG_ENDL;
    }
}
