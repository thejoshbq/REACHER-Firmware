#include "Device.h"
#include "Laser.h"
#include <Arduino.h>

extern Laser laser;                          ///< External reference to the Laser object.
extern bool programIsRunning;                ///< External flag indicating if the program is running.
extern uint32_t differenceFromStartTime;     ///< External offset from the program start time (in milliseconds).

/**
 * @brief Controls the laser's on/off state based on stimulation settings.
 * 
 * Turns the laser on or off depending on its stimulation state and action.
 * 
 * @param laser Reference to the Laser object to manage.
 */
void manageLaser(Laser& laser) {
    if (laser.getStimState() == ACTIVE && laser.getStimAction() == ON) {
        laser.on();
    } else {
        laser.off();
    }
}

/**
 * @brief Logs the laser stimulation period to the serial monitor.
 * 
 * Records the start and end times of a stimulation period, adjusted by an optional offset,
 * and marks the event as logged.
 * 
 * @param laser Reference to the Laser object whose stimulation is being logged.
 */
void logStim(Laser& laser) {
    if (!laser.getStimLog()) {
        String log = "LASER,STIM,";
        if (differenceFromStartTime) {
            log += String(laser.getStimStart() - differenceFromStartTime) + ",";
            log += String(laser.getStimEnd() - differenceFromStartTime);
        } else {
            log += String(laser.getStimStart()) + ",";
            log += String(laser.getStimEnd());
        }
        Serial.println(log);
        laser.setStimLogged(true);
    }
}

/**
 * @brief Checks if the current time is within the laser's stimulation period.
 * 
 * @param currentMillis Current time in milliseconds.
 * @return Boolean indicating if the time falls within the stimulation window.
 */
bool inStimPeriod(uint32_t currentMillis) {
    return currentMillis > laser.getStimStart() && currentMillis < laser.getStimEnd();
}

/**
 * @brief Manages the laser's stimulation behavior based on time and frequency.
 * 
 * Updates the laser's stimulation state and action, oscillating the laser if a frequency
 * is set, or keeping it constant if frequency is 1 Hz.
 * 
 * @param laser Reference to the Laser object to stimulate.
 * @param currentMillis Current time in milliseconds.
 */
void stim(Laser& laser, uint32_t currentMillis) {
    if (inStimPeriod(currentMillis) && laser.getCycleUp()) {
        laser.setStimState(ACTIVE);
        laser.setStimLogged(false);
        if (laser.getFrequency() == 1) { // Constant stimulation
            laser.setStimAction(ON);
        } else { // Oscillate at specified frequency
            if (currentMillis > laser.getStimHalfCycleEnd()) {
                laser.setStimHalfCyclePeriod(currentMillis);
                laser.setStimAction(laser.getStimAction() == ON ? OFF : ON); // Toggle state
            }
        }
    } else {
        laser.setStimState(INACTIVE);
        laser.setStimAction(OFF);
        if (!laser.getStimLog()) {
            logStim(laser);
            laser.setStimLogged(true);
        }
    }
    manageLaser(laser);
}

/**
 * @brief Oversees the laser's stimulation mode and timing.
 * 
 * Manages the laser’s stimulation based on its mode (CYCLE or ACTIVE_PRESS),
 * updating periods and states if the device is armed and the program is running.
 * 
 * @param laser Reference to the Laser object to manage.
 */
void manageStim(Laser& laser) {
    if (laser.isArmed() && programIsRunning) {
        uint32_t currentMillis = static_cast<uint32_t>(millis());
        if (laser.getStimMode() == CYCLE) {
            if (laser.getStimStart() == 0 || currentMillis >= laser.getStimEnd()) {
                laser.setStimPeriod(currentMillis);
                laser.setCycleUp(!laser.getCycleUp());
            }
        } else if (laser.getStimMode() == ACTIVE_PRESS) {
            laser.setCycleUp(true);
        }
        stim(laser, currentMillis);
    }
}