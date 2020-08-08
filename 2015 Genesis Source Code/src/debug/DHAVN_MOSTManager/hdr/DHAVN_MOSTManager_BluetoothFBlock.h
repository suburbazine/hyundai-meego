/*=!AUTO-GENERATED!============================================================
*
*     This code was generated by K2L MAG V2.3.4 at 2012-09-06.
*
*     Changes to this file may cause incorrect behavior and will be lost if
*     the code is regenerated.
*     
*==!AUTO-GENERATED!============================================================*/

#ifndef __DHAVN_MOSTMANAGER_BLUETOOTHFBLOCK_H_
#define __DHAVN_MOSTMANAGER_BLUETOOTHFBLOCK_H_

/**
 * HMC DH Fcat v1.20.1
 **/

#include <stdint.h>
#include <K2LSystem.h>
#include <ACSStreams.h>
#include <K2LACSClient.h>
#include <K2LACSMOST.h>
#include <MOSTProtocolDefinitions.h>

namespace acs
{
    namespace generated
    {
        namespace fblock
        {
namespace Bluetooth
{

class CBluetooth;

class CVersion
{
	CBluetooth *m_pInstance;
public:
	explicit CVersion( CBluetooth *pInstance );
	~CVersion(void);

	void SetListener(::k2l::acs::protocols::most::IACSMOSTEventListener * pListener);
	bool RemoveListener(::k2l::acs::protocols::most::IACSMOSTEventListener *pListener);

	enum
	{
		FunctionID = 0x10 /* = 16 (decimal) */
	};

	::k2l::acs::protocols::most::CMOSTFunctionClassUnclassifiedProperty& FunctionInstance(void);
	bool MajorValue( uint8_t value );
uint8_t MajorValue() const;
::k2l::acs::protocols::most::CMOSTUnsignedByte& MajorDataType();
bool MinorValue( uint8_t value );
uint8_t MinorValue() const;
::k2l::acs::protocols::most::CMOSTUnsignedByte& MinorDataType();
bool BuildValue( uint8_t value );
uint8_t BuildValue() const;
::k2l::acs::protocols::most::CMOSTUnsignedByte& BuildDataType();
bool UpdateShadows();
bool Error( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint32_t payloadLength, const uint8_t *payload );
bool Error( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint8_t errorCode, uint32_t errorInfoLength, const uint8_t *pErrorInfo );            

};


class CBluetooth;

class CFBlockInfo
{
	CBluetooth *m_pInstance;
public:
	explicit CFBlockInfo( CBluetooth *pInstance );
	~CFBlockInfo(void);

	void SetListener(::k2l::acs::protocols::most::IACSMOSTEventListener * pListener);
	bool RemoveListener(::k2l::acs::protocols::most::IACSMOSTEventListener *pListener);

	enum
	{
		FunctionID = 0x11 /* = 17 (decimal) */
	};

