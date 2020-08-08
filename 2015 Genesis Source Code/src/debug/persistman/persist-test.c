/*
* persist-test.c, Test program for Persistence Manager
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <semaphore.h>

#include "persistman.h"

int testrun(char *appname, int backend)
{
    int result;
    
    char *key1 = "key1";
    char *key2 = "key2";
    char tempkey[50];
    int data1;
    int data2;
    int size,fd, i;
    
    PMHANDLE_T *pm1;
    char location[SI_PM_MAX_LENGTH];
    
    switch(backend)
    {
        case SI_PM_SQLITE:
            strcpy(location, "/tmp/persistman.db");
            size = 0;
            break;
        case SI_PM_FLASH:
            size = 300000;
            strcpy(location, "/tmp/persistman.flash");
            if((fd = open(location, O_CREAT|O_RDWR, 0666))==-1)
                printf("%s\n", strerror(errno));
            if(ftruncate(fd, size) == -1)
                printf("%s\n", strerror(errno));
            close(fd);
            break;
    }
    
    result = SIPMOpen(&pm1, backend, location, appname, 0, size);
    if(result != SI_PM_OK)
    {
        printf("Error initializing persistman data store: %d\n",result);
        return 1;
    }
    
    /* Try to retrieve a non-existent value */
    result = SIPMGet(pm1, key2, (unsigned char *)&data1, &size);
    if(result == SI_PM_KNF)
    {
        
    } else if(data1 != 0)
    {
        printf("Error: A value (%X) was returned when none was expected\n", data1);
        return 1;
    } else
    {
        printf("Error occurred when trying to get value from persistman: %d\n",result);
        return 1;
    } 
    
    data2 = 0;
    /* Try to store values */
    for(i=0; i<1024; i++) {
        data2++;
        sprintf(tempkey, "%s_%d", key1, i);
        if(SIPMSet(pm1, tempkey, (unsigned char *)&data2, sizeof(int))!=0)
        {
            printf("Error: Could not store value %s\n", tempkey);
            return 1;
        }
    }
    
    
    data2 = 0;
    /* Try to retrieve values */
    for(i=0; i<1024; i++) {
        data2++;
        sprintf(tempkey, "%s_%d", key1, i);
        if(SIPMGet(pm1, tempkey, (unsigned char *)&data1, &size)!=0)
        {
            printf("Error occurred when trying to get value (key: %s)\n", tempkey);
            return 1;
        } else if(data1 != data2) {
            printf("Error: The retrieved value (%d) is different from the stored value (%d)\n", data1, data2);
            return 1;
        }
    }

    
    if(SIPMClose(pm1) != 0)
    {
        printf("Error closing the data store\n");
        return 1;
    }
    
    
    return 0;
  
}
int main (int argc, char *argv[])
{
    if(testrun(argv[0], SI_PM_SQLITE))
        printf("[FAIL] SQLite testing\n");
    else
        printf("[PASS] SQLite testing passed\n");
    
    if(testrun(argv[0],SI_PM_FLASH))
        printf("[FAIL] Straight to flash testing\n");
    else
        printf("[PASS] Straight to flash testing\n");
            
    return 0;
}
