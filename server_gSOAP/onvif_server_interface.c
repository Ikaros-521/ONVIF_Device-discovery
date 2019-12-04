#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>
#include "soapH.h"
#include "soapStub.h"
 
 
SOAP_FMAC5 int SOAP_FMAC6 SOAP_ENV__Fault(struct soap* soap, char *faultcode, char *faultstring, char *faultactor,
struct SOAP_ENV__Detail *detail, struct SOAP_ENV__Code *_SOAP_ENV__Code,
struct SOAP_ENV__Reason *SOAP_ENV__Reason, char *SOAP_ENV__Node,
	char *SOAP_ENV__Role, struct SOAP_ENV__Detail *SOAP_ENV__Detail)
{
 
	return 0;
}
 
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Hello(struct soap* soap, struct wsdd__HelloType *wsdd__Hello)
{
 
	return 0;
}
 
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Bye(struct soap* soap, struct wsdd__ByeType *wsdd__Bye)
{
 
	return 0;
}
 
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ProbeMatches(struct soap* soap, struct wsdd__ProbeMatchesType *wsdd__ProbeMatches)
{
 
	return 0;
}
 
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Resolve(struct soap* soap, struct wsdd__ResolveType *wsdd__Resolve)
{
 
	return 0;
}
 
SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ResolveMatches(struct soap* soap, struct wsdd__ResolveMatchesType *wsdd__ResolveMatches)
{
 
	return 0;
}
 
SOAP_FMAC5 int SOAP_FMAC6 __tdn__Hello(struct soap* soap, struct wsdd__HelloType tdn__Hello, struct wsdd__ResolveType *tdn__HelloResponse)
{
 
	return 0;
}
 
SOAP_FMAC5 int SOAP_FMAC6 __tdn__Bye(struct soap* soap, struct wsdd__ByeType tdn__Bye, struct wsdd__ResolveType *tdn__ByeResponse)
{
 
	return 0;
}
 
SOAP_FMAC5 int SOAP_FMAC6 __tdn__Probe(struct soap* soap, struct wsdd__ProbeType tdn__Probe, struct wsdd__ProbeMatchesType *tdn__ProbeResponse)
{
 
	return 0;
}
 
