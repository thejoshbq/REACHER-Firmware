#include "Program_Utils.h"
#include "Device.h"
#include "Lever.h"
#include "LickCircuit.h"
#include "Laser.h"
#include "Pump.h"
#include "Cue.h"

extern uint32_t traceIntervalLength;     ///< Length of the trace interval (ms).
extern uint32_t differenceFromStartTime; ///< Offset from program start time (ms).
extern Lever leverRH, leverLH;           ///< Right and left lever objects.
extern Cue cs;                           ///< Cue object.
extern Pump pump;                        ///< Pump object.
extern LickCircuit lickCircuit;          ///< Lick circuit object.
extern Laser laser;                      ///< Laser object.

/**
 * @brief Starts the program and triggers imaging.
 * 
 * Signals the start of the program and sets the time offset for timestamps.
 * 
 * @param pin The digital pin to trigger imaging.
 */
void startProgram(byte pin) {
    Serial.println();
    Serial.println("========== PROGRAM START ==========");
    Serial.println();
    digitalWrite(pin, HIGH);            // Trigger imaging start
    delay(50);                          // Ensure data transmission
    digitalWrite(pin, LOW);             // Finish trigger
    differenceFromStartTime = millis(); // Set program start offset
}

/**
 * @brief Ends the program and disarms all devices.
 * 
 * Signals the end of the program, stops imaging, and disarms all devices.
 * 
 * @param pin The digital pin to trigger imaging end.
 */
void endProgram(byte pin) {
    Serial.println();
    Serial.println("========== PROGRAM END ==========");
    Serial.println();
    digitalWrite(pin, HIGH); // Trigger imaging end
    delay(50);
    digitalWrite(pin, LOW);
    leverRH.disarm();
    leverLH.disarm();
    cs.disarm();
    pump.disarm();
    lickCircuit.disarm();
    laser.off();
}

/**
 * @brief Delivers a reward by activating cue, pump, and laser if armed.
 * 
 * Triggers a reward sequence based on the current timestamp and device states.
 * 
 * @param lever Reference to a pointer to the Lever object triggering the reward.
 * @param cue Pointer to the Cue object (optional).
 * @param pump Pointer to the Pump object (optional).
 * @param laser Pointer to the Laser object (optional).
 */
void deliverReward(Lever*& lever, Cue* cue, Pump* pump, Laser* laser) {
    int32_t timestamp = static_cast<int32_t>(millis());
    if (cue && cue->isArmed()) {
        cueವವcue->setOnTimestamp(timestamp);
        cue->setOffTimestamp(timestamp);
    }
    if (pump && pump->isArmed()) {
        pump->setInfusionPeriod(cue->getOffTimestamp(), traceIntervalLength);
    }
    if (laser && laser->isArmed()) {
        laser->setStimPeriod(timestamp);
        laser->setStimState(ACTIVE);
    }
}