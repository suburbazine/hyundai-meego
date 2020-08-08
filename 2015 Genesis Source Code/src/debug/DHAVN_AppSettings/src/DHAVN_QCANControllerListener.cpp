#include "DHAVN_AppSetting_Log.h"
#include "DHAVN_QCANControllerListener.h"
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QDebug>
#include <QTime>
#include <QByteArray>

#define INTERVAL_HU_CLOCK_PE_02   1000

#define FULL64BITS                                  (0xffffffffffffffff)
#define FULL_BITS(bitSize)                          (FULL64BITS >> (64 - bitSize))
#define MOVE_BITS(startByte, startBit, bitSize)     (FULL_BITS(bitSize) << ((7 - startByte) * 8 + startBit))



QCANControllerListener::QCANControllerListener(SettingsAppEngine* uiAppEngine, SettingsStorage *m_SettingsStorage, QObject *)
    :m_uiAppEngine(uiAppEngine)
    ,m_pSettingsDB(m_SettingsStorage)
    ,m_pSendTimer(new QTimer(this))
    ,m_bIsReceived_CH_E_01(false)
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );
    mAppln = new CQCANController_ApplnTxRx(this);
    mAppln->Init();

    loadValueFromDB();

    connect(mAppln, SIGNAL(Filter_can_id()),this,SLOT(CQCANControllerTest_Filter_can_id()));
    connect(mAppln, SIGNAL(Can_data_received(SQCanFrameList)),this,SLOT(onCAN_data_received(SQCanFrameList)));
    connect(m_pSettingsDB,SIGNAL(timeFormateDataChanged(int)),this,SLOT(onTimeFormateChange(int)));
    connect(m_pSettingsDB,SIGNAL(SettingsModeChanged(bool)),this,SLOT(onSettingsModechange(bool)));
    connect(m_pSendTimer , SIGNAL(timeout()), this, SLOT(sendCAN_HU_CLOCK_E_00()));
    connect(m_uiAppEngine ,SIGNAL(accStatus(int)),this,SLOT(onAccStatus(int)));
    connect(m_uiAppEngine ,SIGNAL(timePickerPopupStatus(bool)),this,SLOT(onTimePickerPopupStatus(bool)));

    //for Rear Screen Lock
    connect( m_uiAppEngine, SIGNAL(sendRearScreenLockStatus(bool)), this, SLOT(sendReakScreenLockStatus(bool)));

    sendCAN_HU_CLOCK_E_00();

    m_pSendTimer->setInterval(INTERVAL_HU_CLOCK_PE_02);
    m_pSendTimer->setSingleShot(false);
    m_pSendTimer->start();
}

QCANControllerListener::~QCANControllerListener()
{
    /*
    if (m_pNaviUtils)
    {
        delete m_pNaviUtils;
        m_pNaviUtils = NULL;
    }
    */

    //    if (m_pSendTimer)
    //    {
    //        m_pSendTimer->stop();
    //        delete m_pSendTimer;
    //        m_pSendTimer = NULL;
    //    }
}


