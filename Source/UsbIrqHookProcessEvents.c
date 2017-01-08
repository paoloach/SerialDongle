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

static void eventWriteValue(osal_event_hdr_t *);

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


void eventWriteValue(osal_event_hdr_t * hdrEvent) {
	struct WriteAttributeValueMsg * msgCmd = (struct WriteAttributeValueMsg * )hdrEvent;
	
	zcl_SendWriteRequest(ENDPOINT, &(msgCmd->afAddrType), msgCmd->cluster,&(msgCmd->writeCmd),  ZCL_CMD_WRITE_NO_RSP, ZCL_FRAME_CLIENT_SERVER_DIR, 0, 0);
}