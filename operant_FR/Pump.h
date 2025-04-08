#ifndef PUMP_H
#define PUMP_H

#include "Device.h"
#include <Arduino.h>

/**
 * @file Pump.h
 * @brief Defines the Pump class for controlling a pump device.
 */

/**
 * @class Pump
 * @brief A subclass of Device representing a pump for infusion delivery.
 * 
 * Manages pump state, infusion timing, and motor settings.
 */
class Pump : public Device {
public:
    bool running;                 ///< Indicates if the pump is running.
    int32_t infusionDuration;     ///< Duration of infusion (ms).
    float infusionAmount;         ///< Amount of infusion (e.g., microliters).
    float motorRPMs;              ///< Motor speed in RPMs.
    int32_t infusionStartTimestamp; ///< Start time of infusion (ms).
    int32_t infusionEndTimestamp;   ///< End time of infusion (ms).

    /**
     * @brief Constructor for the Pump class.
     * @param initPin The digital pin to which the pump is connected.
     */
    Pump(byte initPin);

    /**
     * @brief Sets the pump running state.
     * @param initRunning Boolean running state.
     */
    void setRunning(bool initRunning);

    /**
     * @brief Sets the infusion duration.
     * @param initDuration Duration in milliseconds.
     */
    void setInfusionDuration(int32_t initDuration);

    /**
     * @brief Sets the infusion amount.
     * @param initAmount Amount in arbitrary units.
     */
    void setInfusionAmount(float initAmount);

    /**
     * @brief Sets the motor RPMs.
     * @param initRPMs RPMs of the pump motor.
     */
    void setMotorRPMs(float initRPMs);

    /**
     * @brief Sets the infusion period.
     * @param cueOffTimestamp Cue off timestamp (ms).
     * @param traceInterval Trace interval (ms).
     */
    void setInfusionPeriod(int32_t cueOffTimestamp, int32_t traceInterval);

    /**
     * @brief Turns the pump on.
     */
    void on();

    /**
     * @brief Turns the pump off.
     */
    void off();

    /**
     * @brief Checks if the pump is running.
     * @return Boolean running state.
     */
    bool isRunning() const;

    /**
     * @brief Gets the infusion duration.
     * @return Duration in milliseconds.
     */
    int32_t getInfusionDuration() const;

    /**
     * @brief Gets the infusion amount.
     * @return Amount in arbitrary units.
     */
    float getInfusionAmount() const;

    /**
     * @brief Gets the motor RPMs.
     * @return RPMs of the pump motor.
     */
    float getMotorRPMs() const;

    /**
     * @brief Gets the infusion start timestamp.
     * @return Start time in milliseconds.
     */
    int32_t getInfusionStartTimestamp() const;

    /**
     * @brief Gets the infusion end timestamp.
     * @return End time in milliseconds.
     */
    int32_t getInfusionEndTimestamp() const;
};

#endif // PUMP_H