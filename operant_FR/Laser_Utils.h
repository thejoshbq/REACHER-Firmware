#ifndef LASER_UTILS_H
#define LASER_UTILS_H

#include <Arduino.h>
#include "Laser.h"

/**
 * @file LaserUtils.h
 * @brief Utility functions for managing and logging laser stimulation.
 * 
 * Provides functions to control a laser’s on/off state, log stimulation events,
 * and manage timing-based stimulation behavior.
 */

/**
 * @brief Controls the laser’s on/off state based on stimulation settings.
 * 
 * @param laser Reference to the Laser object to manage.
 */
void manageLaser(Laser& laser);

/**
 * @brief Logs the laser’s stimulation period to the serial monitor.
 * 
 * @param laser Reference to the Laser object to log.
 */
void logStim(Laser& laser);

/**
 * @brief Checks if the current time is within the stimulation period.
 * 
 * @param currentMillis Current time in milliseconds.
 * @return Boolean indicating if the time is within the stimulation period.
 */
bool inStimPeriod(uint32_t currentMillis);

/**
 * @brief Manages the laser’s stimulation behavior.
 * 
 * @param laser Reference to the Laser object to stimulate.
 * @param currentMillis Current time in milliseconds.
 */
void stim(Laser& laser, uint32_t currentMillis);

/**
 * @brief Oversees the laser’s stimulation mode and timing.
 * 
 * @param laser Reference to the Laser object to manage.
 */
void manageStim(Laser& laser);

#endif // LASER_UTILS_H