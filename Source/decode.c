#include "SerialProcessEvents.h"
#include "DataStructures.h"
#include "TimerEvents.h"
#include "SerialProcessEventsInternal.h"

struct Cmds {
	const char * cmd;
	void (  * fn)(void);
};

#define MAX_CMD 12
struct Cmds cmds[MAX_CMD] = { 
	{"IEEE:", decodeIEEEEvents},
	{"INIT:", decodeINIT},
	{"NP:", decodeNodePowerEvents},
	{"BI:", decodeRequestBind},
	{"BT:", decodeRequestBindTable},
	{"DI:", decodeDeviceInfoEvent},
	{"AE:", decodeActiveEndpointEvent},
	{"RAS:", decodeRequestAttributes},
	{"RESET:", reset},
	{"SC:", decodeSendComand},
	{"UBI:", decodeRequestUnbind},
	{"WAI:", decodeSendWriteAttribute}
};

#define endCmds  cmds + sizeof(cmds)




uint8 serialErrorCount;
char * lastDataRead;
uint8 isCmd(const char * cmd);

void serialProcessEvent(void) {
	struct Cmds * iter = cmds;
	char * tmp = rxDataRead;
	uint8 found=0;

	for(uint8 i=0; i < MAX_CMD; i++){
		rxDataRead = tmp;
		if (isCmd(iter->cmd)){
			iter->fn();
			found=1;
			break;
		}
		iter++;
	}
	
	if (found==0){
		serialErrorCount++;
		incDataRead;
	}
	if(*rxDataRead != '\n'){
		serialErrorCount++;
	}

}


uint8 isCmd(const char * cmd) {
	while(*cmd != 0 && *cmd == *rxDataRead){
		cmd++;
		rxDataRead++;
		if (rxDataRead == rxDataEnd)
			rxDataRead = rxData;
	}
	return *cmd == 0;
}