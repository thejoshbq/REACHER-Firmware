#ifndef LEVER_H
#define LEVER_H

#include "Device.h"
#include <Arduino.h>

/**
 * @file Lever.h
 * @brief Defines the Lever class for monitoring lever interactions with variable intervals.
 */

/**
 * @class Lever
 * @brief A subclass of Device representing a lever input device with variable interval logic.
 * 
 * Manages lever state, timestamps, orientation, press type, and variable interval timing
 * for behavioral experiments.
 */
class Lever : public Device {
public:
    bool previousLeverState;     ///< Previous state for debouncing (HIGH or LOW).
    bool stableLeverState;       ///< Stable state after debouncing (HIGH or LOW).
    int32_t pressTimestamp;      ///< Timestamp of the lever press (ms).
    int32_t releaseTimestamp;    ///< Timestamp of the lever release (ms).
    String orientation;          ///< Lever orientation (e.g., "RH" or "LH").
    String pressType;            ///< Type of press (e.g., "ACTIVE", "INACTIVE").
    uint32_t intervalStartTime;  ///< Start time of the current variable interval (ms).
    uint32_t randomInterval;     ///< Random duration within the variable interval (ms).
    bool activePressOccurred;    ///< Indicates if an active press has occurred in the interval.

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
     * @brief Resets the variable interval.
     */
    void resetInterval();

    /**
     * @brief Sets whether an active press has occurred.
     * @param state Boolean indicating active press state.
     */
    void setActivePressOccurred(bool state);

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
     * @brief Gets whether an active press has occurred.
     * @return Boolean active press state.
     */
    bool getActivePressOccurred();

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
     * @brief Gets the start time of the current interval.
     * @return Start time in milliseconds.
     */
    uint32_t getIntervalStartTime();

    /**
     * @brief Gets the random interval duration.
     * @return Random interval in milliseconds.
     */
    uint32_t getRandomInterval();

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