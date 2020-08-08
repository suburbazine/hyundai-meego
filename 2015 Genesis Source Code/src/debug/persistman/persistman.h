/*
* persistman.h, The header file of the persistence manager API
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
 * \file persistman.h
 * \brief The header file of the persistence manager API
 * \version $Revision$
 * \date    $Date$
 */

/*! \mainpage Persistence Manager
 *
 * \section intro_sec Introduction
 *
 * The persistence manager (PM) is an abstraction layer between applications
 * and the storage of their data that is required to persist across sessions.
 * For example, if a user has the radio tuned to a particular station, and
 * turns the radio off then on again, the radio should resume playing the
 * same station. The radio application stores and retrieves which station
 * to play on resume using the persistence manager.
 *
 * The persistence manager consists of shared libary that users link to.
 * the library provides an API that interfaces to the configured backend
 * storage mechanism, which may be a simple file, or an SQL database.
 *
 * When a application starts up it must first call the SIPMOpen() function
 * in the API.  This sets the configurations settings for the session.
 * This sets which type of back-end is used, where the data store is located,
 * and the name of the calling app.

 * The application will then make a call to SIPMGet() to retrieve data that
 * may exist from a past session. The API will check the data store to see if
 * the requested key exists. If the key and data exist, then the data is passed
 * to the calling application. The function will then return ‘SI_PM_OK’
 * signifying success. If the requested key and data do not exist, then the API
 * function will return a number greater than 0, the specific number returned
 * represents a specific error.  See list of error codes below.
 *
 * 
 * \section components_sec Subsections
 *  \li \ref persistman_configuration
 *  \li \ref persistman_examples
 *  \li \ref persistman_api
 */

/** 
 * \defgroup persistman_examples Persistence Manager Examples

\section example1 Example 1: Getting values
 
If the application wishes to read a value from the Persistence Manager, it
calls SIPMGet(). It passes the key, data, and the number of bytes of data as
parameters.

 * \code

#include “persistman.h”

struct blob {
   int aninteger;
   char astring[256];
   double adouble;
};

int main() {
   int result, blobSize;
   struct blob *aBlob;
   PMHANDLE_T *pm1;
   char blobKey = “key1”;

   SIPMOpen(&pm1, SI_PM_SQLITE, “/var/lib/si_pm.db”, “app_name”, 0 , 0);
   result = SIPMGet(pm1, blobKey, aBlob, &blobSize);
   if(result == SI_PM_OK)
      printf(“An integer: %d\n”, glob1->aninteger);
   else if(result == SI_PM_KNF)
      printf(“Key not found in data store\n”);
   else
      printf(“Error: Could not retrieve requested data\n”);
   SIPMClose(pm1);
}

 * \endcode


\section example2 Example 2: Setting values

If the application wishes to record a change to a value in the Persistence
Manager, it calls SIPMSet(). It passes the key, data, and the number of bytes
of data as parameters.

 * \code

#include “persistman.h”

struct blob {
   int aninteger;
   char astring[256];
   double adouble;
};

int main() {
   int result, blobSize;
   struct blob aBlob = {25, “hello”, 10.0};
   PMHANDLE_T *pm1;
   char blobKey = “key1”;

   SIPMOpen(&pm1, SI_PM_SQLITE, “/var/lib/si_pm.db”, “app_name”, 0 , 0);
   result = SIPMSet(pm1, blobKey, &aBlob, sizeof(aBlob));
   if(result == SI_PM_OK)
      printf(“The blob was stored successfully\n”);
   else
      printf(“Error: Could not store data\n”);
   SIPMClose(pm1);
} 

 * \endcode
 */

/** 
 * \defgroup persistman_configuration Persistence Manager Configuration
 *
 * The Persistence Manager is contained in a single package, persistencemanager-<ver>.
 * The package will install the following files into the target file system:

 * \li /usr/lib/libpersistman.so - Persistence Manager API library
 * \li /usr/include/persistman.h - Persistence Manager API header
 * \li /var/lib/persistmand/SI_PM.db – Recommend location for the Persistence Manager SQLite database
 *
 * The configuration for the persistence manager’s behaviour is set by the calling
 * application. The call to SIPMOpen() sets the back-end, the location of the data
 * store, and other parameters.
 * SQLite’s database uses the following schema (the data type is in the square brackets):
 * 	\li key [TEXT, UTF-8] = This text string should be unique for the variable being accessed.
 * 	\li value [BLOB] = This contains the stored value, unaltered.
 */

/** 
 * \defgroup persistman_api Persistence Manager API
 *
 * The API provided by the Persistence Manager shared library is
 * documented below.
 * 
 */ /** @{ */


#ifndef _PERSISTMAN_H
#define _PERSISTMAN_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************/
/*                              Includes                                  */
/**************************************************************************/

/**************************************************************************/
/*                               Defines                                  */
/**************************************************************************/
/* Error codes */
#define SI_PM_OK      0  /**< Successful result             */
#define SI_PM_BBE     1  /**< Error: Bad Back-End           */
#define SI_PM_BDS     2  /**< Error: Bad Data Store         */
#define SI_PM_KNF     3  /**< Error: Key Not Found          */
#define SI_PM_NMS     4  /**< Error: No More Space          */
#define SI_PM_DSTB    5  /**< Error: Data Size Too Big      */
#define SI_PM_OOR     6  /**< Error: Out Of Range           */
#define SI_PM_BLOCKED 7  /**< Error: Data store is blocked  */
#define SI_PM_UE      8  /**< Error: Unknown Error          */

