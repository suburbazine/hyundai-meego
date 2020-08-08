#ifndef	OSD_DATA_H_
#define	OSD_DATA_H_

#include <QObject>
#include <QStringList>
#include <QTimer>

class OSDData : public QObject{
Q_OBJECT
public:
	enum OSDDataType {
		Normal,
		Stay,
		StayTimeout,
		Update,
		ClearAV,
		Clear	
	};

	OSDData(OSDDataType type, const QStringList & cmds, QObject * obj);
	OSDData(uchar id, OSDDataType type, const QStringList & cmds, QObject * obj);
	~OSDData();

	void					startOSD();
	OSDDataType 			getDataType() { return m_dataType; }
	const QStringList &		getCommand() { return m_data; }
	uchar					getId() { return m_id; }
	void					changeType(OSDDataType type);
	void					changeData(const QStringList &);

private:
	OSDDataType m_dataType;
	QStringList m_data;
	QTimer * m_timer;
	uchar	m_id;

	void initData(uchar id, OSDDataType type, const QStringList & cmds);

signals:
	void timeout();
};

#endif
