#ifndef	SYSTEM_INFO_H_
#define	SYSTEM_INFO_H_

#include <QObject>
#include <QStringList>

class SystemInfo {
public:
	enum { UNDEFINED_VERSION_VALUE = 0xFF };
	enum CertStep {
		UnknownCert = 0,
		DV = 1,
		PV = 2,
		PQ = 3,
	};

	SystemInfo();
	bool setVersion(const QString & ver);
	void setBuildDate(const QString & str) { m_BuildDate = str; }

	bool readVersionFromSystem();

	bool operator ==(const SystemInfo &info){
		if ( m_CertStep == info.m_CertStep &&
			m_CertVersion == info.m_CertVersion &&
			m_SoftwareMajor == info.m_SoftwareMajor &&
			m_SoftwareMinor == info.m_SoftwareMinor){
			return true;
		}
		return false;
	}

	bool operator !=(const SystemInfo &info){
		return !(*this == info);
	}

	bool operator >(const SystemInfo &info){
		if ( m_CertStep == "PV" && info.m_CertStep != "PV")
			return true;	// PV is biggest value
		else if ( m_CertStep == info.m_CertStep){
			if ( m_CertVersion > info.m_CertVersion)
				return true;
			else if ( m_CertVersion == info.m_CertVersion){
				if ( m_SoftwareMajor > info.m_SoftwareMajor)
					return true;
				else if ( m_SoftwareMajor == info.m_SoftwareMajor){
					if ( m_SoftwareMinor > info.m_SoftwareMinor)
						return true;
				}	
			}
		}
		return false;
	}

	bool operator <(const SystemInfo &info){
		return !(*this > info);
	}

	unsigned char m_CertVersion;
	unsigned char m_HardwareVersion;
	unsigned char m_SoftwareMajor;
	unsigned char m_SoftwareMinor;

	QString m_RawVersion;
	QString m_CertStep;
	QString m_BuildDate;

private:
	void setVersion(const QStringList & str);
	void clear();
};

#endif
