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

extern char * basePointer;
#define MAX_DATA_SIZE	128

void serialInit(void);
void send(enum MessageCode code, uint8 size);


#endif
