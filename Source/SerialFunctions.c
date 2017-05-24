/**************************************************************************************************

 DESCRIPTION:
  --

 CREATED: 29/12/2016, by Paolo Achdjian

 FILE: SerialFunctions.c

***************************************************************************************************/

#include "iocc2530.h"
#include "SerialFunctions.h"
#include "hal_dma.h"
#include <string.h>
#include "SerialProcessEvents.h"

#define addEndpoint(buffer, endpoint) adduint8(buffer, endpoint)
#define addNwkId(buffer, endpoint) adduint16(buffer, endpoint)
#define addCluster(buffer, cluster) adduint16(buffer, cluster)

static void serialWrite(char * string);
static char * adduint16(char * buffer, uint16 nwkAddr);
static char * addMem(char * buffer, char * src, uint8 len);
static char * addExtAddr(char * buffer,  uint8 extAddr[Z_EXTADDR_LEN]);
static char * adduint8(char * buffer, uint8 value);
static char * addSep(char * buffer);

static char hexDigit[16] = "0123456789ABCDEF";
#define WAIT_ARM	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");

#define HAL_DMA_U0DBUF             0x70C1

#define DATA_BUFFER_SIZE	128
static char db[DATA_BUFFER_SIZE];
static char * dbIter=db;
static char * nextDbIter=db;
static char * dbEnd=db+DATA_BUFFER_SIZE;

#define RX_BUFFER_SIZE 128
static char rxData[RX_BUFFER_SIZE];
static char * rxDataIter=rxData;
static uint8 newCmd=0;

static uint8 DMA1Ready=1;


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

	// U0CSR
	// MODE (7)=1
	// RE (6)=1
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
	DMA1Ready=1;
}

void serialLoop(void) {
	if (EA==0){
		EA=1;
	}
	if (URX0IE == 0){
		URX0IE=1;
	}
	if (newCmd){
		serialProcessEvent(rxData);
		rxDataIter=rxData;
		URX0IE=1;

		newCmd=0;
	}
}

void sendMessage(char * string){
	char * buffer = dbIter;
	while(*string != 0){
		*buffer = *string;
		buffer++;
		string++;
		if (buffer == dbEnd){
			buffer = db;
		}
	}
	serialWrite(buffer);
}

void serialWrite(char * iter) {
	halDMADesc_t * dmaDesc = HAL_DMA_GET_DESC1234(1);
	uint16 len;
	while(DMA1Ready==0);
	if (dbIter  > iter){
		// Write all but the data from dbIter to the end of the buffer
	
		len = 1+dbEnd -  dbIter;
		HAL_DMA_SET_SOURCE(dmaDesc,dbIter );
		HAL_DMA_SET_LEN(dmaDesc, len);
		HAL_DMA_SET_IRQ(dmaDesc, HAL_DMA_IRQMASK_ENABLE);
		HAL_DMA_CLEAR_IRQ(1);
		HAL_DMA_ARM_CH(1);
		nextDbIter=db;
		WAIT_ARM
		DMA1Ready=0;
		HAL_DMA_MAN_TRIGGER(1);
		while(DMA1Ready==0);
	}

	len = iter- dbIter;
	HAL_DMA_SET_SOURCE(dmaDesc,dbIter );
	HAL_DMA_SET_LEN(dmaDesc, len);
	HAL_DMA_SET_IRQ(dmaDesc, HAL_DMA_IRQMASK_ENABLE);
	HAL_DMA_CLEAR_IRQ(1);
	HAL_DMA_ARM_CH(1);
	nextDbIter=iter;
	WAIT_ARM
	DMA1Ready=0;
	HAL_DMA_MAN_TRIGGER(1);
}

void serialSendDevice(associated_devices_t * device){
}

// format: DI: network id, node relation, dev status, assoc count,   age   , txCounter, txCost  , rxLqi, 
//             4 digits  ,   2 digits   , 2 digits  , 2 digits   , 2 digits, 2 digits , 2 digits, 2 digits  
void serialSendDeviceInfo(associated_devices_t * device){
	char * iter;
	iter = addMem(dbIter,  "DI: ", 4);
	iter = addNwkId(iter, device->shortAddr);
	iter = addSep(iter);
	iter = adduint8(iter,device->nodeRelation);
	iter = addSep(iter);
	iter = adduint8(iter,device->devStatus);
	iter = addSep(iter);
	iter = adduint8(iter,device->assocCnt);
	iter = addSep(iter);
	iter = adduint8(iter,device->age);
	iter = addSep(iter);
	iter = adduint8(iter,device->linkInfo.txCounter);
	iter = addSep(iter);
	iter = adduint8(iter,device->linkInfo.txCost);
	iter = addSep(iter);
	iter = adduint8(iter,device->linkInfo.rxLqi);
	*iter = '\n';
	iter++;
	if (iter == dbEnd){
		iter = db;
	}	
	while(iter == dbIter);
	*iter=0;
	serialWrite(iter);
}
// format: AEE: networkId, status
//               4digit,   2digit
void serialSendActiveEPError(uint16 nwkAdd, afStatus_t ret){
	char * iter;
	iter = addMem(dbIter,  "AEE: ", 5);
	iter = addNwkId(iter, nwkAdd);
	iter = addSep(iter);
	iter = adduint8(iter,ret);
	*iter = '\n';
	iter++;
	if (iter == dbEnd){
		iter = db;
	}	
	while(iter == dbIter);
	*iter=0;
	serialWrite(iter);
}

