#include "SendMethods.h"
#include "MessageCode.h"



// 2 bytes -> networkId (1)
// 1 byte  -> nodeRelation (3)
// 1 byte  -> device status (4)
// 1 byte  -> association count (6)
// 1 byte  -> age (7)
// 1 byte  -> tx counter (8)
// 1 byte  -> tx cost (9)
// 1 byte  -> rx LQI (10)
void serialSendDeviceInfo(associated_devices_t * device){
	uint8 * iter ;
	struct DataSend * dataSend;
	while((dataSend = getSendBuffer())==NULL);
	iter = dataSend->start;
	
	iter = sendUInt16(iter, device->shortAddr);
	*iter = device->nodeRelation;
	iter++;
	*iter = device->devStatus;
	iter++;
	*iter = device->nodeRelation;
	iter++;
	*iter = device->assocCnt;
	iter++;
	*iter = device->age;
	iter++;
	*iter = device->linkInfo.txCounter;
	iter++;
	*iter = device->linkInfo.txCost;
	iter++;	
	*iter = device->linkInfo.rxLqi;	
	send(DeviceInfo, 10, dataSend);
}