SOAP_FMAC5 int SOAP_FMAC6  __wsdd__Probe(struct soap* soap, struct wsdd__ProbeType *wsdd__Probe)
{
 
#define MACH_ADDR_LENGTH 6
#define INFO_LENGTH 512
#define LARGE_INFO_LENGTH 1024
#define SMALL_INFO_LENGTH 512
 
	printf("[%d] __wsdd__Probe start !\n", __LINE__);
 
	unsigned char macaddr[6] = { 0 };
	char _IPAddr[INFO_LENGTH] = { 0 };
	char _HwId[1024] = { 0 };
 
	wsdd__ProbeMatchesType ProbeMatches;
	ProbeMatches.ProbeMatch = (struct wsdd__ProbeMatchType *)soap_malloc(soap, sizeof(struct wsdd__ProbeMatchType));
	ProbeMatches.ProbeMatch->XAddrs = (char *)soap_malloc(soap, sizeof(char)* INFO_LENGTH);
	ProbeMatches.ProbeMatch->Types = (char *)soap_malloc(soap, sizeof(char)* INFO_LENGTH);
	ProbeMatches.ProbeMatch->Scopes = (struct wsdd__ScopesType*)soap_malloc(soap, sizeof(struct wsdd__ScopesType));
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties = (struct wsa__ReferencePropertiesType*)soap_malloc(soap, sizeof(struct wsa__ReferencePropertiesType));
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters = (struct wsa__ReferenceParametersType*)soap_malloc(soap, sizeof(struct wsa__ReferenceParametersType));
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName = (struct wsa__ServiceNameType*)soap_malloc(soap, sizeof(struct wsa__ServiceNameType));
	ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType = (char **)soap_malloc(soap, sizeof(char *)* SMALL_INFO_LENGTH);
	ProbeMatches.ProbeMatch->wsa__EndpointReference.__any = (char **)soap_malloc(soap, sizeof(char*)* SMALL_INFO_LENGTH);
	ProbeMatches.ProbeMatch->wsa__EndpointReference.__anyAttribute = (char *)soap_malloc(soap, sizeof(char)* SMALL_INFO_LENGTH);
	ProbeMatches.ProbeMatch->wsa__EndpointReference.Address = (char *)soap_malloc(soap, sizeof(char)* INFO_LENGTH);
 
	//netGetMac("eth4", macaddr); //eth0  根据实际情况填充
	macaddr[0] = 0x01; macaddr[1] = 0x01; macaddr[2] = 0x01; macaddr[3] = 0x01; macaddr[4] = 0x01; macaddr[5] = 0x01;
	sprintf(_HwId, "urn:uuid:2419d68a-2dd2-21b2-a205-%02X%02X%02X%02X%02X%02X", macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
 
	sprintf(_IPAddr, "http://%s/onvif/device_service", "10.70.12.188");
	printf("[%d] _IPAddr ==== %s\n", __LINE__, _IPAddr);
 
	ProbeMatches.__sizeProbeMatch = 1;
	ProbeMatches.ProbeMatch->Scopes->__item = (char *)soap_malloc(soap, 1024);
	//memset(ProbeMatches.ProbeMatch->Scopes->__item,0,sizeof(ProbeMatches.ProbeMatch->Scopes->__item));
	memset(ProbeMatches.ProbeMatch->Scopes->__item, 0, 1024);
 
	//Scopes MUST BE
	strcat(ProbeMatches.ProbeMatch->Scopes->__item, "onvif://www.onvif.org/type/NetworkVideoTransmitter");
 
	ProbeMatches.ProbeMatch->Scopes->MatchBy = NULL;
	strcpy(ProbeMatches.ProbeMatch->XAddrs, _IPAddr);
	strcpy(ProbeMatches.ProbeMatch->Types, wsdd__Probe->Types);
	printf("wsdd__Probe->Types=%s\n", wsdd__Probe->Types);
	ProbeMatches.ProbeMatch->MetadataVersion = 1;
 
	//ws-discovery规定 为可选项
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties->__size = 0;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties->__any = NULL;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters->__size = 0;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters->__any = NULL;
 
	ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType[0] = (char *)soap_malloc(soap, sizeof(char)* SMALL_INFO_LENGTH);
	//ws-discovery规定 为可选项
	strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType[0], "ttl");
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->__item = NULL;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->PortName = NULL;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->__anyAttribute = NULL;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.__any[0] = (char *)soap_malloc(soap, sizeof(char)* SMALL_INFO_LENGTH);
	strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.__any[0], "Any");
	strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.__anyAttribute, "Attribute");
	ProbeMatches.ProbeMatch->wsa__EndpointReference.__size = 0;
	strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.Address, _HwId);
 
	soap->header->wsa__To = "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous";
	soap->header->wsa__Action = "http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches";
	soap->header->wsa__RelatesTo = (struct wsa__Relationship*)soap_malloc(soap, sizeof(struct wsa__Relationship));
	soap->header->wsa__RelatesTo->__item = soap->header->wsa__MessageID;
	printf("__item: %p, wsa__MessageID: %p: %s\n", soap->header->wsa__RelatesTo->__item, soap->header->wsa__MessageID, soap->header->wsa__MessageID);
	soap->header->wsa__RelatesTo->RelationshipType = NULL;
	soap->header->wsa__RelatesTo->__anyAttribute = NULL;
 
	soap->header->wsa__MessageID = (char *)soap_malloc(soap, sizeof(char)* INFO_LENGTH);
	strcpy(soap->header->wsa__MessageID, _HwId + 4);
 
	if (SOAP_OK == soap_send___wsdd__ProbeMatches(soap, "http://", NULL, &ProbeMatches))
	{
		printf("send ProbeMatches success !\n");
		return SOAP_OK;
	}
 
	printf("[%d] soap error: %d, %s, %s\n", __LINE__, soap->error, *soap_faultcode(soap), *soap_faultstring(soap));
 
	return soap->error;;
 
}
