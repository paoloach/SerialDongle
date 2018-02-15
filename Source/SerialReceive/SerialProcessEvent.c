#include "SerialReceive/SerialReceive.h"
#include "SerialReceive/DecodingFunctions.h"
#include "ZDProfile.h"

uint8 cmdData[10];
uint8 cmdDataIndex=0;

// 1 byte -> size
// 1 byte -> code
void serialProcessEvent(uint8 * data) {
	data++;
	cmdData[cmdDataIndex] = data[0];
	cmdDataIndex++;
	if (cmdDataIndex >= 10)
		cmdDataIndex=9;
	switch(data[0]){
	case 1:
		getIEEEAddress(data);
		break;
	case 2:
		requestAttribute(data);
		break;
	case 3:
		requestAttributes(data);
		break;
	case 4:
		requestReset(data);
		break;
	case 5:
		writeAttribute(data);
		break;
	case 6:
		sendCmd(data);
		break;
	case 7:	
		requestActiveEndpoints(data);
		break;
	case 8:
		requestNodePower(data);
		break;
	case 9:
		sendReqBind(data,Bind_req);
		break;
	case 10:
		sendReqBind(data,Unbind_req);
		break;
	case 11:
		sendReqDeviceInfo(data);
		break;
	case 12:
		requestBindTable(data);
		break;
	case 13:
		init(data);
		break;
	case 14:
		removeDevice(data);
		break;	
	case 15:
		getNodeDescriptor(data);
		break;
	case 16:
		getMgmtLqiReq(data);
		break;
	default:
		invalidCmd(data);		
	}
}