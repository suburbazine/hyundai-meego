/*!
 *      @date   2011/09/08
 *      @file   dhwr_setting.h
 *      @author jskim
 *
 *      Copyright (c) 2011 by DIOTEK Co.,LTD. All rights reserved.
 */

#ifndef __DHWR_SETTING_OBJECT_H__
#define __DHWR_SETTING_OBJECT_H__ 1

#include "dhwr_define.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup SettingObject SettingObject APIs
 * @ingroup  Main
 * @brief    엔진 동작을 결정하는 SettingObject관련 함수
 */

/*@{*/

/*!
 *  @brief 언어 리스트
 */
typedef enum
{
  // Latin
  DLANG_ENGLISH,
  DLANG_ENGLISH_UK,
  DLANG_ALBANIAN,
  DLANG_AUSTRIA,
  DLANG_GERMANY = DLANG_AUSTRIA,
  DLANG_BASQUE,
  DLANG_CATALAN,
  DLANG_CROATIAN,
  DLANG_CZECH,
  DLANG_DANISH,
  DLANG_DUTCH,
  DLANG_ESTONIAN,
  DLANG_FINNISH,
  DLANG_FRENCH,
  DLANG_GAELIC,
  DLANG_IRISH = DLANG_GAELIC,
  DLANG_SPANISH,
  DLANG_HUNGARIAN,
  DLANG_ICELANDIC,
  DLANG_ITALIAN,
  DLANG_LATVIAN,
  DLANG_LITHUANIAN,
  DLANG_NORWEGIAN,
  DLANG_POLISH,
  DLANG_PORTUGUESE,
  DLANG_PORTUGUESEB,
  DLANG_ROMANIAN,
  DLANG_SERBIAN_L,//Serbian_Latin
  DLANG_SLOVAK,
  DLANG_SLOVENIAN,
  DLANG_SWEDISH,
  DLANG_TURKISH,
  DLANG_VIETNAMESE,
  DLANG_TATAR_L,
  DLANG_REGION1_ALL,
  DLANG_BELARUSIAN,
  DLANG_BULGARIAN,
  // Greek
  DLANG_GREEK,
  // Cyrillic
  DLANG_KAZAKH,
  DLANG_MACEDONIAN,
  DLANG_MONGOLIAN,
  DLANG_RUSSIAN,
  DLANG_SERBIAN_C, //Serbian_Cyriliic
  DLANG_UKRAINIAN,
  DLANG_TATAR_C,
  // Oceania
  DLANG_FIJIAN,
  DLANG_MAORI,
  DLANG_SAMOAN,
  DLANG_TAITIAN,
  DLANG_TONGAN,
  // Africa
  DLANG_OROMO,
  DLANG_SOTHO_S,
  DLANG_SOTHO_N,
  DLANG_SWAHILI,
  DLANG_SWATI,
  DLANG_XHOSA,
  DLANG_ZULU,
  DLANG_REGION2_ALL,

  DLANG_INDONESIAN,
  DLANG_MALAY,
  DLANG_GALICIAN,

  // CJK
  DLANG_KOREAN    = 101,
  DLANG_HANJA,
  DLANG_SIMP_CHN,
  DLANG_SIMP_CHN_COMMON,
  DLANG_SIMP_CHN_RARE,
  DLANG_SIMP_RADICAL,
  DLANG_TRAD_CHN,
  DLANG_TRAD_CHN_COMMON,
  DLANG_TRAD_CHN_RARE,
  DLANG_EXTRA_CHN,
  DLANG_HONGKONG,
  DLANG_HIRAGANA,
  DLANG_KATAKANA,
  DLANG_KANJI,
  DLANG_BPMF,
  // ARABIC
  DLANG_ARABIC,
  DLANG_FARSI,
  DLANG_URDU,
  DLANG_ARABIC_SYM,
  DLANG_ARABIC_NUM,
  // Devanagari
  DLANG_HINDI,
  DLANG_BENGALI,
  // etc
  DLANG_HEBREW,
  DLANG_THAI,        // Thai
  DLANG_THAI_SIGN,    // Thai Sign
  DLANG_THAI_DIGIT,    // Thai Digit
  // gesture
  DLANG_GESTURE,      // gesture for action
  DLANG_GESTURE_EDITING,  // basic gesture for editing (space, backspace 등)
  DLANG_USER_SHAPE,
  DLANG_FLICK,
  // sign
  DLANG_NUMERIC,    // ascii digit
  DLANG_PUNC,
  DLANG_SYMBOL,    // ascii value
  DLANG_EQUALITY,
  DLANG_EMOTICON,
  // user made
  DLANG_USER_SET,
  DLANG_CALCULATOR,
  DLANG_DIALDIGIT,
  DLANG_DIALDIGIT_PW,
  
  // @todo(jskim): need to resort by the language category
  // for avoiding frequently modification.
  DLANG_CHARACTER_COMPONENTS = 150,
  DLANG_TRAD_RADICAL,
  // max
  DLANG_MAX
} DHWR_LANG;

/*!
 *  @brief 언어의 subtype값
 */
