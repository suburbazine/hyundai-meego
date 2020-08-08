#include <QObject>
#include <QtDeclarative/qdeclarative.h>
#include <QtDeclarative/QDeclarativeExtensionPlugin>
#include <QtDeclarative/QDeclarativeListProperty>
#include <QCoreApplication>
#include <QStringList>
#include <QFileInfo>
#include <QTimer>
#include "statusbarmodel.h"
#include "messageprocess.h"
#include "statustable.h"
#include "clock.h"
#include "log_console.h"

#define ICON_COUNT			8
#define EUROPE_ICON_COUNT	3

#define UNKNOWN_VALUE	-1

class StatusIcon : public QObject{
	Q_OBJECT
	Q_PROPERTY(QString iconFile READ iconFile)
public:
	StatusIcon(QObject * parent = 0) : QObject(parent){ 
		m_timer = new QTimer(this);
		m_timer->setInterval(500);
		connect(m_timer, SIGNAL(timeout()), SLOT(updateIcon()));	

		m_index = UNKNOWN_VALUE;
		m_animationIndex = UNKNOWN_VALUE;
		m_value = -1;
		m_iconIndex = -1;
	}

	void setIconIndex(int index){
		m_iconIndex = index;
	}

	const QString & iconFile() { return m_iconFile; }

	void setIconFile(const QString & f){
		if ( f != m_iconFile){
			m_iconFile = f;
#ifdef	CONSOLE_DEBUG
                        lprintf("[StatusBar] set Icon File : %s", qPrintable(m_iconFile));
#endif
			emit iconFileChanged(m_iconIndex);
		}
	}

	void setIndex(int index){
		if ( index == UNKNOWN_VALUE){
			if ( m_timer->isActive())
				m_timer->stop();
			m_animationIndex = UNKNOWN_VALUE;
			m_index = UNKNOWN_VALUE;
		}
		else if(index > UNKNOWN_VALUE){
			if ( m_index == index && m_index > UNKNOWN_VALUE && m_value == g_status_table[m_index].value)
				return;

			m_index = index;

			m_value = g_status_table[m_index].value;

			if ( g_status_table[m_index].icon_count > 1 && g_status_table[m_index].value == -1 ){
				m_animationIndex = g_status_table[m_index].icon_start_index;
				if ( !m_timer->isActive())
					m_timer->start();
			}
			else{
				m_animationIndex = UNKNOWN_VALUE;
				if ( m_timer->isActive())
					m_timer->stop();
			}
		}
		updateIcon();
	}	

private slots:
	void updateIcon(){
		if ( m_index == UNKNOWN_VALUE ){
			setIconFile("");
			return;
		}
		
		if ( m_animationIndex != UNKNOWN_VALUE){
			setIconFile(QString(g_status_table[m_index].icon_name_prefix).append(QString::number(m_animationIndex) + ".png"));
			if ( m_animationIndex >= g_status_table[m_index].icon_count + g_status_table[m_index].icon_start_index - 1){
				m_animationIndex = g_status_table[m_index].icon_start_index;
			}
			else{
				m_animationIndex++;
			}
		}
		else{
			QString picName(g_status_table[m_index].icon_name_prefix);
			if ( g_status_table[m_index].value != -1){
				picName.append( QString::number(g_status_table[m_index].value));
			}
			setIconFile(picName + ".png");
		}	
	}

private:
	int		m_iconIndex;

	int		m_index;
	int		m_value;
	int		m_animationIndex;
	QString m_iconFile;

	QTimer * m_timer;

signals:
	void iconFileChanged(int index);
};

class StatusIconModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QDeclarativeListProperty<StatusIcon> statusIcons READ statusIcons)
	Q_PROPERTY(QDeclarativeListProperty<StatusIcon> europeIcons READ europeIcons)

