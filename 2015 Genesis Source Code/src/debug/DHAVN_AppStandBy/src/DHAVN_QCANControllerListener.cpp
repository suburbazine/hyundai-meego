
#include "DHAVN_QCANControllerListener.h"
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QDebug>
#include <QTime>
#include <QByteArray>

#define FULL64BITS                                  (0xffffffffffffffff)
#define FULL_BITS(bitSize)                          (FULL64BITS >> (64 - bitSize))
#define MOVE_BITS(startByte, startBit, bitSize)     (FULL_BITS(bitSize) << ((7 - startByte) * 8 + startBit))

QCANControllerListener::QCANControllerListener(StandByAppEngine *uiAppEngine, QObject *)
//    :QObject(parent)
    : m_uiAppEngine(uiAppEngine) //m_uiAppEngine(uiAppEngine)
    ,m_bIsReceived_CH_E_01(false)
{
    m_nCnt = 0;
    mAppln = new CQCANController_ApplnTxRx(this);
    mAppln->Init();
    updateTime();
//    connect(m_pSendTimer , SIGNAL(timeout()), this, SLOT(updateTime()));      // appEngien timer 사용
    connect(m_uiAppEngine, SIGNAL(sendTimeoutToExternalClock()), this, SLOT(updateTime()));
}

QCANControllerListener::~QCANControllerListener()
{
}

void QCANControllerListener::sendCAN_HU_CLOCK_P_01(uint baseTime)
{
    SQCanFrame tCanFrame;
//   struct SQCanFrame tCanFrame = {0x11F, 0x8, { 0x0E, 0x10, 0x00, 0x0F, 0x00, 0xF3, 0x02, 0x2F}};
//   struct SQCanFrame tCanFrame = {0x32, 0x8, { 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10}};
//   emit Can_data_send(tCanFrame);
//   return;
   const CanMessageFieldType *pMsgField = getCANMessageField( ClockCANDef::S_CID_HU_CLOCK_P_01 );
   const CanMessageFieldType *pCurField;

   memset( &tCanFrame, 0, sizeof( SQCanFrame ) );
   tCanFrame.uqcanid = ClockCANDef::S_CID_HU_CLOCK_P_01;
   tCanFrame.uqcandlc = 0x8;

   //! HU_OffsetTime
   pCurField = &pMsgField[ClockCANDef::S_HC_P_01_SIG_HU_OffsetTime];
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

   emit Can_data_send(tCanFrame);
}

void QCANControllerListener::CQCANControllerTest_Filter_can_id()
{
   canIdList << ClockCANDef::S_CID_CLOCK_HU_E_01;
   struct SQCanIdList canIdListstruct;
   canIdListstruct.uqcount = canIdList.count();
   canIdListstruct.qcanidlist << canIdList;
   emit Can_filter_msg_id(canIdListstruct);
}