void QCANControllerListener::sendCAN_HU_CLOCK_E_00()
{
    SQCanFrame tCanFrame;
    const CanMessageFieldType *pMsgField = getCANMessageField( SettingsCANDef::S_CID_HU_CLOCK_E_00 );
    const CanMessageFieldType *pCurField;

    memset( &tCanFrame, 0, sizeof( SQCanFrame ) );
    tCanFrame.uqcanid = SettingsCANDef::S_CID_HU_CLOCK_E_00;
    tCanFrame.uqcandlc = 0x8;

    //! HU_TimeFormat
    pCurField = &pMsgField[SettingsCANDef::S_HC_E_00_SIG_HU_TimeFormat];
    setData( &tCanFrame, pCurField->startByte, pCurField->startBit,
             pCurField->bitSize, m_eHUTimeFormat );

    //! HU_TimeSetting
    pCurField = &pMsgField[SettingsCANDef::S_HC_E_00_SIG_HU_TimeSetting];
    setData( &tCanFrame, pCurField->startByte, pCurField->startBit,
             pCurField->bitSize, m_eHUTimeSetting );

    //! HU_ETDReq
    pCurField = &pMsgField[SettingsCANDef::S_HC_E_00_SIG_HU_ETDReq];
    if ( m_bIsReceived_CH_E_01 == true )
    {
        setData( &tCanFrame, pCurField->startByte, pCurField->startBit,
                 pCurField->bitSize, SettingsCANDef::S_HC_E_00_VAL_HETDR_Off );
    }
    else
    {
        setData( &tCanFrame, pCurField->startByte, pCurField->startBit,
                 pCurField->bitSize, SettingsCANDef::S_HC_E_00_VAL_HETDR_Request );
    }

    //! HU_Status
    pCurField = &pMsgField[SettingsCANDef::S_HC_E_00_SIG_HU_Status];
    if ( m_eHUStatus != SettingsCANDef::S_HC_E_00_VAL_HS_NoGPS &&
         m_eHUStatus != SettingsCANDef::S_HC_E_00_VAL_HS_SettingMode )
    {
        //! Check whether GPS is available or not
        if ( m_uiAppEngine->isGPSAvailable() == true )
        {
            m_eHUStatus = SettingsCANDef::S_HC_E_00_VAL_HS_GPSReceiving;
        }
        else
        {
            m_eHUStatus = SettingsCANDef::S_HC_E_00_VAL_HS_GPSUnavailableMode;
        }
    }

    //LOG2 ( QString("m_eHUStatus:%1, m_eHUTimeSetting:%2").arg(m_eHUStatus).arg(m_eHUTimeSetting), Logger::PRIORITY_HI );

    setData( &tCanFrame, pCurField->startByte, pCurField->startBit,
             pCurField->bitSize, m_eHUStatus );

    debugCANFrame(&tCanFrame);

    emit Can_data_send(tCanFrame);
}

void QCANControllerListener::sendCAN_HU_CLOCK_P_01(uint baseTime)
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );
    //qDebug() << ">>>>> QCANControllerListener::sendCAN_HU_CLOCK_P_01";

    SQCanFrame tCanFrame;
    const CanMessageFieldType *pMsgField = getCANMessageField( SettingsCANDef::S_CID_HU_CLOCK_P_01 );
    const CanMessageFieldType *pCurField;

    memset( &tCanFrame, 0, sizeof( SQCanFrame ) );
    tCanFrame.uqcanid = SettingsCANDef::S_CID_HU_CLOCK_P_01;
    tCanFrame.uqcandlc = 0x8;

    //! HU_OffsetTime
    pCurField = &pMsgField[SettingsCANDef::S_HC_P_01_SIG_HU_OffsetTime];
    setData( &tCanFrame, pCurField->startByte, pCurField->startBit,
             pCurField->bitSize, 0 );

    //! HU_BaseTime
    bool ok;
    QByteArray gpsdata=QByteArray::number(baseTime, 2);
    QByteArray secondByteData = gpsdata.right(1);
    secondByteData.append("0000000");
    gpsdata.chop(1);
    QByteArray firstByteData =gpsdata.right(8);
    gpsdata.chop(8);
    QByteArray zerothByteData =gpsdata;

    int j=zerothByteData.size();

    if(j<8)
    {
        for(int i=0;i<8-j;i++)
        {
            zerothByteData.insert(i,"0");
        }
    }

    uint SecByteHex =secondByteData.toUInt(&ok,2);
    uint FirstByteHex =firstByteData.toUInt(&ok,2);
    uint ZeroByteHex = zerothByteData.toUInt(&ok,2);
    uint SecByte = DecimalToHexa(SecByteHex);
    uint FirByte = DecimalToHexa(FirstByteHex);
    uint ZeroByte =DecimalToHexa(ZeroByteHex);

    tCanFrame.uqcandata[0]=ZeroByte;
    tCanFrame.uqcandata[1]=FirByte;
    tCanFrame.uqcandata[2]=SecByte;

    //debugCANFrame( &tCanFrame );

    emit Can_data_send(tCanFrame);

    //qDebug() << "<<<<< QCANControllerListener::sendCAN_HU_CLOCK_P_01";
}

void QCANControllerListener::CQCANControllerTest_Filter_can_id()
{
    canIdList << SettingsCANDef::S_CID_CLOCK_HU_E_01;
    struct SQCanIdList canIdListstruct;
    canIdListstruct.uqcount = canIdList.count();
    canIdListstruct.qcanidlist << canIdList;
    emit Can_filter_msg_id(canIdListstruct);
}