	::k2l::acs::protocols::most::CMOSTFunctionClassUnclassifiedProperty& FunctionInstance(void);
	bool IDValue( uint16_t value );
uint16_t IDValue() const;
::k2l::acs::protocols::most::CMOSTUnsignedWord& IDDataType();
enum TFunctionMaturity
{
	FunctionMaturityUnknown = 0,
	FunctionMaturityInterfaceOnly = 1,
	FunctionMaturityPartlyImplemented = 2,
	FunctionMaturityFullyImplemented = 3,
	FunctionMaturityPartlyImplementedAndVerified = 17,
	FunctionMaturityFullyImplementedAndVerified = 18,
};
bool FunctionMaturityValue( acs::generated::fblock::Bluetooth::CFBlockInfo::TFunctionMaturity value );
acs::generated::fblock::Bluetooth::CFBlockInfo::TFunctionMaturity FunctionMaturityValue() const;
::k2l::acs::protocols::most::CMOSTEnum& FunctionMaturityDataType();
bool FBlockNameValue( uint8_t * value, uint32_t length );
uint8_t * FBlockNameValue() const;
::k2l::acs::protocols::most::CMOSTString& FBlockNameDataType();
uint32_t FBlockNameLength() const;
bool FBlockNameAddEncoding(uint8_t mostStringIdentifier, const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding );
bool FBlockNameString(const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding, const uint8_t* pStringContent, uint32_t stringLengthBytes );
bool FBlockNameString(const k2l::acs::protocols::most::string::IStringEncoding** ppStringEncoding, uint8_t** ppString, uint32_t* pStringLength );
bool SupplierVersionValue( uint8_t * value, uint32_t length );
uint8_t * SupplierVersionValue() const;
::k2l::acs::protocols::most::CMOSTString& SupplierVersionDataType();
uint32_t SupplierVersionLength() const;
bool SupplierVersionAddEncoding(uint8_t mostStringIdentifier, const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding );
bool SupplierVersionString(const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding, const uint8_t* pStringContent, uint32_t stringLengthBytes );
bool SupplierVersionString(const k2l::acs::protocols::most::string::IStringEncoding** ppStringEncoding, uint8_t** ppString, uint32_t* pStringLength );
bool FBlockVersionValue( uint8_t * value, uint32_t length );
uint8_t * FBlockVersionValue() const;
::k2l::acs::protocols::most::CMOSTString& FBlockVersionDataType();
uint32_t FBlockVersionLength() const;
bool FBlockVersionAddEncoding(uint8_t mostStringIdentifier, const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding );
bool FBlockVersionString(const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding, const uint8_t* pStringContent, uint32_t stringLengthBytes );
bool FBlockVersionString(const k2l::acs::protocols::most::string::IStringEncoding** ppStringEncoding, uint8_t** ppString, uint32_t* pStringLength );
bool MOSTVersionValue( uint8_t * value, uint32_t length );
uint8_t * MOSTVersionValue() const;
::k2l::acs::protocols::most::CMOSTString& MOSTVersionDataType();
uint32_t MOSTVersionLength() const;
bool MOSTVersionAddEncoding(uint8_t mostStringIdentifier, const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding );
bool MOSTVersionString(const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding, const uint8_t* pStringContent, uint32_t stringLengthBytes );
bool MOSTVersionString(const k2l::acs::protocols::most::string::IStringEncoding** ppStringEncoding, uint8_t** ppString, uint32_t* pStringLength );
bool SystemIntegratorValue( uint8_t * value, uint32_t length );
uint8_t * SystemIntegratorValue() const;
::k2l::acs::protocols::most::CMOSTString& SystemIntegratorDataType();
uint32_t SystemIntegratorLength() const;
bool SystemIntegratorAddEncoding(uint8_t mostStringIdentifier, const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding );
bool SystemIntegratorString(const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding, const uint8_t* pStringContent, uint32_t stringLengthBytes );
bool SystemIntegratorString(const k2l::acs::protocols::most::string::IStringEncoding** ppStringEncoding, uint8_t** ppString, uint32_t* pStringLength );
bool FBlockTypeValue( uint8_t * value, uint32_t length );
uint8_t * FBlockTypeValue() const;
::k2l::acs::protocols::most::CMOSTString& FBlockTypeDataType();
uint32_t FBlockTypeLength() const;
bool FBlockTypeAddEncoding(uint8_t mostStringIdentifier, const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding );
bool FBlockTypeString(const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding, const uint8_t* pStringContent, uint32_t stringLengthBytes );
bool FBlockTypeString(const k2l::acs::protocols::most::string::IStringEncoding** ppStringEncoding, uint8_t** ppString, uint32_t* pStringLength );
bool DescriptionValue( uint8_t * value, uint32_t length );
const uint8_t * DescriptionValue() const;
::k2l::acs::protocols::most::CMOSTCaseStream& DescriptionDataType();
uint32_t DescriptionStreamLength() const;
bool DescriptionLoadNext();
bool DescriptionSaveNext();
bool UpdateShadows();
bool Error( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint32_t payloadLength, const uint8_t *payload );
bool Error( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint8_t errorCode, uint32_t errorInfoLength, const uint8_t *pErrorInfo );            

};


class CBluetooth;

class CDialNumber
{
	CBluetooth *m_pInstance;
public:
	explicit CDialNumber( CBluetooth *pInstance );
	~CDialNumber(void);

