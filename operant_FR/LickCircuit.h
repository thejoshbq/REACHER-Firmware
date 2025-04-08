#ifndef LICKCIRCUIT_H
#define LICKCIRCUIT_H

#include "Arduino.h"
#include "Device.h"

/**
 * @file LickCircuit.h
 * @brief Defines the LickCircuit class for monitoring lick interactions.
 */

/**
 * @class LickCircuit
 * @brief A subclass of Device representing a lick detection circuit.
 * 
 * Manages lick state and timestamps for behavioral experiments.
 */
class LickCircuit : public Device {
private:
    bool previousLickState;   ///< Previous state for debouncing (HIGH or LOW).
    bool stableLickState;     ///< Stable state after debouncing (HIGH or LOW).
    int32_t lickTimestamp;    ///< Timestamp of the lick touch (ms).
    int32_t releaseTimestamp; ///< Timestamp of the lick release (ms).

public:
    /**
     * @brief Constructor for the LickCircuit class.
     * @param initPin The digital pin to which the lick circuit is connected.
     */
    LickCircuit(byte initPin);

    /**
     * @brief Sets the previous lick state.
     * @param initState Boolean previous state.
     */
    void setPreviousLickState(bool initState);

    /**
     * @brief Sets the stable lick state.
     * @param state Boolean stable state.
     */
    void setStableLickState(bool state);

    /**
     * @brief Sets the lick touch timestamp.
     * @param initTimestamp Time in milliseconds.
     */
    void setLickTouchTimestamp(int32_t initTimestamp);

    /**
     * @brief Sets the lick release timestamp.
     * @param initTimestamp Time in milliseconds.
     */
    void setLickReleaseTimestamp(int32_t initTimestamp);

    /**
     * @brief Gets the previous lick state.
     * @return Boolean previous state.
     */
    bool getPreviousLickState() const;

    /**
     * @brief Gets the stable lick state.
     * @return Boolean stable state.
     */
    bool getStableLickState() const;

    /**
     * @brief Gets the lick touch timestamp.
     * @return Time in milliseconds.
     */
    int32_t getLickTouchTimestamp() const;

    /**
     * @brief Gets the lick release timestamp.
     * @return Time in milliseconds.
     */
    int32_t getLickReleaseTimestamp() const;
};

#endif // LICKCIRCUIT_H