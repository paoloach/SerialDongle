/**************************************************************************************************
  Filename:       ZdoMessageHandlers.h


  Copyright 2014 Paolo Achdjian

**************************************************************************************************/

#include "ZDObject.h"
#include "ZdoMessageHandlers.h"
#include "hal_board.h"
#include "hal_types.h"
#include "endpointRequestList.h"
//#include "SerialFunctions.h"
#include "SerialSend/SendMethods.h"
#include "CheckingChildList.h"

/*********************************************************************
 * LOCAL VARIABLES
 */
static ZDO_DeviceAnnce_t device;
static ZDO_SimpleDescRsp_t simpleDesc;
static uint8 *msg;
static uint8 len;
static uint8 i;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void notHandledMessage(zdoIncomingMsg_t * msg);
static void annunceMessage(zdoIncomingMsg_t * msg);
static void activeEndpointResponseMessage(zdoIncomingMsg_t * msg);
static void simpleDecriptorMessage(zdoIncomingMsg_t * msg);
static void mgmtBindResponseMessage(zdoIncomingMsg_t * msg);
static void ieeeAddrResponseMessage(zdoIncomingMsg_t * msg);

ZDOMessageHandler ZDOMessageHandlerFactory(cId_t clusterId) {
	ZDOMessageHandler zdoMessageHandler;
	
	switch(clusterId){
		case Device_annce:
			zdoMessageHandler = annunceMessage;
			break;
		case Active_EP_rsp:
			zdoMessageHandler = activeEndpointResponseMessage;
			break;
		case Simple_Desc_rsp:
			zdoMessageHandler = simpleDecriptorMessage;
			break;
		case Mgmt_Bind_rsp:
			zdoMessageHandler = mgmtBindResponseMessage;
			break;
		case Mgmt_Lqi_rsp:
			zdoMessageHandler = mgmtLqiResponseMessage;
			break;			
		case IEEE_addr_rsp:
		case NWK_addr_rsp:
			zdoMessageHandler =ieeeAddrResponseMessage;
			break;
		case Power_Desc_rsp:
			zdoMessageHandler = nodePowerResponseMessage;
			break;
		case Node_Desc_rsp:
			zdoMessageHandler = nodeDescriptionResponseMessage;
			break;
		default:
			zdoMessageHandler = notHandledMessage;
	}
	return zdoMessageHandler;
}


static void notHandledMessage(zdoIncomingMsg_t * msg) {
}


static void annunceMessage(zdoIncomingMsg_t * msg) {
	ZDO_ParseDeviceAnnce( msg, &device );
	serialSendAnnunce(&device);
}

static void activeEndpointResponseMessage(zdoIncomingMsg_t * msg) {
	ZDO_ActiveEndpointRsp_t * EPList = ZDO_ParseEPListRsp(msg );
	for (int i=0; i < EPList->cnt; i++){
		addEndpointRequest( EPList->nwkAddr, EPList->epList[i], 0);
	}
	sendOneEndpointRequest();
	osal_mem_free(EPList);
}

static void simpleDecriptorMessage(zdoIncomingMsg_t * msg) {
	ZDO_ParseSimpleDescRsp( msg, &simpleDesc );
	if (simpleDesc.status == ZDP_SUCCESS){
		serialSendSimpleDescriptor(&simpleDesc);
		if (simpleDesc.simpleDesc.pAppInClusterList!=NULL)
			osal_mem_free(simpleDesc.simpleDesc.pAppInClusterList);
		if (simpleDesc.simpleDesc.pAppOutClusterList!=NULL)
			osal_mem_free(simpleDesc.simpleDesc.pAppOutClusterList);
	}
}

static void ieeeAddrResponseMessage(zdoIncomingMsg_t * msg) {
	serialSendIeeeAddress(msg);
}

static void mgmtBindResponseMessage(zdoIncomingMsg_t * inMsg) {
	struct BindTableResponseEntry bindTableEntry;

	msg =  inMsg->asdu;
	if (*msg++ == ZSuccess ){
		msg++;
		msg++;
		len = *msg++;
	} else {
		return;
	}

	for (i=0; i< len; i++){
		osal_memcpy(bindTableEntry.srcAddr, msg, SADDR_EXT_LEN);
		msg += Z_EXTADDR_LEN;
		bindTableEntry.srcEP = *msg++;
		bindTableEntry.clusterID = BUILD_UINT16( msg[0], msg[1] );
		msg += 2;
		if (*msg++ == Addr64Bit){
			msg += Z_EXTADDR_LEN;
		} else {
			bindTableEntry.dstAddr = BUILD_UINT16( msg[0], msg[1] );
			msg += 2;
		}
		bindTableEntry.dstEP = *msg++;
		serialSendBindTable(&bindTableEntry);
	}
}