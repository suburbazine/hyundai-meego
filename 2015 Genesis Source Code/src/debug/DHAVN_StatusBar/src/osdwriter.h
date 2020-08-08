#ifndef	OSD_WRITER_H_
#define OSD_WRITER_H_

#include <QString>

class OSDWriter{
public:
	static void SendOSD(const QString &msg);
};


#endif
