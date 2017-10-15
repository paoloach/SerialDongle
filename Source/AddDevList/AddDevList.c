#include "AssocList.h"

uint16 AssocCount_2( byte startRelation, byte endRelation ){
	uint16	count=0;
	uint8   relation;
	for(uint8_t i=0; i < gNWK_MAX_DEVICE_LIST; i++){
		relation = AssociatedDevList[i].nodeRelation;
		if (relation >= startRelation && relation < endRelation)
			count++;
	}
	return count;
}