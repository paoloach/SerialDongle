
#include "SendMethods.h"
#include "MessageCode.h"

// 2 bytes --> network id
// 1 byte  --> status
void serialSendActiveEPError(uint16 nwkAdd, afStatus_t ret){
	uint8 * iter ;
	struct DataSend * dataSend;
	while((dataSend = getSendBuffer(3))==NULL);
	iter = dataSend->start;
	
	iter = sendUInt16(iter, nwkAdd);
	*iter = ret;
	send(ActiveEP_Error, dataSend);
}