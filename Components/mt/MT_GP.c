/***************************************************************************************************
  Filename:       MT_GP.c
  Revised:        $Date: 2016-06-21 01:06:52 -0700 (Thu, 21 July 2016) $
  Revision:       $Revision:  $

  Description:    MonitorTest functions GP interface.

  Copyright 2007-2013 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License"). You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product. Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED ?AS IS? WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
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

 ***************************************************************************************************/

/***************************************************************************************************
 * INCLUDES
 ***************************************************************************************************/
 
 
#include "ZComDef.h"

 
/***************************************************************************************************
* LOCAL FUNCTIONs
***************************************************************************************************/

/* GP event header type */
typedef struct
{
  uint8   event;              /* MAC event */
  uint8   status;             /* MAC status */
} gpEventHdr_t;


typedef struct
{
uint8        AppID;
union
  {
    uint32       SrcID;
    uint8        GPDExtAddr[Z_EXTADDR_LEN];
  }GPDId;
}gpd_ID_t;


typedef struct
{
gpEventHdr_t hdr;
uint8        status;
uint8        GPEPhandle;
}gp_DataCnf_t;

typedef struct
{
uint8        GPDFSecLvl;
uint8        GPDFKeyType;
uint32       GPDSecFrameCounter;
}gp_SecData_t;


typedef struct
{
gpEventHdr_t hdr;
gpd_ID_t     gpd_ID;
uint8        EndPoint;
gp_SecData_t gp_SecData;
uint8        dGPStubHandle;
}gp_SecReq_t;

typedef struct
{
uint8                  dGPStubHandle;
struct gp_DataInd_tag  *next;
uint32                 timeout;
}gp_DataIndSecReq_t;


typedef struct gp_DataInd_tag
{
gpEventHdr_t        hdr;
gp_DataIndSecReq_t  SecReqHandling;
uint32              timestamp;
uint8               status;
int8                Rssi;
uint8               LinkQuality;
uint8               SeqNumber;
sAddr_t             srcAddr;
uint16              srcPanID;
uint8               appID;
uint8               GPDFSecLvl;
uint8               GPDFKeyType;
bool                AutoCommissioning;
bool                RxAfterTx;
uint32              SrcId;
uint8               EndPoint;
uint32              GPDSecFrameCounter;
uint8               GPDCmmdID;
uint32              MIC;
uint8               GPDasduLength;
uint8               GPDasdu[1];         //This is a place holder for the buffer, the length depends on GPDasduLength
}gp_DataInd_t;

void MT_GPDataCnf(gp_DataCnf_t* gp_DataCnf);
void MT_GPSecReq(gp_SecReq_t* gp_SecReq);
void MT_GPDataInd(gp_DataInd_t* gp_DataInd);



/***************************************************************************************************
* @fn      MT_GPDataInd
*
* @brief   Send GP Data Ind to Host Processor
*
* @param   gp_DataInd
*
* @return  void
***************************************************************************************************/
void MT_GPDataInd(gp_DataInd_t* gp_DataInd)
{
  (void)gp_DataInd;
}




/***************************************************************************************************
* @fn      MT_GPDataCnf
*
* @brief   Send GP Data Cnf to Host Processor
*
* @param   gp_DataCnf
*
* @return  void
***************************************************************************************************/
void MT_GPDataCnf(gp_DataCnf_t* gp_DataCnf)
{

  (void)gp_DataCnf;
}

/***************************************************************************************************
* @fn      MT_GPSecReq
*
* @brief   Send GP Sec Req to Host Processor
*
* @param   gp_SecReq
*
* @return  void
***************************************************************************************************/
void MT_GPSecReq(gp_SecReq_t* gp_SecReq)
{
  (void)gp_SecReq;
}

