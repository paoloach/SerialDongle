
#include "SerialProcessEvents.h"
#include "DataStructures.h"
#include "TimerEvents.h"
#include "SerialProcessEventsInternal.h"
#include "OnBoard.h"


void reset() {
	struct UsbISR * msg = (struct UsbISR *)osal_msg_allocate(sizeof(struct UsbISR) );
	msg->msg.event = EVENT_SERIAL_CMD;
	msg->isr = eventReset;
}

void eventReset(osal_event_hdr_t * hdrEvent) {
	SystemReset();
}