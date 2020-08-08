#ifndef __DKBD_H__
#define __DKBD_H__

/* Common Definition *************************************/
/* Language -------------------------------------------- */
#define DKBD_LANG_SIMP_CHINESE           HWLANG_Simp_Chinese
#define DKBD_LANG_TRAD_CHINESE           HWLANG_Trad_Chinese
#define DKBD_LANG_ENGLISH_USA            HWLANG_English_USA
#define DKBD_LANG_JAPANESE               1
#define DKBD_LANG_KOREAN                 2

/* Input Method ---------------------------------------- */
#define DKBD_INPUTMETHOD_QWERTY          HWIM_KEYBOARD_NOTREDUCE
#define DKBD_INPUTMETHOD_KEYPAD          HWIM_KEYBOARD_REDUCE
#define DKBD_INPUTMETHOD_USER_DEFINE     HWIM_KEYBOARD_USERDEFINE

/* Input Mode ------------------------------------------ */
// Chinese
#define DKBD_INPUTMODE_PHONETIC          HWIM_MODE_PHONETIC
#define DKBD_INPUTMODE_STROKE            HWIM_MODE_STROKE
#define DKBD_INPUTMODE_CANGJIE           HWIM_MODE_CANGJIE
#define DKBD_INPUTMODE_ZHUYIN            HWIM_MODE_ZHUYIN
// Japanese
#define DKBD_INPUTMODE_HIRAGANA          6
#define DKBD_INPUTMODE_FULLWIDTH_KATAKAN 7
#define DKBD_INPUTMODE_HALFWIDTH_KATAKAN 8
// Korean
#define DKBD_INPUTMODE_QWERTY_SEBUL      9
#define DKBD_INPUTMODE_QWERTY_DUBUL      10
#define DKBD_INPUTMODE_CHYNJYIN          11
#define DKBD_INPUTMODE_SASANG_HANGUL     12
#define DKBD_INPUTMODE_MOTOROLA_HANGU    13
#define DKBD_INPUTMODE_NARAGUL_HANGUL    14
#define DKBD_INPUTMODE_SKY2_HANGUL       15
#define DKBD_INPUTMODE_SKY_SMART_HANGUL  16
#define DKBD_INPUTMODE_ENG 17

/* Multitap Index (for Japanese and Korean) ------------ */
#define DKBD_MULTITAP_KEY_IDX_0          0
#define	DKBD_MULTITAP_KEY_IDX_1          1
#define	DKBD_MULTITAP_KEY_IDX_2          2
#define	DKBD_MULTITAP_KEY_IDX_3          3
#define	DKBD_MULTITAP_KEY_IDX_4          4
#define	DKBD_MULTITAP_KEY_IDX_5          5
#define	DKBD_MULTITAP_KEY_IDX_6          6
#define	DKBD_MULTITAP_KEY_IDX_7          7
#define	DKBD_MULTITAP_KEY_IDX_8          8
#define	DKBD_MULTITAP_KEY_IDX_9          9
#define	DKBD_MULTITAP_KEY_IDX_STAR       10
#define	DKBD_MULTITAP_KEY_IDX_POUND      11
#define	DKBD_MULTITAP_KEY_IDX_BSPACE     12
#define DKBD_MULTITAP_KEY_IDX_TOGGLE     13 // for just Japanese
#define DKBD_MULTITAP_KEY_IDX_MOVE_LEFT  14
#define DKBD_MULTITAP_KEY_IDX_MOVE_RIGHT 15
#define DKBD_MULTITAP_KEY_IDX_MOVE_LEFT_END  16
#define DKBD_MULTITAP_KEY_IDX_MOVE_RIGHT_END 17
#define	DKBD_MULTITAP_KEY_IDX_MAX        18

/* Key code -------------------------------------------- */
#define DKBD_KEYCODE_SPACE_VALUE         32
#define DKBD_KEYCODE_BACKSPACE_VALUE     8
#define DKBD_KEYCODE_ENTER_VALUE         10

/* Chinese Definition ************************************/
/* Recognition Language -------------------------------- */
#define HWLANG_Simp_Chinese    0x1000 // Simplified Chinese, counterpart: ChsSysDic.dic
#define HWLANG_Trad_Chinese    0x1010 // Traditional Chinese, counterpart: ChtSysDic.dic
#define HWLANG_English_USA     0x0130 // English（US）counterpart: en-US.dic
#define HWLANG_English_GBR     0x0131 // English（GBR）counterpart: en-GB.dic

