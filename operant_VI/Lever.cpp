#include "Lever.h"

/**
 * @brief Constructs a Lever object with an initial pin and default settings.
 * 
 * Initializes a Lever object as a subclass of Device with default states, timestamps,
 * and variable interval settings.
 * 
 * @param initPin The digital pin (byte) to which the lever is connected.
 */
Lever::Lever(byte initPin) 
    : Device(initPin), previousLeverState(HIGH), stableLeverState(HIGH), 
      pressTimestamp(0), releaseTimestamp(0), orientation(""), pressType("NO CONDITION"), 
      intervalStartTime(0), randomInterval(0), activePressOccurred(false) {}

/**
 * @brief Sets the previous lever state for debouncing.
 * @param initState Boolean indicating the previous state.
 */
void Lever::setPreviousLeverState(bool initState) {
    previousLeverState = initState;
}

/**
 * @brief Sets the stable lever state after debouncing.
 * @param state Boolean indicating the stable state.
 */
void Lever::setStableLeverState(bool state) {
    stableLeverState = state;
}

/**
 * @brief Sets the timestamp of a lever press.
 * @param initTimestamp Time in milliseconds when the press occurred.
 */
void Lever::setPressTimestamp(int32_t initTimestamp) {
    pressTimestamp = initTimestamp;
}

/**
 * @brief Sets the timestamp of a lever release.
 * @param initTimestamp Time in milliseconds when the release occurred.
 */
void Lever::setReleaseTimestamp(int32_t initTimestamp) {
    releaseTimestamp = initTimestamp;
}

/**
 * @brief Sets the orientation of the lever (e.g., "RH" or "LH").
 * @param initOrientation String indicating the lever's orientation.
 */
void Lever::setOrientation(String initOrientation) {
    orientation = initOrientation;
}

/**
 * @brief Sets the type of press (e.g., "ACTIVE", "INACTIVE").
 * @param initPressType String indicating the press type.
 */
void Lever::setPressType(String initPressType) {
    pressType = initPressType;
}

/**
 * @brief Resets the variable interval for the lever.
 * 
 * Sets a new start time and generates a random interval between 0 and 15000 ms.
 */
void Lever::resetInterval() {
    intervalStartTime = millis();
    randomInterval = random(0, 15000);
    activePressOccurred = false;
}

/**
 * @brief Sets whether an active press has occurred in the current interval.
 * @param state Boolean indicating if an active press has occurred.
 */
void Lever::setActivePressOccurred(bool state) {
    activePressOccurred = state;
}

/**
 * @brief Retrieves the previous lever state.
 * @return Boolean indicating the previous state.
 */
bool Lever::getPreviousLeverState() const {
    return previousLeverState;
}

/**
 * @brief Retrieves the stable lever state.
 * @return Boolean indicating the stable state.
 */
bool Lever::getStableLeverState() const {
    return stableLeverState;
}

/**
 * @brief Checks if an active press has occurred in the current interval.
 * @return Boolean indicating if an active press has occurred.
 */
bool Lever::getActivePressOccurred() {
    return activePressOccurred;
}

/**
 * @brief Retrieves the press timestamp.
 * @return Time in milliseconds of the press.
 */
int32_t Lever::getPressTimestamp() const {
    return pressTimestamp;
}

/**
 * @brief Retrieves the release timestamp.
 * @return Time in milliseconds of the release.
 */
int32_t Lever::getReleaseTimestamp() const {
    return releaseTimestamp;
}

/**
 * @brief Retrieves the start time of the current interval.
 * @return Start time in milliseconds.
 */
uint32_t Lever::getIntervalStartTime() {
    return intervalStartTime;
}

/**
 * @brief Retrieves the random interval duration.
 * @return Random interval in milliseconds.
 */
uint32_t Lever::getRandomInterval() {
    return randomInterval;
}

/**
 * @brief Retrieves the lever's orientation.
 * @return String indicating the orientation.
 */
String Lever::getOrientation() const {
    return orientation;
}

/**
 * @brief Retrieves the press type.
 * @return String indicating the press type.
 */
String Lever::getPressType() const {
    return pressType;
}