#include "SendMethods.h"
#include "MessageCode.h"

// if no error
//   1 byte  -> code NodePower
//   2 bytes -> network 1
//   1 bytes -> powerMode | availablePowerSources
//   1 byte  -> currentPowerSource | currentPowerSourceLevel
// if error
//   1 byte  -> code NodePowerError
//   2 bytes -> network 1
//   1 byte  -> status
void nodePowerResponseMessage(zdoIncomingMsg_t * inMsg) {
	uint8 * iter ;
	struct DataSend * dataSend;
	uint8 *msg;
	uint8 status;

	msg = inMsg->asdu;
	status = *msg++;
	if (status == ZDP_SUCCESS){
		while((dataSend = getSendBuffer(4))==NULL);
		iter = dataSend->start;
		
		*iter = *msg;
		iter++;
		msg++;
		*iter = *msg;
		iter++;
		msg++;
		*iter = *msg;
		iter++;
		msg++;
		*iter = *msg;
		send(NodePower,dataSend);
	} else {
		while((dataSend = getSendBuffer(3))==NULL);
		iter = dataSend->start;
		
		*iter = *msg;
		iter++;
		msg++;
		*iter = *msg;
		iter++;
		*iter = status;
		send(MgmqLqiNotSupported, dataSend);			
	}

}
