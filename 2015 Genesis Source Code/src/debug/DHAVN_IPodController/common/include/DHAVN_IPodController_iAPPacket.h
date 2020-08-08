/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                    IAP packet base class, all the IAP commands *
 *                    will be derived from this class             *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef IAPPACKET_H
#define IAPPACKET_H

#include <QByteArray>
#include <QObject>
#include <QDebug>
//#include <DHAVN_IPodController_Def.h>

class CIAPCmds : public QObject
{
public:
    enum CIAPLingoId{
        GeneralLingoId      = 0x00,
        DisplayRemote       = 0x03,
        ExtenededLingoId    = 0x04,
        DigitalLingoId      = 0x0A,
        StorageLingoId      = 0x0C
    };

    enum CIAPGenaralLingoCmds{
        L0_00_RequestIdentify                               = 0x00,   // All
        L0_01_Identify                                      = 0x01,   // All, deprecated
        L0_02_iPodAck                                       = 0x02,   // 1.00
        L0_03_RequestExtendedInterfaceMode                  = 0x03,   // 1.00
        L0_04_ReturnExtendedInterfaceMode                   = 0x04,   // 1.00
        L0_05_EnterExtendedInterfaceMode                    = 0x05,   // deprecated, use SetUIMode
        L0_06_ExitEntendedInterfaceMode                     = 0x06,   // 1.00
        L0_07_RequestiPodName                               = 0x07,   // 1.00
        L0_08_ReturniPodName                                = 0x08,   // 1.00
        L0_09_ReqestiPodSoftwareversion                     = 0x09,   // 1.00
        L0_0A_ReturniPodSoftwareVersion                     = 0x0A,   // 1.00
        L0_0B_RequestiPodSerialNum                          = 0x0B,   // 1.00
        L0_0C_ReturniPodSerialNum                           = 0x0C,   // 1.00
        L0_0D_RequestiPodModelNum                           = 0x0D,   // deprecated, use GetiPodOptionsForLingo
        L0_0E_ReturniPodModelNum                            = 0x0E,   // deprecated, use RetiPodOptionsForLingo
        L0_0F_RequestLingoProtocolVersion                   = 0x0F,   // 1.00
        L0_10_ReturnLingoProtocolVersion                    = 0x10,   // 1.00
        L0_11_RequestTransportMaxPayloadSize                = 0x11,   // 1.09
        L0_12_ReturnTransportMaxPayloadSize                 = 0x12,   // 1.09
        L0_13_IdentifyDeviceLingoes                         = 0x13,   // 1.01
        L0_14_GetAccessoryAuthenticationInfo                = 0x14,   // 1.01
        L0_15_RetAccessoryAuthenticationInfo                = 0x15,   // 1.01
        L0_16_AckAccessoryAuthenticationInfo                = 0x16,   // 1.01
        L0_17_GetAccessoryAuthenticationSignature           = 0x17,   // 1.01
        L0_18_RetAccessoryAuthenticationSignature           = 0x18,   // 1.01
        L0_19_AckAccessoryAuthenticationStatus              = 0x19,   // 1.01
        L0_1A_GetiPodAuthenticationInfo                     = 0x1A,   // 1.01
        L0_1B_RetiPodAuthenticationInfo                     = 0x1B,   // 1.01
        L0_1C_AetiPodAuthenticationInfo                     = 0x1C,   // 1.01
        L0_1D_GetiPodAuthenticationSignature                = 0x1D,   // 1.01
        L0_1E_RetiPodAuthenticationSignature                = 0x1E,   // 1.01
        L0_1F_AckiPodAuthenticationStatus                   = 0x1F,   // 1.01
        // Reserved 0x20~0x22
        L0_23_NotifyiPodStateChange                         = 0x23,   // 1.02
        L0_24_GetiPodOptions                                = 0x24,   // 1.05
        L0_25_RetiPodOptions                                = 0x25,   // 1.05
        // Reserved 0x26
        L0_27_GetAccessoryInfo                              = 0x27,   // 1.04
        L0_28_RetAccessoryInfo                              = 0x28,   // 1.04
        L0_29_GetiPodPreferences                            = 0x29,   // 1.05
        L0_2A_RetiPodPreferences                            = 0x2A,   // 1.05
        L0_2B_SetiPodPreferences                            = 0x2B,   // 1.05
        // Reserved 0x2C~0x34
        L0_35_GetUIMode                                     = 0x35,   // 1.09
        L0_36_RetUIMode                                     = 0x36,   // 1.09
        L0_37_SetUIMode                                     = 0x37,   // 1.09
        L0_38_StartIDPS                                     = 0x38,   // 1.09
        L0_39_SetFIDTokenValues                             = 0x39,   // 1.09
        L0_3A_AckFIDTokenValues                             = 0x3A,   // 1.09
        L0_3B_EndIDPS                                       = 0x3B,   // 1.09
        L0_3C_IDPSStatus                                    = 0x3C,   // 1.09
        // Reserved 0x3D~0x3E
        L0_3F_OpenDataSessionForProtocol                    = 0x3F,   // 1.09
        L0_40_CloseDataSession                              = 0x40,   // 1.09
        L0_41_AccessoryAck                                  = 0x41,   // 1.09
        L0_42_AccessoryDataTransfer                         = 0x42,   // 1.09
        L0_43_iPodDataTransfer                              = 0x43,   // 1.09
        // Reserved 0x44~0x45
        L0_46_SetAccessoryStatusNotification                = 0x46,   // 1.09
        L0_47_RetAccessoryStatusNotification                = 0x47,   // 1.09
        L0_48_AccessoryStatusNotification                   = 0x48,   // 1.09
        L0_49_SetEventNotification                          = 0x49,   // 1.09
        L0_4A_iPodNotification                              = 0x4A,   // 1.09
        L0_4B_GetiPodOptionsForLingo                        = 0x4B,   // 1.09
        L0_4C_RetiPodOptionsForLingo                        = 0x4C,   // 1.09
        L0_4D_GetEventNotification                          = 0x4D,   // 1.09
        L0_4E_RetEventNotification                          = 0x4E,   // 1.09
        L0_4F_GetSupportedEventNotification                 = 0x4F,   // 1.09
        L0_50_CancelCommand                                 = 0x50,   // 1.09
        L0_51_RetSupportedEventNotification                 = 0x51,   // 1.09
        // Reserved 0x52~0x53
        L0_54_SetAvailableCurrent                           = 0x54,   // 1.09
        // Reserved 0x55
        L0_56_SetInternalBatteryChargingState               = 0x56,   // 1.09
        // Reserved 0x57~0x63
        L0_64_RequestApplicationLaunch                      = 0x64,   // 1.09
        L0_65_GetNowPlayingApplicationBundleName            = 0x65,   // 1.09
        L0_66_RetNowPlayingApplicationBundleName            = 0x66,   // 1.09
        L0_67_GetLocalizationInfo                           = 0x67,   // 1.09
        L0_68_RetLocalizationInfo                           = 0x68,   // 1.09
        L0_69_RequestWifiCommectionInfo                     = 0x69,   // 1.09
        L0_6A_WifiConnectionInfo                            = 0x6A,   // 1.09
        // Reserved 0x6B~0xFF
        L0_F0_DummyCommand                                  = 0xF0    // not real command
    };

