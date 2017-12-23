
#include "SendMethods.h"
#include "MessageCode.h"


// 2 bytes -> network id (1)
// 1 byte  -> endpoint id (3)
// 2 bytes -> cluster id(4)
// 2 bytes -> attribute id(6)
void serialSendAttributeResponseMsgError(struct ReqAttributeMsg * attr, ZStatus_t status){
	char * iter ;
	while(basePointer==NULL);
	iter = basePointer;
	
	iter = sendUInt16(iter, attr->afAddrType.addr.shortAddr);
	*iter = attr->afAddrType.endPoint;
	iter++;
	iter = sendUInt16(iter, attr->cluster);
	iter = sendUInt16(iter, attr->attrID[0]);
	*iter = status;
	send(ActiveEP_Error, 8);
}