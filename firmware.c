#include <stdlib.h>
#include <assert.h>
#include "AT91SAM7S256.h"
#include "hwinit.h"
#include "pit.h"
#include "aic.h"
#include "display.h"
#include "sound.h"
#include "i2c.h"
#include "input.h"
#include "button.h"
#include "output.h"
#include "led.h"
#include "aclock.h"

#define NO_MOTORS 1
#define INPUT_PORTS 3

#define FRIENDLY_VALUES_ON 1

//port 1 on brick :: button
//port 2 on brick :: microphone
//port 3 on brick :: lightsensor
//port 4 on brick :: reserved

void PrintSpeed(void) {
	UBYTE i;
	DisplayString(5,32,(UBYTE*)"v-MotorSpeeds-v");
	DisplayString(0,40,(UBYTE*)"A:");
	DisplayString(0,48,(UBYTE*)"B:");
	DisplayString(0,56,(UBYTE*)"C:");
	UBYTE tmp;
	UBYTE offset =0;
	for (i = 0; i < NO_MOTORS; i++) {
		tmp=OutputGetSpeed(i);
		DisplayString(18,40+offset,(UBYTE*)"      ");
		if(tmp <= 100){
		  DisplayNum(18,40+offset,tmp);
		}else {
		  DisplayString(18,40+offset,(UBYTE*)"NegVal");
		}
		offset += 8;	
	}
return;
}

void stop(void) {
	UBYTE i;
	for (i = 0; i < NO_MOTORS; i++) {
		OutputSetSpeed(i, 0);	
	}
return;
}

void move(SWORD value) {
	OutputSetSpeed(0, value);
return;
}

void PrintSensorValues(void) {
	UBYTE i;
	UBYTE friendly_val=0;
	UWORD value;
	DisplayString(0,0,(UBYTE*)"Button:");
	DisplayString(0,8,(UBYTE*)"Microphone:");
	DisplayString(0,16,(UBYTE*)"Lsensor:");
	DisplayString(0,24,(UBYTE*)"Reserved:");
	
	for (i = 0; i < INPUT_PORTS; i++) {
		InputGetSensorValue(&value, i);
		if(value == 1 && i == 0){
		DisplayString(67,i*8,(UBYTE*) "     ");
		  LedSwitchOn(2);
		  DisplayString(67,i*8,(UBYTE*) "TRUE");
		  move(100);
		}else if(value == 0 && i == 0){
		  DisplayString(67,i*8,(UBYTE*) "FALSE");
		  stop();
		}else if(i == 1){
			if(FRIENDLY_VALUES_ON){
			  friendly_val=100 - (((float)value / (float)100)*10);
			  if(friendly_val > 80) {
			  	  LedSwitchOn(2);
			  	}
			  DisplayString(67,i*8,(UBYTE*) "     ");
		  	  DisplayNum(67,i*8,friendly_val);			
		  	  DisplayString(86,i*8,(UBYTE*) "%");
			}else{	
		  	  DisplayNum(67,i*8,value);
			}
		}else if (i == 2) {
			if(FRIENDLY_VALUES_ON){
			  friendly_val=100 - (((float)value / (float)86)*10);
			  DisplayString(67,i*8,(UBYTE*) "     ");
			  	if(friendly_val > 50) {
			  	  move(-100);
			  	}
			  DisplayNum(67,i*8,friendly_val);
			  DisplayString(86,i*8,(UBYTE*) "%");
			}else{
		    	  DisplayNum(67,i*8,value);
			}
		}
	}
return;
}

void ButtonsCheck(void) {
	DisplayString(58,47,(UBYTE*)"button:");
	DisplayString(66,55,(UBYTE*)"      ");
	switch (ButtonRead()){
		case BUTTON_ENTER:
			DisplayString(66,55,(UBYTE*)"ENTER");
			LedSwitchOn(2);
			I2CCtrl (REPROGRAM);
			break;
		case BUTTON_EXIT:
			DisplayString(66,55,(UBYTE*)"EXIT");
			LedSwitchOn(2);
			I2CCtrl (POWERDOWN);
			break;
		case BUTTON_LEFT:
			DisplayString(66,55,(UBYTE*)"LEFT");
			LedSwitchOn(2);
			break;
		case BUTTON_RIGHT:
			DisplayString(66,55,(UBYTE*)"RIGHT");
			LedSwitchOn(2);
			break;
		case BUTTON_NONE:
			DisplayString(66,55,(UBYTE*)"NONE");
			LedSwitchOff(2);
			break;
		default:
			break;
  	}
return;  
}
void PrintButtons (void) {
	return;
}
int main(void) {
  LedSwitchOn(2);
  HardwareInit(); // need this to init PIOA clock
  DisplayInit(); 
  PITEnable();
  AICInit();
  SoundInit();
  I2CInit();
  
  InputInit();
  OutputInit();
  
  DisplayString(34,16, (UBYTE*)"Hello");
  DisplayString(26,25, (UBYTE*)"Stranger");
  DisplayString(44,34, (UBYTE*)":)");
  
  DisplayUpdateSync();
  spindelayms(3500);
  
  DisplayString(34,16, (UBYTE*)"      ");
  DisplayString(26,25, (UBYTE*)"          ");
  DisplayString(44,34, (UBYTE*)"  ");
  
  DisplayUpdateSync();
  while(1) {
    I2CTransfer();
    ButtonsCheck();
    PrintSpeed();
    PrintSensorValues();
 
    //DisplayString(0,0, (UBYTE*)"    ");
    //DisplayNum(0,0,GetButtonValue());
    DisplayUpdateSync();
  }

  ButtonExit();
  InputExit();
  I2CExit();
  PITInterruptDisable();
  PITDisable();
  SoundExit();
  DisplayExit();

  return 0;
}
