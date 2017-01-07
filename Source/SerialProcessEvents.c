/**************************************************************************************************

 DESCRIPTION:
  --

 CREATED: 01/01/2017, by Paolo Achdjian

 FILE: SerialProcessEvents.c

***************************************************************************************************/
#include "OnBoard.h"
#include "ZComDef.h"
#include "ZDProfile.h"
#include "SerialProcessEvents.h"
#include "TimerEvents.h"
#include "DataStructures.h"
#include "SerialFunctions.h"
#include "ZigbeeSerialBridge.h"
#include "ZigbeeSerialBridge_data.h"

extern byte serialDongleTaskID;
static uint8 decodeLowDigit(char );
static uint8 decodeHiDigit(char );
static void eventReset(osal_event_hdr_t *);
static void decodeIEEEEvents(char * strEvent);
static void decodeActiveEndpointEvent(char * strEvent);
static void decodeRequestAttribute(char * strEvent);
static void decodeRequestAttributes(char * strEvent);
static void decodeSendComand(char * strEvent);
static void eventReqIeeeAddr(osal_event_hdr_t *);
static void eventActiveEP(osal_event_hdr_t * hdrEvent);
static void eventSendCmd(osal_event_hdr_t *);
static void attributeValue(osal_event_hdr_t * hdrEvent);
static uint16 decodeUInt16(char * iter);
static uint8 decodeUInt8(char * iter);

struct ReqIeeeAddrMsg {
	struct UsbISR	isr;
	union  nwkAddr {
		uint16      nwkAddr;
		uint8		data[2];
	};
	uint8		requestType;
	uint8 		startIndex;
};

// events:
//  "IEEE: networkid, requestType, index     -->  ZDP_IEEEAddrReq
//         4digits,    '0' or '1',  2 digitis
void serialProcessEvent(char * strEvent) {
	if (*strEvent == 'I'){
		strEvent++;
		if (*strEvent == 'E'){
			strEvent++;
			if (*strEvent == 'E'){
				strEvent++;
				if (*strEvent == 'E'){
					strEvent++;
					if (*strEvent == ':'){
						decodeIEEEEvents(strEvent);
					}
				}
			}
		}
	} else if (*strEvent == 'A'){
		strEvent++;
		if (*strEvent == 'E'){
			strEvent++;
			if (*strEvent == ':'){
				decodeActiveEndpointEvent(strEvent);
			}
		}
	} else if (*strEvent == 'R'){
		strEvent++;
		if (*strEvent == 'A'){
			strEvent++;
			if (*strEvent == ':'){
				decodeRequestAttribute(strEvent);
			} else if (*strEvent == 'S'){
				strEvent++;
				if (*strEvent == ':'){
					decodeRequestAttributes(strEvent);
				}
			}
		} else if (*strEvent == 'E'){
			strEvent++;
			if (*strEvent == 'S'){
				strEvent++;
				if (*strEvent == 'E'){
					strEvent++;
					if (*strEvent == 'T'){
						strEvent++;
						if (*strEvent == ':'){
							struct UsbISR * msg = (struct UsbISR *)osal_msg_allocate(sizeof(struct UsbISR) );
							msg->msg.event = EVENT_USB_ISR;
							msg->isr = eventReset;
						}
					}
				}
			}
		}
	} else if (*strEvent == 'S'){
		strEvent++;
		if (*strEvent == 'C'){
			strEvent++;
			if (*strEvent == ':'){
				decodeSendComand(strEvent);
			}
		}
	}
}

static uint8 decodeLowDigit(char c ) {
	if (c >= '0' && c <= '9')
		return c -'0';
	else
		return 10+c - 'A';
}

static uint8 decodeHiDigit(char c ) {
	if (c >= '0' && c <= '9')
		return (c -'0') << 4;
	else
		return (10 +(c - 'A')) << 4;
}

static uint16 decodeUInt16(char * iter){
	uint8 data[2];
	data[1] = decodeHiDigit(*iter++);
	data[1] += decodeLowDigit(*iter++);
	data[0] = decodeHiDigit(*iter++);
	data[0] += decodeLowDigit(*iter++);
	return *((uint16 *)data);
}

