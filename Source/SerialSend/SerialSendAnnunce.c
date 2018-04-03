#include "SendMethods.h"
#include "MessageCode.h"

// 1 byte  -> code
// 2 bytes -> networkd id (1)
// 8 bytes -> extended address (3)
// 1 byte  -> capability (11)
void serialSendAnnunce(ZDO_DeviceAnnce_t * deviceAnnce ){
	uint8 * iter ;
	struct DataSend * dataSend;
	while((dataSend = getSendBuffer(12))==NULL);
	if (dataSend->start == NULL){
		dataSend->used=Free;
		return;
	}
	iter = dataSend->start;
	
	iter = sendUInt16(iter, deviceAnnce->nwkAddr);
	iter = sendExtAddr(iter, deviceAnnce->extAddr);
	*iter = deviceAnnce->capabilities;
	send(Annunce, dataSend);
}