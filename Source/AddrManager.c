#include "hal_types.h"
#include "OSAL_Memory.h"
#include "OSAL.h"
#include "ZComDef.h"
#include "saddr.h"
#include "AddrMgr.h"
#include "ZComDef.h"
#include "OSAL_Nv.h"
#include "nwk_globals.h"
#include "AddrManager.h"


__sfr __no_init uint8 i @ 0xd2;

struct Element {
	uint8  user;
	uint16 nwkAddr;
	uint8  extAddr[Z_EXTADDR_LEN];
};

struct Element elements[NWK_MAX_ADDRESSES];

#define UPDATE	0x80
#define USED	0x40

static uint8 MyAddrMgrEntryValidate(AddrMgrEntry_t * entry){
	if (entry->index < NWK_MAX_ADDRESSES  && (entry->user & 0x0F)!=0)
		return 1;
	return 0;
}

/******************************************************************************
 * @fn          MyAddrMgrInit
 *
 * @brief       Initialize Address Manager.
 *
 * @param       entryTotal - [in] total number of address entries
 *
 * @return      none
 */
void MyAddrMgrInit( uint16 entryTotal ) {
	osal_memset(elements, 0, NWK_MAX_ADDRESSES); 
}


/******************************************************************************
 * @fn          MyAddrMgrSetDefaultNV
 *
 * @brief       Set default address entry data in NV.
 *
 * @param       none
 *
 * @return      none
 */
void MyAddrMgrSetDefaultNV( void ){
	osal_nv_item_init(ZCD_NV_ADDRMGR, NWK_MAX_ADDRESSES*sizeof(struct Element), NULL);
}


/******************************************************************************
 * @fn          MyAddrMgrRestoreFromNV
 *
 * @brief       Restore the address entry data from NV.
 *
 * @param       none
 *
 * @return      none
 */
void MyAddrMgrRestoreFromNV( void ) {
	/*
	uint16 len = osal_nv_item_len(ZCD_NV_ADDRMGR);
	if (len > NWK_MAX_ADDRESSES*sizeof(struct Element))
		len = NWK_MAX_ADDRESSES*sizeof(struct Element);
	osal_nv_read(ZCD_NV_ADDRMGR, 0, len, elements);
	if (len < NWK_MAX_ADDRESSES*sizeof(struct Element)){
		osal_memset(elements+len,0, NWK_MAX_ADDRESSES*sizeof(struct Element) - len);
	}
	*/
}


/******************************************************************************
 * @fn          MyAddrMgrWriteNV
 *
 * @brief       Save the address entry data to NV.
 *
 * @param       none
 *
 * @return      none
 */
void MyAddrMgrWriteNV( void ) {
	for(uint8 i=0; i < NWK_MAX_ADDRESSES; i++){
		if (elements[i].user & UPDATE){
			osal_nv_write(ZCD_NV_ADDRMGR, i*sizeof(struct Element), sizeof(struct Element), elements+i);
		}
	}
}



/******************************************************************************
 * @fn          AddrMgrExtAddrSet
 *
 * @brief       Set destination address to source address or empty{0x00}.
 *
 * @param       dstExtAddr - [in] destination EXT address
 *              srcExtAddr - [in] source EXT address
 *
 * @return      none
 */
void MyAddrMgrExtAddrSet( uint8* dstExtAddr, uint8* srcExtAddr ){
	if (srcExtAddr == NULL){
		osal_memset(dstExtAddr, 0, Z_EXTADDR_LEN);
	} else {
		sAddrExtCpy(dstExtAddr, srcExtAddr);
	}
}


/******************************************************************************
 * @fn          MyAddrMgrExtAddrValid
 *
 * @brief       Check if EXT address is valid - not NULL, not empty{0x00}.
 *
 * @param       extAddr - [in] EXT address
 *
 * @return      uint8 - success(TRUE:FALSE)
 */
uint8 MyAddrMgrExtAddrValid( uint8* extAddr ){
	for(uint8 i=0; i < Z_EXTADDR_LEN; i++){
		if (extAddr[i] !=0)
			return 1;
	}
	return 0;
}

/******************************************************************************
 * @fn          AddrMgrExtAddrLookup
 *
 * @brief       Lookup EXT address using the NWK address.
 *
 * @param       nwkAddr - [in] NWK address
 *              extAddr - [out] EXT address
 *
 * @return      uint8 - success(TRUE:FALSE)
 */
uint8 MyAddrMgrExtAddrLookup( uint16 nwkAddr, uint8* extAddr ) {
	for(uint8 i=0; i < NWK_MAX_ADDRESSES; i++){
		if (elements[i].nwkAddr == nwkAddr){
			sAddrExtCpy(extAddr, elements[i].extAddr);
			return 1;
		}
	}
	return 0;
}

/******************************************************************************
 * @fn          AddrMgrNwkAddrLookup
 *
 * @brief       Lookup NWK address using the EXT address.
 *
 * @param       extAddr - [in] EXT address
 *              nwkAddr - [out] NWK address
 *
 * @return      uint8 - success(TRUE:FALSE)
 */
uint8 MyAddrMgrNwkAddrLookup( uint8* extAddr, uint16* nwkAddr ) {
	for(uint8 i=0; i < NWK_MAX_ADDRESSES; i++){
		if (sAddrExtCmp(extAddr, elements[i].extAddr)){
			*nwkAddr = elements[i].nwkAddr;
			return 1;
		}
	}
	return 0;
}

/******************************************************************************
 * @fn          AddrMgrEntryRelease
 *
 * @brief       Release a user reference from an entry in the Address Manager.
 *
 * @param       entry
 *                ::user  - [in] user ID
 *                ::index - [in] index of data
 *                ::nwkAddr - not used
 *                ::extAddr - not used
 *
 * @return      uint8 - success(TRUE:FALSE)
 */
