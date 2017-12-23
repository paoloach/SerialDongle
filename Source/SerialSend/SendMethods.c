#include "SendMethods.h"


char * addMem(char * iter, const char * data, uint8 size) {
	for(uint8 i=0; i < size; i++){
		*iter = *data;
		iter++;
		data++;
	}
	return iter;
}

char * sendUInt16(char * iter, uint16 value) {
	*iter = LO_UINT16(value);
	iter++;
	*iter = HI_UINT16(value);
	iter++;
	return iter;
}

char * sendExtAddr(char * iter,  uint8  extAddr[Z_EXTADDR_LEN]) {
	for (uint8 i=0; i < Z_EXTADDR_LEN; i++){
		*iter = *extAddr;
		iter++;
		extAddr++;
	}
	return iter;
}