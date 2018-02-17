/**************************************************************************************************

 DESCRIPTION:
  --

 CREATED: 19/12/2017, by Paolo Achdjian

 FILE: SendMessage.c

***************************************************************************************************/
#include <OSAL_Memory.h>

#include "SerialSend.h"
#include "SendMethods.h"


// 1 byte code
// 1 byte size
// n bytes datas
void sendMessage(char * msg){
	uint8 * iter;
	uint8 size=0;
	struct DataSend * dataSend;
	char * tmp;
	
	tmp = msg;
	while(*tmp != 0){
		size++;
		tmp++;
	}
	
	
	while((dataSend = getSendBuffer(size))==NULL);
		if (dataSend->start == NULL){
		dataSend->used=Free;
		return;
	}
	iter = dataSend->start;
	while(*msg != 0){
		*iter = *msg;
		iter++;
		msg++;
	}
	send(STRING, dataSend);
}

