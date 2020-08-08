#ifndef	UPDATE_H_
#define	UPDATE_H_

#include <QThread>
class QDBusConnection;
class Controller;

class Update : public QThread{
Q_OBJECT
public:
	Update(QObject * obj);
	~Update();

	void run();

private:
	QObject * m_uiThread;
	Controller * m_controller;

	bool	checkUpdate(const QString & mountPoint);

signals:
	void cannotProceedUpdate(const QString &);

	void showUSBDetachDialog();

	void exitApplication();

	void setMode(int);
};

#endif
