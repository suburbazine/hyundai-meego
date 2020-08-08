#include "most.h"

MOSTFileCheckSum * g_ibox_file_checksum = NULL;
MOSTFileCheckSum * g_amplifier_file_checksum = NULL;

static MOSTFileCheckSum * find_file_last_content(MOSTFileCheckSum ** header){
	if ( *header == NULL)
		return NULL;

	MOSTFileCheckSum * current = *header;
	while (1){
		if ( current->next == NULL)
			return current;

		current = current->next;
	}
	return NULL;
}

void gf_append_file_checksum(MOSTFileCheckSum ** header, 
									const QString & fileName,
									const QString & fullPath,
									qulonglong fileSize,
									unsigned char * md5sum){
	MOSTFileCheckSum * checksum = new MOSTFileCheckSum;

	checksum->fileName = fileName;
	checksum->fullPath = fullPath;
	checksum->fileSize = fileSize;
	checksum->md5sum = new unsigned char[MD5_DIGEST_LENGTH];
	memcpy(checksum->md5sum, md5sum, MD5_DIGEST_LENGTH);
	checksum->next = NULL;

	MOSTFileCheckSum * lastContent = find_file_last_content(header);

	if ( lastContent == NULL){
		*header = checksum;
	}
	else{
		lastContent->next = checksum;
	}
}

quint32 gf_get_all_blocksize(MOSTFileCheckSum * header, quint32 blocksize){
	if ( header == NULL ) return 0;

	MOSTFileCheckSum * current = header;
	quint32 allBlockCount = 0;

	while(1){
		qulonglong s = current->fileSize;
		quint32 blockCount = s / blocksize;

		if ( s % blocksize ) blockCount++;

		allBlockCount += blockCount;

		if ( current->next == NULL)
			break;

		current = current->next;
	}

	return allBlockCount;
}
