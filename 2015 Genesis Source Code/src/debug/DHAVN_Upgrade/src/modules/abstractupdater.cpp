#include "abstractupdater.h"
#include "updatelauncher.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>

AbstractUpdater::AbstractUpdater(LauncherMode aMode, QObject * obj)
: QObject(obj){
	m_mode = aMode;
}

unsigned int AbstractUpdater::getLauncherStatus(AbstractUpdater::LauncherMode mode){
	UpdateLauncher * launcher = qobject_cast<UpdateLauncher *>(parent());
	Q_ASSERT(launcher);
	return launcher->getUpdaterStatus(mode);
}

bool AbstractUpdater::removeDir(const QString & name){
	bool result = true;
	QDir dir(name);

	if ( dir.exists(name)){
		Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst)){
			if ( info.isDir()){
				result = removeDir(info.absoluteFilePath());
			}
			else{
				result = QFile::remove(info.absoluteFilePath());
			}

			if ( !result){
				return result;
			}
		}
		result = dir.rmdir(name);
	}
	return result;
}

void AbstractUpdater::copyDir(const QString & from, const QString & to){
	QDir dir(from);

	if ( !dir.exists())
		return;

	QDir destDir(to);
	if ( !destDir.exists())
		destDir.mkdir(to);

	QStringList files = dir.entryList(QDir::Files);
	for (int i = 0; i < files.count(); ++i){
		QString srcName = from + "/" + files.at(i);
		QString destName = to + "/" + files.at(i);
		QFile::copy(srcName, destName);
	}
	files.clear();

	files = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
	for (int i = 0; i < files.count(); ++i){
		QString srcName = from + "/" + files.at(i);
		QString destName = to + "/" + files.at(i);
		copyDir(srcName, destName);
	}
}


