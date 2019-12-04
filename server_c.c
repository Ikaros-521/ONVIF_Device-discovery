/*************************************************************************
    > File Name: ipc_server_c.c
  > Author: hlx
  > Mail: 327209194@qq.com 
  > Created Time: 2019年12月02日 星期一 14时11分59秒
 ************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <netdb.h>
//#include <errno.h>
//#include <uuid/uuid.h>
 
#define BUFLEN 4095
 
int main (int argc, char **argv) 
{
    char uu_buf[1024]={0};
    char *cust_uuid = 0;
	
    struct sockaddr_in groupcast_addr,the_member;
    int sockfd; 
    unsigned char loop;
    char recmsg[BUFLEN + 1]; 
    unsigned int socklen, n; 
    struct ip_mreq mreq; 
#if 0
    char *msg="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:wsdd=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:chan=\"http://schemas.microsoft.com/ws/2005/02/duplex\" xmlns:wsa5=\"http://www.w3.org/2005/08/addressing\" xmlns:xmime=\"http://tempuri.org/xmime.xsd\" xmlns:xop=\"http://www.w3.org/2004/08/xop/include\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:wsrfbf=\"http://docs.oasis-open.org/wsrf/bf-2\" xmlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\" xmlns:wsrfr=\"http://docs.oasis-open.org/wsrf/r-2\" xmlns:tdn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" xmlns:tev=\"http://www.onvif.org/ver10/events/wsdl\" xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" xmlns:tptz=\"http://www.onvif.org/ver20/ptz/wsdl\" xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\">\n\
<SOAP-ENV:Header>\n\
<wsa:MessageID>uuid:2419d68a-2dd2-21b2-a205-4A69A95DB56D</wsa:MessageID>\n\
<wsa:RelatesTo>uuid:251984b5-d773-4d2e-a21d-d248cdd2eebf</wsa:RelatesTo>\n\
<wsa:To SOAP-ENV:mustUnderstand=\"true\">http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</wsa:To>\n\
<wsa:Action SOAP-ENV:mustUnderstand=\"true\">http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches</wsa:Action>\n\
</SOAP-ENV:Header>\n\
<SOAP-ENV:Body>\n\
<wsdd:ProbeMatches>\n\
<wsdd:ProbeMatch>\n\
<wsa:EndpointReference>\n\
<wsa:Address>urn:uuid:2419d68a-2dd2-21b2-a205-4A69A95DB56D</wsa:Address>\n\
<wsa:ReferenceProperties />\n\
<wsa:ReferenceParameters />\n\
<wsa:PortType>ttl</wsa:PortType>\n\
</wsa:EndpointReference>\n\
<wsdd:Types>tds:Device</wsdd:Types>\n\
<wsdd:Scopes>onvif://www.onvif.org/type/NetworkVideoTransmitter\r\nonvif://www.onvif.org/name/IPC_2802222\r\nonvif://www.onvif.org/location/Country/China</wsdd:Scopes>\n\
<wsdd:XAddrs>http://192.168.42.103:5000/onvif/device_service</wsdd:XAddrs>\n\
<wsdd:MetadataVersion>1</wsdd:MetadataVersion>\n\</wsdd:ProbeMatch>\n\
</wsdd:ProbeMatches>\n\
</SOAP-ENV:Body>\n\
</SOAP-ENV:Envelope>\n\n";
#endif
 
    char *aa= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:wsdd=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:chan=\"http://schemas.microsoft.com/ws/2005/02/duplex\" xmlns:wsa5=\"http://www.w3.org/2005/08/addressing\" xmlns:xmime=\"http://tempuri.org/xmime.xsd\" xmlns:xop=\"http://www.w3.org/2004/08/xop/include\" xmlns:tt=\"http://www.onvif.org/ver10/schema\" xmlns:wsrfbf=\"http://docs.oasis-open.org/wsrf/bf-2\" xmlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\" xmlns:wsrfr=\"http://docs.oasis-open.org/wsrf/r-2\" xmlns:tdn=\"http://www.onvif.org/ver10/network/wsdl\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\" xmlns:tev=\"http://www.onvif.org/ver10/events/wsdl\" xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\" xmlns:tptz=\"http://www.onvif.org/ver20/ptz/wsdl\" xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\">\n\
<SOAP-ENV:Header>\n\
<wsa:MessageID>uuid:2419d68a-2dd2-21b2-a205-4A69A95DB56D</wsa:MessageID>\n\
<wsa:RelatesTo>uuid:";
 
    char *bb = "</wsa:RelatesTo>\n\
<wsa:To SOAP-ENV:mustUnderstand=\"true\">http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</wsa:To>\n\
<wsa:Action SOAP-ENV:mustUnderstand=\"true\">http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches</wsa:Action>\n\
</SOAP-ENV:Header>\n\
<SOAP-ENV:Body>\n\
<wsdd:ProbeMatches>\n\
<wsdd:ProbeMatch>\n\
<wsa:EndpointReference>\n\
<wsa:Address>urn:uuid:2419d68a-2dd2-21b2-a205-4A69A95DB56D</wsa:Address>\n\
<wsa:ReferenceProperties />\n\
<wsa:ReferenceParameters />\n\
<wsa:PortType>ttl</wsa:PortType>\n\
</wsa:EndpointReference>\n\
<wsdd:Types>tds:Device</wsdd:Types>\n\
<wsdd:Scopes>onvif://www.onvif.org/type/NetworkVideoTransmitter\r\nonvif://www.onvif.org/name/IPC_2802222\r\nonvif://www.onvif.org/location/Country/China</wsdd:Scopes>\n\
<wsdd:XAddrs>http://192.168.42.100:5000/onvif/device_service</wsdd:XAddrs>\n\
<wsdd:MetadataVersion>1</wsdd:MetadataVersion>\n\</wsdd:ProbeMatch>\n\
</wsdd:ProbeMatches>\n\
</SOAP-ENV:Body>\n\
</SOAP-ENV:Envelope>\n\n";
 
    /* 创建 socket 用于UDP通讯 */ 
    sockfd = socket (AF_INET, SOCK_DGRAM, 0); 
    if (sockfd < 0)
    {          
        printf ("socket creating err in udptalk\n");          
        exit (1);        
    } 
    /* 设置要加入组播的地址 */ 
    bzero(&mreq, sizeof (struct ip_mreq));
    
    inet_pton(AF_INET,"239.255.255.250",&the_member.sin_addr);
    /* 设置组地址 */ 
    bcopy (&the_member.sin_addr.s_addr, &mreq.imr_multiaddr.s_addr, sizeof (struct in_addr)); 
    /* 设置发送组播消息的源主机的地址信息 */ 
    mreq.imr_interface.s_addr = htonl (INADDR_ANY);  
    /* 把本机加入组播地址，即本机网卡作为组播成员，只有加入组才能收到组播消息 */ 
    //if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP | IP_MULTICAST_LOOP, &mreq,sizeof (struct ip_mreq)) == -1)
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP , &mreq,sizeof (struct ip_mreq)) == -1)
    {
        perror ("setsockopt");
        exit (-1);   
    }
	loop = 0;
    if (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop,sizeof (loop)) == -1)
    {
        printf("IP_MULTICAST_LOOP set fail!\n");
    }
	
    socklen = sizeof (struct sockaddr_in); 
    memset (&groupcast_addr, 0, socklen); 
    groupcast_addr.sin_family = AF_INET;
    groupcast_addr.sin_port = htons (3702);
    inet_pton(AF_INET, "239.255.255.250", &groupcast_addr.sin_addr); 
    /* 绑定自己的端口和IP信息到socket上 */ 
    if (bind(sockfd, (struct sockaddr *) &groupcast_addr,sizeof (struct sockaddr_in)) == -1)
    {      
        printf ("Bind error\n");      
        exit (0);    
    }
  
    while (1)
    {     
        bzero (recmsg, BUFLEN + 1); 
        n = recvfrom (sockfd, recmsg, BUFLEN, 0, (struct sockaddr *) &the_member, &socklen);
        if (n < 0)
        {      
            printf ("recvfrom err in udptalk!\n");      
            exit (4);    
        }
        else{      
            recmsg[n] = 0;      
            printf ("recv:[%s]\n\n", recmsg);    
            printf("ip:%s\n",inet_ntoa(the_member.sin_addr));
            printf("port:%d\n", ntohs(the_member.sin_port));
        }
		
        cust_uuid = strstr(recmsg, "uuid:"); //获取recmsg字符串中 子字符串"uuid:"的位置
        if (cust_uuid == 0)
        {
            printf("uuid: err!\n");
            return 0;
        }
        cust_uuid += 5;   //获取接收的uuid的值
        strncpy(uu_buf, cust_uuid, 36);
        printf("%s\n",uu_buf);
		
        memset(recmsg,0,sizeof(recmsg));
        strcpy(recmsg, aa);
        strcat(recmsg, uu_buf);
        strcat(recmsg, bb);
        if (sendto(sockfd, recmsg, strlen (recmsg), 0, (struct sockaddr *) &the_member, sizeof (the_member)) < 0)
        {      
            printf ("sendto error!\n");
            exit (3);
        }
        printf ("send ok\n");
        break;
    }
}
