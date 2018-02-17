#include "SendMethods.h"
#include "MessageCode.h"


// 2 bytes -> network id (1)
// 1 byte  -> endpoint id (3)
// 2 bytes -> cluster id(4)
// 2 bytes -> attribute id(6)
void serialSendWriteAttributeError(uint16 networkId, uint8 endpoint, uint16 cluster, uint16 attribute, ZStatus_t status){
	uint8 * iter ;
	struct DataSend * dataSend;
	while((dataSend = getSendBuffer(8))==NULL);
	if (dataSend->start == NULL){
		dataSend->used=Free;
		return;
	}
	iter = dataSend->start;
	
	iter = sendUInt16(iter, networkId);
	*iter = endpoint;
	iter++;
	iter = sendUInt16(iter, cluster);
	iter = sendUInt16(iter, attribute);
	*iter = status;
	send(WriteAttributeError, dataSend);
}