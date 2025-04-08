#ifndef LASER_H
#define LASER_H

#include "Device.h"
#include <Arduino.h>

/**
 * @file Laser.h
 * @brief Defines the Laser class for controlling a laser device.
 * 
 * The Laser class extends the Device class to manage a laser with stimulation
 * periods, frequencies, and cycling behavior.
 */

/**
 * @enum MODE
 * @brief Defines the operating modes of the laser.
 */
enum MODE { CYCLE,     ///< Laser operates in oscillating cycles.
            ACTIVE_PRESS ///< Laser is active based on external input.
};

/**
 * @enum STATE
 * @brief Defines the stimulation states of the laser.
 */
enum STATE { INACTIVE, ///< Laser is not stimulating.
             ACTIVE    ///< Laser is actively stimulating.
};

/**
 * @enum ACTION
 * @brief Defines the on/off actions of the laser.
 */
enum ACTION { OFF, ///< Laser is turned off.
              ON   ///< Laser is turned on.
};

/**
 * @class Laser
 * @brief A subclass of Device representing a laser with stimulation control.
 * 
 * Manages a laser connected to a digital pin, providing methods to control
 * stimulation duration, frequency, and on/off states with cycling behavior.
 */
class Laser : public Device {
private:
    uint32_t duration;        ///< Total duration of the stimulation period (ms).
    uint32_t frequency;       ///< Frequency of laser pulses (Hz).
    uint32_t stimStart;       ///< Start time of the stimulation period (ms).
    uint32_t stimEnd;         ///< End time of the stimulation period (ms).
    uint32_t halfCycleStart;  ///< Start time of the half-cycle (ms).
    uint32_t halfCycleEnd;    ///< End time of the half-cycle (ms).
    bool logged;              ///< Indicates if the stimulation has been logged.
    bool cycleUp;             ///< Indicates if the laser is in the active cycle phase.
    MODE laserMode;           ///< Current operating mode (CYCLE or ACTIVE_PRESS).
    STATE laserState;         ///< Current stimulation state (ACTIVE or INACTIVE).
    ACTION laserAction;       ///< Current laser action (ON or OFF).

public:
    /**
     * @brief Constructor for the Laser class.
     * 
     * @param initPin The digital pin to which the laser is connected.
     */
    Laser(byte initPin);

    // Setters
    /**
     * @brief Sets the stimulation duration in seconds (converted to ms).
     * @param initDuration Duration in seconds.
     */
    void setDuration(uint32_t initDuration);

    /**
     * @brief Sets the frequency of laser pulses.
     * @param initFrequency Frequency in Hz.
     */
    void setFrequency(uint32_t initFrequency);

    /**
     * @brief Sets the stimulation period start and end times.
     * @param currentMillis Current time in milliseconds.
     */
    void setStimPeriod(uint32_t currentMillis);

    /**
     * @brief Sets the half-cycle period for oscillation.
     * @param currentMillis Current time in milliseconds.
     */
    void setStimHalfCyclePeriod(uint32_t currentMillis);

    /**
     * @brief Sets the logged state of the stimulation.
     * @param log Boolean indicating logged state.
     */
    void setStimLogged(bool log);

    /**
     * @brief Sets the cycle-up state.
     * @param cycle Boolean indicating cycle state.
     */
    void setCycleUp(bool cycle);

    /**
     * @brief Sets the stimulation mode.
     * @param mode MODE enum value (CYCLE or ACTIVE_PRESS).
     */
    void setStimMode(MODE mode);

    /**
     * @brief Sets the stimulation state.
     * @param state STATE enum value (ACTIVE or INACTIVE).
     */
    void setStimState(STATE state);

    /**
     * @brief Sets the laser action.
     * @param action ACTION enum value (ON or OFF).
     */
    void setStimAction(ACTION action);

    // Getters
    /**
     * @brief Gets the stimulation duration.
     * @return Duration in milliseconds.
     */
    uint32_t getDuration();

    /**
     * @brief Gets the laser pulse frequency.
     * @return Frequency in Hz.
     */
    uint32_t getFrequency();

    /**
     * @brief Gets the stimulation start time.
     * @return Start time in milliseconds.
     */
    uint32_t getStimStart();

    /**
     * @brief Gets the stimulation end time.
     * @return End time in milliseconds.
     */
    uint32_t getStimEnd();

    /**
     * @brief Gets the half-cycle start time.
     * @return Half-cycle start time in milliseconds.
     */
    uint32_t getStimHalfCycleStart();

    /**
     * @brief Gets the half-cycle end time.
     * @return Half-cycle end time in milliseconds.
     */
    uint32_t getStimHalfCycleEnd();

    /**
     * @brief Checks if the stimulation has been logged.
     * @return Boolean indicating logged state.
     */
    bool getStimLog();

    /**
     * @brief Checks if the laser is in the active cycle phase.
     * @return Boolean indicating cycle-up state.
     */
    bool getCycleUp();

    /**
     * @brief Gets the current stimulation mode.
     * @return MODE enum value (CYCLE or ACTIVE_PRESS).
     */
    MODE getStimMode();

    /**
     * @brief Gets the current stimulation state.
     * @return STATE enum value (ACTIVE or INACTIVE).
     */
    STATE getStimState();

    /**
     * @brief Gets the current laser action.
     * @return ACTION enum value (ON or OFF).
     */
    ACTION getStimAction();

    // Laser control
    /**
     * @brief Turns the laser on.
     */
    void on();

    /**
     * @brief Turns the laser off.
     */
    void off();
};

#endif // LASER_H