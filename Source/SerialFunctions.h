/**************************************************************************************************

 DESCRIPTION:
  --

 CREATED: 29/12/2016, by Paolo Achdjian

 FILE: SerialFunctions.h

***************************************************************************************************/

#ifndef __SERIAL_FUNCTIONS__H_
#define __SERIAL_FUNCTIONS__H_

#include "AssocList.h"
#include "AF.h"
#include "zcl.h" 
#include "ZDObject.h"
#include "DataStructures.h"


void serialInit(void);
void serialLoop(void);
void sendMessage(char * string);
void serialSendDevice(associated_devices_t * device);
void serialSendDeviceInfo(associated_devices_t * device);
void serialSendActiveEPError(uint16 nwkAdd, afStatus_t ret);
void serialSendAttributeResponseMsgError(struct ReqAttributeMsg * attr, ZStatus_t status);
void serialSendAttributeResponseMsg(zclReadRspCmd_t *, uint16 clusterId, afAddrType_t * addr);
void serialSendAnnunce(ZDO_DeviceAnnce_t * );
void serialSendSimpleDescriptor(ZDO_SimpleDescRsp_t *);
void serialSendIeeeAddress(zdoIncomingMsg_t * );
void serialSendBindTable(struct BindTableResponseEntry * bindTable);
void nodePowerResponseMessage(zdoIncomingMsg_t * msg);
#endif