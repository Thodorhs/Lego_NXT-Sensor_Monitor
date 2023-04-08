#include "output.h"
#include "arm2avr.h"
#include "button.h"
#include <stdconst.h>

IOFROMAVR volatile IoFromAvr;

void   ButtonInit(void) {
  IoFromAvr.Buttons = 0;
  return;
}

void   ButtonExit(void) {
  return;
}

UWORD   GetButtonValue(void) {
  return IoFromAvr.Buttons;
}

enum button_t ButtonRead(void) {

  UWORD value = GetButtonValue();

  	if (value == 2047) {
	  	return BUTTON_ENTER;
	}
  	if (value >= 407 && value <= 1023) {  	
  		return BUTTON_EXIT;
  	}
	if (value >= 200 && value <= 406) {
	 	return BUTTON_RIGHT;
	}
  	if (value >= 1 && value <= 127) {
  		return BUTTON_LEFT;
	}
  return BUTTON_NONE;
}
