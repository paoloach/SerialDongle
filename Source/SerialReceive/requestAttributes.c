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
// 1 byte  -> number of attributes
//   2*n bytes -> attributes id
void requestAttributes(uint8 * data) {
	afAddrType_t dstAddr;
	data++;
	dstAddr.addr.shortAddr = BUILD_UINT16(data[0], data[1]);
	dstAddr.addrMode = afAddr16Bit;
	dstAddr.endPoint = data[2];
	uint16 cluster = BUILD_UINT16(data[3], data[4]);
	
	ZStatus_t status;

	status = zcl_SendCommand( ENDPOINT, &dstAddr, cluster, ZCL_CMD_READ, FALSE, ZCL_FRAME_CLIENT_SERVER_DIR, FALSE, 0, 0, 2*data[5], (uint8 *)(data+6) );
	if (status != ZSuccess){
		serialSendAttributeResponseMsgError(dstAddr.addr.shortAddr, dstAddr.endPoint, cluster, BUILD_UINT16(data[6], data[7]), status);
	}
}