#include "LickCircuit.h"

/**
 * @brief Constructs a LickCircuit object with an initial pin and default settings.
 * 
 * Initializes a LickCircuit object as a subclass of Device with default states and timestamps.
 * 
 * @param initPin The digital pin (byte) to which the lick circuit is connected.
 */
LickCircuit::LickCircuit(byte initPin) 
    : Device(initPin), previousLickState(LOW), stableLickState(LOW), lickTimestamp(0), releaseTimestamp(0) {}

/**
 * @brief Sets the previous lick state for debouncing.
 * @param initState Boolean indicating the previous state.
 */
void LickCircuit::setPreviousLickState(bool initState) {
    previousLickState = initState;
}

/**
 * @brief Sets the stable lick state after debouncing.
 * @param state Boolean indicating the stable state.
 */
void LickCircuit::setStableLickState(bool state) {
    stableLickState = state;
}

/**
 * @brief Sets the timestamp of a lick touch.
 * @param initTimestamp Time in milliseconds when the lick occurred.
 */
void LickCircuit::setLickTouchTimestamp(int32_t initTimestamp) {
    lickTimestamp = initTimestamp;
}

/**
 * @brief Sets the timestamp of a lick release.
 * @param initTimestamp Time in milliseconds when the release occurred.
 */
void LickCircuit::setLickReleaseTimestamp(int32_t initTimestamp) {
    releaseTimestamp = initTimestamp;
}

/**
 * @brief Retrieves the previous lick state.
 * @return Boolean indicating the previous state.
 */
bool LickCircuit::getPreviousLickState() const {
    return previousLickState;
}

/**
 * @brief Retrieves the stable lick state.
 * @return Boolean indicating the stable state.
 */
bool LickCircuit::getStableLickState() const {
    return stableLickState;
}

/**
 * @brief Retrieves the lick touch timestamp.
 * @return Time in milliseconds of the lick.
 */
int32_t LickCircuit::getLickTouchTimestamp() const {
    return lickTimestamp;
}

/**
 * @brief Retrieves the lick release timestamp.
 * @return Time in milliseconds of the release.
 */
int32_t LickCircuit::getLickReleaseTimestamp() const {
    return releaseTimestamp;
}