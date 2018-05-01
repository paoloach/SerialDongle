
#include "SendMethods.h"
#include "MessageCode.h"

// 1 byte  -> code 
// 2 bytes -> network id (1)
// 1 byte  -> endpoint id (3)
// 2 bytes -> cluster id (4)
// 1 byte --> attribute count (6)
//  for each attribute
// 		2 bytes -> attribute id (start+7)
// 		1 byte  -> status (start+9)
// 		if attribute has status == SUCCESS
//   		1 byte  -> data type(start+10)
//   		1 byte  -> attribute data length(start+11)
//   		n bytes -> attribute data(start +12)
void serialSendAttributeResponseMsg(zclReadRspCmd_t * readRsp, uint16 clusterId, afAddrType_t * addr){
	uint8 * iter ;
	struct DataSend * dataSend;

	zclReadRspStatus_t * iterAttr = readRsp->attrList;
	zclReadRspStatus_t * iterEnd = readRsp->attrList+readRsp->numAttr;
	uint8 attrSize;
	uint8 i;
	uint16 size;
	
	size=7;
	for (iterAttr = readRsp->attrList; iterAttr != iterEnd; iterAttr++){
		size+=3;
		if (iterAttr->status == ZCL_STATUS_SUCCESS){
			attrSize = zclGetAttrDataLength(iterAttr->dataType, iterAttr->data);
			size+=2+attrSize;
		}
	}
	
	if (size <  LONG_BUFFER_DMA_SIZE){
		while((dataSend = getSendBuffer(size))==NULL);
		iter = dataSend->start;
		iter = sendUInt16(iter, addr->addr.shortAddr);
		*iter = addr->endPoint;
		iter++;
		iter = sendUInt16(iter, clusterId);
		*iter = readRsp->numAttr;;
		iter++;

		for (iterAttr = readRsp->attrList;iterAttr != iterEnd; iterAttr++){
			iter = sendUInt16(iter, iterAttr->attrID);
			*iter = iterAttr->status;	
			iter++;
			if (iterAttr->status == ZCL_STATUS_SUCCESS){
				*iter = iterAttr->dataType;
				iter++;
				attrSize = zclGetAttrDataLength(iterAttr->dataType, iterAttr->data);
				*iter =attrSize;
				iter++;
				for (i=0; i < attrSize; i++){
					*iter = iterAttr->data[i];
					iter++;
				}
			}
		}
		send(AttributeResponse, dataSend);
	} else {
		initLongBuffer(size, AttributeResponse);
		longBufferSendUInt16(addr->addr.shortAddr);
		longBufferSendUInt8(addr->endPoint);
		longBufferSendUInt16(clusterId);
		longBufferSendUInt8(readRsp->numAttr);
		for (iterAttr = readRsp->attrList;iterAttr != iterEnd; iterAttr++){
			longBufferSendUInt16(iterAttr->attrID);
			longBufferSendUInt8(iterAttr->status);	
			if (iterAttr->status == ZCL_STATUS_SUCCESS){
				longBufferSendUInt8(iterAttr->dataType);
				attrSize = zclGetAttrDataLength(iterAttr->dataType, iterAttr->data);
				longBufferSendUInt8(attrSize);
				for (i=0; i < attrSize; i++){
					longBufferSendUInt8(iterAttr->data[i]);
				}
			}
		}
		longBufferSend();
	}
	
}
