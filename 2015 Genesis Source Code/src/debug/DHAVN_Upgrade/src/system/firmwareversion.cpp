#include "firmwareversion.h"

#include "common.h"
#include "inicversion.h"
#include "firmware.h"
#include "cancontroller.h"
#include "updateenvironment.h"
#include <QEventLoop>
#include <QDebug>
#include <QFile>
extern int LCD_major;
extern int LCD_minor;

FirmwareVersion::FirmwareVersion(const QString & aMountPath, QObject * obj)
: AbstractVersion(aMountPath, obj){
	ULOG->log("Firmware Constructor");
	m_isUser = true;
	m_getLCDVersion = false;
	canController = new CanController(this);
	connect(canController, SIGNAL(canDataArrived(int, const QByteArray &)), this, SLOT(parseCanVersion(int, const QByteArray &)));
	//connect(canController, SIGNAL(finished()), canController, SLOT(deleteLater()));
	canController->start();
	QTimer::singleShot(3000, canController, SLOT(setStopFlag()));
        ::sleep(4);
}

FirmwareVersion::~FirmwareVersion(){ 
	delete canController;
}

void FirmwareVersion::filterUpdateList(){
	int versionRequest = 3;
	/*QEventLoop loop;
	// for can controller
	QTimer::singleShot(3000, &loop, SLOT(quit()));

	loop.exec();*/
	::sleep(3);

	variantFilter();
	::sleep(1);

	if(LCD_major != 0){
		m_getLCDVersion = true;
	}

	while(!m_getLCDVersion){
		if(canController->isRunning()){
			canController->setStopFlagByExternal();
			::sleep(1);
		}
	
		canController->start();
		QTimer::singleShot(3000, canController, SLOT(setStopFlag()));
		::sleep(3);

		if(LCD_major != 0 || versionRequest < 0)
			m_getLCDVersion = true;

		versionRequest--;
	}

	lcdtypeFilter();
	duplicateFilter();
	
	carTypeCheckForMicom();

	gpsVariantCheck();
	fileFilter();

	if ( !m_isUser)
		fileCopy();

	versionFilter();
	changePath();

	biosVariantCheck();
}

void FirmwareVersion::biosVariantCheck(){
	for (int i = 0; i < targetList().count(); ++i){
		if ( targetList().at(i)->name().toLower() == "bios"){
			if(UpdateEnvironment::instance()->getSystemVersionString() == "DH.KOR.SOP.10.003."){
				ULOG->log("Copy driver for BIOS Update");
				QFile::copy(targetList().at(i)->path() +"/amifldrv_mod.o",  "/sbin/bios/amifldrv_mod.o");
				::sync();
			}
			if ( gf_check_variant()){
				QString biosPath = targetList().at(i)->path() + "/" + g_variant_short_str[g_variantCode];
				ULOG->log("BIOS DIR PATH : %s", qPrintable(biosPath));

				QDir dir(biosPath);
				QStringList extList;
				extList << "*.rom";
				QFileInfoList list = dir.entryInfoList(extList, QDir::Files);
				if ( list.count()){
	
					targetList()[i]->setPath(list.at(0).absoluteFilePath());
					return;
				}
			}
			removeTarget(targetList()[i]);

			return;
		}
	}
}

void FirmwareVersion::carTypeCheckForMicom(){
	for(int i = 0 ; i < targetList().count() ; i++){
		if(targetList().at(i)->name().toLower() == "micom"){
			if(g_carTypeCode == 0){
				QString micomPath = targetList().at(i)->path() + "/" + "DH";
				targetList()[i]->setPath(micomPath);
				ULOG->log("micom path : %s", qPrintable(targetList().at(i)->path()));
			} else if(g_carTypeCode == 1) {
				QString micomPath = targetList().at(i)->path() + "/" + "DH15MY";
				targetList()[i]->setPath(micomPath);
				ULOG->log("micom path : %s", qPrintable(targetList().at(i)->path()));
			} else if(g_carTypeCode == 2) {
				QString micomPath = targetList().at(i)->path() + "/" + "DHPE";
				targetList()[i]->setPath(micomPath);
				ULOG->log("micom path : %s", qPrintable(targetList().at(i)->path()));
			} else if(g_carTypeCode == 17) {
				QString micomPath = targetList().at(i)->path() + "/" + "DH17MY";
				targetList()[i]->setPath(micomPath);
				ULOG->log("micom path : %s", qPrintable(targetList().at(i)->path()));
			}
		}
	}
}

