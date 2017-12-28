#include "SerialReceive/DecodingFunctions.h"
#include "SerialSend/SendMethods.h"

void invalidCmd(uint8 * data) {
	sendMessage("Received invalid cmd");
}

