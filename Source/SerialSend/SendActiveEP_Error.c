
#include "SendMethods.h"
#include "MessageCode.h"

// 2 bytes --> network id
// 1 byte  --> status
void serialSendActiveEPError(uint16 nwkAdd, afStatus_t ret){
	char * iter ;
	while(basePointer==NULL);
	iter = basePointer;
	
	iter = sendUInt16(iter, nwkAdd);
	*iter = ret;
	send(ActiveEP_Error, 3);
}