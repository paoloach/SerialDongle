
#include "iocc2530.h"
#include "hal_types.h"
#include "hal_mcu.h"

#include "SerialReceive.h"
#include "SerialProcessEvents.h"

#define RX_BUFFER_SIZE 128
char rxData[RX_BUFFER_SIZE];
#define   rxDataEnd rxData+RX_BUFFER_SIZE
char * rxDataRead=rxData;
static char * rxDataWrite=rxData;
#define CMD_LIST	6
static uint8 newCmd=0;
static char *	startCmd[CMD_LIST];

void serialLoop(void) {
	if (EA==0){
		EA=1;
	}
	if (URX0IE == 0){
		URX0IE=1;
	}
	if (newCmd>0){
		rxDataRead = startCmd[0];
		serialProcessEvent();
		newCmd--;
		startCmd[0] = startCmd[1];
		if (newCmd > 0){
			uint8 cmdIndex = newCmd;
			while (cmdIndex > 0){
				startCmd[cmdIndex] = startCmd[cmdIndex+1];
				cmdIndex--;
			}
		}
	}
}

// USART  0 RX IRQ
HAL_ISR_FUNCTION( usart0RXIsr, URX0_VECTOR ){
	HAL_ENTER_ISR();

	URX0IF = 0;
	char c = U0DBUF;
	if (c=='\n')
		*rxDataWrite=0;
	else			
		*rxDataWrite = c;
	rxDataWrite++;
	if (rxDataWrite == (char*)rxDataEnd)
		rxDataWrite = rxData;
	if (c == '\n'){
		startCmd[newCmd+1] = rxDataWrite;
		if (newCmd < CMD_LIST-1)
			newCmd++;
	}
	
	CLEAR_SLEEP_MODE();
	HAL_EXIT_ISR();
}