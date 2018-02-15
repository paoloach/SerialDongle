#include "SerialReceive/DecodingFunctions.h"
#include "CheckingChildList.h"

extern byte serialDongleTaskID;
extern uint8 maxDataSize;
extern uint8 maxBufferUsed;
extern uint16 rxData1Count;
extern uint16 rxData2Count;
extern uint16 rxData3Count;
extern uint16 rxDataOutOfBuffer;
extern uint16 rxDataError;

void init(uint8 * data) {
	initCheckingChildList(serialDongleTaskID);
	maxDataSize=0;
	maxBufferUsed=0;
	rxData1Count=0;
	rxData2Count=0;
	rxData3Count=0;
	rxDataOutOfBuffer=0;
	rxDataError=0;
}