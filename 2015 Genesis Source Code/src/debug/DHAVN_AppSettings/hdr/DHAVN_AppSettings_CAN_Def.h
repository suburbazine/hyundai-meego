#ifndef DHAVN_APPSETTINGS_CAN_DEF_H
#define DHAVN_APPSETTINGS_CAN_DEF_H

#include <QObject>

struct CanMessageFieldType {
    uint startByte;
    uint startBit;
    uint bitSize;
};

const CanMessageFieldType CMFT_HU_CLOCK_E_00[] = {
   { 0, 0, 2 },
   { 0, 2, 4 },
   { 0, 6, 2 },
   { 7, 4, 2 },
};

const CanMessageFieldType CMFT_HU_CLOCK_P_01[] = {
   { 2, 7, 17 },
   { 3, 4, 11 },
};

const CanMessageFieldType CMFT_CLOCK_HU_E_01[] = {
   { 0, 6, 2 },
   { 6, 4, 2 },
};

class SettingsCANDef : public QObject
{
public:
   //! CAN ID
   enum S_CAN_ID_T
   {
      S_CID_HU_CLOCK_E_00 = 0x12F,
      S_CID_CLOCK_HU_E_01 = 0xC7,
      S_CID_HU_CLOCK_P_01 = 0x509,
   };

   //! HU_CLOCK_E_00 Signal
   enum S_CAN_HU_CLOCK_E_00_SIG_T
   {
      S_HC_E_00_SIG_HU_TimeFormat = 0,
      S_HC_E_00_SIG_HU_Status,
      S_HC_E_00_SIG_HU_TimeSetting,
      S_HC_E_00_SIG_HU_ETDReq,
   };

   //! 'HU_CLOCK_E_00 -> HU_TimeFormat' Value
   enum S_CAN_HC_E_00_HU_TimeFormat_VAL_T
   {
      S_HC_E_00_VAL_HUTF_Off = 0x0,
      S_HC_E_00_VAL_HUTF_12_Hour = 0x1,
      S_HC_E_00_VAL_HUTF_24_Hour = 0x2,
      S_HC_E_00_VAL_HUTF_Invalid = 0x3,
   };

   //! 'HU_CLOCK_E_00 -> HU_Status' Value
   enum S_CAN_HC_E_00_HU_Status_VAL_T
   {
      S_HC_E_00_VAL_HS_NotUsed = 0x0,
      S_HC_E_00_VAL_HS_BootingHeadUnit = 0x1,
      S_HC_E_00_VAL_HS_GPSReceiving = 0x2,
      S_HC_E_00_VAL_HS_SettingMode = 0x3,
      S_HC_E_00_VAL_HS_GPSUnavailableMode = 0x4,
      S_HC_E_00_VAL_HS_NoGPS = 0x5,
      S_HC_E_00_VAL_HS_Invalid = 0xF,
   };

   //! 'HU_CLOCK_E_00 -> HU_TimeSetting' Value
   enum S_CAN_HC_E_00_HU_TimeSetting_VAL_T
   {
      S_HC_E_00_VAL_HTS_Off = 0x0,
      S_HC_E_00_VAL_HTS_UserTime = 0x1,
      S_HC_E_00_VAL_HTS_GPSTime = 0x2,
      S_HC_E_00_VAL_HTS_Invalid = 0x3,
   };

   //! 'HU_CLOCK_E_00 -> HU_ETDReq' Value
   enum S_CAN_HC_E_00_HU_ETDReq_VAL_T
   {
      S_HC_E_00_VAL_HETDR_Off = 0x0,
      S_HC_E_00_VAL_HETDR_Request = 0x1,
      S_HC_E_00_VAL_HETDR_Reserved = 0x2,
      S_HC_E_00_VAL_HETDR_Invalid = 0x3,
   };

   //! HU_CLOCK_P_01 Signal
   enum S_CAN_HU_CLOCK_P_01_SIG_T
   {
      S_HC_P_01_SIG_HU_BaseTime = 0,
      S_HC_P_01_SIG_HU_OffsetTime,
   };

   //! CLOCK_HU_E_01 Signal
   enum S_CAN_CLOCK_HU_E_01_SIG_T
   {
      S_CH_E_01_SIG_AC_HU_Mstatus = 0,
      S_CH_E_01_SIG_AC_HU_SEStatus,
   };

   //! 'CLOCK_HU_E_01 -> AC_HU_Mstatus' Value
   enum S_CAN_CH_E_01_AC_HU_Mstatus_VAL_T
   {
      S_CH_E_01_VAL_HM_Off = 0x0,
      S_CH_E_01_VAL_HM_MotorOK = 0x1,
      S_CH_E_01_VAL_HM_MotorError = 0x2,
      S_CH_E_01_VAL_HM_Invalid = 0x3,
   };

   //! 'CLOCK_HU_E_01 -> AC_HU_SEStatus' Value
   enum S_CAN_CH_E_01_AC_HU_SEStatus_VAL_T
   {
      S_CH_E_01_VAL_HSES_Off = 0x0,
      S_CH_E_01_VAL_HSES_NoEvent = 0x1,
      S_CH_E_01_VAL_HSES_EventExist = 0x2,
      S_CH_E_01_VAL_HSES_Invalid = 0x3,
   };
};

#endif // DHAVN_APPSETTINGS_CAN_DEF_H
