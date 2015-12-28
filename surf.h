#ifndef surf
#define surf
#define SWITCH_SUR 4

int getSurface(){
  int buttonState = digitalRead(SWITCH_SUR);
  if (buttonState == HIGH)
    return true;
  else
    return false;
}

#endif
