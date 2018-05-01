#include "ZDProfile.h"
#include "SerialReceive/DecodingFunctions.h"
#include "SerialSend/SendMethods.h"



static void sendReq( uint8 * data, uint16 bindOrUnbind);

void sendReqBind(uint8 * data) {
	sendReq(data, Bind_req);
}

void sendReqUnBind(uint8 * data) {
	sendReq(data, Unbind_req);
}

// 1 byte  -> code (0)
// 2 bytes -> network id (1)
// 8 bytes -> out cluster addr (3)
// 1 bytes -> out endpoint id (11)
// 2 bytes -> cluster id (12)
// 8 bytes -> in cluster addr (14)
// 1 byte  -> in endpoint id (22)
void sendReq(uint8 * data, uint16 bindOrUnbind) {

	zAddrType_t dstAddr;
	zAddrType_t destinationAddr;
	dstAddr.addr.shortAddr = BUILD_UINT16(data[1], data[2]);
	dstAddr.addrMode = afAddr16Bit;
	uint16 clusterId = BUILD_UINT16(data[12], data[13]);
	for(uint8 i=0; i < 8; i++)destinationAddr.addr.extAddr[i] = data[14+i];
	destinationAddr.addrMode = Addr64Bit;
	
	ZDP_BindUnbindReq(bindOrUnbind, &dstAddr, data+3, data[11],  clusterId, &destinationAddr, data[22], 0);

}