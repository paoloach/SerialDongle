#include "SerialProcessEventsInternal.h"
#include "DataStructures.h"
#include "TimerEvents.h"
#include "ZDProfile.h"

struct ReqIeeeAddrMsg {
	struct UsbISR	isr;
	union  nwkAddr {
		uint16      nwkAddr;
		uint8		data[2];
	};
	uint8		requestType;
	uint8 		startIndex;
};


 void decodeIEEEEvents() {
	incDataRead;
	struct ReqIeeeAddrMsg * msgReq = (struct ReqIeeeAddrMsg *)osal_msg_allocate(sizeof(struct ReqIeeeAddrMsg) );
	msgReq->isr.isr = eventReqIeeeAddr;
	msgReq->isr.msg.event = EVENT_SERIAL_CMD;
	msgReq->nwkAddr = decodeUInt16();
	incDataRead;
	incDataRead;
	msgReq->requestType =(*rxDataRead) == '0' ? 0 : 1;
	incDataRead;
	incDataRead; // --> ','	
	incDataRead; // --> ' '	
	msgReq->startIndex = decodeUInt8();
	osal_msg_send(serialDongleTaskID, (uint8 *)msgReq);
}

void eventReqIeeeAddr(osal_event_hdr_t * hdrEvent) {
	struct ReqIeeeAddrMsg * msg = (struct ReqIeeeAddrMsg *)hdrEvent;
	ZDP_IEEEAddrReq( msg->nwkAddr, msg->requestType, msg->startIndex, 0); 
}