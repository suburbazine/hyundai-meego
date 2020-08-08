/*
	File:       CarPlayAppFramework.c
	Package:    LG CarPlay Application Framework.
	Author:     Jaeheon Kim (jaeheon.kim@lge.com)
	Brief:      CarPlay Client API for the CarPlay Application developers.

	This contains the CarPlay Client for the CarPlay Application developers
	and eventually any macros, constants, or global variables you will need.

	Copyright © [2014-2016] by LG Electronics Inc.

	This program or software including the accompanying associated documentation
	(“Software”) is the proprietary software of LG Electronics Inc. and or its
	licensors, and may only be used, duplicated, modified or distributed pursuant
	to the terms and conditions of a separate written license agreement between you
	and LG Electronics Inc. (“Authorized License”). Except as set forth in an
	Authorized License, LG Electronics Inc. grants no license (express or implied),
	rights to use, or waiver of any kind with respect to the Software, and LG
	Electronics Inc. expressly reserves all rights in and to the Software and all
	intellectual property therein. If you have no Authorized License, then you have
	no rights to use the Software in any ways, and should immediately notify LG
	Electronics Inc. and discontinue all use of the Software.
*/

#ifndef CARPLAYAPPFRAMEWORK_H_
#define CARPLAYAPPFRAMEWORK_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

//===========================================================================================================================
//	Definitions
//===========================================================================================================================

#define kCarPlayVersionStr                      CARPLAY_VERSION_STR	/**< CarPlay 버전 문자열 "CarPlay.제품명.릴리즈날짜"로 구성됨, Makefile 내에 정의됨 */
#define kAppleCommPluginVerStr					kAirPlaySourceVersionStr	/**< Apple Communication Plug-in Source 버전 문자열 */

/*!
 * @defgroup CarPlayResult CarPlay Result
 * @brief CarPlay API 및 delegate Callback에서 사용되는 리턴 값을 정의
 * @{
 */
#define kCarPlayResult_OK						(0)					/**< 성공한 결과 */
#define kCarPlayResult_Error					(-1)				/**< 실패한 결과 */
/*! @} */

/*!
 * @defgroup CarPlayEntity CarPlay Entity
 * @brief CarPlay 리소스를 점유하는 엔티티의 타입과 엔티티 리스트를 정의
 * @{
 */
typedef uint32_t CarPlayEntity;										/**< CarPlay Resource를 점유하는 엔티티의 타입 */
#define kCarPlayEntity_NotApplicable			(0)					/**< CarPlay Resource를 점유하는 엔티티가 없음 */
#define kCarPlayEntity_Controller				(1)					/**< 컨트롤러(e.g. 애플 디바이스)가 CarPlay 리소스를 점유 */
#define kCarPlayEntity_Accessory				(2)					/**< 액서서리(e.g. 헤드 유닛)가 CarPlay 리소스를 점유 */
/*! @} */

/*!
 * @defgroup CarPlaySpeechMode CarPlay Speech Mode
 * @brief CarPlay의 Siri 동작 모드 타입과 리스트를 정의
 * @{
 */
typedef int32_t CarPlaySpeechMode;									/**< CarPlay의 Siri 동작 모드 타입을 정의 */
#define kCarPlaySpeechMode_NotApplicable		(0)					/**< CarPlay의 Siri 동작을 지원하지 않음 */
#define kCarPlaySpeechMode_None					(-1) 				/**< CarPlay의 Siri 동작 중이지 않음 */
#define kCarPlaySpeechMode_Speaking				(1)  				/**< CarPlay의 Siri 동작 중, 디바이스에서부터 사용자로 안내음 재생 중 */
#define kCarPlaySpeechMode_Recognizing			(2)  				/**< CarPlay의 Siri 동작 중, 디바이스는 사용자로부터의 음성을 인식 중 */
/*! @} */

/*!
 * @defgroup CarPlayTriState CarPlay TriState
 * @brief TriState 타입과 TriState 리스트를 정의
 * @{
 */
typedef int32_t CarPlayTriState;									/**< TriState 타입을 정의 */
#define kCarPlayTriState_NotApplicable			(0)					/**< 적용할 수 없는 타입 */
#define kCarPlayTriState_False					(-1)				/**< False 타입 */
#define kCarPlayTriState_True					(1)					/**< True 타입 */
/*! @} */

/*!
 * @defgroup CarPlayTransferType CarPlay Transfer Type
 * @brief Resource Ownership TransferType 타입과 TransferType 리스트를 정의
 * @{
 */
typedef int32_t CarPlayTransferType;								/**< Resource Ownership TransferType 타입을 정의 */
#define kCarPlayTransferType_NotApplicable		(0)					/**< Resource Ownership TransferType이 적용되지 않음 */
#define kCarPlayTransferType_Take				(1) 				/**< Resource의 Ownership을 영구이 획득 */
#define kCarPlayTransferType_Untake				(2) 				/**< 획득한 Resource를 더 이상 사용하지 않아 Ownership을 반환 */
#define kCarPlayTransferType_Borrow				(3) 				/**< Ownership을 임시로 빌림, 예를 들면 FM라디오 재생 중 Siri 실행 */
#define kCarPlayTransferType_Unborrow			(4) 				/**< 임시로 빌린 Resource를 반환 */
/*! @} */

/*!
 * @defgroup CarPlayTransferPriority CarPlay Transfer Priority
 * @brief Resource transfer priority 타입과 priority 리스트를 정의
 * @{
 */
typedef int32_t CarPlayTransferPriority;							/**< Resource transfer priority 타입을 정의 */
#define kCarPlayTransferPriority_NotApplicable	(0)					/**< 제약 조건이 적용되지 않음 */
#define kCarPlayTransferPriority_NiceToHave		(100) 				/**< 제약 조건이 AnyTime이하일 때, transfer 가능 */
#define kCarPlayTransferPriority_UserInitiated	(500)				/**< 제약 조건이 User Initiated 이하일 때, transfer 가능 */
/*! @} */

/*!
 * @defgroup CarPlayConstraint CarPlay Constraint
 * @brief CarPlay Resource의 제약 조건의 타입과 제약 조건 리스트를 정의
 * @{
 */
typedef int32_t CarPlayConstraint;									/**< CarPlay Resource의 제약 조건의 타입을 정의 */
#define kCarPlayConstraint_NotApplicable		(0)					/**< 제약 조건이 적용되지 않음 */
#define kCarPlayConstraint_Anytime				(100)				/**< 언제든지 Resource를 Take/Borrow 가능함 */
#define kCarPlayConstraint_UserInitiated		(500)				/**< 사용자가 초기화 한 경우 Resource를 Take/Borrow 가능함*/
#define kCarPlayConstraint_Never				(1000)				/**< 절대 Resource를 Take/Borrow 할 수 없음 */
/*! @} */

/*!
 * @defgroup CarPlayAudioFormat CarPlay Audio Format
 * @brief CarPlay에서 사용가능한 오디오 포멧의 타입과 오디오 포멧 리스트를 정의
 * @note
 * 		오디오 포멧 변수는 오디오 포멧의 조합으로 설정할 수 있다.\n
 * 		예)
 * @code
 * 		CarPlayAudioFormat audioFormat = 	kCarPlayAudioFormat_PCM_8KHz_16Bit_Mono
 * 										|	kCarPlayAudioFormat_PCM_16KHz_16Bit_Mono
 * 										|	kCarPlayAudioFormat_PCM_24KHz_16Bit_Mono;
 * @endcode
 * @{
 */
