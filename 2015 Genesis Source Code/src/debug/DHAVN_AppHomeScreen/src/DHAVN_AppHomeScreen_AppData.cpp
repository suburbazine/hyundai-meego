#include <DHAVN_AppHomeScreen_AppData.h>
#include "DHAVN_AppFramework_AppEngineBase.h"

struct DATA_T
{
    HSDefP::APP_DATA_T nId;
    QList<HSDefP::APP_DATA_T> lData;
};

const DATA_T DATA_DEPENDS[] =
{
    { HSDefP::APP_DATA_ENABLED_MEDIA,       DataList << HSDefP::APP_DATA_AVAILABLE_JBOX_VIDEO
      << HSDefP::APP_DATA_AVAILABLE_JBOX_AUIDIO
      << HSDefP::APP_DATA_AVAILABLE_JBOX_IMAGE
      << HSDefP::APP_DATA_AVAILABLE_USB1_VIDEO
      << HSDefP::APP_DATA_AVAILABLE_USB1_AUDIO
      << HSDefP::APP_DATA_AVAILABLE_USB1_IMAGE
      << HSDefP::APP_DATA_AVAILABLE_USB2_VIDEO
      << HSDefP::APP_DATA_AVAILABLE_USB2_AUDIO
      << HSDefP::APP_DATA_AVAILABLE_USB2_IMAGE
      << HSDefP::APP_DATA_AVAILABLE_DISC_DVD_VIDEO
      << HSDefP::APP_DATA_AVAILABLE_DISC_VCD
      << HSDefP::APP_DATA_AVAILABLE_DISC_DVD_AUDIO
      << HSDefP::APP_DATA_AVAILABLE_DISC_MP3
      << HSDefP::APP_DATA_AVAILABLE_DISC_CD
      << HSDefP::APP_DATA_AVAILABLE_IPOD1
      << HSDefP::APP_DATA_AVAILABLE_PANDORA1
      << HSDefP::APP_DATA_AVAILABLE_AHA1
      << HSDefP::APP_DATA_AVAILABLE_IPOD2
      << HSDefP::APP_DATA_AVAILABLE_PANDORA2
      << HSDefP::APP_DATA_AVAILABLE_AHA2
      << HSDefP::APP_DATA_AVAILABLE_AUX
      << HSDefP::APP_DATA_AVAILABLE_BT_PANDORA
      << HSDefP::APP_DATA_AVAILABLE_BT_AHA
      << HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_BT },

    { HSDefP::APP_DATA_ENABLED_MEDIA_VIDEO, DataList << HSDefP::APP_DATA_ENABLED_MEDIA_VIDEO_AUX
      << HSDefP::APP_DATA_ENABLED_MEDIA_VIDEO_DISC
      << HSDefP::APP_DATA_ENABLED_MEDIA_VIDEO_JBOX
      << HSDefP::APP_DATA_ENABLED_MEDIA_VIDEO_USB1
      << HSDefP::APP_DATA_ENABLED_MEDIA_VIDEO_USB2 },

    { HSDefP::APP_DATA_ENABLED_MEDIA_PHOTO, DataList << HSDefP::APP_DATA_ENABLED_MEDIA_PHOTO_JBOX
      << HSDefP::APP_DATA_ENABLED_MEDIA_PHOTO_USB1
      << HSDefP::APP_DATA_ENABLED_MEDIA_PHOTO_USB2 },

    { HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO, DataList << HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_AUX
      << HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_BT
      << HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_DISC
      << HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_IPOD1
      << HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_IPOD2
      << HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_JBOX
      << HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_USB1
      << HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_USB2
      << HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_PANDORA
      << HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_AHA},
    { HSDefP::APP_DATA_MAX,                 DataList }
};

