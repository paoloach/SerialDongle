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
#define LONG_BUFFER_DMA_SIZE	120

enum DataSendStatus {
	Free=0,
	UsedByDMA=0x80,
	Waiting=1,
	WaitingDMA=2,
	ToFree=0x40
};

struct DataSend {
	uint8 * start;
	uint8 * bufferStart;
	uint16 size;
	enum DataSendStatus  used;
	uint8 private;
};

void serialInit(void);
void send(enum MessageCode code, struct DataSend * buffer);
struct DataSend * getPrivateSendBuffer(uint8 * buffer, uint8 size);
struct DataSend * getSendBuffer(uint16 size);
void serialSendLoop(void);

void initLongBuffer(uint16 dataSize,enum MessageCode code);
void cpyIntoLongBuffer(uint8 *adsu, uint16 size);
void longBufferSend(void);
void longBufferSendUInt8(uint8 value);
void longBufferSendUInt16(uint16 value);

#endif
