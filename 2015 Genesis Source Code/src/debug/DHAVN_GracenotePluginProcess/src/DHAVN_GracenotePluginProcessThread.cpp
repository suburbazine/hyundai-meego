
/*
 ******************************************************************
 *    COPYRIGHT ?Teleca AB      *
 *----------------------------------------------------------------*
 * MODULE     	:  CGracenoteInterface                            	  *
 *
 * PROGRAMMERS 	:  GADDAM SATEESH
 * DATE       	:  27 April 2012                             	  *
 * VERSION    	:                                             	  *
 *
 *----------------------------------------------------------------*
 *                                                                *
 *  MODULE SUMMARY : Makes the CQueue interface to be in a thread so
 *  that all the requests of Juke,USB,IPOD will be dequeued in prority
 *  base
 *----------------------------------------------------------------*
  ******************************************************************
 */
#include "DHAVN_GracenotePluginProcessThread.h"


/*
 *******************************************************************
 * Function	: GNThread::run()
 *
 * Description 	: starts the threads event loop

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
void GNThread::run()
{

    exec();

}

//End of file
