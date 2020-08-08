#ifndef	ABSTRACT_VERSION_H_
#define	ABSTRACT_VERSION_H_

#include <QObject>
#include <QStringList>
#include <QDir>
#include "updateelement.h"

class AbstractVersion : public QObject{
Q_OBJECT
public:
	AbstractVersion(const QString & aMountPath, QObject * obj = 0) : QObject(obj) {
		m_mountPath = aMountPath;
	}
	const QList<UpdateElement *> getUpdateList() { 
		filterUpdateList();
		return m_targetList; 
	}

	virtual void filterUpdateList() = 0;

	void appendTarget(UpdateElement * ue) { m_targetList.append(ue); }
	void removeTarget(UpdateElement * ue) { 
		int index = m_targetList.indexOf(ue);
		if ( index != -1 ){
			delete m_targetList.takeAt(index);
		}
	}

	UpdateElement * findUpdate(const QString & name){
		for (int i = 0; i < m_targetList.count(); ++i){
			if ( m_targetList.at(i)->name() == name)
				return m_targetList[i];
		}
		return NULL;
	}

	const QString &			mountPath() { return m_mountPath; }
	void setMountPath(const QString & path) { m_mountPath = path; }

protected:
	QList<UpdateElement *> & targetList() { return m_targetList; }

	void createFolder(const QString & path, bool isFile = false){
		QStringList depthList = path.split("/");
		if ( isFile ) depthList.removeLast();

		QString currentFolder = "/";
		for (int i = 0; i < depthList.count(); ++i){
			QDir dir(currentFolder + depthList.at(i));
			if ( !dir.exists()){
				dir.setPath(currentFolder);
				dir.mkdir(currentFolder + depthList.at(i));
			}
			currentFolder.append(depthList.at(i) + "/");
		}
	}


private:
	QList<UpdateElement *> m_targetList;
	QString				   m_mountPath;		
};

#endif
