/*************************************************************************
    > File Name: search_c.c
  > Author: hlx
  > Mail: 327209194@qq.com 
  > Created Time: 2019年12月02日 星期一 11时26分21秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#include <winsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

/* 从技术层面来说，通过单播、多播、广播三种方式都能探测到IPC，但多播最具实用性*/
#define COMM_TYPE_UNICAST         1                                             // 单播
#define COMM_TYPE_MULTICAST       2                                             // 多播
#define COMM_TYPE_BROADCAST       3                                             // 广播
#define COMM_TYPE                 COMM_TYPE_MULTICAST

/* 发送探测消息（Probe）的目标地址、端口号 */
#if COMM_TYPE == COMM_TYPE_UNICAST
    #define CAST_ADDR "100.100.100.15"                                          // 单播地址，预先知道的IPC地址
#elif COMM_TYPE == COMM_TYPE_MULTICAST
    #define CAST_ADDR "239.255.255.250"                                         // 多播地址，固定的239.255.255.250
#elif COMM_TYPE == COMM_TYPE_BROADCAST
    #define CAST_ADDR "100.100.100.255"                                         // 广播地址
#endif

#define CAST_PORT 3702                                                          // 端口号

/* 以下几个宏是为了socket编程能够跨平台，这几个宏是从gsoap中拷贝来的 */
#ifndef SOAP_SOCKET
# ifdef WIN32
#  define SOAP_SOCKET SOCKET
#  define soap_closesocket(n) closesocket(n)
# else
#  define SOAP_SOCKET int
#  define soap_closesocket(n) close(n)
# endif
#endif

#if defined(_AIX) || defined(AIX)
# if defined(_AIX43)
#  define SOAP_SOCKLEN_T socklen_t
# else
#  define SOAP_SOCKLEN_T int
# endif
#elif defined(SOCKLEN_T)
# define SOAP_SOCKLEN_T SOCKLEN_T
#elif defined(__socklen_t_defined) || defined(_SOCKLEN_T) || defined(CYGWIN) || defined(FREEBSD) || defined(__FreeBSD__) || defined(OPENBSD) || defined(__QNX__) || defined(QNX) || defined(OS390) || defined(__ANDROID__) || defined(_XOPEN_SOURCE)
# define SOAP_SOCKLEN_T socklen_t
#elif defined(IRIX) || defined(WIN32) || defined(__APPLE__) || defined(SUN_OS) || defined(OPENSERVER) || defined(TRU64) || defined(VXWORKS) || defined(HP_UX)
# define SOAP_SOCKLEN_T int
#elif !defined(SOAP_SOCKLEN_T)
# define SOAP_SOCKLEN_T size_t
#endif

#ifdef WIN32
#define SLEEP(n)    Sleep(1000 * (n))
#else
#define SLEEP(n)    sleep((n))
#endif

/* 探测消息(Probe)，这些内容是ONVIF Device Test Tool 15.06工具搜索IPC时的Probe消息，通过Wireshark抓包工具抓包到的 */
const char *probe = "<?xml version=\"1.0\" encoding=\"utf-8\"?><Envelope xmlns:dn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns=\"http://www.w3.org/2003/05/soap-envelope\"><Header><wsa:MessageID xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">uuid:fc0bad56-5f5a-47f3-8ae2-c94a4e907d70</wsa:MessageID><wsa:To xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">urn:schemas-xmlsoap-org:ws:2005:04:discovery</wsa:To><wsa:Action xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe</wsa:Action></Header><Body><Probe xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\"><Types>dn:NetworkVideoTransmitter</Types><Scopes /></Probe></Body></Envelope>";

int main(int argc, char **argv)
{
    int ret;
    int optval;
    SOAP_SOCKET s;
    SOAP_SOCKLEN_T len;
    char recv_buff[4096] = {0};
    struct sockaddr_in multi_addr;
    struct sockaddr_in client_addr;

#ifdef WIN32
    WSADATA wsaData;
    if( WSAStartup(MAKEWORD(2,2), &wsaData) != 0 ) {                             // 初始化Windows Sockets DLL
        printf("Could not open Windows connection.\n");
        return 0;
    }
    if ( LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2 ) {
        printf("the version of WinSock DLL is not 2.2.\n");
        return 0;
    }
#endif

    s = socket(AF_INET, SOCK_DGRAM, 0);                                         // 建立数据报套接字
    if (s < 0) {
        perror("socket error");
        return -1;
    }

#if COMM_TYPE == COMM_TYPE_BROADCAST
    optval = 1;
    ret = setsockopt(s, SOL_SOCKET, SO_BROADCAST, (const char*)&optval, sizeof(int));
#endif

    multi_addr.sin_family = AF_INET;                                            // 搜索IPC：使用UDP向指定地址发送探测消息(Probe)
    multi_addr.sin_port = htons(CAST_PORT);
    multi_addr.sin_addr.s_addr = inet_addr(CAST_ADDR);
    ret = sendto(s, probe, strlen(probe), 0, (struct sockaddr*)&multi_addr, sizeof(multi_addr));
    if (ret < 0) {
        soap_closesocket(s);
        perror("sendto error");
        return -1;
    }
    printf("Send Probe message to [%s:%d]\n\n", CAST_ADDR, CAST_PORT);
    SLEEP(1);

    for (;;) {                                                                  // 接收IPC的应答消息(ProbeMatch)
        len = sizeof(client_addr);
        memset(recv_buff, 0, sizeof(recv_buff));
        memset(&client_addr, 0, sizeof(struct sockaddr));
        ret = recvfrom(s, recv_buff, sizeof(recv_buff) - 1, 0, (struct sockaddr*)&client_addr, &len);
        printf("===Recv ProbeMatch from [%s:%d]===\n%s\n\n",  inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), recv_buff);
        SLEEP(1);
    }
    soap_closesocket(s);

    return 0;
}
