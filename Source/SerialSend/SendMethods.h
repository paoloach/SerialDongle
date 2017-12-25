/**************************************************************************************************

 DESCRIPTION:
  --

 CREATED: 19/12/2017, by Paolo Achdjian

 FILE: SendMethods.h

***************************************************************************************************/

#include "AF.h"
#include "zcl.h"
#include "AssocList.h"
#include "SerialSend.h"
#include "DataStructures.h"
#include "ZDObject.h"

void sendMessage(char * msg);
void sendAliveMsg(void);
void serialSendDeviceInfo(associated_devices_t * device);
void serialSendActiveEPError(uint16 nwkAdd, afStatus_t ret);
void serialSendAttributeResponseMsgError(struct ReqAttributeMsg * attr, ZStatus_t status);
void serialSendAttributeResponseMsg(zclReadRspCmd_t * readRsp, uint16 clusterId, afAddrType_t * addr);
void serialSendAnnunce(ZDO_DeviceAnnce_t * deviceAnnce );
void nodePowerResponseMessageError(uint16 nwkAddr, uint8 status);
void nodePowerResponseMessage(zdoIncomingMsg_t * inMsg);
void serialSendSimpleDescriptor(ZDO_SimpleDescRsp_t * simpleDesc);
void serialSendIeeeAddress(zdoIncomingMsg_t * inMsg );
void serialSendBindTable(struct BindTableResponseEntry * bindTable);


uint8 * addMem(uint8 * iter, const char * data, uint8 size);
uint8 * sendUInt16(uint8 * iter, uint16 value);
uint8 * sendExtAddr(uint8 * iter,  uint8  extAddr[Z_EXTADDR_LEN]);