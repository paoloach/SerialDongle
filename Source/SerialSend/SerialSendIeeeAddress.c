
#include "SendMethods.h"
#include "MessageCode.h"

// if successs
//   1 byte  -> code IEEEAddressError (0)
//   8 bytes -> extended address (1)
//   2 bytes -> network id (9)
//   1 byte  -> number of associated devices (11)
//   1 byte  -> start index (12)
//    n*2 bytes -> children network id where n is child number (14 ... )

// if error
//   1 byte -> code IEEEAddressError 
void serialSendIeeeAddress(zdoIncomingMsg_t * inMsg ){
	byte cnt;
	uint8  status;
	uint8 * iter ;
	struct DataSend * dataSend;
	uint8 * msg;
	
	msg = inMsg->asdu;
	if ( inMsg->asduLen > (1 + Z_EXTADDR_LEN + 2) ) {
 		cnt = inMsg->asdu[1 + Z_EXTADDR_LEN + 2];
	} else {
    	cnt = 0;
  	}

	status =  *msg++;

	if (status == ZDO_SUCCESS){
		uint8 size = Z_EXTADDR_LEN + 2 + 2 + 2*cnt;
		while((dataSend = getSendBuffer(size))==NULL);
		if (dataSend->start == NULL){
			dataSend->used=Free;
			return;
		}
		iter = dataSend->start;
		for (uint8 i=0; i < size; i++){
			*iter = *msg;
			iter++;
			msg++;
		}
		send(IEEEAddress, dataSend);	
	} else {
		while((dataSend = getSendBuffer(0))==NULL);
		send(IEEEAddressError, dataSend);
	}

}
