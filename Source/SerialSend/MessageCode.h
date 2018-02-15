#ifndef __MESSAGE_CODE__H_
#define __MESSAGE_CODE__H_

enum MessageCode {
	STRING=1,
	MSG_ALIVE=2,
	DeviceInfo=3,
	ActiveEP_Error=4,
	AttributeResponseMsgError=5,
	AttributeResponse=6,
	Annunce=7,
	NodePowerError=8,
	NodePower=9,
	SimpleDescriptor=10,
	IEEEAddress=11,
	IEEEAddressError=12,
	BindTable=13,
	WriteAttributeError=14,
	NodeDescriptionError=15,
	NodeDescription=16,
	MgmtLqi=17,
	MgmqLqiNotSupported=18
};

#endif