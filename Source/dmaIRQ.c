#include "iocc2530.h"
#include "hal_dma.h"

#include "SerialSend/SerialSend.h"

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

extern struct DataSend dataSends[4];
extern uint8 rxDMA;
extern uint8 rxUsed[3];

HAL_ISR_FUNCTION( halDmaIsr, DMA_VECTOR )
{
  HAL_ENTER_ISR();

  DMAIF = 0;
  if (DMAIF1){
	  DMAIF1=0;
	  if (dataSends[0].used == 0x80)
		  dataSends[0].used  = 0;
	  if (dataSends[1].used == 0x80)
		  dataSends[1].used  = 0;
	  if (dataSends[2].used == 0x80)
		  dataSends[2].used  = 0;	  
	  if (dataSends[3].used == 0x80) 
		  dataSends[3].used  = 0;	  
	  
  }
  if (DMAIF2){
	  DMAIF2=0;
	  URX0IE=1;
	  URX0IF=0;
	  rxUsed[rxDMA]=2;
  }
  CLEAR_SLEEP_MODE();
  HAL_EXIT_ISR();
}