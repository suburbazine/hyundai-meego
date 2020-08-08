/*=== K2L GmbH ===============================================================*/
/**
\file       MOSTProtocolDefinitions.h

\ingroup	MOSTProtocol

\par        COPYRIGHT (c) 2007-2008 by K2L GmbH
            All rights reserved.

*/
/*============================================================================*/

#ifndef __MOSTPROTOCOLDEFINITIONS_H
#define __MOSTPROTOCOLDEFINITIONS_H

namespace k2l
{
    namespace acs
    {
        namespace base
        {
            namespace most
            {
                /// This is helper class that is used to read/write message flags inside 
                /// IACSMOSTProtocol::DataMessage() function.
                class CMessageFlags
                {
                private:
                    typedef enum MessageFlags_tag
                    {  
                    // \attention: if you change the flags here, please 
                    // adjust the code in CMOSTFunctionClassBase

                    // Function-class-wide flags
                        MESSAGE_SOURCE_NETWORK_SERVICES = 0x0001
                    ,   MESSAGE_SOURCE_FBLOCK =           0x0002
                    ,   MESSAGE_SOURCE_SHADOW =           0x0004

                    // Rx-only flags
                    //No flag is set when logical addressing
                    ,   ADDRESSING_TYPE_LOGICAL =         0x0000
                    ,   ADDRESSING_TYPE_PHYSICAL =        0x0010
                    ,   ADDRESSING_TYPE_BROADCAST =       0x0020
                    ,   ADDRESSING_TYPE_GROUPCAST =       0x0040
                    

                    // Operation-wide flags
                    ,   HIGH_FLAG =                       0x0100
                    ,   ASYNC_FLAG =                      0x0200
                    ,   PACKETIZER_FLAG =                 0x0400
					,   HIGH_FIRST_BLOCK =                0x1000
					,   HIGH_NEXT_BLOCK =                 0x2000
					,   HIGH_LAST_BLOCK =                 0x4000
                    } MessageFlags_t;
                public:
                    /// Test if message is sent from the network.
                    static inline bool IsFromNetwork(uint32_t flags) 
                    {
                        return ((flags & MESSAGE_SOURCE_NETWORK_SERVICES) == MESSAGE_SOURCE_NETWORK_SERVICES); 
                    }

                    /// Test if message is sent from the shadow.
                    static inline bool IsFromShadow(uint32_t flags) 
                    {
                        return ((flags & MESSAGE_SOURCE_SHADOW) == MESSAGE_SOURCE_SHADOW);
                    }

                    /// Test if message is sent from the function block.
                    static inline bool IsFromFunctionBlock(uint32_t flags)
                    {
                        return ((flags & MESSAGE_SOURCE_FBLOCK) == MESSAGE_SOURCE_FBLOCK);
                    }

                    /// Test if message uses logical addressing.
                    static inline bool IsLogicalAddressing(uint32_t flags)
                    {
                        return ((flags & (ADDRESSING_TYPE_PHYSICAL|ADDRESSING_TYPE_BROADCAST|ADDRESSING_TYPE_GROUPCAST)) == ADDRESSING_TYPE_LOGICAL);
                    }

                    /// Test if message uses physical addressing.
                    static inline bool IsPhysicalAddressing(uint32_t flags)
                    {
                        return ((flags & ADDRESSING_TYPE_PHYSICAL) == ADDRESSING_TYPE_PHYSICAL);
                    }

                    /// Test if message uses broadcast addressing.
                    static inline bool IsBroadcastAddressing(uint32_t flags)
                    {
                        return ((flags & ADDRESSING_TYPE_BROADCAST) == ADDRESSING_TYPE_BROADCAST);
                    }

                    /// Test if message uses groupcast addressing.
                    static inline bool IsGroupcastAddressing(uint32_t flags)
                    {
                        return ((flags & ADDRESSING_TYPE_GROUPCAST) == ADDRESSING_TYPE_GROUPCAST);
                    }

