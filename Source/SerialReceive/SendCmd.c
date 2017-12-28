#include "ZDProfile.h"

#include "SerialReceive/DecodingFunctions.h"
#include "DataStructures.h"
#include "TimerEvents.h"
#include "ZigbeeSerialBridge_data.h"
#include "SerialSend/SendMethods.h"


// 1 byte  -> code
// 2 bytes -> network id
// 1 byte  -> endpoint
// 2 bytes -> cluster id
// 2 bytes -> commandId id
// 1 byte  -> data value len
// n bytes -> data values
void sendCmd(uint8 * data){
	afAddrType_t dstAddr;
	data++;
	dstAddr.addr.shortAddr = BUILD_UINT16(data[0], data[1]);
	dstAddr.addrMode = afAddr16Bit;
	dstAddr.endPoint = data[2];
	uint16 cluster = BUILD_UINT16(data[3], data[4]);
	uint16 cmdId = BUILD_UINT16(data[5], data[6]);
	uint8 dataLen = data[7];
	ZStatus_t status = zcl_SendCommand( ENDPOINT,  &dstAddr, cluster, cmdId, TRUE,  ZCL_FRAME_CLIENT_SERVER_DIR, 0, 0, 0, dataLen, data+8 );	
	if (status !=ZSuccess){
		serialSendWriteAttributeError(dstAddr.addr.shortAddr, dstAddr.endPoint, cluster, cmdId, status);
	}
}