typedef uint64_t CarPlayAudioFormat;								/**< CarPlay에서 사용가능한 오디오 포멧의 타입을 정의 */
#define kCarPlayAudioFormat_Invalid						(0)			/**< 알수없는 오디오 타입 */
#define kCarPlayAudioFormat_Reserved1					(1 << 0)	/**< (0x00000001)\n Reserved1 */
#define kCarPlayAudioFormat_Reserved2					(1 << 1)	/**< (0x00000002)\n Reserved2 */
#define kCarPlayAudioFormat_PCM_8KHz_16Bit_Mono			(1 << 2)	/**< (0x00000004)\n Audio Format: PCM, 8KHz Sample Rate, 16Bits Bit per Channel, Mono */
#define kCarPlayAudioFormat_PCM_8KHz_16Bit_Stereo		(1 << 3)	/**< (0x00000008)\n Audio Format: PCM, 8KHz Sample Rate, 16Bits Bit per Channel, Stereo */
#define kCarPlayAudioFormat_PCM_16KHz_16Bit_Mono		(1 << 4)	/**< (0x00000010)\n Audio Format: PCM, 16KHz Sample Rate, 16Bits Bit per Channel, Mono */
#define kCarPlayAudioFormat_PCM_16KHz_16Bit_Stereo		(1 << 5)	/**< (0x00000020)\n Audio Format: PCM, 16KHz Sample Rate, 16Bits Bit per Channel, Stereo */
#define kCarPlayAudioFormat_PCM_24KHz_16Bit_Mono		(1 << 6)	/**< (0x00000040)\n Audio Format: PCM, 24KHz Sample Rate, 16Bits Bit per Channel, Mono */
#define kCarPlayAudioFormat_PCM_24KHz_16Bit_Stereo		(1 << 7)	/**< (0x00000080)\n Audio Format: PCM, 24KHz Sample Rate, 16Bits Bit per Channel, Stereo */
#define kCarPlayAudioFormat_PCM_32KHz_16Bit_Mono		(1 << 8)	/**< (0x00000100)\n Audio Format: PCM, 32KHz Sample Rate, 16Bits Bit per Channel, Mono */
#define kCarPlayAudioFormat_PCM_32KHz_16Bit_Stereo		(1 << 9)	/**< (0x00000200)\n Audio Format: PCM, 32KHz Sample Rate, 16Bits Bit per Channel, Stereo */
#define kCarPlayAudioFormat_PCM_44KHz_16Bit_Mono		(1 << 10)	/**< (0x00000400)\n Audio Format: PCM, 44KHz Sample Rate, 16Bits Bit per Channel, Mono */
#define kCarPlayAudioFormat_PCM_44KHz_16Bit_Stereo		(1 << 11)	/**< (0x00000800)\n Audio Format: PCM, 44KHz Sample Rate, 16Bits Bit per Channel, Stereo */
#define kCarPlayAudioFormat_PCM_44KHz_24Bit_Mono		(1 << 12)	/**< (0x00001000)\n Audio Format: PCM, 44KHz Sample Rate, 16Bits Bit per Channel, Mono */
#define kCarPlayAudioFormat_PCM_44KHz_24Bit_Stereo		(1 << 13)	/**< (0x00002000)\n Audio Format: PCM, 44KHz Sample Rate, 16Bits Bit per Channel, Stereo */
#define kCarPlayAudioFormat_PCM_48KHz_16Bit_Mono		(1 << 14)	/**< (0x00004000)\n Audio Format: PCM, 48KHz Sample Rate, 16Bits Bit per Channel, Mono */
#define kCarPlayAudioFormat_PCM_48KHz_16Bit_Stereo		(1 << 15)	/**< (0x00008000)\n Audio Format: PCM, 48KHz Sample Rate, 16Bits Bit per Channel, Stereo */
#define kCarPlayAudioFormat_PCM_48KHz_24Bit_Mono		(1 << 16)	/**< (0x00010000)\n Audio Format: PCM, 48KHz Sample Rate, 24Bits Bit per Channel, Mono */
#define kCarPlayAudioFormat_PCM_48KHz_24Bit_Stereo		(1 << 17)	/**< (0x00020000)\n Audio Format: PCM, 48KHz Sample Rate, 24Bits Bit per Channel, Stereo */
#define kCarPlayAudioFormat_ALAC_44KHz_16Bit_Stereo		(1 << 18)	/**< (0x00040000)\n Audio Format: ALAC, 44KHz Sample Rate, 16Bits Bit per Channel, Stereo */
#define kCarPlayAudioFormat_ALAC_44KHz_24Bit_Stereo		(1 << 19)	/**< (0x00080000)\n Audio Format: ALAC, 44KHz Sample Rate, 24Bits Bit per Channel, Stereo */
#define kCarPlayAudioFormat_ALAC_48KHz_16Bit_Stereo		(1 << 20)	/**< (0x00100000)\n Audio Format: ALAC, 48KHz Sample Rate, 16Bits Bit per Channel, Stereo */
#define kCarPlayAudioFormat_ALAC_48KHz_24Bit_Stereo		(1 << 21)	/**< (0x00200000)\n Audio Format: ALAC, 48KHz Sample Rate, 24Bits Bit per Channel, Stereo */
#define kCarPlayAudioFormat_AAC_LC_44KHz_Stereo			(1 << 22)	/**< (0x00400000)\n Audio Format: ACC_LC, 44KHz Sample Rate, Stereo */
#define kCarPlayAudioFormat_AAC_LC_48KHz_Stereo			(1 << 23)	/**< (0x00800000)\n Audio Format: ACC_LC, 48KHz Sample Rate, Stereo */
#define kCarPlayAudioFormat_AAC_ELD_44KHz_Stereo		(1 << 24)	/**< (0x01000000)\n Audio Format: AAC_ELD, 44KHz Sample Rate, Stereo */
#define kCarPlayAudioFormat_AAC_ELD_48KHz_Stereo		(1 << 25)	/**< (0x02000000)\n Audio Format: AAC_ELD, 48KHz Sample Rate, Stereo */
/*! @} */

/*!
 * @defgroup CarPlaylimitedUIElements CarPlay LimitedUI Elements
 * @brief CarPlay Command 중 setLimitedUI에서 사용되는 Element 타입과 limitedUI Element 리스트를 정의
 * @note
 * 		setLimitedUI 명령어로 limitedUI 값이 설정되면 limitedUIElement 리스트의 Element는 CarPlay에 표시되지 않는다.\n
 * 		limitedUIElement는 Element들의 조합으로 구성할 수 있다.\n
 * 		예)
 * 	@code
 * 		CarPlaylimitedUIElements limitedUIElement = kCarPlaylimitedUIElement_SoftKeyboard |
 * 													kCarPlaylimitedUIElement_SoftPhoneKeypad;
 * 	@endcode
 * @{
 */
typedef uint32_t CarPlaylimitedUIElements;							/**< CarPlay Command 중 setLimitedUI의 Element 타입을 정의 */
#define kCarPlaylimitedUIElement_Invalid				(0)			/**< 알수없는 limitedUI Element */
#define kCarPlaylimitedUIElement_Reserved1				(1 << 0)	/**< (0x00000001)\n Reserved1*/
#define kCarPlaylimitedUIElement_Reserved2				(1 << 1)	/**< (0x00000002)\n Reserved2*/
#define kCarPlaylimitedUIElement_SoftKeyboard			(1 << 2)	/**< (0x00000004)\n Software keyboard*/
#define kCarPlaylimitedUIElement_SoftPhoneKeypad		(1 << 3)	/**< (0x00000008)\n Software Phone Keypad*/
#define kCarPlaylimitedUIElement_NonMusicLists			(1 << 4)	/**< (0x00000010)\n Non Music Lists*/
#define kCarPlaylimitedUIElement_MusicLists				(1 << 5)	/**< (0x00000020)\n Music Lists*/
#define kCarPlaylimitedUIElement_JapanMaps				(1 << 6)	/**< (0x00000040)\n Minor roads in Japan*/
/*! @} */

/*!
 * @defgroup CarPlayCommand CarPlay Command
 * @brief CarPlay Command 타입과 Command 리스트를 정의
 * @note
 * 		Command는 애플 디바이스와 액서서리 간에 action을 수행하기 위해 교환되는 메시지이다.\n
 * 		Command는 /command URL을 사용한 HTTP reqeust로 전송된다.
 * @{
 */
typedef uint32_t CarPlayCommand;										/**< CarPlay Command 타입을 정의 */
#define kCarPlayCommand_Base				(0x00007000)				/**< CarPlay Command 값의 base 값 */
#define kCarPlayCommand_ConfigChanged  		(kCarPlayCommand_Base + 1)	/**< */
#define kCarPlayCommand_UpdateBonjour		(kCarPlayCommand_Base + 2)	/**< */
#define kCarPlayCommand_SetConfig			(kCarPlayCommand_Base + 3)	/**< */
#define kCarPlayCommand_ApplyConfig			(kCarPlayCommand_Base + 4)	/**< */
#define kCarPlayCommand_DuckAudio			(kCarPlayCommand_Base + 5)	/**< */
#define kCarPlayCommand_UnduckAudio			(kCarPlayCommand_Base + 6)	/**< */
#define kCarPlayCommand_FlushAudio			(kCarPlayCommand_Base + 7)	/**< */
#define kCarPlayCommand_DisableBluetooth	(kCarPlayCommand_Base + 8)	/**< */
#define kCarPlayCommand_UpdateFeedback		(kCarPlayCommand_Base + 9)	/**< */
#define kCarPlayCommand_HIDSendReport		(kCarPlayCommand_Base + 10)	/**< */
#define kCarPlayCommand_SetHIDReport		(kCarPlayCommand_Base + 11)	/**< */
#define kCarPlayCommand_HIDCopyInputMode	(kCarPlayCommand_Base + 12)	/**< */
#define kCarPlayCommand_HIDSetInputMode		(kCarPlayCommand_Base + 13)	/**< */
#define kCarPlayCommand_ForceKeyFrame		(kCarPlayCommand_Base + 14)	/**< */
#define kCarPlayCommand_ChangeModes			(kCarPlayCommand_Base + 15)	/**< */
#define kCarPlayCommand_ModesChanged		(kCarPlayCommand_Base + 16)	/**< */
#define kCarPlayCommand_UpdatePrefs			(kCarPlayCommand_Base + 17)	/**< */
#define kCarPlayCommand_RequestSiri			(kCarPlayCommand_Base + 18)	/**< */
#define kCarPlayCommand_RequestUI			(kCarPlayCommand_Base + 19)	/**< */
#define kCarPlayCommand_SetNightMode		(kCarPlayCommand_Base + 20)	/**< */
#define kCarPlayCommand_SetLimitedUI		(kCarPlayCommand_Base + 21)	/**< */
#define kCarPlayCommand_StartServer			(kCarPlayCommand_Base + 22)	/**< */
#define kCarPlayCommand_StopServer			(kCarPlayCommand_Base + 23)	/**< */
#define kCarPlayCommand_SessionDied			(kCarPlayCommand_Base + 24)	/**< */
#define kCarPlayCommand_StartSession		(kCarPlayCommand_Base + 25)	/**< */
#define kCarPlayCommand_StopSession			(kCarPlayCommand_Base + 26)	/**< */
#define kCarPlayCommand_SetUpStreams		(kCarPlayCommand_Base + 27)	/**< */
#define kCarPlayCommand_TearDownStreams		(kCarPlayCommand_Base + 28)	/**< */
#define kCarPlayCommand_UpdateTimestamps	(kCarPlayCommand_Base + 29)	/**< */
/*! @} */

/*!
 * @defgroup CarPlayOwnershipResource CarPlay Ownership Resource
 * @brief CarPlay에서 Ownership에 사용되는 Resource 타입과 리스트 정의
 * @{
 */
typedef uint32_t CarPlayOwnershipResource;												/**< CarPlay에서 Ownership에 사용되는 Resource 타입 */
#define kCarPlayOwnershipResource_Base		(0x00008100)								/**< CarPlay에서 Ownership에 사용되는 Resource 리스트의 Baseline 값 */
#define kCarPlayOwnershipResource_Screen	(kCarPlayOwnershipResource_Base + (1 << 0))	/**< Screen Resource */
#define kCarPlayOwnershipResource_MainAudio	(kCarPlayOwnershipResource_Base + (1 << 1))	/**< Main Audio Resource */
/*! @} */

/*!
 * @defgroup CarPlayAppState CarPlay AppState
 * @brief CarPlay에서 애플디바이스와 액서서리 간 통신할 때의 사용되는 App State 타입과 리스트 정의
 * @note
 * 		애플 디바이스와 액서서리 간 통신할 때에는 리스트에 있는 App State를 항상 전송되어야 한다.\n
 * 		각 state에 대한 값은 @ref CarPlaySpeechMode 또는 @ref CarPlayTriState 를 사용할 수 있다.
 * @{
 */
