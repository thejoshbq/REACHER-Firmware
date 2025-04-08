#include "Pump.h"
#include "Device.h"
#include <Arduino.h>

/**
 * @brief Constructs a Pump object with an initial pin and default settings.
 * 
 * Initializes a Pump object as a subclass of Device with default infusion settings.
 * 
 * @param initPin The digital pin (byte) to which the pump is connected.
 */
Pump::Pump(byte initPin) 
    : Device(initPin), running(false), infusionDuration(2000), infusionAmount(0.0f), 
      motorRPMs(0.0f), infusionStartTimestamp(0), infusionEndTimestamp(0) {}

/**
 * @brief Sets the pump running state.
 * @param initRunning Boolean indicating if the pump is running.
 */
void Pump::setRunning(bool initRunning) {
    running = initRunning;
}

/**
 * @brief Sets the infusion duration.
 * @param initDuration Duration in milliseconds.
 */
void Pump::setInfusionDuration(int32_t initDuration) {
    infusionDuration = initDuration;
}

/**
 * @brief Sets the infusion amount.
 * @param initAmount Amount in arbitrary units (e.g., microliters).
 */
void Pump::setInfusionAmount(float initAmount) {
    infusionAmount = initAmount;
}

/**
 * @brief Sets the motor RPMs.
 * @param initRPMs Revolutions per minute of the pump motor.
 */
void Pump::setMotorRPMs(float initRPMs) {
    motorRPMs = initRPMs;
}

/**
 * @brief Sets the infusion period based on cue offset and trace interval.
 * @param cueOffTimestamp Cue off timestamp in milliseconds.
 * @param traceInterval Trace interval in milliseconds.
 */
void Pump::setInfusionPeriod(int32_t cueOffTimestamp, int32_t traceInterval) {
    infusionStartTimestamp = cueOffTimestamp + traceInterval;
    infusionEndTimestamp = infusionStartTimestamp + infusionDuration;
}

/**
 * @brief Turns the pump on.
 */
void Pump::on() {
    digitalWrite(pin, HIGH);
}

/**
 * @brief Turns the pump off.
 */
void Pump::off() {
    digitalWrite(pin, LOW);
}

/**
 * @brief Checks if the pump is running.
 * @return Boolean indicating running state.
 */
bool Pump::isRunning() const {
    return running;
}

/**
 * @brief Retrieves the infusion duration.
 * @return Duration in milliseconds.
 */
int32_t Pump::getInfusionDuration() const {
    return infusionDuration;
}

/**
 * @brief Retrieves the infusion amount.
 * @return Amount in arbitrary units.
 */
float Pump::getInfusionAmount() const {
    return infusionAmount;
}

/**
 * @brief Retrieves the motor RPMs.
 * @return RPMs of the pump motor.
 */
float Pump::getMotorRPMs() const {
    return motorRPMs;
}

/**
 * @brief Retrieves the infusion start timestamp.
 * @return Start time in milliseconds.
 */
int32_t Pump::getInfusionStartTimestamp() const {
    return infusionStartTimestamp;
}

/**
 * @brief Retrieves the infusion end timestamp.
 * @return End time in milliseconds.
 */
int32_t Pump::getInfusionEndTimestamp() const {
    return infusionEndTimestamp;
}