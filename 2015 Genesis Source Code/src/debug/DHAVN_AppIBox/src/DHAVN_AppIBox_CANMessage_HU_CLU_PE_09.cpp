#include "DHAVN_AppIBox_CANMessage_HU_CLU_PE_09.h"

const CanMessageFieldType CANMessage_HU_CLU_PE_09::s_fields[] = {
    {0, 0, 8},
    {1, 3, 5},
    {2, 0, 11},
    {3, 4, 4},
    {3, 1, 3},
    {4, 2, 7},
    {5, 4, 6},
    {6, 6, 6},
    {7, 0, 14}
};

CANMessage_HU_CLU_PE_09::CANMessage_HU_CLU_PE_09(Logger &pLogger, QObject *parent) :
    CANMessage(pLogger, parent)
{
    setId(0x1E3);
    setLengh(8);
}

void CANMessage_HU_CLU_PE_09::setField(CANMessage_HU_CLU_PE_09::FieldId fieldId, quint64 value)
{
    setData(CANMessage_HU_CLU_PE_09::s_fields[fieldId], value);
}