					/// Check if MOST high protocol is used.
                    static inline bool IsMOSTHighProtocol(uint32_t flags)
                    {
                        return ((flags & HIGH_FLAG) == HIGH_FLAG);
                    }

					/// Check if asynchronous channel is used.
                    static inline bool IsAsynchronousChannel(uint32_t flags)
                    {
                        return ((flags & ASYNC_FLAG) == ASYNC_FLAG);
                    }

					/// Check if packetizer is used.
                    static inline bool IsPacketizerMessage(uint32_t flags)
                    {
                        return ((flags & PACKETIZER_FLAG) == PACKETIZER_FLAG);
                    }
					
					/// Check if this is first block.
					static inline bool IsMOSTHighFirstBlock(uint32_t flags)
                    {
						return ((flags & HIGH_FIRST_BLOCK) == HIGH_FIRST_BLOCK);
                    }

					/// Check if this is next block.
					static inline bool IsMOSTHighNextBlock(uint32_t flags)
                    {
						return ((flags & HIGH_NEXT_BLOCK) == HIGH_NEXT_BLOCK);
                    }	

					/// Check if this is last block.
					static inline bool IsMOSTHighLastBlock(uint32_t flags)
                    {
						return ((flags & HIGH_LAST_BLOCK) == HIGH_LAST_BLOCK);
                    }					

                    /// Initialize flags for control message sent from the shadow.
                    static inline uint32_t ControlChannelFromShadow()
                    {
                        return static_cast<uint32_t>(MESSAGE_SOURCE_SHADOW);
                    }

                    /// Initialize flags for MOST high message over asynchronous channel from the shadow.
                    static inline uint32_t MOSTHighViaAsyncFromShadow() 
                    {
                        return 
                            static_cast<uint32_t>(MESSAGE_SOURCE_SHADOW) |
                            static_cast<uint32_t>(HIGH_FLAG) |
                            static_cast<uint32_t>(ASYNC_FLAG);
                    }

                    /// Initialize flags for control message sent from the function block.
                    static inline uint32_t ControlChannelFromFBlock() 
                    {
                        return static_cast<uint32_t>(MESSAGE_SOURCE_FBLOCK);
                    }

                    /// Initialize flags for MOST high message over asynchronous channel from the function block.
                    static inline uint32_t MOSTHighViaAsyncFromFBlock() 
                    {
                        return 
                            static_cast<uint32_t>(MESSAGE_SOURCE_FBLOCK) |
                            static_cast<uint32_t>(HIGH_FLAG) |
                            static_cast<uint32_t>(ASYNC_FLAG);
                    }
                    
                    /// Initialize flags for control message sent from the network.
                    static inline uint32_t ControlChannelFromNetwork() 
                    {
                        return static_cast<uint32_t>(MESSAGE_SOURCE_NETWORK_SERVICES);
                    }

                    /// Initialize flags for MOST high message over asynchronous channel from the network.
                    static inline uint32_t MOSTHighViaAsyncFromNetwork() 
                    {
                        return 
                            static_cast<uint32_t>(MESSAGE_SOURCE_NETWORK_SERVICES) |
                            static_cast<uint32_t>(HIGH_FLAG) |
                            static_cast<uint32_t>(ASYNC_FLAG);
                    }

					/// Get flag for control channel.
                    static inline uint32_t ControlChannel()
                    {
                        return static_cast<uint32_t>(0);
                    }

					/// Get flag for MOST high protocol.
                    static inline uint32_t MOSTHigh()
                    {
                        return 
                            static_cast<uint32_t>(HIGH_FLAG) |
                            static_cast<uint32_t>(ASYNC_FLAG);
                    }

					/// Get flag for FBlock as source.
                    static inline uint32_t FromFBlock()
                    {
                        return static_cast<uint32_t>(MESSAGE_SOURCE_FBLOCK);
                    }

