#ifndef __SERIAL_PROCESS_EVENTS__H__
#define __SERIAL_PROCESS_EVENTS__H__
#include "OSAL.h"
#include "zcl_general.h"

uint8 decodeLowDigit(void);
uint8 decodeHiDigit(void);

uint16 decodeUInt16(void);
uint8 decodeUInt8(void);
void decodeExtendedAddress( uint8 * extAddress);

void reset(void);
void eventReset(osal_event_hdr_t *);
void decodeIEEEEvents(void);
void decodeINIT(void);
void decodeNodePowerEvents(void);
void decodeActiveEndpointEvent(void);
void decodeRequestAttribute(void);
void decodeRequestAttributes(void);
 void decodeSendComand(void);
void decodeSendWriteAttribute(void);
void decodeDeviceInfoEvent(void);
void decodeRequestBind(void);
void decodeRequestUnbind(void);
void decodeRequestBindTable(void);
void eventInit(osal_event_hdr_t *);
void eventReqIeeeAddr(osal_event_hdr_t *);
void eventReqNodePower(osal_event_hdr_t *);
void eventActiveEP(osal_event_hdr_t * hdrEvent);
void eventSendCmd(osal_event_hdr_t *);
void attributeValue(osal_event_hdr_t * hdrEvent);
void eventDeviceInfo(osal_event_hdr_t * hdrEvent);
void eventBindRequest(osal_event_hdr_t *);
void eventUnbindRequest(osal_event_hdr_t *);
void eventBindReq(osal_event_hdr_t *);
void eventWriteValue(osal_event_hdr_t * hdrEvent);

extern byte serialDongleTaskID;

extern char rxData[];
//extern char * rxDataEnd;
extern char * rxDataRead;
#define RX_BUFFER_SIZE 128
#define  rxDataEnd rxData+RX_BUFFER_SIZE



#define incDataRead 	rxDataRead++;\
						if (rxDataRead >= rxDataEnd) rxDataRead = rxData
#endif