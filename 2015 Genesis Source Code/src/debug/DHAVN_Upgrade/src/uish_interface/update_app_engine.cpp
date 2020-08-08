#include "update_app_engine.h"
#include "common.h"
#include <sys/mount.h>

UpdateAppEngine::UpdateAppEngine(QDeclarativeView &aQMLView,
									QObject * parent)
: AppEngineQML(&aQMLView, "", parent), view(&aQMLView)
{
	qInstallMsgHandler(NULL);
}

UpdateAppEngine::~UpdateAppEngine(){

}

void UpdateAppEngine::NotifyFromUIStateManagerRear(Event aEvent){
	NotifyFromUIStateManager(aEvent);
}

void UpdateAppEngine::NotifyFromUIStateManagerCommon(Event aEvent){
	NotifyFromUIStateManager(aEvent);
}

void UpdateAppEngine::NotifyFromUIStateManager(Event aEvent){
	if ( view->isVisible())
		view->hide();

      //  ULOG->log("Event ID : %d, Data Count : %d", aEvent.GetEventId(), aEvent.Data().count());
	switch ( aEvent.GetEventId()){
	case EVT_REQUEST_FG:
		ULOG->log("NotifyFromUIStateManager:EVT_REQUEST_FG");
		break;
	case EVT_REQUEST_BG:
		ULOG->log("NotifyFromUIStateManager:EVT_REQUEST_BG");
		break;
        /*case EVT_KEY_CCP_JOG_DIAL_ENCODER:
                emit jogDialEvent( (int) aEvent.GetCommand());
                break;*/
	case EVT_KEY_CCP_JOG_DIAL_CENTER:
	{
		ULOG->log("JOG DIAL CENTER : %d", aEvent.GetCommand());
		KEY_EVENT_DATA_T *message = NULL;

         if ( !aEvent.Data().isEmpty() )
         {
            message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
         }

         if ( message )
         {
			if ( (int) message->keyStatus == KEY_STATUS_PRESSED){
				ULOG->log("JOG DIAL ENTER");
				emit jogDialEvent(0);
			} else if ((int) message->keyStatus == KEY_STATUS_RELEASED){
				ULOG->log("JOG DIAL RELEASE");
				emit jogDialEvent(1);
			}
         }
	}
		break;
	default:
		break;
	}
}

void UpdateAppEngine::exitApplication(){
	ULOG->log("Engine:exitApplication");
	::seteuid(MEEGO_USER_ID);
	Event event(EVT_RESPONSE_UPGRADE_CANCEL, GetThisAppId(), APP_UI_MANAGER);
	NotifyUISHCommon(event);

	QApplication::flush();

	::seteuid(ROOT_USER_ID);
	ULOG->closeLog();	

	if ( g_isReadonly ) {
		if ( ::mount("/boot", "/boot", "ext3", MS_REMOUNT | MS_RDONLY | MS_NOATIME, NULL) < 0){
			fprintf(stderr, "Failed to remount boot\n");
		}

		if( ::mount("/", "/", "ext4", MS_REMOUNT | MS_RDONLY, NULL) < 0){
			fprintf(stderr, "Failed to remount rootfs\n");
		}
	}

	::exit(0);
}

