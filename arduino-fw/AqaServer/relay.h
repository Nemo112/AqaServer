#ifndef relay
#define relay
#define RELAY_CNT 3 /*!< \brief Number of relays. */
#define LIGHTS 3 /*!< \brief Light pin relay. */
#define FILTER 4 /*!< \brief Filter pin relay. */
#define HEATER 5 /*!< \brief Heater pin relay. */

/*!
 * \fn initRelays()
 * \brief Relay initiation. 
 */
void initRelays();

/*!
 * \fn switchRelay(int x)
 * \brief Switch relay given by x to different state
 * \param x Number of relay
 */
void switchRelay(int x);

/*!
 * \fn switchSet(int x, int state)
 * \brief Set the realy state by given x.
 * \param x Number of relay.
 * \param state New state of relay.
 */
void switchSet(int x, int state);

/*!
 * \fn switchOn(int x)
 * \brief Switch off relay state by the given x.
 * \param x Number of relay.
 */
void switchOn(int x);

/*!
 * \fn switchOff(int x)
 * \brief Switch off relay state by the given x.
 * \param x Number of relay.
 */
void switchOff(int x);

/*!
 * \fn switchState(int x)
 * \brief Getter for relay state by the given x.
 * \param x Number of relay.
 * \return State of relay.
 */
int switchState(int x);

#endif