// format: RAE: networkId, endpointId, clusterId, attributeId, status
//              4digits  ,  2digits  , 4 digits ,   4 digits , 2 digits
void serialSendAttributeResponseMsgError(struct ReqAttributeMsg * attr, ZStatus_t status){
	char * iter;
	iter = addMem(dbIter,  "RAE: ", 5);
	iter = addNwkId(iter, attr->afAddrType.addr.shortAddr);
	iter = addSep(iter);
	iter = addEndpoint(iter, attr->afAddrType.endPoint);
	iter = addSep(iter);
	iter = adduint16(iter, attr->cluster);
	iter = addSep(iter);
	iter = adduint16(iter, attr->attrID[0]);
	iter = addSep(iter);
	iter = adduint8(iter, status);
	*iter = '\n';
	iter++;
	if (iter == dbEnd){
		iter = db;
	}	
	while(iter == dbIter);
	*iter=0;
	serialWrite(iter);
}

// format: RA: networkId, endpointId, clusterId, attributeId, attributeStatus,  attributeType,  attributeVelueLen, attributeValue
//              4digits  ,  2digits  , 4 digits , 4 digits  ,    2  digits   ,    2 digits   ,    2 digits       ,   2*n digits where n=attributeValueLen   
//  attributeType, attributeVelueLen, attributeValue only if attributeStatus == ZCL_STATUS_SUCCESS
void serialSendAttributeResponseMsg(zclReadRspCmd_t * readRsp, uint16 clusterId, afAddrType_t * addr){
	zclReadRspStatus_t * iterAttr = readRsp->attrList;
	zclReadRspStatus_t * iterEnd = readRsp->attrList+readRsp->numAttr;
	uint8 attrSize;
	uint8 i;
	char * iter=dbIter;
	
	for (;iterAttr != iterEnd; iterAttr++){
		while(DMA1Ready==0);
		iter = addMem(iter,  "RA: ", 4);
		iter = addNwkId(iter, addr->addr.shortAddr);
		iter = addSep(iter);
		iter = addEndpoint(iter, addr->endPoint);
		iter = addSep(iter);
		iter = adduint16(iter, clusterId);
		iter = addSep(iter);
		iter = adduint16(iter, iterAttr->attrID);
		iter = addSep(iter);
		iter = adduint8(iter, iterAttr->status);	
		if (iterAttr->status == ZCL_STATUS_SUCCESS){
			iter = addSep(iter);
			iter = adduint8(iter, iterAttr->dataType);	
			attrSize = zclGetAttrDataLength(iterAttr->dataType, iterAttr->data);
			iter = addSep(iter);
			iter = adduint8(iter, attrSize);	
			iter = addSep(iter);
			for (i=0; i < attrSize; i++){
				iter = adduint8(iter, iterAttr->data[i]);	
			}
		}
		*iter = '\n';
		iter++;
		if (iter == dbEnd){
			iter = db;
		}	
		while(iter == dbIter);
		*iter=0;
		serialWrite(iter);
	}
}

// format: AN: networkId , extendAddress, capability\n
//            4digit   16digits		  2digits
void serialSendAnnunce(ZDO_DeviceAnnce_t * deviceAnnce ){
	char * buffer = dbIter;
	char * iter;
	iter = addMem(buffer,  "AN: ", 4);
	iter = addNwkId(iter, deviceAnnce->nwkAddr);
	iter = addSep(iter);
	iter = addExtAddr(iter, deviceAnnce->extAddr);
	iter = addSep(iter);
	iter = adduint8(iter, deviceAnnce->capabilities);
	*iter = '\n';
	iter++;
	if (iter == dbEnd){
		iter = db;
	}
	while(iter == dbIter);
	*iter=0;
	serialWrite(iter);
}


