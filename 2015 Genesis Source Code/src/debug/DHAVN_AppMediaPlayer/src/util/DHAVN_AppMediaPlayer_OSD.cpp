#include <util/DHAVN_AppMediaPlayer_OSD.h>
#include "DHAVN_StatusControl_Def.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"


// added by eugene 2012.08.21 for stack over flows

QList<OSD_MEDIA_INFO*> OSDMediaInfoManager::m_tMediaInfoList;

OSD_MEDIA_INFO* OSDMediaInfoManager::getMediaInfo()
{
	if (m_tMediaInfoList.empty())
	{
		OSD_MEDIA_INFO *media_info = new OSD_MEDIA_INFO;			
		MP_LOG << "[MP]" << "OSDMediaInfoManager::getMediaInfo()" << "new OSD_MEDIA_INFO" << LOG_ENDL;
		m_tMediaInfoList.append(media_info);
	}
	
	return m_tMediaInfoList.takeFirst();
}

void OSDMediaInfoManager::putMediaInfo(OSD_MEDIA_INFO *pInfo)
{
        m_tMediaInfoList.append(pInfo);
}