/*
void QCANControllerListener::onCAN_data_received(const SQCanFrameList &receivedData)
{
    HS_LOG( "onCAN_data_received " );
   const CanMessageFieldType *pCurField;
   quint64 uiValue;

   if(receivedData.uqcount)
   {
      if( receivedData.qcanframelist[0].uqcanid == ClockCANDef::S_CID_CLOCK_HU_E_01)
      {
         qDebug() << ">>>>> QCANControllerListener::onCAN_data_received";

         debugCANFrame( &receivedData.qcanframelist[0] );
         m_bIsReceived_CH_E_01 = true;

         const CanMessageFieldType *pMsgField = getCANMessageField(
                  ClockCANDef::S_CID_CLOCK_HU_E_01 );
         pCurField = &pMsgField[ClockCANDef::S_CH_E_01_SIG_AC_HU_SEStatus];

         uiValue = getData( &receivedData.qcanframelist[0], pCurField->startByte,
                           pCurField->startBit, pCurField->bitSize );
         qDebug() << "SE Status: " << uiValue;

         if ( uiValue == ClockCANDef::S_CH_E_01_VAL_HSES_Off ||
               uiValue == ClockCANDef::S_CH_E_01_VAL_HSES_NoEvent ||
               uiValue == ClockCANDef::S_CH_E_01_VAL_HSES_Invalid )
         {
            sendCAN_HU_CLOCK_E_00();
         }

         qDebug() << "<<<<< QCANControllerListener::onCAN_data_received";
      }
   }
}
*/
void QCANControllerListener::updateTime()
{
    QDate curDate = curDate.currentDate();
    if (curDate.year() < 2012 ) return;    // 2012년도 이전 값일 경우 시간 전송 안함
    uint uiSecond;
    uiSecond = getSecondFromSystemTime();
//      uiSecond = getSecond( time.hour(), time.minute(), time.second() );
      sendCAN_HU_CLOCK_P_01( uiSecond );
   /*
   if ( m_eHUTimeSetting == ClockCANDef::S_HC_E_00_VAL_HTS_GPSTime )
   {
      if ( m_bIsReceived_GPSTime == false )
      {
         m_navidata = m_pNaviUtils->getGpsData();

         if ( m_navidata != NULL && m_navidata->bValid == true )
         {
            LW_LOG( "GPS data is valid. Get GPS Time" );
            m_bIsReceived_GPSTime = true;

            if ( m_pGPSTime == NULL )
            {
               m_pGPSTime = new QTime();
            }
            m_pGPSTime->setHMS( m_navidata->NaviGPSTime.nHour,
                               m_navidata->NaviGPSTime.nMin,
                               m_navidata->NaviGPSTime.nSec );
            m_pGPSTime->start();

            uiSecond = getSecond( m_pGPSTime->hour(), m_pGPSTime->minute(), m_pGPSTime->second() );
         }
         else
         {
             LW_LOG( "GPS data is invalid. Get GPS Time" );
            uiSecond = getSecondFromSystemTime();
         }
      }
      else
      {
         if ( m_pGPSTime )
         {
            uiSecond = getSecond( m_pGPSTime->hour(), m_pGPSTime->minute(), m_pGPSTime->second() );
         }
         else
         {
            m_bIsReceived_GPSTime = false;
            uiSecond = getSecondFromSystemTime();
         }
      }
   }
   else
   {
      m_bIsReceived_GPSTime = false;
      uiSecond = getSecondFromSystemTime();
   }

   */
}


void QCANControllerListener::onSettingsModechange(bool settingsMode)
{
    /*
   if ( settingsMode )
   {
      m_eHUTimeSetting = ClockCANDef::S_HC_E_00_VAL_HTS_GPSTime;
   }
   else
   {
      m_eHUTimeSetting = ClockCANDef::S_HC_E_00_VAL_HTS_UserTime;
   }
   */
    updateTime();
}

void QCANControllerListener::onAccStatus(int value)
{
   //! Send HU_CLOCK_P_01 for every 2 secs although ACC is off
   /*
    HS_LOG( "onAccStatus " );
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


const CanMessageFieldType * QCANControllerListener::getCANMessageField(ClockCANDef::S_CAN_ID_T CAN_id)
{
   switch( CAN_id )
   {
   case ClockCANDef::S_CID_HU_CLOCK_E_00:
      return CMFT_HU_CLOCK_E_00;

   case ClockCANDef::S_CID_CLOCK_HU_E_01:
      return CMFT_CLOCK_HU_E_01;

   case ClockCANDef::S_CID_HU_CLOCK_P_01:
      return CMFT_HU_CLOCK_P_01;

   default:
      return NULL;
   }

   return NULL;
}

void QCANControllerListener::setData(SQCanFrame *result, uint startByte, uint startBit, uint bitSize, quint64 data)
{
   quint64 existingData = qFromBigEndian<quint64>((const uchar *)result->uqcandata);

   // Clean up
   quint64 moveBits = MOVE_BITS(startByte, startBit, bitSize);
   quint64 xoredBits = moveBits ^ FULL64BITS;
   existingData &= xoredBits;

   // Fill data
   existingData |= (FULL_BITS(bitSize) & data) << ((7 - startByte) * 8 + startBit);

   // To BigEndian
   existingData = qToBigEndian<quint64>(existingData);

   // Copy data
   memcpy(result->uqcandata, &existingData, sizeof(quint64));
}

quint64 QCANControllerListener::getData(const SQCanFrame *data, uint startByte, uint startBit, uint bitSize)
{
   QString dbgStr;
   int i, iMoveBitCnt;
   quint64 existingData, moveBits, result;

   existingData = 0;
   for (i=0; i < 8; i++)
   {
      existingData |= (((quint64) data->uqcandata[i]) << (i*8));
   }

   iMoveBitCnt = startByte * 8 + startBit;
   moveBits = FULL_BITS(bitSize) << iMoveBitCnt;
   existingData &= moveBits;

   result = existingData >> iMoveBitCnt;

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
   for (int i = 0; i < 8; i++ )
   {
      dbgStr.setNum( data->uqcandata[i], 16 );
   }
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

