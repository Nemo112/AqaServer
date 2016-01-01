/*
 * 
 * GZ - get water surface switch 
 * GM - get tank temperature which is suppose to be in water
 * GT - get real temperature in tank
 * GL - get light relay state
 * GF - get filter relay state
 * GH - get heater relay state
 * GN - get time of system
 * GP - get feeder to turn around and give food to fish
 * GW - get wake time for lights
 * GE - get time for shuting down the lights
 * GF - get time for feeding
 * 
 * SE - set time for sleep time in SE hh mm
 * SW - set time for wake up in SW hh mm
 * SF - set time for feeding in SF hh mm
 * ST - set wish temperature in tank in ST nn
 * 
 */
extern "C"{
  #include "relay.h"
  #include "surf.h"
  #include "temp.h"
  #include "timent.h"
  #include "feeder.h"
  #include "timers.h" // obsahovalo EEPROM práci, ale nefunguje správně include
};
#include <EEPROM.h>
#include <Ethernet.h>
#define LIGHTS 3
#define FILTER 4
#define HEATER 5
#define BFS    10

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
//SW hh mm
//SE hh mm
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

void checkTemp(){
  if(tankTmpr > ((int)getTemp())){
    switchOn(HEATER);   
  }else{
    switchOff(HEATER);  
  }
}

byte minute; byte hour;
  
unsigned int loopCounter = 0;

static byte mac[] = { 0xDA, 0xAD, 0x9C, 0xEF, 0xFE, 0xAD };
static byte ip[] = { 10, 0, 0, 173 };
byte gateway[] = { 10, 0, 0, 138 };
byte subnet[] = { 255, 255, 255, 0};

EthernetServer server(9011);

void setupTimes(){
  readDS3231MinutesHoures(&minute,&hour);
  unsigned int upper =  endt.h*60 + endt.m;
  unsigned int lower =  wake.h*60 + wake.m;
  if(lower < (hour*60+minute) && (hour*60+minute) < upper){
    switchOn(LIGHTS);
    switchOn(FILTER);
  }else{
    switchOff(LIGHTS);
    switchOff(FILTER);
  }
  endt.done = true;
  wake.done = true;
}

void setup() {
  Serial.begin(9600);
  //init eeprom constants
  readKonstVal();
  // surface sensors
  pinMode(SWITCH_SUR, INPUT);
  // motor 
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  // Wire for time begin
  Wire.begin();
  // Relays setup
  pinMode(LIGHTS, OUTPUT); 
  pinMode(FILTER, OUTPUT); 
  pinMode(HEATER, OUTPUT); 
  switchSet(LIGHTS,0);
  switchSet(FILTER,0);
  switchSet(HEATER,0);
  // setting up whats needs to be up
  setupTimes();
  // server settings
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.println(Ethernet.localIP());
}
//flags
boolean checktime = true;

void loop() {
  // NTP update
  readDS3231MinutesHoures(&minute,&hour);
  if (minute == 0 && checktime == true){
    setTime();
    checktime = false;
  }else if (minute != 0 && checktime == false){
    checktime = true;  
  }
  // lights sleep & wake
  setupTimes();
  /*if (minute == wake.m && hour == wake.h && wake.done == true){
    switchOn(FILTER);
    switchOn(LIGHTS);
    wake.done = false;
  }else if (minute != wake.m  && wake.done == false){
    wake.done = true;
  }
  if (minute == endt.m && hour == endt.h && endt.done == true){
    switchOff(FILTER);
    switchOff(LIGHTS);
    endt.done = false;
  }else if (minute != endt.m  && endt.done == false){
    endt.done = true;
  }*/
  // feed time
  if (minute == feedt.m && hour == feedt.h && feedt.done == true){
    turnAround();
    feedt.done = false;
  }else if (minute != feedt.m  && feedt.done == false){
    feedt.done = true;
  }
  // check temperature
  checkTemp();
  //
  EthernetClient client = server.available();
  if (client) {
    char combuf[BFS];
    unsigned int msgcnt = 0;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n' || msgcnt == BFS) {
          combuf[msgcnt] = '\0';
          Serial.print("Message: ");
          Serial.println(combuf);
          if(msgcnt >= 2){
            if(combuf[0] == 'G'){ 
              if(combuf[1] == 'Z')
                client.println(getSurface());
              else if(combuf[1] == 'M')
                client.println(tankTmpr);
              else if(combuf[1] == 'T')
                client.println(getTemp());
              else if(combuf[1] == 'L')
                client.println(switchState(LIGHTS));
              else if(combuf[1] == 'F')
                client.println(switchState(FILTER));
              else if(combuf[1] == 'H')
                client.println(switchState(HEATER));
              else if(combuf[1] == 'N')
                displayHttpTime(&client);
              else if(combuf[1] == 'P')
                turnAround();
              else if(combuf[1] == 'W'){
                client.print(wake.h);  
                client.print(":");  
                client.print(wake.m);  
                client.print("\n");  
              }
              else if(combuf[1] == 'E'){
                client.print(endt.h);  
                client.print(":");  
                client.print(endt.m);  
                client.print("\n");  
              }
              else if(combuf[1] == 'F'){
                client.print(feedt.h);  
                client.print(":");  
                client.print(feedt.m);  
                client.print("\n");  
              }
              /*else if(combuf[1] == 'S'){
                client.print(tankTmpr);  
                client.print("\n"); 
              }*/
            }
            else if(combuf[0] == 'S'){
                if(combuf[1] == 'N')
                  setTime();
                else if(combuf[1] == 'T' && msgcnt >= 5)
                  setTempr(combuf);
                else if(msgcnt >= 8)
                  setEETime(combuf);
            }
          }
          client.stop();
          msgcnt = 0;
        }else{    
          combuf[msgcnt] = c;
          msgcnt ++;
        }
        checkTemp();
        setupTimes();
      }
    }
    // give the web browser time to receive the data
    delay(1);
    if (client.available())
      client.stop();
  }
  delay(1000);
  loopCounter ++;
}

