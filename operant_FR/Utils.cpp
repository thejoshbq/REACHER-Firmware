#include "Utils.h"
#include <Arduino.h>

extern bool frameSignalReceived;         ///< Indicates if a frame signal was received.
extern bool collectFrames;               ///< Indicates if frame collection is active.
extern uint32_t frameSignalTimestamp;    ///< Timestamp of the frame signal (ms).
extern uint32_t differenceFromStartTime; ///< Offset from program start time (ms).

/**
 * @brief Sends a periodic ping to ensure serial connection.
 * 
 * Sends a "200" byte at regular intervals to verify connectivity.
 * 
 * @param previousPing Reference to the last ping time (ms).
 * @param pingInterval Interval between pings (ms).
 */
void pingDevice(uint32_t& previousPing, const uint32_t pingInterval) {
    uint32_t currentMillis = millis();
    if (currentMillis - previousPing >= pingInterval) {
        previousPing = currentMillis;
        byte ping = 200;
        Serial.println(ping);
    }
}

/**
 * @brief Interrupt service routine for frame signal detection.
 * 
 * Captures the timestamp of a frame signal, adjusted by the program start time.
 */
void frameSignalISR() {
    frameSignalReceived = true;
    frameSignalTimestamp = millis() - differenceFromStartTime;
}

/**
 * @brief Handles frame signal logging when collection is active.
 * 
 * Logs the frame timestamp to serial when a signal is received.
 */
void handleFrameSignal() {
    if (collectFrames) {
        if (frameSignalReceived) {
            noInterrupts(); // Disable interrupts for safe access
            frameSignalReceived = false;
            int32_t timestamp = frameSignalTimestamp;
            interrupts();   // Re-enable interrupts
            Serial.println("FRAME_TIMESTAMP," + String(timestamp));
        }
    }
}