#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_setRemoteEventNotification.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace DisplayRemoteLingo;

CSetRemoteEventNotification::CSetRemoteEventNotification( int transId, bool isTidEnabled )
    :CIAPPacket(0x03,0x08,transId, isTidEnabled)
{
    LOG_INIT_V2(gControllerLogName);
    m_isTidEnabled = isTidEnabled;
    m_TransId = 0;
    //LOG_TRACE << "in CRetAccSampleRateCaps constructor" << LOG_ENDL;
}

QByteArray CSetRemoteEventNotification::marshall()
{
    QByteArray CommandArray;

    int Index = -1;

    /*
  * Command Packets Packet Header
  * from here
  */
    CommandArray[++Index] = 0x55; 	// Start
    CommandArray[++Index] = 0x07;   // Length
    CommandArray[++Index] = 0x03;   // Display Lingo Version
    CommandArray[++Index] = 0x08;   // SetRemoteEventNotification

    if( m_isTidEnabled == true ) {
        CommandArray[++Index] = ((m_TransactionID & 0xff00) >> 8);
        CommandArray[++Index] = m_TransactionID & 0x00ff;
    }
    /*
  * to here
  * Command Packets Packet Header
     * Total 4 Byte
     * bit Number :  Remote Event
     *  0            Track time Position in milliseconds
     *  1            Track playback Index
     *  2            Chapter Index
     *  3            Play status ( play, pause, stop, FF and RW )
     *  4            Mute / UI Volume
     *  5            Power/battery
     *  6            Equalizer setting
     *  7            Shuffle setting
     *  8            Repeat settting
     *  9            Date and time setting
     *  10           Alarm settting  // deprecated
     *  11           Backlight level
     *  12           Hold switch state
     *  13           Sound check state
     *  14           Audiobook speed
     *  15           Track time position in seconds
     *  16           Mute/UI/Absolute Volume
     *  17           Track capabilities
     *  18           Playback engine contents
     *  31:19        Reserved
     */
    CommandArray[++Index] = 0x00;
    CommandArray[++Index] = 0x00;
    CommandArray[++Index] = 0x41;  // Audiobook speed, Repeat settting
    CommandArray[++Index] = 0x88;  // Play Status, Shuffle settting

    CommandArray[1] = Index - 1;
    CommandArray.append(CIpodControllerGlobal::checkSum(CommandArray, Index));

    m_CommandBytes = CommandArray;
    return CommandArray;

}

void  CSetRemoteEventNotification::unmarshall( QByteArray )
{
}
