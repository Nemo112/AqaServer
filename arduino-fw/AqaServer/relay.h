#ifndef relay
#define relay
#define RELAY_CNT 3
#define LIGHTS 3
#define FILTER 4
#define HEATER 5

void initRelays();

void switchRelay(int x);

void switchSet(int x, int state);

void switchOn(int x);

void switchOff(int x);

int switchState(int x);

#endif
