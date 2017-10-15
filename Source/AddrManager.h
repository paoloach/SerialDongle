#ifndef MY_ADDR_MANAGER_H__
#define MY_ADDR_MANAGER_H__

void MyAddrMgrInit( uint16 entryTotal );
void MyAddrMgrSetDefaultNV( void );
void MyAddrMgrRestoreFromNV( void );
void MyAddrMgrWriteNV( void );
uint8 MyAddrMgrEntryUpdate( AddrMgrEntry_t* entry );
void MyAddrMgrExtAddrSet( uint8* dstExtAddr, uint8* srcExtAddr );
uint8 MyAddrMgrExtAddrValid( uint8* extAddr );
uint8 MyAddrMgrExtAddrLookup( uint16 nwkAddr, uint8* extAddr );
uint8 MyAddrMgrNwkAddrLookup( uint8* extAddr, uint16* nwkAddr );
uint8 MyAddrMgrEntryRelease( AddrMgrEntry_t* entry );
uint8 MyAddrMgrEntryAddRef( AddrMgrEntry_t* entry );
uint8 MyAddrMgrEntryLookupNwk( AddrMgrEntry_t* entry );
uint8 MyAddrMgrEntryLookupExt( AddrMgrEntry_t* entry );
uint8 MyAddrMgrEntryLookupExt( AddrMgrEntry_t* entry );
uint8 MyAddrMgrEntryAdd( AddrMgrEntry_t* entry );
uint8 MyAddrMgrIsFull( void );
uint8 MyAddrMgrEntryGet( AddrMgrEntry_t* entry );


#endif