/*============================================================================
 * COPYRIGHT (c) 2006-2006 by K2L GmbH
 * All rights reserved
 *
 * ABSTRACT: Defines the interface and type contract for MAC <-> NSW interaction.
 *             The API is designed und the assumption, that NSW layer is completely
 *             driven by MAC (logical device driver). No extra thread is required
 *             nor supported for the NSW implementation.
 *             NSW netservices are guaranteed to be invoked in the context of 
 *             exactly one thread (as far as MAC and this API is concerned.
 *             LLD (layered below NSW) typically is the only source of asynchronous
 *             calls (from the interrupt function or an interrupt service thread).
 *             BOTH the IMAC API and LLD API provide for this situation and
 *             define, which entry point(s) may be called from the context of
 *             the interrupt handler or interrupt service thread.
 *             No other than the specifically marked functions may be invoked
 *             from another thread than the MAC thread.

 * GROUP PREFIX: MAC (MOST ABSTRACTION CODE) (group prefix does not apply to C++).
 *=============================================================================        
 *
 * CONTACT:        K2L Software GmbH, Bluecherstr.32, 75177 Pforzheim; info@k2l.de
 *                Tel: 07231/95688-60        Fax: -65    
 *
 *=============================================================================        
 *
 * HISTORY:    
 *
 * Date:        Ver:    Author:    Scope:
 * ----------------------------------------------------------------------------
 * 11.09.2006    1.0        K2L        Initial version
 * 
 *============================================================================*/

#ifndef __IMAC_H
#define __IMAC_H

#include <stdint.h>        // ANSI C99 standard integral types of known size.

// MOST transport types.
#define MOSTMESSAGE_TRANSPORT_CONTROL_CHANNEL    (0x00)
#define MOSTMESSAGE_TRANSPORT_MOST_HIGH          (0x10)

// MOST High message type
#define MOSTMESSAGE_MHPTYPE_UNSEGMENTED          (0x00)
#define MOSTMESSAGE_MHPTYPE_FIRSTSEGMENT         (0x01)
#define MOSTMESSAGE_MHPTYPE_NEXTSEGMENT          (0x02)
#define MOSTMESSAGE_MHPTYPE_LASTSEGMENT          (0x03)

// MOST Addressing types
#define MOSTMESSAGE_ADDRESSING_LOGICAL          (0x00)
#define MOSTMESSAGE_ADDRESSING_PHYSICAL         (0x40)
#define MOSTMESSAGE_ADDRESSING_BROADCAST        (0x80)
#define MOSTMESSAGE_ADDRESSING_GROUPCAST        (0xC0)

// Explicit datatypes.
typedef uint8_t FBLOCKID;
typedef uint8_t INSTANCEID;
typedef uint16_t DEVICEID;
typedef uint16_t FUNCTIONID;
typedef uint8_t OPTYPE;
typedef uint8_t    MPR;
typedef uint8_t EVENTCHANNEL;
typedef uint8_t EVENTID;
typedef uint32_t MILLISECONDS;

class CLowLevelServices
{
public:
    
    CLowLevelServices()
     : mostHighSocketManager( false ),
       syncManager( false ),
       powerMaster( false ),
       networkMaster( false )
    {
        // NOP
    }

    CLowLevelServices( const CLowLevelServices &rhs )
    {
        if ( this != &rhs )
        {
            mostHighSocketManager = rhs.mostHighSocketManager;
            syncManager           = rhs.syncManager;
            powerMaster           = rhs.powerMaster;
            networkMaster         = rhs.networkMaster;
        }
    }

    CLowLevelServices& operator = ( const CLowLevelServices &rhs )
    {
        if ( this != &rhs )
        {
            mostHighSocketManager = rhs.mostHighSocketManager;
            syncManager           = rhs.syncManager;
            powerMaster           = rhs.powerMaster;
            networkMaster         = rhs.networkMaster;
        }
        return *this;
    }

    bool mostHighSocketManager;
    bool syncManager;
    bool powerMaster;
    bool networkMaster;

};


// forward declarations
struct IMAC;
struct IMACEvents;
struct INetServices;
struct INetServicesEvents;

class CMOSTMessageImpl;

class CMOSTMessage
{
    CMOSTMessageImpl *m_pImpl;

public:
    CMOSTMessage(); // K2L/MAC internal use. Makes no sense if you have no access to m_pImpl.
    explicit CMOSTMessage( uint16_t wPayloadLength );
    CMOSTMessage( const CMOSTMessage& crOther );
    const CMOSTMessage& operator=(const CMOSTMessage& crOther );

