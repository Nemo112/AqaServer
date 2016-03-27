#ifndef timent
#define timent
#include <SPI.h>         
#include <Ethernet.h>

/*!
 * \fn initTiment()
 * \brief Init of RTC
 */
void initTiment();

/*!
 * \fn decToBcd(byte val)
 * \param val Decadic number
 * \brief Decadic format to binary coded decimal
 * \return Binary coded decimal value.
 */
byte decToBcd(byte val);

/*!
 * \fn bcdToDec(byte val)
 * \param val Binary coded number.
 * \brief Binary coded decimal to decadic format. 
 * \return Decimal value.
 */
byte bcdToDec(byte val);

/*!
 * \fn setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
 * \param second Second of time.
 * \param minute Minute of time.
 * \param hour Hours of time
 * \param dayOfWeek Day of the week.
 * \param dayOfMonth Day of the month.
 * \param month Current month.
 * \param year Current year.
 * \brief Setter for RTC 
 */
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year);

/*!
 * \fn readDS3231MinutesHoures(byte minute, byte hour)
 * \param minute Minute of time.
 * \param hour Hours of time
 * \brief Getter of hours and minutes
 */
void readDS3231MinutesHoures(byte *minute, byte *hour);

/*!
 * \fn readDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
 * \param second Second of time.
 * \param minute Minute of time.
 * \param hour Hours of time
 * \param dayOfWeek Day of the week.
 * \param dayOfMonth Day of the month.
 * \param month Current month.
 * \param year Current year.
 * \brief Reader of RTC values. 
 */
void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year);

/*!
 * \fn displayHttpTime(EthernetClient * client)
 * \param client Ethernet client to send the message to.
 * \brief Response for ethernet connection for time values. 
 */
void displayHttpTime(EthernetClient * client);

/*!
 * \fn displayTime()
 * \brief Displays time on the console. 
 */
void displayTime();

/*!
 * \fn sendNTPpacket(byte *address)
 * \param address Address of NTP server.
 * \brief Sender of NTP request to NTP server given by address.
 * \return NTP time stamp.
 */
unsigned long sendNTPpacket(byte *address);

/*!
 * \fn setTime()
 * \brief Set time by NTP server to RTC.
 * \return Status of time setting.
 */
boolean setTime();

#endif
