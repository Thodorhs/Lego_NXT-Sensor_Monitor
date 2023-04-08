#include "output.h"
#include "arm2avr.h"
#include <stdconst.h>

IOTOAVR volatile IoToAvr;

void OutputInit(void) {
    
    UBYTE i;
    IoToAvr.Power = 0;
    IoToAvr.PwmFreq = 0x8;
    
    for(i=0; i<4; i++)
        IoToAvr.PwmValue[i] = 0;
    
    IoToAvr.OutputMode = 0;
    
    return;
}

void OutputExit(void) {
     UBYTE i;
     for(i=0; i<4; i++)
          IoToAvr.PwmValue[i] = 0;
    return;
}

void OutputSetSpeed (UBYTE MotorNr, SBYTE Speed){
    
    if(MotorNr > 3)
        return;
    if(Speed > MAX_SPEED_FW || Speed < MAX_SPEED_RW)
        return;
        
    IoToAvr.OutputMode = 1;
    IoToAvr.PwmValue[MotorNr] = Speed;
    
    return;
}

SBYTE OutputGetSpeed(UBYTE MotorNr){
   if(MotorNr > 3)
   	return 0;
   return IoToAvr.PwmValue[MotorNr];
}
