/*
    File:       utils.cpp
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

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include "iAp2ServiceLog.h"
#include "utils.h"

pid_t getTid(void)
{
	return syscall( __NR_gettid );
}  

uint64_t getMilliseconds(void)
{
	struct timeval mytime;
	gettimeofday(&mytime, NULL);
	return (mytime.tv_sec * 1000000LL + mytime.tv_usec)/1000;
}

uint64_t getMicroSeconds(void)
{
	struct timeval mytime;
	gettimeofday(&mytime, NULL);
	return (mytime.tv_sec * 1000000LL + mytime.tv_usec);
}

void writeDataToFile(const char* filePath, unsigned char* data, int length)
{
	FILE* file = NULL;
    if ((file = fopen(filePath, "wb")) == NULL)
    {
		LOGD("can't open fie[%s]", filePath);
		return;
	}
	
	fwrite(data, length, 1, file);
	fclose(file);
}

void printDataByHexa(char* pLogTile, unsigned char* pData, int dataLen)
{
	int i = 0;
	if ((pLogTile == NULL) || (pData == NULL) || (dataLen == 0))
		return;
	
	LOGE("[%s] dataLen[%d]", pLogTile, dataLen);

	for (i=0; i <= dataLen; i += 16)
	{
		fprintf(stderr, "	[%04d] %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x\n", i, \
			pData[i], pData[i+1], pData[i+2], pData[i+3], pData[i+4], pData[i+5], pData[i+6], pData[i+7], \
			pData[i+8], pData[i+9], pData[i+10], pData[i+11], pData[i+12], pData[i+13], pData[i+14], pData[i+15]);
	}
}

void queueInit(dnode_t** ppHead, dnode_t** ppTail)
{
	//LOGD("called queueInit()");

	listInitNode(ppHead, ppTail);
}

BOOL queuePut(dnode_t* pHead, dnode_t* pTail, void* pData)
{
    return listInsertDataAfterNode(pHead, pTail, pData, pHead);
}

void* queueGet(dnode_t* pHead, dnode_t* pTail)
{
	return listGetDataRemoveNodeBeforeNode(pHead, pTail, pTail);
}

void queueClearAll(dnode_t* pHead, dnode_t* pTail, BOOL bFreeData)
{
	listClearAllNode(pHead, pTail, bFreeData);
}

unsigned int  queueGetCount(dnode_t* pHead, dnode_t* pTail)
{
	return listGetNodeCount(pHead, pTail);
}


void queueFinal(dnode_t* pHead, dnode_t* pTail, BOOL bFreeData)
{
	listFinalNode(pHead, pTail, bFreeData);
}

// double linked list
void listInitNode(dnode_t** ppHead, dnode_t** ppTail)
{
	//LOGD("called listInitNode()");
	*ppHead = (dnode_t*) malloc(sizeof(dnode_t));
	if (*ppHead == NULL)
	{
		LOGE("ERROR : fail to malloc head");
		return;
	}

	*ppTail = (dnode_t*) malloc(sizeof(dnode_t));
	if (*ppTail == NULL)
	{
		LOGE("ERROR : fail to malloc tail");
		return;
	}

	(*ppHead)->prev = (*ppHead);
	(*ppHead)->next = (*ppTail);
	
	(*ppTail)->prev = (*ppHead);
	(*ppTail)->next = (*ppTail);
}

dnode_t* listFindNodeWithData(dnode_t* pHead, dnode_t* pTail, void* pData)
{
	dnode_t* t;

	if (pHead == NULL || pTail == NULL)
	{
		LOGE("ERROR : pHead[%p] or pTail[%p] is NULL", pHead, pTail);
		return NULL;
	}

	if (pData == NULL)
	{
		LOGE("ERROR : pData is NULL");
		return NULL;
	}

	t = pHead->next;

	while (t != pTail)
	{
		if (t->pData == pData)
			return t;

        t = t->next;
	}
	
	return NULL;
}

static BOOL listIsNodeInList(dnode_t* pHead, dnode_t* pTail, dnode_t* pNode, BOOL bIncludeHeadTail)
{
	dnode_t* t;

	if (pHead == NULL || pTail == NULL)
	{
		LOGE("ERROR : pHead[%p] or pTail[%p] is NULL", pHead, pTail);
        return FALSE;
	}

	if (pNode == NULL)
	{
		LOGE("ERROR : pNode is NULL");
        return FALSE;
	}

	if ((bIncludeHeadTail == TRUE) && ((pNode == pHead) || (pNode == pTail)))
	{
		return TRUE;
	}
	
	t = pHead->next;
	while (t != pTail)
	{
		if (t == pNode)
			return TRUE;

        t = t->next;
	}
	
	return FALSE;
}


/* get pData before pNode and Remove it from list */
void* listGetDataRemoveNodeBeforeNode(dnode_t* pHead, dnode_t* pTail, dnode_t* pNode)
{
	dnode_t* t;
	void* pData =  NULL;

	if (pHead == NULL || pTail == NULL)
	{
		LOGE("ERROR : pHead[%p] or pTail[%p] is NULL", pHead, pTail);
		return FALSE;
	}

	if (pNode == NULL)
	{
		LOGE("ERROR : pNode is NULL");
		return NULL;
	}

	if (listIsNodeInList(pHead, pTail, pNode, TRUE) == FALSE)
	{
		LOGE("ERROR : pNode[%p] is not member of this list", pNode);
		return NULL;
	}

	t = pNode->prev;
	if (t == pHead)
		return NULL;

	pData = t->pData;

	pNode->prev = t->prev;
	t->prev->next = pNode;

	free(t);
	return pData;
}

