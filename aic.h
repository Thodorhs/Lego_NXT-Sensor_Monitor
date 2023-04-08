#ifndef   AIC
#define   AIC

#include <stdconst.h>

void      AICInit(void);
void      AICInterruptEnable(int which, void (*handler)(void));
void      AICInterruptDisable(int which);

#endif
