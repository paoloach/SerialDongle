/**************************************************************************************************
  Filename:       ZdoMessageHandlers.h


  Copyright 2014 Paolo Achdjian

**************************************************************************************************/

#include "ZDObject.h"
#include "ZdoMessageHandlers.h"
#include "hal_board.h"
#include "hal_types.h"
#ifndef NO_SERIAL
#include "endpointRequestList.h"

#include "SerialSend/SendMethods.h"
#include "CheckingChildList.h"
#endif
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
			#ifndef NO_SERIAL
			zdoMessageHandler = mgmtLqiResponseMessage;
#endif
			break;			
		case IEEE_addr_rsp:
		case NWK_addr_rsp:
			zdoMessageHandler =ieeeAddrResponseMessage;
			break;
#ifndef NO_SERIAL			
		case Power_Desc_rsp:
			zdoMessageHandler = nodePowerResponseMessage;
			break;
		case Node_Desc_rsp:
			zdoMessageHandler = nodeDescriptionResponseMessage;
			break;
#endif			
		default:
			zdoMessageHandler = notHandledMessage;
	}
	return zdoMessageHandler;
}


static void notHandledMessage(zdoIncomingMsg_t * msg) {
}


static void annunceMessage(zdoIncomingMsg_t * msg) {
	ZDO_ParseDeviceAnnce( msg, &device );
	#ifndef NO_SERIAL
	serialSendAnnunce(&device);
#endif
}

static void activeEndpointResponseMessage(zdoIncomingMsg_t * msg) {
	ZDO_ActiveEndpointRsp_t * EPList = ZDO_ParseEPListRsp(msg );
#ifndef NO_SERIAL
	for (int i=0; i < EPList->cnt; i++){
		addEndpointRequest( EPList->nwkAddr, EPList->epList[i], 0);
	}
	sendOneEndpointRequest();
#endif
	osal_mem_free(EPList);
}

static void simpleDecriptorMessage(zdoIncomingMsg_t * msg) {
	ZDO_ParseSimpleDescRsp( msg, &simpleDesc );
	if (simpleDesc.status == ZDP_SUCCESS){
#ifndef NO_SERIAL
		serialSendSimpleDescriptor(&simpleDesc);
#endif
		if (simpleDesc.simpleDesc.pAppInClusterList!=NULL)
			osal_mem_free(simpleDesc.simpleDesc.pAppInClusterList);
		if (simpleDesc.simpleDesc.pAppOutClusterList!=NULL)
			osal_mem_free(simpleDesc.simpleDesc.pAppOutClusterList);
	}
	
}

static void ieeeAddrResponseMessage(zdoIncomingMsg_t * msg) {
#ifndef NO_SERIAL
	serialSendIeeeAddress(msg);
#endif
}

static void mgmtBindResponseMessage(zdoIncomingMsg_t * inMsg) {
#ifndef NO_SERIAL
	struct BindTableResponseEntry bindTableEntry;
#endif
	msg =  inMsg->asdu;
	if (*msg++ == ZSuccess ){
		msg++;
		msg++;
		len = *msg++;
	} else {
		return;
	}

	for (i=0; i< len; i++){
#ifndef NO_SERIAL
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
#endif
	}
}