#include "Lever.h"
#include "Cue.h"
#include "Cue_Utils.h"
#include "Pump.h"
#include "Pump_Utils.h"
#include "Laser.h"
#include "Program_Utils.h"
#include <Arduino.h>

extern uint32_t timeoutIntervalStart;       ///< Start time of the timeout interval (ms).
extern uint32_t timeoutIntervalEnd;         ///< End time of the timeout interval (ms).
extern uint32_t timeoutIntervalLength;      ///< Length of the timeout interval (ms).
extern int32_t pressCount;                  ///< Counter for lever presses.
extern int32_t fRatio;                      ///< Fixed ratio for reward delivery.
extern uint32_t differenceFromStartTime;    ///< Offset from program start time (ms).
extern Lever* activeLever;                  ///< Pointer to the active lever.
extern Lever* inactiveLever;                ///< Pointer to the inactive lever.

/**
 * @brief Logs lever press and release data to the serial monitor.
 * 
 * Records the press and release timestamps of a lever, adjusted by the program start time if applicable.
 * 
 * @param lever Reference to a pointer to the Lever object being monitored.
 * @param pump Pointer to the Pump object (optional, can be nullptr).
 */
void pressingDataEntry(Lever*& lever, Pump* pump) {
    String pressEntry;
    lever->setReleaseTimestamp(millis()); // Set press release timestamp
    pressEntry = lever->getOrientation() + "_LEVER,";
    pressEntry += lever->getPressType() + "_PRESS,";
    if (differenceFromStartTime) {
        pressEntry += String(lever->getPressTimestamp() - differenceFromStartTime) + ",";
        pressEntry += String(lever->getReleaseTimestamp() - differenceFromStartTime);
    } else {
        pressEntry += String(lever->getPressTimestamp()) + ",";
        pressEntry += String(lever->getReleaseTimestamp());
    }
    Serial.println(pressEntry); // Send data to serial connection
}

/**
 * @brief Defines the type of lever press and triggers associated actions.
 * 
 * Determines whether a lever press is "ACTIVE", "TIMEOUT", or "INACTIVE" based on timing and device states,
 * and triggers rewards or updates counters accordingly.
 * 
 * @param programRunning Boolean indicating if the program is running.
 * @param lever Reference to a pointer to the Lever object being pressed.
 * @param cue Pointer to the Cue object (optional, can be nullptr).
 * @param pump Pointer to the Pump object (optional, can be nullptr).
 * @param laser Pointer to the Laser object (optional, can be nullptr).
 */
void definePressActivity(bool programRunning, Lever*& lever, Cue* cue, Pump* pump, Laser* laser) {
    int32_t timestamp = static_cast<int32_t>(millis()); // Capture initial timestamp
    if ((cue && cue->isArmed()) && (!pump || !pump->isArmed())) {
        if (timestamp >= cue->getOnTimestamp() && timestamp <= cue->getOffTimestamp() ||
            timestamp >= timeoutIntervalStart && timestamp <= timeoutIntervalEnd) {
            lever->setPressType("TIMEOUT");
        } else {
            lever->setPressType("ACTIVE");
            if (pressCount == fRatio - 1) {
                pressCount = 0;
                deliverReward(activeLever, cue, pump, laser);
                if (programRunning) {
                    timeoutIntervalStart = cue->getOffTimestamp();
                    timeoutIntervalEnd = timeoutIntervalStart + timeoutIntervalLength;
                }
            } else {
                pressCount++;
            }
        }
    } else if ((cue && cue->isArmed()) && (pump && pump->isArmed())) {
        if (timestamp >= cue->getOnTimestamp() && timestamp <= pump->getInfusionEndTimestamp() ||
            timestamp >= timeoutIntervalStart && timestamp <= timeoutIntervalEnd) {
            lever->setPressType("TIMEOUT");
        } else {
            lever->setPressType("ACTIVE");
            if (pressCount == fRatio - 1) {
                pressCount = 0;
                deliverReward(activeLever, cue, pump, laser);
                String infusionEntry = "PUMP,INFUSION,";
                infusionEntry += differenceFromStartTime ? String(pump->getInfusionStartTimestamp() - differenceFromStartTime) : String(pump->getInfusionStartTimestamp());
                infusionEntry += ",";
                infusionEntry += differenceFromStartTime ? String(pump->getInfusionEndTimestamp() - differenceFromStartTime) : String(pump->getInfusionEndTimestamp());
                Serial.println(infusionEntry);
                if (programRunning) {
                    timeoutIntervalStart = cue->getOffTimestamp();
                    timeoutIntervalEnd = timeoutIntervalStart + timeoutIntervalLength;
                }
            } else {
                pressCount++;
            }
        }
    } else {
        lever->setPressType("INACTIVE");
    }
}

/**
 * @brief Monitors lever pressing with debouncing and triggers associated actions.
 * 
 * Continuously checks the lever state, applies debouncing, and handles press/release events.
 * 
 * @param programRunning Boolean indicating if the program is running.
 * @param lever Reference to a pointer to the Lever object being monitored.
 * @param cue Pointer to the Cue object (optional, can be nullptr).
 * @param pump Pointer to the Pump object (optional, can be nullptr).
 * @param laser Pointer to the Laser object (optional, can be nullptr).
 */
void monitorPressing(bool programRunning, Lever*& lever, Cue* cue, Pump* pump, Laser* laser) {
    static uint32_t lastDebounceTime = 0; // Last time the lever input was toggled
    const uint32_t debounceDelay = 100;   // Debounce time in milliseconds
    manageCue(cue);                       // Manage cue delivery
    managePump(pump);                     // Manage infusion delivery
    if (lever->isArmed()) {
        bool currentLeverState = digitalRead(lever->getPin()); // Read current state
        if (currentLeverState != lever->getPreviousLeverState()) {
            lastDebounceTime = millis(); // Reset debouncing timer
        }
        if ((millis() - lastDebounceTime) > debounceDelay) {
            if (currentLeverState != lever->getStableLeverState()) {
                lever->setStableLeverState(currentLeverState); // Update stable state
                if (currentLeverState == LOW) { // Lever press detected
                    lever->setPressTimestamp(millis());
                    definePressActivity(programRunning, lever, cue, pump, laser);
                } else { // Lever release detected
                    lever->setReleaseTimestamp(millis());
                    pressingDataEntry(lever, pump);
                }
            }
        }
        lever->setPreviousLeverState(currentLeverState); // Update previous state
    }
}