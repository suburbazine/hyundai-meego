//Warning disabled because of following arrays:
//-CMOSTHighRxBufferConfiguration::m_Configurations
//It is only a tip from compiler that array initialization behavior was changed.
//It has no impact on our code.
#pragma warning( disable : 4351 )

#include <stddef.h>
#include <K2LDebug.h>
#include <K2LMOSTHigh.h>
extern "C"
{
#include <mostns.h>

}

#include <K2LDebug.h>

CMOSTHighRxBufferConfiguration::CMOSTHighRxBufferConfiguration
(
 )
: m_pMAC(NULL)
, m_Configurations()
, m_pBegin(&m_Configurations[0])
, m_pEndOfBuffer(&m_Configurations[LIMIT_CONFIG_STORAGE_CAPACITY])
, m_pEnd(&m_Configurations[0])
{

}

//virtual
CMOSTHighRxBufferConfiguration::~CMOSTHighRxBufferConfiguration()
{
}

void
CMOSTHighRxBufferConfiguration::Initialize
(IMAC *pMAC
 )
{
    K2L_DEBUGCHK(NULL != pMAC);
    m_pMAC = pMAC;
    pMAC->AddEventListener(this);
    pMAC->JoinChannel(EVENT_CHANNEL_MOST_HIGH);
}

void
CMOSTHighRxBufferConfiguration::Terminate
(
 )
{
    if( NULL != m_pMAC )
    {
    m_pMAC->RemoveEventListener(this);
    m_pMAC = NULL;
}
    else
    {
        K2L_RETAILMSG(K2L_ZONE_ERROR, (K2LDEBUG_TEXT("CMOSTHighRxBufferConfiguration::Terminate() - m_pMAC == NULL\r\n")));
    }
}

//
// IMACEvents implementation.
//
void
CMOSTHighRxBufferConfiguration::OnK2LEvent
(EVENTCHANNEL channel // [0..0xFE]
 , EVENTID id // [0..0xFF]
 , uint8_t tagLength // [0..3]
 , const uint8_t* cpTagData // ((cpTagData == NULL) && (tagLength == 0)) || (cpTagData != NULL && tagLength >= 0)
 , uint16_t payloadLength
 , const uint8_t* cpPayload
 )
{
    if (EVENT_CHANNEL_MOST_HIGH == channel)
    { // This is our channel.
        switch (id)
        {
        case EVENT_ID_MOST_HIGH_ADD_CONFIGURATION:
        {
            if (10 == payloadLength)
            {
                uint16_t readPosition = 0;
                uint16_t wFBlock;
                uint16_t wInstance;
                uint16_t wFunction;
                uint16_t wOpType;
                uint16_t wLength;

                wFBlock = ReadMOST_WORD(cpPayload, readPosition, payloadLength);
                wInstance = ReadMOST_WORD(cpPayload, readPosition, payloadLength);
                wFunction = ReadMOST_WORD(cpPayload, readPosition, payloadLength);
                wOpType = ReadMOST_WORD(cpPayload, readPosition, payloadLength);
                wLength = ReadMOST_WORD(cpPayload, readPosition, payloadLength);

                uint64_t key = InternalKeyFromConfigurationKeys(wFBlock, wInstance, wFunction, wOpType);
                Config_t config;
                config.key = key;
                config.value = wLength;
                AddConfiguration(config);
            }
        }
            break;
        case EVENT_ID_MOST_HIGH_REMOVE_CONFIGURATION:
        {
            if (8 == payloadLength)
            {
                uint16_t readPosition = 0;
                uint16_t wFBlock;
                uint16_t wInstance;
                uint16_t wFunction;
                uint16_t wOpType;

                wFBlock = ReadMOST_WORD(cpPayload, readPosition, payloadLength);
                wInstance = ReadMOST_WORD(cpPayload, readPosition, payloadLength);
                wFunction = ReadMOST_WORD(cpPayload, readPosition, payloadLength);
                wOpType = ReadMOST_WORD(cpPayload, readPosition, payloadLength);

                uint64_t key = InternalKeyFromConfigurationKeys(wFBlock, wInstance, wFunction, wOpType);
                Config_t config;
                config.key = key;
                config.value = 0;
                RemoveConfiguration(config);
            }
        }
            break;
        case EVENT_ID_MOST_HIGH_REMOVE_ALL_CONFIGURATIONS:
        {
            Clear();
        }
            break;
        default:
            K2L_DEBUGCHK(false); // We are supposed to be the only sender of any other MOST High events!
            break;
        }
    }
}

