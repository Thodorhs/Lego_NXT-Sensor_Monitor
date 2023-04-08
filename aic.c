#include "aic.h"
#include "AT91SAM7S256.h"

#define AIC_ISMR_SET_TYPE_MASK 0x00000020
#define AIC_IDCR_DIS_ALL_MASK 0x11111111

void      AICInit(void)
{
*AT91C_AIC_IDCR &= AIC_IDCR_DIS_ALL_MASK;
}

void      AICInterruptEnable(int which, void (*handler)(void))
{
*AT91C_AIC_IECR |= (1 << which);
AT91C_BASE_AIC -> AIC_SVR[which] = handler;
AT91C_BASE_AIC -> AIC_SMR[which] = AIC_ISMR_SET_TYPE_MASK;
}

void      AICInterruptDisable(int which)
{
*AT91C_AIC_IDCR |= (1 << which);
*AT91C_AIC_ICCR |= (1 << which);
}
