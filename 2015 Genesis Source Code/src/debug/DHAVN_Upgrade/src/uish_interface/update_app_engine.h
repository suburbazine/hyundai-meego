#ifndef	APP_UPDATE_ENGINE_H_
#define	APP_UPDATE_ENGINE_H_

#include <DHAVN_AppFramework_AppEngineQML_Def.h>

class UpdateAppEngine : public AppEngineQML{
Q_OBJECT

public:
	UpdateAppEngine(QDeclarativeView &aQMLView, QObject * parent = NULL);
	~UpdateAppEngine();

public slots:
	virtual void NotifyFromUIStateManager(Event aEvent);
	virtual void NotifyFromUIStateManagerRear(Event aEvent);
	virtual void NotifyFromUIStateManagerCommon(Event aEvent);

private:
	virtual APP_ID_T	GetThisAppId() const { return APP_UPGRADE; }
	QDeclarativeView * view;

private slots:
	void exitApplication();

signals:
	void jogDialEvent(int);
};

#endif
