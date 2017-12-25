
#include "SendMethods.h"
#include "MessageCode.h"

// if successs
//   1 byte  -> code IEEEAddressError
//   8 bytes -> extended address
//   2 bytes -> network id
//   1 byte  -> number of associated devices
//   1 byte  -> start index
//	 1 byte  -> child number
//    n*2 bytes -> children network id where n is child number

// if error
//   1 byte -> code IEEEAddressError 
void serialSendIeeeAddress(zdoIncomingMsg_t * inMsg ){
	byte cnt = 0;
	uint8  status;
	uint8 * iter ;
	
	if ( inMsg->asduLen > (1 + Z_EXTADDR_LEN + 2) ) {
 		cnt = inMsg->asdu[1 + Z_EXTADDR_LEN + 2];
	} else {
    	cnt = 0;
  	}
	uint8 * msg = inMsg->asdu;
	status =  *msg++;

	struct DataSend * dataSend;
	while((dataSend = getSendBuffer())==NULL);
	iter = dataSend->start;
	
	if (status == ZDO_SUCCESS){
		uint8 size = Z_EXTADDR_LEN + 2 + 2 + cnt;
		for (uint8 i=0; i < size; i++){
			*iter = *msg;
			iter++;
			msg++;
		}
		send(IEEEAddress, size,dataSend);	
	} else {
		send(IEEEAddressError, 0,dataSend);
	}

}
