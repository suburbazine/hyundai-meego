#ifndef QWERTYKEYPAD_AUTOMATA_H
#define QWERTYKEYPAD_AUTOMATA_H

#include <QString>

class QwertyKeypadAutomata
{
public:
    explicit QwertyKeypadAutomata();
    virtual ~QwertyKeypadAutomata();

    int     getAutomataID();
    void    setAutomataID(int nLangId);

    virtual QString makeWord(int nKeyCode, QString keyCode)=0;
    virtual bool    isComposing()=0;
    virtual void    reset()=0;

protected:
    int     m_nAutomataID;
};

#endif // QWERTYKEYPAD_AUTOMATA_H
