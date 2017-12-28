#include "ZDProfile.h"

#include "SerialReceive/DecodingFunctions.h"
#include "DataStructures.h"
#include "TimerEvents.h"

// 1 byte  -> code
// 2 bytes -> network id
// 1 byte  -> request type
// 1 byte  -> start index
void getIEEEAddress(uint8 * data) {
	data++;
	uint16 network = BUILD_UINT16(data[0], data[1]);
	ZDP_IEEEAddrReq( network, data[2], data[3], 0); 
}

