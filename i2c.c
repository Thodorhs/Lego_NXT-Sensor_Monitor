#include "AT91SAM7S256.h"
#include  <stdconst.h>
#include  <string.h>
#include  <assert.h>
#include  "i2c.h"
#include  "arm2avr.h"
#include  "aic.h"
#include  "display.h"
#include "pit.h"
#include "output.h"

#define BYTES_TO_TX     sizeof(IOTOAVR)
#define BYTES_TO_RX     sizeof(IOFROMAVR)
#define TIMEOUT         2100
#define I2CClk          400000L
#define TIME400KHz      (((OSC/16L)/(I2CClk * 2)) + 1)
#define CLDIV           (((OSC/I2CClk)/2)-3)
#define DEVICE_ADR      0x01
#define COPYRIGHTSTRING "Let's samba nxt arm in arm, (c)LEGO System A/S"
#define COPYRIGHTSTRINGLENGTH 47
const   UBYTE CopyrightStr[] = {"\xCC"COPYRIGHTSTRING};
#define MAX(x,y) (((x)>(y))?(x):(y))
#define piir (*AT91C_PITC_PIIR & AT91C_PITC_CPIV)
#define pimr (*AT91C_PITC_PIMR & AT91C_PITC_CPIV)
#define DISABLEI2cIrqs *AT91C_TWI_IDR = 0x000001C7

#define CR_MSEN_START (AT91C_TWI_MSEN | AT91C_TWI_START)
#define CR_MSEN_STOP (AT91C_TWI_MSEN | AT91C_TWI_STOP)

// send/receive state machine variables
static enum state_t {receive, receiving, send, sending, reset}; 
static enum state_t volatile State;

IOTOAVR volatile IoToAvr;
IOFROMAVR volatile IoFromAvr;

UBYTE tmp[COPYRIGHTSTRINGLENGTH]; //46 + 1 bytes
UBYTE volatile checksum;
UBYTE volatile index_tmp;
UBYTE len_tmp;
UBYTE volatile first;
void DataTxInit(UBYTE *buf, UBYTE len) {
  
  memcpy(tmp, buf, len);
  index_tmp = 0;
  checksum = 0;
  len_tmp = len;

  *AT91C_TWI_THR = tmp[index_tmp];
  checksum += tmp[index_tmp++]; 
  
   *AT91C_TWI_MMR = 0;
  *AT91C_TWI_MMR |= (DEVICE_ADR << 16);  
  
  *AT91C_TWI_IER = 0;
  *AT91C_TWI_IER |= AT91C_TWI_TXCOMP;
  *AT91C_TWI_IER |= AT91C_TWI_NACK;
  *AT91C_TWI_IER |= AT91C_TWI_TXRDY;
  
  *AT91C_TWI_CR = 0;
  *AT91C_TWI_CR = CR_MSEN_START; 

  return;
}  

void DataRxInit(void) {
  checksum = 0;
  index_tmp = 0;
  first = 1;
  len_tmp = BYTES_TO_RX;
  
  *AT91C_TWI_MMR = 0;
  *AT91C_TWI_MMR |= AT91C_TWI_MREAD;
  *AT91C_TWI_MMR |= (DEVICE_ADR << 16);  
  
  *AT91C_TWI_IER = 0;
  *AT91C_TWI_IER |= AT91C_TWI_TXCOMP;
  *AT91C_TWI_IER |= AT91C_TWI_RXRDY;
  
   *AT91C_TWI_CR = 0;
   *AT91C_TWI_CR = CR_MSEN_START;
  
  return;
}
__ramfunc void I2cHandler(void) {
	
	if (AT91C_TWI_NACK & *AT91C_TWI_SR){
		*AT91C_TWI_THR = tmp[index_tmp-1];
		return;
	}
	
	if (State == sending && (AT91C_TWI_TXRDY & *AT91C_TWI_SR || AT91C_TWI_TXCOMP & *AT91C_TWI_SR)){
		
  		if(index_tmp == len_tmp){
  			*AT91C_TWI_THR = ~checksum;
  			*AT91C_TWI_CR |= AT91C_TWI_STOP;
  			index_tmp++;
  		}else if(index_tmp == len_tmp+1 /*&& *AT91C_TWI_SR & AT91C_TWI_TXCOMP*/){			
  			DISABLEI2cIrqs;
  			while(!(*AT91C_TWI_SR & AT91C_TWI_TXCOMP));
  			State = receive;
  		}else{
  			*AT91C_TWI_THR = tmp[index_tmp];
  			checksum += tmp[index_tmp++];
  		}
  		return;
	}
		
	if (State == receiving && (AT91C_TWI_RXRDY & *AT91C_TWI_SR || AT91C_TWI_TXCOMP & *AT91C_TWI_SR)){
		if (first == 1){
			first = *AT91C_TWI_RHR; //drop
			first = 0; //from now on not in first
			return;
		}
		if(index_tmp == len_tmp /*&& *AT91C_TWI_SR & AT91C_TWI_TXCOMP*/){
  			DISABLEI2cIrqs;  
  			while(!(*AT91C_TWI_SR & AT91C_TWI_TXCOMP));			
  		  	if((checksum + *AT91C_TWI_RHR) != 0xFF ){
  		  		DisplayString(0,0,(UBYTE*)"             ");
  		  		DisplayString(0,0,(UBYTE*)"CHECKSUM ERR!");
				DisplayUpdateSync();
  		  	}
  		  	State = send;
			
  		}else if(index_tmp == len_tmp-1){		
			tmp[index_tmp] = *AT91C_TWI_RHR;
			memcpy((UBYTE *)&IoFromAvr, tmp, len_tmp);
			checksum += tmp[index_tmp++];
  			*AT91C_TWI_CR |= AT91C_TWI_STOP;
  		}else{
  			tmp[index_tmp] = *AT91C_TWI_RHR;
  			checksum += tmp[index_tmp++];
  		}
  		return;
       	}
       	
  State = reset;
  return;
}

