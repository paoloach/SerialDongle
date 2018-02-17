/**************************************************************************************************

 DESCRIPTION:
  --

 CREATED: 18/12/2017, by Paolo Achdjian

 FILE: SerialSend.h

***************************************************************************************************/
#ifndef __SERIAL_SEND_H__
#define __SERIAL_SEND_H__

#include <hal_types.h>
#include "MessageCode.h"

#define MAX_DATA_SIZE	32

enum DataSendStatus {
	Free=0,
	UsedByDMA=0x80,
	Waiting=1,
	WaitingDMA=2,
	ToFree=0x40
};

struct DataSend {
	uint8 * start;
	enum DataSendStatus  used;
	uint8 private;
};

void serialInit(void);
void send(enum MessageCode code, struct DataSend * buffer);
struct DataSend * getPrivateSendBuffer(uint8 * buffer);
struct DataSend * getSendBuffer(uint8 size);
void serialSendLoop(void);


#endif
