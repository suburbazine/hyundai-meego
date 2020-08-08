#ifndef	DHAVN_WM_H_
#define	DHAVN_WM_H_

#include <QThread>
#include <QHash>
#include <QMutex>
#include <QStringList>
#include <X11/X.h>
#include <X11/Xatom.h>

typedef struct{
	Window window;
	int x;
	int y;
	int width;
	int height;
	QString name;
} ClientInfo;

class DHAVNWm : public QThread{
Q_OBJECT
public:
	DHAVNWm(QObject * obj = 0);

	void setOSD(Window frontOSD, Window rearOSD);

private:
	void addNewClient(Window w);	

	bool isFrontWindow(ClientInfo *);
	bool isRearWindow(ClientInfo *);

	Window	m_frontOSD;
	Window	m_rearOSD;

	QMutex	m_mutex;
	QStringList m_appList;

	bool isRegisteredApp(QString str);

	bool	m_agreed;
	bool	m_isFirst;

	bool isAgreeGroup(const QString & app);

protected:
	void run();

private slots:
	void registApp(const QString &);

	void checkAgreeApp(const QString &);

signals:
	void firstActivated();
	void agreed();
	void backToAgree();

	void screenChanged(bool, const QString &);

	void checkAgree(const QString &);
};

#endif
