#ifndef	STDIN_PROCESS_H_
#define	STDIN_PROCESS_H_

#include <QObject>
#include <QThread>
#include <stdio.h>
#include <sys/wait.h>
#include "common.h"

class ReadStdin : public QThread{
Q_OBJECT
public:
	ReadStdin(const QString & cmd, QObject * obj ) : QThread(obj){
		m_cmd = cmd;
	}

	void run(){
		char buf[255];
		FILE * fpin = NULL;
		ULOG->log("Run ReadStdin : %s\n", qPrintable(m_cmd));
		if ( (fpin = popen(qPrintable(m_cmd + QString(" 2>&1")), "r")) != NULL){
			while(1){
				if ( fgets(buf, 255, fpin) == NULL)
					break;

				emit cmdLineIncome(QString(buf).trimmed());
			}
		}
		if ( fpin )
			pclose(fpin);

	}

private:
	QString m_cmd;

signals:
	void cmdLineIncome(const QString &);
};


class StdinProcess : public QObject{
Q_OBJECT
public:
	enum RunMode{
		SingleInstance,
		NewInstance
	};
	static StdinProcess * CreateInstance(){
		if ( instance == NULL)
			instance = new StdinProcess(StdinProcess::SingleInstance);
		disconnect(instance, SIGNAL(output(const QString &)), 0, 0);
		return instance;
	}

	static StdinProcess * CreateNewInstance(){
		return new StdinProcess(StdinProcess::NewInstance);
	}

private:
	StdinProcess(RunMode);
	~StdinProcess();
	RunMode	mode;
	ReadStdin * m_rs;

private:
	static StdinProcess * instance;

public:
	bool execute(const QString & m_cmd);

signals:
	void output(const QString &);
	void doFinished();
	void processFinished(const QString &);
};


#endif
