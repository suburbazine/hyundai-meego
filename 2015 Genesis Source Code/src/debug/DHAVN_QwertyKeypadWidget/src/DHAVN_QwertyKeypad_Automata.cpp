#include "DHAVN_QwertyKeypad_Automata.h"
#include "DHAVN_QwertyKeypad_Utility.h"


QwertyKeypadAutomata::QwertyKeypadAutomata()
{
    m_nAutomataID = QwertyKeypadUtility::AUTOMATA_NONE;
}

QwertyKeypadAutomata::~QwertyKeypadAutomata()
{
}

int QwertyKeypadAutomata::getAutomataID()
{
    return m_nAutomataID;
}

void QwertyKeypadAutomata::setAutomataID(int nLangId)
{
    m_nAutomataID = nLangId;
    return;
}