void FirmwareVersion::changePath(){
	for (int i = 0; i < targetList().count(); ++i){
		QString path = targetList()[i]->path();
		path.prepend(UPGRADE_DIR);
		targetList()[i]->setPath(path);
	}
}

void FirmwareVersion::lcdtypeFilter(){
           int front_lcd_ver_major      =  LCD_major; //m_canVersion[CanController::e_FRONT_LCD].at(0);
           int front_lcd_ver_minor      =  LCD_minor;//m_canVersion[CanController::e_FRONT_LCD].at(1);
     //      int rear_left_lcd_ver  = m_canVersion[CanController::e_FRONT_LCD].at(1);
    //       int rear_right_lcd_ver = m_canVersion[CanController::e_FRONT_LCD].at(1);
ULOG->log("lcdtypeFilter front_lcd_ver_major[%d], front_lcd_ver_minor[%d]", front_lcd_ver_major,front_lcd_ver_minor/*, rear_left_lcd_ver, rear_right_lcd_ver*/);

            for (int i = 0; i < targetList().count(); ++i){
                        UpdateElement * ue = targetList()[i];
                        QString lname = ue->lname();
                        QString lcdname = ue->name();
                            ULOG->log("lcdtypeFilter forin device lcdname[%s], lname[%s]", qPrintable(lcdname),qPrintable(lname));
                        if ((front_lcd_ver_major==38&&front_lcd_ver_minor==19)
                            ||(front_lcd_ver_major==39&&front_lcd_ver_minor==21)
                            ||(front_lcd_ver_major==40&&front_lcd_ver_minor==21)
                            ||(front_lcd_ver_major==41&&front_lcd_ver_minor==21))
                        {
                            front_lcd_ver_major = 64;
                        }

                        if ( ue->lname().isEmpty() || ue->lname().isNull())
                                continue;
                        else if ((lcdname.toLower()=="front_lcd")&&(front_lcd_ver_major<64)){
                                if (lname.toLower()=="truly"){
                                    ULOG->log("lcdtypeFilter truly del device name[%s]", qPrintable(lcdname));
                                    removeTarget(ue);
                                    --i;
                                }
                        }
                        else if ((lcdname.toLower()=="front_lcd")&&(front_lcd_ver_major>63)){
                                if (lname.toLower()=="trais"){
                                    ULOG->log("lcdtypeFilter trais del device name[%s]", qPrintable(lcdname));
                                    removeTarget(ue);
                                    --i;
                                }
                        }
                }
        
}
void FirmwareVersion::gpsVariantCheck(){
        for (int i = 0; i < targetList().count(); ++i){
                if ( targetList().at(i)->name().toLower() == "gps" || targetList().at(i)->name().toLower() == "gnss" ){
                        if ( gf_check_variant() ){
                                QString gpsPath = targetList().at(i)->path() + "/" + g_variant_short_str[g_variantCode];
                                ULOG->log("gps DIR PATH : %s", qPrintable(gpsPath));

								if ( gpsPath.isEmpty() && gpsPath.isNull()){
                                        continue;
                                }
                                else{
                                        targetList()[i]->setPath(gpsPath);
                                }
                        } else {
							removeTarget(targetList()[i]);
							return;
						}
		}
	}
}

void FirmwareVersion::versionFilter(){
	// inic
	checkInic();
	checkCanDevice(CanController::e_CCP, "ccp");
	checkCanDevice(CanController::e_RRC, "rrc");
	checkCanDevice(CanController::e_FRONT_LCD, "front_lcd");
	checkCanDevice(CanController::e_RL_LCD, "rear_left_lcd");
	checkCanDevice(CanController::e_RR_LCD, "rear_right_lcd");

	for (int i = 0; i < targetList().count(); ++i)
		ULOG->log("Update Target Firmware : %s", qPrintable(targetList().at(i)->name()));
	
}

void FirmwareVersion::checkCanDevice(int type, const QString & device){
	int index = existFirmware(device);
	if ( index >= 0){
		UpdateElement * ue = findUpdate(device);
		if ( !m_canVersion.contains(type)){
			removeTarget(ue);	
		}
		else{
			Firmware firmware;
			firmware.setBase(10);
			firmware.setFirmwareVersion(m_canVersion[type]);
			firmware.setFileVersion(ue->version());
			ULOG->log("compare %s", qPrintable(device));
			if ( firmware.compare() <= 0)
				removeTarget(ue);
		}
	}
}

