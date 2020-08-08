#ifndef	OSD_ENTRY_H_
#define	OSD_ENTRY_H_

#include <uistate_manager_def.h>

class OSDEntry{
public:
	static OSDEntry * instance();
	void setFrontEntry(APP_ENTRY_POINT_T entry);
	void setRearEntry(APP_ENTRY_POINT_T entry);

	APP_ENTRY_POINT_T frontEntry() { return m_frontEntry;}
	APP_ENTRY_POINT_T rearEntry() { return m_rearEntry; }

	static QString EntryToString(APP_ENTRY_POINT_T entryPoint);
	static bool IsVideoAppEntry(APP_ENTRY_POINT_T entry);
	static bool IsVideoAppDRSEntry(APP_ENTRY_POINT_T entry); // added by cychoi 2014.02.24 for ITS 226456 always display front OSD on DRS mode
	static bool IsMusicAppEntry(APP_ENTRY_POINT_T entry);
	static bool IsAvAppEntry(APP_ENTRY_POINT_T entry);

	static bool IsImageAppEntry(APP_ENTRY_POINT_T entry);
	static bool IsAuxEntry(APP_ENTRY_POINT_T entry);
	static bool IsIpodEntry(APP_ENTRY_POINT_T entry);
	static bool IsDiscEntry(APP_ENTRY_POINT_T entry);
	static bool IsUsbEntry(APP_ENTRY_POINT_T entry);

private:
	static OSDEntry * pOSDEntry;

	OSDEntry();
	APP_ENTRY_POINT_T m_frontEntry;
	APP_ENTRY_POINT_T m_rearEntry;
};

#endif
