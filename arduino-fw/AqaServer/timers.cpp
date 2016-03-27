#include "timers.h"
#include <Arduino.h>
#include <EEPROM.h>
#include "timent.h"
#include "feeder.h"
#include "temp.h"
#include "relay.h"

Tmr wake; /*!< \brief Aquarium wake time. */
Tmr endt; /*!< \brief Aquarium sleep time. */
Tmr feedt; /*!< \brief Aquarium feed time. */
int tankTmpr;  /*!< \brief Aquarium wish temperature. */

byte minute;  /*!< \brief Current aquarium minute time. */
byte hour; /*!< \brief Current aquarium minute time. */

boolean checktemp = true;

//===================================================================================
//===================================================================================

void setTimers(){
  readDS3231MinutesHoures(&minute,&hour);   
}

void checkTempFrMin(int heater){
  if(minute % 4 == 0 && checktemp){
    checkTemp(heater);
    checktemp = false;
  }else if (minute % 4 != 0 && !checktemp){
    checktemp = true;  
  }  
}

void feedFish(){
if (minute == feedt.m && hour == feedt.h && feedt.done == true){
    turnAround();
    feedt.done = false;
  }else if (minute != feedt.m  && feedt.done == false){
    feedt.done = true;
  }
}

void readKonstVal(){
  tankTmpr =  ((EEPROM.read(1)<<0) & 0xFF) + ((EEPROM.read(0)<<8) & 0xFF);
  wake.h = EEPROM.read(2) & 0xFF;
  wake.m = EEPROM.read(3) & 0xFF;
  endt.h = EEPROM.read(4) & 0xFF;
  endt.m = EEPROM.read(5) & 0xFF;
  feedt.h = EEPROM.read(6) & 0xFF;
  feedt.m = EEPROM.read(7) & 0xFF;
  Serial.print("Tank wish temperature: ");
  Serial.print(tankTmpr);
  Serial.print("\n");
  Serial.print("Tank feed time: ");
  Serial.print(feedt.h);
  Serial.print(":");
  Serial.print(feedt.m);
  Serial.print("\n");
  Serial.print("Tank wake time: ");
  Serial.print(wake.h);
  Serial.print(":");
  Serial.print(wake.m);
  Serial.print("\n");
  Serial.print("Tank sleep time: ");
  Serial.print(endt.h);
  Serial.print(":");
  Serial.print(endt.m);
  Serial.print("\n");
}

bool setTempr(char * input){
  int tempr = (input[3] - '0')*10 + (input[4] - '0');
  EEPROM.write(1,(byte)(tempr & 0xFF));
  EEPROM.write(0,(byte)((8>>tempr) & 0xFF));
  tankTmpr = tempr;
}

void checkTemp(int heater){
  if(tankTmpr > ((int)getTemp())){
    switchOn(heater);
  }else{
    switchOff(heater);  
  }
}

boolean setEETime(char * input){
  int hh = (input[3] - '0')*10 + (input[4] - '0');
  int mm = (input[6] - '0')*10 + (input[7] - '0');
  if(input[1] == 'W'){
    if((60*hh+mm) > (60*endt.h+endt.m)){
      return false;  
    }else{
      EEPROM.write(2,hh);
      EEPROM.write(3,mm);
      wake.h = hh;
      wake.m = mm;
      return true;  
    }
  }else if(input[1] == 'E'){
    if((60*hh+mm) < (60*wake.h+wake.m)){
      return false;
    }else{
      EEPROM.write(4,hh);
      EEPROM.write(5,mm);
      endt.h = hh;
      endt.m = mm;
      return true;
    }
  }else if(input[1] == 'F'){
    EEPROM.write(6,hh);
    EEPROM.write(7,mm);
    feedt.h = hh;
    feedt.m = mm;
    return true;
  }
}

unsigned int getWishTemp(){
  return tankTmpr;  
}

void setupTimes(int lights,int filter){
  unsigned int upper =  endt.h*60 + endt.m;
  unsigned int lower =  wake.h*60 + wake.m;
  if(lower < (hour*60+minute) && (hour*60+minute) < upper){
    switchOn(lights);
    switchOn(filter);
  }else{
    switchOff(lights);
    switchOff(filter);
  }
  endt.done = true;
  wake.done = true;
}

void displayHttpWakeTime(EthernetClient * client){
  client->print(wake.h); 
  client->print(":");  
  client->print(wake.m);
  client->print("\n"); 
}

void displayHttpSleepTime(EthernetClient * client){
  client->print(endt.h); 
  client->print(":");  
  client->print(endt.m);
  client->print("\n"); 
}

void displayHttpFeedTime(EthernetClient * client){
  client->print(feedt.h); 
  client->print(":");  
  client->print(feedt.m);
  client->print("\n"); 
}



