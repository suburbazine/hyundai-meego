#include <QDebug>
#include "DHAVN_QwertyKeypad_Automata_Korean.h"
#include <QTime>
#include "DHAVN_QwertyKeypad_Log.h"

#define KEY_SPACE       (0x0020)
#define KEY_

const int BASE_CODE = 0xac00;		// Jamo base code ()
const int LIMIT_MIN = 0xac00;		// Minumum jamo value ()
const int LIMIT_MAX = 0xd7a3;		// Maximum jamo value

// Jamo table
/*
wchar_t SOUND_TABLE[68] =
{
    // lead consonant (19) 0 ~ 18
    _T(''), _T(''), _T(''), _T(''), _T(''),
    _T(''), _T(''), _T(''), _T(''), _T(''),
    _T(''), _T(''), _T(''), _T(''), _T(''),
    _T(''), _T(''), _T(''), _T(''),

    // vowel (21) 19 ~ 39
    _T(''), _T(''), _T(''), _T(''), _T(''),
    _T(''), _T(''), _T(''), _T(''), _T(''),
    _T(''), _T(''), _T(''), _T(''), _T(''),
    _T(''), _T(''), _T(''), _T(''), _T(''),
    _T(''),

    // tail consonant (28) 40 ~ 67
    _T(' ') , _T(''), _T(''), _T(''), _T(''),
    _T(''), _T(''), _T(''), _T(''), _T(''),
    _T(''), _T(''), _T(''), _T(''), _T(''),
    _T(''), _T(''), _T(''), _T(''), _T(''),
    _T(''), _T(''), _T(''), _T(''), _T(''),
    _T(''), _T(''), _T('')
};
*/

wchar_t SOUND_TABLE[68] =
{
    // lead consonant (19) 0 ~ 18
    0x3131, 0x3132, 0x3134, 0x3137, 0x3138, // '', '', '', '', '',
    0x3139, 0x3141, 0x3142, 0x3143, 0x3145, // '', '', '', '', '',
    0x3146, 0x3147, 0x3148, 0x3149, 0x314A, // '', '', '', '', '',
    0x314B, 0x314C, 0x314D, 0x314E,         // '', '', '', '',

    // vowel (21) 19 ~ 39
    0x314F, 0x3150, 0x3151, 0x3152, 0x3153, // '', '', '', '', '',
    0x3154, 0x3155, 0x3156, 0x3157, 0x3158, // '', '', '', '', '',
    0x3159, 0x315A, 0x315B, 0x315C, 0x315D, // '', '', '', '', '',
    0x315E, 0x315F, 0x3160, 0x3161, 0x3162, // '', '', '', '', '',
    0x3163,                                 // '',

    // tail consonant (28) 40 ~ 67
    0x0020, 0x3131, 0x3132, 0x3133, 0x3134, // ' ', '', '', '', '',
    0x3135, 0x3136, 0x3137, 0x3139, 0x313A, // '', '', '', '', '',
    0x313B, 0x313C, 0x313D, 0x313E, 0x313F, // '', '', '', '', '',
    0x3140, 0x3141, 0x3142, 0x3144, 0x3145, // '', '', '', '', '',
    0x3146, 0x3147, 0x3148, 0x314A, 0x314B, // '', '', '', '', '',
    0x314C, 0x314D, 0x314E                  // '', '', ''
};

int COMPATIBILITY_JAMO_TABLE[51] =
{
     0, //''
     1, //''
    -1, //''
     2, //''
    -1, -1, //'', ''
     3, //''
     4, //''
     5, //''
    -1, -1, -1, -1, -1, -1, -1, // '', '', ', '', '', ''
     6, //''
     7, //''
     8, //''
    -1, //''
     9, //''
    10, //''
    11, //''
    12, //''
    13, //''
    14, //''
    15, //''
    16, //''
    17, //''
    18, //'',
    19, //''
    20, //''
    21, //''
    22, //''
    23, //''
    24, //''
    25, //''
    26, //''
    27, //''
    -1, -1, -1, //'', '', ''
    31, //''
    32, //''
    -1, -1, -1, //'', '', ''
    36, //''
    37, //''
    -1, //''
    39  //'',
};

