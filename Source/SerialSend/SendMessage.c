/**************************************************************************************************

 DESCRIPTION:
  --

 CREATED: 19/12/2017, by Paolo Achdjian

 FILE: SendMessage.c

***************************************************************************************************/
#include <OSAL_Memory.h>

#include "SerialSend.h"
#include "SendMethods.h"


// 1 byte code
// 1 byte size
// n bytes datas
void sendMessage(char * msg){
	char * iter;
	uint8 size=0;
	while(basePointer==NULL);
	iter = basePointer;
	while(*msg != 0){
		*iter = *msg;
		iter++;
		msg++;
		size++;
	}
	send(STRING, size);
}



// 2 bytes->mem_used
// 2 bytes->bkock used
void sendAliveMsg(void) {
	char * iter ;
	while(basePointer==NULL);
	iter = basePointer;
	iter = sendUInt16(iter, osal_heap_mem_used());
	iter = sendUInt16(iter, osal_heap_block_cnt());
	send(MSG_ALIVE, 4);
}

// 2 bytes -> networkId (1)
// 1 byte  -> nodeRelation (3)
// 1 byte  -> device status (4)
// 1 byte  -> association count (5)
// 1 byte  -> age (6)
// 1 byte  -> tx counter (7)
// 1 byte  -> tx cost (8)
// 1 byte  -> rx LQI (9)
void serialSendDeviceInfo(associated_devices_t * device){
	char * iter ;
	while(basePointer==NULL);
	iter = basePointer;
	
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
	send(DeviceInfo, 10);
}


