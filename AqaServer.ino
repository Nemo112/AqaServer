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

unsigned int loopCounter = 0;

static byte mac[] = { 0xDA, 0xAD, 0x9C, 0xEF, 0xFE, 0xAD };
static byte ip[] = { 10, 0, 0, 173 };
byte gateway[] = { 10, 0, 0, 138 };
byte subnet[] = { 255, 255, 255, 0};

EthernetServer server(9011);

void setup() {
  Serial.begin(9600);
  //init eeprom constants
  readKonstVal();
  // surface sensors
  //pinMode(SWITCH_SUR, INPUT_PULLUP);
  // motor 
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  // Wire for time begin
  Wire.begin();
  // Relays setup
  pinMode(LIGHTS, OUTPUT); 
  switchOff(LIGHTS);
  pinMode(FILTER, OUTPUT); 
  switchOff(FILTER);
  pinMode(HEATER, OUTPUT); 
  switchOff(HEATER);
  // server settings
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.println(Ethernet.localIP());
}


void loop() {
  /*if (loopCounter % 60*60*1000 == 0 && loopCounter != 0){
    setTime();
  }*/
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
              if(combuf[1] == 'N')
                client.println(getTemp());
              if(combuf[1] == 'T')
                client.println(getTemp());
              if(combuf[1] == 'L')
                client.println(switchState(LIGHTS));
              if(combuf[1] == 'F')
                client.println(switchState(FILTER));
              if(combuf[1] == 'H')
                client.println(switchState(HEATER));
              if(combuf[1] == 'N')
                displayHttpTime(&client);
              if(combuf[1] == 'P')
                turnAround();
              if(combuf[1] == 'W'){
                client.print(wake.h);  
                client.print(":");  
                client.print(wake.m);  
                client.print("\n");  
              }
              if(combuf[1] == 'E'){
                client.print(endt.h);  
                client.print(":");  
                client.print(endt.m);  
                client.print("\n");  
              }
              if(combuf[1] == 'F'){
                client.print(feedt.h);  
                client.print(":");  
                client.print(feedt.m);  
                client.print("\n");  
              }
              if(combuf[1] == 'S'){
                client.print(tankTmpr);  
                client.print("\n"); 
              }
            }
            if(combuf[0] == 'S'){
                if(combuf[1] == 'N')
                  setTime();
                else if(combuf[1] == 'T' && msgcnt >= 5)
                  setTempr(combuf);
                else if(msgcnt >= 8)
                  setEETime(combuf);
            }
          }
          msgcnt = 0;
        }else{    
          combuf[msgcnt] = c;
          msgcnt ++;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
  delay(1000);
  loopCounter ++;
}

