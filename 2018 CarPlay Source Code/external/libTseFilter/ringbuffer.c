/*
 *    @file        <RingBuffer.c>
 *    @brief       ring buffer API of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */

#ifndef RINGBUFFER_C_
#define RINGBUFFER_C_

#include <stdlib.h>
#include "ringbuffer.h"
#include "dbg_mutex.h"
#include <string.h>

struct _sRingBuffer {
	int iLength;
	int iReadIndex;
	int iWriteIndex;
	char * pcHead;
	tDbgMutex mutex;
};

/** @brief create ring buffer
 *
 *  @param ppsRingbuffer ring buffer handle structure.
 *  @param uiSize ring buffer unit size.
 *  @return NULL.
 *  @remark 주의사항 없음
 */
int createRingbuffer(tsRingbuffer ** ppsRingbuffer, unsigned int uiSize)
{
	if ((ppsRingbuffer == NULL) || (*ppsRingbuffer != NULL) || (uiSize == 0))
	{
		return iRINGBUFFER_ERROR;
	}
	else
	{
		tsRingbuffer * psRingbuffer = (tsRingbuffer*)calloc(1, sizeof(tsRingbuffer));
		if (psRingbuffer == NULL) {
			return iRINGBUFFER_ERROR;
		} else {
			int iError = DbgMutexInit(&(psRingbuffer->mutex));
			if (iError != 0) {
				free(psRingbuffer); // fix leak 20150311 DefectId=74389
				return iRINGBUFFER_ERROR;
			}

			psRingbuffer->pcHead = (char*)calloc(uiSize +1, sizeof(char));

			if (psRingbuffer->pcHead == NULL) {
				free(psRingbuffer);
				return iRINGBUFFER_ERROR;
			} else {
				psRingbuffer->iLength = uiSize + 1; /* one byte more to detect a full ringbuffer */
				psRingbuffer->iReadIndex = 0;
				psRingbuffer->iWriteIndex = 0;

				*ppsRingbuffer = psRingbuffer;
				return iRINGBUFFER_OK;
			}
		}
	}
}

/** @brief destroy ring buffer
 *
 *  @param ppsRingbuffer ring buffer handle structure.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int destroyRingbuffer(tsRingbuffer ** ppsRingbuffer)
{
	if ((ppsRingbuffer == NULL) || (*ppsRingbuffer == NULL)) {
		return iRINGBUFFER_ERROR;
	} else {
		tsRingbuffer * psRingbuffer = *ppsRingbuffer;

		DbgMutexDestroy(&(psRingbuffer->mutex));
		free(psRingbuffer->pcHead);
		free(psRingbuffer);

		*ppsRingbuffer = NULL;

		return iRINGBUFFER_OK;
	}
}

/** @brief get free bytes in ring buffer
 *
 *  @param psRingbuffer ring buffer handle structure.
 *  @return free byte number.
 *  @remark 주의사항 없음
 */
int getFreeBytesRingbuffer(tsRingbuffer * psRingbuffer)
{
	if (psRingbuffer == NULL) {
		return iRINGBUFFER_ERROR;
	} else {
		int iFreeBytes = 0;

		if (psRingbuffer->iWriteIndex == psRingbuffer->iReadIndex) {
			iFreeBytes = psRingbuffer->iLength - 1;
		} else if (psRingbuffer->iWriteIndex > psRingbuffer->iReadIndex) {
			iFreeBytes = (psRingbuffer->iLength - 1) + (psRingbuffer->iReadIndex - psRingbuffer->iWriteIndex);
		} else {
			iFreeBytes = (psRingbuffer->iReadIndex - psRingbuffer->iWriteIndex) - 1;
		}

		return iFreeBytes;
	}
}

/** @brief insert data into the ring buffer
 *
 *  @param psRingbuffer ring buffer handle structure.
 *  @param pcData data buffer pointer for insert.
 *  @param iSize size of data buffer.
 *  @return free byte number.
 *  @remark 주의사항 없음
 */
int insertIntoRingbuffer(tsRingbuffer * psRingbuffer, const char * pcData, int iSize)
{
	if ((psRingbuffer == NULL) || (pcData == NULL) ||  (iSize < 0)) {
		return iRINGBUFFER_ERROR;
	} else {
		int iFreeBytes = 0;
		int iWrittenBytes = 0;
		int i;

		DbgMutexLock(&(psRingbuffer->mutex));
		iFreeBytes = getFreeBytesRingbuffer(psRingbuffer);

		if (iFreeBytes < iSize) {
			iWrittenBytes = iFreeBytes;
		} else {
			iWrittenBytes = iSize;
		}

		for (i=0; i<iWrittenBytes; i++)
		{
			psRingbuffer->pcHead[ psRingbuffer->iWriteIndex ] = pcData[i];

		/* mind the ringbuffer wrap */
			psRingbuffer->iWriteIndex++;
			if (psRingbuffer->iWriteIndex == psRingbuffer->iLength)
			{
				psRingbuffer->iWriteIndex = 0;
			}
		}
		DbgMutexUnlock(&(psRingbuffer->mutex));

		return iWrittenBytes;
	}
}

/** @brief extract data from the ring buffer
 *
 *  @param psRingbuffer ring buffer handle structure.
 *  @param pcData output data buffer pointer.
 *  @param iSize requested data size.
 *  @return extracted data byte number.
 *  @remark 주의사항 없음
 */
int extractFromRingbuffer(tsRingbuffer * psRingbuffer, char * pcData, int iSize)
{
	if ((psRingbuffer == NULL) || (pcData == NULL) || (iSize < 0))
	{
		return iRINGBUFFER_ERROR;
	}
	else
	{
		int iAvailableBytes = 0;
		int iExtractedBytes = 0;
		int i;

		DbgMutexLock(&(psRingbuffer->mutex));
		iAvailableBytes = (psRingbuffer->iLength - 1) - getFreeBytesRingbuffer(psRingbuffer);

		if (iAvailableBytes < iSize) {
			iExtractedBytes = iAvailableBytes;
		} else {
			iExtractedBytes = iSize;
		}

		for (i=0; i<iExtractedBytes; i++) {
			pcData[i] = psRingbuffer->pcHead[ psRingbuffer->iReadIndex ];

		/* mind the ringbuffer wrap */
			psRingbuffer->iReadIndex++;
			if (psRingbuffer->iReadIndex == psRingbuffer->iLength) {
				psRingbuffer->iReadIndex = 0;
			}
		}

		DbgMutexUnlock(&(psRingbuffer->mutex));
		return iExtractedBytes;
	}
}

/** @brief reset ring buffer
 *
 *  @param psRingbuffer ring buffer handle structure.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int resetRingbuffer(tsRingbuffer * psRingbuffer)
{
	if (psRingbuffer == NULL) {
		return iRINGBUFFER_ERROR;
	} else {
		DbgMutexLock(&(psRingbuffer->mutex));

		memset(psRingbuffer->pcHead, 0x00, psRingbuffer->iLength);

		psRingbuffer->iReadIndex = 0;
		psRingbuffer->iWriteIndex = 0;

		DbgMutexUnlock(&(psRingbuffer->mutex));
		return iRINGBUFFER_OK;
	}
}
#endif /* RINGBUFFER_C_ */
