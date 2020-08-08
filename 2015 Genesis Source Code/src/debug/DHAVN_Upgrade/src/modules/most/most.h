#ifndef	MOST_H_
#define	MOST_H_

#include <QFileInfo>
#include <openssl/md5.h>

#define DEVICE_ID			1
#define SENDER_HANDLE		1

#define REPROGRAMMING_REPROGRAMMING_MODE	0x01
#define REPROGRAMMING_NORMAL_OPERATION_MODE	0x00



namespace SWDLID{
	enum SWDLID{
		HMI = 1,
		IBOX,
		AMP
	};

	enum SEQUENCE{
		Idle						 = 0,
		IdentificationHWIdentifier,
		IdentificationDevName,
		IdentificationHWVersion,
		IdentificationNumberOfModule,
		IdentificationReadModuleVersion,
		ReprogrammingReprogramingMode,
		ReprogrammingNormalOperationMode,
		RequestDownload,
		PrepareFileTransfer,
		TransferData,
		RequestTransferExit,
		CheckActivateModule,
		CopyUSBToHDD,
	};

	enum DIRECTION {
		Send,
		Receive
	};

	enum STATUS {
		StatusSuccess = 0,
		StatusError
	};
};

namespace MOSTDef{
	enum IDState{
		ID_STATE_SET = 1,
		ID_STATE_GET
	};
};


typedef struct MOSTFileCheckSum{
	QString	fileName;
	QString	fullPath;
	unsigned char * md5sum;
	qulonglong fileSize;
	MOSTFileCheckSum * next;
} MOSTFileCheckSum;

extern MOSTFileCheckSum * g_ibox_file_checksum;
extern MOSTFileCheckSum * g_amplifier_file_checksum;

extern void gf_append_file_checksum(MOSTFileCheckSum ** header,
										const QString & fileName,
										const QString & fullPath,
										qulonglong fileSize,
										unsigned char * md5sum);

extern quint32 gf_get_all_blocksize(MOSTFileCheckSum * header, quint32 blocksize);
#endif
