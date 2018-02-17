/**************************************************************************************************

 DESCRIPTION:
  --

 CREATED: 18/12/2017, by Paolo Achdjian

 FILE: SerialSend.c

***************************************************************************************************/
#include "iocc2530.h"
#include "hal_dma.h"
#include "hal_assert.h"

#include "SerialSend.h"

__sfr __no_init volatile struct  {
	unsigned char DMA_ARM0: 1;
	unsigned char DMA_ARM1: 1;
	unsigned char DMA_ARM2: 1;
	unsigned char DMA_ARM3: 1;
	unsigned char DMA_ARM4: 1;
	unsigned char DMA_ARM_NOTUSED1: 1;
	unsigned char DMA_ARM_NOTUSED2: 1;
	unsigned char DMA_ABORT: 1;
} @ 0xD6;

__sfr __no_init volatile struct  {
	unsigned char U0CSR_ACTIVE: 1;
	unsigned char U0CSR_TX_BYTE: 1;
	unsigned char U0CSR_RX_BYTE: 1;
	unsigned char U0CSR_ERR: 1;
	unsigned char U0CSR_FE: 1;
	unsigned char U0CSR_SLAVE: 1;
	unsigned char U0CSR_RE: 1;
	unsigned char U0CSR_MODE: 1;
} @ 0x86;



#define HAL_DMA_U0DBUF             0x70C1
#define HEADER1 0x55
#define HEADER2 0xAA
#define HEADER3 0x42
uint8 allocation=0;
uint8 deallocation=0;
static uint8 sendingBuffer[256];
uint16 bufferMap=0;
struct DataSend dataSends[4];
static struct DataSend * getFreeSendBuffer(void);
static uint8 * sendAlloc(uint8 size);
static void sendFree(uint8 * ptr);

/*********************************************************************
 * @fn      serialInit
 *
 * @brief   Initialize the serial port
 * 		USART0, 
 *			TX->P0_3
 *			RX->P0_2
*		BAUD: 115200
 *
 * @param   void
 *
 * @return  none
 */
#if 1
void serialInit(void){
	P2DIR &= ~0xC0;
	PERCFG &= 0xFE;
	
	P0SEL = 0x0C;

	U0CSR = 0x80;
	U0UCR = 0x80; // flush
	
	// U0UCR
	// FLOW(6) = 0
	// BIT9(4) = 0
	// PARITY(3) = 0
	// SPB(2) = 0
	// STOP(1) = 1
	// STAR(0) = 0
	U0UCR = 0x02;

	// SET BAUD at 115200
	// U0GCR
	// ORDER(5) 0 --> LSB first
	// BAUD_E(4:0) = B
	//U0GCR |=0x0B; //-->115200
	U0GCR |=0x0A; //-->57600
	U0BAUD = 216;

	U0CSR_RE = 1;
	
	// Enable irq on receive
	URX0IE = 1;
	URX0IF = 0;
	EA=1;
	
	// SET DMA 3 for writing
	halDMADesc_t * dmaDesc = HAL_DMA_GET_DESC1234(3);
	HAL_DMA_SET_DEST(dmaDesc, HAL_DMA_U0DBUF);
	HAL_DMA_SET_VLEN(dmaDesc, HAL_DMA_VLEN_USE_LEN);
	HAL_DMA_SET_WORD_SIZE(dmaDesc, HAL_DMA_WORDSIZE_BYTE);
	HAL_DMA_SET_TRIG_MODE(dmaDesc, HAL_DMA_TMODE_SINGLE);
	HAL_DMA_SET_TRIG_SRC(dmaDesc, HAL_DMA_TRIG_UTX0);
	HAL_DMA_SET_SRC_INC(dmaDesc, HAL_DMA_SRCINC_1);
	HAL_DMA_SET_DST_INC(dmaDesc, HAL_DMA_DSTINC_0);
	HAL_DMA_SET_IRQ(dmaDesc, HAL_DMA_IRQMASK_DISABLE);
	HAL_DMA_SET_M8(dmaDesc, HAL_DMA_M8_USE_8_BITS);
	HAL_DMA_SET_PRIORITY(dmaDesc, HAL_DMA_PRI_GUARANTEED);
	// SET DMA 4 for reading
	dmaDesc = HAL_DMA_GET_DESC1234(4);
	HAL_DMA_SET_SOURCE(dmaDesc, HAL_DMA_U0DBUF);
	HAL_DMA_SET_VLEN(dmaDesc, HAL_DMA_VLEN_USE_LEN);
	HAL_DMA_SET_WORD_SIZE(dmaDesc, HAL_DMA_WORDSIZE_BYTE);
	HAL_DMA_SET_TRIG_MODE(dmaDesc, HAL_DMA_TMODE_SINGLE);
	
	HAL_DMA_SET_TRIG_SRC(dmaDesc, HAL_DMA_TRIG_URX0);
	
	HAL_DMA_SET_SRC_INC(dmaDesc, HAL_DMA_SRCINC_0);
	HAL_DMA_SET_DST_INC(dmaDesc, HAL_DMA_DSTINC_1);
	HAL_DMA_SET_IRQ(dmaDesc, HAL_DMA_IRQMASK_DISABLE);
	HAL_DMA_SET_M8(dmaDesc, HAL_DMA_M8_USE_8_BITS);
	HAL_DMA_SET_PRIORITY(dmaDesc, HAL_DMA_PRI_HIGH);	
		
	DMAIE=1;
	
	dataSends[0].used=Free;
	dataSends[0].private=0;
	dataSends[1].used=Free;
	dataSends[1].private=0;
	dataSends[2].used=Free;
	dataSends[2].private=0;
	dataSends[3].used=Free;
	dataSends[3].private=0;
}

