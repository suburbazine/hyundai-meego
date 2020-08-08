#ifndef QWERTYKEYPAD_AUTOMATA_KOREAN_H
#define QWERTYKEYPAD_AUTOMATA_KOREAN_H

#include <QString>
#include "DHAVN_QwertyKeypad_Automata.h"


#define	KEY_CODE_SPACE		-1  // Space
#define	KEY_CODE_ENTER		-2  // Enter
#define	KEY_CODE_BACKSPACE	-3  // Backspace

#define KOR(str)   QString::fromLocal8Bit(str)      // change "char" to QString
#define KOR_W(str) QString::fromWCharArray(&str, 1)     // for "wide char" to QString

class QwertyKeypadAutomataKorean : public QwertyKeypadAutomata
{

public:
    QwertyKeypadAutomataKorean(void);
    ~QwertyKeypadAutomataKorean(void);

public:
    void	reset();                            // Clear buffer
    QString     makeWord(int nKeyCode, QString strKeyCode);
    bool        isComposing();
    bool        isKorean( int nKeyCode );
    void        setAutomataState( int nStatus );
    wchar_t	SetKeyCode(int	nKeyCode);          // Composite keycode
    wchar_t	SetKoreanKeyCode(int	nKeyCode);  // Composite keycode
    wchar_t	ingWord;                            // Compositing word
    QString	completeText;                       // Composited word

private:
    enum HAN_STATUS		// Automata state
    {
        HS_FIRST = 0,		// (lead consonant)
        HS_FIRST_V,		// (consonant) + (consonant)
        HS_FIRST_C,		// (vowel) + (vowel)
        HS_MIDDLE_STATE,	// (lead consonant) + (vowel) + (vowel)
        HS_END,			// (lead consonant) + (vowel) + (tail consonant)
        HS_END_STATE,		// (lead consonant) + (vowel) + (consonant) + (consonant)
        HS_END_EXCEPTION	// (lead consonant) + (vowel) + (compatibility tail consonant)
    };

    int		m_nPrevStatus;	// Previous automata state
    int		m_nStatus;	// Current automata state
    int		m_nPhonemez[5]; // Save sequence [tail consonant, vowel, tail consonant, compatibility consonant 1, compatibility consonant 1]
    wchar_t	m_completeWord;	// Composited word.

    int		ToInitial(int nKeyCode);                // to make lead consonant
    int		ToFinal(int nKeyCode);                  // to make tail consonant
    void	DecomposeConsonant();                   // decomposition consonant
    bool	MixInitial(int nKeyCode);               // Composite consonant
    bool	MixFinal(int nKeyCode);                 // Composite vowel
    bool	MixVowel(int * nCurCode, int nInCode);	// Composite vowel

    wchar_t	CombineHangle(int cho, int jung, int jong);
    wchar_t	CombineHangle(int status);
    void	CombineIngWord(int status);

    int		DownGradeIngWordStatus(wchar_t word);

};
#endif // QWERTYKEYPAD_AUTOMATA_KOREAN_H