/* Back-end */
#define SI_PM_SQLITE 0   /**< Specifies that the data store is SQLite based */
#define SI_PM_FLASH 1    /**< Specifies that the data store is flat memory  */

/* Init flags */
#define SI_PM_NB  1      /**< Specifies that blocking should be avoided */
#define SI_PM_SEM 2      /**< Specifies that semaphore locking should be used (only used for flash back-end) */

/* Some extra constants */
#define SI_PM_MAX_LENGTH 50 /**< Max length for any string passed to the persistence manager */
#define SI_PM_READ 0        /**< Specifies a read operation                                  */
#define SI_PM_WRITE 1       /**< Specifies a write operation                                 */

/**************************************************************************/
/*                              Typedefs                                  */
/**************************************************************************/
typedef void PMHANDLE_T;  /**< Opaque handle type that is used to identify a session */

/** \brief Initializes the persistence manager session
 *  
 *  \param pm1 [out] Contains the information required to access a given data store during a session
 *  \param backEnd [in] Specifies the data store mechanism
 *  \param location [in] Specifies the file/device location of the data store
 *  \param appName [in] The name of the calling app, used to make unique keys in the data store
 *  \param flags [in] Specifies options such as non-blocking
 *  \param maxSize [out] The maximum size of the data store, if 0, the data store will have no specified limit
 *  \return SI_PM_OK on success, another value representing the error on fail
 */
int SIPMOpen(PMHANDLE_T **pm1, int backEnd, const char *location, const char *appName, int flags, int maxSize);

/** \brief Closes the specified persistence manager connection
 *  
 *  \param pm1 [in] Contains the information required to access a given data store during a session
 *  \return SI_PM_OK on success, another value representing the error on fail
 */
int SIPMClose(PMHANDLE_T *pm1);

/** \brief Saves data into the data store using the Storage Key
 *  
 *  \param pm1 [in] Contains the information required to access a given data store during a session
 *  \param keyStr [in] Specified key to be looked up
 *  \param data [in] The location of the data to be stored
 *  \param dataSize [in] The number of bytes to be stored
 *  \return SI_PM_OK on success, another value representing the error on fail
 */
int SIPMSet(PMHANDLE_T *pm1, const char *keyStr, unsigned char *data, int dataSize);

/** \brief Fetches the requested data from the data store
 *  
 *  \param pm1 [in] Contains the information required to access a given data store during a session
 *  \param keyStr [in] Specified key to be looked up
 *  \param data [in] The location for the retrieved data to be stored
 *  \param dataSize [in] The number of bytes that have been retrieved
 *  \return SI_PM_OK on success, another value representing the error on fail
 */
int SIPMGet(PMHANDLE_T *pm1, const char *keyStr, unsigned char *data, int *dataSize);

/** \brief Remove the specified key from the data store
 *  
 *  \param pm1 [in] Contains the information required to access a given data store during a session
 *  \param keyStr [in] Specified key to be looked up
 *  \return SI_PM_OK on success, another value representing the error on fail
*/
int SIPMRemove(PMHANDLE_T *pm1, const char *keyStr);

/** \brief Opens a value in the data store to be read/written to
 *  
 *  \param pm1 [in] Contains the information required to access a given data store during a session
 *  \param keyStr [in] Specified key to be looked up
 *  \param rwFlag [in] Specifies if the blob is to be read from or written to, 0 to read, 1 to write
 *  \param dataSize [out] Returns with the size of the blob that has been opene
 *  \return SI_PM_OK on success, another value representing the error on faild
 */
int SIPMBlobOpen(PMHANDLE_T *pm1, const char *keyStr, int rwFlag, int *dataSize);

/** \brief Closes the connection to the data store, frees up all resources used
 *  
 *  \param pm1 [in] Contains the information required to access a given data store during a session
 *  \return SI_PM_OK on success, another value representing the error on fail
 */
int SIPMBlobClose(PMHANDLE_T *pm1);

/** \brief Fetches the a part of the requested data from the data store.
 *  
 *  \param pm1 [in] Contains the information required to access a given data store during a session
 *  \param data [out] The location for the retrieved data to be stored
 *  \param offset [in] The number of bytes that should be skipped from the start of the data
 *  \param length [in] The number of bytes that should be read, a value of '0' means that the whole value should be read
 *  \return SI_PM_OK on success, another value representing the error on fail
 */
int SIPMBlobRead(PMHANDLE_T *pm1, unsigned char *data, int offset, int length);

/** \brief Stores the specified data to a specific part of a stored value
 *  
 *  \param pm1 [in] Contains the information required to access a given data store during a session
 *  \param data [in] The location of the data to be stored
 *  \param offset [in] The number of bytes that should be skipped from the start of the data in the data store
 *  \param length [in] The number of bytes that are to be written
 *  \return SI_PM_OK on success, another value representing the error on fail
 */
int SIPMBlobWrite(PMHANDLE_T *pm1, unsigned char *data, int offset, int length);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _PERSISTMAN_H */
/** @} end doxygen grouping */
