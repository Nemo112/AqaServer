#ifndef timers
#define timers
#include <Arduino.h>    
#include <Ethernet.h>

struct Tmr{
  byte h;
  byte m;
  byte done;
};

void setTimers();

void readKonstVal();

bool setTempr(char * input);

void feedFish();

void setupTimes(int lights,int filter);

unsigned int getWishTemp();

void checkTempFrMin(int heater);

void checkTemp(int heater);

boolean setEETime(char * input);

void displayHttpWakeTime(EthernetClient * client);

void displayHttpFeedTime(EthernetClient * client);

void displayHttpSleepTime(EthernetClient * client);

#endif
