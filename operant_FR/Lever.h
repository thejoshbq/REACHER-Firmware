#ifndef LEVER_H
#define LEVER_H

#include "Device.h"
#include <Arduino.h>

/**
 * @file Lever.h
 * @brief Defines the Lever class for monitoring lever interactions.
 */

/**
 * @class Lever
 * @brief A subclass of Device representing a lever input device.
 * 
 * Manages lever state, timestamps, orientation, and press type for behavioral experiments.
 */
class Lever : public Device {
public:
    bool previousLeverState;     ///< Previous state for debouncing (HIGH or LOW).
    bool stableLeverState;       ///< Stable state after debouncing (HIGH or LOW).
    int32_t pressTimestamp;      ///< Timestamp of the lever press (ms).
    int32_t releaseTimestamp;    ///< Timestamp of the lever release (ms).
    String orientation;          ///< Lever orientation (e.g., "RH" or "LH").
    String pressType;            ///< Type of press (e.g., "ACTIVE", "INACTIVE", "TIMEOUT").

    /**
     * @brief Constructor for the Lever class.
     * @param initPin The digital pin to which the lever is connected.
     */
    Lever(byte initPin);

    /**
     * @brief Sets the previous lever state.
     * @param initState Boolean previous state.
     */
    void setPreviousLeverState(bool initState);

    /**
     * @brief Sets the stable lever state.
     * @param state Boolean stable state.
     */
    void setStableLeverState(bool state);

    /**
     * @brief Sets the press timestamp.
     * @param initTimestamp Time in milliseconds.
     */
    void setPressTimestamp(int32_t initTimestamp);

    /**
     * @brief Sets the release timestamp.
     * @param initTimestamp Time in milliseconds.
     */
    void setReleaseTimestamp(int32_t initTimestamp);

    /**
     * @brief Sets the lever orientation.
     * @param initOrientation String orientation (e.g., "RH" or "LH").
     */
    void setOrientation(String initOrientation);

    /**
     * @brief Sets the press type.
     * @param initPressType String press type (e.g., "ACTIVE").
     */
    void setPressType(String initPressType);

    /**
     * @brief Gets the previous lever state.
     * @return Boolean previous state.
     */
    bool getPreviousLeverState() const;

    /**
     * @brief Gets the stable lever state.
     * @return Boolean stable state.
     */
    bool getStableLeverState() const;

    /**
     * @brief Gets the press timestamp.
     * @return Time in milliseconds.
     */
    int32_t getPressTimestamp() const;

    /**
     * @brief Gets the release timestamp.
     * @return Time in milliseconds.
     */
    int32_t getReleaseTimestamp() const;

    /**
     * @brief Gets the lever orientation.
     * @return String orientation.
     */
    String getOrientation() const;

    /**
     * @brief Gets the press type.
     * @return String press type.
     */
    String getPressType() const;
};

#endif // LEVER_H