#include "SerialReceive/DecodingFunctions.h"
#include "CheckingChildList.h"

extern byte serialDongleTaskID;
extern uint8 maxDataSize;
extern uint8 maxBufferUsed;
extern uint16 rxDataUsed;
extern uint16 rxDataOutOfBuffer;
extern uint16 rxDataError;

void init(uint8 * data) {
	initCheckingChildList(serialDongleTaskID);
	maxDataSize=0;
	maxBufferUsed=0;
	rxDataOutOfBuffer=0;
	rxDataError=0;
	rxDataUsed=0;
}