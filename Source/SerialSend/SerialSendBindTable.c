#include "SendMethods.h"
#include "MessageCode.h"

// 1 byte  -> code BindTable
// 8 bytes -> source extended address (1)
// 1 byte  -> source endpoint (9)
// 2 bytes -> cluster id (10)
// 2 bytes -> destination networkd id (12)
// 1 byte  -> destination endpoint (14)
void serialSendBindTable(struct BindTableResponseEntry * bindTable) {
	uint8 * iter ;
	struct DataSend * dataSend;
	while((dataSend = getSendBuffer(14))==NULL);
	if (dataSend->start == NULL){
		dataSend->used=Free;
		return;
	}
	iter = dataSend->start;
	
	iter = sendExtAddr(iter, bindTable->srcAddr);
	*iter = bindTable->srcEP;
	iter++;
	iter = sendUInt16(iter, bindTable->clusterID);		
	iter = sendUInt16(iter, bindTable->dstAddr);
	*iter = bindTable->dstEP;
	send(BindTable, dataSend);				
}