typedef uint32_t CarPlayAppState;										/**< CarPlay 통신시 사용되는 App State 타입을 정의 */
#define kCarPlayAppState_Base				(0x00008200)				/**< CarPlay 통신시 사용되는 App State 타입의 baseline 값 */
#define kCarPlayAppState_PhoneCall			(kCarPlayAppState_Base + 1) /**< 전화 통화 중인 상태 */
#define kCarPlayAppState_SpeechMode			(kCarPlayAppState_Base + 2) /**< 시리 입력과 같은 음성인식 상태 */
#define kCarPlayAppState_TurnByTurn			(kCarPlayAppState_Base + 3) /**< 네비게이션 동작 상태 */
#define kCarPlayAppState_ResetState			(kCarPlayAppState_Base + 4) /**< 리셋 상태 */
/*! @} */

/*!
 * @defgroup CarPlaySiriAction CarPlay Siri Action
 * @brief CarPlay에서 사용자가 시리를 동작 시킬 때 함께 전달하는 시리의 동작의 타입과 리스트 정의
 * @{
 */
typedef uint32_t CarPlaySiriAction;											/**< 시리 동작의 타입을 정의 */
#define kCarPlaySiriAction_Base				(0x00008300)					/**< 시리 동작 타입의 Baseline 값 */
#define kCarPlaySiriAction_NotApplicable	(kCarPlaySiriAction_Base + 1)	/**< 알 수 없는 시리 동작 */
#define kCarPlaySiriAction_Prewarm			(kCarPlaySiriAction_Base + 2)	/**< 애플디바이스가 시리를 준비시키도록 지시하는 동작 */
#define kCarPlaySiriAction_ButtonPressed	(kCarPlaySiriAction_Base + 3)	/**< 애플디바이스로 시리 버튼이 눌러졌음을 알리는 동작 */
#define kCarPlaySiriAction_ButtonReleased	(kCarPlaySiriAction_Base + 4)	/**< 애플디바이스로 시리 버튼이 풀렸음을 알리는 동작 */
/*! @} */

/*!
 * @defgroup CarPlayAudioType CarPlay Audio Type
 * @brief CarPlay의 오디오 종류의 타입과 리스트를 정의
 * @{
 */
typedef uint32_t CarPlayAudioType;											/**< CarPlay의 오디오 종류의 타입을 정의 */
#define kCarPlayAudioType_Base				(0x00008400)					/**< CarPlay의 오디오 종류의 Baseline 값 */
#define kCarPlayAudioType_Invalid			(kCarPlayAudioType_Base + 0)	/**< 확실하지 않은 오디오 타입 */
#define kCarPlayAudioType_Default			(kCarPlayAudioType_Base + 1)	/**< 기본 오디오 타입 */
#define kCarPlayAudioType_Alert				(kCarPlayAudioType_Base + 2)	/**< 알림 오디오 타입 */
#define kCarPlayAudioType_Media				(kCarPlayAudioType_Base + 3)	/**< 미디어 오디오 타입 */
#define kCarPlayAudioType_Telephony			(kCarPlayAudioType_Base + 4)	/**< 전화 통화 오디오 타입 */
#define kCarPlayAudioType_SpeechRecognition	(kCarPlayAudioType_Base + 5)	/**< 음성 인식 오디오 타입 */
#define kCarPlayAudioType_Measurement		(kCarPlayAudioType_Base + 6)	/**< 측정 오디오 타입 */
#define kCarPlayAudioType_SpokenAudio		(kCarPlayAudioType_Base + 7)	/**< 안내음 재생 오디오 타입 */
/*! @} */

/*!
 * @defgroup CarPlayStreamType CarPlay Stream Type
 * @brief CarPlay의 스트림 종류의 타입과 리스트를 정의
 * @{
 */
typedef uint32_t CarPlayStreamType;											/**< CarPlay의 스트림 종류의 타입을 정의 */
#define kCarPlayStreamType_Base				(0x00008500)					/**< CarPlay의 스트림 종류의 Baseline 값 */
#define kCarPlayStreamType_Invalid			(kCarPlayStreamType_Base + 0)	/**< 확실하지 않은 스트림 타입 */
#define kCarPlayStreamType_GeneralAudio		(kCarPlayStreamType_Base + 1)	/**< 일반 오디오 스트림 타입 */
#define kCarPlayStreamType_MainAudio		(kCarPlayStreamType_Base + 2)	/**< 메인 오디오 스트림 타입 */
#define kCarPlayStreamType_AltAudio			(kCarPlayStreamType_Base + 3)	/**< 보조 오디오 스트림 타입 */
#define kCarPlayStreamType_Screen			(kCarPlayStreamType_Base + 4)	/**< 스크린 스트림 타입 */
/*! @} */

/*!
 * @defgroup CarPlayPrimaryInputDevice CarPlay Primary Input Device
 * @brief CarPlay의 Primary Input Device 타입을 정의
 * @{
 */
#define kCarPlayPrimaryInputDevice_TouchScreen  (0x01)                      /**< 터치 스크린을 Priamry 입력 장치로 사용함 */
#define kCarPlayPrimaryInputDevice_TouchPad     (0x02)                      /**< 터치 패드를 Priamry 입력 장치로 사용함 */
#define kCarPlayPrimaryInputDevice_Knob         (0x03)                      /**< 노브를 Priamry 입력 장치로 사용함 */
/*! @} */

/*!
 * @defgroup CarPlayStreamReady CarPlay Stream Device Ready Type
 * @brief CarPlay의 스트림 사용할 준비됨의 타입을 정의
 * @{
 */
typedef int32_t CarPlayStreamReadyType;										/**< CarPlay의 스트림 사용할 준비됨의 타입을 정의 */
#define kCarPlayStreamReadyType_Ready		(0)								/**< CarPlay 스트림 사용할 준비가 되었음 */
#define kCarPlayStreamReadyType_NotReady	(-1)							/**< CarPlay 스트림 사용할 준비가 되지 않았음 */
/*! @} */

/*!
 * @struct CarPlayAppFrameworkModeState
 * @brief CarPlay Mode State 구조체로 각 엔티티의 owner를 저장한다.\n
 * modeChanged Command를 수신 받았을 때 업데이트하기 위한 모드를 저장할 때 사용됨.
 */
typedef struct {
	CarPlayEntity		screen;							/**< Screen의 오너 값 저장되는 변수\n @ref CarPlayEntity 중 하나의 값을 가짐 */
	CarPlayEntity		mainAudio;						/**< Main Audio의 오너 값 저장되는 변수\n @ref CarPlayEntity 중 하나의 값을 가짐 */
	CarPlayEntity		phoneCall;						/**< 전화 통화 모드의 오너 값 저장되는 변수\n @ref CarPlayEntity 중 하나의 값을 가짐 */
	CarPlayEntity		turnByTurn;						/**< 네비게이션 모드의 오너 값 저장되는 변수\n @ref CarPlayEntity 중 하나의 값을 가짐 */
	CarPlayEntity		speech;							/**< 음성 인식 모드의 오너 값 저장되는 변수\n @ref CarPlayEntity 중 하나의 값을 가짐 */
	CarPlaySpeechMode	speechMode;						/**< 음성 인식 모드의 동작 모드 값 저장되는 변수\n @ref CarPlaySpeechMode 중 하나의 값을 가짐 */
} CarPlayAppFrameworkModeState;

/*!
 * @struct CarPlayAppFrameworkResourceChange
 * @brief CarPlay의 리소스 변경에 사용되는 멤버로 구성된 구조체
 */
typedef struct
{
	CarPlayTransferType		type;						/**< 리소스 전송 타입\n @ref CarPlayTransferType 중 하나의 값을 가짐 */
	CarPlayTransferPriority	priority;					/**< 리소스 전송 타입이 Take 또는 borrow일 때, 리소스 전송 시 우선 순위 설정.\n @ref CarPlayTransferPriority 중 하나의 값을 가짐 */
	CarPlayConstraint		takeConstraint;				/**< 리소스 전송 타입이 Take일 때 사용되어야 하며, 상대편의 take 요청에 대한 제약 조건 설정\n @ref CarPlayConstraint 중 하나의 값을 가짐 */
	CarPlayConstraint		borrowOrUnborrowConstraint;	/**< 리소스 전송 타입이 Take일 때 사용되어야 하며, 상대편의 borrow 요청에 대한 제약 조건 설정\n
														 * 리소스 전송 타입이 Borrow 일 때 사용되어야 하며, 상대편의 unborrow 요청에 대한 제약 조건 설정\n
														 * @ref CarPlayConstraint 중 하나의 값을 가짐 */
} CarPlayAppFrameworkResourceChange;

/*!
 * @struct CarPlayAppFrameworkModeChanges
 * @brief CarPlay의 모드 변경에 사용되는 멤버로 구성된 구조체\n
 * changeModes command를 전송할 때 사용됨.
 */
typedef struct
{
	CarPlayAppFrameworkResourceChange	screen;			/**< Screen 리소스를 변경하기 위해 설정하는 변수 */
	CarPlayAppFrameworkResourceChange	mainAudio;		/**< Main Audio 리소스를 변경하기 위해 설정하는 변수 */
	CarPlaySpeechMode					speech;			/**< 음성 인식의 모드를 변경하기 위해 설정하는 변수 */
	CarPlayTriState						phoneCall;		/**< 전화 통화의 모드를 변경하기 위해 설정하는 변수 */
	CarPlayTriState						turnByTurn;		/**< 네비게이션의 모드를 변경하기 위해 설정하는 변수 */
} CarPlayAppFrameworkModeChanges;

/*!
 * @struct CarPlayAudioStreamDescription
 * @brief 현재 CarPlay에서 사용되고 있는 세션의 오디오 스트림 정보를 멤버로 구성된 구조체
 */
typedef struct
{
	uint32_t		mSampleRate;						/**< 오디오 스트림의 샘플 레이트 */
	uint32_t		mBytesPerPacket;					/**< 오디오 스트림의 패킷 당 바이트 수 */
	uint32_t		mFramesPerPacket;					/**< 오디오 스트림의 패킷 당 프레임 수 */
	uint32_t		mBytesPerFrame;						/**< 오디오 스트림의 프레임 당 바이트 수 */
	uint32_t		mChannelsPerFrame;					/**< 오디오 스트림의 프레임 당 채널 수 */
	uint32_t		mBitsPerChannel;					/**< 오디오 스트림의 채널 당 비트 수 */
} CarPlayAudioStreamDescription;

