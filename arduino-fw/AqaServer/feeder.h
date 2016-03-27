#ifndef feeder
#define feeder
#define motorPin1  6 /*!< \brief Pin number of step motor */
#define motorPin2  7 /*!< \brief Pin number of step motor */
#define motorPin3  8 /*!< \brief Pin number of step motor */
#define motorPin4  9  /*!< \brief Pin number of step motor */
#define delayTime 10  /*!< \brief Time delay at turning */

/*!
 * \fn initFeed()
 * \brief Step motor initiation 
 */
void initFeed();

/*!
 * \fn turnAround()
 * \brief Turning step motor one time around. 
 */
void turnAround();

#endif