static uint8 * sendAlloc(uint8 size){
	uint8 blk;
	uint16 mask;
	uint8 i;
	uint16 tmpMap;
	
	size += 5;
	blk = (size / 16);
	if ( (size % 16) != 0)
		blk++;
	mask=0;
	for (i=0; i < blk; i++)
		mask = (mask << 1) | 1;
	tmpMap = bufferMap;
	for (i=0; i < 16; i++){
		if ( (tmpMap & mask) == 0){
			break;
		}
		tmpMap = tmpMap >> 1;
	}
	if (i==16)
		return NULL;
	mask = mask << i;
	bufferMap = bufferMap | mask;
	uint8 * result = sendingBuffer + i*16;
	*result = HEADER1;
	result++;
	*result = HEADER2;
	result++;
	*result = HEADER3;
	result++;
	*result = size-5;
	result++;
	result++;
	return result;
}
static void sendFree(uint8 * ptr) {
	uint8 blkStart;
	uint8 blkCnt;
	uint8 size;
	uint8 * buffer = ptr-5;
	uint16 mask;
	uint8 i;
	
	if (buffer[0] != HEADER1 || buffer[1] != HEADER2 || buffer[2] != HEADER3){
		return;
	}
	
	if (buffer < sendingBuffer){
		halAssertHandler();
		return;
		
	}
	
	blkStart = (buffer- sendingBuffer)%16;
	size = buffer[3]+5;
	blkCnt = size / 16;
	if ( (size % 16)  != 0)
		blkCnt++;
	mask = 0;
	for(i=0; i< blkCnt; i++)
		mask = (mask << 1) | 1;
	mask = mask << blkStart;
	mask = mask ^ 0xFFFF;
	bufferMap = bufferMap & mask;
	if (bufferMap != 0){
		halAssertHandler();
	}
}

struct DataSend * getFreeSendBuffer(void) {
	if (dataSends[0].used == Free){
		return dataSends;
	}
	if (dataSends[1].used == Free){
		return &dataSends[1];
	}
	if (dataSends[2].used == Free){
		return &dataSends[2];
	}
	if (dataSends[3].used == Free){
		return &dataSends[3];
	}
	return NULL;
}

struct DataSend * getPrivateSendBuffer(uint8 * buffer){
	buffer[0] = HEADER1;
	buffer[1] = HEADER2;
	buffer[2] = HEADER3;
	struct DataSend * sendData =  getFreeSendBuffer();
	if (sendData != NULL){
		sendData->start = buffer+5;
		sendData->used = Waiting;
		sendData->private=1;
	}
	return sendData;
}

struct DataSend * getSendBuffer(uint8 size) {
	uint8 * buffer ;
	struct DataSend * sendData =  getFreeSendBuffer();
	if (sendData != NULL){
		buffer = sendAlloc(size);
		allocation++;
		if (buffer != NULL){
			sendData->start = buffer;
			sendData->used = Waiting;
			sendData->private=0;
		} else {
			sendData->start = NULL;
		}
	}
	return sendData;
}


