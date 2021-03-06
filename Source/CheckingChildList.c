/**************************************************************************************************

 DESCRIPTION:
  --

 CREATED: 10/09/2017, by Paolo Achdjian

 FILE: CheckingChildList.c

***************************************************************************************************/
#include "CheckingChildList.h"
#include "ZDApp.h"
#include "TimerEvents.h"
#include "AddrMgr.h"
//#include "SerialFunctions.h"
#include "SerialSend/SendMethods.h"
#include "osal_memory.h"
#include "string.h"

static uint8 actualIndex;
static void callingLoop(uint16 mainTaskId);

void initCheckingChildList( uint16 mainTaskID ) {
	actualIndex=0;
	callingLoop(mainTaskID );
}


void nextCheckRequest(uint16 mainTaskID  ) {
	callingLoop(mainTaskID);
}


  extern void _itoa( uint16 num, uint8 *buf, uint8 radix );
void callingLoop(uint16 mainTaskId) {
    AddrMgrEntry_t entry;
	while(true){
		entry.index = actualIndex;
		entry.user = ADDRMGR_USER_DEFAULT | ADDRMGR_USER_ASSOC | ADDRMGR_USER_SECURITY | ADDRMGR_USER_BINDING;
		if (AddrMgrEntryGet(&entry)==1){
			ZDO_DeviceAnnce_t deviceAnnce;
			deviceAnnce.capabilities=0;
			deviceAnnce.nwkAddr = entry.nwkAddr;
			AddrMgrExtAddrSet(deviceAnnce.extAddr, entry.extAddr);
			
			serialSendAnnunce(&deviceAnnce);
			
			actualIndex++;
			osal_start_timerEx(mainTaskId, CHECHKING_CHILD_TIMER, 1000);
			return;
		} else {
			actualIndex++;
			if (actualIndex >= NWK_MAX_ADDRESSES)
				break;
		}
	}
}