// NPR: networkdId, powerMode, availablePowerSources, currentPowerSource, currentPowerSourceLevel
//       4digit       1Digit        1Digit               1Digit                1Digit
//  or if error
// NPRE: networkId, status
//        4Digit    2 digit
void nodePowerResponseMessage(zdoIncomingMsg_t * inMsg) {
	uint8 *msg;
	uint8 status;
	uint16 nwkAddr;
	char * iter;
	char * buffer = dbIter;
	
	msg = inMsg->asdu;
	status = *msg++;
	nwkAddr = BUILD_UINT16( msg[0], msg[1] );
	if (status == ZDP_SUCCESS){
		iter = addMem(buffer, "NPR: ", 5);
		iter = addNwkId(iter, nwkAddr);
		iter = addSep(iter);
		msg += 2;
		*iter = hexDigit[*msg >> 4];
		iter = addSep(iter);
		*iter = hexDigit[*msg++ & 0x0F];
		iter = addSep(iter);
		*iter = hexDigit[*msg >> 4];
		iter = addSep(iter);
		*iter = hexDigit[*msg++ & 0x0F];
		*iter = 0;
	} else {
		iter = addMem(buffer, "NPRE: ", 6);
		iter = addNwkId(iter, nwkAddr);
		iter = adduint8(iter, status);
		*iter=0;
		iter++;
	}
	serialWrite(iter);
}


// SD: networkID, endpoint, AppProfId, deviceId, deviceVersion, numInClusters, firstInCluster, ..., lastInCluster, numOutClusters, firstOutCluster, ..., lastOutCluster
//      4digits,   2digits,  4digits,   4digits,    2digits,       2digits,      4digits,    , ...,  4digits   ,        2digits,     4digits,     , ..., 4digits 
void serialSendSimpleDescriptor(ZDO_SimpleDescRsp_t * simpleDesc){
	char * buffer = dbIter;
	char * iter;
	uint8 i;
	iter = addMem(buffer, "SD: ", 4);
	iter = addNwkId(iter, simpleDesc->nwkAddr);
	iter = addSep(iter);
	iter = addEndpoint(iter, simpleDesc->simpleDesc.EndPoint);
	iter = addSep(iter);
	iter = adduint16(iter, simpleDesc->simpleDesc.AppProfId);
	iter = addSep(iter);
	iter = adduint16(iter, simpleDesc->simpleDesc.AppDeviceId);
	iter = addSep(iter);
	iter = adduint8(iter, simpleDesc->simpleDesc.AppDevVer);	
	iter = addSep(iter);
	iter = adduint8(iter, simpleDesc->simpleDesc.AppNumInClusters);	
	iter = addSep(iter);	
	for (i=0; i <simpleDesc->simpleDesc.AppNumInClusters; i++){
		iter = addCluster(iter, simpleDesc->simpleDesc.pAppInClusterList[i]);
		iter = addSep(iter);
	}
	iter = adduint8(iter, simpleDesc->simpleDesc.AppNumOutClusters);	
	iter = addSep(iter);	
	for (i=0; i <simpleDesc->simpleDesc.AppNumOutClusters; i++){
		iter = addCluster(iter, simpleDesc->simpleDesc.pAppOutClusterList[i]);
		iter = addSep(iter);
	}	
	iter--;
	iter--;
	*iter = '\n';
	iter++;
	if (iter == dbEnd){
		iter = db;
	}
	while(iter == dbIter);
	*iter=0;
	serialWrite(iter);
}

// IE: ieeeaddress, networkAddress, numChild, firstChildNwkId, ..., lastChildNwkId
//      16digits ,     4digits,      2digits,  4digits,      , ...,  4digits
// or in case of error
// EI: 
void serialSendIeeeAddress(zdoIncomingMsg_t * inMsg ){
	byte cnt = 0;
	uint8  status;
	char * iter = dbIter;
	
	if ( inMsg->asduLen > (1 + Z_EXTADDR_LEN + 2) ) {
 		cnt = inMsg->asdu[1 + Z_EXTADDR_LEN + 2];
	} else {
    	cnt = 0;
  	}
	uint8 * msg = inMsg->asdu;
	status =  *msg++;
	if (status == ZDO_SUCCESS){
		iter = addMem(iter, "IE: ", 4);
		iter = addExtAddr(iter, msg);
		msg +=Z_EXTADDR_LEN;
		iter = addSep(iter);
		iter = addNwkId(iter,*(uint16 *)msg);
		msg+=4;
		iter = addSep(iter);					
		iter = adduint8(iter, cnt);		
		iter = addSep(iter);	
		for (;cnt > 0; cnt--){
			iter = addNwkId(iter,*(uint16 *)msg);
			iter = addSep(iter);	
		}
		if (iter == db){
			iter = dbEnd;
		}
		iter--;
		if (iter == db){
			iter = dbEnd;
		}
		iter--;
		
	} else {
		iter = addMem(iter, "IE: ", 4);
	}
	*iter = '\n';
	iter++;
	if (iter == dbEnd){
		iter = db;
	}	
	while(iter == dbIter);
	*iter=0;
	serialWrite(iter);
}