/* get pData after pNode and Remove it from list */
void* listGetDataRemoveNodeAfterNode(dnode_t* pHead, dnode_t* pTail, dnode_t* pNode)
{
	dnode_t* t;

	if (pHead == NULL || pTail == NULL)
	{
		LOGE("ERROR : pHead[%p] or pTail[%p] is NULL", pHead, pTail);
		return FALSE;
	}

	if (pNode == NULL)
	{
		LOGE("ERROR : pNode is NULL");
		return NULL;
	}

	if (listIsNodeInList(pHead, pTail, pNode, TRUE) == FALSE)
	{
		LOGE("ERROR : pNode[%p] is not member of this list", pNode);
		return NULL;
	}

	t = pNode->next;
	if (t == pTail)
		return NULL;

	return t;
}


/* get Node before pNode from list */
dnode_t* listGetNodeBeforeNode(dnode_t* pHead, dnode_t* pTail, dnode_t* pNode)
{
	dnode_t* t;

	if (pHead == NULL || pTail == NULL)
	{
		LOGE("ERROR : pHead[%p] or pTail[%p] is NULL", pHead, pTail);
		return FALSE;
	}

	if (pNode == NULL)
	{
		LOGE("ERROR : pNode is NULL");
		return NULL;
	}

	if (listIsNodeInList(pHead, pTail, pNode, TRUE) == FALSE)
	{
		LOGE("ERROR : pNode[%p] is not member of this list", pNode);
		return NULL;
	}

	t = pNode->prev;
	if (t == pHead)
		return NULL;

	return t;
}

/* get Node after pNode from list */
dnode_t* listGetNodeAfterNode(dnode_t* pHead, dnode_t* pTail, dnode_t* pNode)
{
	dnode_t* t;
	void* pData =  NULL;

	if (pHead == NULL || pTail == NULL)
	{
		LOGE("ERROR : pHead[%p] or pTail[%p] is NULL", pHead, pTail);
		return FALSE;
	}

	if (pNode == NULL)
	{
		LOGE("ERROR : pNode is NULL");
		return NULL;
	}

	if (listIsNodeInList(pHead, pTail, pNode, TRUE) == FALSE)
	{
		LOGE("ERROR : pNode[%p] is not member of this list", pNode);
		return NULL;
	}

	t = pNode->next;
	if (t == pTail)
		return NULL;

	pData = t->pData;
#if 0 // test // soohwan.oh - 2015-01-17
	pNode->next = t->next;
	t->next->prev = pNode;

	free(t);
#endif
	return pData;
}

BOOL listDeleteNodeWithData(dnode_t* pHead, dnode_t* pTail, void* pData)
{
    dnode_t* s;

	if (pHead == NULL || pTail == NULL)
	{
		LOGE("ERROR : pHead[%p] or pTail[%p] is NULL", pHead, pTail);
		return FALSE;
	}

	if (pData == NULL)
	{
		LOGE("ERROR : pData is NULL");
		return FALSE;
	}
	
    s = listFindNodeWithData(pHead, pTail, pData);

	if (s != NULL)
	{
		s->prev->next = s->next;
		s->next->prev = s->prev;
		free(s);

		return TRUE;
	}

	return FALSE;
}

