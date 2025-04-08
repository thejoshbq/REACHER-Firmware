#include "Laser.h"
#include "Device.h"

/**
 * @brief Constructs a Laser object with an initial pin and default settings.
 * 
 * Initializes a Laser object as a subclass of Device with default stimulation settings.
 * 
 * @param initPin The digital pin (byte) to which the laser is connected.
 * @warning This implementation is in testing and not fully verified.
 */
Laser::Laser(byte initPin) 
    : Device(initPin), onDuration(2000), offDuration(3000), onTimestamp(0), offTimestamp(0), trigger("ON-PRESS") {}

/**
 * @brief Sets the stimulation start and end timestamps.
 * @param initTimestamp Time in milliseconds when the stimulation begins.
 */
void Laser::setStim(int32_t initTimestamp) {
    onTimestamp = initTimestamp;
    offTimestamp = initTimestamp + onDuration;
}

/**
 * @brief Sets the trigger mode for the laser.
 * @param initTrigger String indicating the trigger mode (e.g., "ON-PRESS").
 */
void Laser::setTrigger(String initTrigger) {
    trigger = initTrigger;
}

/**
 * @brief Sets the duration the laser is on during stimulation.
 * @param initDuration Duration in milliseconds for the laser to be on.
 */
void Laser::setStimOnDuration(int32_t initDuration) {
    onDuration = initDuration;
}

/**
 * @brief Sets the duration the laser is off after stimulation.
 * @param initDuration Duration in milliseconds for the laser to be off.
 */
void Laser::setStimOffDuration(int32_t initDuration) {
    offDuration = initDuration;
}

/**
 * @brief Sets the stimulation period with a trace interval.
 * @param initTimestamp Base time in milliseconds for the stimulation start.
 * @param traceInterval Delay in milliseconds before stimulation begins.
 */
void Laser::setStimPeriod(int32_t initTimestamp, int32_t traceInterval) {
    onTimestamp = initTimestamp + traceInterval;
    offTimestamp = onTimestamp + onDuration;
}

/**
 * @brief Turns the laser on and logs the action.
 * @note Currently logs to Serial; actual pin control is commented out.
 */
void Laser::on() {
    // digitalWrite(pin, HIGH);
    Serial.println("LASER ON");
}

/**
 * @brief Turns the laser off.
 * @note Actual pin control is commented out in this testing phase.
 */
void Laser::off() {
    // digitalWrite(pin, LOW);
}

/**
 * @brief Retrieves the stimulation start timestamp.
 * @return Start time in milliseconds.
 */
int32_t Laser::getStimOnTimestamp() {
    return onTimestamp;
}

/**
 * @brief Retrieves the stimulation end timestamp.
 * @return End time in milliseconds.
 */
int32_t Laser::getStimOffTimestamp() {
    return offTimestamp;
}

/**
 * @brief Retrieves the stimulation duration.
 * @return Duration in milliseconds.
 */
int32_t Laser::getStimDuration() {
    return onDuration;
}

/**
 * @brief Retrieves the trigger mode.
 * @return String indicating the trigger mode.
 */
String Laser::getTrigger() {
    return trigger;
}