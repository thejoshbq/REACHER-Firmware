#ifndef LASER_H
#define LASER_H

#include "Device.h"
#include <Arduino.h>

/**
 * @file Laser.h
 * @brief Defines the Laser class for controlling a laser device.
 * @warning This implementation is in testing and not fully verified.
 */

/**
 * @class Laser
 * @brief A subclass of Device representing a laser for stimulation.
 * 
 * Manages a laser connected to a digital pin, providing control over stimulation
 * timing and state.
 */
class Laser : public Device {
public:
    int32_t stimDuration;  ///< Duration of the laser stimulation (ms).
    int32_t previousStim;  ///< Timestamp of the previous stimulation (ms).
    bool isRunning;        ///< Indicates if the laser is currently running.

    /**
     * @brief Constructor for the Laser class.
     * @param initPin The digital pin to which the laser is connected.
     */
    Laser(byte initPin);

    /**
     * @brief Sets the stimulation duration.
     * @param initDuration Duration in milliseconds.
     */
    void setStimDuration(int32_t initDuration);

    /**
     * @brief Sets the timestamp of the previous stimulation.
     * @param timestamp Time in milliseconds.
     */
    void setPreviousStim(int32_t timestamp);

    /**
     * @brief Sets the running state of the laser.
     * @param initRunning Boolean running state.
     */
    void setIsRunning(bool initRunning);

    /**
     * @brief Turns the laser on.
     */
    void on();

    /**
     * @brief Turns the laser off.
     */
    void off();

    /**
     * @brief Gets the timestamp of the previous stimulation.
     * @return Time in milliseconds.
     */
    int32_t getPreviousStim();

    /**
     * @brief Gets the stimulation duration.
     * @return Duration in milliseconds.
     */
    int32_t getStimDuration();

    /**
     * @brief Checks if the laser is running.
     * @return Boolean running state.
     */
    bool getIsRunning();
};

#endif // LASER_H