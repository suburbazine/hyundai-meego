#include "osdview.h"
#include "common.h"
#include <QStringList>
#include <QPainter>
#include <QStackedWidget>

#include "osddata.h"

#include "osd_null.h"
#include "osd_common.h"
#include "osd_radio.h"
#include "osd_dab.h"
#include "osd_ta.h"
#include "osd_dmb.h"
#include "osd_media_info.h"
#include "osd_media.h"
#include "osd_seatcontrol.h"
#include "osd_driving.h"
#include "osd_bluetooth.h"
#include "osd_climate.h"
#include "osd_tag.h"
#include "osd_sxm_info.h"
#include "osd_aha.h"
#include "osd_volume.h"
#include "osd_ibox.h"

#include "dhavnlog.h"

#define DEFAULT_OSD_ICON_FONT_SIZE	34

OSDView::OSDView(int variant, QWidget * p)
: QWidget(p){
	setWindowFlags(Qt::X11BypassWindowManagerHint);
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setFocusPolicy(Qt::NoFocus);

	QPalette pal = palette();
	pal.setColor(QPalette::WindowText, OSDView::BrightGrey());
	setPalette(pal);
	setAutoFillBackground(true);

	QFont f = font();
	f.setPointSize(DEFAULT_OSD_ICON_FONT_SIZE);
	setFont(f);

	m_isShow = false;
	rearScreen = false;

	separator.load(":images/osd/divide_indi.png");

	stack = new QStackedWidget(this);

	stack->setGeometry(0, 0, 1280, 93);


	osdList.append(new OSDCommon(&separator, variant));
	osdList.append(new OSDRadio(&separator, variant));
	osdList.append(new OSDDab(&separator, variant));
	osdList.append(new OSDTa(&separator, variant));
	osdList.append(new OSDDmb(&separator, variant));
	osdList.append(new OSDMediaInfo(&separator, variant));
	osdList.append(new OSDMedia(&separator, variant));
	osdList.append(new OSDSeatControl(&separator, variant));
	osdList.append(new OSDDriving(&separator, variant));
	osdList.append(new OSDBluetooth(&separator, variant));
	osdList.append(new OSDClimate(&separator, variant));
	osdList.append(new OSDTagging(&separator, variant));
	osdList.append(new OSDSxmInfo(&separator, variant));
	osdList.append(new OSDAha(&separator, variant));
	osdList.append(new OSDVolume(&separator, variant));

	osdList.append(new OSDIBox(&separator, variant));

	for (int i = 0; i < osdList.count(); ++i)
		stack->addWidget(osdList.at(i));

	stack->show();
}

void OSDView::setLanguage(int language){
	QFont f = font();
//	if ( language == (int)LANG_zh_CN || language == (int)LANG_zh_HK){
//		lprintf("Apply Chinese Font");
//		f.setFamily("CHINESE_HDB");
//	}
//	else{
//		lprintf("Apply Common Font");
//		f.setFamily("REGULAR_HDB");
//	}
        f.setFamily("DH_HDB");
	setFont(f);

	for (int i = 0; i < osdList.count(); ++i){
		osdList[i]->setLanguage(language);
		if ( osdList[i]->isVisible()){
			osdList[i]->processLastMessage();
		}
	}
}

void OSDView::setRearScreen(){
	rearScreen = true;
}

bool OSDView::isRearScreen(){
	return rearScreen;
}

void OSDView::logOSD(const QString & cmd) {

    if (cmd.contains("@OSD:FR:UPDATE^MEDIA^INFO^JUKEBOX")) {
        if(m_isShow) {
            lprintf("Command : %s", qPrintable(cmd));
        }
    }
    else {
        lprintf("Command : %s", qPrintable(cmd));
    }
}
void OSDView::displayOSD(OSDData * data){
	AbstractOSDDraw * currentOSD = NULL;	
	for (int i = 0; i < osdList.count(); ++i)
	{
		AbstractOSDDraw * osd = osdList.at(i);	
		for (int j = 0; j < osd->getCommand().count(); ++j){
			if ( data->getCommand().at(0) == osd->getCommand().at(j)){
				currentOSD = osd;

				if (currentOSD->process(data->getCommand())){
					stack->setCurrentWidget(currentOSD);
					startOSD();
				}
				return;
			}
		}
	}
}

void OSDView::startOSD(){
	if ( !isVisible()){
//		lprintf("Display OSD");
		if ( rearScreen )
			setGeometry(0, 720, 1280, 93);
		else
			setGeometry(0, 0, 1280, 93);

		show();
	}
	m_isShow = true;
}

void OSDView::hideOSD(){
	lprintf("hide OSD");
	if ( isVisible()){
		hide();
	}
	m_isShow = false;
}
