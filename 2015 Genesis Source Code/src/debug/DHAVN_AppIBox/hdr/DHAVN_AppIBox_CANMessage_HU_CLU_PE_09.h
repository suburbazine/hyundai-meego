#ifndef DHAVN_APPIBOX_CANMESSAGE_HU_CLU_PE_09_H
#define DHAVN_APPIBOX_CANMESSAGE_HU_CLU_PE_09_H

#include "DHAVN_AppFramework_Logger_Def.h"
#include "DHAVN_AppIBox_CANMessage.h"

class CANMessage_HU_CLU_PE_09 : public CANMessage
{
    Q_OBJECT
public:
    explicit CANMessage_HU_CLU_PE_09(Logger &pLogger, QObject *parent = 0);

    enum FieldId {
        FIELD_OP_STS = 0,
        FIELD_PRESET_NO,
        FIELD_FREQ_CH_NO,
        FIELD_HD_NO,
        FIELD_DISC_NO,
        FIELD_PLAY_TIME_HOUR,
        FIELD_PLAY_TIME_MIN,
        FIELD_PLAY_TIME_SEC,
        FIELD_TRACK_NO
    } ;

    enum FieldValue_OP_STS {
        OP_STS_INITIAL = 0,

        OP_STS_DLNA = 0xB2,

        OP_STS_INVALID = 0xff
    };

    void setField(FieldId fieldId, quint64 value);

signals:

public slots:

private:

    static const struct CanMessageFieldType s_fields[];
};

#endif // DHAVN_APPIBOX_CANMESSAGE_HU_CLU_PE_09_H
