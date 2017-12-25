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

struct DataSend {
	uint8 * start;
	uint8   used;
};

void serialInit(void);
void send(enum MessageCode code, uint8 size, struct DataSend * buffer);
struct DataSend * getSendBuffer(void);
void serialSendLoop(void);


#endif