    enum CIAPExtendedCmds{
        // Reserved 0x00
        L4_01_iPodAck                                       = 0x0001,   // 1.00
        L4_02_GetCurrentPlayingTrackChapterInfo             = 0x0002,   // 1.06
        L4_03_ReturnCurrentPlayingTrackChapterInfo          = 0x0003,   // 1.06
        L4_04_SetCurrentPlayingTrackChapter                 = 0x0004,   // 1.06
        L4_05_GetCurrentPlayingTrackChapterPlayStatus       = 0x0005,   // 1.06
        L4_06_ReturnCurrentPlayingTrackChapterPlayStatus    = 0x0006,   // 1.06
        L4_07_GetCurrentPlayingTrackChapterName             = 0x0007,   // 1.06
        L4_08_ReturnCurrentPlayingTrackChapterName          = 0x0008,   // 1.06
        L4_09_GetAudiobookSpeed                             = 0x0009,   // 1.06
        L4_0A_ReturnAudiobookSpeed                          = 0x000A,   // 1.06
        L4_0B_SetAudiobookSpeed                             = 0x000B,   // 1.06
        L4_0C_GetIndexedPlayingTrackInfo                    = 0x000C,   // 1.08
        L4_0D_ReturnIndexedPlayingTrackInfo                 = 0x000D,   // 1.08
        L4_0E_GetArtworkFormats                             = 0x000E,   // 1.10
        L4_0F_RetArtworkFormats                             = 0x000F,   // 1.10
        L4_10_GetTrackArtworkData                           = 0x0010,   // 1.10
        L4_11_RetTrackArtworkData                           = 0x0011,   // 1.10
        L4_12_RequestProtocolVersion                        = 0x0012,   // deprecated
        L4_13_ReturnProtocolVersion                         = 0x0013,   // deprecated
        L4_14_RequestiPodName                               = 0x0014,   // deprecated
        L4_15_ReturniPodName                                = 0x0015,   // deprecated
        L4_16_ResetDBSelection                              = 0x0016,   // 1.00
        L4_17_SelectDBRecord                                = 0x0017,   // 1.00
        L4_18_GetNumberCategorizedDBRecords                 = 0x0018,   // 1.00
        L4_19_ReturnNumberCategorizedDBRecords              = 0x0019,   // 1.00
        L4_1A_ReceiveCategorizedDatabaseRecords             = 0x001A,   // 1.00
        L4_1B_ReturnCategorizedDatabaseRecords              = 0x001B,   // 1.00
        L4_1C_GetPlayStatus                                 = 0x001C,   // 1.00
        L4_1D_ReturnPlayStatus                              = 0x001D,   // 1.00
        L4_1E_GetCurrentPlayingTrackIndex                   = 0x001E,   // 1.00
        L4_1F_ReturnCurrentPlayingTrackIndex                = 0x001F,   // 1.00
        L4_20_GetIndexedPlayingTrackTitle                   = 0x0020,   // 1.00
        L4_21_ReturnIndexedPlayingTrackTitle                = 0x0021,   // 1.00
        L4_22_GetIndexedPlayingTrackArtistName              = 0x0022,   // 1.00
        L4_23_ReturnIndexedPlayingTrackArtistName           = 0x0023,   // 1.00
        L4_24_GetIndexedPlayingTrackAlbumName               = 0x0024,   // 1.00
        L4_25_ReturnIndexedPlayingTrackAlbumName            = 0x0025,   // 1.00
        L4_26_SetPlayStatusChangeNotification               = 0x0026,   // 1.00
        L4_27_PlayStatusChangeNotification                  = 0x0027,   // 1.00
        L4_28_PlayCurrentSelection                          = 0x0028,   // deprecated, use SelectDBRecord
        L4_29_PlayControl                                   = 0x0029,   // 1.00
        L4_2A_GetTrackArtworkTimes                          = 0x002A,   // 1.10
        L4_2B_RetTrackArtworkTimes                          = 0x002B,   // 1.10
        L4_2C_GetShuffle                                    = 0x002C,   // 1.00
        L4_2D_ReturnShuffle                                 = 0x002D,   // 1.00
        L4_2E_SetShuffle                                    = 0x002E,   // 1.00
        L4_2F_GetRepeat                                     = 0x002F,   // 1.00
        L4_30_ReturnRepeat                                  = 0x0030,   // 1.00
        L4_31_SetRepeat                                     = 0x0031,   // 1.00
        L4_32_SetDisplayImage                               = 0x0032,   // 1.01
        L4_33_GetMonoDisplayImageLimits                     = 0x0033,   // 1.01
        L4_34_ReturnMonoDisplayImageLimits                  = 0x0034,   // 1.01
        L4_35_GeNumPlayingTracks                            = 0x0035,   // 1.01
        L4_36_ReturnNumPlayingTracks                        = 0x0036,   // 1.01
        L4_37_SetCurrentPlayingTracks                       = 0x0037,   // 1.01
        L4_38_SelectSortDBRecord                            = 0x0038,   // deprecated, use SelectSortDBRecord
        L4_39_GetColorDisplayImageLimits                    = 0x0039,   // 1.09
        L4_3A_ReturnColorDisplayImageLimits                 = 0x003A,   // 1.09
        L4_3B_ResetDBSelectionHierarchy                     = 0x003B,   // 1.11
        L4_3C_GetDBiTunrsInfo                               = 0x003C,   // 1.13
        L4_3D_RetDBiTunesInfo                               = 0x003D,   // 1.13
        L4_3E_GetUIDTrackInfo                               = 0x003E,   // 1.13
        L4_3F_RetUIDTrackInfo                               = 0x003F,   // 1.13
        L4_40_GetDBTrackInfo                                = 0x0040,   // 1.13
        L4_41_RetDBTrackInfo                                = 0x0041,   // 1.13
        L4_42_GetPBTrackInfo                                = 0x0042,   // 1.13
        L4_43_RetPBTrackInfo                                = 0x0043,   // 1.13
        L4_44_CreateGeniusPlaylist                          = 0x0044,   // 1.13
        L4_45_RefreshGeniusPlaylist                         = 0x0045,   // 1.13
        // Reserve 0x46
        L4_47_IsGeniusAvailableForTrack                     = 0x0047,   // 1.13
        L4_48_GetPlaylistInfo                               = 0x0048,   // 1.13
        L4_49_RetPlaylistInfo                               = 0x0049,   // 1.13
        L4_4A_PrepareUIDList                                = 0x004A,   // 1.14
        L4_4B_PlayPreparedUIDList                           = 0x004B,   // 1.14
        L4_4C_GetTrackArtworkTimesUID                       = 0x004C,   // 1.14
        L4_4D_RetTrackArtworkTimesUID                       = 0x004D,   // 1.14
        L4_4E_GetTrackArtworkDataUID                        = 0x004E,   // 1.14
        L4_4F_RetTrackArtworkDataUID                        = 0x004F,   // 1.14
        // Reserved 0x50~0xFFFF
        L4_F0_DummyCommand                                  = 0x00F0,   // not real command
        L4_F1_GetIndexedTrackMetadata                       = 0x00F1    // not real command
    };

