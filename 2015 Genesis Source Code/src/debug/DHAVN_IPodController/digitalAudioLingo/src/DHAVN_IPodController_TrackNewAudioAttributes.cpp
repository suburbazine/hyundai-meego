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
#include <DHAVN_IPodController_TrackNewAudioAttributes.h>
#include <DHAVN_IPodController_CommonVars.h>

using namespace DigitalAudioLingo;

CTrackNewAudioAttributes::CTrackNewAudioAttributes(int transIdArgs,  bool isTidEnabled) :  m_TransId(transIdArgs), m_SampleRate(0), m_SoundCheckVal(0), m_VolumeAdjustment(0)
{
    LOG_INIT_V2(gControllerLogName);
    m_isTidEnabled = isTidEnabled;
    //LOG_TRACE << "in CTrackNewAudioAttributes constructor" << LOG_ENDL;
}



QByteArray CTrackNewAudioAttributes::marshall()
{
    return NULL;
}

void  CTrackNewAudioAttributes::unmarshall(QByteArray returnCommand)
{
    if( ((int)returnCommand[0] == 0x55) &&
        ((int)returnCommand[2] == 0x0A) &&
        ((int)returnCommand[3] == 0x04) )
    {
        int i = 4;
        if(m_isTidEnabled){
            m_TransactionID = 0;
            m_TransactionID |= (int)returnCommand[i++] & 0xFF;
            m_TransactionID <<= 8;
            m_TransactionID |= (int)returnCommand[i++] & 0xFF;
        }
        m_SampleRate = 0 | (0x000000FF & returnCommand[i])<<24 | ( 0x000000FF & returnCommand[i+1])<<16 | ( 0x000000FF & returnCommand[i+2])<<8 | ( 0x000000FF & returnCommand[i+3]);
        //LOG_INFO << " ++++++++unmarshall 1: the samplerate = " << m_SampleRate << LOG_ENDL;
        m_SoundCheckVal = 0 | returnCommand[i+4]<<24 | returnCommand[i+5]<<16 | returnCommand[i+6]<<4 | returnCommand[i+7];
        m_VolumeAdjustment = 0 | returnCommand[i+8]<<24 | returnCommand[i+9]<<16 | returnCommand[i+10]<<4 | returnCommand[i+11];
    }
}

int CTrackNewAudioAttributes :: GetSampleRate()
{
    int sr = m_SampleRate;
    //LOG_INFO << " ++++++++Trac changed: the samplerate = " << m_SampleRate << LOG_ENDL;
    return sr;
}
/*
int CTrackNewAudioAttributes :: GetSoundCheckVal()
{
    return m_SoundCheckVal;
}

int CTrackNewAudioAttributes :: GetTrackVolumeAdjustment()
{
    return m_VolumeAdjustment;
}
*/
