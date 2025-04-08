#include "Cue.h"
#include "Device.h"

/**
 * @brief Constructs a Cue object with an initial pin and default settings.
 * 
 * Initializes a Cue object as a subclass of Device, setting the pin for tone output
 * and default values for running state, frequency, and duration.
 * 
 * @param initPin The digital pin (byte) to which the cue speaker is connected.
 */
Cue::Cue(byte initPin) 
    : Device(initPin), running(false), frequency(8000), duration(1600), onTimestamp(0), offTimestamp(0) {}

/**
 * @brief Sets the running state of the cue.
 * 
 * @param initRunning Boolean indicating whether the cue is currently playing a tone.
 */
void Cue::setRunning(bool initRunning) {
    running = initRunning;
}

/**
 * @brief Sets the frequency of the cue tone and logs the change.
 * 
 * @param initFrequency The frequency (in Hz) to set for the cue tone.
 */
void Cue::setFrequency(int32_t initFrequency) {
    frequency = initFrequency;
    Serial.println("SET CUE FREQUENCY TO: " + String(frequency));
}

/**
 * @brief Sets the duration of the cue tone and logs the change.
 * 
 * @param initDuration The duration (in milliseconds) for which the tone will play.
 */
void Cue::setDuration(int32_t initDuration) {
    duration = initDuration;
    Serial.println("SET CUE DURATION TO: " + String(duration));
}

/**
 * @brief Activates the cue speaker with the set frequency.
 * 
 * Starts playing a tone on the assigned pin at the specified frequency.
 */
void Cue::on() {
    tone(pin, frequency);
}

/**
 * @brief Deactivates the cue speaker.
 * 
 * Stops the tone on the assigned pin.
 */
void Cue::off() {
    noTone(pin);
}

/**
 * @brief Checks if the cue is currently running.
 * 
 * @return Boolean indicating whether the cue is playing a tone.
 */
bool Cue::isRunning() const {
    return running;
}

/**
 * @brief Retrieves the current frequency of the cue.
 * 
 * @return The frequency (in Hz) of the cue tone.
 */
int32_t Cue::getFrequency() const {
    return frequency;
}

/**
 * @brief Retrieves the current duration of the cue.
 * 
 * @return The duration (in milliseconds) of the cue tone.
 */
int32_t Cue::getDuration() const {
    return duration;
}

/**
 * @brief Sets the timestamp when the cue should turn on.
 * 
 * @param currentTimestamp The time (in milliseconds) when the cue should activate.
 */
void Cue::setOnTimestamp(int32_t currentTimestamp) {
    onTimestamp = currentTimestamp;
}

/**
 * @brief Sets the timestamp when the cue should turn off.
 * 
 * Calculates the off timestamp based on the current timestamp plus the duration.
 * 
 * @param currentTimestamp The starting time (in milliseconds) for the off calculation.
 */
void Cue::setOffTimestamp(int32_t currentTimestamp) {
    offTimestamp = currentTimestamp + duration;
}

/**
 * @brief Retrieves the timestamp when the cue turns on.
 * 
 * @return The on timestamp (in milliseconds).
 */
int32_t Cue::getOnTimestamp() const {
    return onTimestamp;
}

/**
 * @brief Retrieves the timestamp when the cue turns off.
 * 
 * @return The off timestamp (in milliseconds).
 */
int32_t Cue::getOffTimestamp() const {
    return offTimestamp;
}