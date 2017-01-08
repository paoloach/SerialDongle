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
static void decodeSendWriteAttribute(char * strEvent);
static void decodeDeviceInfoEvent(char * strEvent);
static void decodeRequestBindUnBinf(char * strEvent, uint8 bind);
static void decodeRequestBindTable(char * strEvent);
static void eventReqIeeeAddr(osal_event_hdr_t *);
static void eventActiveEP(osal_event_hdr_t * hdrEvent);
static void eventSendCmd(osal_event_hdr_t *);
static void attributeValue(osal_event_hdr_t * hdrEvent);
static void eventDeviceInfo(osal_event_hdr_t * hdrEvent);
static void eventBindRequest(osal_event_hdr_t *);
static void eventUnbindRequest(osal_event_hdr_t *);
static void eventBindReq(osal_event_hdr_t *);
static void eventWriteValue(osal_event_hdr_t * hdrEvent);


static uint16 decodeUInt16(char * iter);
static uint8 decodeUInt8(char * iter);
static void decodeExtendedAddress(char * iter, uint8 * extAddress);

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
	} else if (*strEvent == 'B'){
		strEvent++;
		if (*strEvent == 'E'){
			strEvent++;
			if (*strEvent == ':'){
				decodeRequestBindUnBinf(strEvent, 1);
			}
		} else if (*strEvent == 'T'){
			strEvent++;
			if (*strEvent == ':'){
				decodeRequestBindTable(strEvent);
			}
		}
	} else if (*strEvent == 'D'){
		strEvent++;
		if (*strEvent == 'I'){
			strEvent++;
			if (*strEvent == ':'){
				decodeDeviceInfoEvent(strEvent);
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
							msg->msg.event = EVENT_SERIAL_CMD;
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
	} else if (*strEvent == 'U'){
		strEvent++;
		if (*strEvent == 'B'){
			strEvent++;
			if (*strEvent == 'I'){
				strEvent++;
				if (*strEvent == ':'){
					decodeRequestBindUnBinf(strEvent, 0);
				}
			}
		}
	} else if (*strEvent == 'W'){
		strEvent++;
		if (*strEvent == 'A'){
			strEvent++;
			if (*strEvent == 'I'){
				strEvent++;
				if (*strEvent == ':'){
					decodeSendWriteAttribute(strEvent);
				}
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

static void decodeExtendedAddress(char * iter, uint8 * extAddress){
	uint8 i;
	for (i=0; i < Z_EXTADDR_LEN; i++){
		extAddress[i] = decodeHiDigit(*iter++);
		extAddress[i] += decodeLowDigit(*iter++);
	}
}

// Send message: BI: network id, extend address,  endpointId, clusterId, extend address, endpoint Id
//                    4 digits ,  16 digits    ,    2 digits,  4 digits,  16 digits    ,   2 digits
static void decodeRequestBindUnBinf(char * strEvent, uint8 bind){
	strEvent++;
	strEvent++;

	struct BindRequestMsg * msgBind = (struct BindRequestMsg *)osal_msg_allocate(sizeof(struct BindRequestMsg) );

	msgBind->isr.msg.event = EVENT_SERIAL_CMD;
	if (bind)
		msgBind->isr.isr = eventBindRequest;
	else
		msgBind->isr.isr = eventUnbindRequest;
	msgBind->destAddr.addrMode = Addr16Bit;
	msgBind->destAddr.addr.shortAddr = decodeUInt16(strEvent);
	strEvent += 6;
	decodeExtendedAddress(strEvent, msgBind->outClusterAddr);
	strEvent+= 18;
	msgBind->outClusterEP =  decodeUInt8(strEvent);
	strEvent+=4;
	msgBind->clusterID =  decodeUInt16(strEvent);
	strEvent+=6;
	msgBind->inCluster.addrMode = Addr64Bit;
	decodeExtendedAddress(strEvent, msgBind->inCluster.addr.extAddr);
	strEvent+= 18;
	msgBind->inClusterEP =  decodeUInt8(strEvent);
	osal_msg_send(serialDongleTaskID, (uint8 *)msgBind);
}


static void decodeActiveEndpointEvent(char * strEvent){
	strEvent++;
	strEvent++;
	struct ReqActiveEndpointsEvent * msgEP = (struct ReqActiveEndpointsEvent *)osal_msg_allocate(sizeof(struct ReqActiveEndpointsEvent) );
	msgEP->isr.isr = eventActiveEP;
	msgEP->isr.msg.event = EVENT_SERIAL_CMD;
	msgEP->nwkAddr = decodeUInt16(strEvent);
	osal_msg_send(serialDongleTaskID, (uint8 *)msgEP);
}

static void decodeDeviceInfoEvent(char * strEvent) {
	strEvent++;
	strEvent++;
	struct ReqDeviceInformationEvent * msgDI = (struct ReqDeviceInformationEvent *)osal_msg_allocate(sizeof(struct ReqDeviceInformationEvent) );
	msgDI->isr.isr = eventDeviceInfo;
	msgDI->isr.msg.event = EVENT_SERIAL_CMD;
	msgDI->nwkAddr = decodeUInt16(strEvent);
	osal_msg_send(serialDongleTaskID, (uint8 *)msgDI);
}

static void decodeIEEEEvents(char * strEvent) {
	strEvent++;
	strEvent++;
	struct ReqIeeeAddrMsg * msgReq = (struct ReqIeeeAddrMsg *)osal_msg_allocate(sizeof(struct ReqIeeeAddrMsg) );
	msgReq->isr.isr = eventReqIeeeAddr;
	msgReq->isr.msg.event = EVENT_SERIAL_CMD;
	msgReq->nwkAddr = decodeUInt16(strEvent);
	strEvent += 6;
	msgReq->requestType =(*strEvent++) == '0' ? 0 : 1;
	strEvent++; // --> ','	
	strEvent++; // --> ' '	
	msgReq->startIndex = decodeUInt8(strEvent++);
	osal_msg_send(serialDongleTaskID, (uint8 *)msgReq);
}

// Send message: WA: networkid, endpointId, clusterId, attributeId, dataType, , dataLen  ,         data
//                   4 digits ,  2 digits ,  4 digits,  4 digits  ,  2 digits ,  2 digits,  n*2 digits, where n=dataLen
static void decodeSendWriteAttribute(char * strEvent){
	uint8 dataLen;
	uint8 * data;
	struct WriteAttributeValueMsg * msgCmd = (struct WriteAttributeValueMsg *)osal_msg_allocate(sizeof(struct WriteAttributeValueMsg) +sizeof(zclWriteRec_t)  );
	msgCmd->isr.isr = eventWriteValue;
	msgCmd->isr.msg.event = EVENT_SERIAL_CMD;
	msgCmd->afAddrType.addrMode = afAddr16Bit;
	msgCmd->writeCmd.numAttr=1;
	
	strEvent++;
	strEvent++;
	msgCmd->afAddrType.addr.shortAddr = decodeUInt16(strEvent);
	strEvent+=6;
	msgCmd->afAddrType.endPoint  =  decodeUInt8(strEvent);
	strEvent+=4;
	msgCmd->cluster  = decodeUInt16(strEvent);
	strEvent+=6;
	msgCmd->writeCmd.attrList->attrID  = decodeUInt16(strEvent);
	strEvent+=6;
	msgCmd->writeCmd.attrList->dataType =  decodeUInt8(strEvent);
	strEvent+=4;
	dataLen =  decodeUInt8(strEvent);
	strEvent+=4;

	if (dataLen > 0){
		data = msgCmd->writeCmd.attrList->attrData = osal_mem_alloc(dataLen);
		for (;dataLen > 0; dataLen--){
			*data = decodeUInt8(strEvent);
			data++;
			strEvent+=2;
		}
	}
	osal_msg_send(serialDongleTaskID, (uint8 *)msgCmd);
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
	msgCmd->isr.msg.event = EVENT_SERIAL_CMD;
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
	osal_msg_send(serialDongleTaskID, (uint8 *)msgCmd);
}

// decode RA: networkAddress, endpointId, clusterId, attributeId
//              4 digitis   ,  2 digits ,  4digits ,  4 digits
static void decodeRequestAttribute(char * strEvent){
	strEvent++;
	strEvent++;
	struct ReqAttributeMsg * msgAttr = (struct ReqAttributeMsg *)osal_msg_allocate(sizeof(struct ReqAttributeMsg) + sizeof(uint16)  );
	msgAttr->isr.isr = attributeValue;
	msgAttr->isr.msg.event = EVENT_SERIAL_CMD;
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
	msgAttr->isr.msg.event = EVENT_SERIAL_CMD;
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

void decodeRequestBindTable(char * strEvent){
	struct BindTableRequestMsg * msgReq = (struct BindTableRequestMsg *)osal_msg_allocate(sizeof(struct BindTableRequestMsg) );
	strEvent++;
	strEvent++;
	msgReq->isr.isr = eventBindReq;
	msgReq->isr.msg.event = EVENT_SERIAL_CMD;
	msgReq->afAddrType.addrMode = Addr16Bit;
	msgReq->afAddrType.addr.shortAddr= decodeUInt16(strEvent);
	osal_msg_send(serialDongleTaskID, (uint8 *)msgReq);
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


void eventDeviceInfo(osal_event_hdr_t * hdrEvent){
	struct ReqDeviceInformationEvent * msgEP = (struct ReqDeviceInformationEvent *)hdrEvent;
	associated_devices_t * device= AssocGetWithShort( msgEP->nwkAddr);
	if (device == NULL){
	} else {
		serialSendDeviceInfo(device);
	}	
}

void eventBindRequest(osal_event_hdr_t * hdrEvent) {
	struct BindRequestMsg * msg = (struct BindRequestMsg *)hdrEvent;
	
	ZDP_BindReq(&(msg->destAddr),  msg->outClusterAddr, msg->outClusterEP, msg->clusterID, &(msg->inCluster),  msg->inClusterEP, 0);
}

void eventUnbindRequest(osal_event_hdr_t * hdrEvent) {
	struct BindRequestMsg * msg = (struct BindRequestMsg *)hdrEvent;
	
	ZDP_UnbindReq(&(msg->destAddr),  msg->outClusterAddr, msg->outClusterEP, msg->clusterID, &(msg->inCluster),  msg->inClusterEP, 0);
}

void eventBindReq(osal_event_hdr_t * hdrEvent) {
	uint8 first=0;
	struct BindTableRequestMsg * msg = (struct BindTableRequestMsg *)hdrEvent;
	
	ZDP_MgmtBindReq( &(msg->afAddrType),first, 0 );
}

void eventWriteValue(osal_event_hdr_t * hdrEvent) {
	struct WriteAttributeValueMsg * msgCmd = (struct WriteAttributeValueMsg * )hdrEvent;
	
	zcl_SendWriteRequest(ENDPOINT, &(msgCmd->afAddrType), msgCmd->cluster,&(msgCmd->writeCmd),  ZCL_CMD_WRITE_NO_RSP, ZCL_FRAME_CLIENT_SERVER_DIR, 0, 0);
}
