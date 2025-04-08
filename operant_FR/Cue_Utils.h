#ifndef CUE_UTILS_H
#define CUE_UTILS_H

#include <Arduino.h>
#include "Cue.h"

/**
 * @file CueUtils.h
 * @brief Utility functions for managing cue operations and connection feedback.
 */

/**
 * @brief Plays a connection or disconnection jingle.
 * 
 * @param connected Connection status string ("LINK" or "UNLINK").
 * @param cue Reference to the Cue object for tone output.
 * @param linkedToGUI Reference to the GUI connection status flag.
 */
void connectionJingle(String connected, Cue& cue, bool& linkedToGUI);

/**
 * @brief Manages the state of a cue based on time intervals.
 * 
 * @param cue Pointer to the Cue object to be managed (can be nullptr).
 */
void manageCue(Cue* cue);

#endif // CUE_UTILS_H