    virtual ~CMOSTMessage();

    void DeviceID( DEVICEID deviceID );    // Set
    DEVICEID DeviceID() const;            // Get
    void FBlockID( FBLOCKID fblockID ); // Set
    FBLOCKID FBlockID() const;            // Get
    void InstanceID( INSTANCEID instanceID ); // Set
    INSTANCEID InstanceID() const;        // Get
    void FunctionID( FUNCTIONID functionID ); // Set
    FUNCTIONID FunctionID() const;        // Get
    void OpType( OPTYPE opType );        // Set
    OPTYPE OpType() const;                // Get

    void Transport( uint8_t transport ); // Set
    uint8_t Transport() const;        // Get

    void AddressingType(uint8_t addressingType); //Set
    uint8_t AddressingType() const; //Get

    void MHPType(uint8_t mhpType);
    uint8_t MHPType(void) const;

    // The message source MUST be set up with either of the subsequent 3 functions.
    void SenderIsNetwork();
    void SenderIsModel();
    void SenderIsFBlock();

    //    Payload handling
    uint16_t PayloadLength() const;        // Get
    const uint8_t * Payload() const;        // Get
    uint8_t * Payload();

    // Allows to shrink the payload to shorter length.
    // This is not reallocating or copying the data. Instead, it internally adjusts
    // the stored length information. Stored payload content will be preserved up to the
    // newAndShorterPayloadLength.
    // The length will not be modified, if the new length is larger or equal to the current
    // one.
    // You cannot enlarge the payload.
    // Normally, you should not need to use this function. It is better to find out the
    // exact required length of the payload before you instantatiate the CMOSTMessage.
    // Only in the rare occasion, where you cannot know the payload length beforehand,
    // this function will help to solve the problem.
    void TrimPayloadLength( uint16_t newAndShorterPayloadLength );


    // This is new feature from MHP. Support for Packets>64KBytes.
    // TotalMessageLength indicates length of all chunks transmitted over MHP protocol.
    // Note that chunks are still limited to the 64KBytes length.
    uint32_t TotalMessageLength() const;

private:
    // K2L/MAC internal use only.
    friend class CMACMessageHelper;
};

struct INetServices
{
    // Creates the SINGLE (one and only) instance of the Netservices wrapper class, which implements INetServices.
    // Has to be implemented by Netservices Wrapper module (NSW).
    static INetServices *GetInstance();
    
    virtual ~INetServices() {};
    virtual void Initialize(IMAC *pMAC) {}
    virtual void Initialize(IMAC *pMAC, const CLowLevelServices &servicesEnabled)
    {
        //Default implementation
        Initialize(pMAC);
    }
    virtual void Terminate() = 0;
    virtual void AddEventListener( INetServicesEvents *pListener ) = 0;
    virtual void RemoveEventListener( INetServicesEvents *pListener ) = 0;

    //AddMACEventListener registers ACSMAC INetServicesEvents listener.
    //In old version of the INetServices API no differentiation was done therefore
    //default implementation calls AddEventListener().
    virtual void AddMACEventListener(INetServicesEvents *pListener)
    {
        AddEventListener(pListener);
    }

    //RemoveMACEventListener deregisters ACSMAC INetServicesEvents listener.
    //In old version of the INetServices API no differentiation was done therefore
    //default implementation calls RemoveEventListener().
    virtual void RemoveMACEventListener(INetServicesEvents *pListener)
    {
        RemoveEventListener(pListener);
    }

    virtual void EnterNetwork() = 0;
    virtual void LeaveNetwork() = 0;
    virtual bool SendMessageToMOSTControlChannel( const CMOSTMessage& crMessage ) = 0;
    virtual bool SendMessageToMOSTAsyncChannel( const CMOSTMessage& crMessage ) = 0;
    virtual void CyclicService( MILLISECONDS nominalInterval ) = 0;            // Periodic calls; could be useful for extensions.
    virtual void ServiceMOST() = 0;                                            // Intended for call to MostService().
    virtual void ServiceMOSTTimers() = 0;											
    virtual void ServiceInterrupt() = 0;
    virtual void InterruptDone() = 0;
    virtual void ExtraService() = 0;
    // Some information, available in Netservices is queried by MOST driver (MAC).
    virtual uint8_t GetNodePosition() = 0;
    virtual DEVICEID GetNodeAddress() = 0;
	virtual void SetNodeAddress(DEVICEID nodeAddress) = 0;
};

