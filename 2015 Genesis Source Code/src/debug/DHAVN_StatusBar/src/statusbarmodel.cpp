#include "statusbarmodel.h"
#include "statustable.h"
#include "clock.h"
#include "log_console.h"

StatusBarModel * StatusBarModel::statusBarModel = NULL;

StatusBarModel::StatusBarModel(QObject * obj)
: QObject(obj){
	m_taEnabled = m_newsEnabled = m_regEnabled = false;
	m_clock = new Clock(this);

        m_powerStatusListener = PowerStatusListener::instance();

        connect(m_clock, SIGNAL(timeChanged(int, int, const QString &)), SIGNAL(timeChanged(int, int, const QString &)));
        connect(m_powerStatusListener, SIGNAL(powerStatusChanged()), SIGNAL(powerStatusChanged()));
}

void StatusBarModel::eventChanged(const QStringList & cmds){
	if ( cmds.at(0) == "TIME_CHANGE"){
		int hour = cmds.at(1).toInt();
		int minute = cmds.at(2).toInt();
		
		if ( cmds.count() >= 4){
			m_clock->setTime(hour, minute, cmds.at(3));	
		}
		else{
			m_clock->setTime(hour, minute, "");
		}
	}
}	

void StatusBarModel::statusChanged(const QStringList & cmds){
#ifdef	CONSOLE_DEBUG
//         lprintf("[StatusBar] StatusBarModel statusChanged : %s", qPrintable(cmds));
//    if (cmds.at(0) != NULL && cmds.at(1) != NULL) {
//        lprintf("[StatusBar] StatusBarModel statusChanged : %s %s", qPrintable(cmds.at(0)), qPrintable(cmds.at(1)));
//    }
#endif
	int index = 0;
        if (cmds.count() > 1) {
            while(1){
                    if ( g_status_table[index].command == NULL)
                            break;

                    if ( cmds.at(0) == g_status_table[index].command){
                            if ( cmds.count() == g_status_table[index].argc){
                                    bool changed = false;
                                    if ( cmds.at(1) == "0" && g_status_table[index].show){
                                            g_status_table[index].show = false;
                                            changed = true;
                                    }
                                    else if ( cmds.at(1) == "1" && !g_status_table[index].show){
                                            g_status_table[index].show = true;
                                            changed = true;
                                    }

                                    if ( cmds.count() == 3 && g_status_table[index].value != ( cmds.at(2).toInt()) ){
                                            g_status_table[index].value = cmds.at(2).toInt();
                                            changed = true;
                                    }

                                    if ( changed ){
#ifdef	CONSOLE_DEBUG
                                            lprintf("[StatusBar] Update Icon");
#endif
                                            updateIconView();
                                    }
                            }

                            break;
                    }

                    index++;
            }

            if ( cmds.at(0) == "TA"){
                    bool isEnabled = (cmds.at(1) == "1" ? true:false);
                    if ( isEnabled ^ isTA()){
                            m_taEnabled = isEnabled;
                            updateEuropeInfo();
                    }
            }
            else if ( cmds.at(0) == "NEWS"){
                    bool isEnabled = (cmds.at(1) == "1" ? true:false);
                    if ( isEnabled ^ isNEWS()){
                            m_newsEnabled = isEnabled;
                            updateEuropeInfo();
                    }
            }
            else if ( cmds.at(0) == "REG"){
                    bool isEnabled = (cmds.at(1) == "1" ? true:false);

                    if ( isEnabled ^ isREG()){
                            m_regEnabled = isEnabled;
                            updateEuropeInfo();
                    }
            }
        }
}

bool StatusBarModel::isPowerOn()
{
    return m_powerStatusListener->getPowerOn();
}