    enum CIAPDigitalLingoCmds{
        LA_00_AccessoryAck                                  = 0x00,     // 1.00
        LA_01_iPodAck                                       = 0x01,     // 1.00
        LA_02_GetAccessorySampleRateCaps                    = 0x02,     // 1.00
        LA_03_RetAccessorySampleRateCaps                    = 0x03,     // 1.00
        LA_04_TrackNewAudioAttributes                       = 0x04,     // 1.00
        LA_05_SetVideoDelay                                 = 0x05      // 1.03
        // Reserved 0x06~0xFF
    };

    enum CIAPStorageLingoCmds{
        LC_00_iPodAck                                       = 0x00,     // 1.01
        LC_01_GetiPodCaps                                   = 0x01,     // 1.01
        LC_02_RetiPodCaps                                   = 0x02,     // 1.01
        // Reserved 0x03
        LC_04_ReturniPodFileHandle                          = 0x04,     // 1.01
        // Reserved 0x05~0x06
        LC_07_WriteiPodFileData                             = 0x07,     // 1.01
        LC_08_CloseiPodFile                                 = 0x08,     // 1.01
        // Reserved 0x09~0x0F
        LC_10_GetiPodFreeSpace                              = 0x10,     // 1.01
        LC_11_RetiPodFreeSpace                              = 0x11,     // 1.01
        LC_12_OpeniPodFeatureFile                           = 0x12,     // 1.01
        // Reserved 0x13~0x7F
        LC_80_AccessoryAck                                  = 0x80,     // 1.02
        LC_81_GetAccessoryCaps                              = 0x81,     // 1.02
        LC_82_RetAccessoryCaps                              = 0x82      // 1.02
        // Reserved 0x83~0xFF
    };
};


