#ifndef LICKCIRCUIT_UTILS_H
#define LICKCIRCUIT_UTILS_H

#include <Arduino.h>
#include "LickCircuit.h"

/**
 * @file LickCircuit_Utils.h
 * @brief Utility function for monitoring licking behavior.
 */

/**
 * @brief Monitors licking events on a lick circuit.
 * 
 * @param lickSpout Reference to the LickCircuit object to monitor.
 */
void monitorLicking(LickCircuit& lickSpout);

#endif // LICKCIRCUIT_UTILS_H