// Vowel composition table
int MIXED_VOWEL[9][3] =
{
    {27,19,28},	// ,,
    {27,39,30},	// ,,
    {27,20,29},	// ,,
    {28,39,29},	// ,,

    {32,23,33},	// ,,
    {32,24,34},	// ,,
    {33,39,34},	// ,,
    {32,39,35},	// ,,

    {37,39,38}	// ,,
};

// Tail consonant composition table
int MIXED_JONG_CONSON[11][3] =
{
    {41,59,43}, // ,,
    {44,62,45}, // ,,
    {44,67,46}, // ,,
    {48,41,49}, // ,,
    {48,56,50}, // ,,
    {48,57,51}, // ,,
    {51,57,54}, // ,,
    {48,59,52}, // ,,
    {48,65,53}, // ,,
    {48,67,55}, // ,,
    {57,59,58} // ,,
};

// Tail consonant decomposition table
int DIVIDE_JONG_CONSON[11][3] =
{
    {41,59,43}, // ,,
    {44,62,45}, // ,,
    {44,67,46}, // ,,
    {48,41,49}, // ,,
    {48,56,50}, // ,,
    {48,57,51}, // ,,
    {48,66,54}, // ,,
    {48,59,52}, // ,,
    {48,65,53}, // ,,
    {48,67,55}, // ,,
    {57,59,58} // ,,
};

QwertyKeypadAutomataKorean::QwertyKeypadAutomataKorean(void)
{
    memset(m_nPhonemez,0, sizeof(m_nPhonemez));
    m_nAutomataID = 0;
    reset();
}

QwertyKeypadAutomataKorean::~QwertyKeypadAutomataKorean(void)
{
}

// Initialization
void QwertyKeypadAutomataKorean::reset()
{
    m_nStatus = m_nPrevStatus = HS_FIRST;
    completeText    = KOR("");
    ingWord         = 0;
    m_completeWord  = 0;
}

bool QwertyKeypadAutomataKorean::isComposing()
{
    // true : ~ing
    // false : end
   return (m_nPrevStatus!=HS_FIRST ? true : false);
    // return (ingWord!=0 ? true : false);
}

void QwertyKeypadAutomataKorean::setAutomataState( int nStatus )
{
    m_nPrevStatus = m_nStatus;
    m_nStatus = nStatus;

    return;
}

QString QwertyKeypadAutomataKorean::makeWord(int nKeyCode, QString strKeyCode)
{
    QString strText;
    int nStrKeyCode = strKeyCode.data()[0].unicode();

    //qCritical() << "makeWord()::strKeyCode - "<< strKeyCode;
    //qCritical() << "makeWord()::nStrKeyCode - " << nStrKeyCode;

    if( isKorean(nStrKeyCode) )
    {
        unsigned int nIndex = nStrKeyCode - 0x3131;// 0x3131 : ''
        if(nIndex < (sizeof(COMPATIBILITY_JAMO_TABLE)/sizeof(int)))
        {
            SetKoreanKeyCode(COMPATIBILITY_JAMO_TABLE[nIndex]);
        }
        else
        {
            //qCritical()<<"makeWord(): Out of table range"; //added for Log System Lock Issue
            LOG2(QString("makeWord(): Out of table range"), Logger::PRIORITY_HI); //added for Log System Lock Issue
        }
    }
    else
    {
        SetKeyCode(nKeyCode);
    }
    //strText = completeText;
    if( m_completeWord != 0)
        strText = KOR_W(m_completeWord);
    if(ingWord != 0)
        strText += KOR_W(ingWord);

    return strText;
}

bool QwertyKeypadAutomataKorean::isKorean( int nKeyCode )
{
    if( (nKeyCode >= 0x3130 && nKeyCode <= 0x318F) ||
        (nKeyCode >= 0xAC00 && nKeyCode <= 0xD7AF ) )
        return true;

    return false;
}

