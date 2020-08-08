#include "DABEPGDateListItem.h"

DABEPGDateListItem::DABEPGDateListItem(QObject *parent) :
    QObject(parent)
{
    this->m_sEPGDate = 0;
    this->m_sEnsembleName = "";
    this->m_sServiceName = "";
    this->m_pEPGHashTable.clear();
}