	void SetListener(::k2l::acs::protocols::most::IACSMOSTEventListener * pListener);
	bool RemoveListener(::k2l::acs::protocols::most::IACSMOSTEventListener *pListener);

	enum
	{
		FunctionID = 0x250 /* = 592 (decimal) */
	};

	::k2l::acs::protocols::most::CMOSTFunctionClassSequenceMethod& FunctionInstance(void);
	bool SenderHandleValue( uint16_t value );
uint16_t SenderHandleValue() const;
::k2l::acs::protocols::most::CMOSTUnsignedWord& SenderHandleDataType();
bool CallIdValue( uint8_t value );
uint8_t CallIdValue() const;
::k2l::acs::protocols::most::CMOSTUnsignedByte& CallIdDataType();
bool ResultAck( ::k2l::acs::protocols::most::DeviceID_t targetDevice );
bool TelNumberValue( uint8_t * value, uint32_t length );
uint8_t * TelNumberValue() const;
::k2l::acs::protocols::most::CMOSTString& TelNumberDataType();
uint32_t TelNumberLength() const;
bool TelNumberAddEncoding(uint8_t mostStringIdentifier, const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding );
bool TelNumberString(const k2l::acs::protocols::most::string::IStringEncoding* pStringEncoding, const uint8_t* pStringContent, uint32_t stringLengthBytes );
bool TelNumberString(const k2l::acs::protocols::most::string::IStringEncoding** ppStringEncoding, uint8_t** ppString, uint32_t* pStringLength );
bool ParseStartResultAck( uint32_t payloadLength, const uint8_t * payload );
bool ParseStartAck( uint32_t payloadLength, const uint8_t * payload );
bool ProcessingAck( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint16_t senderHandle );
void SetHandler( ::k2l::acs::protocols::most::IFunctionBlockMethodHandler *pHandler );
bool ParseSenderHandle( uint32_t payloadLength, const uint8_t *payload, uint16_t *pSenderHandle );            
bool ErrorAck( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint16_t senderHandle, uint8_t errorCode, uint32_t errorInfoLength, const uint8_t *pErrorInfo );            

};


class CBluetooth;

class CHangupCall
{
	CBluetooth *m_pInstance;
public:
	explicit CHangupCall( CBluetooth *pInstance );
	~CHangupCall(void);

	void SetListener(::k2l::acs::protocols::most::IACSMOSTEventListener * pListener);
	bool RemoveListener(::k2l::acs::protocols::most::IACSMOSTEventListener *pListener);

	enum
	{
		FunctionID = 0x251 /* = 593 (decimal) */
	};

	::k2l::acs::protocols::most::CMOSTFunctionClassSequenceMethod& FunctionInstance(void);
	bool SenderHandleValue( uint16_t value );
uint16_t SenderHandleValue() const;
::k2l::acs::protocols::most::CMOSTUnsignedWord& SenderHandleDataType();
bool CallIdValue( uint8_t value );
uint8_t CallIdValue() const;
::k2l::acs::protocols::most::CMOSTUnsignedByte& CallIdDataType();
bool ParseStartResultAck( uint32_t payloadLength, const uint8_t * payload );
bool ParseStartAck( uint32_t payloadLength, const uint8_t * payload );
bool ResultAck( ::k2l::acs::protocols::most::DeviceID_t targetDevice );
bool ProcessingAck( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint16_t senderHandle );
void SetHandler( ::k2l::acs::protocols::most::IFunctionBlockMethodHandler *pHandler );
bool ParseSenderHandle( uint32_t payloadLength, const uint8_t *payload, uint16_t *pSenderHandle );            
bool ErrorAck( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint16_t senderHandle, uint8_t errorCode, uint32_t errorInfoLength, const uint8_t *pErrorInfo );            

};


class CBluetooth;

class CCallState
{
	CBluetooth *m_pInstance;
public:
	explicit CCallState( CBluetooth *pInstance );
	~CCallState(void);

