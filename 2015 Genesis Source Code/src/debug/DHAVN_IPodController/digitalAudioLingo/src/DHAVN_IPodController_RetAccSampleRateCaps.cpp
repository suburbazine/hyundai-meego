
/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                                                                *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodController_RetAccSampleRateCaps.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace DigitalAudioLingo;

CRetAccSampleRateCaps::CRetAccSampleRateCaps(int transIdArgs,  bool isTidEnabled) :  m_TransId(transIdArgs)
{
    LOG_INIT_V2(gControllerLogName);
    m_isTidEnabled = isTidEnabled;
    //LOG_TRACE << "in CRetAccSampleRateCaps constructor" << LOG_ENDL;
}

QByteArray CRetAccSampleRateCaps::marshall()
{
    QByteArray command;
    int i = -1;

    //Packing format =  Big Endianll
    //command.resize(19);
    command[++i] = 0x55;
    command[++i] = 0x10;
    command[++i] = 0x0A;
    command[++i] = 0x03;
    if( m_isTidEnabled){
        command[++i] = ((m_TransId & 0xff00) >> 8);
        command[++i] = m_TransId & 0x00ff;
    }

#if 0
    /* 8000 Hz */
    command[++i] = 0x00;
    command[++i] = 0x00;
    command[++i] = 0x1F;
    command[++i] = 0x40;

    /* 11025 Hz */
    command[++i] = 0x00;
    command[++i] = 0x00;
    command[++i] = 0x2B;
    command[++i] = 0x11;

    /* 12000 Hz */
    command[++i] = 0x00;
    command[++i] = 0x00;
    command[++i] = 0x2E;
    command[++i] = 0xE0;

    /* 16000 Hz */
    command[++i] = 0x00;
    command[++i] = 0x00;
    command[++i] = 0x3E;
    command[++i] = 0x80;

    /* 22050 Hz */
    command[++i] = 0x00;
    command[++i] = 0x00;
    command[++i] = 0x56;
    command[++i] = 0x22;

    /* 24000 Hz */
    command[++i] = 0x00;
    command[++i] = 0x00;
    command[++i] = 0x5D;
    command[++i] = 0xC0;
#endif

    /* 32000 Hz */
    command[++i] = 0x00;
    command[++i] = 0x00;
    command[++i] = 0x7D;
    command[++i] = 0x00;

    /* 44100 Hz */
    command[++i] = 0x00;
    command[++i] = 0x00;
    command[++i] = 0xAC;
    command[++i] = 0x44;

    /* 48000 Hz */
    command[++i] = 0x00;
    command[++i] = 0x00;
    command[++i] = 0xBB;
    command[++i] = 0x80;
    //TODO calculate a valid checksum and insert
    command[1] = i -1;
    command[i+1] = CIpodControllerGlobal::checkSum(command, i);

    m_CommandBytes = command;
    return command;
}

void  CRetAccSampleRateCaps::unmarshall(QByteArray /*returnCommand*/)
{

}