const MEDIA_CONTENT_MAP_T MEDIA_MAP[] =
{
    { HomeScreenDef::MEDIA_SOURCE_JBOX,        HSDefP::APP_DATA_AVAILABLE_JBOX,
      HSDefP::APP_DATA_AVAILABLE_JBOX_VIDEO,
      HSDefP::APP_DATA_AVAILABLE_JBOX_IMAGE,
      HSDefP::APP_DATA_AVAILABLE_JBOX_AUIDIO,
      "Jukebox" },
    { HomeScreenDef::MEDIA_SOURCE_USB1,        HSDefP::APP_DATA_AVAILABLE_USB1,
      HSDefP::APP_DATA_AVAILABLE_USB1_VIDEO,
      HSDefP::APP_DATA_AVAILABLE_USB1_IMAGE,
      HSDefP::APP_DATA_AVAILABLE_USB1_AUDIO,
      "USB1" },
    { HomeScreenDef::MEDIA_SOURCE_USB2,        HSDefP::APP_DATA_AVAILABLE_USB2,
      HSDefP::APP_DATA_AVAILABLE_USB2_VIDEO,
      HSDefP::APP_DATA_AVAILABLE_USB2_IMAGE,
      HSDefP::APP_DATA_AVAILABLE_USB2_AUDIO,
      "USB2" },
    { HomeScreenDef::MEDIA_SOURCE_CD_AUDIO,    HSDefP::APP_DATA_AVAILABLE_DISC,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_AVAILABLE_DISC_CD,
      "Audio CD" },
    { HomeScreenDef::MEDIA_SOURCE_CD_DATA,     HSDefP::APP_DATA_AVAILABLE_DISC,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_AVAILABLE_DISC_MP3,
      "Data CD" },
    { HomeScreenDef::MEDIA_SOURCE_VCD,         HSDefP::APP_DATA_AVAILABLE_DISC,
      HSDefP::APP_DATA_AVAILABLE_DISC_VCD,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_INVALID,
      "VCD" },
    { HomeScreenDef::MEDIA_SOURCE_DVD_AUDIO,   HSDefP::APP_DATA_AVAILABLE_DISC,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_AVAILABLE_DISC_DVD_AUDIO,
      "DVD Audio" },
    { HomeScreenDef::MEDIA_SOURCE_DVD_VIDEO,   HSDefP::APP_DATA_AVAILABLE_DISC,
      HSDefP::APP_DATA_AVAILABLE_DISC_DVD_VIDEO,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_INVALID,
      "DVD Video" },
    { HomeScreenDef::MEDIA_SOURCE_IPOD1,       HSDefP::APP_DATA_AVAILABLE_IPOD1,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_AVAILABLE_IPOD1,
      "IPOD1" },
    { HomeScreenDef::MEDIA_SOURCE_IPOD2,       HSDefP::APP_DATA_AVAILABLE_IPOD2,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_AVAILABLE_IPOD2,
      "IPOD2" },
    { HomeScreenDef::MEDIA_SOURCE_AUX,         HSDefP::APP_DATA_AVAILABLE_AUX,
      HSDefP::APP_DATA_ENABLED_MEDIA_VIDEO_AUX,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_AUX,
      "AUX" },
    { HomeScreenDef::MEDIA_SOURCE_BLUETOOTH,   HSDefP::APP_DATA_AVAILABLE_BT,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_BT,
      "Bluetooth" },

    { HomeScreenDef::MEDIA_SOURCE_MAX,         HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_INVALID,
      HSDefP::APP_DATA_INVALID,
      "" }
};