/*void TimeOutHandler(void) {
  State = _return;
}*/

void I2CTransfer(void) {
  spindelayms(2);
  switch ( State ){
  	case reset:
  		State = sending;
  		DataTxInit((UBYTE *)CopyrightStr, (UBYTE) COPYRIGHTSTRINGLENGTH);
  		break;
  	case sending:
  		break;
  	case receiving:
  		break;
  	case receive:
  		State = receiving;
  		DataRxInit();
  		break;
  	case send:
  		State = sending;
  		DataTxInit((UBYTE *)&IoToAvr, (UBYTE)sizeof(IoToAvr));
  		break;
  	default:
  		break;
  }
  return;
}

void I2CCtrl (enum power_t p) {
	switch (p) {
	  	case POWERDOWN:
	  		IoToAvr.Power = 0x5A;
      			IoToAvr.PwmFreq = 0x00;
	       		break;
	       	case REPROGRAM:
	       		IoToAvr.Power = 0xA5;
      			IoToAvr.PwmFreq = 0x5A;
	       		break;
		default:
			break;
	}
  OutputExit();
  return;
}

#define WAITClk(t) {\
	  ULONG pit = piir + (t);\
          if (pit >= pimr) pit -= pimr;\
          while (piir < pit){;}\
        }

void I2CInit(void) { 
  //
  // disable I2C on PIO
  // this is called also during an error, so interrupts etc may be enabled
  //
  State = reset;
  *AT91C_AIC_IDCR = (1L<<AT91C_ID_TWI);			/* disable AIC irq  */
  DISABLEI2cIrqs;                      			/* disable TWI irq  */
  *AT91C_PMC_PCER  = (1L<<AT91C_ID_TWI);		/* enable TWI Clock */
  *AT91C_PIOA_OER  = AT91C_PA4_TWCK;  		  	/* SCL is output    */
  *AT91C_PIOA_ODR  = AT91C_PA3_TWD;			/* SDA is input     */
  *AT91C_PIOA_MDER = (AT91C_PA4_TWCK | AT91C_PA3_TWD);  /* open drain       */
  *AT91C_PIOA_PPUDR = (AT91C_PA4_TWCK | AT91C_PA3_TWD); /* no pull up       */
  // 
  // synch I2C clocks using PIO
  // generate a 400KHz pulse on SCK and wait until both SCK and SDA are high, 
  // which means the slave ticks with this clock
  //
  *AT91C_PIOA_PER  = (AT91C_PA4_TWCK | AT91C_PA3_TWD); /* enable PIO control for these pins */
  while(((*AT91C_PIOA_PDSR & AT91C_PA3_TWD) == 0) || ((*AT91C_PIOA_PDSR & AT91C_PA4_TWCK) == 0)){
      *AT91C_PIOA_CODR = AT91C_PA4_TWCK; /* drive SCL Low  */
      WAITClk(TIME400KHz);
      *AT91C_PIOA_SODR = AT91C_PA4_TWCK; /* drive SCL High */
      WAITClk(TIME400KHz);
  }
  // 
  // init I2C on PIO
  //
  *AT91C_TWI_CR    =  AT91C_TWI_SWRST;			/* this has to happen before the rest */
  *AT91C_PIOA_PDR   = (AT91C_PA4_TWCK | AT91C_PA3_TWD); /* disable PIO control for these pins */
  *AT91C_PIOA_ASR   = (AT91C_PA4_TWCK | AT91C_PA3_TWD); /* select peripheral A = TWI */
  *AT91C_TWI_CWGR   = (CLDIV | (CLDIV << 8));           /* 400KHz clock    */
  *AT91C_AIC_ICCR   = (1L<<AT91C_ID_TWI);               /* clear AIC irq   */
  AT91C_AIC_SVR[AT91C_ID_TWI] = (unsigned int)I2cHandler;
  AT91C_AIC_SMR[AT91C_ID_TWI] = ((AT91C_AIC_PRIOR_HIGHEST) | (AT91C_AIC_SRCTYPE_INT_EDGE_TRIGGERED));
  *AT91C_AIC_IECR   = (1L<<AT91C_ID_TWI);               /* Enables AIC irq */

  IoToAvr.Power     = 0;

  return;
}

void I2CExit(void) {
  DISABLEI2cIrqs;
  *AT91C_AIC_IDCR   = (1L<<AT91C_ID_TWI);               /* Disable AIC irq  */
  *AT91C_AIC_ICCR   = (1L<<AT91C_ID_TWI);               /* Clear AIC irq    */
  *AT91C_PMC_PCDR   = (1L<<AT91C_ID_TWI);               /* Disable clock    */
  *AT91C_PIOA_MDER  = (AT91C_PA4_TWCK | AT91C_PA3_TWD); /* Open drain       */
  *AT91C_PIOA_PPUDR = (AT91C_PA4_TWCK | AT91C_PA3_TWD); /* no pull up       */
  *AT91C_PIOA_PER   = (AT91C_PA4_TWCK | AT91C_PA3_TWD); /* Disable peripheal*/
}
