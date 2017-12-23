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
	unsigned char U0CSR_ACTIVE: 1;
	unsigned char U0CSR_TX_BYTE: 1;
	unsigned char U0CSR_RX_BYTE: 1;
	unsigned char U0CSR_ERR: 1;
	unsigned char U0CSR_FE: 1;
	unsigned char U0CSR_SLAVE: 1;
	unsigned char U0CSR_RE: 1;
	unsigned char U0CSR_MODE: 1;
} @ 0x86;

__sfr __no_init volatile struct  {
	unsigned char DMAIF0: 1;
	unsigned char DMAIF1: 1;
	unsigned char DMAIF2: 1;
	unsigned char DMAIF3: 1;
	unsigned char DMAIF4: 1;
	unsigned char DMAIF5: 1;
	unsigned char DMAIF6: 1;
	unsigned char DMAIF7: 1;
} @ 0xD1;

char * basePointer;


#define HAL_DMA_U0DBUF             0x70C1

char data1[MAX_DATA_SIZE];

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
	
	// SET DMA for writing
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
	HAL_DMA_SET_PRIORITY(dmaDesc, HAL_DMA_PRI_HIGH);
	DMAIE=1;
	
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
