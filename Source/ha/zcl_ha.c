/**************************************************************************************************
  Filename:       zcl_ha.c
  Revised:        $Date: 2013-10-21 12:15:10 -0700 (Mon, 21 Oct 2013) $
  Revision:       $Revision: 35738 $

  Description:    Describe the purpose and contents of the file.


  Copyright 2013 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "ZComDef.h"
#include "OSAL.h"

#include "zcl.h"
#include "zcl_general.h"
#include "zcl_closures.h"
#include "zcl_HVAC.h"
#include "zcl_ss.h"
#include "zcl_ms.h"
#include "zcl_lighting.h"
#include "zcl_ha.h"


/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * STATUS STRINGS
 */
// NwkInfo displayed on line 1 once on the network
// 0123456789012345
// Zc: PPPP CH ADDR
// Zr: PPPP CH ADDR
// Zed:PPPP CH ADDR
const char NwkInfoZcStr[]  = "ZC: ";
const char NwkInfoZrStr[]  = "ZR: ";
const char NwkInfoZedStr[] = "Zed:";
const char * const NwkInfoStrs[] = { NwkInfoZcStr, NwkInfoZrStr, NwkInfoZedStr };

/*********************************************************************
 * LOCAL FUNCTIONS
 */

/*********************************************************************
 * @fn      zclHA_Init
 *
 * @brief   Register the Simple descriptor with the HA profile.
 *          This function also registers the profile's cluster
 *          conversion table.
 *
 * @param   simpleDesc - a pointer to a valid SimpleDescriptionFormat_t, must not be NULL.
 *
 * @return  none
 */
void zclHA_Init( SimpleDescriptionFormat_t *simpleDesc )
{
  endPointDesc_t *epDesc;

  // Register the application's endpoint descriptor
  //  - This memory is allocated and never freed.
  epDesc = osal_mem_alloc( sizeof ( endPointDesc_t ) );
  if ( epDesc )
  {
    // Fill out the endpoint description.
    epDesc->endPoint = simpleDesc->EndPoint;
    epDesc->task_id = &zcl_TaskID;   // all messages get sent to ZCL first
    epDesc->simpleDesc = simpleDesc;
    epDesc->latencyReq = noLatencyReqs;

    // Register the endpoint description with the AF
    afRegister( epDesc );
  }
}

/*********************************************************************
 * @fn      zclHA_isbit
 *
 * @brief   Returns TRUE if the index bit is set
 *
 * @param   pArray   - array with bits in it
 *          bitIndex - index into the array of bits
 *
 * @return  TRUE if index bit is set
 */
bool zclHA_isbit(uint8 *pArray, uint8 bitIndex)
{
  uint8 bit;
  bit = ( 1 << ( bitIndex & 0x7 ) );
  if( pArray[bitIndex >> 3] & bit )
    return TRUE;
  return FALSE;
}

/*********************************************************************
 * @fn      zclHA_setbit
 *
 * @brief   Sets a bit in a bit array
 *
 * @param   pArray   - array with bits in it
 *          bitIndex - index into the array of bits
 *
 * @return  none
 */
void zclHA_setbit(uint8 *pArray, uint8 bitIndex)
{
  uint8 bit;
  bit = ( 1 << ( bitIndex & 0x7 ) );
  pArray[bitIndex >> 3] |= bit;
}

/*********************************************************************
 * @fn      zclHA_clearbit
 *
 * @brief   clears a bit in a bit array
 *
 * @param   pArray   - array with bits in it
 *          bitIndex - index into the array of bits
 *
 * @return  none
 */
void zclHA_clearbit(uint8 *pArray, uint8 bitIndex)
{
  uint8 bit;
  bit = ( 1 << ( bitIndex & 0x7 ) );
  pArray[bitIndex >> 3] &= (~(unsigned)bit);
}

/*********************************************************************
 * @fn      zclHA_uint16toa
 *
 * @brief   Converts from a uint16 to ascii hex string.
 *          The # will be exactly 4 hex digits (e.g. 0x0000 or 0x1E3F).
 *          NULL terminates the string.
 *
 * @param   u - Number to be converted
 *          string - pointer to coverted string
 *
 * @return  none
 */
void zclHA_uint16toa(uint16 u, char *string)
{
  // add preceding zeros
  if ( u < 0x1000 )
  {
    *string++ = '0';
  }

  if ( u < 0x0100 )
  {
    *string++ = '0';
  }

  if ( u < 0x0010 )
  {
    *string++ = '0';
  }

  _ltoa((unsigned long)u, (unsigned char *)string, 16);
}

/*********************************************************************
 * @fn      zclHA_uint8toa
 *
 * @brief   Converts from a uint8 to ascii decimal string.
 *          The # will be exactly 3 digits (e.g. "123", " 64", "  0"), preceded by spaces.
 *          No null terminator.
 *
 * @param   b - Byte to be converted
 *          string - pointer to coverted string
 *
 * @return  none
 */
void zclHA_uint8toa(uint8 b, char *string)
{
  // write the level (up to 3 digits, pre-padded with spaces)
  string[0] = string[1] = string[2] = '0';
  string[2] = '0' + (b % 10);  // 1s
  b = b / 10;
  if( b )
  {
    string[1] = '0' + (b % 10);  // 10s
    b = b / 10;
  }
  if( b )
  {
   string[0] = '0' + (b % 10);  // 100s
  }
}


/*********************************************************************
*********************************************************************/
