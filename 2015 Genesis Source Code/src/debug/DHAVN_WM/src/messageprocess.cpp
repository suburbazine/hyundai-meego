#include "messageprocess.h"
#include "intel_emgd.h"
#include "common.h"
#include "eventlistener.h"
#include "dhavnlog.h"

#include <QStringList>
#include <QTimer>
#include <QFile>

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <X11/X.h>

#ifndef	UISH_SOCK_SERVER_PATH
#define	UISH_SOCK_SERVER_PATH "/tmp/uish_srv"
#endif

extern int variant;

extern void apply_language_info(int variant, int lang);

MessageProcess::MessageProcess(QObject * obj)
: QObject(obj){
	isLCDOff = isPowerOff = false;
	requestLCDStatus = false;
	m_ignoreMode = false;

	if ( QFile::exists("/etc/ignore_osd_mode"))
		m_ignoreMode = true;
	
	QTimer::singleShot(0, this, SLOT(runListener()));
}

void MessageProcess::sendFrontHomeClick(){
	emit beep();
	sendToMain("@MAIN:F^HOME");
}

void MessageProcess::sendToMain(const QString & str){
    int result,ret,tmp, getsocRet = 0;
    struct sockaddr_un  servaddr;
    socklen_t optlen = sizeof(int);

	lprintf("MessageProcess:sendToMain : %s", qPrintable(str));

    int fd = ::socket(AF_LOCAL, SOCK_STREAM, 0);
    ::bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_LOCAL;
    ::strncpy(servaddr.sun_path, UISH_SOCK_SERVER_PATH, sizeof(servaddr.sun_path) - 1);

    /*__fd      : 소켓지정번호
      __level   : 소켓의 레벨로 어떤 레벨의 소켓정보를 가져오거나 변경할 것인지를 명시한다.
                  보통 SOL_SOCKET와 IPPROTO_TCP 중 하나를 사용한다.
      __optname : 설정을 위한 소켓옵션의 번호
      __optval  : 설정값을 저장하기 위한 버퍼의 포인터
      __optlen  : optval 버퍼의 크기*/

    if(fd != -1) {
        getsocRet = setsockopt(fd,SOL_SOCKET, SO_SNDBUF,(struct sockaddr *)&servaddr, sizeof(int));
        ret = getsockopt(fd,SOL_SOCKET, SO_SNDBUF,(int *)&tmp, &optlen);

        lprintf("MessageProcess:sendToMain ret : %d", ret);

        if(getsocRet == -1) {
            lprintf("MessageProcess:sendToMain ret error : %d", getsocRet);
            ::close(fd);
            return;
        }

        if(ret == -1) {
            ::close(fd);
            sendToMain("@MAIN:F^HOME");
        }
        else
        {
                    result = ::connect(fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
                    QByteArray array( (const char *) (str.utf16()), str.size() * 2);

                    if ( result == 0){
                            ::write(fd, array.data(), array.count());
                            ::fsync(fd);
                        ::close(fd);
                    }
                    else {
                        lprintf("MessageProcess:sendToMain connect error : %d", result);
                        ::close(fd);
                        return;
                    }
        }
    }
}

void MessageProcess::statusBarHide(){
	if ( requestLCDStatus ){
		requestLCDStatus = false;

		if ( !isLCDOff)
			sendToMain("@MAIN:F^LCD^OFF");
	}
}

void MessageProcess::process(const QString & cmd){
	QString commands;
	QString prevCommand;

	if ( cmd.count(WM_STH) > 1){
		QStringList commandList = cmd.split(WM_STH);
		commands = prevCommand = commandList.at(1);
		for (int i = 2; i < commandList.count(); ++i){
			if ( prevCommand == commandList.at(i))
				continue;

			prevCommand = commandList.at(i);
			commandQueue.append(QString(WM_STH) + commandList.at(i));
			lprintf("Queue : %s", qPrintable(QString(WM_STH) + commandList.at(i)));
		}
	}
	else{
		commands = cmd;
	}

	if ( commands.startsWith(WM_STH))
		commands.remove(0, 1);

//         lprintf("MessageProcess : %s", qPrintable(QString(commands)));

        if (QString(commands).contains("^VOLUME^Audio^") && (m_messageFilter.getModeString(m_messageFilter.m_currentMode) == "MODE_AVOFF")) {

            return;
        }


	QStringList list;
	QString conv = commands.replace("\\^", "\\wm_caret");
	conv.replace("\\wm_atsign", "@");
	QStringList originList = conv.split(QChar(WM_US));

	while( !originList.isEmpty()){
		QString str = originList.takeLast();

		if ( str.isNull() || str.isEmpty()){
			list.prepend("");
		}
		else{
			str.replace("\\wm_caret", "^");
			list.prepend(str);
		}
	}

	if ( list.count()){
		int screenVal = 0;
		QString command = list.at(0);
		list.removeFirst();
		QStringList scr = command.split(QChar(WM_SS));

		if ( scr.count() >= 2){
			if ( scr.at(1).contains(WM_FRONT))
				screenVal |= WM_FRONT_VAL;
			if ( scr.at(1).contains(WM_REAR))
				screenVal |= WM_REAR_VAL;	

			command = scr.at(0);
		}

		if ( command == EVT_CMD){
			if ( list.at(0) == EVT_TIME_CHANGE){
                                lastTime = QString("@") + commands;
                                emit broadcastCommand(lastTime);
			}
			else{
				process_evtCommand(screenVal, list);
			}
		}
		else if ( command == OPERATION_CMD){
			emit operationCommand(screenVal, list);		// deprecated : do nothing
		}
		else if ( command == OSD_CMD){
			if ( isLCDOff && !scr.contains("CLEAR_AV") && !scr.contains("STAY_TIMEOUT") && !scr.contains("CLEAR") && !scr.contains("UPDATE") && ( list.contains("SEAT_CONTROL") || list.contains("CLIMATE"))){
				requestLCDStatus = true;
				sendToMain("@MAIN:F^LCD^ON");
			}

			QString osdOption;
			if ( scr.count() == 3)
				osdOption = scr.at(2);
			
			if ( m_ignoreMode || ( scr.contains("CLEAR_AV") || scr.contains("STAY_TIMEOUT") || scr.contains("CLEAR") || m_messageFilter.canPass(list)))
				emit osdCommand(screenVal, osdOption, list, 0);
		}
		else if ( command == STATUS_CMD){
			emit broadcastCommand(QString("@") + commands);
			statusCommand(list);
		}
	}

	while ( commandQueue.count()){
		QString str = commandQueue.takeFirst();
                //lprintf("Take Next Queue : %s", qPrintable(QString(str.toUtf8())));
		process(str);
	}
}

void MessageProcess::statusCommand(const QStringList & list){
	int index = 0;

	while (1){
		if ( g_status_table[index].command == NULL)
			break;

		if ( list.at(0) == g_status_table[index].command){
			if ( list.count() == g_status_table[index].argc){
				if ( list.at(1) == "0" )
					g_status_table[index].show = false;
				else if ( list.at(1) == "1" )
					g_status_table[index].show = true;
		
				if ( list.count() == 3){
					g_status_table[index].value = list.at(2).toInt();
				}
			}
		}

		index++;
	}
}

void MessageProcess::process_evtCommand(int screenVal, const QStringList & list){
	if ( list.at(0) == EVT_LANG){
		int lang = list.at(1).toInt();
		lprintf("Changed Lang : %d", lang);
		apply_language_info(variant, lang);
		emit languageChanged(lang);
	}
	else if ( list.at(0) == EVT_SWAP){
		if ( list.at(1) == "ON"){
                    lprintf("Swap ON");
                    emit screenSwap(true);
		}
		else if ( list.at(1) == "OFF"){
                    lprintf("Swap OFF");
                    emit screenSwap(false);
		}
	}
        else if (list.at(0) == EVT_CLONE) {

            int value = 0;  // OFF
            if (list.at(1) == "FRONT") {
                value = 1;

            }
            else if (list.at(1) == "REAR") {
                value = 2;
            }

            lprintf("process_evtCommand Clone:%d", value);

            emit screenClone(value);
        }
	else if ( list.at(0) == EVT_MODE){
                if(list.at(1) == "EMERGENCY") {
                    m_messageFilter.setCurrentMode(56);
                } else {
                    m_messageFilter.setCurrentMode(list.at(1).toInt());
                }
	}
	else if ( list.at(0) == EVT_LOCK){
		if ( screenVal == 0){
			emit osdLock(true, false);
			emit osdLock(false, false);
		}
		else{
			if ( screenVal & WM_FRONT_VAL){
				emit osdLock(true, false);
			}
			
			if ( screenVal & WM_REAR_VAL){
				emit osdLock(false, false);
			}	
		}
	}
	else if ( list.at(0) == EVT_UNLOCK){
		if ( screenVal == 0){
			emit osdLock(true, true);
			emit osdLock(false, true);
		}
		else {
			if ( screenVal & WM_FRONT_VAL){
				emit osdLock(true, true);
			}
			if ( screenVal & WM_REAR_VAL){
				emit osdLock(false, true);
			}
		}
	}
	else if ( list.at(0) == EVT_LCD){
		if ( screenVal & WM_FRONT_VAL && screenVal & WM_REAR_VAL){
			// Power Off
			if ( list.at(1) == "ON"){
				lprintf("Changed Power ON Status");
				isPowerOff = false;
			}
			else if ( list.at(1) == "OFF"){
				lprintf("Changed Power OFF Status");
				// add OSD CLEAR
                                emit clearOSDAll();
				isPowerOff = true;
			}
		}
		else if ( screenVal & WM_FRONT_VAL){
			// LCD Off
			if ( list.at(1) == "ON"){
				lprintf("Changed LCD ON Status");
				isLCDOff = false;
			}
			else if ( list.at(1) == "OFF"){
				lprintf("Changed LCD OFF Status");
				isLCDOff = true;
				emit clearFrontOSD();
			}
		}
		// Ignore Rear LCD Off
	}
	else if ( list.at(0) == EVT_DISP){
		if ( screenVal & WM_FRONT_VAL && ( (screenVal & WM_REAR_VAL) == 0)){
			if ( list.at(1) == "ON"){
				lprintf("Changed DISP ON Status");
				requestLCDStatus = false;	
				isLCDOff = false;
			}
			else if ( list.at(1) == "OFF"){
				lprintf("Changed DISP OFF Status");
				requestLCDStatus = false;
				isLCDOff = true;
			}
		}
	}
	else if ( list.at(0) == EVT_REGIST){
		if ( list.count() >= 2){
			emit applicationRegistered(list.at(1).toUpper());
			if ( !lastTime.isEmpty())
				broadcastCommand(lastTime);
		}
	}
	else if ( list.at(0) == EVT_HOME){
		sendFrontHomeClick();
	}
        else if (list.at(0) == EVT_CLEAR_ALL) {
            lprintf("process_evtCommand CLEAR ALL");
            emit clearOSDAll();
        }
}

void MessageProcess::runListener(){
	EventListener * listener = new EventListener(this);
	connect(listener, SIGNAL(eventTriggered(const QString &)), SLOT(process(const QString &)));
	connect(this, SIGNAL(beep()), listener, SLOT(beep()));
}

