/*
    File:       utils.h
    Package:    LG iAP2 Service
    Author:     Yongil Park (yongil.park@lge.com)
    Copyright © [2014-2016] by LG Electronics Inc.


    This program or software including the accompanying associated documentation
    (“Software”) is the proprietary software of LG Electronics Inc. and or its
    licensors, and may only be used, duplicated, modified or distributed pursuant
    to the terms and conditions of a separate written license agreement between you
    and LG Electronics Inc. (“Authorized License”). Except as set forth in an
    Authorized License, LG Electronics Inc. grants no license (express or implied),
    rights to use, or waiver of any kind with respect to the Software, and LG
    Electronics Inc. expressly reserves all rights in and to the Software and all
    intellectual property therein. If you have no Authorized License, then you have
    no rights to use the Software in any ways, and should immediately notify LG
    Electronics Inc. and discontinue all use of the Software.
*/

#ifndef _UTIL_H_
#define _UTIL_H_

#include <linux/unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdint.h>
#include "iAP2Defines.h"


#ifdef __cplusplus
extern "C" {
#endif

	typedef struct dnode_st
	{
		void* pData;
		struct dnode_st* prev;
		struct dnode_st* next;
	} dnode_t;

	pid_t getTid(void);
	uint64_t getMilliseconds(void);
	uint64_t getMicroSeconds(void);
	void writeDataToFile(const char* filePath, unsigned char* data, int length);
	void printDataByHexa(char* pLogTile, unsigned char* pData, int dataLen);

	BOOL queuePut(dnode_t* pHead, dnode_t* pTail, void* pData);
	void queueInit(dnode_t** ppHead, dnode_t** ppTail);
	void* queueGet(dnode_t* pHead, dnode_t* pTail);
	void queueClearAll(dnode_t* pHead, dnode_t* pTail, BOOL bFreeData);
	unsigned int queueGetCount(dnode_t* pHead, dnode_t* pTail);
	void queueFinal(dnode_t* pHead, dnode_t* pTail, BOOL bFreeData);

	void listInitNode(dnode_t** ppHead, dnode_t** ppTail);
	dnode_t* listFindNodeWithData(dnode_t* pHead, dnode_t* pTail, void* pData);
	/* get pData before pNode and Remove it from list */
	void* listGetDataRemoveNodeBeforeNode(dnode_t* pHead, dnode_t* pTail, dnode_t* pNode);
	/* get pData after pNode and Remove it from list */
	void* listGetDataRemoveNodeAfterNode(dnode_t* pHead, dnode_t* pTail, dnode_t* pNode);
	void* listGetDataRemoveNodeAfterNode(dnode_t* pHead, dnode_t* pTail, dnode_t* pNode);
	dnode_t* listGetNodeBeforeNode(dnode_t* pHead, dnode_t* pTail, dnode_t* pNode);
	dnode_t* listGetNodeAfterNode(dnode_t* pHead, dnode_t* pTail, dnode_t* pNode);
	BOOL listDeleteNodeWithData(dnode_t* pHead, dnode_t* pTail, void* pData);
	BOOL listDeleteNode(dnode_t* pHead, dnode_t* pTail, dnode_t* pNode);
	/* insert pData, before pNode */
    BOOL listInsertDataBeforeNode(dnode_t* pHead, dnode_t* pTail, void* pData, dnode_t* pNode);
	BOOL listInsertDataAfterNode(dnode_t* pHead, dnode_t* pTail, void* pData, dnode_t* pNode);
	void listClearAllNode(dnode_t* pHead, dnode_t* pTail, BOOL bFreeData);
	unsigned int  listGetNodeCount(dnode_t* pHead, dnode_t* pTail);
	void listFinalNode(dnode_t* pHead, dnode_t* pTail, BOOL bFreeData);
#ifdef __cplusplus
}
#endif


#endif // _UTIL_H_

