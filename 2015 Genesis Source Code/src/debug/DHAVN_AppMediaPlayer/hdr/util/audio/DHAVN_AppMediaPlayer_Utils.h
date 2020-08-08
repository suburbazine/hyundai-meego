#ifndef DHAVN_APPMEDIAPLAYER_AUDIOUTILS_H
#define DHAVN_APPMEDIAPLAYER_AUDIOUTILS_H

#include "controller/DHAVN_AppMediaPlayer_ControllerAudio.h"
#include "util/DHAVN_AppMediaPlayer_MediaManager.h"


class AudioUtils
{
public:

    static MEDIA_SOURCE_T DeviceType(MP::DEVICE_T internalType);
    static MEDIA_SOURCE_T DeviceType(ETrackerAbstarctor_VolumeType externalType);
    static MP::DEVICE_T DeviceType(MODE_STATE_T externalType);
    static MP::DEVICE_T DeviceType(MEDIA_SOURCE_T externalType);
    static MP::DEVICE_T DeviceByVolume(ETrackerAbstarctor_VolumeType externalType); // added by eugeny.novikov 2012.10.25 for CR 14047

    static ETrackerAbstarctor_VolumeType VolumeType(MP::DEVICE_T internalType);

    static QString GetFrontUSBPath();
    static QString GetRearUSBPath();

    static QString GetUSBRoot();

    static QString m_strUSBRoot;
    static QString m_strUSB1Path;
    static QString m_strUSB2Path;    

    static wchar_t getFirstChar(QString title, bool bIPodSort = false); //added by junam 2013.06.19 for iPod sorted first char

#ifdef AVP_LAST_AUDIOFILE_BACKUP
    static void backupFile(QString filePath, qint64 limitedFileSize);
#endif
};

#endif // DHAVN_APPMEDIAPLAYER_AUDIOUTILS_H