/* Define Character Set Range(Chinese)，Single-Choice -- */
// Simplified Chinese
#define ALC_GB2312         0x0001
#define ALC_GBK_ADDED      0x0002
#define ALC_GB18030_ADDED  0x0004
#define ALC_GBK            (ALC_GB2312 | ALC_GBK_ADDED)
#define ALC_GB18030        (ALC_GB2312 | ALC_GBK_ADDED | ALC_GB18030_ADDED)
// Traditional Chinese
#define ALC_BIG5           0x0001
#define ALC_HKSCS          0x0002

/* Fuzzy Pinyin Setting-Multiple-Choice ---------------- */
#define HW_FUZZY_Z_ZH      0x0001 // z<-->zh
#define HW_FUZZY_C_CH      0x0002 // c<-->ch
#define HW_FUZZY_S_SH      0x0004 // s<-->sh
#define HW_FUZZY_F_H       0x0008 // f<-->h
#define HW_FUZZY_K_G       0x0010 // k<-->g
#define HW_FUZZY_L_N       0x0020 // l<-->n
#define HW_FUZZY_L_R       0x0040 // l<-->r
#define HW_FUZZY_IAN_IANG  0x0080 // ian<-->iang
#define HW_FUZZY_UAN_UANG  0x0100 // uan<-->uang
#define HW_FUZZY_AN_ANG    0x0200 // an<-->ang
#define HW_FUZZY_EN_ENG    0x0400 // en<-->eng
#define HW_FUZZY_IN_ING    0x0800 // in<-->ing

/* Define Constant ------------------------------------- */
#define HWIM_MAX_PHONETIC_LEN           6  // the max length of pinyin(chinese)
#define HWRE_MAX_CHINESE_WORD_LEN       6  // the max length of word(chinese)
#define HWRE_MAX_LATIN_WORD_LEN         31 // the max length of word(latin)
#define HWRE_MAX_PHONETIC_GROUP_NUM     20 // the max number of pinyin group(chinese). This is available only in Pinyin Input and Bopomofo Input. The grouping is depend on the location of the first separator that automatic append to the input string.

#define HWRE_OUT_PHONETIC_BUFSIZE (HWRE_MAX_PHONETIC_GROUP_NUM * (HWIM_MAX_PHONETIC_LEN + 1)) // the max length of pinyin group(chinese)
#define HW_MAX_INPUTSTR_LEN       (HWRE_MAX_CHINESE_WORD_LEN * (HWIM_MAX_PHONETIC_LEN + 1))   // the max length of input string which can be entered,including manual separator
#define HW_MAX_INPUT_SHOW_LEN     (HW_MAX_INPUTSTR_LEN * 2)                                   // the max length of input string showed on the screen

/* Define Ram Size ------------------------------------- */
#define HWKEYIM_MIN_RAMSIZE  (46*1024) // buffer size that the engine request

/* Define Output Mode of Uppercase and Lowercase(English)*/
#define HWIM_ENGLISH_OUTPUT_SENSITIVITY 0 // Sensitive
#define HWIM_ENGLISH_OUTPUT_LOWERCASE   1 // output uppercase
#define HWIM_ENGLISH_OUTPUT_UPPERCASE   2 // output lowercase

/* Keyboard Mode --------------------------------------- */
#define HWIM_KEYBOARD_NOTREDUCE   1   // Instead of QWERTY Keyboard; one key mapping one symbol;
#define HWIM_KEYBOARD_REDUCE      2   // Compact Keyboard Mode; Instead of Numeric Keyboard; One key mapping more than one keys;
#define HWIM_KEYBOARD_USERDEFINE  3   // User customized layout; This layout can be set to one key mapping one symbol or mapping more than one keys;

/* Chinese Input Method -------------------------------- */
#define HWIM_MODE_PHONETIC       1   // Pinyin Input, Available Layout:HWIM_KEYBOARD_NOTREDUCE,HWIM_KEYBOARD_REDUCE,HWIM_KEYBOARD_USERDEFINE. HWIM_KEYBOARD_REDUCE is mean Numeric Keyboard layout, valid input symbols:'2-9' and HW_KEY_Separator_Code. HWIM_KEYBOARD_NOTREDUCE layout: valid symbols:'a-z' and HW_KEY_Separator_Code.
#define HWIM_MODE_STROKE         3   // Stroke Input: only HWIM_KEYBOARD_NOTREDUCE layout is valid; Valid input symbols:'1'-'5',HW_KEY_Separator_Code, HW_KEY_Wildcard_Code. HWIM_KEYBOARD_USERDEFINE is not available in this version.
#define HWIM_MODE_CANGJIE        4   // Cangjie Input: valid layout is HWIM_KEYBOARD_NOTREDUCE. Valid input:'a-y'. HWIM_KEYBOARD_USERDEFINE is not available in this version.
#define HWIM_MODE_ZHUYIN         5   // Bopomofo Input: Valid layout:HWIM_KEYBOARD_NOTREDUCE, HWIM_KEYBOARD_REDUCE,HWIM_KEYBOARD_USERDEFINE. HWIM_KEYBOARD_REDUCE layout is same with 祿莖layout, and 'a-z', HW_KEY_SpaceBar_Code, HW_KEY_Separator_Code are valid input symbols. HWIM_KEYBOARD_NOTREDUCE layout: all the Bopomofo symbols,HW_KEY_SpaceBar_Code and HW_KEY_Separator_Code are valid symbols for input.