// Composition keycode
wchar_t QwertyKeypadAutomataKorean::SetKeyCode(int nKeyCode)
{
    m_completeWord = 0;
    if(nKeyCode == Qt::Key_Back)
    {
        if(ingWord == 0)
        {
            if(completeText.length() > 0)
            {
                if(completeText.right(1) == KOR("\n"))
                    completeText = completeText.left(completeText.length() -2);
                else
                    completeText = completeText.left(completeText.length() -1);
            }
        }
        else
        {
            setAutomataState( DownGradeIngWordStatus(ingWord) );
        }
    }
    else
    {
        setAutomataState(HS_FIRST);
        ingWord = nKeyCode & 0xFFFF;
        m_nPrevStatus = HS_FIRST;
    }

    return m_completeWord;
}


// Composition hangul keycode
wchar_t QwertyKeypadAutomataKorean::SetKoreanKeyCode(int nKeyCode)
{
    m_completeWord = 0;

    switch(m_nStatus)
    {
    case HS_FIRST:
        // Lead consonant
        m_nPhonemez[0]	= nKeyCode;
        ingWord		= SOUND_TABLE[m_nPhonemez[0]];
        setAutomataState( nKeyCode > 18 ? HS_FIRST : HS_FIRST_V );
        break;

    case HS_FIRST_C:
        // vowel + vowel
        if(nKeyCode > 18)	// vowel
        {
            if(MixVowel(&m_nPhonemez[0], nKeyCode) == FALSE)
            {
                m_completeWord = SOUND_TABLE[m_nPhonemez[0]];
                m_nPhonemez[0] = nKeyCode;
            }
        }
        else				// consonant
        {
            m_completeWord	= SOUND_TABLE[m_nPhonemez[0]];
            m_nPhonemez[0]	= nKeyCode;
            setAutomataState( HS_FIRST_V );
        }
        break;

    case HS_FIRST_V:
        // consonant + consonant
        if(nKeyCode > 18)	// vowel
        {
            m_nPhonemez[1]	= nKeyCode;
            setAutomataState( HS_MIDDLE_STATE );
        }
        else			// consonant
        {
            m_completeWord	= SOUND_TABLE[m_nPhonemez[0]];
            m_nPhonemez[0]	= nKeyCode;
            setAutomataState( HS_FIRST_V );
        }
        break;

    case HS_MIDDLE_STATE:
        // consonant + vowel + vowel
        if(nKeyCode > 18)
        {
            if(MixVowel(&m_nPhonemez[1], nKeyCode) == FALSE)
            {
                m_completeWord	= CombineHangle(1);
                m_nPhonemez[0]	= nKeyCode;
                setAutomataState( HS_FIRST_C );
            }
        }
        else
        {
            int jungCode = ToFinal(nKeyCode);

            if(jungCode > 0)
            {
                m_nPhonemez[2]	= jungCode;
                setAutomataState( HS_END_STATE );
            }
            else
            {
                m_completeWord	= CombineHangle(1);
                m_nPhonemez[0]	= nKeyCode;
                setAutomataState( HS_FIRST );
            }
        }
    break;

    case HS_END:
        // (lead consonant) + (vowel) + (tail consonant)
        if(nKeyCode > 18)
        {
            m_completeWord	= CombineHangle(1);
            m_nPhonemez[0]	= nKeyCode;
            setAutomataState( HS_FIRST );
        }
        else
        {
            int jungCode = ToFinal(nKeyCode);
            if(jungCode > 0)
            {
                m_nPhonemez[2]	= jungCode;
                setAutomataState( HS_END_STATE );
            }
            else
            {
                m_completeWord	= CombineHangle(1);
                m_nPhonemez[0]	= nKeyCode;
                setAutomataState( HS_FIRST );
            }
        }
        break;

    case HS_END_STATE:
        // ʼ + ߼ + () + ()
        if(nKeyCode > 18)
        {
            m_completeWord = CombineHangle(1);

            m_nPhonemez[0] = ToInitial(m_nPhonemez[2]);
            m_nPhonemez[1] = nKeyCode;
            setAutomataState( HS_MIDDLE_STATE );
        }
        else
        {
            int jungCode = ToFinal(nKeyCode);
            if(jungCode > 0)
            {
                setAutomataState( HS_END_EXCEPTION );

                if(!MixFinal(jungCode))
                {
                    m_completeWord	= CombineHangle(2);
                    m_nPhonemez[0]	= nKeyCode;
                    setAutomataState( HS_FIRST_V );
                }
            }
            else
            {
                m_completeWord	= CombineHangle(2);
                m_nPhonemez[0]	= nKeyCode;
                setAutomataState( HS_FIRST_V );
            }
        }
        break;

    case HS_END_EXCEPTION:
        // (lead consonant) + (vowel) + (compatibility tail consonant)
        if(nKeyCode > 18)
        {
            DecomposeConsonant();
            m_nPhonemez[1]	= nKeyCode;
            setAutomataState( HS_MIDDLE_STATE );
        }
        else
        {
            int jungCode = ToFinal(nKeyCode);
            if(jungCode > 0)
            {
                setAutomataState( HS_END_EXCEPTION );

                if(!MixFinal(jungCode))
                {
                    m_completeWord	= CombineHangle(2);
                    m_nPhonemez[0]	= nKeyCode;
                    setAutomataState( HS_FIRST_V );
                }
            }
            else
            {
                m_completeWord	= CombineHangle(2);
                m_nPhonemez[0]	= nKeyCode;
                setAutomataState( HS_FIRST_V );
            }
        }
        break;
    }
    CombineIngWord(m_nStatus);
    if(m_completeWord != 0)
        completeText += KOR_W(m_completeWord);

    return m_completeWord;
}