void QCANControllerListener::onCAN_data_received(const SQCanFrameList &receivedData)
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );
    const CanMessageFieldType *pCurField;
    quint64 uiValue;

    if(receivedData.uqcount)
    {
        if( receivedData.qcanframelist[0].uqcanid == SettingsCANDef::S_CID_CLOCK_HU_E_01)
        {
            //qDebug() << ">>>>> QCANControllerListener::onCAN_data_received";

            debugCANFrame( &receivedData.qcanframelist[0] );
            m_bIsReceived_CH_E_01 = true;

            const CanMessageFieldType *pMsgField = getCANMessageField(
                        SettingsCANDef::S_CID_CLOCK_HU_E_01 );
            pCurField = &pMsgField[SettingsCANDef::S_CH_E_01_SIG_AC_HU_SEStatus];

            uiValue = getData( &receivedData.qcanframelist[0], pCurField->startByte,
                    pCurField->startBit, pCurField->bitSize );
            //qDebug() << "SE Status: " << uiValue;

            if ( uiValue == SettingsCANDef::S_CH_E_01_VAL_HSES_Off ||
                 uiValue == SettingsCANDef::S_CH_E_01_VAL_HSES_NoEvent ||
                 uiValue == SettingsCANDef::S_CH_E_01_VAL_HSES_Invalid )
            {
                sendCAN_HU_CLOCK_E_00();
            }



            //! Get AC_HU_Mstatus
            pCurField = &pMsgField[SettingsCANDef::S_CH_E_01_SIG_AC_HU_Mstatus];
            uiValue = getData( &receivedData.qcanframelist[0], pCurField->startByte,
                    pCurField->startBit, pCurField->bitSize );

            //qDebug() <<"[MOTOR_STATUS] getData SettingsCANDef::S_CH_E_01_SIG_AC_HU_Mstatus  uiValue = " << uiValue;

            if ( uiValue == SettingsCANDef::S_CH_E_01_VAL_HM_MotorError )
            {
                //qDebug() <<"[MOTOR_STATUS]   uiValue == S_CH_E_01_VAL_HM_MotorError";

                m_uiAppEngine->showMotorErrorPopup();
            }

            //qDebug() << "<<<<< QCANControllerListener::onCAN_data_received";

        }
    }
}

//void QCANControllerListener::updateGpsTime()
//{
//   uint uiSecond;

//   uiSecond = getSecondFromSystemTime();

//   qDebug() << "QCANControllerListener::updateGpsTime - Sec: " << uiSecond;

//   sendCAN_HU_CLOCK_P_01( uiSecond );
//}

void QCANControllerListener::onTimeFormateChange(int timeFormat)
{
    LOG2 ( QString("timeFormatCh: %1").arg(timeFormat), Logger::PRIORITY_HI );
    switch( timeFormat )
    {
    case AppSettingsDef::TIME_TYPE_12H:
        m_eHUTimeFormat = SettingsCANDef::S_HC_E_00_VAL_HUTF_12_Hour;
        break;

    case AppSettingsDef::TIME_TYPE_24H:
        m_eHUTimeFormat = SettingsCANDef::S_HC_E_00_VAL_HUTF_24_Hour;
        break;

    default:
        m_eHUTimeFormat = SettingsCANDef::S_HC_E_00_VAL_HUTF_Off;
        break;
    }

    sendCAN_HU_CLOCK_E_00();
}

void QCANControllerListener::onSettingsModechange(bool settingsMode)
{
    if ( settingsMode == true )
    {
        m_eHUTimeSetting = SettingsCANDef::S_HC_E_00_VAL_HTS_GPSTime;
    }
    else
    {
        m_eHUTimeSetting = SettingsCANDef::S_HC_E_00_VAL_HTS_UserTime;
    }

    sendCAN_HU_CLOCK_E_00();
}

void QCANControllerListener::onAccStatus(int value)
{
    //! Send HU_CLOCK_P_01 for every 2 secs although ACC is off
    /*
   if(m_bIsReceived_CH_E_01)
   {
      switch(value)
      {
      case 0:
         if(m_pSendTimer->isActive())
            m_pSendTimer->stop();
         break;
      case 1:
         m_pSendTimer->start(INTERVAL_HU_CLOCK_P_01);
         break;
      }
   }
   */
}

