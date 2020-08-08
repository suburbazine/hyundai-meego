/*!
 *  @mainpage DHWR APIs Document
 *
 *  @section intro_sec Introduction
 *  본 문서는 (주)디오텍의 필기인식 솔루션인 DioPen의 인식엔진(DHWR)의 API 가이드이다. \n
 *  DHWR은 표준 C언어 기반의 라이브러리로 터치 스크린에 기반한 임베디드 플랫폼에 적용되고 있다.
 *
 *  @section install_sec Setting
 *  @subsection event_sec Touch Event
 *  필기 입력기 구현을 위해서는 다음과 같은 터치 이벤트 사양이 요구된다. \n
 *    - Touch Down : 펜을 터치 디바이스상에 처음 대는 순간 발생하는 이벤트. \n
 *    - Touch Move : 펜을 터치 디바이스에서 떼지 않은 상태로 있을 경우 발생하는 이벤트. \n
 *    - Touch Up : 펜을 터치 디바이스 상에서 떼는 순간 발생하는 이벤트. Touch Down이 발생한 경우라면 항상 Touch Up이 발생하여야 한다. 즉, 사용자가 필기 도중 터치 패드 영역을 벗어난 경우에도 드라이버에서는 Touch Up 이벤트를 발생 시켜야 한다.
 *
 *  @subsection samplingrate_sec Sampling Rate
 *  'Sampling Rate’란 터치 디바이스에서 터치 이벤트를 체크하는 시간(or 양)을 말한다. \n
 *  필기 입력기 적용을 위해서는 보통 10~40mSec(초당 25~100개의 터치이벤트) 정도로 조정한다. \n
 *  유의할 점은 Sampling Rate가 40mSec를 넘게 되면 필기한 모양이 자연스럽지 못하고 인식률도 떨어지게 된다. 대개의 경우 10~20mSec를 권장한다.
 *
 *  @subsection delaytime_sec Delay time
 *  인식 대기 시간은 필기 입력이 끝났다고 판단하는데 걸리는 시간이다. \n
 *  즉, 터치 디바이스에서 펜을 뗀 후 일정 시간 동안 필기 입력이 없으면 사용자의 입력이 끝났다고 판단하고 인식 엔진을 호출하게 된다. \n
 *  인식 대기 시간은 설정 메뉴를 두어 사용자로 하여 적당한 값을 선택할 수 있도록 할 수도 있으며, 일반적으로 300~500mSec의 값을 사용한다. \n
 *  인식 대상 문자를 지정하는 경우 대상 문자의 종류에 따라 차등 적용할 수도 있다.(예, 한글 - 400mSec, 영숫자 - 200mSec) \n
 *  물론, 인식 대기 시간을 활용하지 않고 특정 버튼을 두어 시간에 관계없이 버튼을 클릭하면 바로 인식 엔진을 호출하게끔 구현할 수도 있다.
 *
 *  @section porting_sec Porting
 *  포팅에 필요한 파일들은 다음과 같다. \n
 *    - dhwr.h - main API function 등이 선언되어 있다. \n
 *    - dhwr_define.h - 각종 definition \n
 *    - dhwr_gesture.h - gesture 관련 definition \n
 *    - dhwr_ink.h - ink 관련 definition \n
 *    - dhwr_result.h - result 관련 definition \n
 *    - dhwr_setting.h - setting 관련 definition \n
 *    - dhwr.a (libdhwr.so) - 인식엔진 라이브러리로 App.와 인식 엔진간의 인터페이스를 위한 라이브러리이다. \n
 *
 *  @section link_sec Link
 *  (주)디오텍 http://www.diotek.co.kr
 */

/*!
 *  @file  dhwr.h
 *  @date  2011/10/12
 *  @author  shchoi
 *
 *  Copyright (c) 2011 by DIOTEK Co.,LTD. All rights reserved.
 */

/*! 
 *  @example example.c
 *  DHWR API들을 이용하여 필기인식기를 구현하는 예제
 */

#ifndef __DHWR_H__
#define __DHWR_H__

#include "dhwr_define.h"
#include "dhwr_gesture.h"
#include "dhwr_ink.h"
#include "dhwr_result.h"
#include "dhwr_setting.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  @brief 엔진 내부 오류 코드 값
 */
