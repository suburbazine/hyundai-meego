/*
 *    @file        <Dbg_socket.h>
 *    @brief       TCP socket communication API header of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */


#ifndef __DBG_SOCKET__
#define __DBG_SOCKET__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define iDBG_INVALID_SOCKET    -1

#define iDBG_SOCKET_SHUT_RD    0 /*SHUT_RD*/
#define iDBG_SOCKET_SHUT_WR    1 /*SHUT_WR*/
#define iDBG_SOCKET_SHUT_RDWR  2 /*SHUT_RDWR*/

#define DBG_AF_INET           AF_INET
#define DBG_INADDR_ANY        INADDR_ANY
#define DBG_SOCK_STREAM       SOCK_STREAM
#define DBG_SOL_SOCKET        SOL_SOCKET
#define DBG_SO_REUSEADDR      SO_REUSEADDR

#define tDBG_SOCKET           int
#define tDBG_SOCKLEN          socklen_t
#define sDBG_SOCKADDR         sockaddr
#define sDBG_SOCKADDR_IN      sockaddr_in
#define sDBG_PROTO            protoent

int DbgSocketStartService(void);
int DbgSocketStopService(void);

tDBG_SOCKET DbgSocketCreate( int af , int type, int protocol );
int DbgSocketClose( tDBG_SOCKET hSocket );

struct sDBG_PROTO * DbgSocketGetProtoByName(const char* name);
int DbgSocketBind( tDBG_SOCKET hSocket, struct sDBG_SOCKADDR *name, int namelen);

int DbgSocketShutdown( tDBG_SOCKET hSocket, int how );
int DbgSocketSetSockOpt( tDBG_SOCKET hSocket, int level, int optname,  const void *optval, tDBG_SOCKLEN optlen );

tDBG_SOCKET DbgSocketAccept( tDBG_SOCKET hSocket, struct sDBG_SOCKADDR *addr, tDBG_SOCKLEN *addrlen);

int DbgSocketListen( tDBG_SOCKET hSocket, int backlog );
int DbgSocketSend( tDBG_SOCKET hSocket, const char *buf, int len, int flag );
int DbgSocketRecv( tDBG_SOCKET hSocket, const char *buf, int len, int flag );

unsigned short Dbghtons ( unsigned short iPort);

#endif // __DbgSOCKET__