/*!
 * @defgroup CarPlayAppFrameworkDelegate CarPlayAppFramwork Callback Functions
 * @brief CarPlay에서 Callback으로 호출되는 함수들을 정의하고 그것들의 delegate를 정의함.
 * @{
 */

/*!
 * @brief CarPlay의 AirPlay Server가 실행되면 호출되는 Callback 함수
 * @param inServer 실행된 서버
 * @return
 */
typedef int32_t (*CarPlayAppFrameworkServerStarted_f)(void *inServer);
/*!
 * @brief CarPlay의 세션이 생성되면 호출되는 Callback 함수
 * @param inSession 연결된 세션
 * @return
 */
typedef int32_t (*CarPlayAppFrameworkSessionCreated_f)(void *inSession);
/*!
 * @brief CarPlay의 세션이 종료되면 호출되는 Callback 함수
 * @param inSession 종료된 세션
 * @return
 */
typedef int32_t (*CarPlayAppFrameworkSessionFinalized_f)(void *inSession);
/*!
 * @brief CarPlay 세션 내에서 Command 전송과 같은 Control을 하였을 경우 호출되는 CallBack 함수
 * @param inSession 연결된 세션
 * @param inRequest 전송된 Command
 * @return
 */
typedef int32_t (*CarPlayAppFrameworkSessionControl_f)(void *inSession, CarPlayCommand inRequest);
/*!
 * @brief 애플 디바이스로부터 modeChanged Command가 수신되면 호출되는 Callback 함수
 * @param inSession 연결된 세션
 * @param inState 애플 디바이스로부터 modeChanged로 업데이트 요구된 Mode의 상태. @ref CarPlayAppFrameworkModeState 구조로 전달.
 * @return
 */
typedef int32_t (*CarPlayAppFrameworkModesChanged_f)(void *inSession,
	CarPlayAppFrameworkModeState *inState);
/*!
 * @brief 애플 디바이스로부터 RequestUI command가 수신되면 호출되는 Callback 함수
 * @param inSession 연결된 세션
 * @return
 */
typedef int32_t (*CarPlayAppFrameworkRequestUI_f)(void *inSession);
/*!
 * @brief 애플 디바이스로부터 DuckAudio command가 수신되면 호출되는 Callback 함수
 * @param inSession 연결된 세션
 * @param inDurationSecs ducking을 위해 볼륨이 ramp되는(점점 낮아지는) 시간(millisecond 단위)
 * @param inVolume ducking되는 볼륨의 레벨
 * @return
 */
typedef int32_t (*CarPlayAppFrameworkDuckAudio_f)(void *inSession, double inDurationSecs, double inVolume);
/*!
 * @brief 애플 디바이스로부터 DuckAudio command가 수신되면 호출되는 Callback 함수
 * @param inSession 연결된 세션
 * @param inDurationSecs unducking을 위해 볼륨이 ramp되는(점점 높아지는) 시간(millisecond 단위)
 * @return
 */
typedef int32_t (*CarPlayAppFrameworkUnduckAudio_f)(void *inSession, double inDurationSecs);
/*!
 * @brief 애플 디바이스에서 블루투스 해제(disableBluetooth)를 수신하였을 때, 호출되는 Callback 함수.
 * @param inSession 연결된 세션
 * @param inDeviceID 블루투스 Mac 어드레스
 * @return
 */
typedef void (*CarPlayAppFrameworkDisableBluetooth_f)(void *inSession, const char * inDeviceID);
/*!
 * @brief 액서서리가 ChangeModes command를 전송하고 난 뒤, 애플 디바이스로부터 response가 수신되면 호출되는 Callback 함수
 * @param inSession 연결된 세션
 * @param inResult 수신 결과의 성공 유무, @ref kCarPlayResult_OK 와 @ref kCarPlayResult_Error 중 하나의 값으로 전달됨
 * @param inState 업데이트 된 모드 상태를 전달, @ref CarPlayAppFrameworkModeState 타입으로 전달됨
 * @param inContext @ref CarPlayAppFrameworkRef 전달.
 */
typedef void 	(*CarPlayAppFrameworkChangeModesComplete_f)(void *inSession, int32_t inResult, CarPlayAppFrameworkModeState *inState, void *inContext);
/*!
 * @brief 오디오 스트림이 시작될 때 호출되는 CallBack 함수
 * @param inSession 연결된 세션
 * @param inStream  시작된 오디오 스트림 핸들
 * @param inStreamType 오디오 스트림의 타입, @ref CarPlayStreamType 중 하나의 값으로 전달됨
 * @param inAudioType 오디오 타입, @ref CarPlayStreamType 중 하나의 값으로 전달됨
 * @param inFormat 오디오 스트림의 오디오 포멧 @ref CarPlayAudioStreamDescription 형식으로 값이 전달됨
 * @return
 */
typedef int32_t (*CarPlayAppFrameworkAudioStreamStarted_f)(void *inSession, void *inStream,
	CarPlayStreamType inStreamType, CarPlayAudioType inAudioType,
	CarPlayAudioStreamDescription *inFormat);
/*!
 * @brief 오디오 스트림이 종료될 때 호출되는 CallBack 함수
 * @param inSession 연결된 세션
 * @param inStream 종료된 오디오 스트림 핸들
 * @param inStreamType 오디오 스트림의 타입, @ref CarPlayStreamType 중 하나의 값으로 전달됨
 * @param inAudioType 오디오 타입, @ref CarPlayStreamType 중 하나의 값으로 전달됨
 * @return
 */
typedef int32_t (*CarPlayAppFrameworkAudioStreamStopped_f)(void *inSession, void *inStream,
    CarPlayStreamType inStreamType, CarPlayAudioType inAudioType);
/*!
 * @brief 오디오 스트림이 torn down될 때 호출되는 CallBack 함수
 * @param inSession 연결된 세션
 * @return
 */
typedef int32_t (*CarPlayAppFrameworkAudioSessionTornDown_f)(void *inSession);
/*!
 * @brief 스크린 스트림이 시작될 때 호출되는 CallBack 함수
 * @param inSession 연결된 세션
 * @param inStream 시작된 스크린 스트림 핸들
 * @return
 */
typedef int32_t (*CarPlayAppFrameworkScreenStreamStarted_f)(void *inSession, void *inStream);
/*!
 * @brief 스크린 스트림이 재생될 때 호출되는 CallBack 함수
 * @param inSession 연결된 세션
 * @param inStream 재생이 시작된 스크린 스트림 핸들
 * @return
 */
typedef int32_t (*CarPlayAppFrameworkScreenStreamPlayed_f)(void *inSession, void *inStream);
/*!
 * @brief 스크린 스트림이 종료될 때 호출되는 CallBack 함수
 * @param inSession 연결된 세션
 * @param inStream 종료된 스크린 스트림 핸들
 * @return
 */
typedef int32_t (*CarPlayAppFrameworkScreenStreamStopped_f)(void *inSession, void *inStream);

#if defined(KOECN)
typedef int32_t (*CarPlayAppFrameworkScreenStreamProcessData_f)(void *inSession, void *inStream, uint8_t *data, int32_t size);
#endif

/*!
 * @brief 오디오/스크린 디바이스가 사용가능한지 확인하기 위해 호출되는 Callback 함수
 * @param inSession 연결된 세션
 * @param inStreamType 사용하고자 하는 스트림의 타입, @ref inStreamType 중 하나가 사용됨.
 * @return 0 : 디바이스를 사용할 수 없음\n 1 : 디바이스를 사용할 수 있음.
 */
typedef int32_t (*CarPlayAppFrameworkCopyDeviceAvailable_f)(void *inSession, CarPlayStreamType inStreamType);
/*!
 * @brief 오디오/스크린 디바이스가 오류가 발생하였을 때 호출되는 Callback 함수
 * @param inSession 연결된 세션
 * @param inStreamType 사용하고자 하는 스트림의 타입, @ref inStreamType 중 하나가 사용됨.
 * @param inErrorCode 에러 코드
 * @note
 * 		ScreenStream에 오류가 난 경우에는 Screen Device의 권한을 Take하도록 하고, CarPlay 연결을 다시 하는 것을 권장한다.
 * @return
 */
typedef int32_t (*CarPlayAppFrameworkReportDeviceError_f)(void *inSession, CarPlayStreamType inStreamType, int32_t inErrorCode);
/*!
 * @brief 애플 디바이스에서 설정 요청(Setup Request)를 수신하였을 때, 호출되는 Callback 함수.
 * @param inSession 연결된 세션
 * @param inStreamType 사용하고자 하는 스트림의 타입, @ref CarPlayStreamType 중 하나가 사용됨.
 * @param inAudioType 오디오의 타입, @ref CarPlayAudioType 중 하나가 사용됨.
 * @param inInput 입력 오디오 사용 유무.
 * @param inFormat 오디오 스트림의 오디오 포멧 @ref CarPlayAudioStreamDescription 형식으로 값이 전달됨.
 * @return @ref CarPlayStreamReadyType 중 하나가 리턴됨.\n
 *         @ref kCarPlayStreamReadyType_Ready 스트림 사용할 준비가 되었음.\n
 *         @ref kCarPlayStreamReadyType_NotReady 스트림 사용할 준비가 되지 않았음.
 */
typedef int32_t (*CarPlayAppFrameworkSetupRequest_f)(void *inSession, CarPlayStreamType inStreamType, CarPlayAudioType inAudioType,
                                                     int32_t inInput, CarPlayAudioStreamDescription * inFormat);

/*! @} */

/*!
 * @struct CarPlayAppFrameworkDelegate
 * @brief CarPlay를 사용하기 위해 CallBack으로 등록해주어야 하는 함수들로 구성된 Delegate의 구조체
 */
