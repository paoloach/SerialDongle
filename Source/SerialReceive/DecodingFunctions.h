#ifndef __DECODING_FUNCTIONS__H__
#define __DECODING_FUNCTIONS__H__
#include "hal_types.h"
void getIEEEAddress(uint8 * data);
void requestAttribute(uint8 * data);
void requestAttributes(uint8 * data);
void requestReset(uint8 * data);
void writeAttribute(uint8 * data);
void sendCmd(uint8 * data);
void getNodeDescriptor(uint8 * data);
void requestActiveEndpoints(uint8 * data);
void requestNodePower(uint8 * data);
void sendReqBind(uint8 * data);
void sendReqUnBind(uint8 * data);
void sendReqDeviceInfo(uint8 * data);
void requestBindTable(uint8 * data);
void init(uint8 * data);
void removeDevice(uint8 * data);
void invalidCmd(uint8 * data);
void getMgmtLqiReq(uint8 * data);
#endif