BOOL listDeleteNode(dnode_t* pHead, dnode_t* pTail, dnode_t* pNode)
{
	if (pHead == NULL || pTail == NULL)
	{
		LOGE("ERROR : pHead[%p] or pTail[%p] is NULL", pHead, pTail);
		return FALSE;
	}

	if (pNode == pHead || pNode == pTail)
	{
		LOGE("ERROR : pNode[%p] is pHead[%p] or pTail[%p]", pNode, pHead, pTail);
		return FALSE;
	}	

	if (pNode == NULL)
	{
		LOGE("ERROR : pNode is NULL");
		return FALSE;
	}

	if (listIsNodeInList(pHead, pTail, pNode, FALSE) == FALSE)
	{
		LOGE("ERROR : pNode[%p] is not member of this list", pNode);
        return FALSE;
	}
	
	pNode->prev->next = pNode->next;
	pNode->next->prev = pNode->prev;

	free(pNode);

	return TRUE;
}


/* insert pData before pNode */
BOOL listInsertDataBeforeNode(dnode_t* pHead, dnode_t* pTail, void* pData, dnode_t* pNode)
{
	dnode_t* t;

	if (pHead == NULL || pTail == NULL)
	{
		LOGE("ERROR : pHead[%p] or pTail[%p] is NULL", pHead, pTail);
		return FALSE;
	}

	if (pNode == pHead)
        return FALSE;

	t = (dnode_t*) malloc(sizeof(dnode_t));
	if (t == NULL )
	{
		LOGE("ERROR : out of memory");
		return FALSE;
	}

	if (listIsNodeInList(pHead, pTail, pNode, TRUE) == FALSE)
	{
		LOGE("ERROR : pNode[%p] is not member of this list", pNode);
		free(t);
		return FALSE;
	}
	
	t->pData = pData;
	pNode->prev->next = t;
	t->prev = pNode->prev;
	pNode->prev = t;
	t->next = pNode;

	return TRUE;
}

/* insert pData After pNode */
BOOL listInsertDataAfterNode(dnode_t* pHead, dnode_t* pTail, void* pData, dnode_t* pNode)
{
	dnode_t* t;

	if (pHead == NULL || pTail == NULL)
	{
		LOGE("ERROR : pHead[%p] or pTail[%p] is NULL", pHead, pTail);
		return FALSE;
	}
	
	if (pNode == pTail)
        return FALSE;

	t = (dnode_t*) malloc(sizeof(dnode_t));
	if (t == NULL )
	{
		LOGE("ERROR : out of memory");
		return FALSE;
	}

	if (listIsNodeInList(pHead, pTail, pNode, TRUE) == FALSE)
	{
		LOGE("ERROR : pNode[%p] is not member of this list", pNode);
		free(t);
		return FALSE;
	}
	
	t->pData = pData;

	// verify ...
	pNode->next->prev = t;
	t->next = pNode->next;
	pNode->next = t;
	t->prev = pNode;
	
	return TRUE;
}


void listClearAllNode(dnode_t* pHead, dnode_t* pTail, BOOL bFreeData)
{
	dnode_t* t;
	dnode_t* s;

	if (pHead == NULL || pTail == NULL)
	{
		LOGE("ERROR : pHead[%p] or pTail[%p] is NULL", pHead, pTail);
		return;
	}

	t = pHead->next;

	while (t != pTail)
	{
		s = t;
		t = t->next;

		if ( (bFreeData == TRUE) && (s->pData != NULL))
			free(s->pData);
		
		free(s);
	}

	pHead->next = pTail;
    pTail->prev = pHead;
}

unsigned int listGetNodeCount(dnode_t* pHead, dnode_t* pTail)
{
	dnode_t* t;
	unsigned int count = 0;

	if (pHead == NULL || pTail == NULL)
	{
		LOGE("ERROR : pHead[%p] or pTail[%p] is NULL", pHead, pTail);
		return 0;
	}

	t = pHead->next;

	while (t != pTail)
	{
		t = t->next;
		count++;
	}

	return count;
}

void listFinalNode(dnode_t* pHead, dnode_t* pTail, BOOL bFreeData)
{
	if (pHead == NULL || pTail == NULL)
	{
		LOGE("ERROR : pHead[%p] or pTail[%p] is NULL", pHead, pTail);
		return;
	}

    listClearAllNode(pHead, pTail, bFreeData);

	if (pHead)
		free(pHead);

	if (pTail)
        free(pTail);
}