typedef struct {
	void *											context;
	CarPlayAppFrameworkServerStarted_f				serverStarted_f;			/**< CarPlay의 AirPlay Server가 실행되면 호출되는 Callback 함수 */
	CarPlayAppFrameworkSessionCreated_f				sessionCreated_f;			/**< CarPlay의 세션이 생성되면 호출되는 Callback 함수 */
	CarPlayAppFrameworkSessionFinalized_f			sessionFinalized_f;			/**< CarPlay의 세션이 종료되면 호출되는 Callback 함수 */
	CarPlayAppFrameworkSessionControl_f				sessionControl_f;			/**< CarPlay 세션 내에서 Command 전송과 같은 Control을 하였을 경우 호출되는 CallBack 함수 */
	CarPlayAppFrameworkModesChanged_f				modesChanged_f;				/**< 애플 디바이스로부터 modeChanged Command가 수신되면 호출되는 Callback 함수 */
	CarPlayAppFrameworkRequestUI_f					requestUI_f;				/**< 애플 디바이스로부터 RequestUI command가 수신되면 호출되는 Callback 함수 */
	CarPlayAppFrameworkDuckAudio_f					duckAudio_f;				/**< 애플 디바이스로부터 DuckAudio command가 수신되면 호출되는 Callback 함수 */
	CarPlayAppFrameworkUnduckAudio_f				unduckAudio_f;				/**< 애플 디바이스로부터 UnduckAudio command가 수신되면 호출되는 Callback 함수 */
	CarPlayAppFrameworkDisableBluetooth_f			disableBluetooth_f;			/**< 애플 디바이스에서 블루투스 해제(disableBluetooth)를 수신하였을 때, 호출되는 Callback 함수. */
	CarPlayAppFrameworkChangeModesComplete_f		changeModeComplete_f;		/**< 액서서리가 ChangeModes command를 전송하고 난 뒤, 애플 디바이스로부터 response가 수신되면 호출되는 Callback 함수 */
	CarPlayAppFrameworkAudioStreamStarted_f			audioStreamStarted_f;		/**< 오디오 스트림이 시작될 때 호출되는 CallBack 함수 */
	CarPlayAppFrameworkAudioStreamStopped_f			audioStreamStopped_f;		/**< 오디오 스트림이 종료될 때 호출되는 CallBack 함수 */
	CarPlayAppFrameworkAudioSessionTornDown_f		audioSessionTornDown_f;		/**< 오디오 스트림이 torn down될 때 호출되는 CallBack 함수 */
	CarPlayAppFrameworkScreenStreamStarted_f		screenStreamStarted_f;		/**< 스크린 스트림이 시작될 때 호출되는 CallBack 함수 */
	CarPlayAppFrameworkScreenStreamPlayed_f			screenStreamPlayed_f;		/**< 스크린 스트림이 재생될 때 호출되는 CallBack 함수 */
	CarPlayAppFrameworkScreenStreamStopped_f		screenStreamStopped_f;		/**< 스크린 스트림이 종료될 때 호출되는 CallBack 함수 */
#if defined(KOECN)
	CarPlayAppFrameworkScreenStreamProcessData_f		screenStreamProcessData_f;		/**< 스크린 스트림 Thread callback 함수 */
#endif
	CarPlayAppFrameworkCopyDeviceAvailable_f		copyDeviceAvailable_f;		/**< 오디오/스크린 디바이스가 사용가능한지 확인하기 위해 호출되는 Callback 함수 */
	CarPlayAppFrameworkReportDeviceError_f			reportDeviceError_f;		/**< 오디오/스크린 디바이스가 오류가 발생하였을 때 호출되는 Callback 함수 */
	CarPlayAppFrameworkSetupRequest_f				setupRequest_f;				/**< 오디오/스트린 스트림을 초기화 할 때, 호출되는 Callback 함수. 애플 디바이스가 소리를 재생하는 것을 지연할 때 사용됨. 지연하는 이유는 H/U에서 소리를 재생하기까지 준비 시간이 필요하기 때문임 */
} CarPlayAppFrameworkDelegate;

/*!
 * @struct CarPlayAppFrameworkAudioTrackParams
 * @brief 오디오 트랙의 리소스 설정으로 구성된 구조체 정의
 */
typedef struct {
    int32_t             streamType;                                /**< 오디오 트랙의 스트립 타입 설정 */
    int32_t             sessionID;                                 /**< 오디오 트랙의 세션 아이디 설정 */
} CarPlayAppFrameworkAudioTrackParams;

/*!
 * @struct CarPlayAppFrameworkAudioParams
 * @brief 오디오 리소스의 설정으로 구성된 구조체 정의
 */
typedef struct {
	char *                              inputDevice;                /**< 입력 디바이스 이름을 설정 */
	char *                              outputDevice;               /**< 출력 디바이스 이름을 설정 */
    CarPlayAppFrameworkAudioTrackParams inputAudioTrack;            /**< 입력 오디오 트랙 설정 */
    CarPlayAppFrameworkAudioTrackParams outputAudioTrack;           /**< 출력 오디오 트랙 설정 */
    CarPlayAudioFormat                  inputFormat;                /**< 오디오 입력 포멧 설정, @ref CarPlayAudioFormat 참고 */
    CarPlayAudioFormat                  outputFormat;               /**< 오디오 출력 포멧 설정, @ref CarPlayAudioFormat 참고 */
    uint32_t                            inputLatency;               /**< 입력 디바이스의 레이턴시 설정 */
    uint32_t                            outputLatency;              /**< 출력 디바이스의 레이턴시 설정 */
} CarPlayAppFrameworkAudioParams;

/*!
 * @struct CarPlayAppFrameworkAudioConfig
 * @brief 오디오 리소스의 Configuration 구조체 정의
 */
typedef struct {
    CarPlayAppFrameworkAudioParams  deflt;                              /**< default audio type의 오디오 디바이스 설정 */
	CarPlayAppFrameworkAudioParams	media;								/**< media audio type의 오디오 디바이스 설정 */
    CarPlayAppFrameworkAudioParams  telephony;                          /**< telephony audio type의 오디오 디바이스 설정 */
	CarPlayAppFrameworkAudioParams	speechRecognition;					/**< Siri(SpeechRecognition) audio type의 오디오 디바이스 설정 */
	CarPlayAppFrameworkAudioParams	alert;								/**< alert audio type의 오디오 디바이스 설정 */
    CarPlayAppFrameworkAudioParams  alt;                                /**< Alt 오디오 리소스를 사용하는 오디오 디바이스 설정 */
} CarPlayAppFrameworkAudioConfig;

/*!
 * @struct CarPlayAppFrameworkHIDInfo
 * @brief HID(Human Interface Device) 설정 값 구조체 정의
 */
typedef struct {
	uint32_t		hidCountryCode;										/**< [Integer] USB-style HID 국가 코드 */
	uint32_t		hidProductID;										/**< [Integer] USB-style HID 제품 ID */
	uint32_t		hidVendorID;										/**< [Integer] USB-style HID 제조회사 ID */
} CarPlayAppFrameworkHIDInfo;

/*!
 * @struct CarPlayAppFrameworkHIDInfoList
 * @brief HID(Human Interface Device) 리스트 구조체 정의
 */
typedef struct {
	CarPlayAppFrameworkHIDInfo		hidDPad;							/**< D-Pad 컨트롤러 HID 설정 */
	CarPlayAppFrameworkHIDInfo		hidKnob;							/**< 노브 컨트롤러 HID 설정 */
	CarPlayAppFrameworkHIDInfo		hidTouch;							/**< 터치 인터페이스 HID 설정 */
	CarPlayAppFrameworkHIDInfo		hidButton;							/**< 버튼 인터페이스 HID 설정 */
	CarPlayAppFrameworkHIDInfo		hidTelephony;						/**< 전화 번호 키 입력 인터페이스 HID 설정 */
	CarPlayAppFrameworkHIDInfo		hidTrackpad;						/**< Trackpad 인터페이스 HID 설정 */
} CarPlayAppFrameworkHIDInfoList;

/*!
 * @struct CarPlayAppFrameworkOEMIcons
 * @brief OEM Icons 구조체 정의
 */
typedef struct {
    const char *                    filePath;                           /**< [String] File path for OEM icon */
    int32_t                         widthPixel;                         /**< [Integer] Width of OEM icon in pixels */
    int32_t                         heightPixel;                        /**< [Integer] Height of OEM icon in pixels */
    int32_t                         prerendered;                        /**< [Boolean] Prerendered for OEM icon */
} CarPlayAppFrameworkOEMIcons;


/*!
 * @struct CarPlayAppFrameworkConfig
 * @brief CarPlay Client 설정을 위한 Configuration의 구조체
 */
typedef struct {
    int32_t                     actualWidthPixels;	/**< [Integer] Screen width in pixels */
    int32_t                     actualHeightPixels;	/**< [Integer] Screen height in pixels */
    int32_t                     widthPixels;		/**< [Integer] Width forwarded to iPhone in pixels */
    int32_t                     heightPixels;		/**< [Integer] Height forwarded to iPhone in pixels */
    int32_t                     widthPhysical;		/**< [Integer] Width forwarded to iPhone in millimeters */
    int32_t                     heightPhysical;		/**< [Integer] Height forwarded to iPhone in millimeters */
    const char *                displayEDID;		/**< [String] Display's EDID */
    const char *                displayUUID;		/**< [String] Display's UUID */
    int32_t                     hasDPad;			/**< [Boolean] Support DPad device */
    int32_t                     hasKnob;			/**< [Boolean] Support knob device */
    int32_t                     hasButton;			/**< [Boolean] Support button device */
    int32_t                     hasTelephony;		/**< [Boolean] Support telephony device */
    int32_t                     hasTrackpad;		/**< [Boolean] Support trackpad device */
    int32_t                     hasHiFiTouch;		/**< [Boolean] Support high fidelity touch */
    int32_t                     hasLoFiTouch;		/**< [Boolean] Support low fidelity touch */
    int32_t                     hasMultiTouch;		/**< [Boolean] Support Multi touch(single touch is enabled default */
    int32_t                     nightMode;			/**< [Boolean] Start with night mode in Maps */
    uint32_t                    limitedUIElements;	/**< [Unsigned Integer] Start with limited UI Element : ## Important ## Make sure that limitedUIElements list at least one of list */
    int32_t                     limitedUI;			/**< [Boolean] Start with limited UI */
    int32_t                     rightHandDrive;		/**< [Boolean] Start with right-hand-drive mode */
    uint32_t                    maxFPS;				/**< [Unsigned Integer] Preferred maximum FPS */
    uint32_t                    renderLayer;		/**< [Unsigned Integer] Window ID to use in MediaFramework */
    uint32_t                    primaryInputDevice; /**< [Unsigned Integer] Primary Input Device */
    int32_t                     oemIconVisible;		/**< [Boolean] Label for OEM icon */
    CarPlayAppFrameworkOEMIcons oemIcons[3];        /**< [Struct] Array for OEM icons for iOS9 */
	const char *				oemIconFilePath;	/**< [String] File path for OEM icon for prior to iOS9, use 104x104 pixel icon*/
    const char *                oemIconLabel;		/**< [String] Label for OEM icon */
    const char *                bluetoothIDs;		/**< [String] BluetoothIDs - you can describe multiple BT IDs with delimiter - semicolon(;)*/
    const char *                manufacturer;		/**< [String] Manufacturer */
    const char *                modelName;			/**< [String] Model name of the accessory (e.g. Device1,1) */
    const char *                ecnrConfigPath;		/**< [String] ECNR configuration directory */
} CarPlayAppFrameworkConfig;


