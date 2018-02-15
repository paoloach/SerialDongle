#include "SendMethods.h"
#include "MessageCode.h"

// 1 byte  -> code 
// 2 bytes ->mem_used  (1)
// 2 bytes ->bkock used (3)
// 1 byte  -> max data size (5)
// 1 byte  -> max buffer used (6)
// 2 bytes -> rxData1Count (7)
// 2 bytes -> rxData2Count (8)
// 2 bytes -> rxData3Count (11)
// 2 bytes -> rxDataOutOfBuffer (13)
// 2 bytes -> rxDataError (15)
// 1 byte  -> dataErrorLen (17)
// n1 bytes -> dataError (18)
// 1 byte  -> dataSizeLen(18+n1)
// n2 bytes -> dataSize(19+n1)
// 1 byte  -> cmdDataLen(19+n1+n2)
// n3 bytes -> cmdData(120+n1+n2)
extern uint8 maxDataSize;
extern uint8 maxBufferUsed;
extern uint16 rxData1Count;
extern uint16 rxData2Count;
extern uint16 rxData3Count;
extern uint16 rxDataOutOfBuffer;
extern uint16 rxDataError;
extern uint8 errorData[20];
extern uint8 errorDataIndex;
extern uint8 sizeData[10];
extern uint8 sizeDataIndex;

extern uint8 cmdData[10];
extern uint8 cmdDataIndex;


void sendAliveMsg(void) {
	uint8 * iter ;
	struct DataSend * dataSend;
	while((dataSend = getSendBuffer())==NULL);
	iter = dataSend->start;
	iter = sendUInt16(iter, osal_heap_mem_used());
	iter = sendUInt16(iter, osal_heap_block_cnt());
	*iter = maxDataSize;
	iter++;
	*iter = maxBufferUsed;
	iter++;
	iter = sendUInt16(iter, rxData1Count);
	iter = sendUInt16(iter, rxData2Count);
	iter = sendUInt16(iter, rxData3Count);
	iter = sendUInt16(iter, rxDataOutOfBuffer);
	iter = sendUInt16(iter, rxDataError);
	*iter = errorDataIndex;
	iter++;
	for(uint8 i=0; i< errorDataIndex; i++){
		*iter = errorData[i];
		iter++;
	}
	*iter = sizeDataIndex;
	iter++;
	for(uint8 i=0; i< sizeDataIndex; i++){
		*iter = sizeData[i];
		iter++;
	}
	*iter = cmdDataIndex;
	iter++;
	for(uint8 i=0; i< cmdDataIndex; i++){
		*iter = cmdData[i];
		iter++;
	}	
	
	send(MSG_ALIVE, 19+errorDataIndex+sizeDataIndex+cmdDataIndex, dataSend);
	
	errorDataIndex=0;
	sizeDataIndex=0;
	cmdDataIndex=0;
}