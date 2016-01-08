#include "timent.h"
#include <SPI.h>         
#include <Ethernet.h>
#include <RTClib.h>
#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
#if ARDUINO >= 100
#include <EthernetUdp.h>  // New from IDE 1.0
#else
#include <Udp.h>  
#endif  

byte timeServer[] = {10, 0, 0, 4}; 
unsigned int localPort = 31011;

const int NTP_PACKET_SIZE= 48;
byte pb[NTP_PACKET_SIZE];
EthernetUDP Udp;

//===================================================================================
//===================================================================================

void initTiment(){
  Wire.begin();
}

byte decToBcd(byte val){
    return( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
    return( (val/16*10) + (val%16) );
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year){
    // sets time and date data to DS3231
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set next input to start at the seconds register
    Wire.write(decToBcd(second)); // set seconds
    Wire.write(decToBcd(minute)); // set minutes
    Wire.write(decToBcd(hour)); // set hours
    Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
    Wire.write(decToBcd(month)); // set month
    Wire.write(decToBcd(year)); // set year (0 to 99)
    Wire.endTransmission();
}

void readDS3231MinutesHoures(byte *minute, byte *hour){
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    // request seven bytes of data from DS3231 starting from register 00h
    bcdToDec(Wire.read() & 0x7f); // seconds
    *minute = bcdToDec(Wire.read());
    *hour = bcdToDec(Wire.read() & 0x3f);
}

void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year){
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    // request seven bytes of data from DS3231 starting from register 00h
    *second = bcdToDec(Wire.read() & 0x7f);
    *minute = bcdToDec(Wire.read());
    *hour = bcdToDec(Wire.read() & 0x3f);
    *dayOfWeek = bcdToDec(Wire.read());
    *dayOfMonth = bcdToDec(Wire.read());
    *month = bcdToDec(Wire.read());
    *year = bcdToDec(Wire.read());
}

void displayHttpTime(EthernetClient * client){
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    // retrieve data from DS3231
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    // send it to the serial monitor
    client->print(hour, DEC);
    // convert the byte variable to a decimal number when displayed
    client->print(":");
    if (minute<10){
        client->print("0");
    }
    client->print(minute, DEC);
    client->print(":");
    if (second<10){
        client->print("0");
    }
    client->print(second, DEC);
    client->print(" ");
    client->print(dayOfMonth, DEC);
    client->print("/");
    client->print(month, DEC);
    client->print("/");
    client->print(year, DEC);
    client->print(" Day of week: ");
    switch(dayOfWeek){
        case 1:
            client->println("Sunday");
            break;
        case 2:
            client->println("Monday");
            break;
        case 3:
            client->println("Tuesday");
            break;
        case 4:
            client->println("Wednesday");
            break;
        case 5:
            client->println("Thursday");
            break;
        case 6:
            client->println("Friday");
            break;
        case 7:
            client->println("Saturday");
            break;
    }
}

void displayTime(){
    byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    // retrieve data from DS3231
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    // send it to the serial monitor
    Serial.print(hour, DEC);
    // convert the byte variable to a decimal number when displayed
    Serial.print(":");
    if (minute<10){
        Serial.print("0");
    }
    Serial.print(minute, DEC);
    Serial.print(":");
    if (second<10){
        Serial.print("0");
    }
    Serial.print(second, DEC);
    Serial.print(" ");
    Serial.print(dayOfMonth, DEC);
    Serial.print("/");
    Serial.print(month, DEC);
    Serial.print("/");
    Serial.print(year, DEC);
    Serial.print(" Day of week: ");
    switch(dayOfWeek){
        case 1:
            Serial.println("Sunday");
            break;
        case 2:
            Serial.println("Monday");
            break;
        case 3:
            Serial.println("Tuesday");
            break;
        case 4:
            Serial.println("Wednesday");
            break;
        case 5:
            Serial.println("Thursday");
            break;
        case 6:
            Serial.println("Friday");
            break;
        case 7:
            Serial.println("Saturday");
            break;
    }
}


unsigned long sendNTPpacket(byte *address)
{
  memset(pb, 0, NTP_PACKET_SIZE); 
  pb[0] = 0b11100011;   // LI, Version, Mode
  pb[1] = 0;     // Stratum, or type of clock
  pb[2] = 6;     // Polling Interval
  pb[3] = 0xEC;  // Peer Clock Precision
  pb[12]  = 49; 
  pb[13]  = 0x4E;
  pb[14]  = 49;
  pb[15]  = 52;
#if ARDUINO >= 100
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(pb,NTP_PACKET_SIZE);
  Udp.endPacket(); 
#else
  Udp.sendPacket( pb,NTP_PACKET_SIZE,  address, 123); //NTP requests are to port 123
#endif    
}

boolean setTime(){
  displayTime();
  Udp.begin(localPort);
  sendNTPpacket(timeServer);
  delay(2000);
  int packetSize = Udp.parsePacket();
  if ( Udp.available() ) {
    // read the packet into the buffer
#if ARDUINO >= 100
    Udp.read(pb, NTP_PACKET_SIZE);      // New from IDE 1.0,
#else
    Udp.readPacket(pb, NTP_PACKET_SIZE);
#endif  

    // NTP contains four timestamps with an integer part and a fraction part
    // we only use the integer part here
    unsigned long t1, t2, t3, t4;
    t1 = t2 = t3 = t4 = 0;
    for (int i=0; i< 4; i++)
    {
      t1 = t1 << 8 | pb[16+i];      
      t2 = t2 << 8 | pb[24+i];      
      t3 = t3 << 8 | pb[32+i];      
      t4 = t4 << 8 | pb[40+i];
    }

    float f1,f2,f3,f4;
    f1 = ((long)pb[20] * 256 + pb[21]) / 65536.0;      
    f2 = ((long)pb[28] * 256 + pb[29]) / 65536.0;      
    f3 = ((long)pb[36] * 256 + pb[37]) / 65536.0;      
    f4 = ((long)pb[44] * 256 + pb[45]) / 65536.0;

    const unsigned long seventyYears = 2208988800UL;
    t1 -= seventyYears;
    t2 -= seventyYears;
    t3 -= seventyYears;
    t4 -= seventyYears;
    t4 -= (3 * 3600L);     // Notice the L for long calculations!!
    t4 += 1;               // adjust the delay(1000) at begin of loop!
    if (f4 > 0.4) t4++;    // adjust fractional part, see above
    Serial.println("UDP available ...");
    Serial.println("Asking 1307 ...");
    setDS3231time((byte)DateTime(t4).second(), (byte)DateTime(t4).minute(),(byte)DateTime(t4).hour()+4, (byte)DateTime(t4).dayOfTheWeek()+1, (byte)DateTime(t4).day(), (byte)DateTime(t4).month(), (byte)DateTime(t4).year()-2000);
    Serial.println("1307 after update: ");
    displayTime();
    Serial.println();
    return true;
  }else
  {
    Serial.println("No UDP available ...");
    Serial.println("Asking 1307 ...");
    displayTime();
    return false;
  }
}


