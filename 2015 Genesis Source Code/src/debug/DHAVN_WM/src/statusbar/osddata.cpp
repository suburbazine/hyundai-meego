#include "osddata.h"

#include "osdmodel.h"

OSDData::OSDData(OSDDataType type, const QStringList & cmds, QObject * obj)
: QObject(obj){
	initData(0, type, cmds);
}

OSDData::OSDData(uchar id, OSDDataType type, const QStringList & cmds, QObject * obj){
	initData(id, type, cmds);
}

void OSDData::initData(uchar id, OSDDataType type, const QStringList & cmds){
	m_id = id;
	m_data = cmds;
	m_dataType = type;
	m_timer = new QTimer(this);
	m_timer->setSingleShot(true);

	connect(m_timer, SIGNAL(timeout()), SIGNAL(timeout()));
}

OSDData::~OSDData(){
	if ( m_timer->isActive())
		m_timer->stop();
}

void OSDData::startOSD(){
	if ( m_dataType  == OSDData::Normal )
		m_timer->start(OSD_TIMEOUT);
}

void OSDData::changeData(const QStringList & list){
	m_data = list;
}

void OSDData::changeType(OSDDataType type){
	if ( m_dataType == OSDData::Stay && type == OSDData::StayTimeout){
		m_dataType = OSDData::Normal;
		m_timer->start(OSD_TIMEOUT);
	}
}
