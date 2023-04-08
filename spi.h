#ifndef   _SPI_H_
#define   _SPI_H_

#include <stdconst.h>

void SPIInit(void);
unsigned int SPITxReady(void);
unsigned int SPIRxReady(void);
void SPIWrite(UBYTE *buff, UBYTE l);
void SPIWriteDMA(UBYTE *buff, UBYTE l);
void SPIRead(UBYTE *buff, UBYTE l);
void SPIPIOSetData(void);
void SPIPIOClearData(void);
#endif
