#include "SendMethods.h"


uint8 * addMem(uint8 * iter, const char * data, uint8 size) {
	for(uint8 i=0; i < size; i++){
		*iter = *data;
		iter++;
		data++;
	}
	return iter;
}

uint8 * sendUInt16(uint8 * iter, uint16 value) {
	*iter = LO_UINT16(value);
	iter++;
	*iter = HI_UINT16(value);
	iter++;
	return iter;
}

uint8 * sendExtAddr(uint8 * iter,  uint8  extAddr[Z_EXTADDR_LEN]) {
	for (uint8 i=0; i < Z_EXTADDR_LEN; i++){
		*iter = *extAddr;
		iter++;
		extAddr++;
	}
	return iter;
}