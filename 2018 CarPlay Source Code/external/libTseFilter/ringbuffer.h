/*
 *    @file        <RingBuffer.h>
 *    @brief       ring buffer API header of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
/* --------------------------------------------------------------- */

#define iRINGBUFFER_OK (0)
#define iRINGBUFFER_ERROR (-1)

typedef struct _sRingBuffer tsRingbuffer;

int createRingbuffer( tsRingbuffer ** ppsRingbuffer, unsigned int uiSize );

int destroyRingbuffer( tsRingbuffer ** ppsRingbuffer );

/* returns number of actually written bytes or iRINGBUFFER_ERROR for a failure */
int insertIntoRingbuffer( tsRingbuffer * psRingbuffer, const char * pcData, int uiSize );

/* returns number of actually extracted bytes or iRINGBUFFER_ERROR for a failure */
int extractFromRingbuffer( tsRingbuffer * psRingbuffer, char * pcData, int uiSize );

int resetRingbuffer(tsRingbuffer * psRingbuffer);
int getFreeBytesRingbuffer(tsRingbuffer * psRingbuffer);
/* --------------------------------------------------------------- */
#ifdef __cplusplus
}
#endif /* C++ */


#endif /* RINGBUFFER_H_ */
