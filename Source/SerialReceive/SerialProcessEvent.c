#include "SerialReceive/SerialReceive.h"
#include "SerialReceive/DecodingFunctions.h"
#include "ZDProfile.h"

typedef void (*EventFn )(uint8 * data);

#define MAX_EVENT_CODE	16

const EventFn eventsFn[MAX_EVENT_CODE] = {getIEEEAddress,  // 1
						requestAttribute,  // 2
						requestAttributes, // 3
						requestReset,  // 4
						writeAttribute, // 5
						sendCmd ,  // 6
						requestActiveEndpoints, // 7
						requestNodePower, // 8
						sendReqBind,  // 9
						sendReqUnBind, // 10 
						sendReqDeviceInfo, // 11
						requestBindTable,// 12
						init,  // 13
						removeDevice, // 14 
						getNodeDescriptor,  // 15
						getMgmtLqiReq // 16
						};

// 1 byte -> size
// 1 byte -> code
void serialProcessEvent(uint8 * data) {
	data++;
	if (data[0] <= MAX_EVENT_CODE && data[0] >= 1)
		eventsFn[data[0]-1](data);
}