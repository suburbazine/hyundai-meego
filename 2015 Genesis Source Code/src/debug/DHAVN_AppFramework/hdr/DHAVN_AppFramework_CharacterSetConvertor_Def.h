/**
 * DHAVN_AppFramework_CharacterSetDetector.h
 *
 * Declaration of the class CharacterSetDetector
 * @author Alexander.Golyshkin@teleca.com
 *
 * At this momemt need to move current functionality from applications to AFW
 * Second step will be to find way how to determinate current text codepage and convert it to
 * human readable format
 */

#ifndef DHAVN_APPFRAMEWORK_CHARACTERSETCONVERTOR_DEF_H
#define DHAVN_APPFRAMEWORK_CHARACTERSETCONVERTOR_DEF_H

#include <QString>

typedef enum
{
   CHARSET_CODEPAGE_NONE,
   CHARSET_CODEPAGE_UTF8,
   CHARSET_CODEPAGE_UTF16,
   CHARSET_CODEPAGE_EUC_JP,
   CHARSET_CODEPAGE_SJIS,
   CHARSET_CODEPAGE_ISO_2022_JP,
   CHARSET_CODEPAGE_BIG5,
   CHARSET_CODEPAGE_GB2312,
   CHARSET_CODEPAGE_GB18030,
   CHARSET_CODEPAGE_EUC_KR,
   CHARSET_CODEPAGE_JOHAB,
   CHARSET_CODEPAGE_ISO_2022_KR,
   CHARSET_CODEPAGE_ISO_8859_6,
   CHARSET_CODEPAGE_CP1253,
   CHARSET_CODEPAGE_ISO_8859_7,
   CHARSET_CODEPAGE_CP1251,
   CHARSET_CODEPAGE_CP1256,
   CHARSET_CODEPAGE_KOI8_U,
   CHARSET_CODEPAGE_KOI8_R,
   CHARSET_CODEPAGE_ISO_8859_2,
   CHARSET_CODEPAGE_ISO_8859_5,
   CHARSET_CODEPAGE_CP1255,
   CHARSET_CODEPAGE_ISO_8859_8_I,
   CHARSET_CODEPAGE_CP1250,
   CHARSET_CODEPAGE_CP1254,
   CHARSET_CODEPAGE_ISO_8859_9,
   CHARSET_CODEPAGE_CP1257,
   CHARSET_CODEPAGE_ISO_8859_13

} CHARSET_CODEPAGE_T;

typedef struct
{
   CHARSET_CODEPAGE_T codepage;
   QString codepageString;
   QString codepageQtCodecString;

} CHARSET_CODEPAGE_QT_MAP_T;

const CHARSET_CODEPAGE_QT_MAP_T CHARSET_CODEPAGE_MAP[] =
{
   { CHARSET_CODEPAGE_EUC_JP,       "EUC-JP",      "EUC-JP"       }, // Japan
   { CHARSET_CODEPAGE_SJIS,         "SJIS",        "Shift-JIS"    }, // Japan
   { CHARSET_CODEPAGE_ISO_2022_JP,  "ISO-2022-JP", "ISO 2022-JP"  }, // Japan
   { CHARSET_CODEPAGE_BIG5,         "BIG5",        "Big5"         }, // Taiwanese
   { CHARSET_CODEPAGE_GB2312,       "GB2312",      "GB18030-0"    }, // Chinese
   { CHARSET_CODEPAGE_GB18030,      "GB18030",     "GB18030-0"    }, // Chinese
   { CHARSET_CODEPAGE_EUC_KR,       "EUC-KR",      "EUC-KR"       }, // Korean
   { CHARSET_CODEPAGE_JOHAB,        "JOHAB",       "EUC-KR"       }, // Korean
   { CHARSET_CODEPAGE_ISO_2022_KR,  "ISO-2022-KR", "EUC-KR"       }, // Korean
   { CHARSET_CODEPAGE_ISO_8859_2,   "ISO-8859-2",  "ISO 8859-2"   }, // Polish
   { CHARSET_CODEPAGE_ISO_8859_5,   "ISO-8859-5",  "ISO 8859-5"   }, // Russian
   { CHARSET_CODEPAGE_ISO_8859_6,   "ISO-8859-6",  "ISO 8859-6"   }, // Arabic
   { CHARSET_CODEPAGE_ISO_8859_7,   "ISO-8859-7",  "ISO 8859-7"   }, // Greek
   { CHARSET_CODEPAGE_ISO_8859_8_I, "ISO-8859-8",  "ISO 8859-8"   }, // Hebrew
   { CHARSET_CODEPAGE_ISO_8859_9,   "ISO-8859-9",  "ISO 8859-9"   }, // Turkish
   { CHARSET_CODEPAGE_ISO_8859_13,  "ISO-8859-13", "ISO 8859-13"  }, // Baltic
   { CHARSET_CODEPAGE_CP1250,       "CP1250",      "Windows-1250" }, // Polish
   { CHARSET_CODEPAGE_CP1251,       "CP1251",      "Windows-1251" }, // Russian
   { CHARSET_CODEPAGE_CP1253,       "CP1253",      "Windows-1253" }, // Greek
   { CHARSET_CODEPAGE_CP1254,       "CP1254",      "Windows-1254" }, // Turkish
   { CHARSET_CODEPAGE_CP1255,       "CP1255",      "Windows-1255" }, // Hebrew
   { CHARSET_CODEPAGE_CP1256,       "CP1256",      "Windows-1256" }, // Arabic
   { CHARSET_CODEPAGE_CP1257,       "CP1257",      "Windows-1257" }, // Baltic
   { CHARSET_CODEPAGE_KOI8_U,       "KOI8-U",      "KOI8-U"       }, // Russian
   { CHARSET_CODEPAGE_KOI8_R,       "KOI8-R",      "KOI8-R"       }, // Russian

   { CHARSET_CODEPAGE_NONE,         "",            ""             }
};

typedef enum
{
   CHARSET_REGION_NONE,
   CHARSET_REGION_JP,
   CHARSET_REGION_TW,
   CHARSET_REGION_CN,
   CHARSET_REGION_KR,
   CHARSET_REGION_RU,
   CHARSET_REGION_AR,
   CHARSET_REGION_TR,
   CHARSET_REGION_GR,
   CHARSET_REGION_HW,
   CHARSET_REGION_PL,
   CHARSET_REGION_BL

} CHARSET_REGION_T;

class CharacterSetConvertor
{
public:

   CharacterSetConvertor();

   CharacterSetConvertor( const QString &aInputString );

   ~CharacterSetConvertor();

   const QString & Convert( bool &aStatus );

   const QString & Convert( const QString &aInputString, bool &aStatus );

   CHARSET_REGION_T GetRegion() const  { return m_Region; }

   CHARSET_CODEPAGE_T GetLastConvertedCodePage() const { return m_CodePage; }

private:

   bool IsUTF8();

   void SetRegion();

private:

   QString m_InputString, m_OutputString;

   CHARSET_REGION_T m_Region;

   CHARSET_CODEPAGE_T m_CodePage;
};

#endif // DHAVN_APPFRAMEWORK_CHARACTERSETCONVERTOR_DEF_H
