#include "SerialReceive/DecodingFunctions.h"
#include "SerialSend/SendMethods.h"

// 1 byte  -> code
// 2 bytes -> network id
// 1 byte -> start index
void getMgmtLqiReq(uint8 * data) {
	zAddrType_t destAddr;
	data++;
	destAddr.addr.shortAddr = BUILD_UINT16(data[0], data[1]);
	destAddr.addrMode = afAddr16Bit;
	ZDP_MgmtLqiReq(&destAddr,data[2], 0);
}