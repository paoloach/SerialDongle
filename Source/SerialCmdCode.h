/**************************************************************************************************

 DESCRIPTION:
  --

 CREATED: 29/12/2016, by Paolo Achdjian

 FILE: SerialCmdCode.h

***************************************************************************************************/

#define Z_EXTADDR_LEN   8
#define ANNUNCE 0x01
#define SIMPLE_DESC 0x03
#define REQ_ACTIVE_EP 0x04
#define ACTIVE_EP 0x05
#define REQ_ATTRIBUTE_VALUES 0x06
#define ATTRIBUTE_VALUES 0x07
#define SEND_CMD 0x08
#define WRITE_ATTRIBUTE_VALUE 0x09
#define REQ_ALL_NODES 0x0A
#define ALL_NODES 0x0B
#define REQ_BIND_TABLE 0x0C
#define BIND_TABLE					0x0D
#define REQ_ADD_BIND_TABLE_ENTRY    0x0E
#define REQ_REMOVE_BIND_TABLE_ENTRY 0x0F
#define REQ_RESET				    0x10
#define REQ_DEVICE_INFO				0x11
#define DEVICE_INFO					0x12
#define REQ_IEEE_ADDRESS			0x13
#define IEEE_ADDRESS_RESPONSE		0x14


#define INFO_MESSAGE				0x20
#define ENABLE_INFO_MESSAGE			0x21
#define ATTRIBUTE_VALUE_REQ_ERROR	0x40
#define ACTIVE_EP_REQ_ERROR			0x41
#define IEEE_ADDRESS_RESPONSE_ERROR 0x42