					/// Get flag for Shadow as source.
                    static inline uint32_t FromShadow()
                    {
                        return static_cast<uint32_t>(MESSAGE_SOURCE_SHADOW);
                    }

					/// Get flag for Network as source.
                    static inline uint32_t FromNetwork()
                    {
                        return static_cast<uint32_t>(MESSAGE_SOURCE_NETWORK_SERVICES);
                    }

					/// Get flag for logical addressing.
                    static inline uint32_t LogicalAddressing()
                    {
                        return static_cast<uint32_t>(ADDRESSING_TYPE_LOGICAL);
                    }

					/// Get flag for physical addressing.
                    static inline uint32_t PhysicalAddressing()
                    {
                        return static_cast<uint32_t>(ADDRESSING_TYPE_PHYSICAL);
                    }

					/// Get flag for broadcast addressing.
                    static inline uint32_t BroadcastAddressing()
                    {
                        return static_cast<uint32_t>(ADDRESSING_TYPE_BROADCAST);
                    }

					/// Get flag for groupcast addressing.
                    static inline uint32_t GroupcastAddressing()
                    {
                        return static_cast<uint32_t>(ADDRESSING_TYPE_GROUPCAST);
                    }

					/// Get flag for packetizer on.
                    static inline uint32_t UsePacketizer()
                    {
                        return static_cast<uint32_t>(PACKETIZER_FLAG);
                    }
					
					/// Get flag for first MHP block.
                    static inline uint32_t MOSTHighFirstBlock()
                    {
                        return 
                            static_cast<uint32_t>(HIGH_FLAG) |
                            static_cast<uint32_t>(ASYNC_FLAG) |
							static_cast<uint32_t>(HIGH_FIRST_BLOCK);
                    }
					
					/// Get flag for next MHP block.
                    static inline uint32_t MOSTHighNextBlock()
                    {
                        return 
                            static_cast<uint32_t>(HIGH_FLAG) |
                            static_cast<uint32_t>(ASYNC_FLAG) |
							static_cast<uint32_t>(HIGH_NEXT_BLOCK);
                    }
					
					/// Get flag for last MHP block.
                    static inline uint32_t MOSTHighLastBlock()
                    {
                        return 
                            static_cast<uint32_t>(HIGH_FLAG) |
                            static_cast<uint32_t>(ASYNC_FLAG) |
							static_cast<uint32_t>(HIGH_LAST_BLOCK);
                    }					
                };
           
                /// Defines well known function IDs
                typedef enum WellKnownFunctions_tag
                {   FUNCTIONIDS =                     0x0000
                ,   NOTIFICATION =                    0x0001
                ,   NOTIFICATION_CHECK =              0x0002
                ,   MAX_FUNCTION_ID =                 0x0FFF
                } WellKnownFunctions_t;

                /// Define operation types
                typedef enum Operations_tag
                {   OPTYPE_SET =                       0x00
                ,   OPTYPE_GET =                       0x01
                ,   OPTYPE_SET_GET =                   0x02
                ,   OPTYPE_INCREMENT =                 0x03
                ,   OPTYPE_DECREMENT =                 0x04
                ,   OPTYPE_GET_INTERFACE =             0x05
                ,   OPTYPE_STATUS =                    0x0C
                ,   OPTYPE_INTERFACE =                 0x0E
                ,   OPTYPE_ERROR =                     0x0F
                ,   OPTYPE_START =                     0x00
                ,   OPTYPE_ABORT =                     0x01
                ,   OPTYPE_START_RESULT =              0x02
                ,   OPTYPE_START_RESULT_ACK =          0x06
                ,   OPTYPE_ABORT_ACK =                 0x07
                ,   OPTYPE_START_ACK =                 0x08
                ,   OPTYPE_ERROR_ACK =                 0x09
                ,   OPTYPE_PROCESSING_ACK =            0x0A
                ,   OPTYPE_PROCESSING =                0x0B
                ,   OPTYPE_RESULT =                    0x0C
                ,   OPTYPE_RESULT_ACK =                0x0D
                } Operations_t;

