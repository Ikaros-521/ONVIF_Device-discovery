#include "wsdd.h"
#include <stdio.h>
 
static struct soap* ONVIF_Initsoap(struct SOAP_ENV__Header *header, const char *was_To, const char *was_Action, int timeout)
{
	struct soap *soap = NULL; 
	unsigned char macaddr[6];
	char _HwId[1024];
	unsigned int Flagrand;
	soap = soap_new();
	if(soap == NULL)
	{
		printf("[%d]soap = NULL\n", __LINE__);
		return NULL;
	}
	
	soap_set_namespaces( soap, namespaces);
 
	if (timeout > 0)
	{
		soap->recv_timeout = timeout;
		soap->send_timeout = timeout;
		soap->connect_timeout = timeout;
	}
	else
	{
		//Maximum wait time: 20S 
		soap->recv_timeout    = 20;
		soap->send_timeout    = 20;
		soap->connect_timeout = 20;
	}
	soap_default_SOAP_ENV__Header(soap, header);
 
	// Create SessionID randomly
	srand((int)time(0));
	Flagrand = rand()%9000 + 8888; 
	macaddr[0] = 0x1; macaddr[1] = 0x2; macaddr[2] = 0x3; macaddr[3] = 0x4; macaddr[4] = 0x5; macaddr[5] = 0x6;
	sprintf(_HwId,"urn:uuid:%ud68a-1dd2-11b2-a105-%02X%02X%02X%02X%02X%02X", 
			Flagrand, macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
	header->wsa__MessageID =(char *)malloc( 100);
	memset(header->wsa__MessageID, 0, 100);
	strncpy(header->wsa__MessageID, _HwId, strlen(_HwId));
 
	if (was_Action != NULL)
	{
		header->wsa__Action =(char *)malloc(1024);
		memset(header->wsa__Action, '\0', 1024);
		strncpy(header->wsa__Action, was_Action, 1024);//"http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";
	}
	if (was_To != NULL)
	{
		header->wsa__To =(char *)malloc(1024);
		memset(header->wsa__To, '\0', 1024);
		strncpy(header->wsa__To,  was_To, 1024);//"urn:schemas-xmlsoap-org:ws:2005:04:discovery";	
	}
	soap->header = header;
	return soap;
} 
 
int ONVIF_ClientDiscovery( )
{
	int FoundDevNo = 0;
	int retval = SOAP_OK;
	wsdd__ProbeType req;       
	struct __wsdd__ProbeMatches resp;
	wsdd__ScopesType sScope;
	struct SOAP_ENV__Header header;
	struct soap* soap;
	
 
	const char *was_To = "urn:schemas-xmlsoap-org:ws:2005:04:discovery";
	const char *was_Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";
	//IP Adress and PortNo, broadCast	
	const char *soap_endpoint = "soap.udp://239.255.255.250:3702/";
 
	//Create new soap object with info
	soap = ONVIF_Initsoap(&header, was_To, was_Action, 10);
	
	soap_default_SOAP_ENV__Header(soap, &header);
	soap->header = &header;
 
	soap_default_wsdd__ScopesType(soap, &sScope);
	sScope.__item = "";
	soap_default_wsdd__ProbeType(soap, &req);
	req.Scopes = &sScope;
	req.Types = ""; //"dn:NetworkVideoTransmitter";
    
	//sent the message broadcast and wait
	retval = soap_send___wsdd__Probe(soap, soap_endpoint, NULL, &req);			
	while (retval == SOAP_OK)
    {
		retval = soap_recv___wsdd__ProbeMatches(soap, &resp);
        if (retval == SOAP_OK) 
        {
            if (soap->error)
            {
                printf("[%d]: recv soap error :%d, %s, %s\n", __LINE__, soap->error, *soap_faultcode(soap), *soap_faultstring(soap)); 
			    retval = soap->error;
            }
            else //we find a device
			{
				FoundDevNo ++;
				if (resp.wsdd__ProbeMatches->ProbeMatch != NULL && resp.wsdd__ProbeMatches->ProbeMatch->XAddrs != NULL)
				{
					printf("****** No %d Devices Information ******\n", FoundDevNo );
					printf("Device Service Address  : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->XAddrs);	
					printf("Device EP Address       : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->wsa__EndpointReference.Address);  
					printf("Device Type             : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->Types);  
					printf("Device Metadata Version : %d\r\n", resp.wsdd__ProbeMatches->ProbeMatch->MetadataVersion);  
					//sleep(1);
				}
			}
		}
		else if (soap->error)  
		{  
			if (FoundDevNo == 0)
			{
				printf("No Device found!\n"); 
				retval = soap->error;  
			}
			else 
			{
				printf("Search end! Find %d Device! \n", FoundDevNo);
				retval = 0;
			}
			break;
		}  
    }
 
    soap_destroy(soap); 
    soap_end(soap); 
    soap_free(soap);
	
	return retval;
}
 
int main(void )
{
 
	if (ONVIF_ClientDiscovery() != 0 )
	{
		printf("discovery failed!\n");
		return -1;
	}
 
	return 0;
}
