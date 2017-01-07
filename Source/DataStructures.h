/**************************************************************************************************

 DESCRIPTION:
  --

 CREATED: 29/12/2016, by Paolo Achdjian

 FILE: DataStructures.h

***************************************************************************************************/
#ifndef __DATA_STRUCTURES__H__
#define __DATA_STRUCTURES__H__

#include "zcl.h"

typedef void (*IsrMethod)(osal_event_hdr_t *);

struct UsbISR {
	osal_event_hdr_t msg;
	IsrMethod isr;
	uint16	time;
};

struct GenericDataMsg{
	uint8 msgCode;
};

struct BindTableResponseEntry{
	uint8 srcAddr[Z_EXTADDR_LEN];
 	byte srcEP; 
 	uint16 clusterID;
 	uint16 dstAddr;
 	byte dstEP; 
};

struct BindTableResponse {
	struct GenericDataMsg generticDataMsg;
	uint8 elementSize;
	struct BindTableResponseEntry list[];
};

struct ReqAttributeMsg {
	struct UsbISR	isr;
	afAddrType_t afAddrType;
	uint16	cluster;
	uint8  numAttr;   
	uint16 attrID[];
};
struct ReqActiveEndpointsEvent {
	struct UsbISR	isr;
	union  {
		uint16      nwkAddr;
		uint8		data[2];
	};
};

struct ReqDeviceInformationEvent {
	struct UsbISR	isr;
	union  {
		uint16      nwkAddr;
		uint8		data[2];
	};
};

struct SendCmdMsg{
	struct UsbISR	isr;
	afAddrType_t afAddrType;
	uint16		cluster;
	uint16      cmdClusterId;
	uint8		dataLen;
	uint8 		data[];
};

struct BindTableRequestMsg{
	struct UsbISR	isr;
	zAddrType_t afAddrType;
};

struct WriteAttributeValueMsg {
	struct UsbISR	isr;
	afAddrType_t afAddrType;
	uint16 		 cluster;
	zclWriteCmd_t writeCmd;
};

struct BindRequestMsg {
	struct UsbISR	isr;
	 zAddrType_t destAddr;
	 uint8 outClusterAddr[Z_EXTADDR_LEN];
	 uint8 outClusterEP;     
	 uint16 clusterID; 
	 zAddrType_t inCluster;
	 uint8 inClusterEP;
};

#endif