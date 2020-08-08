#ifndef __MVOICEFILTER_PRIV_H__
#define __MVOICEFILTER_PRIV_H__

/*
 *    @file        <tseFilterDebugAPI.h>
 *    @brief       Filter context structure and Filter configuration structure header of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */

#include "tse.h"
#include "tse_err.h"
#include "tseFilterDebug.h"

typedef struct {
	void			*pMemory;
	ttseMain		*ptseMain;
	ttseConfigData	*pConfigData;

	int				frameCount;
	int				channels;

	ttseAudio		micIn;
	ttseAudio		recvIn;
	ttseAudio		micOut;
	ttseAudio		recvOut;

	Debugger		*debugger;
	int				enabled;
	char			*dummyRecvIn;

	short int		engineerDebugMode;

	char			tsfPath[2048];
} FilterContext;

typedef struct {
	short int operationMode;
	short int frameSize;
	short int sampleRate;
	short int micChannel;
	short int recvChannel;
	short int refChannel;
	int mvoiceSwitch;
} LGTSE_Config;


#endif /* __MVOICEFILTER_PRIV_H__ */
