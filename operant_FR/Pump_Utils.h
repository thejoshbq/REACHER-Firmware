#ifndef PUMP_UTILS_H
#define PUMP_UTILS_H

#include <Arduino.h>
#include "Pump.h"

/**
 * @file Pump_Utils.h
 * @brief Utility function for managing pump operation.
 */

/**
 * @brief Manages pump on/off state based on infusion period.
 * @param pump Pointer to the Pump object (optional).
 */
void managePump(Pump* pump);

#endif // PUMP_UTILS_H