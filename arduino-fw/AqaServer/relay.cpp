#include "relay.h"
#include <Arduino.h>

int relays_stats[RELAY_CNT]; /*!< \brief Relay status array */

//===================================================================================
//===================================================================================

void initRelays(){
  pinMode(LIGHTS, OUTPUT); 
  pinMode(FILTER, OUTPUT); 
  pinMode(HEATER, OUTPUT); 
  switchSet(LIGHTS,1);
  switchSet(FILTER,1);
  switchSet(HEATER,0); 
}

void switchRelay(int x){
  relays_stats[x-3] = (relays_stats[x-3] + 1) % 2;
  digitalWrite(x, relays_stats[x]);
}
void switchSet(int x, int state){
  relays_stats[x-3] = state;
  digitalWrite(x, relays_stats[x-3]);
}
void switchOn(int x){
  if(relays_stats[x-3] != 1){
   digitalWrite(x, 1);
   relays_stats[x-3] = 1;
  }
}
void switchOff(int x){
  if(relays_stats[x-3] != 0){
    digitalWrite(x,0);
    relays_stats[x-3] = 0;
  }
}
int switchState(int x){
  return relays_stats[x-3];
}

