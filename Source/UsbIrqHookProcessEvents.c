/**************************************************************************************************

 DESCRIPTION:
  --

 CREATED: 17/08/2016, by Paolo Achdjian

 FILE: UsbIrqHookProcessEvents.c

***************************************************************************************************/

#include "usb_framework.h"
#include "usb_interrupt.h"
#include "mac_mcu.h"
#include "TimerEvents.h"
#include "ZComDef.h"
#include "OSAL.h"
#include "OnBoard.h"
#include "UsbIrqHookProcessEvents.h"
#include "ZigBeeUsbBridge_data.h"
#include "ZDProfile.h"
#include "SerialFunctions.h"
#include "DataStructures.h"


extern byte serialDongleTaskID;
extern uint16  currentDeviceElement;
extern uint8  usbOn;

struct ReqAttributeValueMsg {
	uint16      nwkAddr;
	uint8       endpoint;
	uint16		cluster;
	uint8		numAttributes;
};

struct SendCmdUsbMsg {
	uint16      nwkAddr;
	uint8       endpoint;
	uint16		cluster;
	uint16      cmdClusterId;
	uint8		dataLen;
};

struct WriteAttributeValueUsbMsg {
	uint16      nwkAddr;
	uint8       endpoint;
	uint16		cluster;
	uint16      attributeId;
    uint8       dataType;
	uint8       dataValueLen;
	uint8       dataValue[];
};



static void attributeValue(osal_event_hdr_t *);

static void eventDeviceInfo(osal_event_hdr_t *);
static void eventBindReq(osal_event_hdr_t *);
static void eventWriteValue(osal_event_hdr_t *);
static void eventBindRequest(osal_event_hdr_t *);
static void eventUnbindRequest(osal_event_hdr_t *);
static struct UsbISR * createMsgForBind(void);

// ************************ USB interrupt event processing *************************
void usbirqHookProcessEvents(void)
{
	T1CNTL=0;
	if (usbirqData.eventMask & USBIRQ_EVENT_EP2OUT){
		 uint8 oldEndpoint = USBFW_GET_SELECTED_ENDPOINT();
	     USBFW_SELECT_ENDPOINT(2);

		uint8 length = USBFW_GET_OUT_ENDPOINT_COUNT_LOW();
 	    

   		if (length) {
			struct UsbISR * msg =NULL;
			uint8 code = USBF2;
			switch( code){
			case REQ_BIND_TABLE: {
					uint8 addr[2];
					addr[0] = USBF2;
					addr[1] = USBF2;
					struct BindTableRequestMsg * msgReq = (struct BindTableRequestMsg *)osal_msg_allocate(sizeof(struct BindTableRequestMsg) );
					msg = &(msgReq->isr);
					msg->isr = eventBindReq;
					msg->msg.event = EVENT_USB_ISR;
					msgReq->afAddrType.addrMode = Addr16Bit;
					msgReq->afAddrType.addr.shortAddr = *(uint16 *)(addr);
					}
					break;
			case REQ_ADD_BIND_TABLE_ENTRY:
					msg = createMsgForBind();
					msg->isr = eventBindRequest;
					break;
			case REQ_REMOVE_BIND_TABLE_ENTRY:
					msg = createMsgForBind();
					msg->isr = eventUnbindRequest;
					break;
			case WRITE_ATTRIBUTE_VALUE:{
					struct WriteAttributeValueUsbMsg usbMsg;
					uint8  * data = (uint8 *)(&usbMsg);
					uint8  i;
					for(i=0; i < sizeof(struct WriteAttributeValueUsbMsg); i++){
						*data = USBF2;
						data++;
					}
					struct WriteAttributeValueMsg * msgCmd = (struct WriteAttributeValueMsg *)osal_msg_allocate(sizeof(struct WriteAttributeValueMsg) +sizeof(zclWriteRec_t) + usbMsg.dataValueLen  );
					msg = &(msgCmd->isr);
					msg->isr = eventWriteValue;
					msg->msg.event = EVENT_USB_ISR;
					
					msgCmd->afAddrType.addrMode=afAddr16Bit;
					msgCmd->afAddrType.addr.shortAddr=usbMsg.nwkAddr;
					msgCmd->afAddrType.endPoint=usbMsg.endpoint;
					msgCmd->cluster = usbMsg.cluster;
					msgCmd->writeCmd.numAttr=1;
					msgCmd->writeCmd.attrList->attrID = usbMsg.attributeId;
					msgCmd->writeCmd.attrList->dataType=usbMsg.dataType;
					data = ((uint8 *)msgCmd) + sizeof(struct WriteAttributeValueMsg) +sizeof(zclWriteRec_t);
					msgCmd->writeCmd.attrList->attrData = data;
					for(i=0; i < usbMsg.dataValueLen; i++){
						*data = USBF2;
						data++;
					}
					}
					break;
			
			case REQ_DEVICE_INFO:{
				struct ReqDeviceInformationEvent * msgEP = (struct ReqDeviceInformationEvent *)osal_msg_allocate(sizeof(struct ReqDeviceInformationEvent) );
				msg = &(msgEP->isr);
				msg->isr = eventDeviceInfo;
				msg->msg.event = EVENT_USB_ISR;
				msgEP->data[0] = USBF2;
				msgEP->data[1] = USBF2;
				}
				break;
					
			}
			if (msg != NULL) {
				uint8 low = T1CNTL;
				uint8 hi = T1CNTH;
				msg->time=BUILD_UINT16(low,hi);
				osal_msg_send(serialDongleTaskID, (uint8 *)msg);
			}
			/*uint8 __generic *pTemp = rxData;
      		do {
         		*(pTemp++) = USBF2;
      		} while (--length);*/
   		}
      
		USBFW_ARM_OUT_ENDPOINT();
		USBFW_SELECT_ENDPOINT(oldEndpoint);
	}
}