	void SetListener(::k2l::acs::protocols::most::IACSMOSTEventListener * pListener);
	bool RemoveListener(::k2l::acs::protocols::most::IACSMOSTEventListener *pListener);

	enum
	{
		FunctionID = 0x253 /* = 595 (decimal) */
	};

	::k2l::acs::protocols::most::CMOSTFunctionClassArray& FunctionInstance(void);
	enum TDataCallState
{
	DataCallStateIdle = 0,
	DataCallStateRinging = 1,
	DataCallStateActive = 2,
	DataCallStateDialing = 3,
	DataCallStateDisconnecting = 4,
	TDataCallStateValue5 = 5,
	DataCallStateReestablish = 6,
	DataCallStateCallingRBT = 7,
	DataCallStateBusy = 8,
	DataCallStateMultipartyCallRinging = 9,
	DataCallStateMultipartyCallActive = 10,
	DataCallStateConferenceCall = 11,
	DataCallStateNotAcceptedCall = 12,
	DataCallStateInvalid = 255,
};
bool DataCallStateValue( uint8_t index, acs::generated::fblock::Bluetooth::CCallState::TDataCallState value );
acs::generated::fblock::Bluetooth::CCallState::TDataCallState DataCallStateValue( uint8_t index ) const;
::k2l::acs::protocols::most::CMOSTEnum& DataCallStateDataType( uint8_t index );
enum TDataCallType
{
	TDataCallTypeValue0 = 0,
	DataCallTypeEmergencyCall = 1,
	DataCallTypeInvalid = 255,
};
bool DataCallTypeValue( uint8_t index, acs::generated::fblock::Bluetooth::CCallState::TDataCallType value );
acs::generated::fblock::Bluetooth::CCallState::TDataCallType DataCallTypeValue( uint8_t index ) const;
::k2l::acs::protocols::most::CMOSTEnum& DataCallTypeDataType( uint8_t index );
::k2l::acs::protocols::most::CMOSTRecord& RecordDataType( uint8_t index );
uint32_t Size() const;
bool Size( uint32_t newSize ); 
bool UpdateShadows();
::k2l::acs::protocols::most::CMOSTArray& DataType( uint8_t index );
bool Error( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint32_t payloadLength, const uint8_t *payload );
bool Error( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint8_t errorCode, uint32_t errorInfoLength, const uint8_t *pErrorInfo );            

};


class CBluetooth;

class CAcceptCall
{
	CBluetooth *m_pInstance;
public:
	explicit CAcceptCall( CBluetooth *pInstance );
	~CAcceptCall(void);

	void SetListener(::k2l::acs::protocols::most::IACSMOSTEventListener * pListener);
	bool RemoveListener(::k2l::acs::protocols::most::IACSMOSTEventListener *pListener);

	enum
	{
		FunctionID = 0x255 /* = 597 (decimal) */
	};

	::k2l::acs::protocols::most::CMOSTFunctionClassSequenceMethod& FunctionInstance(void);
	bool SenderHandleValue( uint16_t value );
uint16_t SenderHandleValue() const;
::k2l::acs::protocols::most::CMOSTUnsignedWord& SenderHandleDataType();
bool CallIdValue( uint8_t value );
uint8_t CallIdValue() const;
::k2l::acs::protocols::most::CMOSTUnsignedByte& CallIdDataType();
bool ParseStartResultAck( uint32_t payloadLength, const uint8_t * payload );
bool ParseStartAck( uint32_t payloadLength, const uint8_t * payload );
bool ResultAck( ::k2l::acs::protocols::most::DeviceID_t targetDevice );
bool ProcessingAck( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint16_t senderHandle );
void SetHandler( ::k2l::acs::protocols::most::IFunctionBlockMethodHandler *pHandler );
bool ParseSenderHandle( uint32_t payloadLength, const uint8_t *payload, uint16_t *pSenderHandle );            
bool ErrorAck( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint16_t senderHandle, uint8_t errorCode, uint32_t errorInfoLength, const uint8_t *pErrorInfo );            

};


class CBluetooth;

class CCallHold
{
	CBluetooth *m_pInstance;
public:
	explicit CCallHold( CBluetooth *pInstance );
	~CCallHold(void);

