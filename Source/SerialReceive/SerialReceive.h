#ifndef __SERIAL_RECEIVE__H_
#define __SERIAL_RECEIVE__H_

#include "hal_types.h"

void serialReceiveLoop(void);
void serialProcessEvent(uint8 * data);

#endif