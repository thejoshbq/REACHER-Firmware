#ifndef CUE_H
#define CUE_H

#include "Device.h"
#include <Arduino.h>

/**
 * @file Cue.h
 * @brief Defines the Cue class for managing a tone-based cue speaker.
 * 
 * The Cue class extends the Device class to handle a speaker that plays tones at
 * specified frequencies and durations, with timestamp-based control.
 */

/**
 * @class Cue
 * @brief A subclass of Device representing a cue speaker.
 * 
 * Manages a speaker connected to a digital pin, providing methods to control tone
 * frequency, duration, and on/off states based on timestamps.
 */
class Cue : public Device {
public:
    bool running;         ///< Indicates whether the cue is currently playing a tone.
    int32_t frequency;    ///< Frequency (in Hz) of the tone to be played.
    int32_t duration;     ///< Duration (in milliseconds) of the tone.
    int32_t onTimestamp;  ///< Timestamp (in milliseconds) when the tone starts.
    int32_t offTimestamp; ///< Timestamp (in milliseconds) when the tone stops.

    /**
     * @brief Constructor for the Cue class.
     * 
     * @param initPin The digital pin to which the cue speaker is connected.
     */
    Cue(byte initPin);

    /**
     * @brief Sets the running state of the cue.
     * @param initRunning Boolean to set the running state.
     */
    void setRunning(bool initRunning);

    /**
     * @brief Sets the tone frequency.
     * @param initFrequency Frequency (in Hz) to set.
     */
    void setFrequency(int32_t initFrequency);

    /**
     * @brief Sets the tone duration.
     * @param initDuration Duration (in milliseconds) to set.
     */
    void setDuration(int32_t initDuration);

    /**
     * @brief Turns the cue speaker on.
     */
    void on();

    /**
     * @brief Turns the cue speaker off.
     */
    void off();

    /**
     * @brief Checks if the cue is running.
     * @return Boolean indicating running state.
     */
    bool isRunning() const;

    /**
     * @brief Gets the current frequency.
     * @return Frequency (in Hz).
     */
    int32_t getFrequency() const;

    /**
     * @brief Gets the current duration.
     * @return Duration (in milliseconds).
     */
    int32_t getDuration() const;

    /**
     * @brief Sets the on timestamp.
     * @param currentTimestamp Time (in milliseconds) to start the tone.
     */
    void setOnTimestamp(int32_t currentTimestamp);

    /**
     * @brief Sets the off timestamp based on duration.
     * @param currentTimestamp Starting time (in milliseconds) for calculation.
     */
    void setOffTimestamp(int32_t currentTimestamp);

    /**
     * @brief Gets the on timestamp.
     * @return On timestamp (in milliseconds).
     */
    int32_t getOnTimestamp() const;

    /**
     * @brief Gets the off timestamp.
     * @return Off timestamp (in milliseconds).
     */
    int32_t getOffTimestamp() const;
};

#endif // CUE_H