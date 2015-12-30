#ifndef surf
#define surf
#define SWITCH_SUR A0

int getSurface(){
  int buttonState = analogRead(SWITCH_SUR);
  if (buttonState > 0)
    return true;
  else
    return false;
}

#endif