uint16_t
CMOSTHighRxBufferConfiguration::FindBufferSize
(FBLOCKID fblockID
 , INSTANCEID instanceID
 , FUNCTIONID functionID
 , OPTYPE opType
 )
{
    uint16_t bufferSize = CONFIGURATION_DEFAULT_BUFFER_SIZE;
    uint_fast16_t index;

    uint64_t key = InternalKeyFromConfigurationKeys(fblockID, instanceID, functionID, opType);

    Config_t *pResult = NULL;
    for (index = 0; index < 5; ++index)
    {
        if (Find(key, pResult))
        {
            K2L_DEBUGCHK(NULL != pResult);
            K2L_DEBUGCHK(m_pEnd != pResult);
            bufferSize = pResult->value;
            break;
        }
        else
        { // try to find a less specific configuration
            uint64_t mask = ((uint64_t) 0xFFFF) << (16 * index);
            key |= mask;
            K2L_DEBUGCHK(index != 0 || mask == 0x000000000000FFFFull);
            K2L_DEBUGCHK(index != 1 || mask == 0x00000000FFFF0000ull);
            K2L_DEBUGCHK(index != 2 || mask == 0x0000FFFF00000000ull);
            K2L_DEBUGCHK(index != 3 || mask == 0xFFFF000000000000ull);
        }
    }

    return bufferSize;
}

void
CMOSTHighRxBufferConfiguration::AddConfiguration
(const Config_t &config
 )
{
    if (m_pBegin != m_pEnd)
    {
        Config_t *pInsertPosition = LowerBound(m_pBegin, m_pEnd, config.key);
        if (pInsertPosition == m_pEnd && m_pEnd != m_pEndOfBuffer)
        { // Insert at the back of the sequence.
            (*m_pEnd).key = config.key;
            (*m_pEnd).value = config.value;
            ++m_pEnd;
        }
        else
        {
            if (pInsertPosition->key == config.key)
            { // Entry already exists -> override the previous configuration. Length of sequence does not change.
                (*pInsertPosition).key = config.key;
                (*pInsertPosition).value = config.value;
            }
            else
            {
                // Entry not existing yet, pInsertPosition points to the location, where the entry may be inserted.
                // Move from back to front the configs on place to the right and then
                // insert the new entry in the spare location.
                if (m_pEnd != m_pEndOfBuffer)
                { // We have enough space to add another entry.
                    Config_t *pTemp = m_pEnd;
                    m_pEnd++;
                    while (pTemp != pInsertPosition)
                    {
                        (*pTemp) = (*(pTemp - 1));
                        --pTemp;
                    }
                    K2L_DEBUGCHK(pTemp == pInsertPosition);
                    (*(pTemp)) = config;
                }
            }
        }
    }
    else
    { // First entry.
        (*m_pEnd).key = config.key;
        (*m_pEnd).value = config.value;
        ++m_pEnd;
    }
}

void
CMOSTHighRxBufferConfiguration::RemoveConfiguration
(const Config_t & config
 )
{
    Config_t *pEntry = NULL;
    if (Find(config.key, pEntry))
    {
        K2L_DEBUGCHK(pEntry != m_pEnd);
        K2L_DEBUGCHK(pEntry->key == config.key);
        Config_t *pNext = pEntry + 1;
        while (pNext != m_pEnd)
        {
            (*pEntry) = (*pNext);
            pEntry++;
            pNext++;
        }
        --m_pEnd;
    }
}

void
CMOSTHighRxBufferConfiguration::Clear
(
 )
{
    m_pBegin = &m_Configurations[0];
    m_pEnd = m_pBegin;
}

CMOSTHighRxBufferConfiguration::Config_t *
CMOSTHighRxBufferConfiguration::LowerBound
(Config_t* pFirst
 , Config_t *pLast
 , uint64_t key
 )
{
    Config_t *pLower = pFirst;
    Config_t *pCurrent = pLast;
    uint_fast16_t count = pLast - pFirst;
    CheckDistance(pLower, pLast, count);

    for (/* EMPTY */; 0 < count; /* EMPTY */)
    {
        uint_fast16_t count2 = count / 2;
        pCurrent = pLower + count2;
        CheckAdvance(pLower, pCurrent, count2);

        if (pCurrent->key < key)
        {
            pLower = ++pCurrent;
            count -= count2 + 1;
        }
        else
        {
            count = count2;
        }
    }
    return pCurrent;
}

bool
CMOSTHighRxBufferConfiguration::Find
(uint64_t key
 , Config_t *&rpResult
 )
{
    bool bFound = false;
    Config_t *pCurrent = LowerBound(m_pBegin, m_pEnd, key);
    if ((pCurrent != m_pEnd) && !(key < pCurrent->key))
    {
        K2L_DEBUGCHK(pCurrent->key == key);
        rpResult = pCurrent;
        bFound = true;
    }
    return bFound;
}