public:
	StatusIconModel(QObject * parent = 0) : QObject(parent){
#ifdef	CONSOLE_DEBUG
                 lprintf("[StatusBar] StatusIconModel Consturction");
#endif
		QFileInfo info(QCoreApplication::applicationFilePath());
		MessageProcess::instance(info.fileName());
		m_model = StatusBarModel::instance();

		connect(MessageProcess::instance(), SIGNAL(statusCommandReceived(const QStringList &)), m_model, SLOT(statusChanged(const QStringList &)));
		connect(MessageProcess::instance(), SIGNAL(eventCommandReceived(const QStringList &)), m_model, SLOT(eventChanged(const QStringList &)));
		connect(MessageProcess::instance(), SIGNAL(agreed()), SIGNAL(agreed()));
		connect(MessageProcess::instance(), SIGNAL(backToAgree()), SIGNAL(backToAgree()));

		for (int i = 0; i < ICON_COUNT; ++i){
			StatusIcon * icon = new StatusIcon(this);
			icon->setIconIndex(i);
			m_statusIcons.append(icon);
			connect(icon, SIGNAL(iconFileChanged(int)), SIGNAL(iconFileChanged(int)));
		}

		for (int i = 0; i < EUROPE_ICON_COUNT; ++i){
			StatusIcon * icon = new StatusIcon(this);
			icon->setIconIndex(i);
			m_europeIcons.append(icon);
			connect(icon, SIGNAL(iconFileChanged(int)),	SIGNAL(europeIconFileChanged(int)));
		}

		connect(m_model, SIGNAL(updateIconView()), SLOT(updateIcons()));
		connect(m_model, SIGNAL(updateEuropeInfo()), SLOT(updateEuropeIcons()));
		connect(m_model, SIGNAL(timeChanged(int, int, const QString &)), SIGNAL(timeChanged(int, int, const QString &)));
//                connect(m_model, SIGNAL(powerStatusChanged()), SIGNAL(powerStatusChanged()));

		MessageProcess::instance()->registApp();

		QTimer::singleShot(0, this, SLOT(lazyUpdate()));
	}

	// consider delete
	
	QDeclarativeListProperty<StatusIcon> statusIcons(){
		return QDeclarativeListProperty<StatusIcon>(this, m_statusIcons);
	}

	QDeclarativeListProperty<StatusIcon> europeIcons(){
		return QDeclarativeListProperty<StatusIcon>(this, m_europeIcons);
	}

/*
	StatusIcon * statusIcon(int index) const{
		return m_statusIcons.at(index);
	}
*/

	Q_INVOKABLE void sendHome() { 
		MessageProcess::instance()->writeMessage("@EVT^HOME");
	}

        Q_INVOKABLE int getHour() {
            Clock* c = m_model->getClock();
            return c->hour();
        }
        Q_INVOKABLE int getMin() {
            Clock* c = m_model->getClock();
            return c->minute();

        }
        Q_INVOKABLE QString getAMPM() {
            Clock* c = m_model->getClock();
            return c->ampm();
        }

        Q_INVOKABLE bool isPowerOn() {
            return m_model->isPowerOn();
        }

private:
	QList<StatusIcon *> m_statusIcons;
	QList<StatusIcon *> m_europeIcons;
	StatusBarModel * m_model;

signals:
	void iconFileChanged(int index);
	void europeIconFileChanged(int index);
	void agreed();
	void backToAgree();
	void timeChanged(int chour, int cminute, const QString & cAMPM);
        void powerStatusChanged();

private slots:
	void lazyUpdate(){
		updateIcons();
		updateEuropeIcons();
		if ( MessageProcess::instance()->isAgreed())
			emit agreed();
	}

	void updateIcons(){
		int index = 0;
		int iconIndex = 0;
		int currentPriority = 0;
	
#ifdef	CONSOLE_DEBUG
                lprintf("[StatusBar] Model Update Icons");
#endif
		while ( g_status_table[index].command != NULL ){
			if ( g_status_table[index].show ){
#ifdef	CONSOLE_DEBUG
                                lprintf("[StatusBar] Update Icon pos(%d), index : %d", iconIndex, index);
#endif
				m_statusIcons[iconIndex++]->setIndex(index);
				currentPriority = g_status_table[index].priority;
				while(1){
					index++;
					if ( g_status_table[index].priority != currentPriority)
							break;
				}
			}
			else{	
				index++;	
			}
		}
		for (int i = iconIndex; i < ICON_COUNT; ++i){
#ifdef	CONSOLE_DEBUG
                        lprintf("[StatusBar] set(%d) to NULL", iconIndex);
#endif
			m_statusIcons[i]->setIndex(UNKNOWN_VALUE);
		}
	}

	void updateEuropeIcons(){
		int iconIndex = 0;
	
		if ( m_model->isTA())
			m_europeIcons[iconIndex++]->setIconFile("qrc:/images/indi/indi_ta.png");

		if ( m_model->isNEWS())
			m_europeIcons[iconIndex++]->setIconFile("qrc:/images/indi/indi_news.png");

		if ( m_model->isREG())
			m_europeIcons[iconIndex++]->setIconFile("qrc:/images/indi/indi_reg.png");

		for (int i = iconIndex; i < EUROPE_ICON_COUNT; ++i)
			m_europeIcons[i]->setIconFile("");
	}
};

class QmlStatusBarPlugin : public QDeclarativeExtensionPlugin
{
	Q_OBJECT

public:
	void registerTypes(const char * uri){
		Q_ASSERT(uri == QLatin1String("QmlStatusBar"));
		qmlRegisterType<StatusIconModel>(uri, 1, 0, "StatusIconModel");
		qmlRegisterType<StatusIcon>(uri, 1, 0, "StatusIcon");
	}
};

#include "main.moc"

Q_EXPORT_PLUGIN2(qmlstatusbarplugin, QmlStatusBarPlugin);
