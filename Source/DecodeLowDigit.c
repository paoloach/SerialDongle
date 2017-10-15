
#include "SerialProcessEventsInternal.h"

uint8 decodeLowDigit( ) {
	uint8 c = *rxDataRead;
	incDataRead;
	if (c >= '0' && c <= '9')
		return c -'0';
	else
		return 10+c - 'A';
}