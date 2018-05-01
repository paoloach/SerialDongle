
/**************************************************************************************************

 DESCRIPTION:
  --

 CREATED: 18/02/2015, by Paolo Achdjian

 FILE: OSAL_UsbCoordinator.c

***************************************************************************************************/


/*********************************************************************
 * INCLUDES
 */

#include "ZComDef.h"
#include "hal_drivers.h"
#include "OSAL.h"
#include "OSAL_Tasks.h"
#include "nwk.h"
#include "APS.h"
#include "ZDApp.h"
#include "bdb_interface.h"
#include "ZDNwkMgr.h"
#include "aps_frag.h"
#include "gp_common.h"

//#include "zcl_UsbCoordinator.h"
#include "zcl.h"
#include "ZigBeeSerialBridge.h"

/*********************************************************************
 * OBAL VARIABLES
 */


// The order in this table must be identical to the task initialization calls below in osalInitTask.
const pTaskEventHandlerFn tasksArr[11] = {
  macEventLoop,
  nwk_event_loop,
  gp_event_loop,
  Hal_ProcessEvent,
  APS_event_loop,
  APSF_ProcessEvent,
  ZDApp_event_loop,
  ZDNwkMgr_event_loop,
  //Added to include TouchLink functionality
  //#if defined ( INTER_PAN )
  //  StubAPS_ProcessEvent,
  //#endif
  // Added to include TouchLink initiator functionality
  //#if defined ( BDB_TL_INITIATOR )
  //  touchLinkInitiator_event_loop,
  //#endif
  // Added to include TouchLink target functionality
  //#if defined ( BDB_TL_TARGET )
  //  touchLinkTarget_event_loop,
  //#endif
  zcl_event_loop,
  bdb_event_loop,
  processEvent
};


uint16  tasksEvents[11];

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/

/*********************************************************************
 * @fn      osalInitTasks
 *
 * @brief   This function invokes the initialization function for each task.
 *
 * @param   void
 *
 * @return  none
 */

void osalInitTasks( void )
{
  macTaskInit( 0 );
  nwk_init( 1 );
  gp_Init( 2 );
  Hal_Init( 3 );
  APS_Init( 4 );
  APSF_Init( 5 );
  ZDApp_Init( 6);
  ZDNwkMgr_Init( 7 );
  // Added to include TouchLink functionality 
//  #if defined ( INTER_PAN )
//    StubAPS_Init( taskID++ );
//  #endif
  // Added to include TouchLink initiator functionality 
//  #if defined( BDB_TL_INITIATOR )
//    touchLinkInitiator_Init( taskID++ );
//  #endif
  // Added to include TouchLink target functionality 
//  #if defined ( BDB_TL_TARGET )
//    touchLinkTarget_Init( taskID++ );
//  #endif
  zcl_Init( 8 );
  bdb_Init( 9 );
  serialDongleAppInit( 10 );
}




/*********************************************************************
*********************************************************************/