void QCANControllerListener::onTimePickerPopupStatus( bool isShow )
{
    //LOG2 ( QString(">>>>>"), Logger::PRIORITY_HI );
    if ( isShow == true )
    {
        m_eHUStatus = SettingsCANDef::S_HC_E_00_VAL_HS_SettingMode;
    }
    else
    {
        //! If it is not Setting mode, return
        if ( m_eHUStatus != SettingsCANDef::S_HC_E_00_VAL_HS_SettingMode )
        {
            return;
        }

        uint uiSystemVariant = m_uiAppEngine->GetSystemVariant();
        if ( uiSystemVariant & 0x1 )
        {
            m_eHUStatus = SettingsCANDef::S_HC_E_00_VAL_HS_GPSUnavailableMode;
        }
        else
        {
            m_eHUStatus = SettingsCANDef::S_HC_E_00_VAL_HS_NoGPS;
        }
    }
    //LOG2 ( QString("IsShow: %1, HU Status: %2").arg(isShow).arg(m_eHUStatus), Logger::PRIORITY_HI );

    sendCAN_HU_CLOCK_E_00();

    //LOG2 ( QString("<<<<<"), Logger::PRIORITY_HI );
}

const CanMessageFieldType * QCANControllerListener::getCANMessageField(SettingsCANDef::S_CAN_ID_T CAN_id)
{
    switch( CAN_id )
    {
    case SettingsCANDef::S_CID_HU_CLOCK_E_00:
        return CMFT_HU_CLOCK_E_00;

    case SettingsCANDef::S_CID_CLOCK_HU_E_01:
        return CMFT_CLOCK_HU_E_01;

    case SettingsCANDef::S_CID_HU_CLOCK_P_01:
        return CMFT_HU_CLOCK_P_01;

    default:
        return NULL;
    }

    return NULL;
}

void QCANControllerListener::loadValueFromDB()
{
    //LOG2 ( QString(""), Logger::PRIORITY_HI );
    int iTimeType;
    uint uiSystemVariant;

    bool gps_time = m_pSettingsDB->gpsTime();
    LOG2 ( QString("lVFDB gps_time = %1").arg(gps_time), Logger::PRIORITY_HI );
    //added for Not send GPS Timset when SD removed and reboot
    //bool navi_mount = m_pSettingsDB->naviSDCard();
    //LOG2 ( QString("navi_mount = %1").arg(navi_mount), Logger::PRIORITY_HI );
    //int gps_time_from_db = m_pSettingsDB->LoadSetting(SettingsDef::DB_KEY_GPS_TIME_SETTINGS);
    //LOG2 ( QString("gps_time_from_db = %1").arg(gps_time_from_db), Logger::PRIORITY_HI );

    //! Time Setting
    if (gps_time) //added for Not send GPS Timset when SD removed and reboot
    {
        m_eHUTimeSetting = SettingsCANDef::S_HC_E_00_VAL_HTS_GPSTime;
    }
    else
    {
        m_eHUTimeSetting = SettingsCANDef::S_HC_E_00_VAL_HTS_UserTime;
    }

    //! Time Format
    iTimeType = m_pSettingsDB->LoadSetting(SettingsDef::DB_KEY_TIME_TYPE);
    switch( iTimeType )
    {
    case AppSettingsDef::TIME_TYPE_12H:
        m_eHUTimeFormat = SettingsCANDef::S_HC_E_00_VAL_HUTF_12_Hour;
        break;

    case AppSettingsDef::TIME_TYPE_24H:
        m_eHUTimeFormat = SettingsCANDef::S_HC_E_00_VAL_HUTF_24_Hour;
        break;

    default:
        m_eHUTimeFormat = SettingsCANDef::S_HC_E_00_VAL_HUTF_Off;
        break;
    }

    //! Navi
    uiSystemVariant = m_uiAppEngine->GetSystemVariant();
    if ( uiSystemVariant & 0x1 )
    {
        m_eHUStatus = SettingsCANDef::S_HC_E_00_VAL_HS_GPSUnavailableMode;
    }
    else
    {
        m_eHUStatus = SettingsCANDef::S_HC_E_00_VAL_HS_NoGPS;
    }
}

