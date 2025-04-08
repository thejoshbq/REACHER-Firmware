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
 * timing and trigger modes.
 */
class Laser : public Device {
public:
    uint32_t onDuration;   ///< Duration the laser is on during stimulation (ms).
    uint32_t offDuration;  ///< Duration the laser is off after stimulation (ms).
    uint32_t onTimestamp;  ///< Timestamp when the laser turns on (ms).
    uint32_t offTimestamp; ///< Timestamp when the laser turns off (ms).
    String trigger;        ///< Trigger mode for the laser (e.g., "ON-PRESS").

    /**
     * @brief Constructor for the Laser class.
     * @param initPin The digital pin to which the laser is connected.
     */
    Laser(byte initPin);

    /**
     * @brief Sets the stimulation start and end times.
     * @param initTimestamp Start time in milliseconds.
     */
    void setStim(int32_t initTimestamp);

    /**
     * @brief Sets the trigger mode for the laser.
     * @param initTrigger Trigger mode string.
     */
    void setTrigger(String initTrigger);

    /**
     * @brief Sets the stimulation period with a trace interval.
     * @param initTimestamp Base time in milliseconds.
     * @param traceInterval Delay in milliseconds before stimulation.
     */
    void setStimPeriod(int32_t initTimestamp, int32_t traceInterval);

    /**
     * @brief Sets the on duration for stimulation.
     * @param initDuration Duration in milliseconds.
     */
    void setStimOnDuration(int32_t initDuration);

    /**
     * @brief Sets the off duration after stimulation.
     * @param initDuration Duration in milliseconds.
     */
    void setStimOffDuration(int32_t initDuration);

    /**
     * @brief Turns the laser on.
     */
    void on();

    /**
     * @brief Turns the laser off.
     */
    void off();

    /**
     * @brief Gets the stimulation start timestamp.
     * @return Start time in milliseconds.
     */
    int32_t getStimOnTimestamp();

    /**
     * @brief Gets the stimulation end timestamp.
     * @return End time in milliseconds.
     */
    int32_t getStimOffTimestamp();

    /**
     * @brief Gets the stimulation duration.
     * @return Duration in milliseconds.
     */
    int32_t getStimDuration();

    /**
     * @brief Gets the trigger mode.
     * @return Trigger mode string.
     */
    String getTrigger();
};

#endif // LASER_H