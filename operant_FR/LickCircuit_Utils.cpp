#include "LickCircuit.h"
#include <Arduino.h>

extern uint32_t differenceFromStartTime; ///< Offset from program start time (ms).

/**
 * @brief Monitors licking activity on a lick circuit with debouncing.
 * 
 * Detects lick touch and release events, applies debouncing, and logs timestamps
 * adjusted by the program start time when the lick circuit is armed.
 * 
 * @param lickSpout Reference to the LickCircuit object being monitored.
 */
void monitorLicking(LickCircuit& lickSpout) {
    static uint32_t lastDebounceTime = 0; // Last time the lick input was toggled
    const uint32_t debounceDelay = 25;    // Debounce time in milliseconds

    if (lickSpout.isArmed()) {
        bool currentLickState = digitalRead(lickSpout.getPin()); // Read current state
        if (currentLickState != lickSpout.getPreviousLickState()) {
            lastDebounceTime = millis(); // Reset debouncing timer
        }
        if ((millis() - lastDebounceTime) > debounceDelay) {
            if (currentLickState != lickSpout.getStableLickState()) {
                lickSpout.setStableLickState(currentLickState); // Update stable state
                if (currentLickState == HIGH) { // Lick touch detected
                    lickSpout.setLickTouchTimestamp(millis());
                } else { // Lick release detected
                    lickSpout.setLickReleaseTimestamp(millis());
                    String lickEntry = "LICK_CIRCUIT,LICK," +
                                       String(lickSpout.getLickTouchTimestamp() - differenceFromStartTime) + "," +
                                       String(lickSpout.getLickReleaseTimestamp() - differenceFromStartTime);
                    Serial.println(lickEntry); // Log lick event
                }
            }
        }
        lickSpout.setPreviousLickState(currentLickState); // Update previous state
    }
}