void QCANControllerListener::setData(SQCanFrame *result, uint startByte, uint startBit, uint bitSize, quint64 data)
{
    /*
   QString dbgStr;
   qDebug() << "QCANControllerListener::setData - startByte=[" << startByte
            << "], startBit=[" << startBit << "], bitSize=[" << bitSize <<
               "], data=[" << dbgStr.setNum(data, 16) << "]";
               */

    quint64 existingData = qFromBigEndian<quint64>((const uchar *)result->uqcandata);

    //qDebug() << "(origin) existingData=[" << dbgStr.setNum(existingData, 16) << "]";

    // Clean up
    quint64 moveBits = MOVE_BITS(startByte, startBit, bitSize);
    quint64 xoredBits = moveBits ^ FULL64BITS;
    existingData &= xoredBits;

    // Fill data
    existingData |= (FULL_BITS(bitSize) & data) << ((7 - startByte) * 8 + startBit);

    //qDebug() << "(update) existingData=[" << dbgStr.setNum(existingData, 16) << "]";

    // To BigEndian
    existingData = qToBigEndian<quint64>(existingData);

    // Copy data
    memcpy(result->uqcandata, &existingData, sizeof(quint64));
}

quint64 QCANControllerListener::getData(const SQCanFrame *data, uint startByte, uint startBit, uint bitSize)
{
    //QString dbgStr;
    int i, iMoveBitCnt;
    quint64 existingData, moveBits, result;

    existingData = 0;
    for (i=0; i < 8; i++)
    {
        existingData |= (((quint64) data->uqcandata[i]) << (i*8));
    }

    //qDebug() << "QCANControllerListener::getData - startByte=[" << startByte
    //         << "], startBit=[" << startBit << "], bitSize=[" << bitSize <<
    //            "], data=[" << dbgStr.setNum(existingData, 16) << "]";

    iMoveBitCnt = startByte * 8 + startBit;
    moveBits = FULL_BITS(bitSize) << iMoveBitCnt;
    existingData &= moveBits;

    result = existingData >> iMoveBitCnt;
    //qDebug() << "Result=[" << result << "]";

    return result;
}
uint QCANControllerListener::DecimalToHexa(const uint value)
{
    bool ok;
    QString numInDec;
    numInDec.append("0x");
    QString numInDec1=numInDec.setNum(value,16);
    uint hexValue = numInDec1.toUInt(&ok,16);
    return hexValue;
}

void QCANControllerListener::debugCANFrame(const SQCanFrame *data)
{
    QString dbgStr;
    //qDebug() << ">>>>> QCANControllerListener::debugCANFrame";
    //qDebug() << "CAN ID: " << data->uqcanid << " ,Data Length: " << data->uqcandlc;

    for (int i = 0; i < 8; i++ )
    {
        dbgStr.setNum( data->uqcandata[i], 16 );
        //qDebug() << "Idx: " << i << " , Data: " << dbgStr;
    }
    //qDebug() << "<<<<< QCANControllerListener::debugCANFrame";
}

uint QCANControllerListener::getSecond( int hour, int min, int sec )
{
    uint result;

    result = ( hour * 3600 ) +
            ( min * 60 ) +
            ( sec );

    return result;
}

uint QCANControllerListener::getSecondFromSystemTime()
{
    uint result = 0;

    QTime time = QTime::currentTime();
    result = getSecond( time.hour(), time.minute(), time.second() );

    return result;
}


//for Rear Screen Lock
void QCANControllerListener::sendReakScreenLockStatus(bool status)
{
    LOG2 ( QString(""), Logger::PRIORITY_HI );
    struct SQCanFrame canFrame;

    if( status )
    {
        canFrame.uqcanid = 0x1C0;
        canFrame.uqcandlc = 0x01;
        canFrame.uqcandata[0] = 0x40;
    }
    else
    {
        canFrame.uqcanid = 0x1C0;
        canFrame.uqcandlc = 0x01;
        canFrame.uqcandata[0] = 0x00;
    }

    emit Can_data_send(canFrame);
}
