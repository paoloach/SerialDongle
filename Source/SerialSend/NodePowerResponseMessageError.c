#include "SendMethods.h"
#include "MessageCode.h"


// 1 byte  -> code
// 2 bytes -> networkd id
// 1 byte  -> status
void nodePowerResponseMessageError(uint16 nwkAddr, uint8 status) {
	char * iter ;
	while(basePointer==NULL);
	iter = basePointer;
	
	iter = sendUInt16(iter, nwkAddr);
	*iter = status;
	send(NodePowerError, 3);
}