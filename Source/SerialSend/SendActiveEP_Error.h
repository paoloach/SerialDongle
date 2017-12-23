#include "SendMethods.h"

void serialSendActiveEPError(uint16 nwkAdd, afStatus_t ret){
	char * iter;
	while(nextDbIter != dbIter);
	iter = addMem(dbIter,  "AEE: ", 5);
	iter = addNwkId(iter, nwkAdd);
	iter = addSep(iter);
	iter = adduint8(iter,ret);
	*iter = '\n';
	iter++;
	if (iter == dbEnd){
		iter = db;
	}	
	*iter=0;
	serialWrite(iter);
}