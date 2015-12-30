#ifndef timers
#define timers

struct Tmr{
  byte h;
  byte m;
  byte done;
};

Tmr wake;
Tmr endt;
Tmr feedt;
int tankTmpr;

#endif
