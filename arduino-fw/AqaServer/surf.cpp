#include "surf.h"
#include <Arduino.h>

//===================================================================================
//===================================================================================

void initSurface(){
	pinMode(SWITCH_SUR, INPUT);
}

int getSurface(){
  int buttonState = analogRead(SWITCH_SUR);
  if (buttonState > 0)
    return true;
  else
    return false;
}

