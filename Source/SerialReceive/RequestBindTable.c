#include "SerialReceive/DecodingFunctions.h"
#include "SerialSend/SendMethods.h"

// 1 byte  -> code
// 2 bytes -> network id
void requestBindTable(uint8 * data){
	uint8 first=0;
	zAddrType_t dstAddr;
	data++;
	dstAddr.addr.shortAddr = BUILD_UINT16(data[0], data[1]);
	dstAddr.addrMode = afAddr16Bit;

	ZDP_SendData( &ZDP_TransID, &dstAddr, Mgmt_Bind_req, 1, &first, 0 );
}