                /// Define common error codes
                typedef enum ErrorCodes_tag
                {   ERROR_FUNCTION_BLOCK_NOT_AVAILABLE = 0x01
                ,   ERROR_INSTANCE_ID_NOT_AVAILABLE =    0x02
                ,   ERROR_FUNCTION_ID_NOT_AVAILABLE =    0x03
                ,   ERROR_OPTYPE_NOT_AVAILABLE =         0x04
                ,   ERROR_INVALID_LENGTH =               0x05
                ,   ERROR_PARAMETER_OUT_OF_RANGE =       0x06
                ,   ERROR_PARAMETER_NOT_AVAILABLE =      0x07
                ,   ERROR_DEVICE_MALFUNCTION =           0x0B
                ,   ERROR_SEGMENTATION_ERROR =           0x0C
                ,   ERROR_FUNCTION_SPECIFIC =            0x20
                ,   ERROR_FUNCTION_BUSY =                0x40
                ,   ERROR_NOT_AVAILABLE =                0x41
                ,   ERROR_PROCESSING_ERROR =             0x42
                ,   ERROR_METHOD_ABORTED =               0x43
                } ErrorCodes_t;

                /// Function specifics error infos
                typedef enum FunctionSpecificErrorInfos_tag
                {
                    FUNCTION_SPECIFIC_ERROR_BUFFER_OVERFLOW        = 0x01,
                    FUNCTION_SPECIFIC_ERROR_LIST_OVERFLOW        = 0x02,
                    FUNCTION_SPECIFIC_ERROR_ELEMENT_OVERFLOW    = 0x03,
                    FUNCTION_SPECIFIC_ERROR_VALUE_NOT_AVAILABLE = 0x04,
                    FUNCTION_SPECIFIC_ERROR_ERROR_MATRIX        = 0x10
                } FunctionSpecificErrorInfos_t;

                /// Define notification control IDs
                typedef enum NotificationControl_tag
                {   NOTIFICATION_SETALL =          0x00
                ,   NOTIFICATION_SETFUNCTION =     0x01
                ,   NOTIFICATION_CLEARALL =        0x02
                ,   NOTIFICATION_CLEARFUNCTION =   0x03
                } NotificationControl_t;



                /// DeviceID specifics - like special values etc.
                class  CDeviceAddressInfo
                {
                public:
					/// Retrieve device ID wildcard used when sending message rom shadow to function block (0xFFFF).
                    static inline uint16_t ShadowToFunctionBlockWildcard() 
                    { return 0xFFFF; 
                    }

					/// Retrieve broadcast Device ID (0x3C8).
                    static inline uint16_t Broadcast() 
                    { return 0x3C8; 
                    }

					/// Retrieve invalid device ID (0). Please note that this is specific to the ACS implementation. It can be defined differently in other system.
                    static inline uint16_t InvalidDeviceID()
                    { return 0;
                    }

					/// Check if specified device ID is broadcast (0x3C8).
                    static inline bool IsBroadcast( uint16_t deviceID ) 
                    { return Broadcast() == deviceID; 
                    }

                    /// This function check only if deviceID is in group address range from
                    /// 0x300 - 0x3FF, but DOES NOT check if this is really group address
                    /// defined in the NetBlock function GroupAddress.
                    static inline bool IsInGroupAddressRange( uint16_t deviceID ) 
                    { return ((deviceID & 0xFF00) == 0x0300); 
                    }

					/// Check if wildcard addressing is used.
                    static inline bool IsShadowToFunctionBlockWildcard( uint16_t deviceID ) 
                    { return ShadowToFunctionBlockWildcard() == deviceID; 
                    }

					/// Check if device ID is invalid (only for ACS scope).
                    static inline bool IsInvalidDeviceID( uint16_t deviceID )
                    { return InvalidDeviceID() == deviceID;
                    }
                    