typedef enum
{
  DTYPE_NONE        = BIT_FLAG(0),
  DTYPE_MULTI_CHARS    = BIT_FLAG(1),
  DTYPE_UPPERCASE      = BIT_FLAG(2),
  DTYPE_LOWERCASE      = BIT_FLAG(3),
  //DTYPE_AUTO_SPACE    = BIT_FLAG(4),
  DTYPE_AUTO_SPACE    = BIT_FLAG(0),
  DTYPE_MULTI_LINE    = BIT_FLAG(5),    // auto wrapping
  DTYPE_UNISTROKE      = BIT_FLAG(6),
  DTYPE_CURSIVE      = BIT_FLAG(7),
  DTYPE_WANSUNG      = BIT_FLAG(8),
  DTYPE_JOHAP        = BIT_FLAG(9),
  DTYPE_CONSONANT      = BIT_FLAG(10),
  DTYPE_CONS_RECOMMEND  = BIT_FLAG(11),
  DTYPE_VOWEL        = BIT_FLAG(12),
  DTYPE_VOWEL_DEPENDENT  = BIT_FLAG(13),
  DTYPE_TONE        = BIT_FLAG(14),
  DTYPE_CURRENCY      = BIT_FLAG(15),
  DTYPE_SIMP_TO_TRAD    = BIT_FLAG(16),
  DTYPE_TRAD_TO_SIMP    = BIT_FLAG(17),
  DTYPE_ONLY_TRAD      = BIT_FLAG(2),      // DLANG_SIMP_CHN, DLANG_TRAD_CHN
  DTYPE_ONLY_SIMP      = BIT_FLAG(3),      // DLANG_SIMP_CHN, DLANG_TRAD_CHN
  DTYPE_BOTH_SIMP_TRAD  = BIT_FLAG(8),      // DLANG_SIMP_CHN, DLANG_TRAD_CHN
  DTYPE_ENDPUNC      = BIT_FLAG(18),
  DTYPE_WANSUNG_EX    = BIT_FLAG(19),    // 확장완성형
  DTYPE_NUMERIC      = BIT_FLAG(20),    // 언어에 따른 숫자
  DTYPE_SIGN      = BIT_FLAG(21)    // 언어에 따른 기호
} DHWR_TYPE;

typedef enum
{
  DSTATUS_DISABLE,
  DSTATUS_DISABLE_INSTALLABLE,
  DSTATUS_ENABLE,
  DSTATUS_ENABLE_INSTALLABLE,
}DHWR_LANGUAGE_STATUS;

/*!
*  @brief 인식 타입 종류(낱자 or 여러문자)
*/
typedef enum
{
  //! Recognize type for single character
  SINGLECHAR,
  //! Recognize type for multi-character
  MULTICHAR,
} DHWR_RECOGMODE;

//! SettingObject Handle
typedef const void* DHWRSettingObject;

/*!
 *  @brief 설정 오브젝트를 생성한다
 *  @return DHWRSettingObject
 */
DHWR_API DHWRSettingObject DHWRCreateSettingObject(void);

/*!
 *  @brief 설정 오브젝트를 제거한다
 *  @param [in] setting 설정 오브젝트
 *  @return
 */
DHWR_API void DHWRDestroySettingObject(DHWRSettingObject setting);

/*!
 *  @brief 필기영역을 설정한다
 *  @param [in] setting 설정 오브젝트
 *  @param [in] left 필기 영역 좌
 *  @param [in] top 필기 영역 상
 *  @param [in] right 필기 영역 우
 *  @param [in] bottom 필기 영역 하
 *  @param [in] size_rate 영어권 알파벳 대 소문자 구분을 위한 필기영역 가이드 라인 (%)
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned int DHWRSetWritingArea(DHWRSettingObject setting,
  const int left, const int top, const int right, const int bottom,
  const int size_rate);

/*!
 *  @brief 필기 인식 모드를 설정한다(낱자, 연속)
 *  @param [in] setting 설정 오브젝트
 *  @param [in] mode 인식 모드
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned int DHWRSetRecognitionMode(DHWRSettingObject setting,
  const DHWR_RECOGMODE mode);

/*!
 *  @brief 인식 후보 출력 크기를 설정한다(낱자, 연속)
 *  @param [in] setting 설정 오브젝트
 *  @param [in] size_cand 후보수
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned int DHWRSetCandidateSize(DHWRSettingObject setting,
  const int size_cand);

/*!
 *  @brief 인식할 언어를 추가 한다
 *  @param [in] setting 설정 오브젝트
 *  @param [in] lang 언어 값
 *  @param [in] option 언어에 따른 옵션
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned int DHWRAddLanguage(DHWRSettingObject setting,
  const DHWR_LANG lang, const DHWR_TYPE option);

/*!
 *  @brief 설정된 언어 크기를 가져온다
 *  @param [in] setting 설정 오브젝트
 *  @param [out] size 크기
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned int DHWRGetLanguageSize(DHWRSettingObject setting, int* size);

/*!
 *  @brief 설정된 언어를 초기화 한다
 *  @param [in] setting 설정 오브젝트
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned int DHWRClearLanguage(DHWRSettingObject setting);

/*!
 *  @brief 인식 시 이후에 추가로 입력이 들어오는지에 대한 정보를 설정한다 
 *  @param [in] setting 설정 오브젝트
 *  @param [in] type 입력 지속 여부 (true = 입력종료, false = 입력 지속)
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned int DHWRSetContinuity(DHWRSettingObject setting, const bool type);

/*!
 *  @brief 인식할 심볼을 정의 
 *  @param [in] setting 설정 오브젝트
 *  @param [in] charset 인식할 심볼의 리스트
 *  @return DHWR_ERROR_CODE
 */
DHWR_API unsigned int DHWRSetUserCharSet(DHWRSettingObject setting, 
  const unsigned short* charset);

/*@}*/

/**
 * @defgroup API
 */

/*@{*/
DHWR_API unsigned int DHWRGetLanguageStatus(const DHWR_LANG lang, unsigned short ** package, int *length);

/*@}*/


#ifdef __cplusplus
}
#endif

#endif // __DHWR_SETTING_OBJECT_H__