static uint8 decodeUInt8(char * iter){
	uint8 data;
	data = decodeHiDigit(*iter++);
	data += decodeLowDigit(*iter);
	return data;
}


static void decodeActiveEndpointEvent(char * strEvent){
	strEvent++;
	strEvent++;
	struct ReqActiveEndpointsEvent * msgEP = (struct ReqActiveEndpointsEvent *)osal_msg_allocate(sizeof(struct ReqActiveEndpointsEvent) );
	msgEP->isr.isr = eventActiveEP;
	msgEP->isr.msg.event = EVENT_USB_ISR;
	msgEP->nwkAddr = decodeUInt16(strEvent);
	osal_msg_send(serialDongleTaskID, (uint8 *)msgEP);
}

static void decodeIEEEEvents(char * strEvent) {
	strEvent++;
	strEvent++;
	struct ReqIeeeAddrMsg * msgReq = (struct ReqIeeeAddrMsg *)osal_msg_allocate(sizeof(struct ReqIeeeAddrMsg) );
	msgReq->isr.isr = eventReqIeeeAddr;
	msgReq->isr.msg.event = EVENT_USB_ISR;
	msgReq->nwkAddr = decodeUInt16(strEvent);
	strEvent += 6;
	msgReq->requestType =(*strEvent++) == '0' ? 0 : 1;
	strEvent++; // --> ','	
	strEvent++; // --> ' '	
	msgReq->startIndex = decodeUInt8(strEvent++);
	osal_msg_send(serialDongleTaskID, (uint8 *)msgReq);
}

  // Send message: SC: networkid, endpointId, clusterId, commandId, dataLen  , data
 //                   4 digits ,  2 digits ,  4 digits,  4 digits,  2 digits,  n*2 digits, where n=dataLen
static void decodeSendComand(char * strEvent){
	uint16 networkid, clusterId, commandId;
	uint8  endpointId, dataLen;
	uint8 * data;
	
	strEvent++;
	strEvent++;
	networkid = decodeUInt16(strEvent);
	strEvent+=6;
	endpointId =  decodeUInt8(strEvent);
	strEvent+=4;
	clusterId = decodeUInt16(strEvent);
	strEvent+=6;
	commandId = decodeUInt16(strEvent);
	strEvent+=6;
	dataLen =  decodeUInt8(strEvent);
	strEvent+=4;
	
	struct SendCmdMsg * msgCmd = (struct SendCmdMsg *)osal_msg_allocate(sizeof(struct SendCmdMsg) +dataLen  );
	msgCmd->isr.isr = eventSendCmd;
	msgCmd->isr.msg.event = EVENT_USB_ISR;
	msgCmd->afAddrType.addrMode = afAddr16Bit;
	msgCmd->afAddrType.addr.shortAddr = networkid;
	msgCmd->afAddrType.endPoint = endpointId;
	msgCmd->cluster = clusterId;
	msgCmd->cmdClusterId = commandId;
	msgCmd->dataLen = dataLen;
	data = (uint8 *)(msgCmd->data);
	for (;dataLen > 0; dataLen--){
		*data = decodeUInt8(strEvent);
		data++;
		strEvent+=2;
	}
}

// decode RA: networkAddress, endpointId, clusterId, attributeId
//              4 digitis   ,  2 digits ,  4digits ,  4 digits
static void decodeRequestAttribute(char * strEvent){
	strEvent++;
	strEvent++;
	struct ReqAttributeMsg * msgAttr = (struct ReqAttributeMsg *)osal_msg_allocate(sizeof(struct ReqAttributeMsg) + sizeof(uint16)  );
	msgAttr->isr.isr = attributeValue;
	msgAttr->isr.msg.event = EVENT_USB_ISR;
	msgAttr->afAddrType.addr.shortAddr = decodeUInt16(strEvent);
	strEvent+=6;
	msgAttr->afAddrType.addrMode = afAddr16Bit;
	msgAttr->afAddrType.endPoint =  decodeUInt8(strEvent);
	strEvent+=4;
	msgAttr->cluster = decodeUInt16(strEvent);
	strEvent+=6;
	msgAttr->numAttr=1;
	msgAttr->attrID[0] = decodeUInt16(strEvent);
	osal_msg_send(serialDongleTaskID, (uint8 *)msgAttr);
}

