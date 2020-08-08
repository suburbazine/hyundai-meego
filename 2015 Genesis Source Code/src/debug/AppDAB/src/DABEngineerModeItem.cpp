#include "DABEngineerModeItem.h"

DABEngineerModeItem::DABEngineerModeItem(QObject *parent) :
    ListItem(parent),
    m_dataName(""),
    m_dataValue("")
{
    m_dataFreq = 0x00;
    m_dataSubChId = 0x00;
}

DABEngineerModeItem::DABEngineerModeItem(QString dataName, QString dataValue, QObject *parent) :
    ListItem(parent),
    m_dataName(dataName),
    m_dataValue(dataValue)
{
    m_dataFreq = 0x00;
    m_dataSubChId = 0x00;
}

QHash<int, QByteArray>
DABEngineerModeItem::roleNames() const
{
    QHash<int, QByteArray> names;

    names[DataName] = "dataName";
    names[DataValue] = "dataValue";

    return names;
}

QVariant
DABEngineerModeItem::data(int role) const
{
    switch(role) {
    case DataName:
        return dataName();
    case DataValue:
        return dataValue();
    default:
        return QVariant();
    }
}
