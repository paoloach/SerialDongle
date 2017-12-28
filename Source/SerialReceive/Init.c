#include "SerialReceive/DecodingFunctions.h"
#include "CheckingChildList.h"

extern byte serialDongleTaskID;

void init(uint8 * data) {
	initCheckingChildList(serialDongleTaskID);
}