typedef enum {
  //! 성공
  ERR_SUCCESS        = 0,
  //! 인식결과가 없음
  ERR_NORESULT,
  //! Null Pointer를 참조함
  ERR_NULL_POINTER,
  //! memory access 범위를 벗어남 fatal error
  ERR_OUTOFMEMORY,
  //! value or size 범위를 벗어남 exception
  ERR_OUTOFRANGE,
  //! 입력된 데이터가 없음
  ERR_EMPTY_INK,
  //! 입력된 인자가 잘못됨
  ERR_INVALID_ARGUMENTS,
  //! 설정된 인식모델이 비정상적임
  ERR_INVALID_MODEL,
  //! 설정된 사전파일이 비정상적임
  ERR_INVALID_DICTIONARY,
  //! 잘못된 객체에 접근
  ERR_INVALID_INSTANCE,
  //! 데모 기간/횟수 만료
  ERR_EXPIRE_DEMO,
  //! 지원되지 않는 모드
  ERR_UNSUPPORTED_MODE,
  //! 엔진이 실행 중
  ERR_ENGINE_BUSY,
  //! 인증 실패
  ERR_AUTHORIZATION_FAIL
} DHWR_ERROR_CODE;

/* ----------------------------------------------------------------------
 * Functions
 * All functions return combination of DHWR_ERROR_CODE flags
 * -------------------------------------------------------------------- */

/**
 * @defgroup  Main Main APIs
 * @brief     엔진 사용에 필수적인 함수
 */

/*@{*/

/*!
 *  @brief 인식엔진 세션을 시작함
 *  @return DHWR_ERROR_CODE: 함수의 수행 결과에 따른 오류 코드
 */
DHWR_API unsigned long DHWRCreate(void);

/*!
 *  @brief 인식모드 속성 값에 따라 입력 데이터를 인식함
 *  @param [in] setting 설정이 완료된 Setting Object
 *  @see DHWRSettingObject
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned int DHWRSetAttribute(DHWRSettingObject setting);

/*!
 *  @brief 인식모드 속성 값에 따라 입력 데이터를 인식함
 *  @param [in] ink 좌표가 입력된 Ink Object
 *  @param [out] context 인식된 결과가 저장된 Result Object
 *  @remarks DHWRInkObject, DHWRResultObject
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned long DHWRecognize(DHWRInkObject ink, DHWRResultObject context);

/*!
 *  @brief 인식엔진 세션을 마침
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned long DHWRClose(void);

/*@}*/

/**
 * @defgroup Option Optional APIs
 */

/*!
*  @brief  엔진내부에서 사용하는 외부리소스(언어모델 파일 등)를 확인하여 인식가능한 언어 리스트 제공
*  @param [out] list 결과 버퍼로 메모리 할당 및 해제는 엔진 내부에서 수행함
*  @returns  unsigned long
*/
DHWR_API unsigned int DHWRAvailableLanguageList(const unsigned short ** list, int* size);

/*!
*  @brief  엔진내부에서 사용하는 외부리소스(언어모델 파일 등)의 경로 설정
*  @param [in] path 리소스 경로
*  @returns  unsigned long
*/
DHWR_API unsigned long DHWRSetExternalResourcePath(const char* path);

/*!
* @brief  엔진내부에서 사용하는 외부라이브러리의 경로 설정
* @returns  unsigned long
*/
DHWR_API unsigned long DHWRSetExternalLibraryPath(const char* path);

/*!
 *  @brief 특정 타입의 파라미터 값을 가져옴
 *  @param [in] type 파라미터 타입
 *  @param [in] param 특정 파리미터 타입의 포인터
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned long DHWRGetParam(const unsigned long type, void * param);

/*!
 *  @brief 특정 타입의 파라미터 값을 설정함
 *  @param [in] type 파라미터 타입
 *  @param [in] param 특정 파리미터 타입의 포인터
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned long DHWRSetParam(const unsigned long type, const void * param);

/*!
 *  @brief 엔진의 빌드넘버를 가져옴
 *  @param [out] revision 엔진 빌드넘버
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned long DHWRGetRevision(char ** revision);

/*!
 *  @brief 모음의 user character set을 지정
 *  @param [in] code_set 인식대상 문자 배열
 *  @param [in] code_count code_set에 지정한 총 인식 대상 문자 개수
 *  @remarks code_count는 50 이하로 설정
 *  @remarks DLANG_KOREAN 이면서 DTYPE_VOWEL 모드와 다른 모드(DTYPE_WANSUNG 또는 DTYPE_JOHAP)와 혼용일 때만 동작
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned long DHWRSetUserVowelCharSet(unsigned short * code_set,
  int code_count);

/*!
 *  @brief 모음의 user character set을 해제
  *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned long DHWRFreeUserVowelCharSet(void);

/*!
 *  @brief 입력 데이터를 외부 버퍼에 모은 다음 한 번에 입력함
 *  @param [in] ink_buff 잉크 구조체 버퍼
 *  @param [in] points 잉크의 점의 개수
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned long DHWRSetInkBuff(unsigned char * ink_buff, int points);

/*@}*/

#ifdef __cplusplus
}
#endif

#endif  // __DHWR_H__
