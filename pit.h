#ifndef   PIT
#define   PIT
#include <stdconst.h>

#define PIT_USE_INTERUPTS 1

void	  PITMrPivSet(ULONG piv);
void      PITEnable(void);
void      PITDisable(void);
ULONG     PITRead(void);
ULONG     PITReadReset(void);
void      PITInterruptEnable(ULONG period, void (*handler)(void));
void      PITInterruptDisable(void);
void      PITAckInterrupt(void);
UWORD	  PITTicks2ms(ULONG ticks);
UWORD	  PITTicks2s(ULONG ticks);
void 	  spindelayms(ULONG ms);

#endif
