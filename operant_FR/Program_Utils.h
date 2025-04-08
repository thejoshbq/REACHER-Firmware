#ifndef PROGRAM_UTILS_H
#define PROGRAM_UTILS_H

#include <Arduino.h>
#include "Device.h"
#include "Lever.h"
#include "Laser.h"
#include "Pump.h"
#include "Cue.h"

/**
 * @file Program_Utils.h
 * @brief Utility functions for program control and reward delivery.
 */

/**
 * @brief Starts the program and triggers imaging.
 * @param pin The digital pin to trigger imaging.
 */
void startProgram(byte pin);

/**
 * @brief Ends the program and disarms devices.
 * @param pin The digital pin to trigger imaging end.
 */
void endProgram(byte pin);

/**
 * @brief Delivers a reward by activating devices.
 * @param lever Reference to a pointer to the Lever object.
 * @param cue Pointer to the Cue object (optional).
 * @param pump Pointer to the Pump object (optional).
 * @param laser Pointer to the Laser object (optional).
 */
void deliverReward(Lever*& lever, Cue* cue, Pump* pump, Laser* laser);

#endif // PROGRAM_UTILS_H