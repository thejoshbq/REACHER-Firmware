#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

/**
 * @file Utils.h
 * @brief General utility functions for connectivity and frame handling.
 */

/**
 * @brief Sends a periodic ping via serial.
 * @param previousPing Reference to the last ping time (ms).
 * @param pingInterval Interval between pings (ms).
 */
void pingDevice(uint32_t& previousPing, const uint32_t pingInterval);

/**
 * @brief ISR for capturing frame signal timestamps.
 */
void frameSignalISR();

/**
 * @brief Logs frame signals when collection is enabled.
 */
void handleFrameSignal();

#endif // UTILS_H