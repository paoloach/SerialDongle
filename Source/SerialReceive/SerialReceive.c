
#include "iocc2530.h"
#include "hal_types.h"
#include "hal_mcu.h"
#include "hal_dma.h"
#include "SerialReceive/SerialReceive.h"

#define RX_BUFFER_SIZE 64
#define RX_DATA_TMP_LEN 24

uint16 rxDataOutOfBuffer=0;
uint16 rxDataError=0;

uint8 rxData[RX_BUFFER_SIZE];
uint8 *rxDataIter;
uint8 rxDataTmp[RX_DATA_TMP_LEN];
volatile uint16 rxDataUsed=0;
uint16 rxDataUsedMax=0;
volatile uint8 * rxDataTmpWRIter = rxDataTmp;
uint8 * rxDataTmpRDIter=rxDataTmp;
uint8 errorData[20];
uint8 errorDataIndex=0;
uint8 sizeData[10];
uint8 sizeDataIndex=0;
uint8 size=0;
uint8 rxReady=0;

#define HEADER1 0x45
#define HEADER2 0x65
#define HEADER3 0x42

enum StatusReceive {
	Header1,
	Header2,
	Header3,
	Size,
	Payload
};



enum StatusReceive statusReceived=Header1;

static void addErrorData(uint8 c);
static void addSizeData(uint8 c);


void serialReceiveLoop(void) {
	while (rxDataTmpRDIter != rxDataTmpWRIter){
		char c = *rxDataTmpRDIter;
		rxDataTmpRDIter++;
		if (rxDataTmpRDIter >= rxDataTmp +  RX_DATA_TMP_LEN)
			rxDataTmpRDIter=rxDataTmp;
		rxDataUsed--;
		switch(statusReceived){
			case Header1:
				if (c ==  HEADER1)
					statusReceived = Header2;
				else 
					addErrorData(c);				
				break;
			case Header2:
				if (c ==  HEADER2)
					statusReceived = Header3;
				else 
					addErrorData(c);
				break;
			case Header3:
				if (c ==  HEADER3)
					statusReceived = Size;
				else
					addErrorData(c);
				break;	
			case Size:
				size=c;
				addSizeData(c);
				if (size <  RX_BUFFER_SIZE){
					statusReceived = Payload;
					rxDataIter	= rxData;
					*rxDataIter = size;
					rxDataIter++;
				}else 
					statusReceived = Header1;
				break;
			case Payload:
				if (size > 0){
					*rxDataIter = c;
					rxDataIter++;
					size--;
				}
				if (size == 0){
					statusReceived = Header1;
					rxReady=1;
				}
				break;
			default:
				statusReceived = Header1;
		}
	}
			
	
	if (rxReady ){
		serialProcessEvent(rxData);
		rxReady=0;
	}
}

HAL_ISR_FUNCTION( usart0RXIsr, URX0_VECTOR ){
	HAL_ENTER_ISR();
		
	URX0IF = 0;
	*rxDataTmpWRIter = U0DBUF;
	rxDataTmpWRIter ++;
	rxDataUsed++;
	if (rxDataUsed > rxDataUsedMax)
		rxDataUsedMax = rxDataUsed;
	if (rxDataTmpWRIter >=rxDataTmp +  RX_DATA_TMP_LEN)
		rxDataTmpWRIter = rxDataTmp;
	
	CLEAR_SLEEP_MODE();
	HAL_EXIT_ISR();
}

static void addErrorData(uint8 c) {
	rxDataError++;
	errorData[errorDataIndex] = c;
	errorDataIndex++;
	if (errorDataIndex >= 20)
		errorDataIndex=19;
}

static void addSizeData(uint8 c){
	sizeData[sizeDataIndex]=size;
	sizeDataIndex++;
	if (sizeDataIndex>=10)
		sizeDataIndex=9;
}