	void SetListener(::k2l::acs::protocols::most::IACSMOSTEventListener * pListener);
	bool RemoveListener(::k2l::acs::protocols::most::IACSMOSTEventListener *pListener);

	enum
	{
		FunctionID = 0x260 /* = 608 (decimal) */
	};

	::k2l::acs::protocols::most::CMOSTFunctionClassUnclassifiedMethod& FunctionInstance(void);
	bool SenderHandleValue( uint16_t value );
uint16_t SenderHandleValue() const;
::k2l::acs::protocols::most::CMOSTUnsignedWord& SenderHandleDataType();
bool ParseStartResultAck( uint32_t payloadLength, const uint8_t * payload );
bool ParseStartAck( uint32_t payloadLength, const uint8_t * payload );
bool ResultAck( ::k2l::acs::protocols::most::DeviceID_t targetDevice );
bool ProcessingAck( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint16_t senderHandle );
void SetHandler( ::k2l::acs::protocols::most::IFunctionBlockMethodHandler *pHandler );
bool ParseSenderHandle( uint32_t payloadLength, const uint8_t *payload, uint16_t *pSenderHandle );            
bool ErrorAck( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint16_t senderHandle, uint8_t errorCode, uint32_t errorInfoLength, const uint8_t *pErrorInfo );            

};


class CBluetooth;

class CResumeCall
{
	CBluetooth *m_pInstance;
public:
	explicit CResumeCall( CBluetooth *pInstance );
	~CResumeCall(void);

	void SetListener(::k2l::acs::protocols::most::IACSMOSTEventListener * pListener);
	bool RemoveListener(::k2l::acs::protocols::most::IACSMOSTEventListener *pListener);

	enum
	{
		FunctionID = 0x261 /* = 609 (decimal) */
	};

	::k2l::acs::protocols::most::CMOSTFunctionClassUnclassifiedMethod& FunctionInstance(void);
	bool SenderHandleValue( uint16_t value );
uint16_t SenderHandleValue() const;
::k2l::acs::protocols::most::CMOSTUnsignedWord& SenderHandleDataType();
bool ParseStartResultAck( uint32_t payloadLength, const uint8_t * payload );
bool ParseStartAck( uint32_t payloadLength, const uint8_t * payload );
bool ResultAck( ::k2l::acs::protocols::most::DeviceID_t targetDevice );
bool ProcessingAck( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint16_t senderHandle );
void SetHandler( ::k2l::acs::protocols::most::IFunctionBlockMethodHandler *pHandler );
bool ParseSenderHandle( uint32_t payloadLength, const uint8_t *payload, uint16_t *pSenderHandle );            
bool ErrorAck( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint16_t senderHandle, uint8_t errorCode, uint32_t errorInfoLength, const uint8_t *pErrorInfo );            

};


class CBluetooth;

class CBTStatus
{
	CBluetooth *m_pInstance;
public:
	explicit CBTStatus( CBluetooth *pInstance );
	~CBTStatus(void);

	void SetListener(::k2l::acs::protocols::most::IACSMOSTEventListener * pListener);
	bool RemoveListener(::k2l::acs::protocols::most::IACSMOSTEventListener *pListener);

	enum
	{
		FunctionID = 0xC05 /* = 3077 (decimal) */
	};

	::k2l::acs::protocols::most::CMOSTFunctionClassEnumeration& FunctionInstance(void);
	enum TBTMode
{
	BTModeNotConnected = 0,
	BTModeConnecting = 1,
	BTModeBTHandsfree = 2,
	BTModeBTStreaming = 3,
	BTModeBTHF = 4,
	BTModeInvalid = 255,
};
bool UpdateShadows();
bool Value( acs::generated::fblock::Bluetooth::CBTStatus::TBTMode value );
acs::generated::fblock::Bluetooth::CBTStatus::TBTMode Value() const;
::k2l::acs::protocols::most::CMOSTEnum& DataType();
bool Error( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint32_t payloadLength, const uint8_t *payload );
bool Error( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint8_t errorCode, uint32_t errorInfoLength, const uint8_t *pErrorInfo );            

};


class CBluetooth;

class CUserMode
{
	CBluetooth *m_pInstance;
public:
	explicit CUserMode( CBluetooth *pInstance );
	~CUserMode(void);

