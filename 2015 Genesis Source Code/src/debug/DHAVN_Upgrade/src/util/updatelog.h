#ifndef	UPDATE_LOG_H
#define	UPDATE_LOG_H

#include <QString>

class UpdateLog
{
public:
	static UpdateLog * instance(){
		if ( updateLog == NULL)
			updateLog = new UpdateLog;

		return updateLog;
	}
	void openLog();

	void closeLog();
	void log(const char * fmt, ...);
	void exception_log(const char *, const char *, int);
private:
	static UpdateLog * updateLog;

	UpdateLog();
	~UpdateLog();

	int	fd;
	bool opened;

	QString getCurrentDate() const;

	ssize_t writen(int fd, const void * vptr, size_t n);
};

#endif
