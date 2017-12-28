/**************************************************************************************************
  Filename:       ZigBeeUsbBrudge.c


  Copyright 2014 Paolo Achdjian

**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "OSAL.h"
#include "OSAL_Memory.h"
#include "AF.h"
#include "ZDApp.h"
#include "ZDObject.h"
#include "ZDProfile.h"
#include "ZCL_HA.h"
#include "zcl.h"
#include "zcl_functions.h"
#include "TimerEvents.h"
#include "AddrMgr.h"

#include "OnBoard.h"

#include "ZigBeeSerialBridge.h"
#include "ZdoMessageHandlers.h"
//#include "SerialFunctions.h"
#include "SerialSend/SendMethods.h"	 
#include "CheckingChildList.h"
#include "ZigbeeSerialBridge_data.h"	 

	 
#if !defined( WIN32 )
  #include "OnBoard.h"
#endif

/* HAL */
#include "hal_led.h"
#include "stdio.h"
#include "endpointRequestList.h"

#define DEVICE_VERSION     0
#define FLAGS              0


/*********************************************************************
 * LOCAL VARIABLES
 */
byte serialDongleTaskID;   // Task ID for internal task/event processing


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void messageMSGCB( afIncomingMSGPacket_t *pckt );

#define MAX_INCLUSTERS       4
const cId_t InClusterList[MAX_INCLUSTERS] ={
  ZCL_CLUSTER_ID_GEN_BASIC,
  ZCL_CLUSTER_ID_GEN_IDENTIFY,
};

#define OUTCLUSTERS       2
const cId_t OutClusterList[OUTCLUSTERS] ={
  ZCL_CLUSTER_ID_GEN_BASIC,
  ZCL_CLUSTER_ID_GEN_IDENTIFY
};

SimpleDescriptionFormat_t simpleDesc = {
  ENDPOINT,                  			//  int Endpoint;
  ZCL_HA_PROFILE_ID,                	//  uint16 AppProfId[2];
  ZCL_HA_DEVICEID_CONFIGURATION_TOOL,     	//  uint16 AppDeviceId[2];
  DEVICE_VERSION,           			//  int   AppDevVer:4;
  FLAGS,                    			//  int   AppFlags:4;
  MAX_INCLUSTERS,              //  byte  AppNumInClusters;
  (cId_t *)InClusterList, 	//  byte *pAppInClusterList;
  OUTCLUSTERS	,           //  byte  AppNumInClusters;
  (cId_t *)OutClusterList 	//  byte *pAppInClusterList;
};

/*********************************************************************
 * @fn      serialDongleAppInit
 *
 * @brief   Initialization function for the Generic App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void serialDongleAppInit( byte task_id ){
	serialDongleTaskID = task_id;
	
	serialInit();
	
	sendMessage("Start Serial ZDongle\n");
	  
	endpointRequestTaskId(serialDongleTaskID);
	zclHA_Init( &simpleDesc );
  	// Register the Application to receive the unprocessed Foundation command/response messages
 	zcl_registerForMsg( serialDongleTaskID );
	
	ZDO_RegisterForZDOMsg( serialDongleTaskID, End_Device_Bind_rsp );
	ZDO_RegisterForZDOMsg( serialDongleTaskID, Mgmt_Bind_rsp );
	ZDO_RegisterForZDOMsg( serialDongleTaskID, Match_Desc_rsp );
	ZDO_RegisterForZDOMsg( serialDongleTaskID, Device_annce);
	ZDO_RegisterForZDOMsg( serialDongleTaskID, Active_EP_rsp);
	ZDO_RegisterForZDOMsg( serialDongleTaskID, Simple_Desc_rsp);
	ZDO_RegisterForZDOMsg( serialDongleTaskID, IEEE_addr_rsp);
	ZDO_RegisterForZDOMsg( serialDongleTaskID, NWK_addr_rsp);
	ZDO_RegisterForZDOMsg( serialDongleTaskID, Power_Desc_rsp);
	
	
	T1CTL=1;
	
	osal_start_timerEx(task_id, ALIVE, 5000);
//	osal_start_timerEx(task_id, WATCHDOG, 100);
}

/*********************************************************************
 * @fn      GenericApp_ProcessEvent
 *
 * @brief   Generic Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The OSAL assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  none
 */
UINT16 processEvent( byte task_id, UINT16 events ){
	(void)task_id;  // Intentionally unreferenced parameter
	UINT16 result=0;
		
	if ( events & SYS_EVENT_MSG ) {
		osal_event_hdr_t * hdrEvent = (osal_event_hdr_t *) osal_msg_receive( serialDongleTaskID );
		switch(hdrEvent->event){
		case ZCL_INCOMING_MSG:
			zclCoordinatort_ProcessZCLIncomingMsg( (zclIncomingMsg_t *)hdrEvent );
			break;
		case ZDO_CB_MSG:{
			zdoIncomingMsg_t * zdoMsg =  (zdoIncomingMsg_t *)hdrEvent;
			ZDOMessageHandlerFactory(zdoMsg->clusterID)(zdoMsg);
			}
			break;
		case AF_INCOMING_MSG_CMD:
			messageMSGCB((afIncomingMSGPacket_t *) hdrEvent );
			break;
		case AF_DATA_CONFIRM_CMD:
			break;
		case EVENT_SERIAL_CMD:	{
			struct UsbISR * isr = (struct UsbISR*)hdrEvent;
			isr->isr(hdrEvent);
			}
			break;
		}
		osal_msg_deallocate( (uint8 *)hdrEvent );
	    result = (events ^ SYS_EVENT_MSG);
		goto end;
	}

	if (events & ENDPOINT_REQUEST_MSG){
		sendOneEndpointRequest();
		result = (events ^ ENDPOINT_REQUEST_MSG);
		goto end;
	}
	
	if (events & CHECHKING_CHILD_TIMER){
		nextCheckRequest(serialDongleTaskID);
		result = (events ^ CHECHKING_CHILD_TIMER);
	}
	
	if (events & ALIVE) {
		sendAliveMsg();
		sendAliveMsg();
		result = (events ^ ALIVE);
		osal_start_timerEx(task_id, ALIVE, 5000);
	}
	
	if (events & WATCHDOG) {
		WDCTL = WDCLP1 | 0x08;
  		WDCTL = WDCLP2 | 0x08;
		result = (events ^ WATCHDOG);
		osal_start_timerEx(task_id, WATCHDOG, 100);
	}
	
end:
	return result;
}



/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      messageMSGCB
 *
 * @brief   Data message processor callback.  This function processes
 *          any incoming data - probably from other devices.  So, based
 *          on cluster ID, perform the intended action.
 *
 * @param   none
 *
 * @return  none
 */
void messageMSGCB( afIncomingMSGPacket_t *pkt )
{
  switch ( pkt->clusterId )
  {
    case GENERICAPP_CLUSTERID:
      // "the" message
      break;
  }
}




/*********************************************************************
*********************************************************************/
