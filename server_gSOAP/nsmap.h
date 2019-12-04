
#include "stdsoap2.h"
/* This defines the global XML namespaces[] table to #include and compile */
SOAP_NMAC struct Namespace namespaces[] = {
        { "SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL },
        { "SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL },
        { "xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL },
        { "xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL },
        { "wsa", "http://schemas.xmlsoap.org/ws/2004/08/addressing", "http://www.w3.org/2005/08/addressing", NULL },
        { "wsdd", "http://schemas.xmlsoap.org/ws/2005/04/discovery", NULL, NULL },
        { "tdn", "http://www.onvif.org/ver10/network/wsdl", NULL, NULL },
        { NULL, NULL, NULL, NULL}
    };
