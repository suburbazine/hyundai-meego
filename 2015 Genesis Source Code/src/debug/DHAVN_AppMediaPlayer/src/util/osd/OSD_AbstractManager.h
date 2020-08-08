#ifndef	OSD_ABSTRACT_MANAGER_H_
#define OSD_ABSTRACT_MANAGER_H_

#include <QObject>
#include "OSD_Mode.h"
#include "OSD_Entry.h"
#include <uistate_manager_def.h>
#include "engine/DHAVN_AppMediaPlayer_EngineMain.h"
#include "util/DHAVN_AppMediaPlayer_LogHelper.h"

#define FETCH_EVENT_DATA(EventType) \
	if ( aEvent.Data().isEmpty()) return; \
	EventType * pEventData; \
	if ( ( pEventData = (EventType *) aEvent.Data().data()) == NULL) return;

class OSDAbstractManager : public QObject{
Q_OBJECT
public:
	OSDAbstractManager(AppMediaPlayer_EngineMain * pEngineMain);

	virtual void preHandleUishEvent(Event aEvent, DISPLAY_T display = DISPLAY_FRONT) = 0;
	virtual void postHandleUishEvent(Event aEvent, DISPLAY_T display = DISPLAY_FRONT) = 0;

	AppMediaPlayer_EngineMain * engine() { return m_engine; }
	MODE_STATE_T currentMode() { return m_currentMode->mode(); }

	APP_ENTRY_POINT_T currentFrontEntry(); 
	APP_ENTRY_POINT_T currentRearEntry(); 

protected:
	QString displayToString(DISPLAY_T display);

	APP_ENTRY_POINT_T mapToEntryState(MODE_STATE_T mode);

	MODE_STATE_T mapEntryToMode(APP_ENTRY_POINT_T entry);

	bool setMode(MODE_STATE_T mode);
	bool setCurrentMode(APP_ENTRY_POINT_T entry);

	bool isVideoModeCurrent();
	bool isAudioModeCurrent();

	bool compareDeviceToMode(OSD_DEVICE device);

	DISPLAY_T outputEvent(unsigned int event, APP_ENTRY_POINT_T entry);

	void setEntry(bool isFront, APP_ENTRY_POINT_T entry);
private:
	AppMediaPlayer_EngineMain * m_engine;
	OSDMode * m_currentMode;
	OSDEntry * m_currentEntry;

	MODE_STATE_T mapDeviceToMode(OSD_DEVICE device);

	APP_ENTRY_POINT_T m_frontEntry;
	APP_ENTRY_POINT_T m_rearEntry;

signals:
	void auxModeEntered();
};

#endif
