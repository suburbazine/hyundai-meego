#ifndef	STATUS_TABLE_H_
#define	STATUS_TABLE_H_


typedef struct{
	const char * command;
	int argc;
	int priority;
	bool show;
	int value;

	const char * icon_name_prefix;
	int icon_count;	
	int icon_start_index;
} Commands;

extern Commands g_status_table[];


#endif