AppData::AppData()
{
    /*
    for( int i = HSDefP::APP_DATA_STRING_MINIMUM + 1; i < HSDefP::APP_DATA_STRING_MAXIMUM; i++ )
    {
        SetValue( (HSDefP::APP_DATA_T)i, QVariant( QString("") ) );
    }

    for( int i = HSDefP::APP_DATA_AVAILABLE_MINIMUM + 1; i < HSDefP::APP_DATA_AVAILABLE_MAXIMUM; i++ )
    {
        SetValue( (HSDefP::APP_DATA_T)i, QVariant( false ) );
    }

    for( int i = HSDefP::APP_DATA_ENABLED_MINIMUM + 1; i < HSDefP::APP_DATA_ENABLED_MAXIMUM; i++ )
    {
        SetValue( (HSDefP::APP_DATA_T)i, QVariant( false ) );
    }
    */

    SetValue( HSDefP::APP_DATA_ENABLED_VR, QVariant( GetSettingsVRIconInfo( AppEngineBase::GetLanguage() ) ) );
    SetValue( HSDefP::APP_DATA_ENABLED_HELP, QVariant( GetSettingsHELPIconInfo( AppEngineBase::GetLanguage() ) ) );
    //SetValue( HSDefP::APP_DATA_ENABLED_DRIVING_MODE, QVariant(true) );
    SetValue( HSDefP::APP_DATA_AVAILABLE_NAVI, QVariant( AppEngineBase::GetSystemVariant() & 0x1 ) );
    SetValue( HSDefP::APP_DATA_AVAILABLE_IBOX_MODEM, QVariant( AppEngineBase::GetSystemVariant() & 0x2 ) );
    SetValue( HSDefP::APP_DATA_ENABLED_NAVI, QVariant( QFile::exists( "/navi/bin/AppNavi" ) ) );
    // not used SetValue( HSDefP::APP_DATA_AVAILABLE_DAB, QVariant( QFile::exists( "/app/bin/AppDAB" ) ) );
    SetValue( HSDefP::APP_DATA_ENABLED_PARKED, QVariant( true ) ); /** By default car is parked */
    //HS_LOG( "GetSystemVariant() = " << AppEngineBase::GetSystemVariant() );

}

AppData::~AppData()
{

}

bool AppData::GetSettingsVRIconInfo(LANGUAGE_T language)
{
    ECountryVariant CV = AppEngineBase::GetCountryVariant();

    switch (CV)
    {
    case eCVKorea:
    {
        if (language == LANGUAGE_KO)    return true;
        else                            return false;
    }
    case eCVCanada:
    case eCVNorthAmerica:
    {
        if (language == LANGUAGE_KO)    return false;
        else                            return true;
    }

    case eCVGeneral: return false;
    case eCVChina: return false;

    case eCVMiddleEast:
    {
        if (language == LANGUAGE_AR) return true;
        else                         return false;
    }
    case eCVRussia:
    case eCVEuropa:
    {
        if (language == LANGUAGE_KO || language == LANGUAGE_SK || language == LANGUAGE_EN || language == LANGUAGE_EN_US) return false;
        else                                                    return true;
    }

    default: return false;
    }
}

bool AppData::GetSettingsHELPIconInfo(LANGUAGE_T language)
{
    ECountryVariant CV = AppEngineBase::GetCountryVariant();

    switch (CV)
    {
    case eCVKorea:
    {
        if (language == LANGUAGE_KO)    return true;
        else                            return false;
    }
/*
    case eCVCanada:
    case eCVNorthAmerica:
    case eCVGeneral:
    case eCVChina:
    case eCVMiddleEast:
    case eCVRussia:
    case eCVEuropa:
    {
        return true;
    }
*/
    default: return true;
    }
}

QVariant AppData::GetValue( HSDefP::APP_DATA_T nId )
{
    return m_AppData[ nId ];
}