	void SetListener(::k2l::acs::protocols::most::IACSMOSTEventListener * pListener);
	bool RemoveListener(::k2l::acs::protocols::most::IACSMOSTEventListener *pListener);

	enum
	{
		FunctionID = 0xC22 /* = 3106 (decimal) */
	};

	::k2l::acs::protocols::most::CMOSTFunctionClassSequenceProperty& FunctionInstance(void);
	enum TUserMode
{
	UserModeNotUsed = 0,
	UserModeHandsFreeModeFront = 1,
	UserModeHandsFreeModeRear = 2,
	UserModePrivateMode = 3,
	UserModeMICOff = 4,
	UserModeInvalid = 255,
};
bool UserModeValue( acs::generated::fblock::Bluetooth::CUserMode::TUserMode value );
acs::generated::fblock::Bluetooth::CUserMode::TUserMode UserModeValue() const;
::k2l::acs::protocols::most::CMOSTEnum& UserModeDataType();
bool MicOffValue( bool value );
bool MicOffValue() const;
::k2l::acs::protocols::most::CMOSTBoolean& MicOffDataType();
bool UpdateShadows();
bool Error( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint32_t payloadLength, const uint8_t *payload );
bool Error( ::k2l::acs::protocols::most::DeviceID_t targetDevice, uint8_t errorCode, uint32_t errorInfoLength, const uint8_t *pErrorInfo );            

};

class CBluetooth: public ::k2l::acs::protocols::most::CFunctionBlockBase
{
private:
	CBluetooth();
	CBluetooth(CBluetooth& toCopy);
public:
	enum
	{
		FBlockID = 0x54 /* = 84 (decimal) */
	};

	CBluetooth( ::k2l::acs::protocols::most::InstanceID_t instanceID );
	CBluetooth( ::k2l::acs::protocols::most::FBlockID_t fblockID,
		k2l::acs::protocols::most::InstanceID_t instanceID );
	virtual ~CBluetooth( void );