// BT:  extAddressSource, endpointIdSource, clusterId, networkIdDest, endpointIdDesc
//       16 digits      ,      2digits    ,  4 digits,   4 digits   ,   2 digits
void serialSendBindTable(struct BindTableResponseEntry * bindTable) {
	char * iter = dbIter;
	iter = addMem(iter, "BT: ", 4);
	iter = addExtAddr(iter, bindTable->srcAddr);
	iter = addSep(iter);
	iter = addEndpoint(iter, bindTable->srcEP);
	iter = addSep(iter);					
	iter = addCluster(iter, bindTable->clusterID);		
	iter = addSep(iter);					
	iter = addNwkId(iter, bindTable->dstAddr);
	iter = addSep(iter);		
	iter = addEndpoint(iter,bindTable->dstEP);
	*iter = '\n';
	iter++;
	if (iter == dbEnd){
		iter = db;
	}	
	while(iter == dbIter);
	*iter=0;
	serialWrite(iter);				
}

static char * addMem(char * buffer, char * src, uint8 len) {
	uint8 i;
	for (i=0; i < len; i++){
		*buffer = *src;
		buffer++;
		src++;
		if (buffer == dbEnd){
			buffer = db;
		}
		while(buffer == dbIter);
	}
	return buffer;
}

static char * addSep(char * buffer) {
	*buffer = ',';
	buffer++;
	if (buffer == dbEnd){
		buffer = db;
	}
	while(buffer == dbIter);	
	*buffer = ' ';
	buffer++;
	if (buffer == dbEnd){
		buffer = db;
	}
	while(buffer == dbIter);
	return buffer;
}

static char * adduint16(char * buffer, uint16 nwkAddr) {
	*buffer = hexDigit[(nwkAddr >> 12)];
	buffer++;
	if (buffer == dbEnd){
		buffer = db;
	}
	while(buffer == dbIter);	
	*buffer = hexDigit[((nwkAddr >> 8) & 0x0F)];
	buffer++;
	if (buffer == dbEnd){
		buffer = db;
	}
	while(buffer == dbIter);		
	*buffer = hexDigit[((nwkAddr >> 4) & 0x0F)];
	buffer++;
	if (buffer == dbEnd){
		buffer = db;
	}	
	while(buffer == dbIter);		
	*buffer = hexDigit[(nwkAddr & 0x0F)];
	buffer++;
	if (buffer == dbEnd){
		buffer = db;
	}	
	while(buffer == dbIter);		
	return buffer;
}

char * addExtAddr(char * buffer,  uint8 extAddr[Z_EXTADDR_LEN]) {
	uint8 * iter = extAddr;
	uint8 * end = extAddr + Z_EXTADDR_LEN;
	char  c;
	while (iter != end){
		c = *iter;
		*buffer = hexDigit[((c >> 4) & 0x0F)];
		buffer++;
		if (buffer == dbEnd){
			buffer = db;
		}
		while(buffer == dbIter);		
		*buffer = hexDigit[(c & 0x0F)];
		buffer++;
		if (buffer == dbEnd){
			buffer = db;
		}	
		while(buffer == dbIter);
		*buffer = '-';
		iter++;
		if (iter != end){
			buffer++;
			if (buffer == dbEnd){
				buffer = db;
			}
			while(buffer == dbIter);
		}
	}
	return buffer;
}

char * adduint8(char * buffer, uint8 value) {
	*buffer = hexDigit[((value >> 4) & 0x0F)];
	buffer++;
	if (buffer == dbEnd){
		buffer = db;
	}	
	while(buffer == dbIter);
	*buffer = hexDigit[(value & 0x0F)];
	buffer++;
	if (buffer == dbEnd){
		buffer = db;
	}	
	while(buffer == dbIter);
	return buffer;
}

// USART  0 RX IRQ
HAL_ISR_FUNCTION( usart0RXIsr, URX0_VECTOR ){
	HAL_ENTER_ISR();

	URX0IF = 0;
	*rxDataIter = U0DBUF;
	if (*rxDataIter == '\n'){
		*rxDataIter = 0;
		newCmd=1;
		// Disable the interrupt until the command is elaborated
		URX0IE=0;
	}
	rxDataIter++;

	CLEAR_SLEEP_MODE();
	HAL_EXIT_ISR();
}

HAL_ISR_FUNCTION( halDmaIsr, DMA_VECTOR )
{
  HAL_ENTER_ISR();

  DMAIF = 0;
  if (DMAIF1){
	  DMAIF1=0;
	  DMA1Ready=1;
	  dbIter=nextDbIter;
  }
  CLEAR_SLEEP_MODE();
  HAL_EXIT_ISR();
}
