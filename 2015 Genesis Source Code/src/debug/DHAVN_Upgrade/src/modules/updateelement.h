#ifndef	UPDATE_ELEMENT_H_
#define	UPDATE_ELEMENT_H_

#include <QString>
#include <QFile>
#include <QTime>

class UpdateElement{
public:
	enum ElementType {
		et_PARTITION,
		et_RPM,
		et_FIRMWARE,
		et_IBOX,
		et_AMP
	};

	enum ElementStatus {
		es_READY,
		es_DONE,
		es_FAIL
	};

	enum RPMType {
		rpm_UPDATE,
		rpm_INSTALL
	};

	static UpdateElement * CreateFirmwareElement(
							const QString & name,
							const QString & version,
							const QString & path,
                                                        const QString & variant,
                                                        const QString & lname);
	static UpdateElement * CreateIBoxElement(
							const QString & aVersion, 
							const QString & aDate);
	static UpdateElement * CreateAMPElement(const QString & aVersion);
	static UpdateElement * CreatePartitionElement(
							const QString & name,
							const QString & prevVersion,
							const QString & version,
							const QString & fileSource,
							unsigned int weight);
	static UpdateElement * CreateRPMElement(
							const QString & aName,
							const QString & version,
							RPMType type);

	bool	isValid() const;

        void    setLcdType (const QString & lName) {l_name =lName;}
	void	setPath(const QString & aPath) { m_path = aPath; }
	void	setVersion(const QString & aVersion) { m_version = aVersion; }
	void	setName(const QString & aName) { m_name = aName; }
	void	setVariant(const QString & aVariant) { m_variant = aVariant; }
	void	setDate(const QString & aDate) { m_date = aDate; }
	void	setDeviceFile(const QString & dev) { m_deviceFile = dev; }
	void	setPrevVersion(const QString & ver) { m_prevVersion = ver; }
	void	setRPMType(RPMType t) { m_rpmType = t; }
	void	setWeight(int w) { m_weight = w; }
	void	setDescription(const QString & str) { m_description = str; }
	void	setStatus(ElementStatus status) { m_status = status; }
	void	resetAccessTime() { m_lastAccess.restart(); }
	bool	isFirmwareTimeout() {
                if ( m_lastAccess.elapsed() >= 1000 * 180){	// 3 min
			return true;
		}
		return false;
	}	

	ElementType			type() { return m_type; }
	ElementStatus		status() { return m_status; }
        const QString &         lname() {return l_name;}
	const QString &		path() { return m_path; }
	const QString &		version() { return m_version; }
	const QString &		name() { return m_name;}
	const QString &		variant() { return m_variant; }
	const QString &		date() { return m_date;}
	const QString &		deviceFile() { return m_deviceFile; }
	const QString &		prevVersion() { return m_prevVersion; }
	const QString &		description() { return m_description; }

	RPMType				rpmType() { return m_rpmType; }

	int					weight() { return m_weight; }

private:
	UpdateElement(ElementType aType);

private:
	ElementType	m_type;
	ElementStatus m_status;
        QString         l_name;
	QString		m_path;
	QString		m_version;
	QString		m_name;
	QString		m_variant;
	QString		m_date;
	QString		m_deviceFile;
	QString		m_prevVersion;
	QString		m_description;
	RPMType		m_rpmType;
	int			m_weight;
	QTime		m_lastAccess;
};

#endif
