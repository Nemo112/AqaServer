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
 * GK - get time for feeding
 * 
 * SE - set time for sleep time in SE hh mm
 * SW - set time for wake up in SW hh mm
 * SF - set time for feeding in SF hh mm
 * ST - set wish temperature in tank in ST nn
 * 
 */
//extern "C"{
  #include "relay.h"
  #include "surf.h"
  #include "temp.h"
  #include "timent.h"
  #include "feeder.h"
  #include "timers.h"
//};
#include <EEPROM.h>
#include <Ethernet.h>
#define BFS    10 /*!< \brief Size of input buffer */

//SW hh mm
//SE hh mm

  
static byte mac[] = { 0xDA, 0xAD, 0x9C, 0xEF, 0xFE, 0xAD };  /*!< \brief Mac adress of Arduino */
static byte ip[] = { 10, 0, 0, 173 }; /*!< \brief IP adress of Arduino */
byte gateway[] = { 10, 0, 0, 138 }; /*!< \brief IP adress of gateway */
byte subnet[] = { 255, 255, 255, 0}; /*!< \brief Network mask adress of Arduino */

EthernetServer server(9011); /*!< \brief Server instance for port 9011 */

/*!
 * \fn setup()
 * \brief Setting up all the inputs/outputs 
 */
void setup() {
  Serial.begin(9600);
  //init eeprom constants
  readKonstVal();
  // surface sensors
  initSurface();
  // motor 
  initFeed();
  // Wire for time begin
  initTiment();
  // Relays setup
  initRelays();
  // setting up whats needs to be up
  setupTimes(LIGHTS,FILTER);
  // server settings
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.println(Ethernet.localIP());
}


/*!
 * \fn loop()
 * \brief Looping the Arduino 
 */
void loop() {
  setTimers();
  // lights sleep & wake
  setupTimes(LIGHTS,FILTER);
  // feed time
  feedFish();
  // check temperature
  checkTempFrMin(HEATER);
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
                client.println(getWishTemp());
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
                displayHttpWakeTime(&client);
              }
              else if(combuf[1] == 'E'){
                displayHttpSleepTime(&client);
              }
              else if(combuf[1] == 'K'){
                displayHttpFeedTime(&client);
              }
            }
            else if(combuf[0] == 'S'){
                if(combuf[1] == 'N'){
                  setTime();
                  displayHttpTime(&client);
                }else if(combuf[1] == 'T' && msgcnt >= 5)
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
      }
    }
    delay(100);
    if (client.available())
      client.stop();
  }
  delay(1000);
}

