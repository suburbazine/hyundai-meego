#ifndef	MSC_ABSTRACT_H_
#define	MSC_ABSTRACT_H_


#include <QObject>
#include <QTimer>
#include <QDBusAbstractInterface>
#include "common.h"
#include "most.h"
#include "DHAVN_MOSTManager_MOSTStructures.h"
#include "proxy/swdlproxy.h"

typedef struct {
	uint	moduleType;
	uint	moduleIndex;
	uint	blocksize;
	int		swdlchannel;
	QString	moduleVersion;
} ModuleVersion;

class MscAbstract : public QObject{
Q_OBJECT
public:
	MscAbstract(QObject * obj, QDBusAbstractInterface *, SWDLID::SWDLID swdlid);
	virtual ~MscAbstract();
	SWDLID::SWDLID			getSwdlId();

	virtual void			start() = 0;
	void					setNextStep(MscAbstract * next);

	static QString IntegerToAck(int value){
		switch(value){
		case 1: return QString("StartResultAck");
		case 2: return QString("ProcessingAck");
		case 3: return QString("ResultAck");
		case 4: return QString("ErrorAck");
		default: break;
		}
		return QString("UnknownAck");
	}

signals:
	void statusChanged(int,int,int status = 0, const QString & msg = QString());

protected:
	local::CSWDLAdaptor *	getProxy();
	void					mscTrigger(SWDLID::SEQUENCE sequence);
	void					stopTimer();
	void					startNext();

	static void				AddModule(ModuleVersion * mv){
		s_modules.append(mv);
	}

	static QList<ModuleVersion *> & Modules(){
		return s_modules;
	}

	static void						DeleteAllModules(){
		while ( !s_modules.isEmpty()) delete s_modules.takeFirst();
	}

private:
	local::CSWDLAdaptor *	m_proxy;
	SWDLID::SWDLID			m_swdlid;
	QTimer *				m_timer;
	MscAbstract *			m_next;

	static QList<ModuleVersion *> s_modules;

signals:
	void timeout();
};

#endif
