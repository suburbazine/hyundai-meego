#ifndef DHANV_APPFILEMANAGER_SHARED_H
#define DHANV_APPFILEMANAGER_SHARED_H

#include <QString>
#include <QVariant>

#define FILE_PATH_SIZE 1024
enum EAppFileManagerStartState
{
    eStartInvalidSState = -1,
    eStartUsbMusicTitle = 0,
    eStartUsbMusicArtist,
    eStartUsbMusicAlbum,
    eStartUsbVideo,
    eStartUsbPicture,
    eSatrtJukeMusicTitle,
    eStartJukeMusicArtist,
    eStartJukeMusicAlbum,
    eStartJukeVideo,
    eStartJukePicture,
    eStartJukeListVideo,
    eStartUsbListVideo,
    eStartJukeListPicture,
    eStartUsbListPicture,
    eAudioFileChanged,
    eVideoFileChanged,
    eVideoFileAutoChanged
// modified by Dmitry 03.10.13 for ITS0187937
};

struct AppFileManagerStartParameter
{
    EAppFileManagerStartState startState;
    char folderPath[FILE_PATH_SIZE];
    char currentPlayFileName[FILE_PATH_SIZE];
    bool copyState; // addd by eunhye 2013.02.26 for UX Scenario The process of 'Copy to Jukebox' is equal to that of list 
    bool isCopyFileUnsupported; // modified for ITS 0209355
};

// modified for ITS 0214738
struct AppFileManagerFileOperationParameter
{
    bool isFront;
    EAppFileManagerStartState mediaType;
};

// removed by Dmitry 25.07.13

struct AppFileManagerPlayParameter
{
   char filePath[FILE_PATH_SIZE];
};

#endif // DHANV_APPFILEMANAGER_SHARED_H