void AppData::SetValue( HSDefP::APP_DATA_T nId, QVariant value )
{
    LW_LOG( "nId = " << nId << " value = " << value.toString() );

    if( HSDefP::APP_DATA_INVALID == nId ) return;
    if( value == m_AppData[ nId ] ) return;

    m_AppData[ nId ] = value;

    //! Change regarding CR 13401
    //! If audio or video aux is enabled, enable aux. Entry point is audio aux.
    if ( ( nId == HSDefP::APP_DATA_ENABLED_MEDIA_VIDEO_AUX ) ||
            ( nId == HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_AUX ) )
    {
        if ( ( m_AppData[ HSDefP::APP_DATA_ENABLED_MEDIA_VIDEO_AUX ].toBool() == true ) ||
                ( m_AppData[ HSDefP::APP_DATA_ENABLED_MEDIA_AUDIO_AUX ].toBool() == true ) )
        {
            LW_LOG( "Set Aux value to true" );
            m_AppData[ HSDefP::APP_DATA_ENABLED_MEDIA_AUX ] = true;
            emit ValueChanged( HSDefP::APP_DATA_ENABLED_MEDIA_AUX, QVariant(true) );
        }
        else
        {
            LW_LOG( "Set Aux value to false" );
            m_AppData[ HSDefP::APP_DATA_ENABLED_MEDIA_AUX ] = false;
            emit ValueChanged( HSDefP::APP_DATA_ENABLED_MEDIA_AUX, QVariant(false) );
        }
    }
    else
    {
        emit ValueChanged( nId, value );
    }

    UpdateDependentId( nId );
}

void AppData::UpdateDependentId( HSDefP::APP_DATA_T nId )
{
    HSDefP::APP_DATA_T nDependentId = GetDependentId( nId );

    if( HSDefP::APP_DATA_INVALID != nDependentId )
    {
        LW_LOG( "nDependentId = " << nDependentId );

        int nIndex = 0;
        bool bEnabled = false;

        while( HSDefP::APP_DATA_MAX != DATA_DEPENDS[ nIndex ].nId )
        {
            if( nDependentId == DATA_DEPENDS[ nIndex ].nId )
            {
                for( int i = 0; i < DATA_DEPENDS[ nIndex ].lData.size(); i++ )
                {
                    bEnabled |= m_AppData[ DATA_DEPENDS[ nIndex ].lData[ i ] ].toBool();
                }
            }

            nIndex++;
        }

        SetValue( nDependentId, QVariant( bEnabled ) );
    }
}

bool AppData::GetParkingMode()
{
    return m_AppData[ HSDefP::APP_DATA_ENABLED_PARKED ].toBool();
}

HSDefP::APP_DATA_T AppData::GetDependentId( HSDefP::APP_DATA_T nId )
{
    int nIndex = 0;

    while( HSDefP::APP_DATA_MAX != DATA_DEPENDS[ nIndex ].nId )
    {
        for( int i = 0; i < DATA_DEPENDS[ nIndex ].lData.size(); i++ )
        {
            if( nId == DATA_DEPENDS[ nIndex ].lData[ i ] )
            {
                return DATA_DEPENDS[ nIndex ].nId;
            }
        }

        nIndex++;
    }

    return HSDefP::APP_DATA_INVALID;
}

