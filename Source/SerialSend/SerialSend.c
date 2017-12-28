/**************************************************************************************************

 DESCRIPTION:
  --

 CREATED: 18/12/2017, by Paolo Achdjian

 FILE: SerialSend.c

***************************************************************************************************/
#include "iocc2530.h"
#include "hal_dma.h"

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

uint8 data1[MAX_DATA_SIZE+5];
uint8 data2[MAX_DATA_SIZE+5];
uint8 data3[MAX_DATA_SIZE+5];
uint8 data4[MAX_DATA_SIZE+5];

struct DataSend dataSends[4];

uint8 maxDataSize=0; 
uint8 maxBufferUsed=0;
#define HEADER1 0x55
#define HEADER2 0xAA
#define HEADER3 0x42

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
	U0GCR |=0x0B; //-->115200
	//U0GCR |=0x0A; //-->57600
	U0BAUD = 216;

	U0CSR_RE = 1;
	
	// Enable irq on receive
	URX0IE = 1;
	URX0IF = 0;
	EA=1;
	
	// SET DMA 1 for writing
	halDMADesc_t * dmaDesc = HAL_DMA_GET_DESC1234(1);
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
	// SET DMA 2 for reading
	dmaDesc = HAL_DMA_GET_DESC1234(2);
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
	
	data1[0]= HEADER1;
	data2[0]= HEADER1;
	data3[0]= HEADER1;
	data4[0]= HEADER1;

	data1[1]= HEADER2;
	data2[1]= HEADER2;
	data3[1]= HEADER2;
	data4[1]= HEADER2;

	data1[2]= HEADER3;
	data2[2]= HEADER3;
	data3[2]= HEADER3;
	data4[2]= HEADER3;
	
	dataSends[0].start = data1+5;	
	dataSends[0].used=0;
	dataSends[1].start = data2+5;
	dataSends[1].used=0;
	dataSends[2].start = data3+5;
	dataSends[2].used=0;
	dataSends[3].start = data4+5;
	dataSends[3].used=0;
}

struct DataSend * getSendBuffer() {
	if (dataSends[0].used==0){
		dataSends[0].used=1;
		if (maxBufferUsed < 1)
			maxBufferUsed=1;
		return dataSends;
	}
	if (dataSends[1].used==0){
		dataSends[1].used=1;
		if (maxBufferUsed < 2)
			maxBufferUsed=2;

		return dataSends+1;
	}
	if (dataSends[2].used==0){
		dataSends[2].used=1;
		if (maxBufferUsed < 3)
			maxBufferUsed=3;
		return dataSends+2;
	}
	if (dataSends[3].used==0){
		dataSends[3].used=1;
		if (maxBufferUsed < 4)
			maxBufferUsed=4;
		return dataSends+3;
	}
	if (DMA_ARM1 == 0){
		halDMADesc_t * dmaDesc = HAL_DMA_GET_DESC1234(1);
		if (dataSends[0].used==0x80){
			dataSends[0].used=1;
			if (maxBufferUsed < 1)
				maxBufferUsed=1;
			return dataSends;
		}
		if (dataSends[1].used==0x80){
			dataSends[1].used=1;
			if (maxBufferUsed < 2)
				maxBufferUsed=2;

			return dataSends+1;
		}
		if (dataSends[2].used==0x80){
			dataSends[2].used=1;
			if (maxBufferUsed < 3)
				maxBufferUsed=3;
			return dataSends+2;
		}
		if (dataSends[3].used==0x80){
			dataSends[3].used=1;
			if (maxBufferUsed < 4)
				maxBufferUsed=4;
			return dataSends+3;
		}
	}
	return NULL;
}


#define WAIT_ARM	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
void send(enum MessageCode code, uint8 size,struct DataSend * buffer) {
	if (size > maxDataSize)
		maxDataSize=size;
	
	uint8 * base = buffer->start-5;
	base[3] = size;
	base[4] = code;
	if (dataSends[0].used == 0x80 ||dataSends[1].used == 0x80||dataSends[2].used == 0x80||dataSends[3].used == 0x80){
		buffer->used=0x40;
	} else {
		buffer->used = 0x80;
		halDMADesc_t * dmaDesc = HAL_DMA_GET_DESC1234(1);
		HAL_DMA_SET_SOURCE(dmaDesc, buffer->start-5);
		
		HAL_DMA_SET_LEN(dmaDesc, size+5);
		HAL_DMA_SET_IRQ(dmaDesc, HAL_DMA_IRQMASK_ENABLE);
		HAL_DMA_CLEAR_IRQ(1);
		HAL_DMA_ARM_CH(1);
		WAIT_ARM
		HAL_DMA_MAN_TRIGGER(1);
	}
	//while(HAL_DMA_CH_ARMED(1));
}

static void sendBuffer(uint8 index);
void serialSendLoop(void) {
	if (dataSends[0].used == 0x80 ||dataSends[1].used == 0x80||dataSends[2].used == 0x80||dataSends[3].used == 0x80){
		return;
	}
	if (dataSends[0].used == 0x40){
		sendBuffer(0);
	}
	if (dataSends[1].used == 0x40){
		sendBuffer(1);
	}
	if (dataSends[2].used == 0x40){
		sendBuffer(2);
	}
	if (dataSends[3].used == 0x40){
		sendBuffer(3);
	}
}

void sendBuffer(uint8 index) {
	dataSends[index].used = 0x80;
	uint8 * base = dataSends[index].start-5;
	uint8 size = base[3];
	halDMADesc_t * dmaDesc = HAL_DMA_GET_DESC1234(1);
	HAL_DMA_SET_SOURCE(dmaDesc, base);
	
	HAL_DMA_SET_LEN(dmaDesc, size+5);
	HAL_DMA_SET_IRQ(dmaDesc, HAL_DMA_IRQMASK_ENABLE);
	HAL_DMA_CLEAR_IRQ(1);
	HAL_DMA_ARM_CH(1);
	WAIT_ARM
	HAL_DMA_MAN_TRIGGER(1);
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
