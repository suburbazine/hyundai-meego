#include "meegoeuid.h"
#include "common.h"
#include <unistd.h>

MeegoEUID::MeegoEUID(){
	m_successToChange = false;

	int state = ::seteuid(MEEGO_USER_ID);
	if ( state < 0){
		ULOG_EXCEPTION;
		ULOG->log("failed to change meego euid");
	}
	else{
		m_successToChange = true;
	}
}

MeegoEUID::~MeegoEUID(){
	if ( m_successToChange ){
		int state = ::seteuid(ROOT_USER_ID);
		if ( state < 0){
			ULOG_EXCEPTION;
			ULOG->log("failed to change root euid");
		}
	}
}