static struct UsbISR * createMsgForBind(void) {
	uint8 * data;
	uint8  i;
	struct BindRequestMsg * msgBind = (struct BindRequestMsg *)osal_msg_allocate(sizeof(struct BindRequestMsg) );
	msgBind->isr.msg.event = EVENT_USB_ISR;
	msgBind->destAddr.addrMode = Addr16Bit;
	data = (uint8 *)msgBind->destAddr.addr.shortAddr;
	*data = USBF2;
	data++;
	*data = USBF2;
	data = (uint8 *)msgBind->outClusterAddr;
	for (i=0; i < Z_EXTADDR_LEN+3; i++){
		*data = USBF2;
		data++;
	}

	msgBind->inCluster.addrMode = Addr64Bit;
	data = (uint8 *)msgBind->inCluster.addr.extAddr;
	for(i=0; i< Z_EXTADDR_LEN; i++){
		*data = USBF2;
		data++;
	}
	msgBind->inClusterEP = USBF2;
	return 	&(msgBind->isr);			
}

void eventDeviceInfo(osal_event_hdr_t * hdrEvent){
	struct ReqDeviceInformationEvent * msgEP = (struct ReqDeviceInformationEvent *)hdrEvent;
	associated_devices_t * device= AssocGetWithShort( msgEP->nwkAddr);
	if (device == NULL){
	} else {
		serialSendDeviceInfo(device);
	}	
}


void eventBindReq(osal_event_hdr_t * hdrEvent) {
	uint8 first=0;
	struct BindTableRequestMsg * msg = (struct BindTableRequestMsg *)hdrEvent;
	
	ZDP_MgmtBindReq( &(msg->afAddrType),first, 0 );
}

void eventBindRequest(osal_event_hdr_t * hdrEvent) {
	struct BindRequestMsg * msg = (struct BindRequestMsg *)hdrEvent;
	
	ZDP_BindReq(&(msg->destAddr),  msg->outClusterAddr, msg->outClusterEP, msg->clusterID, &(msg->inCluster),  msg->inClusterEP, 0);
}

void eventUnbindRequest(osal_event_hdr_t * hdrEvent) {
	struct BindRequestMsg * msg = (struct BindRequestMsg *)hdrEvent;
	
	ZDP_UnbindReq(&(msg->destAddr),  msg->outClusterAddr, msg->outClusterEP, msg->clusterID, &(msg->inCluster),  msg->inClusterEP, 0);
}

void eventWriteValue(osal_event_hdr_t * hdrEvent) {
	struct WriteAttributeValueMsg * msgCmd = (struct WriteAttributeValueMsg * )hdrEvent;
	
	zcl_SendWriteRequest(ENDPOINT, &(msgCmd->afAddrType), msgCmd->cluster,&(msgCmd->writeCmd),  ZCL_CMD_WRITE_NO_RSP, ZCL_FRAME_CLIENT_SERVER_DIR, 0, 0);
}