#define WAIT_ARM	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
void send(enum MessageCode code,struct DataSend * dataSend) {
	uint8 * base = dataSend->start-5;
	base[4] = code;
	
	if (dataSends[0].used == UsedByDMA ||dataSends[1].used == UsedByDMA ||dataSends[2].used == UsedByDMA ||dataSends[3].used == UsedByDMA){
		dataSend->used = WaitingDMA;
	} else {
		dataSend->used = UsedByDMA;
		halDMADesc_t * dmaDesc = HAL_DMA_GET_DESC1234(3);
		HAL_DMA_SET_SOURCE(dmaDesc, base);
		
		HAL_DMA_SET_LEN(dmaDesc, base[3]+5);
		HAL_DMA_SET_IRQ(dmaDesc, HAL_DMA_IRQMASK_ENABLE);
		HAL_DMA_CLEAR_IRQ(3);
		HAL_DMA_ARM_CH(3);
		WAIT_ARM
		HAL_DMA_MAN_TRIGGER(3);
	}
	//while(HAL_DMA_CH_ARMED(1));
}

static void sendBuffer(uint8 index);
void serialSendLoop(void) {
	if (dataSends[0].used == ToFree){
		if (dataSends[0].private==0){
			sendFree(dataSends[0].start);
			deallocation++;
		}
		dataSends[0].used = Free;
	}
	if (dataSends[1].used == ToFree){
		if (dataSends[1].private==0){
			sendFree(dataSends[1].start);
			deallocation++;
		}
		dataSends[1].used = Free;
	}
	if (dataSends[2].used == ToFree){
		if (dataSends[2].private==0){
			sendFree(dataSends[2].start);
			deallocation++;
		}
		dataSends[2].used = Free;
	}
	if (dataSends[3].used == ToFree){
		if (dataSends[3].private==0){
			sendFree(dataSends[3].start);
			deallocation++;
		}
		dataSends[3].used = Free;
	}
	if (dataSends[0].used == WaitingDMA){
		sendBuffer(0);
	}
	if (dataSends[1].used == WaitingDMA){
		sendBuffer(1);
	}
	if (dataSends[2].used ==WaitingDMA){
		sendBuffer(2);
	}
	if (dataSends[3].used == WaitingDMA){
		sendBuffer(3);
	}
}

void sendBuffer(uint8 index) {
	dataSends[index].used = UsedByDMA;
	uint8 * base = dataSends[index].start-5;
	halDMADesc_t * dmaDesc = HAL_DMA_GET_DESC1234(3);
	HAL_DMA_SET_SOURCE(dmaDesc, base);
	
	HAL_DMA_SET_LEN(dmaDesc,  base[3]+5);
	HAL_DMA_SET_IRQ(dmaDesc, HAL_DMA_IRQMASK_ENABLE);
	HAL_DMA_CLEAR_IRQ(3);
	HAL_DMA_ARM_CH(3);
	WAIT_ARM
	HAL_DMA_MAN_TRIGGER(3);
}



#endif

#if 0

void serialInit(void){
	P2DIR &= ~0xC0;
	PERCFG &= 0xFE;
	
	P0SEL = 0x0C;

	U0CSR = 0x80;
	U0UCR = 0x80; // flush
	
	// U0UCR
	// FLOW(6) = 0
	// BIT9(4) = 0
	// PARITY(3) = 0
	// SPB(2) = 0
	// STOP(1) = 1
	// STAR(0) = 0
	U0UCR = 0x02;

	// SET BAUD at 115200
	// U0GCR
	// ORDER(5) 0 --> LSB first
	// BAUD_E(4:0) = B
	U0GCR |=0x0B; //-->115200
	//U0GCR |=0x0A; //-->57600
	U0BAUD = 216;

	U0CSR_RE = 1;
	
	// Enable irq on receive
	URX0IE = 1;
	URX0IF = 0;
	EA=1;

	
	basePointer = data1;
}


void send(enum MessageCode code, uint8 size) {
	char * base = basePointer;
	basePointer = NULL;
	
	// header
	while(U0CSR_ACTIVE);
	U0DBUF = HEADER1;
	while(U0CSR_ACTIVE);
	U0DBUF = HEADER2;
	while(U0CSR_ACTIVE);
	U0DBUF = HEADER3;

	// len
	while(U0CSR_ACTIVE);
	U0DBUF = size;
	
	// code
	while(U0CSR_ACTIVE);
	U0DBUF = code;
	
	// data
	for (uint8 i=0; i < size; i++){
		while(U0CSR_ACTIVE);
		U0DBUF = *base;
		base++;
	}
	basePointer = data1;
}
#endif
