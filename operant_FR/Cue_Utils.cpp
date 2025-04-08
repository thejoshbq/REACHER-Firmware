#include "Device.h"
#include "Cue.h"
#include <Arduino.h>

/**
 * @brief Plays a connection or disconnection jingle based on the connection status.
 * 
 * This function generates an audible feedback using tones to indicate whether a device
 * has been linked or unlinked from a GUI. The pitch increases for connection and decreases
 * for disconnection.
 * 
 * @param connected A String indicating the connection status ("LINK" or "UNLINK").
 * @param cue Reference to a Cue object controlling the speaker output pin.
 * @param linkedToGUI Reference to a boolean flag tracking GUI connection status.
 */
void connectionJingle(String connected, Cue& cue, bool& linkedToGUI) {
    if (connected == "LINK") {
        linkedToGUI = true;
        static int32_t pitch = 500; // Starting tone frequency in Hz
        for (int32_t i = 0; i < 3; i++) {
            tone(cue.getPin(), pitch, 100); // Play tone for 100ms
            delay(100);                     // Wait for tone duration
            noTone(cue.getPin());           // Stop tone
            pitch += 500;                   // Increment pitch by 500Hz
        }
        pitch = 500;                        // Reset pitch for next use
        Serial.println("LINKED");           // Log connection status
    } else if (connected == "UNLINK") {
        linkedToGUI = false;
        static int32_t pitch = 1500; // Starting tone frequency in Hz
        for (int32_t i = 0; i < 3; i++) {
            tone(cue.getPin(), pitch, 100); // Play tone for 100ms
            delay(100);                     // Wait for tone duration
            noTone(cue.getPin());           // Stop tone
            pitch -= 500;                   // Decrement pitch by 500Hz
        }
        pitch = 1500;                       // Reset pitch for next use
        Serial.println("UNLINKED");         // Log disconnection status
    }
}

/**
 * @brief Manages the operation of a cue tone based on timestamps.
 * 
 * Controls the cue speaker by turning it on or off depending on whether the current time
 * falls within the cue's designated on/off timestamp interval. The cue must be armed
 * for this function to take effect.
 * 
 * @param cue Pointer to a Cue object (optional, nullptr if unused).
 */
void manageCue(Cue* cue) {
    int32_t timestamp = static_cast<int32_t>(millis()); // Current time in milliseconds

    if (cue != nullptr) {
        if (cue->isArmed()) { // Check if cue is armed
            if (timestamp <= cue->getOffTimestamp() && timestamp >= cue->getOnTimestamp()) {
                cue->on();           // Activate cue speaker
                cue->setRunning(true); // Update running state
            } else {
                cue->off();          // Deactivate cue speaker
                cue->setRunning(false); // Update running state
            }
        }
    }
}