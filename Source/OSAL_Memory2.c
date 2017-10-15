#include "OSAL_Memory.h"
#include "string.h"
#include "OnBoard.h"
#include "hal_types.h"


void *osal_mem_alloc_dbg( uint16 size, const char *fname, unsigned lnum );
#undef osal_mem_alloc
void *osal_mem_alloc( uint16 size );

uint16 allocated[50];

#ifdef __cplusplus
extern "C"
{
#endif

//void * osal_mem_alloc( uint16 size );
	
struct osalMemHdrHdr {
	unsigned name: 1;
	unsigned ref: 1;
  	// The 1 MSB of 'val' is used as a boolean to indicate in-use or freed.
	unsigned inUse : 1;
	uint16 id;
	struct osalMemHdrHdr * next ;
	struct osalMemHdrHdr * prev ;
} ;

static const char * getFileName(const char * fname);
static uint8 theHeap[MAXMEMHEAP];
static uint16 counter=1;

void osal_mem_init( void ){
	osal_memset(theHeap,0,MAXMEMHEAP);
	((struct osalMemHdrHdr *)theHeap)->prev = NULL;
	((struct osalMemHdrHdr *)theHeap)->next = (struct osalMemHdrHdr *)( theHeap+MAXMEMHEAP);
}

void osal_mem_kick( void ){
}
  
uint8 memCheck(void) {
	halIntState_t intState;	
	HAL_ENTER_CRITICAL_SECTION( intState );  // Hold off interrupts.
	struct osalMemHdrHdr * iter = (struct osalMemHdrHdr *)theHeap;
	volatile struct osalMemHdrHdr * prev;
	while( (uint8 *)iter < theHeap +MAXMEMHEAP){
		prev = iter;
		if (iter >= iter->next)
			goto errorCheck;
		if (iter <= iter->prev)
			goto errorCheck;
		iter = iter->next;
		if ((uint8 *)iter > theHeap + MAXMEMHEAP  || (uint8 *)iter < theHeap ){
			goto errorCheck;
		}
	}
	HAL_EXIT_CRITICAL_SECTION( intState ); 
	return 0;
errorCheck:
	HAL_EXIT_CRITICAL_SECTION( intState ); 
	return 0xFF;
}

void *osal_mem_alloc_dbg( uint16 size, const char *fname, unsigned lnum ){
		halIntState_t intState;
	size+=4;
	memCheck();	
	uint8 fNameSize;
	uint16 sizeNeed;
	const char * fileName ;
	if (fname == NULL){
		fNameSize=0;
		sizeNeed = size+sizeof(struct osalMemHdrHdr)+sizeof(uint8 *);
	}else{
		fileName = getFileName(fname);
		fNameSize= strlen(fileName)+1;
		sizeNeed = size+2+fNameSize+sizeof(struct osalMemHdrHdr)+sizeof(uint8 *);
	}
	
	struct osalMemHdrHdr * iter = (struct osalMemHdrHdr *)theHeap;
	HAL_ENTER_CRITICAL_SECTION( intState );  // Hold off interrupts.
	struct osalMemHdrHdr * next = NULL;
	while( true){
		if ((uint8 *)iter >= theHeap + MAXMEMHEAP ){
			return NULL;
		}
		if (iter->inUse!=0){
			iter = iter->next;
		} else {
			uint16 len = (uint8*)(iter->next)-(uint8*)iter;
			if ( len < sizeNeed){
				iter = iter->next;
			} else {
				if (len > sizeNeed+ sizeof(struct osalMemHdrHdr)){
					next = (struct osalMemHdrHdr *)((uint8 *)iter +  sizeNeed);
					next->inUse=0;
					next->next=iter->next;
					next->prev=iter;
					iter->next=next;
				}
				iter->inUse=1;
				break;
			}
		}
		
	}

	uint8 * start = (uint8*) iter;
	iter->inUse=1;  
	iter->id = counter;
	counter++;
	uint8 * data = (uint8 *)iter;
	data += sizeof(struct osalMemHdrHdr);
	if (fname != NULL){
		iter->name=1;
		osal_memcpy(data, fileName, fNameSize);
		data+=fNameSize;
		*((uint16 *)data) = lnum;
		data+=2;
	} else {
		iter->name=0;
	}
	* ((uint8 * *)data) = start;
	data+=sizeof(uint8 *);
	memset(data, 0x42,size);
	
	HAL_EXIT_CRITICAL_SECTION( intState );  // Re-enable interrupts.
	memCheck();
	return data;
}


void *osal_mem_alloc( uint16 size ){
	return osal_mem_alloc_dbg(size, NULL, 0);
}

void osal_mem_free( void *ptr){
	memCheck();				   
				   
	uint8 * iter = ptr;
	iter--;
	iter--;
	struct osalMemHdrHdr * memHdr = *(struct osalMemHdrHdr **)iter;
	struct osalMemHdrHdr * prev = memHdr->prev;
	struct osalMemHdrHdr * next = memHdr->next;
	memHdr->inUse=0;
	if (prev != NULL && prev->inUse==0){
		prev->next = memHdr->next;
		memHdr = prev;	
	}
	if ((uint8 *)next < theHeap+MAXMEMHEAP && next->inUse == 0){
		memHdr->next = next->next;
	}
	
	memCheck();
}


uint16 * getAllocated() {
	uint8 i =0;
	struct osalMemHdrHdr * iter = (struct osalMemHdrHdr *)theHeap;
	while( (uint8 *)iter < theHeap+MAXMEMHEAP){
		if (iter->inUse != 0 && iter->name==1){
			if (i < 50){
				allocated[i] = iter->id;
				i++;		
			}
		}
		iter = iter->next;
	}
	for (; i< 50; i++){
		allocated[i]=0;
	}
	
	return allocated;
}

const char * getFileName(const char * fname) {
	const char * start = fname;
	while (*fname != 0){
		if (*fname == '\\')
			start = fname+1;
		fname++;
	}
	return start;
}


struct BlockName getNextNewBlockName(void) {
	struct BlockName result;
	struct osalMemHdrHdr * iter = (struct osalMemHdrHdr *)theHeap;
	while( (uint8 *)iter < theHeap+MAXMEMHEAP){
		if (iter->inUse != 0 && iter->ref==0 && iter->name==1){
			iter->ref = 1;
			char * data =  (char *)iter;
			result.fileName = data+sizeof(struct osalMemHdrHdr);
			data =result.fileName +  strlen(result.fileName)+1;
			result.lineNum = *(uint16 *)data;
			result.id = iter->id;
			return result;
		}
		iter = iter->next;
	}
	result.fileName=NULL;
	
	return result;; 
}

struct HeapData osal_heap_info(void) {
	struct HeapData result;
	result.memUsed = 0;
	result.blockAllocated = 0;
	struct osalMemHdrHdr * iter = (struct osalMemHdrHdr *)theHeap;
	while( (uint8 *)iter < theHeap+MAXMEMHEAP){
		if (iter->inUse != 0){
			result.memUsed += iter->next-iter;
			result.blockAllocated++;
		}
		iter = iter->next;
	}
	return result;

}

  
  #if ( OSALMEM_METRICS )
uint16 osal_heap_block_max( void ){
return 0;
}
uint16 osal_heap_block_cnt( void ){
return 0;
}
uint16 osal_heap_block_free( void ){
return 0;
}
uint16 osal_heap_mem_used( void ){
	uint16 used=0;
	struct osalMemHdrHdr * iter = (struct osalMemHdrHdr *)theHeap;
	while( (uint8 *)iter < theHeap+MAXMEMHEAP){
		if (iter->inUse != 0){
			used += iter->next-iter;
		}
		iter = iter->next;
	}
	return used;
}
#endif
	
	
#ifdef __cplusplus
}
#endif
