#ifndef	DHAVN_LOG_H_
#define	DHAVN_LOG_H_

#define MAX_CHAR_A_LINE	255

void open_log(const char * filename);
void lprintf(const char* fmt, ...);
void close_log();


#endif
