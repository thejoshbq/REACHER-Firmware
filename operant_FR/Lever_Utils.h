#ifndef LEVER_UTILS_H
#define LEVER_UTILS_H

#include "Lever.h"
#include "Cue.h"
#include "Cue_Utils.h"
#include "Pump.h"
#include "Pump_Utils.h"
#include "Laser.h"
#include "Program_Utils.h"
#include <Arduino.h>

/**
 * @file LeverUtils.h
 * @brief Utility functions for managing lever pressing behavior and data logging.
 */

/**
 * @brief Logs lever press and release events.
 * @param lever Reference to a pointer to the Lever object.
 * @param pump Pointer to the Pump object (optional).
 */
void pressingDataEntry(Lever*& lever, Pump* pump);

/**
 * @brief Defines the type of lever press and associated actions.
 * @param programRunning Boolean indicating if the program is running.
 * @param lever Reference to a pointer to the Lever object.
 * @param cue Pointer to the Cue object (optional).
 * @param pump Pointer to the Pump object (optional).
 * @param laser Pointer to the Laser object (optional).
 */
void definePressActivity(bool programRunning, Lever*& lever, Cue* cue, Pump* pump, Laser* laser);

/**
 * @brief Monitors lever pressing with debouncing.
 * @param programRunning Boolean indicating if the program is running.
 * @param lever Reference to a pointer to the Lever object.
 * @param cue Pointer to the Cue object (optional).
 * @param pump Pointer to the Pump object (optional).
 * @param laser Pointer to the Laser object (optional).
 */
void monitorPressing(bool programRunning, Lever*& lever, Cue* cue, Pump* pump, Laser* laser);

#endif // LEVER_UTILS_H