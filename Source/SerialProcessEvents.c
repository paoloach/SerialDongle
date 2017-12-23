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
#include "SerialProcessEventsInternal.h"
#include "TimerEvents.h"
#include "DataStructures.h"
//#include "SerialFunctions.h"
#include "SerialSend/SendMethods.h"
#include "ZigbeeSerialBridge.h"
#include "ZigbeeSerialBridge_data.h"
#include "CheckingChildList.h"

 uint8 decodeHiDigit() {
	uint8 c = *rxDataRead;
	incDataRead;
	if (c >= '0' && c <= '9')
		return (c -'0') << 4;
	else
		return (10 +(c - 'A')) << 4;
}

 uint16 decodeUInt16(){
	uint8 data[2];
	data[1] = decodeHiDigit();
	data[1] += decodeLowDigit();
	data[0] = decodeHiDigit();
	data[0] += decodeLowDigit();
	return *((uint16 *)data);
}

 uint8 decodeUInt8(){
	uint8 data;
	data = decodeHiDigit();
	data += decodeLowDigit();
	return data;
}

  void decodeExtendedAddress( uint8 * extAddress){
	uint8 i;
	for (i=0; i < Z_EXTADDR_LEN; i++){
		extAddress[i] = decodeHiDigit();
		extAddress[i] += decodeLowDigit();
	}
}

static void decodeRequestBindUnBinf(uint8 bind);
 void decodeRequestBind(void){
	 decodeRequestBindUnBinf(1);
 }
 
  void decodeRequestUnbind(void){
	 decodeRequestBindUnBinf(0);
 }

// Send message: BI: network id, extend address,  endpointId, clusterId, extend address, endpoint Id
//                    4 digits ,  16 digits    ,    2 digits,  4 digits,  16 digits    ,   2 digits
  void decodeRequestBindUnBinf(uint8 bind){
	incDataRead;

	struct BindRequestMsg * msgBind = (struct BindRequestMsg *)osal_msg_allocate(sizeof(struct BindRequestMsg) );

	msgBind->isr.msg.event = EVENT_SERIAL_CMD;
	if (bind)
		msgBind->isr.isr = eventBindRequest;
	else
		msgBind->isr.isr = eventUnbindRequest;
	msgBind->destAddr.addrMode = Addr16Bit;
	msgBind->destAddr.addr.shortAddr = decodeUInt16();
	incDataRead;
	incDataRead;
	decodeExtendedAddress( msgBind->outClusterAddr);
	incDataRead;
	incDataRead;
	msgBind->outClusterEP =  decodeUInt8();
	incDataRead;
	incDataRead;
	msgBind->clusterID =  decodeUInt16();
	incDataRead;
	incDataRead;
	msgBind->inCluster.addrMode = Addr64Bit;
	decodeExtendedAddress(msgBind->inCluster.addr.extAddr);
	incDataRead;
	incDataRead;
	msgBind->inClusterEP =  decodeUInt8();
	osal_msg_send(serialDongleTaskID, (uint8 *)msgBind);
}


  void decodeActiveEndpointEvent(){
	incDataRead;
	struct ReqActiveEndpointsEvent * msgEP = (struct ReqActiveEndpointsEvent *)osal_msg_allocate(sizeof(struct ReqActiveEndpointsEvent) );
	msgEP->isr.isr = eventActiveEP;
	msgEP->isr.msg.event = EVENT_SERIAL_CMD;
	msgEP->nwkAddr = decodeUInt16();
	osal_msg_send(serialDongleTaskID, (uint8 *)msgEP);
}

  void decodeDeviceInfoEvent() {
	incDataRead;
	struct ReqDeviceInformationEvent * msgDI = (struct ReqDeviceInformationEvent *)osal_msg_allocate(sizeof(struct ReqDeviceInformationEvent) );
	msgDI->isr.isr = eventDeviceInfo;
	msgDI->isr.msg.event = EVENT_SERIAL_CMD;
	msgDI->nwkAddr = decodeUInt16();
	osal_msg_send(serialDongleTaskID, (uint8 *)msgDI);
}

  void decodeNodePowerEvents () {
	incDataRead;
	struct ReqDeviceNodePower * msgReq = (struct ReqDeviceNodePower *)osal_msg_allocate(sizeof(struct ReqDeviceNodePower) );
	msgReq->isr.isr = eventReqNodePower;
	msgReq->isr.msg.event = EVENT_SERIAL_CMD;
	msgReq->nwkAddr = decodeUInt16();
	osal_msg_send(serialDongleTaskID, (uint8 *)msgReq);
}




  void decodeINIT(void) {
	struct UsbISR * isr  = (struct UsbISR *)osal_msg_allocate(sizeof(struct UsbISR) );
	isr->isr = eventInit;
	isr->msg.event = EVENT_SERIAL_CMD;
	osal_msg_send(serialDongleTaskID, (uint8 *)isr);

}