// Convert to lead consonant
int QwertyKeypadAutomataKorean::ToInitial(int nKeyCode)
{
    switch(nKeyCode)
    {
    case 41: return 0;	//
    case 42: return 1;	//
    case 44: return 2;	//
    case 47: return 3;	//
    case 48: return 5;	//
    case 56: return 6;	//
    case 57: return 7;	//
    case 59: return 9;	//
    case 60: return 10;	//
    case 61: return 11;	//
    case 62: return 12;	//
    case 63: return 14;	//
    case 64: return 15;	//
    case 65: return 16;	//
    case 66: return 17;	//
    case 67: return 18;	//
    }
    return -1;
}

// Convert to tail consonant
int QwertyKeypadAutomataKorean::ToFinal(int nKeyCode)
{
    switch(nKeyCode)
    {
    case 0: return 41;	//
    case 1: return 42;	//
    case 2: return 44;	//
    case 3: return 47;	//
    case 5: return 48;	//
    case 6: return 56;	//
    case 7: return 57;	//
    case 9: return 59;	//
    case 10: return 60;	//
    case 11: return 61;	//
    case 12: return 62;	//
    case 14: return 63;	//
    case 15: return 64;	//
    case 16: return 65;	//
    case 17: return 66;	//
    case 18: return 67;	//
    }
    return -1;
}

// Decomposition consonant
void QwertyKeypadAutomataKorean::DecomposeConsonant()
{
    if(m_nPhonemez[3] > 40 || m_nPhonemez[4] > 40)
    {
        int i = 0;
        do
        {
            if(DIVIDE_JONG_CONSON[i][2] == m_nPhonemez[2])
            {
                m_nPhonemez[3] = DIVIDE_JONG_CONSON[i][0];
                m_nPhonemez[4] = DIVIDE_JONG_CONSON[i][1];

                m_completeWord = CombineHangle(3);
                m_nPhonemez[0]	 = ToInitial(m_nPhonemez[4]);
                return;
            }
        }
        while(++i< 11);
    }

    m_completeWord = CombineHangle(1);
    m_nPhonemez[0]	 = ToInitial(m_nPhonemez[2]);
}