                    // TODO: Is the logical and absolute device addressing scheme (0x100 == logical, 0x400 == absolute) the same for all OEMs?
                    // TODO: Is there something generic and useful we can add here for groupcast thingies?
                };

				/// FBLockID specifics.
				class CFBlockIdHelper
				{
				private:

					typedef enum FBlockIdType_tag
					{
						FBLOCKID_ALL = 0xFF
					} FBlockIdType_t;

				public:

					/// Checks if the fblockID is a fblock addressing wildcard.
					static inline bool IsShadowToFunctionBlockWildcard( uint8_t fblockID )
					{ return fblockID == FBLOCKID_ALL;
					}
				};

				/// InstanceID specifics.
				class CInstanceIdHelper
				{
				private:

					typedef enum InstanceIdType_tag
					{
						INSTANCEID_ANY = 0x00
                    ,   INSTANCEID_ALL = 0xFF
					} InstanceIdType_t;

				public:

					/// Checks if the instanceID is a instance addressing wildcard.
					static inline bool IsShadowToFunctionBlockInstanceWildcard( uint8_t instanceID )
					{ return (instanceID == INSTANCEID_ANY) || (instanceID == INSTANCEID_ALL);
					}
                
					static inline bool IsShadowToFunctionBlockInstanceWildcardAny( uint8_t instanceID )
					{ return instanceID == INSTANCEID_ANY;
					}
                    
                    static inline bool IsShadowToFunctionBlockInstanceWildcardAll( uint8_t instanceID )
					{ return instanceID == INSTANCEID_ALL;
					}
				};

                /// Helper class for OperationTypes
                class COptypeHelper
                {
                    private:
                        /// Defintion for Operations Types. Range for Commands 0..8, Reports 9..F
                        typedef enum OperationsType_tag
                        {
                            OPTYPE_LAST_COMMAND = 0x08
                        } OperationsType_t;
                    public:
						/// Indicates if operation is in command group.
                        static inline bool IsOptypeCommand(uint8_t operation)
                        { return (operation <= OPTYPE_LAST_COMMAND); 
                        }

						/// Indicates if operation is in report group.
                        static inline bool IsOptypeReport(uint8_t operation)
                        { return (operation > OPTYPE_LAST_COMMAND); 
                        }
                };

                /// Represents the MOST state.
                class CMOSTStateFlags
                {
                    typedef enum MOSTStateFlags_tag
                    {   PHYSICAL_MOST_AVAILABLE = 0x0001,
                        DEVICE_ID_CHANGED = 0x0002
                    } MOSTStateFlags;
                public:
					/// Check if MOST is available.
                    static inline bool IsMOSTAvailable(uint32_t mostStateFlags) 
                    {
                        return ((mostStateFlags & PHYSICAL_MOST_AVAILABLE) == PHYSICAL_MOST_AVAILABLE);
                    }

					/// Check if device ID is changed.
                    static inline bool IsLocalDeviceIDChanged(uint32_t mostStateFlags)
                    {
                        return ((mostStateFlags & DEVICE_ID_CHANGED) == DEVICE_ID_CHANGED);
                    }

					/// Retrieve flag for MOST available.
                    static inline uint32_t MOSTAvailable()
                    {
                        return static_cast<uint32_t>(PHYSICAL_MOST_AVAILABLE);
                    }

					/// Retrieve flag for MOST not available.
                    static inline uint32_t MOSTNotAvailable()
                    {
                        return static_cast<uint32_t>(0);
                    }

					/// Retrieve flag for local device ID changed.
                    static inline uint32_t LocalDeviceIDChanged()
                    {
                        return static_cast<uint32_t>(DEVICE_ID_CHANGED);
                    }
                };

            }
        }
    }
}


#endif //__MOSTPROTOCOLDEFINITIONS_H

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