// Send message: WA: networkid, endpointId, clusterId, attributeId, dataType, , dataLen  ,         data
//                   4 digits ,  2 digits ,  4 digits,  4 digits  ,  2 digits ,  2 digits,  n*2 digits, where n=dataLen
  void decodeSendWriteAttribute(){
	uint8 dataLen;
	uint8 * data;
	struct WriteAttributeValueMsg * msgCmd = (struct WriteAttributeValueMsg *)osal_msg_allocate(sizeof(struct WriteAttributeValueMsg) +sizeof(zclWriteRec_t)  );
	msgCmd->isr.isr = eventWriteValue;
	msgCmd->isr.msg.event = EVENT_SERIAL_CMD;
	msgCmd->afAddrType.addrMode = afAddr16Bit;
	msgCmd->writeCmd.numAttr=1;
	
	incDataRead;
	msgCmd->afAddrType.addr.shortAddr = decodeUInt16();
	incDataRead;
	incDataRead;
	msgCmd->afAddrType.endPoint  =  decodeUInt8();
	incDataRead;
	incDataRead;
	msgCmd->cluster  = decodeUInt16();
	incDataRead;
	incDataRead;
	msgCmd->writeCmd.attrList->attrID  = decodeUInt16();
	incDataRead;
	incDataRead;
	msgCmd->writeCmd.attrList->dataType =  decodeUInt8();
	incDataRead;
	incDataRead;
	dataLen =  decodeUInt8();


	if (dataLen > 0){
		incDataRead;
		incDataRead;
		data = msgCmd->writeCmd.attrList->attrData = osal_mem_alloc(dataLen);
		for (;dataLen > 0; dataLen--){
			*data = decodeUInt8();
			if (dataLen > 1){
				data++;
			}
		}
	}
	osal_msg_send(serialDongleTaskID, (uint8 *)msgCmd);
}

// Send message: SC: networkid, endpointId, clusterId, commandId, dataLen  , data
//                   4 digits ,  2 digits ,  4 digits,  4 digits,  2 digits,  n*2 digits, where n=dataLen
  void decodeSendComand(){
	uint16 networkid, clusterId, commandId;
	uint8  endpointId, dataLen;
	uint8 * data;
	
	incDataRead;
	networkid = decodeUInt16();
	incDataRead;
	incDataRead;
	endpointId =  decodeUInt8();
	incDataRead;
	incDataRead;
	clusterId = decodeUInt16();
	incDataRead;
	incDataRead;
	commandId = decodeUInt16();
	incDataRead;
	incDataRead;
	dataLen =  decodeUInt8();
	
	struct SendCmdMsg * msgCmd = (struct SendCmdMsg *)osal_msg_allocate(sizeof(struct SendCmdMsg) +dataLen  );
	msgCmd->isr.isr = eventSendCmd;
	msgCmd->isr.msg.event = EVENT_SERIAL_CMD;
	msgCmd->afAddrType.addrMode = afAddr16Bit;
	msgCmd->afAddrType.addr.shortAddr = networkid;
	msgCmd->afAddrType.endPoint = endpointId;
	msgCmd->cluster = clusterId;
	msgCmd->cmdClusterId = commandId;
	msgCmd->dataLen = dataLen;
	if (dataLen > 0){
		incDataRead;
		incDataRead;
		data = (uint8 *)(msgCmd->data);
		for (;dataLen > 0; dataLen--){
			*data = decodeUInt8();
			if (dataLen > 1){
				data++;	
			}
		}
	}
	osal_msg_send(serialDongleTaskID, (uint8 *)msgCmd);
}

