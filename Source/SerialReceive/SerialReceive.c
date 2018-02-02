
#include "iocc2530.h"
#include "hal_types.h"
#include "hal_mcu.h"
#include "hal_dma.h"
#include "SerialReceive/SerialReceive.h"

uint16 rxData1Count=0;
uint16 rxData2Count=0;
uint16 rxData3Count=0;
uint16 rxDataOutOfBuffer=0;
uint16 rxDataError=0;
uint8 rxData1[32];
uint8 rxData2[32];
uint8 rxData3[32];
uint8 errorData[20];
uint8 errorDataIndex=0;

uint8 rxUsed[3];
uint8 rxDMA;

#define HEADER1 0x45
#define HEADER2 0x65
#define HEADER3 0x42

enum StatusReceive {
	Header1,
	Header2,
	Header3,
	Size,
	DMA
};

enum StatusReceive statusReceived=Header1;

void serialReceiveLoop(void) {
	if (rxUsed[0] == 2){
		serialProcessEvent(rxData1);
		rxUsed[0]=0;
	}
	if (rxUsed[1] == 2){
		serialProcessEvent(rxData2);
		rxUsed[1]=0;
	}
	if (rxUsed[2] == 2){
		serialProcessEvent(rxData3);
		rxUsed[2]=0;
	}
}

// USART  0 RX IRQ
HAL_ISR_FUNCTION( usart0RXIsr, URX0_VECTOR ){
	HAL_ENTER_ISR();

	URX0IF = 0;
	char c = U0DBUF;
	switch(statusReceived){
		case Header1:
			if (c ==  HEADER1)
				statusReceived = Header2;
			else {
				rxDataError++;
				errorData[errorDataIndex] = c;
				errorDataIndex++;
				if (errorDataIndex >= 20)
					errorDataIndex=19;
			}
			break;
		case Header2:
			if (c ==  HEADER2)
				statusReceived = Header3;
			else {
				statusReceived = Header1;
				rxDataError++;
				errorData[errorDataIndex] = c;
				errorDataIndex++;
				if (errorDataIndex >= 20)
					errorDataIndex=19;
			}
			break;
		case Header3:
			if (c ==  HEADER3)
				statusReceived = Size;
			else{
				statusReceived = Header1;
				rxDataError++;
				errorData[errorDataIndex] = c;
				errorDataIndex++;
				if (errorDataIndex >= 20)
					errorDataIndex=19;
			}
			break;	
		case Size:{
			uint8 * rxData;
			if (rxUsed[0] == 0){
				rxData=rxData1;
				rxUsed[0] = 1;
				rxData1Count++;
				rxDMA=0;
				URX0IE = 0;	
			} else if (rxUsed[1]==0){
				rxData=rxData2;
				rxUsed[1] = 1;
				rxData2Count++;
				rxDMA=1;
				URX0IE = 0;	
			}else if (rxUsed[2]==0){
				rxData=rxData3;
				rxUsed[2] = 1;
				rxData3Count++;
				rxDMA=2;
				URX0IE = 0;
			} else {
				statusReceived = Header1;
				rxDataOutOfBuffer++;
				break;
			}
			rxData[0]=c;
			halDMADesc_t * dmaDesc = HAL_DMA_GET_DESC1234(2);
			HAL_DMA_SET_DEST(dmaDesc, rxData+1);
			
			HAL_DMA_SET_LEN(dmaDesc, c);
			HAL_DMA_SET_IRQ(dmaDesc, HAL_DMA_IRQMASK_ENABLE);
			URX0IE = 0;
			HAL_DMA_CLEAR_IRQ(2);
			HAL_DMA_ARM_CH(2);
			statusReceived=Header1;
			break;
			}
	default:
		statusReceived = Header1;
	}

	CLEAR_SLEEP_MODE();
	HAL_EXIT_ISR();
}