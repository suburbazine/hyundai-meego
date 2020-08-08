#ifndef	OSD_MODE_H_
#define	OSD_MODE_H_

#include <uistate_manager_def.h>

class OSDMode{
public:
	static OSDMode * instance();
	MODE_STATE_T mode() { return m_mode; }
	void setMode(MODE_STATE_T entry);

	static QString ModeToString(MODE_STATE_T mode);

	static bool IsVideoMode(MODE_STATE_T mode);
	static bool IsAudioMode(MODE_STATE_T mode);

	static bool IsAuxMode(MODE_STATE_T mode);
	static bool IsIpodMode(MODE_STATE_T mode);
	static bool IsDiscMode(MODE_STATE_T mode);
	static bool IsUsbMode(MODE_STATE_T mode);
private:
	static OSDMode * pOSDMode;
	OSDMode();

	MODE_STATE_T m_mode;
};

#endif