void
AppData::SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_T nSource, bool bDeviceConnected,
                               bool bVideo, bool bPhoto, bool bAudio )
{
    MEDIA_CONTENT_MAP_T sData = GetDataFields( nSource );

    if ( bDeviceConnected ) {
    HS_LOG( "[MEDIA] nSource = " << nSource <<
           " Device = " << sData.sDeviceName <<
           " Device = " << sData.nDevice<<
           " bDeviceConnected = " << bDeviceConnected <<
           " bVideo = " << bVideo <<
           " bPhoto = " << bPhoto <<
           " bAudio = " << bAudio );
    }
    else {
        LW_LOG( "[MEDIA] nSource = " << nSource <<
               " Device = " << sData.sDeviceName <<
               " Device = " << sData.nDevice<<
               " bDeviceConnected = " << bDeviceConnected <<
               " bVideo = " << bVideo <<
               " bPhoto = " << bPhoto <<
               " bAudio = " << bAudio );
    }

    //! In the case of CD / Aux, it needs to check video/photo/audio enable when device is connected.
    //! So change to set video/photo/audio enable flag in prior to device connection flag
    SetValue( sData.nVideo, QVariant( bVideo ) );
    SetValue( sData.nPhoto, QVariant( bPhoto ) );
    SetValue( sData.nAudio, QVariant( bAudio ) );

    if (nSource != HomeScreenDef::MEDIA_SOURCE_IPOD1 && nSource != HomeScreenDef::MEDIA_SOURCE_IPOD2)
    {
        if ( NeedToSetConnectedDevice( nSource, bDeviceConnected ) == true )
        {
            SetValue( sData.nDevice, QVariant( bDeviceConnected ) );
        }
    }

    /*

    if ( nSource == HomeScreenDef::MEDIA_SOURCE_IPOD1 )
    {
        SetValue( HSDefP::APP_DATA_AVAILABLE_PANDORA1, QVariant( bDeviceConnected ) );
        SetValue( HSDefP::APP_DATA_AVAILABLE_AHA1, QVariant( bDeviceConnected ) );
    }

    if ( nSource == HomeScreenDef::MEDIA_SOURCE_IPOD2 )
    {
        SetValue( HSDefP::APP_DATA_AVAILABLE_PANDORA2, QVariant( bDeviceConnected ) );
        SetValue( HSDefP::APP_DATA_AVAILABLE_AHA2, QVariant( bDeviceConnected ) );
    }

    */

}

MEDIA_CONTENT_MAP_T
AppData::GetDataFields( HomeScreenDef::MEDIA_SOURCE_T nSource )
{
    MEDIA_CONTENT_MAP_T sData = { HomeScreenDef::MEDIA_SOURCE_INVALID,
                                  HSDefP::APP_DATA_INVALID,
                                  HSDefP::APP_DATA_INVALID,
                                  HSDefP::APP_DATA_INVALID,
                                  HSDefP::APP_DATA_INVALID,
                                  "" };
    int nIndex = 0;

    while( MEDIA_MAP[ nIndex ].nSource < HomeScreenDef::MEDIA_SOURCE_MAX )
    {
        if( nSource == MEDIA_MAP[ nIndex ].nSource )
        {
            sData = MEDIA_MAP[ nIndex ];
            break;
        }

        nIndex++;
    }

    return sData;
}

bool AppData::NeedToSetConnectedDevice( HomeScreenDef::MEDIA_SOURCE_T nSource, bool bDeviceConnected )
{
    LW_LOG( "Source: " << nSource << ", DeviceConnected: " <<  bDeviceConnected );

    bool bResult = true;
    HomeScreenDef::MEDIA_SOURCE_T eEndSource;
    MEDIA_CONTENT_MAP_T sData;

    //! If it is USB1 or USB2, check whole USB's status
    if ( ( nSource == HomeScreenDef::MEDIA_SOURCE_USB1 ) ||
            ( nSource == HomeScreenDef::MEDIA_SOURCE_USB2 ) )
    {
        //! If this USB is connected
        if ( bDeviceConnected == true )
        {
            LW_LOG( "Device is connected" );

            //! If this USB was connected already, ignore to set device connected status
            if ( m_USBList.contains( nSource ) )
            {
                bResult = false;
            }
            //! If USB is not connected yet
            else
            {
                //! Set this USB to be enable
                LW_LOG( "Enable " << nSource ) ;
                m_USBList.push_back( nSource );
            }
        }
        else
        {
            LW_LOG( "Device is disconnected" );

            //! Find Disconnectd USB and Disable the USB
            if ( m_USBList.contains( nSource ) )
            {
                bResult = false;

                for (int i = 0; i < m_USBList.size(); ++i) {
                     if (m_USBList.at(i) == nSource) {
                         m_USBList.removeAt(i);
                     }
                 }
            }

            sData = GetDataFields( nSource );
            SetValue( sData.nDevice, QVariant( bDeviceConnected ) );
        }
    }

    return bResult;
}