// Composition consonant for lead consonant
bool QwertyKeypadAutomataKorean::MixFinal(int nKeyCode)
{
    if(nKeyCode <= 40) return FALSE;

    int i = 0;
    do
    {
        if(MIXED_JONG_CONSON[i][0] == m_nPhonemez[2] && MIXED_JONG_CONSON[i][1] == nKeyCode)
        {
            m_nPhonemez[3] = m_nPhonemez[2];
            m_nPhonemez[4] = nKeyCode;
            m_nPhonemez[2] = MIXED_JONG_CONSON[i][2];

            return TRUE;
        }
    }
    while(++i < 11);

    return FALSE;
}

// Composition vowel
bool QwertyKeypadAutomataKorean::MixVowel(int * currentCode, int inputCode)
{
    int i = 0;
    do
    {
        if(MIXED_VOWEL[i][0] == * currentCode && MIXED_VOWEL[i][1] == inputCode)
        {
            * currentCode = MIXED_VOWEL[i][2];
            return TRUE;
        }
    }
    while(++i< 9);

    return FALSE;
}

// Make korean letter
wchar_t QwertyKeypadAutomataKorean::CombineHangle(int cho, int jung, int jong)
{
    // (lead consonant) * 21 * 28 + ( vowel - 19) * 28 + ((tail consonant) - 40) + BASE_CODE;
    return BASE_CODE - 572 + jong + cho * 588 + jung * 28;
}

wchar_t QwertyKeypadAutomataKorean::CombineHangle(int status)
{
    switch(status)
    {
    // (lead consonant) + (vowel)
    case 1: return CombineHangle(m_nPhonemez[0], m_nPhonemez[1], 40);

    // (lead consonant) + (vowel) + (tail consonant)
    case 2: return CombineHangle(m_nPhonemez[0], m_nPhonemez[1], m_nPhonemez[2]);

    // (lead consonant) + (vowel) + (compatibility tail consonant)
    case 3: return CombineHangle(m_nPhonemez[0], m_nPhonemez[1], m_nPhonemez[3]);
    }
    return ' ';
}

void QwertyKeypadAutomataKorean::CombineIngWord(int status)
{
    Q_UNUSED(status);

    switch(m_nStatus)
    {
        case HS_FIRST:
        case HS_FIRST_V:
        case HS_FIRST_C:
            ingWord = SOUND_TABLE[m_nPhonemez[0]];
            break;

        case HS_MIDDLE_STATE:
        case HS_END:
            ingWord = CombineHangle(1);
            break;

        case HS_END_STATE:
        case HS_END_EXCEPTION:
            ingWord = CombineHangle(2);
            break;
    }
}

int QwertyKeypadAutomataKorean::DownGradeIngWordStatus(wchar_t word)
{
    int iWord = word;

    // Case) Only exist lead consonant
    if(iWord < LIMIT_MIN || iWord > LIMIT_MAX)
    {
        ingWord = 0;
        return HS_FIRST;
    }

    // (Korean letter) = (lead consonant) * (21*28) + (vowel) * 28 + (tail consonant) * 27 + BASE_CODE;
    int totalWord	= iWord - BASE_CODE;
    int iFirstWord	= totalWord / 28 / 21;
    int iMiddleWord = totalWord / 28 % 21;
    int iLastWord	= totalWord % 28;

    m_nPhonemez[0] = iFirstWord;

    if(iLastWord == 0)
    {
        ingWord = SOUND_TABLE[m_nPhonemez[0]];
        return HS_FIRST_V;
    }

    m_nPhonemez[1] = iMiddleWord + 19;
    iLastWord += 40;

    for(int i = 0; i < 11; i++)
    {
        if(iLastWord == DIVIDE_JONG_CONSON[i][2])
        {
            ingWord = CombineHangle(3);
            m_nPhonemez[2] = DIVIDE_JONG_CONSON[i][0];
            return HS_END_EXCEPTION;
        }
    }
    ingWord = CombineHangle(1);

    return HS_MIDDLE_STATE;
}
