#include "input.h"
#include "arm2avr.h"
#include <stdconst.h>

#define FALSE 0
#define TRUE 1

IOFROMAVR volatile IoFromAvr;

//port 1 on brick :: button
//port 2 on brick :: microphone
//port 3 on brick :: reserved
//port 4 on brick :: reserved

void InputInit(void) {
  UBYTE i;
  IoFromAvr.Buttons=0;
  IoFromAvr.Battery=0;
  
  for (i=0;i<NOS_OF_AVR_INPUTS;i++)
  	IoFromAvr.AdValue[i]=0;
  return;
}

void InputExit(void) {
  UBYTE i;
  IoFromAvr.Buttons=0;
  IoFromAvr.Battery=0;
  
  for (i=0;i<NOS_OF_AVR_INPUTS;i++)
  	IoFromAvr.AdValue[i]=0;
  return;
}

void InputGetSensorValue(UWORD *value, UBYTE port){
if (port == 0 && IoFromAvr.AdValue[port] == 1023){
  *value = FALSE;
}else if(port == 0 && IoFromAvr.AdValue[port] == 183){
  *value = TRUE; 
}else{
  *value = IoFromAvr.AdValue[port];
}
  return;
}
