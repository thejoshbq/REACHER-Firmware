#include "Laser.h"

/**
 * @brief Constructs a Laser object with an initial pin and default settings.
 * 
 * Initializes a Laser object as a subclass of Device, setting the pin and default
 * values for stimulation duration, frequency, timing, and state variables.
 * 
 * @param initPin The digital pin (byte) to which the laser is connected.
 */
Laser::Laser(byte initPin) 
    : Device(initPin), duration(30000), frequency(20), stimStart(0), stimEnd(0), 
      halfCycleStart(0), halfCycleEnd(0), logged(true), cycleUp(false), 
      laserMode(CYCLE), laserState(INACTIVE), laserAction(OFF) {}

/**
 * @brief Sets the stimulation duration in milliseconds.
 * 
 * Converts the input duration from seconds to milliseconds.
 * 
 * @param initDuration Duration in seconds to set for the stimulation period.
 */
void Laser::setDuration(uint32_t initDuration) {
    duration = initDuration * 1000;
}

/**
 * @brief Sets the frequency of the laser pulses.
 * 
 * @param initFrequency Frequency in Hz to set for the laser pulses.
 */
void Laser::setFrequency(uint32_t initFrequency) {
    frequency = initFrequency;
}

/**
 * @brief Sets the stimulation period start and end times.
 * 
 * @param currentMillis Current time in milliseconds to base the period on.
 */
void Laser::setStimPeriod(uint32_t currentMillis) {
    stimStart = currentMillis;
    stimEnd = currentMillis + duration;
}

/**
 * @brief Sets the half-cycle period for laser oscillation.
 * 
 * Calculates the half-cycle duration based on the frequency and updates the start
 * and end times accordingly.
 * 
 * @param currentMillis Current time in milliseconds to base the half-cycle on.
 */
void Laser::setStimHalfCyclePeriod(uint32_t currentMillis) {
    float halfCycleLength = (1.0f / frequency) / 2.0f * 1000.0f; // Half-cycle in ms
    halfCycleStart = currentMillis;
    halfCycleEnd = currentMillis + static_cast<uint32_t>(halfCycleLength);
}

/**
 * @brief Sets the logged state of the stimulation event.
 * 
 * @param log Boolean indicating if the stimulation has been logged.
 */
void Laser::setStimLogged(bool log) {
    logged = log;
}

/**
 * @brief Sets the cycle-up state for stimulation cycling.
 * 
 * @param cycle Boolean indicating if the laser is in the active cycle phase.
 */
void Laser::setCycleUp(bool cycle) {
    cycleUp = cycle;
}

/**
 * @brief Sets the stimulation mode (CYCLE or ACTIVE_PRESS).
 * 
 * @param mode The MODE enum value to set (CYCLE or ACTIVE_PRESS).
 */
void Laser::setStimMode(MODE mode) {
    laserMode = mode;
}

/**
 * @brief Sets the stimulation state (ACTIVE or INACTIVE).
 * 
 * @param state The STATE enum value to set (ACTIVE or INACTIVE).
 */
void Laser::setStimState(STATE state) {
    laserState = state;
}

/**
 * @brief Sets the laser action (ON or OFF).
 * 
 * @param action The ACTION enum value to set (ON or OFF).
 */
void Laser::setStimAction(ACTION action) {
    laserAction = action;
}

/**
 * @brief Retrieves the stimulation duration.
 * 
 * @return Duration in milliseconds.
 */
uint32_t Laser::getDuration() {
    return duration;
}

/**
 * @brief Retrieves the laser pulse frequency.
 * 
 * @return Frequency in Hz.
 */
uint32_t Laser::getFrequency() {
    return frequency;
}

/**
 * @brief Retrieves the stimulation start time.
 * 
 * @return Start time in milliseconds.
 */
uint32_t Laser::getStimStart() {
    return stimStart;
}

/**
 * @brief Retrieves the stimulation end time.
 * 
 * @return End time in milliseconds.
 */
uint32_t Laser::getStimEnd() {
    return stimEnd;
}

/**
 * @brief Retrieves the half-cycle start time.
 * 
 * @return Half-cycle start time in milliseconds.
 */
uint32_t Laser::getStimHalfCycleStart() {
    return halfCycleStart;
}

/**
 * @brief Retrieves the half-cycle end time.
 * 
 * @return Half-cycle end time in milliseconds.
 */
uint32_t Laser::getStimHalfCycleEnd() {
    return halfCycleEnd;
}

/**
 * @brief Checks if the stimulation has been logged.
 * 
 * @return Boolean indicating logged state.
 */
bool Laser::getStimLog() {
    return logged;
}

/**
 * @brief Checks if the laser is in the active cycle phase.
 * 
 * @return Boolean indicating cycle-up state.
 */
bool Laser::getCycleUp() {
    return cycleUp;
}

/**
 * @brief Retrieves the current stimulation mode.
 * 
 * @return MODE enum value (CYCLE or ACTIVE_PRESS).
 */
MODE Laser::getStimMode() {
    return laserMode;
}

/**
 * @brief Retrieves the current stimulation state.
 * 
 * @return STATE enum value (ACTIVE or INACTIVE).
 */
STATE Laser::getStimState() {
    return laserState;
}

/**
 * @brief Retrieves the current laser action.
 * 
 * @return ACTION enum value (ON or OFF).
 */
ACTION Laser::getStimAction() {
    return laserAction;
}

/**
 * @brief Turns the laser on by setting the pin high.
 */
void Laser::on() {
    digitalWrite(pin, HIGH);  // Turn the laser ON
    // Serial.println("ON, " + String(laserAction) + ", " + String(cycleUp) + ", " + String(laserState)); // Uncomment for debugging
}

/**
 * @brief Turns the laser off by setting the pin low.
 */
void Laser::off() {
    digitalWrite(pin, LOW);   // Turn the laser OFF
    // Serial.println("OFF, " + String(laserAction) + ", " + String(cycleUp) + ", " + String(laserState)); // Uncomment for debugging
}