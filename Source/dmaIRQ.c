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

HAL_ISR_FUNCTION( halDmaIsr, DMA_VECTOR )
{
  HAL_ENTER_ISR();

  DMAIF = 0;
  if (DMAIF0) DMAIF0=0;
  if (DMAIF1) DMAIF1=0;
  if (DMAIF2) DMAIF2=0;
  if (DMAIF3){
	  DMAIF3=0;
	  if (dataSends[0].used == UsedByDMA)
		  dataSends[0].used  = ToFree;
	  if (dataSends[1].used == UsedByDMA)
		  dataSends[1].used  = ToFree;
	  if (dataSends[2].used == UsedByDMA)
		  dataSends[2].used  = ToFree;	  
	  if (dataSends[3].used == UsedByDMA) 
		  dataSends[3].used  = ToFree;	  
	  
  }
  if (DMAIF4){
	  DMAIF4=0;
  }
  CLEAR_SLEEP_MODE();
  HAL_EXIT_ISR();
}