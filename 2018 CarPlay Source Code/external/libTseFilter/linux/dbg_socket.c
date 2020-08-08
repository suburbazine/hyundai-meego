/*
 *    @file        <Dbg_socket.h>
 *    @brief       TCP socket communication API of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */


#include "dbg_socket.h"

int DbgSocketStartService(void)
{
  return 0;
}

int DbgSocketStopService(void)
{
   return 0;
}

/** @brief creates an endpoint for communication and returns a descriptor
 *
 *  @param af communication domain.
 *  @param type socket type.
 *  @param protocol specifies a particular protocol to be used with the socket.
 *  @return On success, a file descriptor for the new socket is returned. On error, -1 is returned, and errno is set appropriately.
 *  @remark 주의사항 없음
 */
tDBG_SOCKET DbgSocketCreate( int af , int type, int protocol )
{
  return socket( af, type, protocol );
}

/** @brief close socket
 *
 *  @param hSocket file descriptor for the socket.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int DbgSocketClose( tDBG_SOCKET hSocket )
{
  return close( hSocket );
}

/** @brief shut down part of a full-duplex connection
 *
 *  @param hSocket file descriptor for the socket.
 *  @param how SHUT_RD, further receptions will be disallowed, SHUT_WR further transmissions will be disallowed, SHUT_RDWR further receptions and transmissions will be disallowed.
 *  @return On success, zero is returned. On error, -1 is returned, and errno is set appropriately.
 *  @remark 주의사항 없음
 */
int DbgSocketShutdown( tDBG_SOCKET hSocket, int how )
{
  return shutdown( hSocket, how );
}

/** @brief set options on sockets
 *
 *  @param hSocket file descriptor for the socket.
 *  @param level socket level.
 *  @param optname socket option name.
 *  @param optval socket option value.
 *  @param optlen socket option length.
 *  @return On success, zero is returned. On error, -1 is returned, and errno is set appropriately.
 *  @remark 주의사항 없음
 */
int DbgSocketSetSockOpt( tDBG_SOCKET hSocket, int level, int optname,  const void *optval, tDBG_SOCKLEN optlen )
{
  return setsockopt( hSocket, level, optname, optval, optlen );
}

/** @brief accept a connection on a socket
 *
 *  @param hSocket file descriptor for the socket.
 *  @param addr socket address.
 *  @param addrlen socket address length.
 *  @return On success, zero is returned. On error, -1 is returned, and errno is set appropriately.
 *  @remark 주의사항 없음
 */
tDBG_SOCKET DbgSocketAccept( tDBG_SOCKET hSocket, struct sDBG_SOCKADDR *addr, tDBG_SOCKLEN *addrlen)
{
  return accept( hSocket, addr, addrlen);
}

/** @brief send a message on a socket
 *
 *  @param hSocket file descriptor for the socket.
 *  @param buf data buffer to send.
 *  @param len data length.
 *  @param flag flags on sending message.
 *  @return On success, zero is returned. On error, -1 is returned, and errno is set appropriately.
 *  @remark 주의사항 없음
 */
int DbgSocketSend( tDBG_SOCKET hSocket, const char *buf, int len, int flag )
{
  return send( hSocket, buf, len, flag );
}

/** @brief listen for connections on a socket
 *
 *  @param hSocket file descriptor for the socket.
 *  @param backlog defines the maximum length to which the queue of pending connections for sockfd may grow.
 *  @return On success, zero is returned. On error, -1 is returned, and errno is set appropriately.
 *  @remark 주의사항 없음
 */
int DbgSocketListen( tDBG_SOCKET hSocket, int backlog )
{
  return listen( hSocket, backlog );
}

/** @brief receive a message on a socket
 *
 *  @param hSocket file descriptor for the socket.
 *  @param buf received data buffer.
 *  @param len received data length.
 *  @param flag flags on received message.
 *  @return On success, zero is returned. On error, -1 is returned, and errno is set appropriately.
 *  @remark 주의사항 없음
 */
int DbgSocketRecv( tDBG_SOCKET hSocket, const char *buf, int len, int flag )
{
  return recv( hSocket, (char *)buf, len, flag );
}

/** @brief bind a name to a socket
 *
 *  @param hSocket file descriptor for the socket.
 *  @param name socket address.
 *  @param namelen socket address length.
 *  @return On success, zero is returned. On error, -1 is returned, and errno is set appropriately.
 *  @remark 주의사항 없음
 */
int  DbgSocketBind( tDBG_SOCKET hSocket, struct sDBG_SOCKADDR *name, int namelen)
{
  return bind( hSocket, name, namelen);
}

/** @brief get protoent structure for the entry from the database that matches the protocol name name
 *
 *  @param name socket address.
 *  @return a protoent structure.
 *  @remark 주의사항 없음
 */
struct sDBG_PROTO * DbgSocketGetProtoByName(const char* name)
{

  return getprotobyname(name);
}

/** @brief converts the unsigned integer hostlong from host byte order to network byte order
 *
 *  @param iPort port number.
 *  @return converted port number.
 *  @remark 주의사항 없음
 */
u_short Dbghtons ( unsigned short iPort)
{
  return htons (iPort);
}
