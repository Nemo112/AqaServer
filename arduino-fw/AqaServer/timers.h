#ifndef timers
#define timers
#include <Arduino.h>    
#include <Ethernet.h>

/*! \class Tmr
 * \brief Timer class for containg timers like wake time or sleep time.
 */
struct Tmr{
  byte h; /*!< \brief Hour of timer. */
  byte m; /*!< \brief Minute of timer. */
  byte done; /*!< \brief Flag if time was set */
};

/*!
 * \fn setTimers()
 * \brief Read RTC minute & hour values.
 */
void setTimers();

/*!
 * \fn readKonstVal()
 * \brief Read EEPROM values and set time time values for aqarium.
 */
void readKonstVal();

/*!
 * \fn setTempr(char * input)
 * \param input Input of readed chars
 * \brief Set wish temperature of aquarium.
 * \return Statues of written data.
 */
bool setTempr(char * input);

/*!
 * \fn feedFish()
 * \brief Feeds fish in aquarium.
 */
void feedFish();

/*!
 * \fn setupTimes(int lights, int filter)
 * \param lights Lights pin value.
 * \param filter Filter pin value.
 * \brief Seting up pins by current time.
 */
void setupTimes(int lights, int filter);

/*!
 * \fn getWishTemp()
 * \return Wish temperature.
 * \brief Getter for wish temperature in tank.
 */
unsigned int getWishTemp();

/*!
 * \fn checkTempFrMin(int heater)
 * \param heater Heater pin number.
 * \brief Check temperature in intervals.
 */
void checkTempFrMin(int heater);

/*!
 * \fn checkTemp(int heater)
 * \param heater Heater pin number.
 * \brief Check temperature.
 */
void checkTemp(int heater);

/*!
 * \fn setEETime(char * input)
 * \param input Time to store in EEPROM.
 * \brief Setup of time in EEPROM.
 * \return Statues of stored values.
 */
boolean setEETime(char * input);

/*!
 * \fn displayHttpWakeTime(EthernetClient * client)
 * \param client Ethernet client to send the message to.
 * \brief Response for ethernet connection for wake time values. 
 */
void displayHttpWakeTime(EthernetClient * client);

/*!
 * \fn displayHttpFeedTime(EthernetClient * client)
 * \param client Ethernet client to send the message to.
 * \brief Response for ethernet connection for feep time values. 
 */
void displayHttpFeedTime(EthernetClient * client);

/*!
 * \fn displayHttpSleepTime(EthernetClient * client)
 * \param client Ethernet client to send the message to.
 * \brief Response for ethernet connection for sleep time values. 
 */
void displayHttpSleepTime(EthernetClient * client);

#endif