/*!
 * @struct CarPlayAppFrameworkHIDPointing
 * @brief HID(Human Interface Device) 중 터치 디바이스 입력 정보 구조체
 */
typedef struct {
	int32_t		id;						/**< [Integer] Only for multipointing */
	int32_t		touch;					/**< [Boolean] Pressed is true, otherwise false */
	int32_t		x;						/**< [Integer] x within screen width */
	int32_t		y;						/**< [Integer] y within screen height */
} CarPlayAppFrameworkHIDPointing;

/*!
 * @struct CarPlayAppFrameworkHIDDPad
 * @brief HID(Human Interface Device) 중 D-패드 입력 정보 구조체
 */
typedef struct {
	int32_t		up;						/**< [Boolean] Up button is pressed */
	int32_t		down;					/**< [Boolean] Down button is pressed */
	int32_t		left;					/**< [Boolean] Left button is pressed */
	int32_t		right;					/**< [Boolean] Right button is pressed */
	int32_t		selec;					/**< [Boolean] Select button is pressed */
	int32_t		home;					/**< [Boolean] Home button is pressed */
	int32_t		menu;					/**< [Boolean] Menu button is pressed */
	int32_t		back;					/**< [Boolean] Back button is pressed */
}CarPlayAppFrameworkHIDDPad;

/*!
 * @struct CarPlayAppFrameworkHIDKnob
 * @brief HID(Human Interface Device) 중 노브 컨트롤러 입력 정보 구조체
 */
typedef struct {
	int32_t		button;					/**< [Boolean] Knob button is pressed */
	int32_t		home;					/**< [Boolean] Home button is selected */
	int32_t		back;					/**< [Boolean] Back button is selected */
	int8_t		x;						/**< [Integer] X position of Knob */
	int8_t		y;						/**< [Integer] Y position of Knob */
	int8_t		wheel;					/**< [Integer] Z position of Knob */
} CarPlayAppFrameworkHIDKnob;

/*!
 * @struct CarPlayAppFrameworkHIDButton
 * @brief HID(Human Interface Device) 중 버튼 입력 정보 구조체
 */
typedef struct {
	int32_t		pressed;				/**< [Boolean] Pressed is true, otherwise false */
	int32_t		home;					/**< [Boolean] Home button is selected */
	int32_t		back;					/**< [Boolean] Back button is selected */
	int32_t		play;					/**< [Boolean] Play button is selected */
	int32_t		pause;					/**< [Boolean] Pause button is selected */
	int32_t		playOrPause;			/**< [Boolean] Play/Pause toggle is selected */
	int32_t		scanNextTrack;			/**< [Boolean] Scan next track is selected */
	int32_t		scanPreviousTrack;		/**< [Boolean] Scan previous track is selected */
} CarPlayAppFrameworkHIDButton;

/*!
 * @struct CarPlayAppFrameworkHIDTelephony
 * @brief HID(Human Interface Device) 중 전화버튼 입력 정보 구조체
 */
typedef struct {
	int32_t		pressed;				/**< [Boolean] Pressed is true, otherwise false */
	int32_t		hookSwitch;				/**< [Boolean] HookSwitch button is selected */
	int32_t		flash;					/**< [Boolean] Flash button is selected */
	int32_t		drop;					/**< [Boolean] Drop button is selected */
	int32_t		phoneMute;				/**< [Boolean] Mute button is selected */
	int32_t		phoneKey0;				/**< [Boolean] PhoneKey0 button is selected */
	int32_t		phoneKey1;				/**< [Boolean] PhoneKey1 button is selected */
	int32_t		phoneKey2;				/**< [Boolean] PhoneKey2 button is selected */
	int32_t		phoneKey3;				/**< [Boolean] PhoneKey3 button is selected */
	int32_t		phoneKey4;				/**< [Boolean] PhoneKey4 button is selected */
	int32_t		phoneKey5;				/**< [Boolean] PhoneKey5 button is selected */
	int32_t		phoneKey6;				/**< [Boolean] PhoneKey6 button is selected */
	int32_t		phoneKey7;				/**< [Boolean] PhoneKey7 button is selected */
	int32_t		phoneKey8;				/**< [Boolean] PhoneKey8 button is selected */
	int32_t		phoneKey9;				/**< [Boolean] PhoneKey9 button is selected */
	int32_t		phoneKeyStar;			/**< [Boolean] PhoneKeyStar button is selected */
	int32_t		phoneKeyPound;			/**< [Boolean] PhoneKeyPound button is selected */
} CarPlayAppFrameworkHIDTelephony;

/*!
 * @struct CarPlayAppFrameworkHIDTrackpad
 * @brief HID(Human Interface Device) 중 track pad 입력 정보 구조체
 */
typedef struct {
	uint8_t 	character1Length;		/**< [Integer] character1 length of Trackpad */
	uint8_t 	character1Data[ 4 ];	/**< [Integer] character1 data of Trackpad */
	uint8_t 	character1Quality;		/**< [Integer] character1 quality of Trackpad */
	uint8_t 	character2Length;		/**< [Integer] character2 length of Trackpad */
	uint8_t 	character2Data[ 4 ];	/**< [Integer] character2 data of Trackpad */
	uint8_t 	character2Quality;		/**< [Integer] character2 quality of Trackpad */
	uint8_t 	transducerState;		/**< [Integer] transducer state of Trackpad */
	uint16_t 	transducerX;			/**< [Integer] transducer X position of Trackpad */
	uint16_t 	transducerY;			/**< [Integer] transducer Y position of Trackpad */
} CarPlayAppFrameworkHIDTrackpad;

/*!
 * @struct CarPlayAppFrameworkClientInfo
 * @brief CarPlay Client(애플 디바이스)의 정보 구조체
 */
typedef struct {
	char *	name;						/**< [String] 애플 디바이스의 이름, 예) My iPhone */
	char *	model;						/**< [String] 애플 디바이스의 모델, 예) iPhone7,2 */
	char *	osBuildVersion;				/**< [String] 애플 다바이스의 OS빌드 버전, 예) 12F70 */
} CarPlayAppFrameworkClientInfo;

//===========================================================================================================================
//	Declarations
//===========================================================================================================================
/*!
 * @defgroup CarPlayAppFramework_Struct_Init CarPlayAppFramework Data Structure Initialize Macros
 * @brief CarPlayAppFramework에서 사용되는 구조체를 초기화 하는 Convenience helpers.
 * @{
 */

/*!
 * @typedef CarPlayAppFrameworkRef
 * @brief CarPlay 세션간 전달 되는 Private contents
 */
typedef struct CarPlayAppFrameworkPrivate * CarPlayAppFrameworkRef;


/*!
 * @def CarPlayAppFrameworkDelegateInit
 * @brief @ref CarPlayAppFrameworkDelegate 구조체를 초기화
 * @param PTR CarPlayAppFrameworkDelegate 구조체 변수
 */
#define CarPlayAppFrameworkDelegateInit(PTR) memset((PTR), 0, sizeof(CarPlayAppFrameworkDelegate));
/*!
 * @def CarPlayAppFrameworkModeChangesInit
 * @brief @ref CarPlayAppFrameworkModeChanges 구조체를 초기화
 * @param PTR CarPlayAppFrameworkModeChanges 구조체 변수
 */
#define CarPlayAppFrameworkModeChangesInit(PTR) memset((PTR), 0, sizeof(CarPlayAppFrameworkModeChanges));
/*!
 * @def CarPlayAppFrameworkConfigInit
 * @brief @ref CarPlayAppFrameworkConfig 구조체를 초기화
 * @param PTR CarPlayAppFrameworkConfig 구조체 변수
 */
#define CarPlayAppFrameworkConfigInit(PTR) memset((PTR), 0, sizeof(CarPlayAppFrameworkConfig));
/*!
 * @def CarPlayAppFrameworkAudioConfigInit
 * @brief @ref CarPlayAppFrameworkAudioConfig 구조체를 초기화
 * @param PTR CarPlayAppFrameworkAudioConfig 구조체 변수
 */
#define CarPlayAppFrameworkAudioConfigInit(PTR) memset((PTR), 0, sizeof(CarPlayAppFrameworkAudioConfig));
/*!
 * @def CarPlayAppFrameworkHIDInfoListInit
 * @brief @ref CarPlayAppFrameworkHIDInfoList 구조체를 초기화
 * @param PTR CarPlayAppFrameworkHIDInfoList 구조체 변수
 */
#define CarPlayAppFrameworkHIDInfoListInit(PTR) memset((PTR), 0, sizeof(CarPlayAppFrameworkHIDInfoList));
/*! @} */

//===========================================================================================================================
//	Life cycle
//===========================================================================================================================
/*!
 * @defgroup CarPlayAppFramework_Life_cycle_Group CarPlayAppFramework Life Cycle Relative Functions
 * @brief 	 CarPlayAppFramework에서 life cycle 설정 시 호출되는 함수 그룹
 * @{
 */

/*!
 * @brief 	CarPlayAppFramework 초기화하고 private contents를 생성하여 반환함
 * @param 	outRef 생성된 private contents, @ref CarPlayAppFrameworkRef 구조의 변수.
 * @return 	@ref kCarPlayResult_Error (-1) : 초기화 실패\n
 * 			@ref kCarPlayResult_OK (0) : 초기화 완료
 */