class CIAPPacket : public QObject
{
public:
    CIAPPacket(): m_TransactionID(-1), m_isTidEnabled(false),
        m_LingoID(-1), m_CommandID(-1) {} //CR 15209 added by jungae 2012.10.29, Static_Test 0134470

//    CIAPPacket(QObject* parent):QObject(parent){}
    CIAPPacket(CIAPPacket& args):QObject(&args){m_CommandBytes = args.getCommandBytes();}
    CIAPPacket(QByteArray array) : m_TransactionID(-1),m_isTidEnabled(false),m_LingoID(-1),m_CommandID(-1) {m_CommandBytes = array;} //m_isTidEnabled is not initalized.coverity fix - UnInitialized constant
    CIAPPacket(int lingo,int command,int transactionId, bool isTidEnabled) {
        m_LingoID = lingo;
        m_CommandID = command;
        m_TransactionID = transactionId;
        m_isTidEnabled = isTidEnabled;
    }
    virtual QByteArray marshall() {return NULL;}
    virtual void unmarshall(QByteArray) {}
    QByteArray getCommandBytes()
    {
        return m_CommandBytes;
    }
    int getTransactionId()
    {
        return m_TransactionID;
    }
    int getLingoID()
    {
        return m_LingoID;
    }
    int getCommandID()
    {
        return m_CommandID;
    }

protected:
    QByteArray m_CommandBytes;
    int m_TransactionID;
    bool m_isTidEnabled;
public:
    int m_LingoID;
    int m_CommandID;
};
#endif //IAPPACKET_H