// decode RA: networkAddress, endpointId, clusterId, attributeId
//              4 digitis   ,  2 digits ,  4digits ,  4 digits
  void decodeRequestAttribute(){
	incDataRead;
	struct ReqAttributeMsg * msgAttr = (struct ReqAttributeMsg *)osal_msg_allocate(sizeof(struct ReqAttributeMsg) + sizeof(uint16)  );
	msgAttr->isr.isr = attributeValue;
	msgAttr->isr.msg.event = EVENT_SERIAL_CMD;
	msgAttr->afAddrType.addr.shortAddr = decodeUInt16();
	incDataRead;
	incDataRead;
	msgAttr->afAddrType.addrMode = afAddr16Bit;
	msgAttr->afAddrType.endPoint =  decodeUInt8();
	incDataRead;
	incDataRead;
	msgAttr->cluster = decodeUInt16();
	incDataRead;
	incDataRead;
	msgAttr->numAttr=1;
	msgAttr->attrID[0] = decodeUInt16();
	osal_msg_send(serialDongleTaskID, (uint8 *)msgAttr);
}

// decode:  RAS: networkid, endpointId, clusterId, attributesNum, first attributed id, ..., last attribute id
//                4 digits, 2 digits  ,  4 digits, 2 digits     ,  4 digits          , ...,    4  digits
  void decodeRequestAttributes(){
	uint16 networkid, clusterId;
	uint8  i, endpointId, attributeNum;
	
	incDataRead;
	networkid = decodeUInt16();
	incDataRead;
	incDataRead;
	endpointId =  decodeUInt8();
	incDataRead;
	incDataRead;
	clusterId = decodeUInt16();
	incDataRead;
	incDataRead;
	attributeNum =  decodeUInt8();
	incDataRead;
	incDataRead;
	
	struct ReqAttributeMsg * msgAttr = (struct ReqAttributeMsg *)osal_msg_allocate(sizeof(struct ReqAttributeMsg) + attributeNum*sizeof(uint16)  );
	msgAttr->isr.isr = attributeValue;
	msgAttr->isr.msg.event = EVENT_SERIAL_CMD;
	msgAttr->afAddrType.addr.shortAddr = networkid;
	msgAttr->afAddrType.addrMode = afAddr16Bit;
	msgAttr->afAddrType.endPoint =  endpointId;
	msgAttr->cluster = clusterId;
	msgAttr->numAttr=attributeNum;
	for (i=0; i < attributeNum; i++){
		msgAttr->attrID[i] = decodeUInt16();
		if (i < (attributeNum-1) ){
			incDataRead;
			incDataRead;
		}
	}
	osal_msg_send(serialDongleTaskID, (uint8 *)msgAttr);
}

void decodeRequestBindTable(){
	struct BindTableRequestMsg * msgReq = (struct BindTableRequestMsg *)osal_msg_allocate(sizeof(struct BindTableRequestMsg) );
	incDataRead;
	msgReq->isr.isr = eventBindReq;
	msgReq->isr.msg.event = EVENT_SERIAL_CMD;
	msgReq->afAddrType.addrMode = Addr16Bit;
	msgReq->afAddrType.addr.shortAddr= decodeUInt16();
	osal_msg_send(serialDongleTaskID, (uint8 *)msgReq);
}

void eventInit(osal_event_hdr_t * hdrEvent) {
	initCheckingChildList(serialDongleTaskID);
}



void eventReqNodePower(osal_event_hdr_t * hdrEvent) {
	zAddrType_t destAddr;
	
	struct ReqDeviceNodePower * msg = (struct ReqDeviceNodePower *)hdrEvent;
	destAddr.addrMode = Addr16Bit;
	destAddr.addr.shortAddr = msg->nwkAddr;
	
	afStatus_t ret = ZDP_NWKAddrOfInterestReq(&destAddr,msg->nwkAddr, Power_Desc_req, 0);
	if (ret != ZSuccess){
		nodePowerResponseMessageError(msg->nwkAddr, ret);
	}
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