int32_t CarPlayAppFrameworkInitialize(CarPlayAppFrameworkRef *outRef);
/*!
 * @brief 	CarPlay client configuration을 private contents에 설정함.
 * @param 	inRef CarPlay private contents, CarPlay client configuration 설정 값을 저장함.
 * @param 	inConfig CarPlay client configuration, @ref CarPlayAppFrameworkConfig 구조체 변수에 값을 설정하여 전달
 * @return 	@ref kCarPlayResult_Error (-1) : 설정 실패(private contents(inRef) 또는 설정할 Config(inConfig)가 Null인 경우)\n
 * 			@ref kCarPlayResult_OK (0) : 설정 완료
 */
int32_t CarPlayAppFrameworkSetConfig(CarPlayAppFrameworkRef inRef, const CarPlayAppFrameworkConfig *inConfig);
/*!
 * @brief 	rendering을 위한 Surface를 설정함. 이는 Screen을 출력할 Surface가 들어감.
 * @param 	inRef CarPlay private contents, 설정된 surface를 저장함.
 * @param 	renderLayer rendering을 위한 Surface.
 * @return 	@ref kCarPlayResult_Error (-1) : 설정 실패(private contents(inRef) 또는 private contents의 config가 Null인 경우)\n
 * 			@ref kCarPlayResult_OK (0) : 설정 완료
 */
int32_t CarPlayAppFrameworkSetSurface(CarPlayAppFrameworkRef inRef, const uint32_t renderLayer);
/*!
 * @brief   CarPlay Framework에 Audio Configuration에 각 Audio Type에 따른 Required Audio format을 설정함.
 * @param   outConfig 설정할 Audio Configuration
 * @return  @ref kCarPlayResult_Error (-1) : 설정 실패(private contents(inRef) 또는 Audio config가 Null인 경우)\n
 *          @ref kCarPlayResult_OK (0) : 설정 완료
 */
int32_t CarPlayAppFrameworkSetRequiredAudioFormat(CarPlayAppFrameworkAudioConfig *outConfig);
/*!
 * @brief 	CarPlay Framework에 Audio Configuration을 설정함.
 * @param 	inRef CarPlay private contents, 설정이 완료되면 완료 결과가 저장됨.
 * @param 	inConfig 설정할 Audio Configuration
 * @return 	@ref kCarPlayResult_Error (-1) : 설정 실패(private contents(inRef) 또는 Audio config가 Null인 경우)\n
 * 			@ref kCarPlayResult_OK (0) : 설정 완료
 *
 * @note        inConfig 로 넘겨준 구조체 안의 inputDevice/outputDevice 문자열들은 CarPlay framework 에서 pointer 참조로 사용 되므로  CarPlay 종료시까지 메모리가 유지되어져야 함.
 */
int32_t CarPlayAppFrameworkSetAudioConfig(CarPlayAppFrameworkRef inRef, const CarPlayAppFrameworkAudioConfig *inConfig);
/*!
 * @brief 	CarPlay Framework에 MIC 입력을 음소거 시킴.
 * @param 	inRef CarPlay private contents, MIC 음소거 설정 값이 저장됨.
 * @param 	inMICMute 음소거 설정 값, 0 : 음소거 비활성화, 1 : 음소거 활성화
 * @return 	@ref kCarPlayResult_Error (-1) : 설정 실패(private contents(inRef)가 Null인 경우)\n
 * 			@ref kCarPlayResult_OK (0) : 설정 완료
 */
int32_t CarPlayAppFrameworkSetAudioMICMute(CarPlayAppFrameworkRef inRef, const int32_t inMICMute);
/*!
 * @brief 	CarPlay HID(Human Interface Device) List를 private contents에 설정함.
 * @param 	inRef CarPlay private contents, HID List를 저장함.
 * @param 	inList 설정할 HID List.
 * @return 	@ref kCarPlayResult_Error (-1) : 설정 실패(private contents(inRef) 또는 HIDList(inList)가 Null인 경우)\n
 * 			@ref kCarPlayResult_OK (0) : 설정 완료
 */
int32_t CarPlayAppFrameworkSetHIDInfoList(CarPlayAppFrameworkRef inRef, const CarPlayAppFrameworkHIDInfoList *inList);
/*!
 * @brief 	CarPlay Client에 Callback 함수들를 등록함.
 * @param 	inRef CarPlay private contents, Callback 함수를 호출해주는 delegate를 저장함.
 * @param 	inDelegate Callback 함수를 호출해주기 위한 delegate.
 * @return 	@ref kCarPlayResult_Error (-1) : 설정 실패(private contents(inRef) 또는 Delegate(inDelegate)가 Null인 경우)\n
 * 			@ref kCarPlayResult_OK (0) : 설정 완료
 */
int32_t CarPlayAppFrameworkSetDelegate(CarPlayAppFrameworkRef inRef, const CarPlayAppFrameworkDelegate *inDelegate);
/*!
 * @brief 연결된 CarPlay 세션의 클라이언트 정보를 읽어옴.
 * @param inRef CarPlay Private Contents
 * @param outClientInfo CarPlay Client 정보, @ref CarPlayAppFrameworkClientInfo 구조 참고.
 * @return	@ref kCarPlayResult_Error (-1) : 읽어올 수 없음(private contents(inRef) 또는 CarPlay 세션이 Null인 경우)\n
 * 			@ref kCarPlayResult_OK (0) : 읽어오기 완료
 */
int32_t	CarPlayAppFrameworkGetClientInfo(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkClientInfo *outClientInfo);
/*!
 * @brief 	CarPlay Client의 private content에 설정된 값을 이용하여 CarPlay를 시작함.
 * @param 	inRef CarPlay private contents
 * @return 	@ref kCarPlayResult_Error (-1) : CarPlay Start 실패\n
 * 			@ref kCarPlayResult_OK (0) : CarPlay Start 완료(CarPlay 스레드 실행 완료)
 */
int32_t CarPlayAppFrameworkStart(CarPlayAppFrameworkRef inRef);
/*!
 * @brief 	CarPlay를 중단하고, CarPlay 스레드를 종료함.
 * @param 	inRef CarPlay private contents
 * @return 	@ref kCarPlayResult_Error (-1) : CarPlay Stop 실패\n
 * 			@ref kCarPlayResult_OK (0) : CarPlay Stop 완료(CarPlay 스레드 중단 완료)
 */
int32_t CarPlayAppFrameworkStop(CarPlayAppFrameworkRef inRef);
/*!
 * @brief 	CarPlay에 종료 후 사용되지 않는 private content 메모리를 반환함.
 * @param 	inRef CarPlay private contents
 * @return 	항상 @ref kCarPlayResult_OK (0) 반환.
 */
int32_t CarPlayAppFrameworkTerminate(CarPlayAppFrameworkRef inRef);
/*! @} */

//===========================================================================================================================
// Human Interface Devices
//===========================================================================================================================
/*!
 * @defgroup CarPlayAppFramework_HID_Group CarPlayAppFramework Post HID Functions
 * @brief 	 CarPlayAppFramework에서 HID(Human Interface Device) 설정 시 호출되는 함수 그룹
 * @{
 */
/*!
 * @brief 	터치 디바이스 입력을 애플 디바이스로 전달함.
 * @param 	inRef CarPlay private contents, 터치 HID 정보가 사용됨.
 * @param 	inPointing 터치 디바이스 입력 정보
 * @return 	@ref kCarPlayResult_Error (-1) : 터치 디바이스 입력 전달 실패\n
 * 			@ref kCarPlayResult_OK (0) : 터치 디바이스 입력 전달 성공
 */
int32_t CarPlayAppFrameworkPostPointing(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkHIDPointing inPointing);
/*!
 * @brief 	멀티 터치 디바이스 입력을 애플 디바이스로 전달함.
 * @param 	inRef CarPlay private contents, 멀티 터치 HID 정보가 사용됨.
 * @param 	inPointing 멀티 터치 디바이스 입력 정보
 * @return 	@ref kCarPlayResult_Error (-1) : 멀티 터치 디바이스 입력 전달 실패\n
 * 			@ref kCarPlayResult_OK (0) : 멀티 터치 디바이스 입력 전달 성공
 */
int32_t CarPlayAppFrameworkPostMultiPointing(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkHIDPointing inPointing[2]);
/*!
 * @brief 	D-Pad 컨트롤러 입력을 애플 디바이스로 전달함
 * @param 	inRef CarPlay private contents, D-Pad HID 정보가 사용됨.
 * @param 	inDPad D-Pad 컨트롤러 입력 정보
 * @return 	@ref kCarPlayResult_Error (-1) : D-Pad 컨트롤러 입력 전달 실패\n
 * 			@ref kCarPlayResult_OK (0) : D-Pad 컨트롤러 입력 전달 성공
 */
int32_t CarPlayAppFrameworkPostDPad(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkHIDDPad inDPad);
/*!
 * @brief 	노브 컨트롤러 입력을 애플 디바이스로 전달함
 * @param 	inRef CarPlay private contents, 노브 HID 정보가 사용됨.
 * @param 	inKnob 노브 컨트롤러 입력 정보
 * @return 	@ref kCarPlayResult_Error (-1) : 노브 컨트롤러 입력 전달 실패\n
 * 			@ref kCarPlayResult_OK (0) : 노브 컨트롤러 입력 전달 성공
 */
int32_t CarPlayAppFrameworkPostKnob(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkHIDKnob inKnob);
/*!
 * @brief 	버튼 입력을 애플 디바이스로 전달함
 * @param 	inRef CarPlay private contents, 버튼 HID 정보가 사용됨.
 * @param 	inButton 버튼 입력 정보
 * @return 	@ref kCarPlayResult_Error (-1) : 버튼 입력 전달 실패\n
 * 			@ref kCarPlayResult_OK (0) : 버튼 입력 전달 성공
 */
int32_t CarPlayAppFrameworkPostButton(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkHIDButton inButton);
/*!
 * @brief 	전화 버튼 입력을 애플 디바이스로 전달함
 * @param 	inRef CarPlay private contents, 전화 버튼 HID 정보가 사용됨.
 * @param 	inTelephony 전화 버튼 입력 정보
 * @return 	@ref kCarPlayResult_Error (-1) : 전화 버튼 입력 전달 실패\n
 * 			@ref kCarPlayResult_OK (0) : 전화 버튼 입력 전달 성공
 */
