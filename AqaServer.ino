extern "C"{
  #include "relay.h"
  #include "surf.h"
  #include "temp.h"
  #include "timent.h"
  #include "feeder.h"
};
#include <Ethernet.h>
#define LIGHTS 1
#define FILTER 5
#define HEATER 3
#define FAN    4

unsigned int loopCounter = 0;

static byte mac[] = { 0xDA, 0xAD, 0x9C, 0xEF, 0xFE, 0xAD };
static byte ip[] = { 10, 0, 0, 173 };
byte gateway[] = { 10, 0, 0, 138 };
byte subnet[] = { 255, 255, 255, 0};

EthernetServer server(9010);

void setup() {
  // surface sensors
  pinMode(SWITCH_SUR, INPUT);
  // motor 
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  // Wire for time begin
  Wire.begin();
  /*
  // Relays setup
  pinMode(LIGHTS, OUTPUT); 
  pinMode(FILTER, OUTPUT); 
  pinMode(HEATER, OUTPUT); 
  pinMode(FAN, OUTPUT);
  */
  // server settings
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);
  Serial.println(Ethernet.localIP());

}


void loop() {
  //turnAround();
  /*if (loopCounter % 60*60*1000 == 0){
    setTime();
  }*/
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n' && currentLineIsBlank) {
            // Server output
            server.println("Yep");
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
  //displayTime();
  Serial.println(getSurface());
  delay(1000);
  loopCounter ++;
}