struct INetServicesEvents
{
    virtual ~INetServicesEvents() {};
    virtual void OnNetservicesAPIAvailable() = 0;
    virtual void OnNetservicesAPIUnavailable() = 0;
    virtual void OnNetOn() = 0;
    virtual void OnNetOff() = 0;
    virtual void OnShortUnlock() = 0;
    virtual void OnLongUnlock() = 0;
    virtual void OnMPR( MPR mpr ) = 0;
    virtual void OnMessageReceived( const CMOSTMessage& crMessage, uint32_t &rdwConsumerCount ) = 0;
};

struct IMAC
{
    virtual ~IMAC() {};

    static IMAC *GetInstance();

    virtual 
    void 
    AddEventListener
        ( IMACEvents *pListener 
        ) = 0;

    virtual
    void
    RemoveEventListener
        ( IMACEvents *pListener 
        ) = 0;

    virtual
    void
    JoinChannel
        ( EVENTCHANNEL channel
        ) = 0;

    virtual 
    void 
    SendK2LEvent
        ( EVENTCHANNEL channel            // [0..0xFE]
        , EVENTID id                    // [0..0xFF]
        , uint8_t tagLength                // [0..3]
        , const uint8_t* cpTagData        // ((cpTagData == NULL) && (tagLength == 0)) || (cpTagData != NULL && tagLength >= 0) 
        , uint16_t payloadLength
        , const uint8_t* cpPayload
        ) = 0;

    // Used by internal, low level MOST applications, like a NetworkMaster implementation.
    // 
    virtual
    void
    SendMOSTMessage
        ( const CMOSTMessage& crMessage 
        ) = 0;

    virtual
    void 
    OnInterrupt
        (
        ) = 0;

    // Requests MAC to invoke INetServices::ServiceMOST().
    // The contract of this mechanism is, that RequestMOSTService() triggers a ServiceMOST() call.
    // Multiple calls to RequestMOSTService() do not cause multiple calls to ServiceMOST().
    // Another ServiceMOST() call takes place, if there was a RequestMOSTService() call after
    // the most recent ServiceMOST() call. (Flag semantics, not Queue semantics).
    virtual
    void
    RequestMOSTService
        (
        ) = 0;

    // Usable for extensions, which require immediate service (e.g. Sync channel handling, triggered by
    // Netservices callbacks.
    // This is NOT intended for extra service cycles on MOSTService()! (See RequestMOSTService().)
    virtual 
    void
    RequestExtraService
        (
        ) = 0;

    // This method adds a Low Level FBlocks to the LocalRegistry.
    // For example this method can be used by Network Master or Power Master or any other FBlock 
    // which has to be integrated in to the NSW.
    virtual 
    void
    AddLowLevelFBlock
        ( FBLOCKID fblockID,
          INSTANCEID instanceID
        ) = 0;

    // This method removes a Low Level FBlocks from the LocalRegistry.
    // For example this method can be used by Network Master or Power Master or any other FBlock 
    // which has to be integrated in to the NSW.
    virtual 
    void
    RemoveLowLevelFBlock
        ( FBLOCKID fblockID,
          INSTANCEID instanceID
        ) = 0;

    // This method is called from NSW Layer when error is detected when sending to specified
    // device, fblock and instance ID.
    virtual 
    void 
    TxFailed
        ( DEVICEID deviceId
        , FBLOCKID fblockID
        , INSTANCEID instanceID
        ) = 0;
};

struct IMACEvents
{    
    virtual ~IMACEvents() {};

    // Provided for extended functionality. For well defined core functionality, MAC will call INetservices API.
    // All events, received by MAC's MoccaClient connection are forwarded to IMACEvents::OnK2LEvent.
    // See also: INetServices, IMAC::JoinChannel(), IMAC::SendK2LEvent()
    virtual 
    void 
    OnK2LEvent
        ( EVENTCHANNEL channel            // [0..0xFE]
        , EVENTID id                    // [0..0xFF]
        , uint8_t tagLength                // [0..3]
        , const uint8_t* cpTagData        // ((cpTagData == NULL) && (tagLength == 0)) || (cpTagData != NULL && tagLength >= 0) 
        , uint16_t payloadLength
        , const uint8_t* cpPayload
        ) = 0;
};

#endif