// decode:  RAS: networkid, endpointId, clusterId, attributesNum, first attributed id, ..., last attribute id
//                4 digits, 2 digits  ,  4 digits, 2 digits     ,  4 digits          , ...,    4  digits
static void decodeRequestAttributes(char * strEvent){
	uint16 networkid, clusterId;
	uint8  i, endpointId, attributeNum;
	
	strEvent++;
	strEvent++;
	networkid = decodeUInt16(strEvent);
	strEvent+=6;
	endpointId =  decodeUInt8(strEvent);
	strEvent+=4;
	clusterId = decodeUInt16(strEvent);
	strEvent+=6;
	attributeNum =  decodeUInt8(strEvent);
	strEvent+=4;
	
	struct ReqAttributeMsg * msgAttr = (struct ReqAttributeMsg *)osal_msg_allocate(sizeof(struct ReqAttributeMsg) + attributeNum*sizeof(uint16)  );
	msgAttr->isr.isr = attributeValue;
	msgAttr->isr.msg.event = EVENT_USB_ISR;
	msgAttr->afAddrType.addr.shortAddr = networkid;
	msgAttr->afAddrType.addrMode = afAddr16Bit;
	msgAttr->afAddrType.endPoint =  endpointId;
	msgAttr->cluster = clusterId;
	msgAttr->numAttr=attributeNum;
	for (i=0; i < attributeNum; i++){
		msgAttr->attrID[i] = decodeUInt16(strEvent);
		strEvent+=6;
	}
	osal_msg_send(serialDongleTaskID, (uint8 *)msgAttr);
	
}

void eventReqIeeeAddr(osal_event_hdr_t * hdrEvent) {
	struct ReqIeeeAddrMsg * msg = (struct ReqIeeeAddrMsg *)hdrEvent;
	ZDP_IEEEAddrReq( msg->nwkAddr, msg->requestType, msg->startIndex, 0); 
}

void eventActiveEP(osal_event_hdr_t * hdrEvent) {
	zAddrType_t destAddr;
	struct ReqActiveEndpointsEvent * msgEP = (struct ReqActiveEndpointsEvent *)hdrEvent;

	destAddr.addrMode = Addr16Bit;
	destAddr.addr.shortAddr = msgEP->nwkAddr;
	afStatus_t ret = ZDP_NWKAddrOfInterestReq(&destAddr,msgEP->nwkAddr, Active_EP_req, 0);
	if (ret != ZSuccess){
		serialSendActiveEPError(msgEP->nwkAddr, ret);
	}
}

void attributeValue(osal_event_hdr_t * hdrEvent){
	ZStatus_t status;
	struct ReqAttributeMsg * reqAttributeMsg  = (struct ReqAttributeMsg *)hdrEvent;

	status = zcl_SendCommand( ENDPOINT, &reqAttributeMsg->afAddrType, reqAttributeMsg->cluster, ZCL_CMD_READ, FALSE, ZCL_FRAME_CLIENT_SERVER_DIR, FALSE, 0, 0,  reqAttributeMsg->numAttr * 2, (uint8 *)reqAttributeMsg->attrID );
	if (status != ZSuccess){
		serialSendAttributeResponseMsgError(reqAttributeMsg, status);
	}
}

void eventReset(osal_event_hdr_t * hdrEvent) {
	SystemReset();
}

void eventSendCmd(osal_event_hdr_t * hdrEvent) {
	struct SendCmdMsg * msgCmd = (struct SendCmdMsg * )hdrEvent;
	
	zcl_SendCommand(ENDPOINT, &msgCmd->afAddrType, msgCmd->cluster, msgCmd->cmdClusterId, TRUE, ZCL_FRAME_CLIENT_SERVER_DIR, 0, 0, 0, msgCmd->dataLen, msgCmd->data );
}
