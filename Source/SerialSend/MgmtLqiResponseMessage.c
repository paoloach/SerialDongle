#include "SendMethods.h"
#include "MessageCode.h"
#include "APSStatusValue.h"


// if no error
//   1 byte  -> code MgmtLqi
//   2 bytes -> network id (1)
//   1 byte  -> total table entries (3)
//	 1 byte  -> start index  (4)
//   1 byte  -> response table count (5)
//   for every table
//   8 bytes -> pan ID			 (8)
//   8 bytes -> extended address (8)
//   2 bytes -> network address(16)
//   2 bits  -> neightbor device (18)
//   2 bits  -> rx On When Idle (18)
//   3 bits  -> relationship (18)
//   1 bit   -> reserveved(18)
//   2 bits  -> permit join (19)
//   6 bits  -> reserved(19)
//   1 byte  -> depth (20)
//   1 byte  -> LQI(21)

// if not supported
//   1 byte -> MgmqLqiNotSupported
//   2 bytes ->networkd id (1)
void mgmtLqiResponseMessage(zdoIncomingMsg_t * msg){
	uint8 * iter ;
	struct DataSend * dataSend;
	uint8 *adsu;
	uint8 status;

	adsu = msg->asdu;
	status = *adsu++;
	if (status == ZDP_SUCCESS){
		uint8 respTableCount = adsu[2];
		uint8 byteToSend = 22*respTableCount + 3;
		while((dataSend = getSendBuffer(2+byteToSend))==NULL);
		iter = dataSend->start;
		*iter = msg->srcAddr.addr.shortAddr;
		iter++;
		*iter = (msg->srcAddr.addr.shortAddr >> 8);
		iter++;
		osal_memcpy(iter, adsu, byteToSend);
		send(MgmtLqi,dataSend);
	} else if (status == ZDP_NOT_SUPPORTED){
		while((dataSend = getSendBuffer(2))==NULL);
		iter = dataSend->start;
		*iter = msg->srcAddr.addr.shortAddr;
		iter++;
		*iter = (msg->srcAddr.addr.shortAddr >> 8);
		send(MgmqLqiNotSupported, dataSend);
	}
}