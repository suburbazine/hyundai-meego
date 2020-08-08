#include "messageprocess.h"
#include "dhavnwm.h"
#include "dhavnlog.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysymdef.h>


Window rootWindow;
static Display * display;


DHAVNWm::DHAVNWm(QObject * obj)
: QThread(obj) {
	m_frontOSD = 0;
	m_rearOSD = 0;

	m_agreed = false;
	m_isFirst = false;


	QObject::connect(this, SIGNAL(checkAgree(const QString &)), this, SLOT(checkAgreeApp(const QString &)));
}

void DHAVNWm::setOSD(Window aFrontOSD, Window aRearOSD){
	m_frontOSD = aFrontOSD;
	m_rearOSD = aRearOSD;	
}

void DHAVNWm::registApp(const QString & appName){
	m_mutex.lock();
	lprintf("REGIST APP : %s", qPrintable(appName));
	m_appList.append(appName);	
	m_mutex.unlock();
}

bool DHAVNWm::isRegisteredApp(QString str){
	m_mutex.lock();
	if ( m_appList.contains(str, Qt::CaseInsensitive)){
		m_mutex.unlock();
		return true;
	}

	m_mutex.unlock();
	return false;
}

void DHAVNWm::checkAgreeApp(const QString & app){
	if ( !m_agreed && !isAgreeGroup(app)){
		lprintf("DHAVN_WM AGREED");
		m_agreed = true;
		emit agreed();
	}
}

void DHAVNWm::run(){
	XEvent xev;
	display = XOpenDisplay(":0.0");

	rootWindow = XRootWindow(display, 0);

	XSelectInput(display, rootWindow, SubstructureNotifyMask);

	XSync(display, false);

	while(1){
		XNextEvent(display, &xev);
		switch(xev.type){

		case MapNotify:
			addNewClient(xev.xmap.window);
			XRaiseWindow(display, m_frontOSD);
			XRaiseWindow(display, m_rearOSD);
		break;

		case ConfigureNotify:
		{
			//lprintf("ConfigureNotify");
			if ( xev.xconfigure.window != m_frontOSD && xev.xconfigure.window != m_rearOSD){
				XRaiseWindow(display, m_frontOSD);
				XRaiseWindow(display, m_rearOSD);
			}
		}
		break;

		default: break;
		}
	}

	XCloseDisplay(display);		
}

bool DHAVNWm::isAgreeGroup(const QString & app){
	if ( app == "APPCLOCK" ||
		 app == "APPSTANDBY" ||
		 app == "APPINTERACTIVE" ||
		 app == "APPSETTING" ||
		 app == "APPCAMERA")
		return true;

	return false;
}

void DHAVNWm::addNewClient(Window w){
	XWindowAttributes attr;
	XClassHint ch;
	QString clname;

	if ( XGetClassHint(display, w, &ch)){
		clname = ch.res_class;
		XFree(ch.res_name);
		XFree(ch.res_class);
	}


	int attr_result = XGetWindowAttributes(display, w, &attr);
	lprintf("MapNotify(%d) : %s(%d, %d, %d, %d)", attr_result, qPrintable(clname), attr.x, attr.y, attr.width, attr.height);

	if ( attr_result && attr_result != BadDrawable && attr_result != BadWindow){
		if ( attr.width == 1280 && attr.height == 720){
			if ( !m_agreed )
				emit checkAgree(clname.toUpper());
/*
			else if ( clname.toUpper() == "APPSTANDBY"){
				lprintf("Back to Agree");
				m_agreed = false;	
				emit backToAgree();	
			}
*/
		}
	}

	if ( !m_isFirst ){
		m_isFirst = true;
		emit firstActivated();
	}	

	if ( attr_result && attr_result != BadDrawable && attr_result != BadWindow){
		ClientInfo info;
		info.x = attr.x;
		info.y = attr.y;
		info.width = attr.width;
		info.height = attr.height;
		info.name = clname;

		if ( isFrontWindow(&info)){
			lprintf("Current Front : %s", qPrintable(info.name));
			emit screenChanged(true, QString(info.name));
		}		

		if ( isRearWindow(&info)){
			lprintf("Current Rear : %s", qPrintable(info.name));
			emit screenChanged(false, QString(info.name));
		}
	}
}

bool DHAVNWm::isFrontWindow(ClientInfo * info){
	static int frontGeo[4] = {0, 93, 1280, 720 - 93};
	if ( frontGeo[0] >= info->x && frontGeo[1] >= info->y &&
			frontGeo[2] <= info->x + info->width &&
			frontGeo[3] + frontGeo[1] <= info->y + info->height)
		return true;

	return false;
}

bool DHAVNWm::isRearWindow(ClientInfo * info){
	static int rearGeo[4] = {0, 720 + 93, 1280, 720 - 93};
	if ( rearGeo[0] >= info->x && rearGeo[1] >= info->y &&
			rearGeo[2] <= info->x + info->width &&
			rearGeo[3] + rearGeo[1] <= info->y + info->height)
		return true;

	return false;
}