#define HWIM_MAX_KEYPROJECT_NUN  8;  // Maximum number that can be mapped when the keyboard layout is HWIM_KEYBOARD_USERDEFINE.

#define HW_KEY_SpaceBar_Code      (0x20)
#define HW_KEY_Separator_Code     (0x27)
#define HW_KEY_Wildcard_Code      (0x2A)
#define HW_KEY_BackSpace_Code     (0x08)

/* Error code ------------------------------------------ */
#define HWERR_SUCCESS                     (0)     // Success
#define HWERR_INVALID_PARAM               (-1)    // Invalid parameter
#define HWERR_NOT_ENOUGH_MEMORY           (-2)    // Not enough memory
#define HWERR_INVALID_DATA                (-3)    // Invalid data
#define HWERR_INVALID_MEMORY              (-4)    // Invalid memory pointer
#define HWERR_READ_DICT_FAIL              (-5)    // Read dictionary failed
#define HWERR_HANDLE_NOT_ENOUGH           (-6)    // Not enough handle memory
#define HWERR_NO_FILE_NAME                (-7)    // Lack filename
#define HWERR_NO_LOADER_CALLBACK          (-8)    // Lack the callback function that loads the dictionary
#define HWERR_NO_CHINESE                  (-9)    // Not chinese input method
#define HWERR_NEED_INIT                   (-10)   // Need initialization
#define HWERR_NO_SYSDICT                  (-11)   // Not load System dictionary
#define HWERR_INVALID_INPUT               (-12)   // Invalid input
#define HWERR_INVALID_SYSDICT             (-13)   // Invalid system dictionary
#define HWERR_INVALID_USERDICT            (-14)   // Invalid user dictionary
#define HWERR_NO_RELEASER_CALLBACK        (-15)   // Lack the callback function that releases the dictionary
#define HWERR_INVALID_POINTER             (-16)   // Invalid pointer
#define HWERR_WRITE_FILE_FAIL             (-17)   // Write file failed
#define HWERR_NEED_WORKSPACE              (-18)   // Not enough workspace
#define HWERR_NOT_SUPORT_LANGUAGE         (-19)   // Not support the language choosed
#define HWERR_NOT_SUPORT_COMMAND          (-20)   // Not support the command choosed
#define HWERR_OPEN_FILE_FAIL              (-21)   // Fail while try to open a specified file
#define HWERR_NOT_SUPPORT_KEYBOARDMODE    (-22)   // This keyboard layout is not available in this mode

/* Korean Definition *************************************/
// Flag for available module
#define USE_LAN_KOR_QWERTY_SEBUL     0x01
#define USE_LAN_KOR_QWERTY_DUBUL     0x02
#define USE_LAN_KOR_CHYNJYIN_HANGUL  0x04
#define USE_LAN_KOR_SASANG_HANGUL    0x08
#define USE_LAN_KOR_MOTOROLA_HANGUL  0x10
#define USE_LAN_KOR_NARAGUL_HANGUL   0x20
#define USE_LAN_KOR_SKY2_HANGUL      0x40
#define USE_LAN_KOR_SKY_SMART_HANGUL 0x80

// for strInput array
#define MULTITAP_HANGUL_KEY_MAP_SIZE 36
#define MAX_STRING_INPUT_NUM         3

// CJI only
#define CHUNJYIN_CHUN    0x00B7    // .
#define CHUNJYIN_DCHUN   0x2025    // ..
#define CHUNJYIN_JY      0x3161    // ㅡ
#define CHUNJYIN_IN      0x3163    // ㅣ

// Sasang only
#define SASANG_VOWL_LARGE_V 0x3163;    // 긴 ㅣ
#define SASANG_VOWL_LARGE_H 0X3161;    // 긴 ㅡ
#define SASANG_VOWL_SMALL_V 0x0131;    // 짧은 ㅣ
#define SASANG_VOWL_SMALL_H 0x00AD;    // 짧은 ㅡ

