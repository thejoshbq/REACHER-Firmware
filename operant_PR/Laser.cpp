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
    : Device(initPin), stimDuration(30000), previousStim(0), isRunning(false) {}

/**
 * @brief Sets the duration of the laser stimulation.
 * @param initDuration Duration in milliseconds for the laser stimulation.
 */
void Laser::setStimDuration(int32_t initDuration) {
    stimDuration = initDuration;
}

/**
 * @brief Sets the timestamp of the previous stimulation.
 * @param timestamp Time in milliseconds of the last stimulation.
 */
void Laser::setPreviousStim(int32_t timestamp) {
    previousStim = timestamp;
}

/**
 * @brief Sets the running state of the laser.
 * @param initRunning Boolean indicating if the laser is running.
 */
void Laser::setIsRunning(bool initRunning) {
    isRunning = initRunning;
}

/**
 * @brief Turns the laser on.
 */
void Laser::on() {
    digitalWrite(pin, HIGH);
}

/**
 * @brief Turns the laser off.
 */
void Laser::off() {
    digitalWrite(pin, LOW);
}

/**
 * @brief Retrieves the timestamp of the previous stimulation.
 * @return Time in milliseconds.
 */
int32_t Laser::getPreviousStim() {
    return previousStim;
}

/**
 * @brief Retrieves the stimulation duration.
 * @return Duration in milliseconds.
 */
int32_t Laser::getStimDuration() {
    return stimDuration;
}

/**
 * @brief Checks if the laser is running.
 * @return Boolean indicating running state.
 */
bool Laser::getIsRunning() {
    return isRunning;
}