#include "Cue.h"
#include "Device.h"

/**
 * @brief Constructs a Cue object with an initial pin and default settings.
 * 
 * Initializes a Cue object as a subclass of Device with default tone settings.
 * 
 * @param initPin The digital pin (byte) to which the cue speaker is connected.
 */
Cue::Cue(byte initPin) 
    : Device(initPin), running(false), frequency(8000), duration(1600), onTimestamp(0), offTimestamp(0) {}

/**
 * @brief Sets the running state of the cue tone.
 * @param initRunning Boolean indicating if the cue is playing a tone.
 */
void Cue::setRunning(bool initRunning) {
    running = initRunning;
}

/**
 * @brief Sets the frequency of the cue tone and logs the change.
 * @param initFrequency Frequency in Hz to set for the cue tone.
 */
void Cue::setFrequency(int32_t initFrequency) {
    frequency = initFrequency;
    Serial.println("SET CUE FREQUENCY TO: " + String(frequency));
}

/**
 * @brief Sets the duration of the cue tone and logs the change.
 * @param initDuration Duration in milliseconds to set for the cue tone.
 */
void Cue::setDuration(int32_t initDuration) {
    duration = initDuration;
    Serial.println("SET CUE DURATION TO: " + String(duration));
}

/**
 * @brief Turns the cue tone on at the specified frequency.
 */
void Cue::on() {
    tone(pin, frequency);
}

/**
 * @brief Turns the cue tone off.
 */
void Cue::off() {
    noTone(pin);
}

/**
 * @brief Checks if the cue tone is currently playing.
 * @return Boolean indicating the running state.
 */
bool Cue::isRunning() const {
    return running;
}

/**
 * @brief Retrieves the cue tone frequency.
 * @return Frequency in Hz.
 */
int32_t Cue::getFrequency() const {
    return frequency;
}

/**
 * @brief Retrieves the cue tone duration.
 * @return Duration in milliseconds.
 */
int32_t Cue::getDuration() const {
    return duration;
}

/**
 * @brief Sets the timestamp when the cue tone starts.
 * @param currentTimestamp Time in milliseconds when the tone begins.
 */
void Cue::setOnTimestamp(int32_t currentTimestamp) {
    onTimestamp = currentTimestamp;
}

/**
 * @brief Sets the timestamp when the cue tone ends.
 * @param currentTimestamp Time in milliseconds to base the end time on.
 */
void Cue::setOffTimestamp(int32_t currentTimestamp) {
    offTimestamp = currentTimestamp + duration;
}

/**
 * @brief Retrieves the cue tone start timestamp.
 * @return Start time in milliseconds.
 */
int32_t Cue::getOnTimestamp() const {
    return onTimestamp;
}

/**
 * @brief Retrieves the cue tone end timestamp.
 * @return End time in milliseconds.
 */
int32_t Cue::getOffTimestamp() const {
    return offTimestamp;
}