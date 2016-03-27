#ifndef surf
#define surf
#define SWITCH_SUR A0 /*!< \brief Surface switch input pin. */

/*!
 * \fn initSurface()
 * \brief Relay initiation. 
 */
void initSurface();
/*!
 * \fn getSurface()
 * \brief Relay status getter. 
 * \return The state of switch.
 */
int getSurface();


#endif
