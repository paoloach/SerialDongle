#include "SendMethods.h"
#include "MessageCode.h"

// 1 byte  -> code 
// 2 bytes ->mem_used  (1)
// 2 bytes ->bkock used (3)
// 1 byte  -> max data size (4)
// 1 byte  -> max buffer used (5)
extern uint8 maxDataSize;
extern uint8 maxBufferUsed;
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
	send(MSG_ALIVE, 6, dataSend);
}