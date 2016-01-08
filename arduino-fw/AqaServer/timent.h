#ifndef timent
#define timent
#include <SPI.h>         
#include <Ethernet.h>

void initTiment();

byte decToBcd(byte val);

byte bcdToDec(byte val);

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year);

void readDS3231MinutesHoures(byte *minute, byte *hour);

void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year);

void displayHttpTime(EthernetClient * client);

void displayTime();

unsigned long sendNTPpacket(byte *address);

boolean setTime();

#endif
