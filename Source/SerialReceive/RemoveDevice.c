#include "ZDProfile.h"

#include "SerialReceive/DecodingFunctions.h"
#include "AssocList.h"
#include "addrMgr.h"

// 1 byte  -> code
// 2 bytes -> network id
void removeDevice(uint8 * data){
	data++;
	uint16 shortAddr = BUILD_UINT16(data[0], data[1]);
	associated_devices_t * device = AssocGetWithShort(shortAddr);
	if(device != NULL){
		AddrMgrEntry_t addrEntry;
     	addrEntry.user = ADDRMGR_USER_DEFAULT;
     	addrEntry.index = device->addrIdx;
      	if ( AddrMgrEntryGet( &addrEntry ) ){
			AssocRemove(addrEntry.extAddr);
			AddrMgrEntryRelease(&addrEntry);	
		}
	}
		
}