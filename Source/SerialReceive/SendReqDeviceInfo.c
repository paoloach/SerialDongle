#include "SerialReceive/DecodingFunctions.h"
#include "SerialSend/SendMethods.h"

// 1 byte  -> code
// 2 bytes -> network id
void sendReqDeviceInfo(uint8 * data) {
	uint16 nwkAddr = BUILD_UINT16(data[1], data[2]);
	
	associated_devices_t * device= AssocGetWithShort( nwkAddr);
	if (device == NULL){
	} else {
		serialSendDeviceInfo(device);
	}	
}