// MultiTab || Half-Qwerty add-stroke
#define NARAGUL_STROKE_ADD_QWERTY    0xD68D    // NARAGUL Qwerty 획 추가        ('획')
#define NARAGUL_STROKE_ADD_MULTITAP  0xFF0A    // NARAGUL multitab 획 추가      ('*')
#define NARAGUL_DSTROKE_ADD_MULTITAP 0x0023    // NARAGUL multitab 쌍 자음 추가 ('#')
#define MOTO_STROKE_ADD_QWERTY       0xD68D    // Moto Qwerty 획 추가           ('획')
#define MOTO_STROKE_ADD_MULTITAP     0x0023    // Moto multitab 획 추가         ('#') 
#define SASANG_DSTROKE_ADD_MULTITAP  0x0023    // Sasang multitab 쌍 자음 추가  ('#')

#define ECompositionResult_None                 0   // 조합 결과가 없을 때.
#define ECompositionResult_Exist                1   // 조합 결과가 있을 때.
#define ECompositionError_MemoryIsTooSmall     -1   // pszResult의 size가 작을 때.
#define ECompositionError_InvalidMultiTapState -2   // MultiTapState가 올바른 값이 아닐 때.
#define ECompositionError_InvalidKeyType       -3   // KeyType가 올바른 값이 아닐 때.
#define ECompositionResult_InvaliAutomataType  -4   // Automata Type이 올바르지 않을 때.
#define ECompositionResult_NotSupportModel     -5   // 지원하지 않는 모델 일 때.

#define EHangulKeyType_None 0
#define EHangulKeyType_Cho  1
#define EHangulKeyType_Jung 2
#define EHangulKeyType_Jong 3

#ifdef __cplusplus
extern "C" {
#endif

/* Function ***********************************************/
// Common
bool DKBD_Create(void);
bool DKBD_Destroy(void);
void DKBD_SetLanguage(int language);
int DKBD_GetLanguage(void);
void DKBD_SetKeyboard(int inputMethod, int inputMode);
int DKBD_GetInputMethod(void);
int DKBD_GetInputMode(void);

// Chinese
int DKBD_CNInit();
int DKBD_CNSetWorkSpace(char* pcRam, long lRamSize);
int DKBD_CNSetDictPath (char* filePath);
int DKBD_CNSetLanguageDict(int nLanguage, const char* pszDictFilePathName);
int DKBD_CNReleaseLanguageDict();
int DKBD_CNGetLanguage();
int DKBD_CNSetUserDict(const char* pszDictFilePathName);
int DKBD_CNSaveUserDict(const char* pszDictFilePathName);
int DKBD_CNReleaseUserDict();
int DKBD_CNSetKeyboardMode(int nKeyboard);
int DKBD_CNGetKeyboardMode();
int DKBD_CNSetChineseInputMode(int nChineseInputMode);
int DKBD_CNGetChineseInputMode();
int DKBD_CNSetChineseFuzzy(int nFuzzyCode);
int DKBD_CNGetChineseFuzzy();
int DKBD_CNSetChineseCharset(int nCharset);
int DKBD_CNGetChineseCharset();
int DKBD_CNSetEnglishCapital(int nLatinCapital);
int DKBD_CNGetEnglishCapital();
int DKBD_CNSetChineseEnglishMix(int nIsMix, int nLanguage);
int DKBD_CNGetChineseEnglishMix();
int DKBD_CNChangeDispCode(unsigned short* pwSrcDispCode, unsigned short* pwDstDispCode);
int DKBD_CNInputStrClean();
int DKBD_CNAddChar(unsigned short wChar);
int DKBD_CNAddStr(unsigned short* pwAddStr);
int DKBD_CNGetInputTransResult(unsigned short* pwOutString);
int DKBD_CNGetPhoneticGroup(unsigned short* pwOutPhoneticGroups);
int DKBD_CNGetWordCandidates(int nSelectPhoneIndex, int nStartWordIndex, unsigned short* pwCandWordBuf, int nCandWordBufSize);
int DKBD_CNChooseCandWord(int nWordIndex);
int DKBD_CNIsFinish();
int DKBD_CNGetFullPhonetic(unsigned short* pwFullPhonetic);
int DKBD_CNUpdateUserDict(const unsigned short* pwFullPhonetic, const unsigned short* pwWord);
int DKBD_CNDeleteUserWord(const unsigned short* pwFullPhonetic, const unsigned short* pwWord, int wordType);
int DKBD_CNGetPredictResult(unsigned short* pwInWordPrefix, unsigned short* pwInPhonetic, unsigned short* pwCandWordBuf, int nCandWordBufSize);
int DKBD_CNGetSrcSequence(unsigned short wUnicode, unsigned short* pwSrcCode);
int DKBD_CNTradSimpConvert(unsigned short* pwDstCode, int iDstBufLen, unsigned short* pwSrcCode, int iConvertFlag);

#ifdef __cplusplus
}
#endif
#endif // __DKBD_H__
