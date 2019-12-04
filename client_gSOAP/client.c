#include <stdio.h>
#include <stdlib.h>
#include <uuid/uuid.h>
#include "soapH.h"
#include "soapStub.h"
#include "wsdd.nsmap"
#include "wsaapi.h"

#define MULTICAST_ADDRESS "soap.udp://239.255.255.250:3702"  

int main()  
{  
	printf("[%s][%d][%s][%s] start \n", __FILE__, __LINE__, __TIME__, __func__);
 
	int result = 0;  
	wsdd__ProbeType req;
	struct __wsdd__ProbeMatches resp;
	wsdd__ScopesType sScope;
	struct SOAP_ENV__Header header;  
	
	struct soap *soap;  
	soap = soap_new();  
	if(NULL == soap )  
	{  
		printf("sopa new error\r\n");  
		return -1;  
	}  
 
	soap->recv_timeout = 10;  
	soap_set_namespaces(soap, namespaces);  
	soap_default_SOAP_ENV__Header(soap, &header);  
 
	uuid_t uuid;
	char guid_string[100];
	uuid_generate(uuid);
	uuid_unparse(uuid, guid_string);
 
	header.wsa__MessageID = guid_string; 
	header.wsa__To = "urn:schemas-xmlsoap-org:ws:2005:04:discovery";  
	header.wsa__Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";  
	soap->header = &header;  
	
	soap_default_wsdd__ScopesType(soap, &sScope);  
	sScope.__item = "";  
	soap_default_wsdd__ProbeType(soap, &req);  
	req.Scopes = &sScope;  
	req.Types = "dn:NetworkVideoTransmitter";
 //"dn:NetworkVideoTransmitter";  
 
	int i = 0;  	
       result = soap_send___wsdd__Probe(soap, MULTICAST_ADDRESS, NULL, &req);  
       while(result == SOAP_OK)  
       {  
		result = soap_recv___wsdd__ProbeMatches(soap, &resp);  
		if(result == SOAP_OK)  
		{  
			if(soap->error)  
			{  
				printf("soap error 1: %d, %s, %s\n", soap->error, *soap_faultcode(soap), *soap_faultstring(soap));  
				result = soap->error;  
			}  
			else  
			{  
				printf("guog *********************************************\r\n");  
				if(soap->header->wsa__MessageID)  
				{  
					printf("MessageID   : %s\r\n", soap->header->wsa__MessageID);  
				}  
				if(soap->header->wsa__RelatesTo && soap->header->wsa__RelatesTo->__item)  
				{  
					printf("RelatesTo   : %s\r\n", soap->header->wsa__RelatesTo->__item);  
				}  
				if(soap->header->wsa__To)  
				{  
					printf("To          : %s\r\n", soap->header->wsa__To);  
				}  
				if(soap->header->wsa__Action)  
				{  
					printf("Action      : %s\r\n", soap->header->wsa__Action);  
				}  
 
				for(i = 0; i < resp.wsdd__ProbeMatches->__sizeProbeMatch; i++)  
				{  
					printf("__sizeProbeMatch        : %d\r\n", resp.wsdd__ProbeMatches->__sizeProbeMatch);  
					printf("wsa__EndpointReference       : %p\r\n", resp.wsdd__ProbeMatches->ProbeMatch->wsa__EndpointReference);  
					printf("Target EP Address       : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->wsa__EndpointReference.Address);  
					printf("Target Type             : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->Types);  
					printf("Target Service Address  : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->XAddrs);  
					printf("Target Metadata Version : %d\r\n", resp.wsdd__ProbeMatches->ProbeMatch->MetadataVersion);  
					if(resp.wsdd__ProbeMatches->ProbeMatch->Scopes)  
					{  
						printf("Target Scopes Address   : %s\r\n", resp.wsdd__ProbeMatches->ProbeMatch->Scopes->__item);  
					}  
				}
			}  
		}  
		else if (soap->error)  
		{  
			printf("[%d] soap error 2: %d, %s, %s\n", __LINE__, soap->error, *soap_faultcode(soap), *soap_faultstring(soap));  
			result = soap->error;  
		}  
       }  
 
	soap_destroy(soap); 
	soap_end(soap); 
	soap_free(soap);
 
	printf("[%d] guog discover over !\n", __LINE__);
	
	return result;  
}  
