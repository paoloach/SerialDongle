#include "SendMethods.h"
#include "MessageCode.h"

// 1 byte  -> code SimpleDescriptor
// 2 bytes -> network id(1)
// 1 byte  -> endpointd id(3)
// 2 bytes -> application profile id(4)
// 2 bytes -> application device id(6)
// 1 byte  -> application device version(8)
// 1 byte  -> Cluster in element size(9)
//  n*2 byter --> Cluter id, where n is Cluster in element size(10)
// 1 byte  -> Cluster ou element size
//  n*2 byter --> Cluter id, where n is Cluster out element size

void serialSendSimpleDescriptor(ZDO_SimpleDescRsp_t * simpleDesc){
	uint8 i;
	uint8 size=10+2* simpleDesc->simpleDesc.AppNumInClusters + 2* simpleDesc->simpleDesc.AppNumOutClusters;
	uint8 * iter ;
	struct DataSend * dataSend;
	while((dataSend = getSendBuffer())==NULL);
	iter = dataSend->start;


	iter = sendUInt16(iter,  simpleDesc->nwkAddr);
	*iter =  simpleDesc->simpleDesc.EndPoint;
	iter++;
	iter = sendUInt16(iter, simpleDesc->simpleDesc.AppProfId);
	iter = sendUInt16(iter,  simpleDesc->simpleDesc.AppDeviceId);
	*iter =simpleDesc->simpleDesc.AppDevVer;
	iter++;
	*iter =  simpleDesc->simpleDesc.AppNumInClusters;
	iter++;
	for (i=0; i <simpleDesc->simpleDesc.AppNumInClusters; i++){
		iter = sendUInt16(iter, simpleDesc->simpleDesc.pAppInClusterList[i]);
	}
	*iter =  simpleDesc->simpleDesc.AppNumOutClusters;
	iter++;
	for (i=0; i <simpleDesc->simpleDesc.AppNumOutClusters; i++){
		iter = sendUInt16(iter, simpleDesc->simpleDesc.pAppInClusterList[i]);
	}	
	send(SimpleDescriptor, size,dataSend);
}