	virtual bool InitializeMOSTElement();
	inline ::k2l::acs::protocols::most::CWellknownFunctionNotificationCheck& NotificationCheckFunctionInstance() { return m_notificationCheckFunction; }
inline ::k2l::acs::protocols::most::CWellknownFunctionNotification& NotificationFunctionInstance() { return m_notificationFunction; }
inline ::k2l::acs::protocols::most::CWellknownFunctionFktIDs& FktIDsFunctionInstance() { return m_fktIDsFunction; }

private:
::k2l::acs::protocols::most::CMOSTFunctionClassUnclassifiedProperty m_versionFunction;
::k2l::acs::protocols::most::CMOSTUnsignedByte m_versionMajorParameter;
::k2l::acs::protocols::most::CMOSTUnsignedByte m_versionMinorParameter;
::k2l::acs::protocols::most::CMOSTUnsignedByte m_versionBuildParameter;
::k2l::acs::protocols::most::CMOSTFunctionClassUnclassifiedProperty m_fBlockInfoFunction;
::k2l::acs::protocols::most::CMOSTUnsignedWord m_fBlockInfoIDParameter;
::k2l::acs::protocols::most::CMOSTEnum m_fBlockInfoFunctionMaturityParameter;
::k2l::acs::protocols::most::CMOSTEnumSpecification m_fBlockInfoFunctionMaturityParameterSpec;
const static uint8_t m_fBlockInfoFunctionMaturityParameterSpecValues[6];
::k2l::acs::protocols::most::CMOSTRecord m_fBlockInfoDescription0RecordFixParameter;
::k2l::acs::protocols::most::CMOSTRecord m_fBlockInfoDescription0RecordIteratorParameter;
::k2l::acs::protocols::most::CMOSTList m_fBlockInfoDescription0Parameter;
::k2l::acs::protocols::most::CMOSTString m_fBlockInfoFBlockNameParameter;
::k2l::acs::protocols::most::CMOSTStringSpecification m_fBlockInfoFBlockNameParameterSpec;
::k2l::acs::protocols::most::CMOSTString m_fBlockInfoSupplierVersionParameter;
::k2l::acs::protocols::most::CMOSTStringSpecification m_fBlockInfoSupplierVersionParameterSpec;
::k2l::acs::protocols::most::CMOSTString m_fBlockInfoFBlockVersionParameter;
::k2l::acs::protocols::most::CMOSTStringSpecification m_fBlockInfoFBlockVersionParameterSpec;
::k2l::acs::protocols::most::CMOSTString m_fBlockInfoMOSTVersionParameter;
::k2l::acs::protocols::most::CMOSTStringSpecification m_fBlockInfoMOSTVersionParameterSpec;
::k2l::acs::protocols::most::CMOSTString m_fBlockInfoSystemIntegratorParameter;
::k2l::acs::protocols::most::CMOSTStringSpecification m_fBlockInfoSystemIntegratorParameterSpec;
::k2l::acs::protocols::most::CMOSTString m_fBlockInfoFBlockTypeParameter;
::k2l::acs::protocols::most::CMOSTStringSpecification m_fBlockInfoFBlockTypeParameterSpec;
::k2l::acs::protocols::most::CMOSTRecord m_fBlockInfoDescription61440RecordFixParameter;
::k2l::acs::protocols::most::CMOSTRecord m_fBlockInfoDescription61440RecordIteratorParameter;
::k2l::acs::protocols::most::CMOSTList m_fBlockInfoDescription61440Parameter;
::k2l::acs::protocols::most::CMOSTRecord m_fBlockInfoDescription61441RecordFixParameter;
::k2l::acs::protocols::most::CMOSTRecord m_fBlockInfoDescription61441RecordIteratorParameter;
::k2l::acs::protocols::most::CMOSTList m_fBlockInfoDescription61441Parameter;
::k2l::acs::protocols::most::CMOSTRecord m_fBlockInfoDescription61442RecordFixParameter;
::k2l::acs::protocols::most::CMOSTRecord m_fBlockInfoDescription61442RecordIteratorParameter;
::k2l::acs::protocols::most::CMOSTList m_fBlockInfoDescription61442Parameter;
::k2l::acs::protocols::most::CMOSTRecord m_fBlockInfoDescription61443RecordFixParameter;
::k2l::acs::protocols::most::CMOSTRecord m_fBlockInfoDescription61443RecordIteratorParameter;
::k2l::acs::protocols::most::CMOSTList m_fBlockInfoDescription61443Parameter;
::k2l::acs::protocols::most::CMOSTRecord m_fBlockInfoDescription61444RecordFixParameter;
::k2l::acs::protocols::most::CMOSTRecord m_fBlockInfoDescription61444RecordIteratorParameter;
::k2l::acs::protocols::most::CMOSTList m_fBlockInfoDescription61444Parameter;
::k2l::acs::protocols::most::CMOSTRecord m_fBlockInfoDescription61445RecordFixParameter;
::k2l::acs::protocols::most::CMOSTRecord m_fBlockInfoDescription61445RecordIteratorParameter;
::k2l::acs::protocols::most::CMOSTList m_fBlockInfoDescription61445Parameter;
::k2l::acs::protocols::most::CMOSTRecord m_fBlockInfoDescription61446RecordFixParameter;
::k2l::acs::protocols::most::CMOSTRecord m_fBlockInfoDescription61446RecordIteratorParameter;
::k2l::acs::protocols::most::CMOSTList m_fBlockInfoDescription61446Parameter;
::k2l::acs::protocols::most::CMOSTCaseStream m_fBlockInfoDescriptionParameter;
::k2l::acs::protocols::most::CMOSTFunctionClassSequenceMethod m_dialNumberFunction;
::k2l::acs::protocols::most::CMOSTUnsignedWord m_dialNumberSenderHandleParameter;
::k2l::acs::protocols::most::CMOSTUnsignedByte m_dialNumberCallIdParameter;
::k2l::acs::protocols::most::CNumberSpecificationUnsignedByte m_dialNumberCallIdParameterSpec;
::k2l::acs::protocols::most::CMOSTString m_dialNumberTelNumberParameter;
::k2l::acs::protocols::most::CMOSTStringSpecification m_dialNumberTelNumberParameterSpec;
::k2l::acs::protocols::most::CMOSTFunctionClassSequenceMethod m_hangupCallFunction;
::k2l::acs::protocols::most::CMOSTUnsignedWord m_hangupCallSenderHandleParameter;
::k2l::acs::protocols::most::CMOSTUnsignedByte m_hangupCallCallIdParameter;
::k2l::acs::protocols::most::CNumberSpecificationUnsignedByte m_hangupCallCallIdParameterSpec;
::k2l::acs::protocols::most::CMOSTFunctionClassArray m_callStateFunction;
::k2l::acs::protocols::most::CMOSTRecord m_callStateDataRecordParameter[255];
::k2l::acs::protocols::most::CMOSTEnum m_callStateDataCallStateParameter[255];
::k2l::acs::protocols::most::CMOSTEnumSpecification m_callStateDataCallStateParameterSpec;
const static uint8_t m_callStateDataCallStateParameterSpecValues[14];
::k2l::acs::protocols::most::CMOSTEnum m_callStateDataCallTypeParameter[255];
::k2l::acs::protocols::most::CMOSTEnumSpecification m_callStateDataCallTypeParameterSpec;
const static uint8_t m_callStateDataCallTypeParameterSpecValues[3];
::k2l::acs::protocols::most::CMOSTFunctionClassSequenceMethod m_acceptCallFunction;
::k2l::acs::protocols::most::CMOSTUnsignedWord m_acceptCallSenderHandleParameter;
::k2l::acs::protocols::most::CMOSTUnsignedByte m_acceptCallCallIdParameter;
::k2l::acs::protocols::most::CNumberSpecificationUnsignedByte m_acceptCallCallIdParameterSpec;
::k2l::acs::protocols::most::CMOSTFunctionClassUnclassifiedMethod m_callHoldFunction;
::k2l::acs::protocols::most::CMOSTUnsignedWord m_callHoldSenderHandleParameter;
::k2l::acs::protocols::most::CMOSTFunctionClassUnclassifiedMethod m_resumeCallFunction;
::k2l::acs::protocols::most::CMOSTUnsignedWord m_resumeCallSenderHandleParameter;
::k2l::acs::protocols::most::CMOSTFunctionClassEnumeration m_bTStatusFunction;
::k2l::acs::protocols::most::CMOSTEnum m_bTStatusBTModeParameter;
::k2l::acs::protocols::most::CMOSTEnumSpecification m_bTStatusBTModeParameterSpec;
const static uint8_t m_bTStatusBTModeParameterSpecValues[6];
::k2l::acs::protocols::most::CMOSTFunctionClassSequenceProperty m_userModeFunction;
::k2l::acs::protocols::most::CMOSTEnum m_userModeParameter;
::k2l::acs::protocols::most::CMOSTEnumSpecification m_userModeParameterSpec;
const static uint8_t m_userModeParameterSpecValues[6];
::k2l::acs::protocols::most::CMOSTBoolean m_userModeMicOffParameter;
::k2l::acs::protocols::most::CWellknownFunctionNotificationCheck m_notificationCheckFunction;
::k2l::acs::protocols::most::CWellknownFunctionNotification m_notificationFunction;
::k2l::acs::protocols::most::CWellknownFunctionFktIDs m_fktIDsFunction;
friend class CVersion;
friend class CFBlockInfo;
friend class CDialNumber;
friend class CHangupCall;
friend class CCallState;
friend class CAcceptCall;
friend class CCallHold;
friend class CResumeCall;
friend class CBTStatus;
friend class CUserMode;

};
}
        }
    }
}



#endif // __DHAVN_MOSTMANAGER_BLUETOOTHFBLOCK_H_
/*=============================================================================
 * END OF FILE
 *=============================================================================*/