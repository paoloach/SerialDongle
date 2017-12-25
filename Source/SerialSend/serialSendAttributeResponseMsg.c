
#include "SendMethods.h"
#include "MessageCode.h"

// 1 byte  -> code 
// 2 bytes -> network id (1)
// 1 byte  -> endpoint id (3)
// 2 bytes -> cluster id (4)
// 2 bytes -> attribute id (6)
// 1 byte  -> status (8)
// if attribute has status == SUCCESS
//   1 byte  -> data type
//   1 byte  -> attribute data length
//   n bytes -> attribute data
void serialSendAttributeResponseMsg(zclReadRspCmd_t * readRsp, uint16 clusterId, afAddrType_t * addr){
	uint8 * iter ;

	zclReadRspStatus_t * iterAttr = readRsp->attrList;
	zclReadRspStatus_t * iterEnd = readRsp->attrList+readRsp->numAttr;
	uint8 attrSize;
	uint8 i;
	uint8 size;
	
	for (;iterAttr != iterEnd; iterAttr++){

		struct DataSend * dataSend;
		while((dataSend = getSendBuffer())==NULL);
		iter = dataSend->start;
		iter = sendUInt16(iter, addr->addr.shortAddr);
		*iter = addr->endPoint;
		iter++;
		iter = sendUInt16(iter, clusterId);
		iter = sendUInt16(iter, iterAttr->attrID);
		*iter = iterAttr->status;	
		iter++;
		size=8;
		if (iterAttr->status == ZCL_STATUS_SUCCESS){
			*iter = iterAttr->dataType;
			iter++;
			size++;
			attrSize = zclGetAttrDataLength(iterAttr->dataType, iterAttr->data);
			*iter =attrSize;
			iter++;
			size++;
			for (i=0; i < attrSize; i++){
				*iter = iterAttr->data[i];
				iter++;
				size++;
			}
		}
		send(AttributeResponse, size,dataSend);
	}
}