uint8 MyAddrMgrEntryRelease( AddrMgrEntry_t* entry ) {
	if (MyAddrMgrEntryValidate(entry)){
		elements[entry->index].user = (elements[entry->index].user & entry->user) | UPDATE;
		AddrMgrWriteNVRequest();
		return 1;
	}
	return 0;
}


/******************************************************************************
 * @fn          AddrMgrEntryAddRef
 *
 * @brief       Add a user reference to an entry in the Address Manager.
 *
 * @param       entry
 *                ::user  - [in] user ID
 *                ::index - [in] index of data
 *                ::nwkAddr - not used
 *                ::extAddr - not used
 *
 * @return      uint8 - success(TRUE:FALSE)
 */
uint8 MyAddrMgrEntryAddRef( AddrMgrEntry_t* entry ) {
	if (MyAddrMgrEntryValidate(entry)){
		if(elements[entry->index].user != entry->user){
			elements[entry->index].user = (elements[entry->index].user | entry->user) | UPDATE;
			AddrMgrWriteNVRequest();
			return 1;
		}
	}
	return 0;
}

/******************************************************************************
 * @fn          AddrMgrEntryLookupNwk
 *
 * @brief       Lookup entry based on NWK address.
 *
 * @param       entry
 *                ::user    - [in] user ID
 *                ::nwkAddr - [in] NWK address
 *                ::extAddr - [out] EXT address
 *                ::index   - [out] index of data
 *
 * @return      uint8 - success(TRUE:FALSE)
 */
uint8 MyAddrMgrEntryLookupNwk( AddrMgrEntry_t* entry ) {
	for(uint8 i=0; i < NWK_MAX_ADDRESSES; i++){
		if (elements[i].nwkAddr == entry->nwkAddr){
			sAddrExtCpy(entry->extAddr, elements[i].extAddr);
			entry->index = i;
			return 1;
		}
	}
	return 0;
}

/******************************************************************************
 * @fn          AddrMgrEntryLookupExt
 *
 * @brief       Lookup entry based on EXT address.
 *
 * @param       entry
 *                ::user    - [in] user ID
 *                ::extAddr - [in] EXT address
 *                ::nwkAddr - [out] NWK address
 *                ::index   - [out] index of data
 *
 * @return      uint8 - success(TRUE:FALSE)
 */
uint8 MyAddrMgrEntryLookupExt( AddrMgrEntry_t* entry ){
	for(uint8 i=0; i < NWK_MAX_ADDRESSES; i++){
		if (sAddrExtCmp(entry->extAddr, elements[i].extAddr) && elements[i].user == entry->user){
			entry->nwkAddr  = elements[i].nwkAddr;
			entry->index=i;
			return 1;
		}
	}
	return 0;

}

/******************************************************************************
 * @fn          AddrMgrEntryGet
 *
 * @brief       Get NWK address and EXT address based on index.
 *
 * @param       entry
 *                ::user    - [in] user ID
 *                ::index   - [in] index of data
 *                ::nwkAddr - [out] NWK address
 *                ::extAddr - [out] EXT address
 *
 * @return      uint8 - success(TRUE:FALSE)
 */
uint8 MyAddrMgrEntryGet( AddrMgrEntry_t* entry ) {
	if (MyAddrMgrEntryValidate(entry)){
		if (elements[entry->index].user == entry->user){
			sAddrExtCpy(entry->extAddr, elements[entry->index].extAddr);
			entry->nwkAddr = elements[entry->index].nwkAddr;
			return 1;
		}
	}
	return 0;
}

/******************************************************************************
 * @fn          MyAddrMgrEntryUpdate
 *
 * @brief       Update an entry into the Address Manager.
 *
 * @param       entry
 *                ::user    - [in] user ID
 *                ::nwkAddr - [in] NWK address
 *                ::extAddr - [in] EXT address
 *                ::index   - [out] index of data
 *
 * @return      uint8 - success(TRUE:FALSE)
 */
uint8 MyAddrMgrEntryUpdate( AddrMgrEntry_t* entry ){
	for(uint8 i=0; i < NWK_MAX_ADDRESSES; i++){
		if (sAddrExtCmp(entry->extAddr, elements[i].extAddr)){
			elements[i].nwkAddr = entry->nwkAddr;
			elements[i].user = entry->user| UPDATE;
			entry->index=i;
			return 1;
		}
	}
	
	for(uint8 i=0; i < NWK_MAX_ADDRESSES; i++){
		if (elements[i].nwkAddr == entry->nwkAddr){
			sAddrExtCpy(elements[i].extAddr,entry->extAddr);
			elements[i].user = entry->user| UPDATE;
			entry->index = i;
			return 1;
		}
	}
	
	for(uint8 i=0; i < NWK_MAX_ADDRESSES; i++){
		if ((elements[i].user & USED) == 0){
			sAddrExtCpy(elements[i].extAddr,entry->extAddr);
			elements[i].nwkAddr = entry->nwkAddr;
			elements[i].user = entry->user | UPDATE | USED;
			entry->index = i;
			return 1;
		}
	}
	return 0;
}

/******************************************************************************
 * @fn          AddrMgrIsFull
 *
 * @brief       Checks to see if the address manager is full.
 *
 * @param       none
 *
 * @return      uint8 - success(TRUE:FALSE)
 *                      TRUE if there are no more empty slots, 
 *                      FALSE if available slot
 */
uint8 MyAddrMgrIsFull( void ) {
	for(uint8 i=0; i < NWK_MAX_ADDRESSES; i++){
		if ((elements[i].user & USED) == 0){
			return 0;
		}
	}
	return 1;
}