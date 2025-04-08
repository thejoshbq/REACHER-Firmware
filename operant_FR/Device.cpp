#include "Device.h"
#include <Arduino.h>

/**
 * @brief Constructs a Device object with an initial pin and unarmed state.
 * 
 * Initializes a Device object with a specified digital pin and sets its armed
 * state to false by default.
 * 
 * @param initPin The digital pin (byte) to which the device is connected.
 */
Device::Device(byte initPin) : pin(initPin), armed(false) {}

/**
 * @brief Arms the device and logs the action.
 * 
 * Sets the armed state to true, enabling the device to perform its intended function.
 */
void Device::arm() {
    armed = true;
    Serial.print("DEVICE ARMED AT PIN: ");
    Serial.println(pin);
}

/**
 * @brief Disarms the device and logs the action.
 * 
 * Sets the armed state to false, preventing the device from performing its function.
 */
void Device::disarm() {
    armed = false;
    Serial.print("DEVICE DISARMED AT PIN: ");
    Serial.println(pin);
}

/**
 * @brief Retrieves the pin assigned to the device.
 * 
 * @return The digital pin number (byte) associated with the device.
 */
byte Device::getPin() const {
    return pin;
}

/**
 * @brief Checks if the device is armed.
 * 
 * @return Boolean indicating whether the device is currently armed.
 */
bool Device::isArmed() const {
    return armed;
}