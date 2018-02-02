#include "SendMethods.h"
#include "MessageCode.h"

// if no error
//   1 byte  -> code NodePower
//   2 bytes -> network (1)
//   1 bytes -> logicalType (3)
//   1 byte  -> frequencyBand (4)
//   1 byte  -> MAC capability flags(5)
//   2 bytes -> manifactureCode (6)
//   1 byte   -> maximum buffer size(8)
//   2 byte  -> maximum incoming transfer size(9)
//   2 bytes -> server mask (11)
//   2 bytes -> maximum outgoing transfer size (13)
//   1 byte  -> descriptor capability flags  (15)


// if error
//   1 byte  -> code NodePowerError
//   2 bytes -> network 1
//   1 byte  -> status
void nodeDescriptionResponseMessage(zdoIncomingMsg_t * inMsg) {
	uint8 * iter ;
	struct DataSend * dataSend;
	while((dataSend = getSendBuffer())==NULL);
	iter = dataSend->start;
	
	uint8 *msg;
	uint8 status;
	uint8 len;
	msg = inMsg->asdu;
	status = *msg++;
	

	if (status == ZDP_SUCCESS){
		*iter = *msg;
		iter++;
		msg++;
		*iter = *msg;
		iter++;
		msg++;
		*iter =*msg & 0x07;
		iter++;
		msg++;
		*iter = (*msg >> 3) & 0x1f;
		iter++;
		msg++;
		len = 11;
		while ( len-- )
    		*iter++ = *msg++;
		send(NodeDescription, 16,dataSend);
	} else {
		*iter = *msg;
		iter++;
		msg++;
		*iter = *msg;
		iter++;
		*iter = status;
		send(NodeDescriptionError, 3,dataSend);			
	}

}
