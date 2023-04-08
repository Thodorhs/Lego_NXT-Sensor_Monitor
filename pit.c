#include  "pit.h"
#include "AT91SAM7S256.h"
#include "aic.h"
/*#define AT91C_PITC_PIVR ((AT91_REG *) 	0xFFFFFD38) // (PITC) Period Interval Value Register
#define AT91C_PITC_PISR ((AT91_REG *) 	0xFFFFFD34) // (PITC) Period Interval Status Register
#define AT91C_PITC_PIIR ((AT91_REG *) 	0xFFFFFD3C) // (PITC) Period Interval Image Register
#define AT91C_PITC_PIMR ((AT91_REG *) 	0xFFFFFD30) // (PITC) Period Interval Mode Register*/

#define AT91C_PITC_PIMR_DISABLE_MASK 0xFEFFFFFF
#define AT91C_PITC_PIMR_IDISABLE_MASK 0xFDFFFFFF
#define AT91C_PITC_PIMR_ENIEN_MASK 0xFFF00000
#define AT91C_PITC_PIMR_PIV_VALUE_MASK 0x000FFFFF
#define AT91C_PITC_PIMR_CLEAR 0x00000000

void PITMrPivSet(ULONG piv)
{
 //*AT91C_PITC_PIMR &= AT91C_PITC_PIMR_CLEAR;
 piv &= AT91C_PITC_PIMR_PIV_VALUE_MASK;
 *AT91C_PITC_PIMR &= AT91C_PITC_PIMR_ENIEN_MASK;
 *AT91C_PITC_PIMR |= piv;
}

void PITEnable(void)
{
*AT91C_PITC_PIMR |= (1<<24);
}

void PITDisable(void)
{
*AT91C_PITC_PIMR &= AT91C_PITC_PIMR_DISABLE_MASK;
}

ULONG PITRead(void)
{
return *AT91C_PITC_PIIR;
}

ULONG PITReadReset(void)
{
return *AT91C_PITC_PIVR;
}

void PITInterruptEnable(ULONG period, void (*handler)(void))
{
PITMrPivSet(period);
*AT91C_PITC_PIMR |= (1<<25);
AICInit();
AICInterruptEnable(1, handler);
}

void PITInterruptDisable(void)
{
*AT91C_PITC_PIMR &= AT91C_PITC_PIMR_IDISABLE_MASK;
AICInterruptDisable(1);
}

void PITAckInterrupt(void)
{
*(AT91C_AIC_EOICR)=1;
PITReadReset();
}

UWORD PITTicks2ms(ULONG ticks)
{
return ticks/3003;
}

UWORD PITTicks2s(ULONG ticks)
{
return ticks/30030;
}

void spindelayms(ULONG ms)
{
ULONG temp=0, time=0, prev=0;
PITMrPivSet(3003);

while(time<ms)
{
	temp = PITRead();
	temp = (temp >> 20);
	if(prev != temp)
	{
		time++;
	}
	prev = temp;
}
return;
}

