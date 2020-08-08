#ifndef IUSBEXMEDIAINTERFACE_H
#define IUSBEXMEDIAINTERFACE_H

#include "imediainterface.h"
#include "mediaplayer_global.h"
#include "mediaplayer_def.h"
#include "DHAVN_TrackerAbstractor_constants.h"

class MEDIAPLAYERSHARED_EXPORT IUSBExMediaInterface : public IMediaInterface
{    
public:
    virtual void        RequestAudioLanguages() = 0;
    virtual void        SetAudioLanguage(int nStreamId) = 0;
    virtual bool        IsFileSupported(const QString &strFileName) = 0;
    virtual int         IsFileSupportedAdvanced(const QString &strFileName) = 0;
    virtual bool        getIsFileSupportedCheck() = 0; //added by jeeeun 2012-12-20 for file-check
    virtual void        setIsFileSupportedCheck(bool val) = 0; //added by jeeeun 2012-12-20 for file-check
    virtual bool        getIsSubtitleEnable() = 0; // added by jeeeun 2012-01-24 for checking subtitle enable or disable
    virtual void        DrmConfirmPlay() = 0;
    virtual void        SetFSFolderInfo(QList<FSFolderInfo> &tFolderList) = 0;
    virtual bool        isDivXDRM(int *type, int *limitcnt, int *usecount) = 0;
    virtual bool        setSubtitleLanguage(int subId) = 0;
    virtual int         getCurrentSubtitleLanguage() = 0;
    virtual void        RequestSubtitles() = 0;
};

#endif // IEXMEDIAINTERFACE_H