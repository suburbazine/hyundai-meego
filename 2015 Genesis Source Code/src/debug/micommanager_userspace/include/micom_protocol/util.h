#ifndef UTIL_H
#define UTIL_H

#include <DHAVN_LogUtil.h>
#include <QMutex>

#define MY_ERR      0
#define SYS_ERR     1

class MMusUtil {
	public:	
		MMusUtil();
		~MMusUtil();

	public:
		static MMusUtil	* instance(){
			if(mmusUtil == NULL)
				mmusUtil = new MMusUtil();
			return mmusUtil;
		}
		ssize_t 	gf_writen(int fd, const void *vptr, size_t n);
		quint16 	gf_combine_quint8(quint8 a, quint8 b);
		void 		gf_print_debug(const char * name, const QByteArray & data);
		ssize_t		gf_readn(int fd, void *vptr, size_t n);
		void		gf_err(int, const char * fmt, ...);
		void		gf_dbus_print_debug(const char * name, const QList<QVariant> &);
		void        gf_debug(const char * fmt, ...);
	
	private:
		void err_doit(int errnoflag, unsigned int level, const char *fmt, va_list ap);
		void print_tty(unsigned int level, const QString & msg);
		void gf_print_to_file(unsigned int level, const QString & msg);
	
	private:
		USE_LOG_UTIL;
		static MMusUtil * mmusUtil;
		struct timeval start;
		struct timeval before;
		QMutex mmus_mutex;
		int first_time;
};


#endif
