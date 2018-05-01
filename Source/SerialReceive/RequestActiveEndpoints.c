#include "SerialReceive/DecodingFunctions.h"
#include "SerialSend/SendMethods.h"

// 1 byte  -> code
// 2 bytes -> network id
void requestActiveEndpoints(uint8 * data) {
	zAddrType_t destAddr;
	data++;
	destAddr.addr.shortAddr = BUILD_UINT16(data[0], data[1]);
	destAddr.addrMode = afAddr16Bit;
	afStatus_t ret = ZDP_NWKAddrOfInterestReq(&destAddr,destAddr.addr.shortAddr, Active_EP_req, 0);
	if (ret != ZSuccess){
		serialSendActiveEPError(destAddr.addr.shortAddr, ret);
	}
	
}