/*
* persistman.c, The implementation of the persistence manager API
*
* Copyright (c) 2008 Wind River Systems, Inc.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the Lesser GNU General Public License version 2.1 as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the Lesser GNU General Public License for more details.
*
* You should have received a copy of the Lesser GNU General Public License version 2.1
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
*/ 

/** 
 * \file persistman.c
 * \brief The implementation of the persistence manager API
 * \version $Revision$
 * \date    $Date$
 */

/**************************************************************************/
/*                              Includes                                  */
/**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>

#include "persistman.h"

/**************************************************************************/
/*                               Defines                                  */
/**************************************************************************/
/* Flash memory constants */
#define MAGIC_NUM    0xDEADBEEF

/* Required to make space for SQL commands and file path in a single string */
#define STRBUFF 64

/* The number of available backends */
#define NUM_BACKENDS 2

#define si_pm_errhandler(stmt,args...) \
   fprintf(stderr, stmt ". Error occurred in file %s line %d\n", ##args, __FILE__, __LINE__)

/**************************************************************************/
/*                              Typedefs                                  */
/**************************************************************************/
/** \brief Describes a portion of the flash memory data store
 */
typedef struct SI_PM_flash_block_t {
    int isFree;
    int lastBlock;
    int size;
    char key[SI_PM_MAX_LENGTH];
    char appName[SI_PM_MAX_LENGTH];
} SI_PM_flash_block_t;

/** \brief Placed at the front of the flash data store
 */
typedef struct SI_PM_flash_head_t {
    int magicNum;     /* Used to inform PM that this area has been set up */
    int size;
} SI_PM_flash_head_t;

/** \brief Defines a specific connection between an app and a pm data store 
 */
typedef struct SI_PM_handle_t {
    int storeType;
    char location[SI_PM_MAX_LENGTH];
    char appName[SI_PM_MAX_LENGTH];
    int nonblocking;
    int useSemaphores;
    int maxSize;
    
    sqlite3 *db;
    sqlite3_blob *theBlob;
    int blobSize;
    
    SI_PM_flash_block_t *startFBlock;
    SI_PM_flash_block_t *currentFBlock;
    SI_PM_flash_head_t *fHead;
    int fd;
    sem_t *semid;
} SI_PM_handle_t;

static int si_pm_wait(SI_PM_handle_t *pm, const char *keyStr)
{
    char tempStr[SI_PM_MAX_LENGTH+1];
    int result = SI_PM_OK;

    if(pm->useSemaphores)
    {
	sprintf(tempStr, "/%s", keyStr);
	pm->semid = sem_open(tempStr, O_CREAT, 0644, 1);
	if(pm->semid == SEM_FAILED)
	{
		si_pm_errhandler("Error getting semaphore: %s", strerror(errno));
		return SI_PM_UE;
	}
        
	/* Wait until the data store resource is available */
	if(pm->nonblocking)
	{
		result = sem_trywait(pm->semid);
	}
	else
	{
		result = sem_wait(pm->semid);
	}
    }
    
    return result;
}

static int si_pm_post(SI_PM_handle_t *pm)
{
    if(pm->useSemaphores)
    {
        return sem_post(pm->semid);
    }
    
    return SI_PM_OK;
}

static int si_pm_close_sem(SI_PM_handle_t *pm)
{
    if(pm->useSemaphores)
    {
        return sem_close(pm->semid);
    }
    
    return SI_PM_OK;
}
static int si_pm_sqlite_open(SI_PM_handle_t *pm)
{
   int result;
   char tempStr[SI_PM_MAX_LENGTH+STRBUFF];
   char *zErrMsg;

    result = sqlite3_open(pm->location, &(pm->db));
    if(result)
    {
        si_pm_errhandler("Error: Cannot open database: %s", sqlite3_errmsg(pm->db));
        sqlite3_close(pm->db);
        return SI_PM_BDS;
    }
    sprintf(tempStr, "CREATE TABLE '%s' (key TEXT PRIMARY KEY, value BLOB);", pm->appName);
    result = sqlite3_exec(pm->db, tempStr, 0, 0, &zErrMsg);
    if(result != SQLITE_OK)
    {
        /* An error occurs if the table already exists, but that's 'ok' */
        sprintf(tempStr, "table '%s' already exists", pm->appName);
        if(strcmp(zErrMsg,tempStr))
        {
            si_pm_errhandler("SQL error: %s", zErrMsg);

            /* CR#16384 - Use only sqlite3_free function to free error message.
            if(zErrMsg)
                free(zErrMsg);
            */
            sqlite3_close(pm->db);
            sqlite3_free(zErrMsg);
            return SI_PM_UE;
        }
        sqlite3_free(zErrMsg);
    }
    
    return SI_PM_OK;
}

static int si_pm_flash_open(SI_PM_handle_t *pm)
{
    /* Open the specified device/file */
    pm->fd = open(pm->location, O_RDWR|O_SYNC);
    if(pm->fd < 0)
    {
        si_pm_errhandler("Error opening file/device: %s", strerror(errno));
        return SI_PM_BDS;
    }
            
    pm->fHead = (SI_PM_flash_head_t *)mmap(NULL, pm->maxSize,
                    PROT_READ|PROT_WRITE, MAP_SHARED, pm->fd, 0);
    if(pm->fHead == MAP_FAILED)
    {
        si_pm_errhandler("Error mapping file/device: %s", strerror(errno));
        return SI_PM_BDS;
    }
    
    pm->startFBlock = (SI_PM_flash_block_t *)pm->fHead + sizeof(SI_PM_flash_head_t);
    /* Check to see if that location has existing data store, if not, init it */
    if(pm->fHead->magicNum != MAGIC_NUM)
    {
        pm->startFBlock->isFree    = 1;
        pm->startFBlock->lastBlock = 1;
        pm->startFBlock->size      = pm->maxSize - sizeof(pm->fHead);
        pm->startFBlock->key[0]    = '\0';
        pm->fHead->magicNum        = MAGIC_NUM;
        pm->fHead->size            = pm->maxSize;
    }
    else if (pm->fHead->size != pm->maxSize)
    {
        si_pm_errhandler("Error: Size of existing data store is different from specified size");
        return SI_PM_BDS;
    }
    
    return SI_PM_OK;
}

/* Invoked by sqlite3 API, used to return rowid of requested key */
static int getCallback(void *arg1, int argc, char **argv, char **colNames)
{
    int *rowid = (int *)arg1;
    
    *rowid = atoi(argv[0]);
    
    return 0;
}

static int si_pm_sqlite_blob_open(SI_PM_handle_t *pm, const char *keyStr, int rwFlag, int *dataSize)
{
    char tempStr[SI_PM_MAX_LENGTH+STRBUFF], *zErrMsg;
    int result;
    int rowid             = 0;
    sqlite3_stmt *sqlProg = 0;
    
    if(pm->theBlob != NULL)
    {
        SIPMBlobClose(pm);
    }
            
    while(rowid == 0)
    {
        /* Find the rowid of the requested data */
        sprintf(tempStr, "SELECT rowid FROM '%s' WHERE key='%s';", pm->appName, keyStr);
        rowid=0;
        result = sqlite3_exec(pm->db, tempStr, getCallback, &rowid, &zErrMsg);
        if(result != SQLITE_OK)
        {
            si_pm_errhandler("SQL Error: %s", zErrMsg);
            sqlite3_free(zErrMsg);
            return SI_PM_UE;
        }
        /* Key was not found, create a new entry if needed */
        if(rowid == 0)
        {
            if(rwFlag == SI_PM_READ)
            {
                return SI_PM_KNF;
            }
                
            sprintf(tempStr, "INSERT INTO '%s' VALUES ('%s',?);",
                    pm->appName, keyStr);
                
            /* Now execute the command with the specified blob of data */
            result = sqlite3_prepare_v2(pm->db, tempStr, -1, &sqlProg, 0);
            if (result == SQLITE_OK && sqlProg != NULL)
            {
                sqlite3_bind_zeroblob(sqlProg, 1, *dataSize);
            
                while ((result = sqlite3_step(sqlProg)) == SQLITE_ROW) {}
            
                result = sqlite3_finalize(sqlProg);
            }
            if(result != SQLITE_OK)
            {
                si_pm_errhandler("SQL Error: %s", sqlite3_errmsg(pm->db));
                return SI_PM_UE;
            }
        }
    }
            
    if(sqlite3_blob_open(pm->db, 0, pm->appName, "value", rowid, rwFlag, &(pm->theBlob))!=SQLITE_OK)
    {
        printf("Error opening blob: %s\n", sqlite3_errmsg(pm->db));
        return SI_PM_UE;
    }
        
    pm->blobSize = sqlite3_blob_bytes(pm->theBlob);
    
    return SI_PM_OK;
}

/* Sets aside space in the data store of a requested size */
static SI_PM_flash_block_t *fblock_malloc(SI_PM_flash_block_t *startBlock, int size)
{
    void *currentLocation;
    void *nextLocation;
    SI_PM_flash_block_t *currentBlock;
    SI_PM_flash_block_t *nextBlock;
    
    /* The amount of memory actually needed is includes space for the block header */
    size = size + sizeof(SI_PM_flash_block_t);
    
    /* Begin searching for available memory at the start */
    currentLocation = (void *)startBlock;
    
    /* Search until the end of memory */
    while(1) 
    {
        /* 
         * currentLocation and currentBlock
         * will point to the same location, but currentBlock
         * will be of the correct type to read/write the header
         */
        currentBlock =
                (SI_PM_flash_block_t *)currentLocation;
    
        /* Is this region available? */
        if(currentBlock->isFree)
        {
            /* Find true size of region (next region might be empty too!) */
            if(!currentBlock->lastBlock)
            {
                nextLocation = currentLocation + currentBlock->size;
                while(1)
                {
                    nextBlock = (SI_PM_flash_block_t *)nextLocation;
                    if(nextBlock->isFree)
                    {
                        currentBlock->size += nextBlock->size;
                        if(nextBlock->lastBlock)
                            currentBlock->lastBlock = 1;
                        msync(currentBlock, sizeof(currentBlock), MS_SYNC);
                    }
                    else
                    {
                        break;
                    }
        
                    if(nextBlock->lastBlock)
                    {
                        break;
                    }

                    nextLocation += nextBlock->size;
                }
            }

            /* Is this region big enough */
            if(currentBlock->size >= size)
            {
                /* If necessary, place a header at end of allocated region */
                if((currentBlock->size - size) >
                    sizeof(SI_PM_flash_block_t))
                {
                    nextLocation = currentLocation + size;
                    nextBlock = (SI_PM_flash_block_t *)nextLocation;
                    nextBlock->isFree = 1;
                    if(currentBlock->lastBlock)
                    {
                        nextBlock->lastBlock = 1;
                        currentBlock->lastBlock = 0;
                    }
                    else
                    {
                        nextBlock->lastBlock = 0;
                    }
                    nextBlock->size =
                        currentBlock->size - size;
                    msync(nextBlock, sizeof(nextBlock), MS_SYNC);
                }
        
        
                /* This region will be used now! */
                currentBlock->isFree = 0;
                currentBlock->size = size - sizeof(SI_PM_flash_block_t);
                msync(currentBlock, size, MS_SYNC);
        
                /* The block location returned */
                return currentBlock;
            }
        }
    
        /* We're done looking if that was the last region of memory */
        if(!currentBlock->lastBlock)
        {
            currentLocation += currentBlock->size + sizeof(SI_PM_flash_block_t);
        }
        else
        {
            return NULL;
        }
    }
}

static int si_pm_flash_blob_open(SI_PM_handle_t *pm, const char *keyStr, int rwFlag, int *dataSize)
{
    int result;
    void *nextLocation;
    
    /* Create/open the semaphore for the requested key */
    result = si_pm_wait(pm, keyStr);
    
    /*
     * If the calling application does not want to wait and the resource
     * is blocked, return with SI_PM_BLOCKED
     */
    if(result != 0)
    {
        if(errno == EAGAIN)
        {
            return SI_PM_BLOCKED;
        }
        else
        {
            si_pm_errhandler("Semaphore error: %s", strerror(errno));
            return SI_PM_UE;
        }
    }
            
    /* Try to find the key */
    pm->currentFBlock = pm->startFBlock;
    while(1)
    {
        if((strcmp(pm->currentFBlock->key, keyStr)==0) &&
           (strcmp(pm->currentFBlock->appName, pm->appName)==0))
        {
            result = 1;
            break;
        }
        else if (pm->currentFBlock->lastBlock == 1)
        {
            result = 0;
            break;
        }

        nextLocation = (void *)pm->currentFBlock + pm->currentFBlock->size + sizeof(SI_PM_flash_block_t);
        pm->currentFBlock = (SI_PM_flash_block_t *)nextLocation;
    }
    /* If the key was found */
    if(result == 1)
    {
        pm->blobSize = pm->currentFBlock->size;
    }
    else
    {
        if(rwFlag == SI_PM_READ)
        {
            pm->currentFBlock = NULL;
            si_pm_post(pm);
            return SI_PM_KNF;
        }
                
        /* Reserve a chunk of space for the blob of data */
        pm->currentFBlock = fblock_malloc(pm->startFBlock, *dataSize);
        if(pm->currentFBlock == NULL) {
            si_pm_post(pm);
            return SI_PM_NMS;    /* There was not enough space to store the value */
        }
        strncpy(pm->currentFBlock->appName, pm->appName, SI_PM_MAX_LENGTH);
        strncpy(pm->currentFBlock->key, keyStr, SI_PM_MAX_LENGTH);
        pm->blobSize = *dataSize;
        msync(pm->startFBlock, pm->maxSize-sizeof(pm->fHead), MS_SYNC);
    }
    
    return SI_PM_OK;
}


/* Initializes the persistence manager session */
int SIPMOpen(PMHANDLE_T **pmArg, int backEnd, const char *location, const char *appName, int flags, int maxSize)
{
    int result;
    SI_PM_handle_t *pm;
    
    pm              = (SI_PM_handle_t *)malloc(sizeof(SI_PM_handle_t));
    pm->storeType   = backEnd;
    pm->db          = NULL;
    pm->theBlob     = NULL;
    pm->startFBlock = NULL;
    
    /* Parse flags */
    pm->nonblocking = flags & SI_PM_NB;
    pm->useSemaphores = (flags & SI_PM_SEM)>>1;
    
    /* Use specified size for data store, ignored by some back-ends */
    pm->maxSize = maxSize;
    
    /* Use specified app name */
    strncpy(pm->appName, appName, SI_PM_MAX_LENGTH);
    if(pm->appName[SI_PM_MAX_LENGTH-1] != '\0') {
        si_pm_errhandler("Error: appName is too long, max length: %d", SI_PM_MAX_LENGTH);
        free(pm);
        return SI_PM_UE;
    }
    
    /* Use data store at specified location */
    strncpy(pm->location, location, SI_PM_MAX_LENGTH);
    if(pm->location[SI_PM_MAX_LENGTH-1] != '\0') {
        si_pm_errhandler("Error: Location path is too long, max length: %d", SI_PM_MAX_LENGTH);
        free(pm);
        return SI_PM_BDS;
    }
    
    /* Based on the back-end, do appropriate init */
    switch(pm->storeType) {
        case SI_PM_SQLITE:
            result = si_pm_sqlite_open(pm);
            break;
        case SI_PM_FLASH:
            result = si_pm_flash_open(pm);
            break;
        default:
            si_pm_errhandler("Error: Unknown back-end specified");
            result = SI_PM_BBE;
    }
    
    if(result != SI_PM_OK)
    {
        free(pm);
        return result;
    }
    *pmArg = (PMHANDLE_T *)pm;
    return SI_PM_OK;
}

int SIPMClose(PMHANDLE_T *pmArg)
{
    SI_PM_handle_t *pm = (SI_PM_handle_t *)pmArg;
    int result;
    
    switch(pm->storeType)
    {
        case SI_PM_SQLITE:
            result = SIPMBlobClose(pm);
            if(result != SI_PM_OK)
            {
                return result;
            }
            
            result = sqlite3_close(pm->db);
            if(result != SQLITE_OK)
            {
                si_pm_errhandler("SQL Error: %s", sqlite3_errmsg(pm->db));
                return SI_PM_UE;
            }
            break;
        case SI_PM_FLASH:
            result = munmap(pm->fHead, pm->maxSize);
            if(result != 0)
            {
                si_pm_errhandler("munmap error: %s", strerror(errno));
            }
            close(pm->fd);
            break;
        default:
            si_pm_errhandler("Error: Unknown back-end specified");
            return SI_PM_BBE;
    }
    
    free(pm);
    return SI_PM_OK;
}

/* Saves data into the data store using the Storage Key */
int SIPMSet(PMHANDLE_T *pmArg, const char *keyStr, unsigned char *data, int dataSize)
{
    SI_PM_handle_t *pm = (SI_PM_handle_t *)pmArg;
    int result;
    
    if(pm == NULL)
    {
        si_pm_errhandler("ERROR: SI_PM_handle_t is NULL");
        return SI_PM_UE;
    }
    
    result = SIPMBlobOpen(pm, keyStr, SI_PM_WRITE, &dataSize);
    if(result != SI_PM_OK)
    {
        return result;
    }
    
    result = SIPMBlobWrite(pm, data, 0, 0);
    if(result != SI_PM_OK)
    {
        return result;
    }
    
    result = SIPMBlobClose(pm);
    if(result != SI_PM_OK)
    {
        return result;
    }
    
    return SI_PM_OK;
}

/* Fetches the requested data from the data store. */
int SIPMGet(PMHANDLE_T *pmArg, const char *keyStr, unsigned char *data, int *dataSize)
{
    SI_PM_handle_t *pm = (SI_PM_handle_t *)pmArg;
    int result;
    
    if(pm == NULL)
    {
        si_pm_errhandler("ERROR: SI_PM_handle_t is NULL");
        return SI_PM_UE;
    }
    
    result = SIPMBlobOpen(pm, keyStr, SI_PM_READ, dataSize);
    if(result != SI_PM_OK)
    {
        return result;
    }
    
    result = SIPMBlobRead(pm, data, 0, 0);
    if(result != SI_PM_OK)
    {
        return result;
    }
    
    result = SIPMBlobClose(pm);
    if(result != SI_PM_OK)
    {
        return result;
    }
    
    return SI_PM_OK;
}

/* Opens a value in the data store to be read/written to */
int SIPMBlobOpen(PMHANDLE_T *pmArg, const char *keyStr, int rwFlag, int *dataSize)
{
    SI_PM_handle_t *pm = (SI_PM_handle_t *)pmArg;
    int result;
    
    if(pm == NULL)
    {
        si_pm_errhandler("ERROR: SI_PM_handle_t is NULL");
        return SI_PM_UE;
    }
    
    
    
    switch(pm->storeType)
    {
        case SI_PM_SQLITE:
            result = si_pm_sqlite_blob_open(pm, keyStr, rwFlag, dataSize);
            break;
        case SI_PM_FLASH:
            result = si_pm_flash_blob_open(pm, keyStr, rwFlag, dataSize);
            break;
        default:
            si_pm_errhandler("Error: Unknown back-end specified");
            result = SI_PM_BBE;
    }
    
    if(result != SI_PM_OK)
    {
        return result;
    }
    
    if(rwFlag == SI_PM_WRITE)
    {
        if(pm->blobSize != *dataSize)
        {
            si_pm_errhandler("Error: Requested blob is of a different size than specified size");
            SIPMBlobClose(pm);
            return SI_PM_UE;
        }
    } 
    else if(rwFlag == SI_PM_READ)
    {
        *dataSize = pm->blobSize;
    }
    
    return SI_PM_OK;
}

int SIPMBlobClose(PMHANDLE_T *pmArg)
{
    SI_PM_handle_t *pm = (SI_PM_handle_t *)pmArg;
    
    switch(pm->storeType)
    {
        case SI_PM_SQLITE:
            if(pm->theBlob != NULL)
            {
                sqlite3_blob_close(pm->theBlob);
                pm->theBlob = NULL;
            }
            break;
            
        case SI_PM_FLASH:
            msync(pm->currentFBlock, pm->blobSize+sizeof(SI_PM_flash_block_t), MS_SYNC);
            si_pm_post(pm);
            si_pm_close_sem(pm);
            pm->currentFBlock = NULL;
            break;
        default:
            si_pm_errhandler("Error: Unknown back-end specified");
            return SI_PM_BBE;
    }
    
    return SI_PM_OK;
}

/* 
 * Fetches the a part of the requested data from the data store.
 * Specify the start of the part of the data to be read, and how much to read.
 */
int SIPMBlobRead(PMHANDLE_T *pmArg, unsigned char *data, int offset, int length)
{
    SI_PM_handle_t *pm = (SI_PM_handle_t *)pmArg;
    int result;
    int i;
    unsigned char *storedData;
    
    if(pm == NULL)
    {
        si_pm_errhandler("ERROR: SI_PM_handle_t is NULL");
        return SI_PM_UE;
    }
    
    /* length == 0 means that the whole value should be read */
    if(length == 0)
    {
        length = pm->blobSize;
        offset = 0;
    }
    
    switch(pm->storeType)
    {
        case SI_PM_SQLITE:
            if(pm->theBlob == NULL)
            {
                si_pm_errhandler("ERROR: You must open the blob before reading from it");
                return SI_PM_UE;
            }
            
            result = sqlite3_blob_read(pm->theBlob, (void *)data, length, offset);
            if(result != SQLITE_OK)
            {
                si_pm_errhandler("SQL Error: %s", sqlite3_errmsg(pm->db));
                return SI_PM_UE;
            }
            break;
        
        case SI_PM_FLASH:
            if((offset + length) > pm->currentFBlock->size)
            {
                return SI_PM_OOR;
            }
            storedData = (void *)pm->currentFBlock+sizeof(SI_PM_flash_block_t);
            for(i=offset; i<(offset+length); i++)
            {
                *data = *storedData;
                storedData++;
                data++;
            }
            break;
        default:
            si_pm_errhandler("Error: Unknown back-end specified");
            return SI_PM_BBE;
    }
    
    
    
    return SI_PM_OK;
}

/* 
 * Stores the specified data to a specific part of a stored value
 * Specify the start of the part of the data to be read, and how much to read.
 */
int SIPMBlobWrite(PMHANDLE_T *pmArg, unsigned char *data, int offset, int length)
{
    SI_PM_handle_t *pm = (SI_PM_handle_t *)pmArg;
    int result;
    int i;
    unsigned char *storedData;
    
    if(pm == NULL)
    {
        si_pm_errhandler("ERROR: SI_PM_handle_t is NULL");
        return SI_PM_UE;
    }
    
    
    /* length == 0 means that the whole value should be read */
    if(length == 0)
    {
        length = pm->blobSize;
        offset = 0;
    }
    
    switch(pm->storeType)
    {
        case SI_PM_SQLITE:
            if(pm->theBlob == NULL)
            {
                si_pm_errhandler("ERROR: You must open the blob before reading from it");
                return SI_PM_UE;
            }
            
            result = sqlite3_blob_write(pm->theBlob, (void *)data, length, offset);
            if(result != SQLITE_OK)
            {
                si_pm_errhandler("SQL Error: %s", sqlite3_errmsg(pm->db));
                return SI_PM_UE;
            }
            break;
            
        case SI_PM_FLASH:
            if((offset + length) > pm->currentFBlock->size)
            {
                return SI_PM_OOR;
            }
            storedData = (unsigned char *)pm->currentFBlock+sizeof(SI_PM_flash_block_t);
            for(i=offset; i<offset+length; i++)
            {
                *storedData = *data;
                data++;
                storedData++;
            }
            break;
        default:
            si_pm_errhandler("Error: Unknown back-end specified");
            return SI_PM_BBE;
    }
    
    return SI_PM_OK;
}

/* Remove the specified key from the data store */
int SIPMRemove(PMHANDLE_T *pmArg, const char *keyStr)
{
    SI_PM_handle_t *pm = (SI_PM_handle_t *)pmArg;
    SI_PM_flash_block_t *fBlock;
    int result;
    char tempStr[SI_PM_MAX_LENGTH+STRBUFF];
    char *zErrMsg;
    
    if(pm == NULL)
    {
        si_pm_errhandler("ERROR: SI_PM_handle_t is NULL");
        return SI_PM_UE;
    }
    
    switch(pm->storeType)
    {
        case SI_PM_SQLITE:
            sprintf(tempStr, "DELETE FROM '%s' WHERE key='%s';",pm->appName, keyStr);
            result = sqlite3_exec(pm->db, tempStr, 0, 0, &zErrMsg);
            if(result != SQLITE_OK)
            {
                si_pm_errhandler("SQL Error: %s", zErrMsg);
                sqlite3_free(zErrMsg);
                return SI_PM_UE;
            }
            break;
        case SI_PM_FLASH:
            /* Try to find the key */
            fBlock = pm->startFBlock;
            while((result=strcmp(fBlock->key, keyStr)) && !fBlock->lastBlock)
            {
                fBlock += fBlock->size + sizeof(SI_PM_flash_block_t);
            }
            /* If the key was found */
            if(result == 0)
            {
                fBlock->isFree = 1;
                fBlock->key[0] = '\0';
            }
            else
            {
                return SI_PM_KNF;
            }
            break;
        default:
            si_pm_errhandler("Error: Unknown back-end specified");
            return SI_PM_BBE;
    }
    
    return SI_PM_OK;
}
