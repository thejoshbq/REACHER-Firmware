#include "Pump.h"
#include <Arduino.h>

/**
 * @brief Controls pump operation based on infusion timestamps.
 * 
 * Turns the pump on during the infusion period and off otherwise, if armed.
 * 
 * @param pump Pointer to the Pump object (optional, can be nullptr).
 */
void managePump(Pump* pump) {
    int32_t timestamp = static_cast<int32_t>(millis());
    if (pump && pump->isArmed()) {
        if (timestamp <= pump->getInfusionEndTimestamp() && timestamp >= pump->getInfusionStartTimestamp()) {
            pump->on(); // Turn the pump on
            pump->setRunning(true);
        } else {
            pump->off(); // Turn the pump off
            pump->setRunning(false);
        }
    }
}