int FirmwareVersion::existFirmware(const QString & device){
	for (int i = 0; i < targetList().count(); ++i){
		if ( targetList().at(i)->name().toLower() == device)
			return i;	
	}
	return -1;
}

void FirmwareVersion::setFirmwareUpdateList(const QList<UpdateElement *> & list, bool isUser){
	m_isUser = isUser;
	if ( m_isUser )
		setMountPath(UPGRADE_DIR);

	for (int i = 0; i < list.count(); ++i){
		if ( list.at(i)->type() == UpdateElement::et_FIRMWARE){
			appendTarget(list.at(i));
		}
	}
}

void FirmwareVersion::duplicateFilter(){
	QStringList targetNames;
	for (int i = 0; i < targetList().count(); ++i){
		UpdateElement * ue = targetList()[i];
		if ( targetNames.contains(ue->name())){
			removeTarget(ue);
			--i;
		}
		else{
			targetNames << ue->name();
		}
	}
}

void FirmwareVersion::variantFilter(){
	for (int i = 0; i < targetList().count(); ++i){
		UpdateElement * ue = targetList()[i];
		if ( ue->variant().isEmpty() || ue->variant().isNull()){
			if ( g_variantCode == 7 && ue->name().toLower() == "gps" ){
				removeTarget(ue);
				--i;
			} else
				continue;
		} else if ( ue->variant() != g_variant_str[g_variantCode]){
			removeTarget(ue);
			--i;	
		}
	}
}

void FirmwareVersion::fileFilter(){
	for (int i = 0; i < targetList().count(); ++i){
		UpdateElement * ue = targetList()[i];
		QFile file(mountPath() + ue->path());
		if ( !file.exists()){
			removeTarget(ue);
			--i;
		}
	}
}

void FirmwareVersion::fileCopy(){
	QString destDir = "/common_data/upgrade";
	for (int i = 0; i < targetList().count(); ++i){
		UpdateElement * ue = targetList()[i];
		QFileInfo f(mountPath() + ue->path());
		if ( f.isDir() && ue->name().toLower() == "micom"){
			removeDir("/root/micom");
			createFolder("/root/micom");
			copyDir(mountPath() + ue->path(), "/root/micom");
		}

		if ( f.isDir()){
			removeDir(destDir + ue->path());
			createFolder(destDir + ue->path());
			qWarning() << "copyFiles(" << mountPath() + ue->path() << ") : to : " << destDir + ue->path();
			copyDir(mountPath() + ue->path(), destDir + ue->path());
		}
		else if (f.isFile()){
			createFolder(destDir + ue->path(), true);
			if ( QFile::exists(destDir + ue->path())) QFile::remove(destDir + ue->path());
			if ( !QFile::copy(f.absoluteFilePath(), destDir + ue->path())){
				ULOG_EXCEPTION; ULOG->log("Copy Failed : %s", qPrintable(destDir + ue->path()));
			}
			else{
				qWarning() << "Copy(" << f.absoluteFilePath() << "), to : " << destDir + ue->path();
			}
		}
	}
}

void FirmwareVersion::copyDir(const QString & from, const QString & to){
	QDir dir(from);

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

bool FirmwareVersion::removeDir(const QString & name){
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

void FirmwareVersion::checkInic(){
	UpdateElement * ue = findUpdate("inic");
	if ( ue ){
		QByteArray ar = InicVersion::GetInicVersion();

		Firmware firmware;
		firmware.setFirmwareVersion(ar);
		firmware.setFileVersion(ue->version());
		ULOG->log("Start compare INIC");

                if ( firmware.compare() == 0) {
                     ULOG->log("INIC firmware removeTarget");
                     removeTarget(ue);
                }

                /*if ( firmware.compare() != 0)
			appendTarget(ue);
		else
                        removeTarget(ue);*/
	}
}

void FirmwareVersion::parseCanVersion(int type, const QByteArray & data){
	if (m_canVersion.contains(type)){
		m_canVersion[type].clear();
		m_canVersion[type].append(data);
	}
	else{
		m_canVersion[type] = data;
	}
}
