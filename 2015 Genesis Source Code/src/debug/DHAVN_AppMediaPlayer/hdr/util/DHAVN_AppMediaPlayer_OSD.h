#ifndef HOME_MEDIAPLAYER_OSD_H
#define HOME_MEDIAPLAYER_OSD_H

#include <QObject>
#include <QList>
#include "DHAVN_StatusControl_Def.h"

// added by eugene 2012.08.21 for stack over flow

class OSDMediaInfoManager
{
public:
        static OSD_MEDIA_INFO* getMediaInfo();
        static void putMediaInfo(OSD_MEDIA_INFO *pInfo);
private:
        static QList<OSD_MEDIA_INFO*> m_tMediaInfoList;
};

#endif
