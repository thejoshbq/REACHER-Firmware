#ifndef CUE_H
#define CUE_H

#include "Device.h"
#include <Arduino.h>

/**
 * @file Cue.h
 * @brief Defines the Cue class for controlling a cue speaker device.
 */

/**
 * @class Cue
 * @brief A subclass of Device representing a cue speaker for tone playback.
 * 
 * Manages a speaker connected to a digital pin, providing control over tone frequency,
 * duration, and timing.
 */
class Cue : public Device {
public:
    bool running;         ///< Indicates if the cue tone is playing.
    int32_t frequency;    ///< Frequency of the cue tone (Hz).
    int32_t duration;     ///< Duration of the cue tone (ms).
    int32_t onTimestamp;  ///< Timestamp when the cue tone starts (ms).
    int32_t offTimestamp; ///< Timestamp when the cue tone ends (ms).

    /**
     * @brief Constructor for the Cue class.
     * @param initPin The digital pin to which the cue speaker is connected.
     */
    Cue(byte initPin);

    /**
     * @brief Sets the running state of the cue.
     * @param initRunning Boolean indicating running state.
     */
    void setRunning(bool initRunning);

    /**
     * @brief Sets the frequency of the cue tone.
     * @param initFrequency Frequency in Hz.
     */
    void setFrequency(int32_t initFrequency);

    /**
     * @brief Sets the duration of the cue tone.
     * @param initDuration Duration in milliseconds.
     */
    void setDuration(int32_t initDuration);

    /**
     * @brief Turns the cue tone on.
     */
    void on();

    /**
     * @brief Turns the cue tone off.
     */
    void off();

    /**
     * @brief Checks if the cue is running.
     * @return Boolean indicating running state.
     */
    bool isRunning() const;

    /**
     * @brief Gets the cue tone frequency.
     * @return Frequency in Hz.
     */
    int32_t getFrequency() const;

    /**
     * @brief Gets the cue tone duration.
     * @return Duration in milliseconds.
     */
    int32_t getDuration() const;

    /**
     * @brief Sets the start timestamp of the cue tone.
     * @param currentTimestamp Start time in milliseconds.
     */
    void setOnTimestamp(int32_t currentTimestamp);

    /**
     * @brief Sets the end timestamp of the cue tone.
     * @param currentTimestamp Base time in milliseconds for end calculation.
     */
    void setOffTimestamp(int32_t currentTimestamp);

    /**
     * @brief Gets the start timestamp of the cue tone.
     * @return Start time in milliseconds.
     */
    int32_t getOnTimestamp() const;

    /**
     * @brief Gets the end timestamp of the cue tone.
     * @return End time in milliseconds.
     */
    int32_t getOffTimestamp() const;
};

#endif // CUE_H