int32_t CarPlayAppFrameworkPostTelephony(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkHIDTelephony inTelephony);
/*!
 * @brief 	Trackpad 입력을 애플 디바이스로 전달함
 * @param 	inRef CarPlay private contents, Trackpad HID 정보가 사용됨.
 * @param 	inTrackpad 전화 버튼 입력 정보
 * @return 	@ref kCarPlayResult_Error (-1) : 전화 버튼 입력 전달 실패\n
 * 			@ref kCarPlayResult_OK (0) : 전화 버튼 입력 전달 성공
 */
int32_t CarPlayAppFrameworkPostTrackpad(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkHIDTrackpad inTrackpad);
/*!
 * @brief 	스트림 디바이스가 사용할 준비가 되었음을 알림. Setup Response를 대기하고 있는 스레드에 시그널을 보내는 역할을 함.
 * @param 	inRef CarPlay private contents
 * @param 	inStreamType 스트림 타입
 * @return 	@ref kCarPlayResult_Error (-1) : 스트림 디바이스 준비 전달 실패\n
 * 			@ref kCarPlayResult_OK (0) : 스트림 디바이스 준비 전달 성공
 */
int32_t CarPlayAppFrameworkPostStreamDeviceReady(CarPlayAppFrameworkRef inRef, CarPlayStreamType inStreamType);
/*! @} */

//===========================================================================================================================
// Commands
//===========================================================================================================================
/*!
 * @defgroup CarPlayAppFramework_Command_Group CarPlayAppFramework CarPlay Commands Sending Functions
 * @brief 	 CarPlayAppFramework에서 Command 전송을 요청할 때 호출되는 함수 그룹
 * @{
 */
/*!
 * @brief 	Night Mode 설정 Command을 애플 디바이스로 전달함. 세션 연결이 되어 있지 않으면 Night Mode 값만 저장되고 이후에 CarPlay가 연결되면 저장된 Night Mode 값이 사용됨.
 * @param 	inRef CarPlay private contents, 현재 연결된 세션 정보를 전달받고, 새로 설정한 Night Mode가 저장됨.
 * @param 	inNightMode 0 : Night Mode 설정 안함\n 1 : Night Mode 설정.
 * @return	@ref kCarPlayResult_Error (-1) : Command 전달 실패\n
 * 			@ref kCarPlayResult_OK (0) : Command 전달 성공
 */
int32_t CarPlayAppFrameworkSetNightMode(CarPlayAppFrameworkRef inRef, const int32_t inNightMode);
/*!
 * @brief LimitedUI 설정 Command을 애플 디바이스로 전달함. 세션 연결이 되어 있지 않으면 LimitedUI 값만 저장되고 이후에 CarPlay가 연결되면 저장된 LimitedUI 값이 사용됨.
 * @param inRef CarPlay private contents, 현재 연결된 세션 정보를 전달받고, 새로 설정한 LimitedUI가 저장됨.
 * @param 	inNightMode 0 : LimitedUI 설정 안함\n 1 : LimitedUI 설정.
 * @return	@ref kCarPlayResult_Error (-1) : Command 전달 실패\n
 * 			@ref kCarPlayResult_OK (0) : Command 전달 성공
 */
int32_t CarPlayAppFrameworkSetLimitedUI(CarPlayAppFrameworkRef inRef, const int32_t inLimitedUI);
/*!
 * @brief 	RequestUI command를 애플 디바이스로 전달함
 * @param 	inRef CarPlay private contents, 현재 연결된 세션 정보를 전달함
 * @param 	inURL RequestUI 전달시 함께 전달되는 URL(예로, maps://)로 URL을 이용하면 연동되는 어플이 바로 실행됨
 * @return 	@ref kCarPlayResult_Error (-1) : Command 전달 실패\n
 * 			@ref kCarPlayResult_OK (0) : Command 전달 성공
 */
int32_t CarPlayAppFrameworkRequestUI(CarPlayAppFrameworkRef inRef, const char* inURL);
/*!
 * @brief 	스크린의 Key Frame을 요청하는 Command를 애플 디바이스로 전달함. Key Frame은 한 화면의 변경된 정보가 아닌 모든 정보를 가진 프레임임.
 * @param 	inRef CarPlay private contents, 현재 연결된 세션 정보를 전달함
 * @return 	@ref kCarPlayResult_Error (-1) : Command 전달 실패\n
 * 			@ref kCarPlayResult_OK (0) : Command 전달 성공
 */
int32_t CarPlayAppFrameworkForceKeyFrame(CarPlayAppFrameworkRef inRef);
/*!
 * @brief 	RequestSiri command를 애플 디바이스로 전달함
 * @param 	inRef CarPlay private contents, 현재 연결된 세션 정보를 전달함
 * @param 	inAction 시리의 동작 타입
 * @return 	@ref kCarPlayResult_Error (-1) : Command 전달 실패\n
 * 			@ref kCarPlayResult_OK (0) : Command 전달 성공
 */
int32_t CarPlayAppFrameworkRequestSiri(CarPlayAppFrameworkRef inRef, CarPlaySiriAction inAction);
/*!
 * @brief 	Pending된 Task를 모두 Stop 시킴
 * @param 	inRef CarPlay private contents, pendingTaskMap을 사용함.
 * @return 	@ref kCarPlayResult_Error (-1) : 실패(Config 값들이 Null인 경우)\n
 * 			@ref kCarPlayResult_OK (0) : 수행 완료.
 */
int32_t CarPlayAppFrameworkStopPendingTasks(CarPlayAppFrameworkRef inRef);
/*! @} */

//===========================================================================================================================
// Modes
//===========================================================================================================================
/*!
 * @defgroup CarPlayAppFramework_Mode_Group CarPlayAppFramework CarPlay Resource Mode Control Functions
 * @brief 	 CarPlayAppFramework에서 Mode를 변경을 요청할 때 호출되는 함수 그룹
 * @{
 */
/*!
 * @brief 	AppState 변경 요청을 애플 디바이스로 전달
 * @param 	inRef CarPlay private contents
 * @param 	inState 변경하고자 하는 App state 타입
 * @param 	inEnable App State의 값
 * @return 	@ref kCarPlayResult_Error (-1) : Command 전달 실패\n
 * 			@ref kCarPlayResult_OK (0) : Command 전달 성공
 * @note 이 API 사용하는 것을 추천하지 않고, @ref CarPlayAppFrameworkChangeModes를 사용하는 것을 추천함.
 */
// Function for changing application state
int32_t CarPlayAppFrameworkChangeAppState(CarPlayAppFrameworkRef inRef, CarPlayAppState inState, int32_t inEnable);

// Function for changing mode in detail
// If session isn't established yet, mode is saved locally and used as initial mode when establishing the session.
// Otherwise the mode is transfer to the device directly in the current session.
/*!
 * @brief 	Mode를 변경 요청을 애플 디바이스로 전달(AppState와 Ownership을 모두 변경 요청함)
 * @param 	inRef CarPlay private contents
 * @param 	inModes 변경하고자 하는 모드
 * @return 	@ref kCarPlayResult_Error (-1) : Command 전달 실패\n
 * 			@ref kCarPlayResult_OK (0) : Command 전달 성공
 */
int32_t CarPlayAppFrameworkChangeModes(CarPlayAppFrameworkRef inRef, CarPlayAppFrameworkModeChanges inModes);

// Utility function for changing mode simply with default priority and constraints
// If session isn't established, just return error.
/*!
 * @brief 	Transfer 타입에 따른 기본 Ownership으로 변경 요청을 애플 디바이스에 전달
 * @param 	inRef CarPlay private contents
 * @param 	inTransferType Transter Type, @ref CarPlayTransferType 중 한가지 사용.
 * @param 	inResource 변경하고자 하는 리소스, @ref CarPlayOwnershipResource 중 한가지 사용.
 * @return 	@ref kCarPlayResult_Error (-1) : Command 전달 실패\n
 * 			@ref kCarPlayResult_OK (0) : Command 전달 성공
 * @note 이 API 사용하는 것을 추천하지 않고, @ref CarPlayAppFrameworkChangeModes를 사용하는 것을 추천함.
 */
int32_t CarPlayAppFrameworkSetOwnership(CarPlayAppFrameworkRef inRef, CarPlayTransferType inTransferType, CarPlayOwnershipResource inResource);
/*! @} */

/*!
 * @brief 	tseECNR 엔진쪽으로 현재 AVN 볼륨값을 전달 해 주기 위한 volInfo 설정.
 * @param 	inRef CarPlay private contents
 * @param 	volInfo 현재 볼륨 값을 전달. 볼륨값이 바뀔 때 마다 전달 해 줘야 함.
 *
 * note		AVN 볼륨값(UI 표기되는 값, 현대경제형의 경우 0~45) 를 값이 변동 될 때 마다 호출.
 */
int32_t CarPlayAppFrameworkUpdateVolInfo(CarPlayAppFrameworkRef inRef, uint32_t volInfo);

/*!
 * @brief 	altAudio 음원을 mute/unmute 하기 위한 api
 * @param 	inRef CarPlay private contents
 * @param 	outAltAudioMute mute/unmute 설정
 *
 * note		본 api 를 사용할 경우 apple 사양에 위배되므로 waiver 협의 해야 함.
 */
int32_t CarPlayAppFrameworkSetAltAudioMute(CarPlayAppFrameworkRef inRef, const int32_t outAltAudioMute);

#if defined(MEEGO)
/*!
 * @brief 	AudioStream 을 종료하기 위한 api
 * @param 	inRef CarPlay private contents
 * @param 	inStream 종료 시키기 위한 inStream
 *
 * note		phone 으로부터 들어오는 audio stream 을 임의로 종료 시키면 apple 사양에 위배 되므로,
 *              USB disconnect 등 phone 으로부터 더이상 audio stream 이 들어오지 않는 것이 확실할 경우에만 사용해야 함
 */
int32_t CarPlayAppFrameworkStopAudioStreamForce(CarPlayAppFrameworkRef inRef, void* inStream);
#endif

#ifdef __cplusplus
}
#endif

#endif /* CARPLAYAPPFRAMEWORK_H_ */
